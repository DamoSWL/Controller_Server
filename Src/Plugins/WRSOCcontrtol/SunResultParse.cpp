
#include "SunResultParse.h"
#include <wx/dir.h>
#include <fstream>
#include <sstream>


CSunResultParse::CSunResultParse():m_oldAzimuthConnectionFactor(0),
	m_newAzimuthConnectionFactor(0),
	m_peakPowrFromAzimuthScan(0),
	m_oldElevationConnectionFactor(0),
	m_newElevationConnectionFactor(0),
	m_peakPowerFromElevationScan(0),
	m_meanReceiverNoise(0),
	m_beamWidth(0),
	m_beamWidth_V(0),
	m_updateTime(0),
	m_tmpUpdateTime(0),
	m_produceTime(0),
	m_meanReceiverNoiseFlag(FALSE),
	m_beamWidth_VFlag(FALSE)
{
}


CSunResultParse::~CSunResultParse()
{
}

INT CSunResultParse::loadTXTFile(time_t* time)
{
	auto wFileName = getLatestFileName(time);
	if(wFileName.IsEmpty())
	{
		return EXIT_FAILURE;
	}

	GetPlug()->AddLog(LOG_TYPE_MESSAGE,wFileName.ToStdString().c_str());

	m_meanReceiverNoiseFlag = FALSE;
	m_beamWidth_VFlag = FALSE;

	std::ifstream file(wFileName.ToStdString().c_str());
	if(file.is_open())
	{
		while(!file.eof())
		{
			std::string content;
			std::getline(file,content,'\n');

			if(content.empty())
			{
				continue;
			}
			else
			{
				m_contents = content;
				parseFile();
			}
		}

		file.close();
		return EXIT_SUCCESS;
	}
	file.close();
	return EXIT_FAILURE;

	//wxFile file(wFileName,wxFile::read);
	//if(file.IsOpened())
	//{
	//	auto length = file.Length();
	//	char *buffer = new char[length + 1];
	//	std::memset(buffer,'\0',length + 1);
	//	file.Read(buffer,length);
	//	m_contents.assign(buffer);
	//	delete[] buffer;
	//}
	//else
	//{
	//	file.Close();
	//	return EXIT_FAILURE;
	//}
	//file.Close();
	//return EXIT_SUCCESS;

}

void CSunResultParse::parseFile()
{
	if(m_contents.find("begin") != std::string::npos)
	{
		std::istringstream dateTimeStream(m_contents);

		if((GetPlug()->m_radarType == wxT("SC")) || (GetPlug()->m_radarType == wxT("CD")) || (GetPlug()->m_radarType == wxT("CC")))
		{
			std::string dateTimeStr;
			std::string str1;
			std::string str2;

			if(dateTimeStream >> dateTimeStr >> str1 >> str2)
			{
				m_produceTime = getDateTime(dateTimeStr);
			}
		}
		else if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
		{
			std::string dateStr;
			std::string dayStr;
			std::string timeStr;
			std::string str1;
			std::string str2;

			if(dateTimeStream >> dateStr >> dayStr >> timeStr >> str1 >> str2)
			{
				m_produceTime = getDateTime2(dateStr,timeStr);
			}
		}
		else{}

		updateTime(m_tmpUpdateTime);
	}

	INT pos = 0;

	if(m_contents.find("OLD AZIMUTH CORRECTION FACTOR") != std::string::npos)
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr = m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_oldAzimuthConnectionFactor = (FLOAT)value;
		
	}


	if(m_contents.find("NEW AZIMUTH CORRECTION FACTOR") != std::string::npos)
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_newAzimuthConnectionFactor = (FLOAT)value;
	}

	if(m_contents.find("PEAK POWER MEASURED FROM AZIMUTH SCAN") != std::string::npos)
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_peakPowrFromAzimuthScan = (FLOAT)value;
	}

	if(m_contents.find("OLD ELEVATION CORRECTION FACTOR") != std::string::npos)
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_oldElevationConnectionFactor = (FLOAT)value;
	}


	if(m_contents.find("NEW ELEVATION CORRECTION FACTOR") != std::string::npos)
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_newElevationConnectionFactor = (FLOAT)value;
	}

	if(m_contents.find("PEAK POWER MEASURED FROM ELEVATION SCAN") != std::string::npos)
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_peakPowerFromElevationScan = (FLOAT)value;
	}

	
	if(m_contents.find("MEASURED MEAN RECEIVER NOISE LEVEL") != std::string::npos)
	{
		m_meanReceiverNoiseFlag = TRUE;
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_meanReceiverNoise = (FLOAT)value;
	}

	if((m_contents.find("COMPUTED BEAMWIDTH ESTIMATE") != std::string::npos) || (m_contents.find("COMPUTED BEAMWIDTH-H ESTIMATE") != std::string::npos))
	{
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_beamWidth = (FLOAT)value;
	}

	if((m_contents.find("COMPUTED BEAMWIDTH-V ESTIMATE") != std::string::npos))
	{
		m_beamWidth_VFlag = TRUE;
		pos = m_contents.find_first_of(':');
		pos++;
		wxString valueStr =m_contents.substr(pos , m_contents.length() - pos);
		double value = 0.0;
		valueStr.ToDouble(&value);
		m_beamWidth_V = (FLOAT)value;
	}

	if(m_contents.find("END OF SUNCHECK MEASURMENT") != std::string::npos)
	{
		if(writeToDataBase(m_produceTime))
		{
			updateTime(m_tmpUpdateTime);
		}
	}

	if(m_contents.find("INVALID") != std::string::npos)
	{
		updateTime(m_tmpUpdateTime);
	}
	
}

