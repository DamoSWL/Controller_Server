#include "UPSHandler.h"
#include <fstream>
#include <sstream>





UPSHandler::UPSHandler():m_currentID(0)
{
	
}

UPSHandler::~UPSHandler()
{
}



void UPSHandler::getUPSInfo()
{
	auto pDataBase = GetPlug()->getDatabase();
	if(pDataBase)
	{
		char queryStr[256] = {'\0'};
		sprintf_s(queryStr,256,"SELECT id FROM thresholdparameter WHERE tableName = 'upsinfo'");
		pDataBase->RunSQLQuery(queryStr,cbSQLGetID,wxT("id"),this);

		for(auto id = m_UPSID.cbegin();id != m_UPSID.cend();id++)
		{
			m_currentID = id->first;
			sprintf_s(queryStr,256,"SELECT eName FROM thresholdparameter WHERE id = %d",id->first);
			pDataBase->RunSQLQuery(queryStr,cbSQLGetName,wxT("eName"),this);
		}

		for(auto id = m_UPSID.cbegin();id != m_UPSID.cend();id++)
		{
			m_currentName = id->second;
			sprintf_s(queryStr,256,"SELECT showType FROM thresholdparameter WHERE eName = '%s'",id->second.c_str());
			pDataBase->RunSQLQuery(queryStr,cbSQLGetOID,wxT("showType"),this);
		}

		for(auto id = m_UPSID.cbegin();id != m_UPSID.cend();id++)
		{
			m_currentName = id->second;
			sprintf_s(queryStr,256,"SELECT lowerLimit FROM thresholdparameter WHERE eName = '%s' AND lowerLimit IS NOT NULL",id->second.c_str());
			pDataBase->RunSQLQuery(queryStr,cbSQLGetCoefficient,wxT("lowerLimit"),this);
		}

	}


}

int UPSHandler::cbSQLGetID(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	UPSHandler *pT = static_cast<UPSHandler*>(pIn);
	if(pT)
	{
		pT->m_UPSID[std::strtoul(argv[0],NULL,10)] = std::string("");
	}

	return 0;
}

int UPSHandler::cbSQLGetName(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	UPSHandler *pT = static_cast<UPSHandler*>(pIn);
	if(pT)
	{
		pT->m_UPSID[pT->m_currentID] = std::string(argv[0]);
	}

	return 0;
}

int UPSHandler::cbSQLGetOID(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	UPSHandler *pT = static_cast<UPSHandler*>(pIn);
	if(pT)
	{
		pT->m_UPSOID[pT->m_currentName] = std::string(argv[0]);
		pT->m_UPSData[pT->m_currentName] = 0;
		pT->m_UPSCoefficient[pT->m_currentName] = 1.0;
	}

	return 0;
}

int UPSHandler::cbSQLGetCoefficient(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	UPSHandler *pT = static_cast<UPSHandler*>(pIn);
	if(pT)
	{
		pT->m_UPSCoefficient[pT->m_currentName] = std::stof(std::string(argv[0]));
	}

	return 0;
}


BOOL UPSHandler::queryUPSData()
{
	BOOL result = FALSE;
	for(auto id = m_UPSOID.cbegin();id != m_UPSOID.cend(); id++)
	{
		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("ups.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("Snmputil get %s public  %s > ups.txt"),GetPlug()->m_UPSIPAdress.c_str(),wxString(id->second)));
		}
		cmdFile.Close();

		char cmdLine[] = {"ups.bat"};

		if(::CreateProcessA(NULL,
			cmdLine,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi))
		{
			::WaitForSingleObject(pi.hProcess,INFINITE);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}

		std::ifstream file("ups.txt");
		if(file.is_open())
		{
			while(!file.eof())
			{
				std::string content;
				std::getline(file,content,'\n');

				if(content.find("Value") != std::string::npos)
				{
					std::istringstream valueStream(content);

					std::string strOne;
					std::string strTwo;
					std::string strThree;
					std::string valueStr;

					if(valueStream >> strOne >> strTwo >>strThree >> valueStr)
					{
						m_UPSData[id->first] = std::stol(valueStr);
					}

					break;

				}
				else if(content.find("error on SnmpMgrRequest") != std::string::npos)
				{
					result = FALSE;

					file.close();
					//::wxRemoveFile(wxT("ups.bat"));
					//::wxRemoveFile(wxT("ups.txt"));
					return result;
				}
				else if(content.empty())
				{
					result = FALSE;

					file.close();
					return result;
				}
				else{}


			}
			file.close();	
			result = TRUE;
		}
		else
		{
			result = FALSE;
			break;
			
		}

	}
	//::wxRemoveFile(wxT("ups.bat"));
	//::wxRemoveFile(wxT("ups.txt"));

	return result;
}


