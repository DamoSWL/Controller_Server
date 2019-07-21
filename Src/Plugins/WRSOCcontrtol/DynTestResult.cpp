#include "DynTestResult.h"
#include <sstream>
#include <wx/dir.h>




CDynTestResult::CDynTestResult():m_dynamicRange(0.0),
	m_slope(0.0),
	m_dynamicStart(0.0),
	m_dynamicEnd(0.0),
	m_updateTime(0.0),
	m_Hnoise(0.0),
	m_currentID(0),	
	m_newNoiseFlag(FALSE),
	m_newRangeFlag(FALSE),
	m_newSlopFlag(FALSE),
	m_newStartFlag(FALSE),
	m_newEndFlag(FALSE)
{
}


CDynTestResult::~CDynTestResult()
{
}

INT CDynTestResult::loadTXTFile(time_t* time)
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

void CDynTestResult::parseFile()
{
	m_newNoiseFlag = FALSE;
	m_newRangeFlag  = FALSE;
	m_newSlopFlag = FALSE;
	m_newStartFlag  = FALSE;
	m_newEndFlag  = FALSE;

	std::string::size_type pos = m_contents.find("noise");
	if (pos != std::string::npos)
	{
		pos = m_contents.find_first_of(':', pos);
		pos++;
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		wxString hNoise = wxString(m_contents.substr(pos, newlinePos - pos).c_str(),wxConvLocal);

		if(hNoise.Trim().IsEmpty())
		{
			m_newNoiseFlag = FALSE;
		}
		else
		{
			m_newNoiseFlag = TRUE;
			double noise = 0.0;
			hNoise.ToDouble(&noise);
			m_Hnoise = (FLOAT)noise;
		}
	}

	pos = m_contents.find("Range");
	if (pos != std::string::npos)
	{
		pos = m_contents.find_first_of(':', pos);
		pos++;
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		wxString dynaRangeStr = wxString(m_contents.substr(pos, newlinePos - pos).c_str(),wxConvLocal);
	
		if(dynaRangeStr.Trim().IsEmpty())
		{
			m_newRangeFlag = FALSE;
		}
		else
		{
			m_newRangeFlag = TRUE;
			double range = 0.0;
			dynaRangeStr.ToDouble(&range);
			m_dynamicRange = (FLOAT)range;
		}

	}

	pos = m_contents.find("Slope", pos);
	if (pos != std::string::npos)
	{
		pos = m_contents.find_first_of(':', pos);
		pos++;
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		wxString slope = wxString(m_contents.substr(pos, newlinePos - pos).c_str(),wxConvLocal);

		if(slope.Trim().IsEmpty())
		{
			m_newSlopFlag = FALSE;
		}
		else
		{
			m_newSlopFlag = TRUE;
			double slopeValue = 0.0;
			slope.ToDouble(&slopeValue);
			m_slope = (FLOAT)slopeValue;
		}
	}

	pos = m_contents.find("DynStart", pos);
	if (pos != std::string::npos)
	{
		pos = m_contents.find_first_of(':', pos);
		pos++;
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		wxString dynStart = wxString(m_contents.substr(pos, newlinePos - pos).c_str(),wxConvLocal);

		if(dynStart.Trim().IsEmpty())
		{
			m_newStartFlag = FALSE;
		}
		else
		{
			m_newStartFlag = TRUE;
			double start = 0.0;
			dynStart.ToDouble(&start);
			m_dynamicStart =(FLOAT)start;
		}

	}

	pos = m_contents.find("DynEnd", pos);
	if (pos != std::string::npos)
	{
		pos = m_contents.find_first_of(':', pos);
		pos++;
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		wxString dynEnd = wxString(m_contents.substr(pos, newlinePos - pos).c_str(),wxConvLocal);

		if(dynEnd.Trim().IsEmpty())
		{
			m_newEndFlag = FALSE;
		}
		else
		{
			m_newEndFlag = TRUE;
			double end = 0.0;
			dynEnd.ToDouble(&end);
			m_dynamicEnd = (FLOAT)end;
		}

		pos = newlinePos + 1;
	}

	pos = m_contents.find_first_of('\n',pos);
	if (pos != std::string::npos)
	{
		pos++;

	}

	while(pos <= m_contents.length())
	{
		std::string::size_type newLine = m_contents.find_first_of('\n',pos);
		if(newLine == std::string::npos)
		{
			break;
		}
		else
		{
			std::string dynaResult = m_contents.substr(pos, newLine - pos);

			if(dynaResult.find("Power") != std::string::npos)
			{
				pos = newLine + 1;
				continue;
			}


			if(dynaResult.empty())
			{
				break;
			}

			if(dynaResult.find(" ") == std::string::npos)
			{
				break;
			}
			std::istringstream dynaResultStream(dynaResult);

			FLOAT power = 0;
			FLOAT rawData = 0;
			FLOAT fitData = 0;
			FLOAT delta = 0;

			dynaResultStream >> power >> rawData >> fitData >> delta;
			
			std::vector<FLOAT> dataPerLine;
			dataPerLine.push_back(power);
			dataPerLine.push_back(rawData);
			dataPerLine.push_back(fitData);
			dataPerLine.push_back(delta);

			m_dynamicData.insert(dataPerLine);

			
		}

		pos = newLine + 1;

	}

}