bool CSunResultParse::checkFileTime(time_t* time)
{
	/*struct tm ctime;

	std::string::size_type pos = m_contents.find_first_of('-');
	std::string yearStr = m_contents.substr(0, pos);
	ctime.tm_year = std::stoi(yearStr) - 1900;

	pos++;
	std::string::size_type previousPos = pos;
	pos = m_contents.find_first_of('-', previousPos);
	std::string monthStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_mon = std::stoi(monthStr) - 1;


	pos++;
	previousPos = pos;
	pos = m_contents.find_first_of(' ', previousPos);
	std::string dayStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_mday = std::stoi(dayStr);

	pos = m_contents.find_first_of(':', previousPos);
	previousPos = pos - 2;
	std::string hourStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_hour = std::stoi(hourStr);

	pos++;
	previousPos = pos;
	pos = m_contents.find_first_of(':',previousPos);
	std::string minStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_min = std::stoi(minStr);

	pos++;
	previousPos = pos;
	pos = m_contents.find_first_of(' ', previousPos);
	std::string secStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_sec = std::stoi(secStr);

	*time = std::mktime(&ctime);

	if (*time != m_updateTime)
	{
	return true;
	}

	return false;*/
	return true;

}

void CSunResultParse::updateTime(time_t time)
{
	m_updateTime = time;
	GetPlug()->setSunResulttime(m_updateTime);
}

wxString CSunResultParse::getLatestFileName(time_t* time)
{
	wxString dirName(GetPlug()->getfileConfig().SunResultdir);
	if(dirName.IsEmpty())
	{
		return wxEmptyString;
	}
	if(!::wxDirExists(dirName))
	{
		return wxEmptyString;
	}
	wxDir fileDir(dirName);
	wxString fullFilePath;

	if ( !fileDir.IsOpened() )
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"太阳法测试结果文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName,wxString(wxT("*_SunResult_*.txt")), wxDIR_FILES );
		while( flag )
		{
			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			auto updateTime = GetPlug()->fileModificationTime(fullFilePath);
			if(m_updateTime < updateTime)
			{
				*time = updateTime;
				m_tmpUpdateTime = updateTime;
				newFileFlag = true;
				break;
			}
			flag = fileDir.GetNext(&fileName);
		}

		if(!newFileFlag)
		{
			fullFilePath.clear();
		}
		else
		{
			if(::wxDirExists(GetPlug()->m_uploadPath))
			{
				auto copyFilePath = GetPlug()->m_uploadPath + wxT("\\") + fileName;
				::wxCopyFile(fullFilePath,copyFilePath);
			}
		}
	}
	return fullFilePath;
}

