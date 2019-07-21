#include "LogAlarm.h"
#include <wx/dir.h>
#include <ctime>
#include "mysqldatabase.h"


CLogAlarm::CLogAlarm():m_updateTime(0),m_currentAlarmNum(0)
{
}


CLogAlarm::~CLogAlarm()
{
}


int CLogAlarm::cbSQLGetAlarmNum(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{	
	CLogAlarm *pT = static_cast<CLogAlarm*>(pIn);
	if(pT)
	{
		pT->m_alarmSubSystem[std::strtoul(argv[0],NULL,10)] = std::string("");
		pT->m_alarmContent[std::strtoul(argv[0],NULL,10)] = std::string("");
	}

	return 0;

}

int CLogAlarm::cbSQLGetsubsystem(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CLogAlarm *pT = static_cast<CLogAlarm*>(pIn);
	if(pT)
	{
		pT->m_alarmSubSystem[pT->m_currentAlarmNum] = std::string(argv[0]);
	}

	return 0;
}

int CLogAlarm::cbSQLGetAlarmContent(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CLogAlarm *pT = static_cast<CLogAlarm*>(pIn);
	if(pT)
	{
		pT->m_alarmContent[pT->m_currentAlarmNum] = std::string(argv[0]);
	}

	return 0;
}

void CLogAlarm::getAlarmType()
{

	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{
	
		
			char sqlQuery[256] = {'\0'};

			sprintf_s(sqlQuery,256,"SELECT alarmNum FROM alarmconstinfo WHERE radarType LIKE '%%%s%%'",GetPlug()->getRadarTypeStr().ToStdString().c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmNum,wxT("alarmNum"),this);
			
			for(auto item = m_alarmSubSystem.cbegin(); item != m_alarmSubSystem.cend(); item++)
			{
				m_currentAlarmNum = item->first;
				sprintf_s(sqlQuery,256,"SELECT subsystem FROM alarmconstinfo WHERE alarmNum = %d AND radarType LIKE '%%%s%%'",m_currentAlarmNum, GetPlug()->getRadarTypeStr().ToStdString().c_str());		
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetsubsystem,wxT("subsystem"),this);

			}

			for(auto item = m_alarmContent.cbegin(); item != m_alarmContent.cend(); item++)
			{
				m_currentAlarmNum = item->first;
				sprintf_s(sqlQuery,256,"SELECT cNameDes FROM alarmconstinfo WHERE alarmNum = %d AND radarType LIKE '%%%s%%'",m_currentAlarmNum, GetPlug()->getRadarTypeStr().ToStdString().c_str());		
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmContent,wxT("cNameDes"),this);

			}
		
	}
}

void CLogAlarm::getLatestFileName(std::map<UINT,wxString>& fileInfo,UINT* newTime)
{
	auto dirName = wxString(GetPlug()->getfileConfig().alarmLogDir);

	if(dirName.IsEmpty())
	{
		return;
	}

	wxDir fileDir(dirName);
	wxString fullFilePath;
	wxString fileName;

	if(!::wxDirExists(dirName))
	{
		return;
	}

	if ( !fileDir.IsOpened() )
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"报警文件监控目录异常");

	}
	else
	{

		bool flag = fileDir.GetFirst(&fileName, wxEmptyString, wxDIR_FILES );   //遍历文件夹，获取最新的报警日志文件
		while ( flag )
		{
			if((fileName.Find(wxString(wxT("RadarAlarm"))) != wxNOT_FOUND) || (fileName.Find(wxString(wxT("Z_A_DWRN_ALM"))) != wxNOT_FOUND))
			{
				fullFilePath = dirName + wxT("\\") + fileName;
				auto updateTime = GetPlug()->GetFileModificationTime(fullFilePath);
				if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
				{
					updateTime += 8*60*60;
				}

				if(m_updateTime < updateTime)
				{
					*newTime = updateTime;
					fileInfo[updateTime] = fileName;

					if(::wxDirExists(GetPlug()->m_uploadPath))
					{
						auto copyFilePath = GetPlug()->m_uploadPath + wxT("\\") + fileName;  //将文件复制到upload文件夹下用于FTP上传
						::wxCopyFile(fullFilePath,copyFilePath);
					}

					if(fileInfo.size() >= 30)
					{
						break;
					}
				}
			}

			flag = fileDir.GetNext(&fileName);
		}

	}

}

