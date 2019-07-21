#include "AdaptChg.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <wx/dir.h>



CAdaptChg::CAdaptChg():m_updateTime(0),
	m_previousReadSize(0)
{

}


CAdaptChg::~CAdaptChg(void)
{
}

INT CAdaptChg::loadTXTFile(time_t* time)
{
	m_fileName = getLatestFileName(time);
	if(m_fileName.IsEmpty())
	{
		return EXIT_SUCCESS;
	}

	std::ifstream doc;
	doc.open(std::string(m_fileName.ToStdString().c_str()));
	if (doc.is_open())
	{
		doc.seekg(m_previousReadSize,std::ios_base::beg);

		while (!doc.eof())
		{
			std::string item;
			std::getline(doc, item);
			m_contents.push_back(item);
		}
	}
	else
	{
		doc.close();
		return EXIT_FAILURE;
	}

	doc.close();
	return EXIT_SUCCESS;

}

void CAdaptChg::parseFile()
{

	std::string dateStr;
	std::string timeStr;
	std::string previousName;
	std::string currentName;

	for(auto item = m_contents.cbegin(); item != m_contents.cend(); item++)
	{
		if(item->empty())
		{
			continue;
		}
		std::istringstream adaptStream(*item);
		adaptStream >> dateStr >> timeStr >> previousName >> currentName;

		auto time = getRecordTime(dateStr,timeStr);
		std::pair<std::string,std::string> parameterPair = std::make_pair(previousName,currentName);
		m_alterRecord[time] = parameterPair;	
	}

}

bool CAdaptChg::checkFileTime(time_t* time)
{
	if(!m_fileName.IsEmpty())
	{
		*time = ::wxFileModificationTime(m_fileName);
		return true;
	}

	return false;

}

bool CAdaptChg::writeToDataBase(time_t time)
{
	wxString sqlSentence = wxT("INSERT INTO adaptparameter (updateTime, \
							   parameterName, \
							   updateBefore, \
							   updateAfter)  VALUES");

	wxString sqlSentenceAlarmInfo = wxT("INSERT INTO alarminfo (time, \
										endTime, \
										type, \
										alarmName, \
										level, \
										mode, \
										content, \
										origin) VALUES");

	//wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();

	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			for(auto item = m_alterRecord.cbegin(); item != m_alterRecord.cend(); item++)
			{
				auto sqlSentenceExe = sqlSentence;
				sqlSentenceExe.Append(wxString::Format(wxT("(%I64d, "), item->first));		
				sqlSentenceExe.Append(wxString::Format(wxT("'%s', "), wxString(item->second.first.c_str(),wxConvLocal)));
				sqlSentenceExe.Append(wxString::Format(wxT("'%s', "), wxString(item->second.first.c_str(),wxConvLocal)));
				sqlSentenceExe.Append(wxString::Format(wxT("'%s')"), wxString(item->second.second.c_str(),wxConvLocal)));

				if(!pDataBase->RunSQLNoQuery(sqlSentenceExe))
				{
					break;
				}

				auto sqlSentenceAlarmInfoExe = sqlSentenceAlarmInfo;
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("(%I64d, "), item->first));
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("%d, "), 0));
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("'%s', "), wxT("适配参数")));
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("'%s', "), wxT("NULL")));
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("%d, "),1));
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("'%s', "),wxT("0")));

				std::string content;
				content.append(item->second.first);
				content.append(" changed to ");
				content.append(item->second.second);

				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("'%s', "),wxString(content.c_str(),wxConvLocal)));
				sqlSentenceAlarmInfoExe.Append(wxString::Format(wxT("%d)"), 1));

				if(!pDataBase->RunSQLNoQuery(sqlSentenceAlarmInfoExe))
				{
					break;
				}


			}
			m_alterRecord.clear();
			m_contents.clear();

			return true;
	}
		// catch (wxString& error)
		// {
		// 	if(error.Find(wxT("Duplicate entry")) != wxNOT_FOUND)
		// 	{
		// 		return TRUE;
		// 	}
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }

	
	m_alterRecord.clear();
	m_contents.clear();

	return false;
}


void CAdaptChg::setUpdateTime(time_t time)
{
	m_updateTime = time;
	m_previousReadSize = GetPlug()->getfileConfig().adaptchgreadsize;
}

void CAdaptChg::updateTime(time_t time) 
{
	m_updateTime = time;
	
	GetPlug()->setadaptchgtime(m_updateTime);

	wxFile file(m_fileName);
	m_previousReadSize = file.Length();
	GetPlug()->setadaptchgreadsize(m_previousReadSize);
	file.Close();
	
}


time_t CAdaptChg::getRecordTime(const std::string& dateStr,const std::string& timeStr)
{
	struct tm ctime;
	std::string::size_type pos = 0;
	std::string::size_type previousPos = 0;
	pos = dateStr.find_first_of('-', previousPos);
	std::string yearStr = dateStr.substr(previousPos, pos - previousPos);
	ctime.tm_year = std::stoi(yearStr) - 1900;
	pos++;

	previousPos = pos;
	pos = dateStr.find_first_of('-', previousPos);
	std::string monthStr = dateStr.substr(previousPos, pos - previousPos);
	ctime.tm_mon = std::stoi(monthStr) - 1;
	pos++;

	previousPos = pos;
	std::string dayStr = dateStr.substr(previousPos,  dateStr.length() - previousPos);
	ctime.tm_mday = std::stoi(dayStr);

	pos = 0;
	previousPos = 0;
	pos = timeStr.find_first_of(':', previousPos);
	std::string hourStr = timeStr.substr(previousPos, pos - previousPos);
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
	pos++;

	return std::mktime(&ctime);

}

wxString CAdaptChg::getLatestFileName(time_t* time)
{
	wxString dirName(GetPlug()->getfileConfig().adaptchgdir);
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
		GetPlug()->AddLog(LOG_TYPE_ERROR,"可变适配参数文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName,wxString(wxT("*adapt*.txt")), wxDIR_FILES );
		while( flag )
		{
			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			wxFileName fileAtrribute(fullFilePath);
			if(!fileAtrribute.IsOk())
			{
				GetPlug()->AddLog(LOG_TYPE_ERROR,fullFilePath.mb_str(wxConvLocal));
				break;
			}
			auto updateTime = GetPlug()->fileModificationTime(fullFilePath);
			if(m_updateTime < updateTime)
			{
				*time = updateTime;
				newFileFlag = true;
				break;
			}
			flag = fileDir.GetNext(&fileName);

		}

		if(!newFileFlag)
		{
			fullFilePath.clear();
		}
	}
	return fullFilePath;
}

