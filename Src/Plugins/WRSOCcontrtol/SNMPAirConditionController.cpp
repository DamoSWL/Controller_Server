#include "SNMPAirConditionController.h"
#include <fstream>
#include <sstream>


CSNMPAirConditionController::CSNMPAirConditionController(void):m_currentDeviceIndex(0)
{
}


CSNMPAirConditionController::~CSNMPAirConditionController(void)
{
}

bool CSNMPAirConditionController::parseAirCmd(const char *pData, UINT Len)  //该函数和SpecAirConditionController.h中一样
{
	std::string cmd(pData,Len);
	INT pos = 0;

	if(cmd.find(":") != std::string::npos)
	{
		m_currentDeviceIndex = std::stoi(cmd.substr(std::strlen("#PATCTL:"),1)); 
	}
	else
	{
		return false;
	}


	if(cmd.find("Power") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto tmpStr = cmd.substr(pos,cmd.length() - 1 - pos);
		INT flag = std::stoi(tmpStr);

		sendSwitchCmd(flag);
	}
	else if(cmd.find("Temp") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto tmpStr = cmd.substr(pos,cmd.length() - 1 - pos);
		INT value = std::stoi(tmpStr);

		sendTemperatureCmd(value);
	}
	else if(cmd.find("Humi") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto tmpStr = cmd.substr(pos,cmd.length() - 1 - pos);
		INT value = std::stoi(tmpStr);

		sendHumidityCmd(value);
	}
	else{}

	return true;
}

bool CSNMPAirConditionController::sendSwitchCmd(INT flag)  
{

	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND) //根据不同台站发送控制命令
	{

		wxString airIPAddress;
		wxString cmdOID;

		if(m_currentDeviceIndex == 1)  //根据当前设备编号设置需要控制空调的IP
		{
			airIPAddress = wxT("10.156.137.90");
		}
		else if(m_currentDeviceIndex == 2)
		{
			airIPAddress = wxT("10.156.137.91");
		}
		else
		{
			return false;
		}

		if(flag == 1)  
		{
			cmdOID = wxT(".1.3.6.1.4.1.13400.2.324.2.3.2.0");  //开机的命令，不同台站可能不一样
		}
		else if(flag == 0)
		{
			cmdOID = wxT(".1.3.6.1.4.1.13400.2.324.2.3.1.0");  //关机的命令
		}
		else
		{
			return false;
		}

		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("airController.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpset -v 2c -c private %s %s i 1"),airIPAddress,cmdOID));
		}
		cmdFile.Close();

		char cmdLine[] = {"airController.bat"};   //将命令写入批处理文件中，开启一个进程来调用批处理

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}
	}
	
	return true;
}

bool CSNMPAirConditionController::sendTemperatureCmd(INT temperature)
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND) //根据不同台站发送控制命令
	{

		wxString airIPAddress;

		if(m_currentDeviceIndex == 1)   //根据当前设备编号设置需要控制空调的IP
		{
			airIPAddress = wxT("10.156.137.90");
		}
		else if(m_currentDeviceIndex == 2)
		{
			airIPAddress = wxT("10.156.137.91");
		}
		else
		{
			return false;
		}


		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("airController.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpset -v 2c -c private %s .1.3.6.1.4.1.13400.2.324.2.4.1.0 i %d"),airIPAddress,temperature));
		}
		cmdFile.Close();

		char cmdLine[] = {"airController.bat"};    //将命令写入批处理文件中，开启一个进程来调用批处理

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}
	}

	return true;
}

bool CSNMPAirConditionController::sendHumidityCmd(INT humidity)
{ 
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)  //根据不同台站发送控制命令
	{

		wxString airIPAddress;

		if(m_currentDeviceIndex == 1)    //根据当前设备编号设置需要控制空调的IP
		{
			airIPAddress = wxT("10.156.137.90");
		}
		else if(m_currentDeviceIndex == 2)
		{
			airIPAddress = wxT("10.156.137.91");
		}
		else
		{
			return false;
		}


		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("airController.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpset -v 2c -c private %s .1.3.6.1.4.1.13400.2.324.2.4.2.0 i %d"),airIPAddress,humidity));
		}
		cmdFile.Close();

		char cmdLine[] = {"airController.bat"};    //将命令写入批处理文件中，开启一个进程来调用批处理

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}
	}

	return true;
}