void CLogAlarm::getAlarmInfo(const RADARALARM& data)
{
	m_alarmInfo.push_back(data);
}

void CLogAlarm::getAlarmInfoEd2(const RADAR_ALARM_DATA& data)
{
	m_alarmInfoEd2.push_back(data);
}

bool CLogAlarm::parseAlarmInfo(UINT newTime)
{
	bool flag = false;
	getAlarmType();
	for(auto item = m_alarmInfo.cbegin();item != m_alarmInfo.cend(); item++)  //将报警信息写入数据库中
	{
		flag = writeToDataBase(*item);
		if(flag)
		{
			m_updateTime = newTime;
			GetPlug()->setalarmtime(m_updateTime);
		}

	}
	m_alarmInfo.clear();

	return true;
}

bool CLogAlarm::parseAlarmInfoEd2(UINT newTime)
{
	bool flag = false;
	getAlarmType();
	for(auto item = m_alarmInfoEd2.cbegin();item != m_alarmInfoEd2.cend(); item++)
	{
		flag = writeToDataBaseEd2(*item);
		if(flag)
		{
			m_updateTime = newTime;
			GetPlug()->setalarmtime(m_updateTime);
		}

	}
	m_alarmInfoEd2.clear();

	return true;
}

time_t CLogAlarm::getDateTime(const RADARALARM& alarmInfo)
{
	if((std::strlen(alarmInfo.STATUS_DATA) == 0) || (std::strlen(alarmInfo.STATUS_TIME) == 0))
	{
		return -1;
	}
	struct tm ctime;

	char yearStr[8] = {'\0'};
	char monthStr[8] = {'\0'};
	char dayStr[8] = {'\0'};
	char hourStr[8] = {'\0'};
	char minStr[8] = {'\0'};
	char secStr[8] = {'\0'};

	std::memcpy(yearStr,alarmInfo.STATUS_DATA + 0, 4);
	std::memcpy(monthStr,alarmInfo.STATUS_DATA + 4, 2);
	std::memcpy(dayStr,alarmInfo.STATUS_DATA + 6, 2);
	std::memcpy(hourStr,alarmInfo.STATUS_TIME + 0, 2);
	std::memcpy(minStr,alarmInfo.STATUS_TIME + 2, 2);
	std::memcpy(secStr,alarmInfo.STATUS_TIME + 4, 2);

	ctime.tm_year = std::atoi(yearStr) - 1900;
	ctime.tm_mon = std::atoi(monthStr) - 1;
	ctime.tm_mday = std::atoi(dayStr);
	ctime.tm_hour = std::stoi(hourStr);
	ctime.tm_min = std::atoi(minStr);
	ctime.tm_sec = std::atoi(secStr);

	return std::mktime(&ctime);
}

time_t CLogAlarm::getDateTimeEd2(const RADAR_ALARM_DATA& alarmInfo)
{
	if((std::strlen(alarmInfo.STATUS_DATA) == 0) || (std::strlen(alarmInfo.STATUS_TIME) == 0))
	{
		return -1;
	}
	struct tm ctime;

	char yearStr[8] = {'\0'};
	char monthStr[8] = {'\0'};
	char dayStr[8] = {'\0'};
	char hourStr[8] = {'\0'};
	char minStr[8] = {'\0'};
	char secStr[8] = {'\0'};

	std::memcpy(yearStr,alarmInfo.STATUS_DATA + 0, 4);
	std::memcpy(monthStr,alarmInfo.STATUS_DATA + 4, 2);
	std::memcpy(dayStr,alarmInfo.STATUS_DATA + 6, 2);
	std::memcpy(hourStr,alarmInfo.STATUS_TIME + 0, 2);
	std::memcpy(minStr,alarmInfo.STATUS_TIME + 2, 2);
	std::memcpy(secStr,alarmInfo.STATUS_TIME + 4, 2);

	ctime.tm_year = std::atoi(yearStr) - 1900;
	ctime.tm_mon = std::atoi(monthStr) - 1;
	ctime.tm_mday = std::atoi(dayStr);
	ctime.tm_hour = std::stoi(hourStr);
	ctime.tm_min = std::atoi(minStr);
	ctime.tm_sec = std::atoi(secStr);

	return std::mktime(&ctime);
}