bool UPSHandler::writeToDataBase()
{
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{
		wxString sqlSentence = wxT("INSERT INTO upsinfo(updateTime,");
		for(auto id = m_UPSID.cbegin(); id != m_UPSID.cend(); id++)
		{
			sqlSentence.Append(wxString::Format(wxT("%s,"),wxString(id->second.c_str())));
		}
		sqlSentence.RemoveLast(1);
		sqlSentence.Append(wxString::Format(wxT(") VALUES(%I64d,"),wxDateTime::GetTimeNow()));

		for(auto id = m_UPSID.cbegin(); id != m_UPSID.cend(); id++)
		{
			if(m_UPSCoefficient[id->second] >= 100.0)
			{
				sqlSentence.Append(wxString::Format(wxT("%.2f,"),m_UPSData[id->second] / m_UPSCoefficient[id->second]));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%d,"),m_UPSData[id->second]));
			}
			
		}
		sqlSentence.RemoveLast(1);
		sqlSentence.Append(wxT(")"));

		return pDataBase->RunSQLNoQuery(sqlSentence);

	}

	return false;
}

DWORD WINAPI UPSHandler::exec(LPVOID para)
{
	
	UPSHandler* self = static_cast<UPSHandler*>(para);
	if(self)
	{
		self->getUPSInfo();
		while (!GetPlug()->GetExit())
		{
			if(self->queryUPSData())
			{
				self->writeToDataBase();

			}
			for(UINT index = 0;index < GetPlug()->getfileConfig().UPSInterval*60;index++)
			{
				if(GetPlug()->GetExit())
				{
					break;
				}
				::wxSleep(1);
			}
		}
	}

	return EXIT_SUCCESS;
}

//BOOL UPSHandler::StartCom()
//{
//
//	comConfig serialPortConfig;
//	serialPortConfig.portnr = 1;
//	serialPortConfig.baud = 9600;
//	serialPortConfig.parity = 'N';
//	serialPortConfig.databits = 8;
//	serialPortConfig.stopbits = 1;
//
//	if (m_serialPort.InitPort(this,
//		serialPortConfig.portnr,
//		serialPortConfig.baud,
//		serialPortConfig.parity,
//		serialPortConfig.databits,
//		serialPortConfig.stopbits))
//	{
//		
//		m_serialPort.StartMonitoring();
//		::wxSleep(1);
//		m_serialPort.WriteToPort("AT+CMGF=0\x0D", 10);
//		::wxSleep(1);
//		m_serialPort.WriteToPort("AT+CMGS=27\x0D", 11);
//		::wxMilliSleep(200);
//		
//		m_serialPort.WriteToPort("0011000D91683189024261F20008A70C738B65B05B87800D6D416C13\x1A\x0D", 58);
//
//		return TRUE;
//
//	}
//	else
//	{
//		return FALSE;
//	}
//	return FALSE;
//}

//BOOL UPSHandler::StopCom()
//{
//	return m_serialPort.StopMonitoring();
//}
//
//void UPSHandler::sendUPSCmd()
//{
//	m_serialPort.WriteToPort("Q1\x0D", 3);
//}



//void UPSHandler::CallBack(UINT msg, WPARAM chi, LPARAM port)
//{
//	
//	static int UPSCnt = 0;
//	static char UPSDataStore[46];
//	
//	if (msg == WM_COMM_RXCHAR)
//	{
//		unsigned char data = (unsigned char)(chi);
//		UPSDataStore[UPSCnt] = data;
//		if (UPSCnt == 5 || UPSCnt == 11 || UPSCnt == 17 || UPSCnt == 21 || UPSCnt == 26 || UPSCnt == 31 || UPSCnt == 36)
//		{
//			if (data != ' ')
//			{
//				UPSCnt = 0;
//				return;
//			}
//		}
//		if (UPSCnt == 45)
//		{
//			if (data == 0x0D)
//			{
//				
//				tagUPSStatus UPSStatus;
//				UPSStatus.bValid = true;
//				UPSStatus.inVoltage = static_cast<FLOAT>(atof(std::string(&(UPSDataStore[0]), 5).c_str()));
//				UPSStatus.lastBatteryVoltage = static_cast<FLOAT>(atof(std::string(&(UPSDataStore[6]), 5).c_str()));
//				UPSStatus.outVoltage = static_cast<FLOAT>(atof(std::string(&(UPSDataStore[12]), 5).c_str()));
//				UPSStatus.outLoadPercentage = atoi(std::string(&(UPSDataStore[18]), 3).c_str());
//				UPSStatus.inFreq = static_cast<FLOAT>(atof(std::string(&(UPSDataStore[22]), 4).c_str()));
//				UPSStatus.batteryEleVoltage = static_cast<FLOAT>(atof(std::string(&(UPSDataStore[27]), 4).c_str()));
//				UPSStatus.temperature = static_cast<FLOAT>(atof(std::string(&(UPSDataStore[32]), 4).c_str()));
//				UPSStatus.bElectricSupply = UPSDataStore[37];
//				UPSStatus.bBatteryVoltage = UPSDataStore[38];
//				UPSStatus.bBypassModel = UPSDataStore[39];
//				UPSStatus.bUPSStatus = UPSDataStore[40];
//				UPSStatus.bUPSMode = UPSDataStore[41];
//				UPSStatus.bTesting = UPSDataStore[42];
//				UPSStatus.bShutdown = UPSDataStore[43];
//				UPSStatus.reserve = 0;
//				UPSStatus.Spa = 0;
//
//				if(m_radarEnvDataBase)
//				{
//					m_radarEnvDataBase->insertUPSData(UPSStatus);
//				}
//				
//		
//
//			}
//			UPSCnt = 0;
//			return;
//		}
//		UPSCnt++;
//	}
//	
//}


