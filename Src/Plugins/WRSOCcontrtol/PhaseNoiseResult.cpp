#include "PhaseNoiseResult.h"
#include <sstream>
#include <wx/dir.h>




CPhaseNoiseResult::CPhaseNoiseResult():m_phaseNoise(0),
			m_unfiltered(0),
			m_cluterSuppersion(0),
			m_filtered(0),
			m_updateTime(0),
			m_currentID(0)
{
}


CPhaseNoiseResult::~CPhaseNoiseResult()
{
}

void CPhaseNoiseResult::updateTime(time_t time)
{
	m_updateTime = time;
	GetPlug()->setphasenoiseresulttime(m_updateTime);
}

wxString CPhaseNoiseResult::getLatestFileName(time_t* time)
{
	wxString dirName(GetPlug()->getfileConfig().PhaseNoiseResultdir);
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
		GetPlug()->AddLog(LOG_TYPE_ERROR,"相位噪音测试结果文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName,wxString(wxT("*_PhaseNoiseResult_*.txt")), wxDIR_FILES );
		while( flag )
		{
			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			auto updateTime = GetPlug()->fileModificationTime(fileName);
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

int CPhaseNoiseResult::cbSQLGetID(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CPhaseNoiseResult *pT = static_cast<CPhaseNoiseResult*>(pIn);
	if(pT)
	{
		pT->m_currentID = std::strtoul(argv[0],NULL,10);
	}

	return 0;
}

INT CPhaseNoiseResult::loadTXTFile(time_t* time)
{
	auto wFileName = getLatestFileName(time);
	if(wFileName.IsEmpty())
	{
		return EXIT_FAILURE;
	}

	GetPlug()->AddLog(LOG_TYPE_MESSAGE,wFileName.ToStdString().c_str());

	wxFile file(wFileName,wxFile::read);
	if(file.IsOpened())
	{
		auto length = file.Length();
		char *buffer = new char[length + 1];
		std::memset(buffer,'\0',length + 1);
		file.Read(buffer,length);
		m_contents.assign(buffer);
		delete[] buffer;
	}
	else
	{
		file.Close();
		return EXIT_FAILURE;
	}
	file.Close();
	return EXIT_SUCCESS;
}

void CPhaseNoiseResult::parseFile()
{
	std::string::size_type pos;

	pos = m_contents.find("Angle");
	if (pos != std::string::npos)
	{
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		pos = newlinePos + 1;
	}

	std::string phaseNoise = m_contents.substr(pos, m_contents.length());

	std::istringstream phaseNoiseStream(phaseNoise);
	SHORT index;
	double IData;
	double QData;
	double angle;

	while (phaseNoiseStream >> index >> IData >> QData >> angle)
	{
		std::vector<double> dataPerLine;
		dataPerLine.push_back(IData);
		dataPerLine.push_back(QData);
		dataPerLine.push_back(angle);

		m_NoiseData.insert(std::pair<SHORT, std::vector<double> >(index,dataPerLine));

	}


}

bool CPhaseNoiseResult::checkFileTime(time_t* time)
{
	//struct tm ctime;
	std::string::size_type pos = m_contents.find_first_of('\n');
	if (pos != std::string::npos)
	{
		pos++;
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		std::string subStr = m_contents.substr(pos, newlinePos - pos);
		std::istringstream subStrStream(subStr);

		std::string dateStr;
		std::string timeStr;
		if (subStrStream >> dateStr >> timeStr >> m_phaseNoise >> m_unfiltered >> m_cluterSuppersion >> m_filtered)
		{
			/*pos = dateStr.find_first_of('-');
			std::string yearStr = dateStr.substr(0, pos);
			ctime.tm_year = std::stoi(yearStr) - 1900;
			pos++;

			std::string::size_type previousPos = pos;
			pos = dateStr.find_first_of('-',pos);
			std::string monthStr = dateStr.substr(previousPos, pos - previousPos);
			ctime.tm_mon = std::stoi(monthStr) - 1;
			pos++;

			std::string dayStr = dateStr.substr(pos, dateStr.length() - pos);
			ctime.tm_mday = std::stoi(dayStr);

			pos = timeStr.find_first_of(':');
			std::string hourStr = timeStr.substr(0, pos);
			ctime.tm_hour = std::stoi(hourStr);
			pos++;

			previousPos = pos;
			pos = timeStr.find_first_of(':', pos);
			std::string minStr = timeStr.substr(previousPos, pos - previousPos);
			ctime.tm_min = std::stoi(minStr);
			pos++;

			std::string secStr = timeStr.substr(pos, timeStr.length() - pos);
			ctime.tm_sec = std::stoi(secStr);

			*time = std::mktime(&ctime);
			if (*time != m_updateTime)
			{

			return true;
			}*/
		}
	}

	return true;
}

bool CPhaseNoiseResult::writeToDataBase(time_t time)
{
	wxString sqlSentenceOne = wxT("INSERT INTO phasenoiseinfo(updateTime, \
																			phNoise, \
																			unfiltered, \
																			clutterSupression, \
																			filtered) \
																			 VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		//try
		//{
			sqlSentenceOne.Append(wxString::Format(wxT("(%I64d, "),time));
			sqlSentenceOne.Append(wxString::Format(wxT("%f , "),m_phaseNoise));
			sqlSentenceOne.Append(wxString::Format(wxT("%f , "),m_unfiltered));
			sqlSentenceOne.Append(wxString::Format(wxT("%f , "),m_cluterSuppersion));
			sqlSentenceOne.Append(wxString::Format(wxT("%f)"),m_filtered));

			if(!pDataBase->RunSQLNoQuery(sqlSentenceOne))
			{

				SHOWFUNCNAME();

				m_NoiseData.clear();
				return false;
			}

			char queryStr[128] = {'\0'};
			sprintf_s(queryStr,128,"SELECT id FROM phasenoiseinfo WHERE updateTime = %I64d",time);
			pDataBase->RunSQLQuery(queryStr,cbSQLGetID,wxT("id"),this);
			// UINT infoID = 0;
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		infoID = pDataBase->GetIntValue(index,"id");
			// 	}
			// 	pDataBase->FreeResult(sqlError);
			// }
			
			for (auto item =m_NoiseData.cbegin(); item != m_NoiseData.cend();item++ )
			{
				auto tmp = (*item).second;
				
				wxString sqlSentenceTwo;
				sqlSentenceTwo.Printf(wxT("INSERT INTO phasenoiseresult(phaseNoiseId,  \
																			I,  \
																			Q,  \
																			angle) \
																			VALUES(%d, %f, %f, %f)"),m_currentID,tmp.at(0),tmp.at(1),tmp.at(2));


				if(!pDataBase->RunSQLNoQuery(sqlSentenceTwo))
				{
					SHOWFUNCNAME();
					m_NoiseData.clear();
					return false;
				}
			}

			m_NoiseData.clear();
			return true;

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

	m_NoiseData.clear();
	return false;
}
