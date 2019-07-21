#include "AirHandler.h"


CAirHandler::CAirHandler(void):m_currentChannel(0)
{
	m_ModeValue[1] = 1;
	m_ModeValue[2] = 1;
	m_ModeValue[3] = 1;
	m_ModeValue[4] = 1;

	m_TemperatureValue[1] = 20;
	m_TemperatureValue[2] = 20;
	m_TemperatureValue[3] = 20;
	m_TemperatureValue[4] = 20;

	m_SpeedValue[1] = 1;
	m_SpeedValue[2] = 1;
	m_SpeedValue[3] = 1;
	m_SpeedValue[4] = 1;

	m_DirectionValue[1] = 1;
	m_DirectionValue[2] = 1;
	m_DirectionValue[3] = 1;
	m_DirectionValue[4] = 1;


}


CAirHandler::~CAirHandler(void)
{
}

INT CAirHandler::parseAirCmd(const char *pData, UINT Len,INT* pUserData)
{
	std::string cmd(pData,Len);
	INT pos = 0;
	INT cmdFlag = -1;


	if(cmd.find(":") != std::string::npos)
	{
		m_currentChannel = std::stoi(cmd.substr(std::strlen("#ATCTL:"),1));  //获取当前需要控制的设备编号
	}
	else
	{
		return cmdFlag;
	}

	if(cmd.find("Power") != std::string::npos)  //判断命令
	{
		pos = cmd.find("=") + 1;
		auto value = cmd.substr(pos,cmd.length() - 1 - pos);
		if(value == "1")
		{
			cmdFlag = CAirHandler::SWITCHON;
		}
		else if(value == "0")
		{
			cmdFlag = CAirHandler::SWITCHOFF;
		}

	}
	else if(cmd.find("Mode") != std::string::npos)
	{
		pos = cmd.find("=") + 1;;
		auto value = cmd.substr(pos,cmd.length() - 1 - pos);

		cmdFlag = CAirHandler::MODE;
		*pUserData = std::stoi(value);

		m_ModeValue[m_currentChannel] = *pUserData;

	}
	else if(cmd.find("Temp") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto value = cmd.substr(pos,cmd.length() - 1 - pos);

		cmdFlag = CAirHandler::TEMPERATURE;
		*pUserData = std::stoi(value);

		m_TemperatureValue[m_currentChannel] = *pUserData;
	}
	else if(cmd.find("V=") != std::string::npos)
	{
		pos = cmd.find("V=") + 2;
		auto commaPos = cmd.find(",",pos);
		if(commaPos == std::string::npos)
		{
			commaPos = cmd.length() - 1;
		}

		auto value = cmd.substr(pos,commaPos - pos);

		cmdFlag = CAirHandler::WINDDIRECTION;
		*pUserData = std::stoi(value);
	}
	else if(cmd.find("Speed") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto value = cmd.substr(pos,cmd.length() - 1 - pos);

		cmdFlag = CAirHandler::WINDSPEED;
		*pUserData = std::stoi(value);
	}
	else{}



	return cmdFlag;
}

std::string CAirHandler::fabricateSwitchOnCmd()
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress();
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = 0xFF;

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateSwitchOffCmd()
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress();
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = 0;

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateModeCmd(INT param)
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 1;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = param;

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateTemperatureCmd(INT param)
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 2;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = param;

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateSpeedCmd(INT param)
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 3;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = param;

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateDirectionCmd(INT param)
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 4;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = param;

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateModeCmdForOpen()
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 1;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = m_ModeValue[m_currentChannel];

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateTemperatureCmdForOpen()
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 2;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = m_TemperatureValue[m_currentChannel];

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateSpeedCmdForOpen()
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 3;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = m_SpeedValue[m_currentChannel];

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

std::string CAirHandler::fabricateDirectionCmdForOpen()
{
	UCHAR reponseCmd[11] = {'\x0'};
	reponseCmd[0] = 0x90;
	reponseCmd[1] = 0x43;
	reponseCmd[2] = 0;
	reponseCmd[3] = getSwitchRegisterAddress() + 4;
	reponseCmd[4] = 0;
	reponseCmd[5] = 0x01;
	reponseCmd[6] = 0x02;
	reponseCmd[7] = 0;
	reponseCmd[8] = m_DirectionValue[m_currentChannel];

	CRCData CRCCheckData;
	CRCCheckData.crcData = GetPlug()->chkcrc(reponseCmd,9);

	reponseCmd[9] = CRCCheckData.crcByte[1];
	reponseCmd[10] = CRCCheckData.crcByte[0];

	return std::string((PCHAR)reponseCmd,11);
}

UCHAR CAirHandler::getSwitchRegisterAddress()
{
	UCHAR address = 0;

	switch(GetPlug()->m_ordinaryAirConfiguration[m_currentChannel])
	{
	case 1:
		address = 0x11;
		break;
	case 2:
		address = 0x21;
		break;
	case 3:
		address = 0x31;
		break;
	case 4:
		address = 0x41;
		break;
	default:
		break;
	}

	return address;
}