time_t CSunResultParse::getDateTime(const std::string& dateTimeStr)
{

	if(dateTimeStr.empty())
	{
		return -1;
	}

	wxString TimeStr(dateTimeStr);

	auto yearStr = TimeStr.Left(4);
	auto monthStr = TimeStr.Mid(4,2);
	auto dayStr = TimeStr.Mid(6,2);
	auto hourStr = TimeStr.Mid(8,2);
	auto minStr = TimeStr.Mid(10,2);
	auto secStr= TimeStr.Mid(12,2);

	ULONG year = 0;
	ULONG month = 0;
	ULONG day = 0;
	ULONG hour = 0;
	ULONG min = 0;
	ULONG sec = 0;

	yearStr.ToULong(&year);
	monthStr.ToULong(&month);
	dayStr.ToULong(&day);
	hourStr.ToULong(&hour);
	minStr.ToULong(&min);
	secStr.ToULong(&sec);

	struct tm fileTime;
	fileTime.tm_year = year - 1900;
	fileTime.tm_mon = month - 1;
	fileTime.tm_mday = day;
	fileTime.tm_hour = hour;
	fileTime.tm_min = min;
	fileTime.tm_sec = sec;

	return std::mktime(&fileTime);	

	/*std::string::size_type pos = date.find_first_of('-');
	std::string yearStr = date.substr(0, pos);
	ctime.tm_year = std::stoi(yearStr) - 1900;

	pos++;
	std::string::size_type previousPos = pos;
	pos = date.find_first_of('-', previousPos);
	std::string monthStr = date.substr(previousPos, pos - previousPos);
	ctime.tm_mon = std::stoi(monthStr) - 1;

	pos++;
	previousPos = pos;
	std::string dayStr = date.substr(previousPos, date.length() - previousPos);
	ctime.tm_mday = std::stoi(dayStr);

	pos = time.find_first_of(':');
	std::string hourStr = time.substr(0, pos);
	ctime.tm_hour = std::stoi(hourStr);

	pos++;
	previousPos = pos;
	pos = time.find_first_of(':', previousPos);
	std::string minStr = time.substr(previousPos, pos - previousPos);
	ctime.tm_min = std::stoi(minStr);

	pos++;
	previousPos = pos;
	std::string secStr = time.substr(previousPos, time.length() - previousPos);
	ctime.tm_sec = std::stoi(secStr);


	return static_cast<INT>(std::mktime(&ctime));*/
}

time_t CSunResultParse::getDateTime2(const std::string& dateStr,const std::string& timeStr)
{
	struct tm ctime;
	
	std::string::size_type pos = dateStr.find_first_of('-');
	std::string yearStr = dateStr.substr(0, pos);
	ctime.tm_year = std::stoi(yearStr) - 1900;

	pos++;
	std::string::size_type previousPos = pos;
	pos = dateStr.find_first_of('-', previousPos);
	std::string monthStr = dateStr.substr(previousPos, pos - previousPos);
	ctime.tm_mon = std::stoi(monthStr) - 1;

	pos++;
	previousPos = pos;
	std::string dayStr = dateStr.substr(previousPos, dateStr.length() - previousPos);
	ctime.tm_mday = std::stoi(dayStr);

	pos = timeStr.find_first_of(':');
	std::string hourStr = timeStr.substr(0, pos);
	ctime.tm_hour = std::stoi(hourStr);

	pos++;
	previousPos = pos;
	pos = timeStr.find_first_of(':', previousPos);
	std::string minStr = timeStr.substr(previousPos, pos - previousPos);
	ctime.tm_min = std::stoi(minStr);

	pos++;
	previousPos = pos;
	std::string secStr = timeStr.substr(previousPos, timeStr.length() - previousPos);
	ctime.tm_sec = std::stoi(secStr);


	return std::mktime(&ctime);
}

bool CSunResultParse::writeToDataBase(time_t time)
{
	wxString sqlSentence = wxT("INSERT INTO sunresult(updateTime, \
																			oldACF,\
																			newACF, \
																			ASPT, \
																			oldECF, \
																			newECF, \
																			ESPT, \
																			receiverNoise, \
																			waveWidth, \
																			waveWidth_v) VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "),time));
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_oldAzimuthConnectionFactor));
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_newAzimuthConnectionFactor));
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_peakPowrFromAzimuthScan));
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_oldElevationConnectionFactor));
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_newElevationConnectionFactor));
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_peakPowerFromElevationScan));
			if(m_meanReceiverNoiseFlag)
			{
				sqlSentence.Append(wxString::Format(wxT("%f,"),m_meanReceiverNoise));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s,"),wxT("NULL")));
			}
			
			sqlSentence.Append(wxString::Format(wxT("%f, "),m_beamWidth));

			if(m_beamWidth_VFlag)
			{
				sqlSentence.Append(wxString::Format(wxT("%f)"),m_beamWidth_V));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s)"),wxT("NULL")));
			}


			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}

		// }
		// catch (wxString& error)
		// {
		// 	if(error.Find(wxT("Duplicate entry")) != wxNOT_FOUND)
		// 	{
		// 		return TRUE;
		// 	}
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}

	return true;
}
