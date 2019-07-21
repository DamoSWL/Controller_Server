#include "RefCalibResult.h"
#include <sstream>
#include <wx/dir.h>






CRefCalibResult::CRefCalibResult():m_updateTime(0)
{
}


CRefCalibResult::~CRefCalibResult()
{
}

INT CRefCalibResult::loadTXTFile(time_t* time)
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

void CRefCalibResult::parseFile()
{

	std::string::size_type pos = m_contents.find("km");

	std::vector<UINT> rangeVector;
	if (pos != std::string::npos)
	{
		std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
		std::string range = m_contents.substr(pos, newlinePos - pos);
		std::istringstream rangeStream(range);

		std::string item;
		rangeStream >> item;

		UINT distance = 0;
		while (rangeStream >> distance)
		{
			rangeVector.push_back(distance);
		}	

	}

	UINT index = 1;

	while(1)
	{
		pos = m_contents.find("Expected",pos);
		std::vector<FLOAT> expectedVector;
		if (pos != std::string::npos)
		{
			std::string::size_type newlinePos = m_contents.find_first_of('\n',pos);
			std::string expectedStr = m_contents.substr(pos, newlinePos - pos);
			std::istringstream expectedStream(expectedStr);

			std::string item;
			expectedStream >> item;

			FLOAT expected = 0;
			while (expectedStream >> expected)
			{
				expectedVector.push_back(expected);
			}

			pos = newlinePos + 1;
		}
		else
		{
			break;
		}


		pos = m_contents.find("Measured", pos);
		std::vector<FLOAT> measuredVector;
		if (pos != std::string::npos)
		{
			std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
			std::string measuredStr = m_contents.substr(pos, newlinePos - pos);
			std::istringstream measuredStream(measuredStr);

			std::string item;
			measuredStream >> item;
			FLOAT measured = 0;
			while (measuredStream >> measured)
			{
				measuredVector.push_back(measured);
			}
			pos = newlinePos + 1;
		}

		pos = m_contents.find("Delta", pos);
		std::vector<FLOAT> deltaVector;
		if (pos != std::string::npos)
		{
			std::string::size_type newlinePos = m_contents.find_first_of('\n', pos);
			std::string deltaStr = m_contents.substr(pos, newlinePos - pos);
			std::istringstream deltaStream(deltaStr);

			std::string item;
			deltaStream >> item;
			FLOAT delta = 0;

			while (deltaStream >> delta)
			{
				deltaVector.push_back(delta);
			}

		}

		if (rangeVector.size() == expectedVector.size() - 1)
		{
			m_expectedFlag[index] = true;
			m_expectedPower[index] = expectedVector.at(0);
			expectedVector.erase(expectedVector.begin(),expectedVector.begin() + 1);
		}

		if (rangeVector.size() == measuredVector.size() - 1)
		{
			m_measuredFlag[index] = true;
			m_measuredPower[index] = measuredVector.at(0);
			measuredVector.erase(measuredVector.begin(),measuredVector.begin() + 1);
		}

		std::map<UINT, std::vector<FLOAT> > refCalibResult;
		for (UINT num = 0; num < rangeVector.size(); num++)
		{
			UINT dis = 0;
			std::vector<FLOAT> tmp;

			dis = rangeVector.at(num);
			tmp.push_back(expectedVector.at(num));
			tmp.push_back(measuredVector.at(num));
			tmp.push_back(deltaVector.at(num));


			refCalibResult[dis] = tmp;
		}
		m_refCalibResult[index] = refCalibResult;
		index++;
	}
	
}

UINT CRefCalibResult::getMonthNum(std::string & monthStr)
{
	if (monthStr == "Jan")
	{
		return static_cast<UINT>(Month::Jan);
	}
	else if (monthStr == "Feb")
	{
		return static_cast<UINT>(Month::Feb);
	}
	else if (monthStr == "Mar")
	{
		return static_cast<UINT>(Month::Mar);
	}
	else if (monthStr == "Apr")
	{
		return static_cast<UINT>(Month::Apr);
	}
	else if (monthStr == "May")
	{
		return static_cast<UINT>(Month::May);
	}
	else if (monthStr == "Jun")
	{
		return static_cast<UINT>(Month::Jun);
	}
	else if (monthStr == "Jul")
	{
		return static_cast<UINT>(Month::Jul);
	}
	else if (monthStr == "Aug")
	{
		return static_cast<UINT>(Month::Aug);
	}
	else if (monthStr == "Sept")
	{
		return static_cast<UINT>(Month::Sept);
	}
	else if (monthStr == "Sep")
	{
		return static_cast<UINT>(Month::Sept);
	}
	else if (monthStr == "Oct")
	{
		return static_cast<UINT>(Month::Oct);
	}
	else if (monthStr == "Nov")
	{
		return static_cast<UINT>(Month::Nov);
	}
	else if (monthStr == "Dec")
	{
		return static_cast<UINT>(Month::Dec);
	}
	else
	{
		return 0;
	}

}

