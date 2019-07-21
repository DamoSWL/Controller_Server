#include "SiteAlarm.h"
#include <ctime>
#include <memory>
#include "mysqldatabase.h"



CSiteAlarm::CSiteAlarm()
{
	m_SiteExpFlag["transmitFre"] = false;
	m_SiteExpFlag["hBeamWidth"] = false;
	m_SiteExpFlag["vBeamWidth"] = false;
	m_SiteExpFlag["antGain"] = false;
	m_SiteExpFlag["ltx"] = false;
	m_SiteExpFlag["lrx"] = false;
	m_SiteExpFlag["lx"] = false;

	m_alarmLevel["transmitFre"] = 0;
	m_alarmLevel["hBeamWidth"] = 0;
	m_alarmLevel["vBeamWidth"] = 0;
	m_alarmLevel["antGain"] = 0;
	m_alarmLevel["ltx"] = 0;
	m_alarmLevel["lrx"] = 0;
	m_alarmLevel["lx"] = 0;

	m_SiteContent["transmitFre"] = "工作频率";
	m_SiteContent["hBeamWidth"] = "水平波速宽度";
	m_SiteContent["vBeamWidth"] = "垂直波束宽度";
	m_SiteContent["antGain"] = "天线增益";
	m_SiteContent["ltx"] = "发射馈线损耗";
	m_SiteContent["lrx"] = "接收馈线损耗";
	m_SiteContent["lx"] = "其它损耗";

	m_alarmType["transmitFre"] = std::string("");
	m_alarmType["hBeamWidth"] = std::string("");
	m_alarmType["vBeamWidth"] = std::string("");
	m_alarmType["antGain"] = std::string("");
	m_alarmType["ltx"] = std::string("");
	m_alarmType["lrx"] = std::string("");
	m_alarmType["lx"] = std::string("");

	m_ThreadholdValues["transmitFre"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hBeamWidth"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vBeamWidth"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["antGain"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ltx"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["lrx"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["lx"] = std::make_pair(0.0,0.0);
}


CSiteAlarm::~CSiteAlarm()
{
}

void CSiteAlarm::getAlarmType()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {

			for(auto item = m_alarmType.cbegin(); item != m_alarmType.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256,"SELECT type FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmType,wxT("type"),this);	
			}
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		int length = 0;
			// 		char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
			// 		if(sqlTmpStr)
			// 		{
			// 			std::string eName(sqlTmpStr,strlen(sqlTmpStr));
			// 			sqlTmpStr = pDataBase->GetStringValue(index,"type",length);
			// 			if(sqlTmpStr)
			// 			{
			// 				std::string type(sqlTmpStr,strlen(sqlTmpStr));
			// 				m_alarmType[eName] = type;
			// 			}
			// 		}
			// 	}

			// 	pDataBase->FreeResult(sqlError);
			
			// }

		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }

	}
}

void CSiteAlarm::getAlarmLevel()
{
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{
		for(auto item = m_alarmLevel.cbegin(); item != m_alarmLevel.cend(); item++)
		{
			m_currenteName = item->first;
			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256,"SELECT level FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmLevel,wxT("level"),this);	
		}

	}
}

void CSiteAlarm::getAlarmCNname()
{
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{
		for(auto item = m_SiteContent.cbegin(); item != m_SiteContent.cend(); item++)
		{
			m_currenteName = item->first;
			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256,"SELECT cName FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmCNname,wxT("cName"),this);	
		}

	}
}

void CSiteAlarm::getThresholdValue()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			for(auto item = m_ThreadholdValues.cbegin(); item != m_ThreadholdValues.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256, "SELECT lowerLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetLowerValue,wxT("lowerLimit"),this);
	
	
				sprintf_s(sqlQuery,256, "SELECT upperLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetUpperValue,wxT("upperLimit"),this);
			}
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		int length = 0;
			// 		char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
			// 		{
			// 			if(sqlTmpStr)
			// 			{
			// 				std::string eName(sqlTmpStr,strlen(sqlTmpStr));
			// 				std::pair<FLOAT,FLOAT> pair;
			// 				pair.first = pDataBase->GetFloatValue(index,"lowerLimit");
			// 				pair.second = pDataBase->GetFloatValue(index,"upperLimit");
			// 				if((pair.first == 0) && (pair.second == 0))
			// 				{
			// 					continue;
			// 				}
			// 				m_ThreadholdValues[eName] = pair;
			// 			}
			// 		}
					
			// 	}
			// 	pDataBase->FreeResult(sqlError);
			// }

		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }

	}
}