bool CLogAlarm::writeToDataBase(const RADARALARM& alarmInfo)
{
	wxString sqlSentence;

	auto  pDataBase = GetPlug()->getDatabase();
	

	if (pDataBase)
	{

		if(m_alarmContent.find(alarmInfo.ALARM_NO) == m_alarmContent.end()) //如果报警编号没有在数据库有记录 则无法识别
		{
			return true;
		}

		auto time = getDateTime(alarmInfo);
		if(time <= 0)
		{
			return false;
		}

		if(std::memcmp(alarmInfo.MONITOR_ALARM_FLAG,"ALARM",std::strlen(alarmInfo.MONITOR_ALARM_FLAG)) == 0)
		{
			if(m_activeAlarmNum.find(alarmInfo.ALARM_NO) != m_activeAlarmNum.end())   //如果报警未消除，来了重复的报警编号则不再插入数据库
			{
				return true;
			}

			m_activeAlarmNum.insert(alarmInfo.ALARM_NO);  //发现新的报警编号，插入容器中，报警消除后再删除

			sqlSentence = wxT("INSERT INTO alarminfo (time, endTime, type, alarmName, level, mode, content, origin) VALUES");

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), time));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_alarmSubSystem[alarmInfo.ALARM_NO])));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString::Format(wxT("%d"),alarmInfo.ALARM_NO)));

			sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
			sqlSentence.Append(wxString::Format(wxT("'%s',"), wxT("0")));

			sqlSentence.Append(wxString::Format(wxT("'%s',"),wxString(m_alarmContent[alarmInfo.ALARM_NO])));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 2));

			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}

		}
		else if(std::memcmp(alarmInfo.MONITOR_ALARM_FLAG,"CLEAR",std::strlen(alarmInfo.MONITOR_ALARM_FLAG)) == 0)  //784发送0编号时将endTime写上数据
		{
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND alarmName= '%s'"),time,wxString::Format(wxT("%d"),alarmInfo.ALARM_NO));

			m_activeAlarmNum.erase(alarmInfo.ALARM_NO);

			bool ret =  pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}

		}
		else{}

		GetPlug()->AddLog(LOG_TYPE_MESSAGE,sqlSentence.ToStdString().c_str());
			
	}



	return true;

}

bool CLogAlarm::writeToDataBaseEd2(const RADAR_ALARM_DATA& alarmInfo)  //对应不用型号雷达的报警数据写入
{
	wxString sqlSentence;
	auto  pDataBase = GetPlug()->getDatabase();


	if (pDataBase)
	{

		auto time = getDateTimeEd2(alarmInfo);
		if(time <= 0)
		{
			return false;
		}

		if(alarmInfo.MONITOR_ALARM_FLAG == ALARMON)
		{

			if(alarmInfo.ALARM_NO == 0)  //784发送编号为0的报警时将endTime写入
			{
				sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND alarmName = '%s'"),time,wxString::Format(wxT("%d"),alarmInfo.ALARM_NO));
			}
			else
			{
				if(m_alarmContent.find(alarmInfo.ALARM_NO) == m_alarmContent.end())
				{
					return true;
				}

				if(m_activeAlarmNum.find(alarmInfo.ALARM_NO) != m_activeAlarmNum.end())
				{		
					return true;
				}

				m_activeAlarmNum.insert(alarmInfo.ALARM_NO);

				sqlSentence = wxT("INSERT INTO alarminfo (time, endTime, type, alarmName, level, mode, content, origin) VALUES");


				sqlSentence.Append(wxString::Format(wxT("(%I64d, "), time));
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_alarmSubSystem[alarmInfo.ALARM_NO])));
				sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString::Format(wxT("%d"),alarmInfo.ALARM_NO)));

				sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
				sqlSentence.Append(wxString::Format(wxT("'%s',"), wxT("0")));

				sqlSentence.Append(wxString::Format(wxT("'%s',"),wxString(m_alarmContent[alarmInfo.ALARM_NO])));
				sqlSentence.Append(wxString::Format(wxT("%d)"), 2));
			}


			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}

		}
		else if(alarmInfo.MONITOR_ALARM_FLAG == ALARMOFF)
		{
			if(alarmInfo.ALARM_NO == 0)  //784发送编号为0的报警插入报警信息，并把其他报警信息endTime写上
			{
				wxString sqlSentenceInsert = wxT("INSERT INTO alarminfo (time, endTime, type, alarmName, level, mode, content, origin) VALUES");

				sqlSentenceInsert.Append(wxString::Format(wxT("(%I64d, "), time));
				sqlSentenceInsert.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				sqlSentenceInsert.Append(wxString::Format(wxT("'%s', "), wxT("整机")));
				sqlSentenceInsert.Append(wxString::Format(wxT("'%s', "), wxString::Format(wxT("%d"),alarmInfo.ALARM_NO)));

				sqlSentenceInsert.Append(wxString::Format(wxT("%d, "), 2));
				sqlSentenceInsert.Append(wxString::Format(wxT("'%s',"), wxT("0")));

				sqlSentenceInsert.Append(wxString::Format(wxT("'%s',"),wxT("雷达正在检修或维护")));
				sqlSentenceInsert.Append(wxString::Format(wxT("%d)"), 2));

				bool ret =  pDataBase->RunSQLNoQuery(sqlSentenceInsert);
				if(!ret)
				{
					SHOWFUNCNAME();
				}

				sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND alarmName IS NOT NULL AND alarmName != '0'"),time);

			}
			else
			{
				sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND alarmName = '%s'"),time,wxString::Format(wxT("%d"),alarmInfo.ALARM_NO));
				m_activeAlarmNum.erase(alarmInfo.ALARM_NO);
			}


			bool ret =  pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}
		}
		else{}

		GetPlug()->AddLog(LOG_TYPE_MESSAGE,sqlSentence.ToStdString().c_str());
	}



	return true;
}