bool CSNMPAirConditionController::AcquireStatus()
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)  //根据不同台站发查询制命令
	{
		AcquireStatusByIP(1,wxT("10.156.137.90"));
		writeAirStatusToDataBase(1);
		AcquireStatusByIP(2,wxT("10.156.137.91"));
		writeAirStatusToDataBase(2);
	}

	return true;
}

bool CSNMPAirConditionController::AcquireStatusByIP(INT index, const wxString& IP)
{
	AcquireOnOffStatus(IP);
	AcquireSetTemperature(IP);
	AcquireSetHumidity(IP);
	AcquireCurrentTemperature(index,IP);
	AcquireCurrentHumidity(index,IP);

	return true;
}

bool CSNMPAirConditionController::AcquireOnOffStatus(const wxString& IP)
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)   //根据不同台站发查询制命令
	{

		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("airStatus.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpget -v 2c -c public %s .1.3.6.1.4.1.13400.2.324.2.2.26.0 > airStatus.txt"),IP));  //.1.3.6.1.4.1.13400.2.324.2.2.26.0为查询编码，不同信号空调可能不一样
		}
		cmdFile.Close();

		char cmdLine[] = {"airStatus.bat"};  //将命令写入批处理文件中，开启一个进程来调用批处理

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}

		if(wxFileName::GetSize(wxT("airStatus.txt")) <= 0)
		{
			return false;
		}

		std::ifstream file("airStatus.txt");
		if(file.is_open())
		{
			while(!file.eof())
			{
				std::string content;
				std::getline(file,content,'\n');


				std::istringstream valueStream(content);

				std::string strOne;
				std::string strTwo;
				std::string strThree;
				std::string valueStr;

				if(valueStream >> strOne >> strTwo >>strThree >> valueStr)
				{
					m_AirStatus.onoffStatus = std::stoi(valueStr);
					if(m_AirStatus.onoffStatus > 0)
					{
						m_AirStatus.onoffStatus = 0;
					}
					else
					{
						m_AirStatus.onoffStatus = 1;
					}
				}

			}
			file.close();	

		}
	}

	return true;
}

bool CSNMPAirConditionController::AcquireSetTemperature(const wxString& IP)
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)
	{

		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("airStatus.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpget -v 2c -c public %s .1.3.6.1.4.1.13400.2.324.2.4.1.0 > airStatus.txt"),IP));
		}
		cmdFile.Close();

		char cmdLine[] = {"airStatus.bat"};

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}

		if(wxFileName::GetSize(wxT("airStatus.txt")) <= 0)
		{
			return false;
		}

		std::ifstream file("airStatus.txt");
		if(file.is_open())
		{
			while(!file.eof())
			{
				std::string content;
				std::getline(file,content,'\n');
				
				if(content.find("No Such") != std::string::npos)
				{
					file.close();
					return false;
				}

				std::istringstream valueStream(content);

				std::string strOne;
				std::string strTwo;
				std::string strThree;
				std::string valueStr;

				if(valueStream >> strOne >> strTwo >>strThree >> valueStr)
				{
					m_AirStatus.setTemperature = std::stoi(valueStr) * 1.0;
				}

			}
			file.close();	

		}
	}

	return true;
}

bool CSNMPAirConditionController::AcquireSetHumidity(const wxString& IP)
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)
	{

		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;


		wxFile cmdFile(wxT("airStatus.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpget -v 2c -c public %s .1.3.6.1.4.1.13400.2.324.2.4.2.0 > airStatus.txt"),IP));
		}
		cmdFile.Close();

		char cmdLine[] = {"airStatus.bat"};

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}

		if(wxFileName::GetSize(wxT("airStatus.txt")) <= 0)
		{
			return false;
		}

		std::ifstream file("airStatus.txt");
		if(file.is_open())
		{
			while(!file.eof())
			{
				std::string content;
				std::getline(file,content,'\n');

				if(content.find("No Such") != std::string::npos)
				{
					file.close();
					return false;
				}

				std::istringstream valueStream(content);

				std::string strOne;
				std::string strTwo;
				std::string strThree;
				std::string valueStr;

				if(valueStream >> strOne >> strTwo >>strThree >> valueStr)
				{
					m_AirStatus.setHumidity = std::stoi(valueStr) * 1.0;
				}

			}
			file.close();	

		}
	}

	return true;
}