void CSiteAlarm::getSiteInfoData(const RadarStructure::SiteInfo& siteInfo)
{
	m_SiteValues.clear();
	m_SiteValues["transmitFre"] = siteInfo.Frequency;
	m_SiteValues["hBeamWidth"] = siteInfo.BeamWidthHori;
	m_SiteValues["vBeamWidth"] = siteInfo.BeamWidthHori;
	m_SiteValues["antGain"] = siteInfo.AntennaGain * 0.01;
	m_SiteValues["ltx"] = siteInfo.TransmittingFeederLoss * 0.01;
	m_SiteValues["lrx"] = siteInfo.ReceivingFeederLoss * 0.01;
	m_SiteValues["lx"] = siteInfo.OtherLoss * 0.01;

	
}

void CSiteAlarm::siteInfosCheck()
{
	for(auto item = m_SiteValues.cbegin(); item != m_SiteValues.cend(); item++)
	{
		auto name = item->first;
		
		if(m_ThreadholdValues.find(name) != m_ThreadholdValues.cend())
		{
			if(m_alarmLevel.find(name) == m_alarmLevel.cend())
			{
				break;
			}

			if(m_alarmLevel[name] < 0)
			{
				break;
			}
			auto value = m_SiteValues[name];
			auto lowerLimit = m_ThreadholdValues[name].first;
			auto upperLimit = m_ThreadholdValues[name].second;

			if((std::fabs(lowerLimit) < ZEROBIAS) && (std::fabs(upperLimit) < ZEROBIAS))
			{
				continue;
			}
			

			if(m_SiteExpFlag[name])
			{
				if((value <= upperLimit) && (value >= lowerLimit))
				{
					m_SiteExpFlag[name] = false;
					m_SiteEndureTime[name].second = ::wxDateTime::GetTimeNow();
					writeToDataBasewithEndTime(name);
				}

			}
			else
			{
				if(value > upperLimit)
				{
					m_SiteExpFlag[name] = true;
					m_SiteEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SiteConcreteContent[name] = m_SiteContent[name];
					m_SiteConcreteContent[name] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),m_SiteValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);

				}
				else if(value < lowerLimit)
				{
					m_SiteExpFlag[name] = true;
					m_SiteEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SiteConcreteContent[name] = m_SiteContent[name];
					m_SiteConcreteContent[name] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),m_SiteValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);
				}
			}

		}

	}
}

bool CSiteAlarm::writeToDataBase(const std::string name)
{
	wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
																	endTime, \
																	type, \
																	alarmName, \
																	level, \
																	mode, \
																	content, \
																	origin) VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_SiteEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_alarmType[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_alarmLevel[name]));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_SiteConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));



			return pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

bool CSiteAlarm::writeToDataBasewithEndTime(const std::string name)
{

	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			wxString sqlSentence; 
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_SiteEndureTime[name].second,
																															m_SiteEndureTime[name].first,
																															wxString(m_SiteConcreteContent[name].c_str(),wxConvLocal));


			return pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

int CSiteAlarm::cbSQLGetAlarmType(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSiteAlarm *pT = static_cast<CSiteAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmType(std::string(argv[0]));
	}

	return 0;
}

int CSiteAlarm::cbSQLGetLowerValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSiteAlarm *pT = static_cast<CSiteAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetLowerValue(std::string(argv[0]));
	}

	return 0;
}

int CSiteAlarm::cbSQLGetUpperValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSiteAlarm *pT = static_cast<CSiteAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetUpperValue(std::string(argv[0]));
	}

	return 0;
}

int CSiteAlarm::cbSQLGetAlarmCNname(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSiteAlarm *pT = static_cast<CSiteAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmCNname(std::string(argv[0]));
	}

	return 0;
}

int CSiteAlarm::cbSQLGetAlarmLevel(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSiteAlarm *pT = static_cast<CSiteAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmLevel(std::string(argv[0]));
	}

	return 0;
}