DWORD CLogAlarm::alarmFileMonitor(LPVOID para)
{
	CLogAlarm* self = static_cast<CLogAlarm*>(para);
	if(self)
	{
		UINT newTime = 0;
		std::map<UINT,wxString> alarmFileInfo;
		self->setUpdateTime(GetPlug()->getfileConfig().alarmtime);

		while(!GetPlug()->GetExit())
		{
			self->getLatestFileName(alarmFileInfo,&newTime);  //获取最新的报警信息
			if(!alarmFileInfo.empty())
			{
				for(auto item = alarmFileInfo.cbegin(); item != alarmFileInfo.cend(); item++)
				{
					auto fullPath = wxString(GetPlug()->getfileConfig().alarmLogDir);
					fullPath += wxT("\\");
					fullPath += item->second;

					GetPlug()->AddLog(LOG_TYPE_MESSAGE,item->second.ToStdString().c_str());

					wxFile file(fullPath);   //打开报警文件
					if(file.IsOpened())
					{
						char magicNum[5] = {0};
						file.Read((void*)&magicNum,4);
						file.Seek(0);

						if((std::strncmp(magicNum,"RSTM",4) == 0) && ((file.Length() - sizeof(RadarStructure::GeneHeader) - sizeof(RadarStructure::SiteInfo)) > 0))  //两种类型报警问价做判断
						{
							RADARALARM alarmData;

							file.Seek(sizeof(RadarStructure::GeneHeader) + sizeof(RadarStructure::SiteInfo));
							while(!file.Eof())
							{
								file.Read((void*)&alarmData,sizeof(RADARALARM));  //读取报警文件信息
								self->getAlarmInfo(alarmData);
							}

							file.Close();

							self->parseAlarmInfo(newTime);  //解析报警文件

						}
						else if(file.Length() % sizeof(RADAR_ALARM_DATA) == 0)
						{

							RADAR_ALARM_DATA alarmData;
							while(!file.Eof())
							{
								file.Read((void*)&alarmData,sizeof(RADAR_ALARM_DATA));
								self->getAlarmInfoEd2(alarmData);
							}

							file.Close();

							self->parseAlarmInfoEd2(newTime);
						}
						else{}

					}

				}
				alarmFileInfo.clear();
			

			}

			for(auto i= 0; i< 10; i++)
			{
				if(GetPlug()->GetExit())
				{
					break;
				}
				::wxSleep(1);
			}
			
		}


	}
	return TRUE;
}

	