wxString CRefCalibResult::getLatestFileName(time_t* time)
{
	wxString dirName(GetPlug()->getfileConfig().RefCalibResultdir);
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
		GetPlug()->AddLog(LOG_TYPE_ERROR,"发射率标定结果文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName,wxString(wxT("*_RefCalibResult_*.txt")), wxDIR_FILES );
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

bool CRefCalibResult::checkFileTime(time_t* time)
{
	/*struct tm ctime;

	std::istringstream dateTimeStream(m_contents);

	std::string dayOfWeekStr;
	std::string monthStr;
	std::string dayStr;
	std::string timeStr;
	std::string yearStr;

	if (dateTimeStream >> dayOfWeekStr >> monthStr >> dayStr >> timeStr >> yearStr)
	{
	ctime.tm_mon = getMonthNum(monthStr) - 1;
	ctime.tm_mday = std::stoi(dayStr);
	ctime.tm_year = std::stoi(yearStr) - 1900;

	std::string::size_type pos = timeStr.find_first_of(':');
	std::string hourStr = timeStr.substr(0, pos);
	ctime.tm_hour = std::stoi(hourStr);
	pos++;

	std::string::size_type previousPos = pos;
	pos = timeStr.find_first_of(':', previousPos);
	std::string minStr = timeStr.substr(previousPos, pos - previousPos);
	ctime.tm_min = std::stoi(minStr);

	pos++;
	previousPos = pos;
	pos = timeStr.find_first_of(' ', previousPos);
	std::string secStr = timeStr.substr(previousPos, pos - previousPos);
	ctime.tm_sec = std::stoi(secStr);

	*time = std::mktime(&ctime);

	if (*time != m_updateTime)
	{
	return true;
	}
	}

	return false;*/
	return true;

}

bool CRefCalibResult::writeToDataBase(time_t time)
{
	wxString sqlSentence = wxT("INSERT INTO refcalibresult(updateTime, \
	ranges, \
	expected, \
	measured, \
	delta, \
	expectedPower, \
	measuredPower) \
	VALUES");

	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			sqlSentence.Append(wxString::Format(wxT("(%I64d, "),time));

			for (auto item = m_refCalibResult.cbegin();item != m_refCalibResult.cend();item++)
			{
				auto index = item->first;
				auto refCalibraResult = item->second;

				for(auto innerItem= refCalibraResult.cbegin(); innerItem != refCalibraResult.cend(); innerItem++)
				{
					UINT dis = (*innerItem).first;
					std::vector<FLOAT> tmp = (*innerItem).second;

					wxString sqlTmp = sqlSentence;

					sqlTmp.Append(wxString::Format(wxT("%d, "),dis));
					sqlTmp.Append(wxString::Format(wxT("%f, "),tmp.at(0)));
					sqlTmp.Append(wxString::Format(wxT("%f, "),tmp.at(1)));
					sqlTmp.Append(wxString::Format(wxT("%f, "),tmp.at(2)));

					if (m_expectedFlag[index])
					{
						sqlTmp.Append(wxString::Format(wxT("%f, "),m_expectedPower[index]));
						m_expectedFlag[index] = false;
					}
					else
					{
						sqlTmp.Append(wxString::Format(wxT("%f, "),0.0));
					}

					if (m_measuredFlag[index])
					{
						sqlTmp.Append(wxString::Format(wxT("%f)"),m_measuredPower[index]));
						m_measuredFlag[index] = false;
					}
					else
					{
						sqlTmp.Append(wxString::Format(wxT("%f)"),0.0));
					}

					if(!pDataBase->RunSQLNoQuery(sqlTmp))
					{

						SHOWFUNCNAME();
						break;
					}
				}
			}

			m_refCalibResult.clear();
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

	m_refCalibResult.clear();
	return false;
}

void CRefCalibResult::updateTime(time_t time)
{
	m_updateTime = time;
	GetPlug()->setrefcalibresulttime(time);
}