bool CSNMPAirConditionController::AcquireCurrentTemperature(INT index,const wxString& IP)
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)
	{

		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		wxString OID;
		FLOAT cof = 1.0;

		if(index == 1)
		{
			OID = wxT(".1.3.6.1.4.1.13400.2.324.2.2.3.0");
			cof = 0.1f;
		}
		else if(index == 2)
		{
			OID = wxT(".1.3.6.1.4.1.13400.2.324.2.2.2.0");
			cof = 0.01f;
		}

		wxFile cmdFile(wxT("airStatus.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpget -v 2c -c public %s %s > airStatus.txt"),IP,OID));
		}
		cmdFile.Close();

		char cmdLine[] = {"airStatus.bat"};

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}

		if(wxFileName::GetSize(wxT("airStatus.txt")) <= 0)
		{
			return false;
		}

		std::ifstream file("airStatus.txt");
		if(file.is_open())
		{
			while(!file.eof())
			{
				std::string content;
				std::getline(file,content,'\n');


				std::istringstream valueStream(content);

				std::string strOne;
				std::string strTwo;
				std::string strThree;
				std::string valueStr;

				if(valueStream >> strOne >> strTwo >>strThree >> valueStr)
				{
					m_AirStatus.currentTemperature = std::stoi(valueStr) * cof;
				}

			}
			file.close();	

		}
	}

	return true;
}

bool CSNMPAirConditionController::AcquireCurrentHumidity(INT index,const wxString& IP)
{
	if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)
	{

		PROCESS_INFORMATION pi;
		STARTUPINFOA si;			
		ZeroMemory(&si,sizeof(STARTUPINFOA));
		si.cb = sizeof(STARTUPINFOA);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		wxString OID;
		FLOAT cof = 1.0;

		if(index == 1)
		{
			OID = wxT(".1.3.6.1.4.1.13400.2.324.2.2.2.0");
			cof = 0.1f;
		}
		else if(index == 2)
		{
			OID = wxT(".1.3.6.1.4.1.13400.2.324.2.2.3.0");
			cof = 0.01f;
		}

		wxFile cmdFile(wxT("airStatus.bat"),wxFile::write);
		if(cmdFile.IsOpened())
		{
			cmdFile.Write(wxString::Format(wxT("snmpget -v 2c -c public %s %s > airStatus.txt"),IP,OID));
		}
		cmdFile.Close();

		char cmdLine[] = {"airStatus.bat"};

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
			::WaitForSingleObject(pi.hProcess,3000);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}

		if(wxFileName::GetSize(wxT("airStatus.txt")) <= 0)
		{
			return false;
		}

		std::ifstream file("airStatus.txt");
		if(file.is_open())
		{
			while(!file.eof())
			{
				std::string content;
				std::getline(file,content,'\n');


				std::istringstream valueStream(content);

				std::string strOne;
				std::string strTwo;
				std::string strThree;
				std::string valueStr;

				if(valueStream >> strOne >> strTwo >>strThree >> valueStr)
				{
					m_AirStatus.currentHumidity = std::stoi(valueStr) * cof;
				}

			}
			file.close();	

		}
	}

	return true;
}

bool CSNMPAirConditionController::writeAirStatusToDataBase(UINT index)
{
	wxString sqlSentence = wxT("INSERT INTO airconditioninginfo (updateTime,atcNum,onOffStatus,setTemp,setHumidity,RATemp,RAHumi,status,alarm1,alarm2) VALUES");
	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{
		sqlSentence.Append(wxString::Format(wxT("(%I64d, "), wxDateTime::GetTimeNow()));
		sqlSentence.Append(wxString::Format(wxT("%d, "), index));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_AirStatus.onoffStatus));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_AirStatus.setTemperature));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_AirStatus.setHumidity));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_AirStatus.currentTemperature));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_AirStatus.currentHumidity));
		sqlSentence.Append(wxString::Format(wxT("%d, "), 0));
		sqlSentence.Append(wxString::Format(wxT("%d, "), 0));
		sqlSentence.Append(wxString::Format(wxT("%d)"), 0));

		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}
	}

	return true;
}

DWORD WINAPI CSNMPAirConditionController::QueryStatus(LPVOID para)
{
	CSNMPAirConditionController* self = static_cast<CSNMPAirConditionController*>(para);
	if(self)
	{
		
		while (!GetPlug()->GetExit())
		{

			if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)  //根据不同站台编号发送查询命令
			{
				self->AcquireStatus();
			}

			for(UINT index = 0;index < 60;index++)
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