bool CDynTestResult::checkFileTime(time_t* time)
{
	/*struct tm ctime;
	std::string::size_type pos = m_contents.find_first_of(':');
	std::string::size_type previousPos = pos + 1;
	pos = m_contents.find_first_of('-', previousPos);
	std::string yearSautotr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_year = std::stoi(yearStr) + 2000 - 1900;
	pos++;

	previousPos = pos;
	pos = m_contents.find_first_of('-', previousPos);
	std::string monthStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_mon = std::stoi(monthStr) - 1;
	pos++;

	previousPos = pos;
	pos = m_contents.find_first_of(' ', previousPos);
	std::string dayStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_mday = std::stoi(dayStr);
	pos++;

	previousPos = pos;
	pos = m_contents.find_first_of(':', previousPos);
	std::string hourStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_hour = std::stoi(hourStr);
	pos++;

	previousPos = pos;
	pos = m_contents.find_first_of(':', previousPos);
	std::string minStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_min = std::stoi(minStr);
	pos++;

	previousPos = pos;
	pos = m_contents.find_first_of('\n', previousPos);
	std::string secStr = m_contents.substr(previousPos, pos - previousPos);
	ctime.tm_sec = std::stoi(secStr);
	pos++;

	*time = std::mktime(&ctime);
	if (*time != m_updateTime)
	{
		return true;
	}

	return false;*/
	return true;
}

bool CDynTestResult::writeToDataBase(time_t time)
{
	wxString sqlSentenceOne = wxT("INSERT INTO dyntestinfo(updateTime, \
																			hNoise, \
																			dynamicRange, \
																			slope, \
																			dynStart, \
																			dynEnd) \
																			 VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	
	if (pDataBase)
	{
		// try
		// {

			sqlSentenceOne.Append(wxString::Format(wxT("(%I64d, "),time));
			if(!m_newNoiseFlag)
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
			}
			else
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%f, "),m_Hnoise));
			}

			if(!m_newRangeFlag)
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
			}
			else
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%f, "),m_dynamicRange));
			}
			
			if(!m_newSlopFlag)
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
			}
			else
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%f, "),m_slope));
			}
			
			if(!m_newStartFlag)
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
			}
			else
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%f, "),m_dynamicStart));
			}
			
			if(!m_newEndFlag)
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%s)"),wxT("NULL")));
			}
			else
			{
				sqlSentenceOne.Append(wxString::Format(wxT("%f)"),m_dynamicEnd));
			}
			

			if(!pDataBase->RunSQLNoQuery(sqlSentenceOne))
			{
				SHOWFUNCNAME();
				m_dynamicData.clear();
				return false;
			}

			char queryStr[128] = {'\0'};
			sprintf_s(queryStr,128,"SELECT id FROM dyntestinfo WHERE updateTime = %I64d",time);
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
		
			for (auto item = m_dynamicData.cbegin();item != m_dynamicData.cend();item++)
			{
			
				auto injectionPower = item->at(0);
				auto measuredPower1 = item->at(1);
				auto measuredPower2 = item->at(2);
				auto deviation = item->at(3);

				wxString sqlSentenceTwo;
				sqlSentenceTwo.Printf(wxT("INSERT INTO dyntestresult(dynTestId,  \
																	injectionPower,  \
																	measuredPower1,  \
																	measuredPower2,  \
																	deviation)  \
																	VALUES(%d, %f, %f, %f, %f)"),m_currentID,injectionPower,measuredPower1,measuredPower2,deviation);
					
				if(!pDataBase->RunSQLNoQuery(sqlSentenceTwo))
				{
					SHOWFUNCNAME();
					break;
				}
	
				
			}
			

			m_dynamicData.clear();
			
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

	m_dynamicData.clear();

	return false;
}

void CDynTestResult::updateTime(time_t time)
{
	m_updateTime = time;
	GetPlug()->setdyntestresulttime(m_updateTime);
}

int CDynTestResult::cbSQLGetID(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CDynTestResult *pT = static_cast<CDynTestResult*>(pIn);
	if(pT)
	{
		pT->m_currentID = std::strtoul(argv[0],NULL,10);
	}

	return 0;
}

wxString CDynTestResult::getLatestFileName(time_t* time)
{
	wxString dirName(GetPlug()->getfileConfig().DynTestResultdir);
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
		GetPlug()->AddLog(LOG_TYPE_ERROR,"动态范围测试结果文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName,wxT("*_DynTestResult_*.txt"), wxDIR_FILES );
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

