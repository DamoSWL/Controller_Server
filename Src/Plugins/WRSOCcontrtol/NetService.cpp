
#include "NetService.h"


CNetService::CNetService():m_udpHandler(NULL)
{

}


CNetService::~CNetService()
{
}

BOOL CNetService::startReceive()
{

	m_udpHandler = GetPlug()->m_UDPFactory.GetInstance();

	if(m_udpHandler)
	{
		m_udpHandler->RegHandle(this, CNetService::processData,CNetService::processEvent);

		if (m_udpHandler->StartReceive(0, "0.0.0.0", 9797, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"端口9797绑定成功");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"端口9797绑定失败");
			return FALSE;
		}
	}

	m_AirConditionerController.startReceive();
	m_AirConditionerController.setNetService(this);
	
	m_radarController.startReceive();
	m_radarController.setNetService(this);

	if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))
	{
		m_SpecAirConditionerController.startReceive();
		m_SpecAirConditionerController.setNetService(this);
		m_SpecAirConditionerController.startTimer();
	}
	else if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)
	{
		m_SNMPAirHandler = CreateThread(nullptr,0,CSNMPAirConditionController::QueryStatus,&m_SNMPAirConditionerController,0,nullptr);
	}
	else{}



	return TRUE;
}

BOOL CNetService::stopReceive()
{
	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();
	
		GetPlug()->m_UDPFactory.ReleaseInstance(m_udpHandler);
		m_udpHandler = NULL;

		m_AirConditionerController.stopReceive();
		m_radarController.stopReceive();

		m_SpecAirConditionerController.stopTImer();
		m_AirConditionerController.stopReceive();

		if(WaitForSingleObject(m_SNMPAirHandler,INFINITE) == WAIT_OBJECT_0)
		{
			CloseHandle(m_SNMPAirHandler);
		}

		return TRUE;
	}



	return FALSE;
}

bool CNetService::sendDataToFront(const char* cmd,UINT len,const char* dstIP,UINT port)
{
	GetPlug()->AddLog(LOG_TYPE_MESSAGE,cmd);
	if(m_udpHandler->IsWorking())
	{
		return m_udpHandler->SendData(cmd,len,dstIP,port);
	}
	return false;
	
}

bool CNetService::sendDataToAirConditioner(const char* cmd,UINT len,const char* dstIP,UINT port)
{
	GetPlug()->AddLog(LOG_TYPE_MESSAGE,cmd);
	if(m_udpHandler->IsWorking())
	{
		return m_AirConditionerController.sendDataToAirConditioner(cmd,len,dstIP,port);
	}
	return false;
	
}

bool CNetService::sendDataToRadar(const char* cmd,UINT len,const char* dstIP,UINT port)
{
	GetPlug()->AddLog(LOG_TYPE_MESSAGE,cmd);
	if(m_udpHandler->IsWorking())
	{
		return m_radarController.sendDataToRadar(cmd,len,dstIP,port);
	}
	return false;
}

int CNetService::processData(void *pHandle, const char *pData, UINT Len)
{
	CNetService* self = static_cast<CNetService*>(pHandle);
	if(self)
	{
		std::string message(pData,Len);
		GetPlug()->AddLog(LOG_TYPE_MESSAGE,message.c_str());

		if(std::memcmp("cdywConnect",pData,Len) == 0)
		{
			char responseCmd[] = "cdywOK";
			auto dstIP = GetPlug()->m_ClientIPAddress;
			self->sendDataToFront(responseCmd,std::strlen(responseCmd),dstIP.c_str(),9898);
			
		}
		else if((std::memcmp("#ATCTL",pData,6) == 0))  //接受普通空调命令
		{
			self->m_AirConditionerController.parseAirCmd(pData,Len);
			self->setAirConditionerCmdFlag(true);
		}
		else if((std::memcmp("#PATCTL",pData,7) == 0))  //接受精密空调命令
		{
			if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))
			{
				self->m_SpecAirConditionerController.parseAirCmd(pData,Len);
			}
			else if(GetPlug()->m_Radarcd.Find(wxT("Z9898")) != wxNOT_FOUND)
			{
				self->m_SNMPAirConditionerController.parseAirCmd(pData,Len);
				self->sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);
			}
			else{}
			
		}
  		else if(std::memcmp("#RADARCTL:",pData,10) == 0)  //接受雷达控制命令
  		{
  			if(self->sendDataToRadar(pData,Len,GetPlug()->getConfig()->NetConfig.Radar.ip,GetPlug()->getConfig()->NetConfig.Radar.port))
  			{
  				self->sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);
  			}
  			else
  			{
  				self->sendDataToFront("FAIL",4,GetPlug()->m_ClientIPAddress.c_str(),9898);
  			}
  			
  		}
  		else if(std::memcmp("SMS:END",pData,Len) == 0)  //接受发送短信命令
  		{
  			GetPlug()->sendSMSText();	
  		}
  		else if(std::memcmp("SMS:",pData,4) == 0)  //接受短信联系人信息和短信内容
  		{	
  			GetPlug()->addSMSData(std::string(pData+4,Len-4));
  			self->sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);
  		}
  		else
  		{
  			self->sendDataToFront("INVALID",7,GetPlug()->m_ClientIPAddress.c_str(),9898);
  		}

		return EXIT_SUCCESS;

	}
	return EXIT_FAILURE;
}

void CNetService::processEvent(void *pHandle, int Opt, char *pData /*= nullptr*/, UINT Len /*= 0*/)
{

}

bool CNetService::is_str_gbk(const char* str)
{
	unsigned int nBytes = 0;//GBK可用1-2个字节编码,中文两个 ,英文一个
	unsigned char chr = *str;
	bool bAllAscii = true; //如果全部都是ASCII,
	for (unsigned int i = 0; str[i] != '\0'; ++i){
		chr = *(str + i);
		if ((chr & 0x80) != 0 && nBytes == 0){// 判断是否ASCII编码,如果不是,说明有可能是GBK
			bAllAscii = false;
		}
		if (nBytes == 0) {
			if (chr >= 0x80) {
				if (chr >= 0x81 && chr <= 0xFE){
					nBytes = +2;
				}
				else{
					return false;
				}
				nBytes--;
			}
		}
		else{
			if (chr < 0x40 || chr>0xFE){
				return false;
			}
			nBytes--;
		}//else end
	}
	if (nBytes != 0) {   //违返规则
		return false;
	}
	if (bAllAscii){ //如果全部都是ASCII, 也是GBK
		return true;
	}
	return true;

}
