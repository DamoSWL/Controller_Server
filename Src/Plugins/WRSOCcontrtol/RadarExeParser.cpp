
#include "RadarExeParser.h"
#include <wx/dir.h>



CRadarExeParser::CRadarExeParser():m_updateTime(0),
	m_fileSize(0)
{
}


CRadarExeParser::~CRadarExeParser()
{
}

bool CRadarExeParser::checkFileTime(time_t* newTime,wxString& newFileName)
{
	wxString dirName(GetPlug()->getfileConfig().radarexeDir);
	if(dirName.IsEmpty())
	{
		return false;
	}
	if(!::wxDirExists(dirName))
	{
		return false;
	}
	wxDir fileDir(dirName);
	wxString fullFilePath;

	if ( !fileDir.IsOpened() )
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"雷达软件目录异常");
	}
	else
	{
		wxString fileName;
		bool flag = fileDir.GetFirst(&fileName, wxT("*.exe"), wxDIR_FILES );
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

			wxFile File(fullFilePath);
			m_fileSize = File.Length();
			File.Close();

			auto updateTime = ::wxFileModificationTime(fullFilePath);
			if(m_updateTime !=  updateTime) 
			{
				*newTime = updateTime;
				newFileName = fileName;
				return true;
			}
			if(wxString(m_previousFileName.c_str(),wxConvLocal) != fileName)
			{
				wxString mess;
				mess.Printf(wxT("filename, size : %d"),m_fileSize);
			//	GetPlug()->AddLog(LOG_TYPE_MESSAGE,mess.mb_str(wxConvLocal));

				*newTime = ::wxDateTime::GetTimeNow();
				newFileName = fileName;
				return true;
			}
			flag = fileDir.GetNext(&fileName);
		}

	}

	return false;
}

bool CRadarExeParser::writeToDataBase(time_t newTime,wxString& newFileName)
{
	wxString sqlSentence = wxT("INSERT INTO radarsoftversion (updateTime, \
																			exeName, \
																			version, \
																			fileSize) \
																			VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), newTime));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), newFileName));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%f)"),m_fileSize * 1.0));

			return pDataBase->RunSQLNoQuery(sqlSentence);

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
	return false;
}

void CRadarExeParser::setUpdateTime()
{
	m_updateTime = GetPlug()->getfileConfig().radarexetime;
}

void CRadarExeParser::setPreviousFileName()
{
	m_previousFileName = GetPlug()->getfileConfig().radarexefilename;
}

void CRadarExeParser::updateTime(time_t newTime)
{
	m_updateTime = newTime;
	GetPlug()->setradarexetime(m_updateTime);
}

void CRadarExeParser::updateFileName(const wxString& newFileName)
{
	m_previousFileName.assign(newFileName.ToStdString());
	GetPlug()->setradarexefilename(m_previousFileName);
}
