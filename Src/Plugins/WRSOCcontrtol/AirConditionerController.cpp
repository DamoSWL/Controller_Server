
#include "AirConditionerController.h"
#include "NetService.h"
#include "ywsocketdefine.h"

CAirConditionerController::CAirConditionerController() :m_udpHandler(NULL),
															m_localNetService(NULL),
															m_airConditionCmdFlag(false)
											
{
														
}



CAirConditionerController::~CAirConditionerController()
{

}


int CAirConditionerController::processData(void * pHandle, const char * pData, UINT Len)
{
	CAirConditionerController* self = static_cast<CAirConditionerController*>(pHandle);
	if(self)
	{
		if(Len >= 1)
		{
			if(((UCHAR)pData[0] == 0x90))
			{

				self->setairConditionFlag(false);  //复位标志位

				CNetService* localNetService = self->getNetService();
				if(localNetService)
				{
					localNetService->sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);  //向java段发送success
				}

			}
			else{}

		}
		else if(Len == 7)
		{
			if(((UCHAR)pData[3] == 0x45) && ((UCHAR)pData[4] == 0x52))
			{
				self->setairConditionFlag(false);

				CNetService* localNetService = self->getNetService();
				if(localNetService)
				{
					localNetService->sendDataToFront("FAIL",4,GetPlug()->m_ClientIPAddress.c_str(),9898);  //向java段发送fail
				}

			}
			else{}
		}
		else{}

 		//if(Len == 1)
 		//{
 		//	if((UCHAR)pData[0] == 0x89)
 		//	{
 		//		self->setairConditionFlag(false);
 		//		CNetService* localNetService = self->getNetService();
 		//		if(localNetService)
 		//		{
 		//		 	localNetService->sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);
 		//		}
 
 		//	}
 		//	else if((UCHAR)pData[0] == 0xE0)
 		//	{
 		//		self->setairConditionFlag(false);			 
 		//		CNetService* localNetService = self->getNetService();
 		//		if(localNetService)
 		//		{
 		//		 	localNetService->sendDataToFront("FAIL",4,GetPlug()->m_ClientIPAddress.c_str(),9898);
 		//		}
 		//	}
 		//	else{}
 
 		//}
	}


	return 0;
}

void CAirConditionerController::processEvent(void * pHandle, int Opt, char * pData, UINT uiLen)
{

	CAirConditionerController* self = static_cast<CAirConditionerController*>(pHandle);

	if(self)
	{
		switch(Opt)
		{
		case YW_EVENT_RECEIVE_THREAD_TIMEOUT:  
			if(self->getairConditonFlag())
			{
				self->setairConditionFlag(false);

				CNetService* localNetService = self->getNetService();
				if(localNetService)
				{
					localNetService->sendDataToFront("FAIL",4,GetPlug()->m_ClientIPAddress.c_str(),9898);  //超时则发送fail
				}					
			}				
			break;
		default:
			break;

		}
	}
}

BOOL CAirConditionerController::startReceive()
{
	m_udpHandler = GetPlug()->m_UDPFactory.GetInstance();
	
	if(m_udpHandler)
	{
		m_udpHandler->RegHandle(this, CAirConditionerController::processData,CAirConditionerController::processEvent);
	
		if (m_udpHandler->StartReceive(0, "0.0.0.0", 7777, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"端口7777绑定成功");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"端口7777绑定失败");
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CAirConditionerController::stopReceive()
{
	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();
		GetPlug()->m_UDPFactory.ReleaseInstance(m_udpHandler);
		m_udpHandler = NULL;
		return TRUE;
	}

	return FALSE;
}

bool CAirConditionerController::sendDataToAirConditioner(const char* cmd,UINT len,const char* dstIP,UINT port)
{
	if(m_udpHandler->IsWorking())
	{
		return m_udpHandler->SendData(cmd,len,dstIP,port);
	}
	return false;

}

void CAirConditionerController::parseAirCmd(const char *pData, UINT Len)
{

	INT userData = 0;
	auto  cmdType =  m_airHandler.parseAirCmd(pData,Len,&userData);  //解析空调命令

	std::string airCmd; 
	switch(cmdType)   //根据命令发送对应的开关等命令帧
	{
	case CAirHandler::SWITCHON:
		airCmd = m_airHandler.fabricateSwitchOnCmd();
		break;
	case CAirHandler::SWITCHOFF:
		airCmd = m_airHandler.fabricateSwitchOffCmd();
		break;
	case CAirHandler::MODE:
		airCmd = m_airHandler.fabricateModeCmd(userData);
		break;
	case CAirHandler::TEMPERATURE:
		airCmd = m_airHandler.fabricateTemperatureCmd(userData);
		break;
	case CAirHandler::WINDDIRECTION:
		airCmd = m_airHandler.fabricateDirectionCmd(userData);
		break;
	case CAirHandler::WINDSPEED:
		airCmd = m_airHandler.fabricateSpeedCmd(userData);
		break;
	default:
		break;
	}


	sendDataToAirConditioner(airCmd.c_str(),airCmd.length(),GetPlug()->getConfig()->NetConfig.Env.ip,GetPlug()->getConfig()->NetConfig.Env.port);


	

}

