#include "RadarController.h"
#include "mysqldatabase.h"


static const int REQUESTTIMERID = 140;

BEGIN_EVENT_TABLE(CRadarController,wxEvtHandler)
	EVT_TIMER(REQUESTTIMERID,CRadarController::onTimeout)
END_EVENT_TABLE()

CRadarController::CRadarController(void):m_udpHandler(NULL),
	m_localNetService(NULL),
	m_Timer(this,REQUESTTIMERID),
	m_DisconnectCount(0),
	m_radarConnectionPingFlag(TRUE),
	m_radarNOCmdFlag(FALSE)
{
}


CRadarController::~CRadarController(void)
{
}

int CRadarController::processData(void *pHandle, const char *pData, UINT Len)
{
	CRadarController* self = static_cast<CRadarController*>(pHandle);
	if(self)
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE,pData);
		if(std::memcmp(pData,"#RADA",5) == 0)
		{
			wxString currentStatus(pData,wxConvLocal,Len);
			{
				if(!self->cmdCompare(currentStatus))  //接收到雷达状态信息后，如果不同则解析并存入数据库
				{
					std::map<wxString,wxString> statusMap;
					self->setStatusStr(currentStatus);
					if(self->parseStatusStr(statusMap) == EXIT_SUCCESS)
					{
						self->writeToDataBase(statusMap);
					}

				}
			}
		}

	}

	return 0;

}

void CRadarController::processEvent(void *pHandle, int Opt, char *pData /*= nullptr*/, UINT uiLen /*= 0*/)
{

}

BOOL CRadarController::startReceive()
{
	m_udpHandler = GetPlug()->m_UDPFactory.GetInstance();

	if(m_udpHandler)
	{
		m_udpHandler->RegHandle(this, CRadarController::processData,CRadarController::processEvent);

		if (m_udpHandler->StartReceive(0, "0.0.0.0", GetPlug()->getConfig()->NetConfig.Local.port, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"雷达控制端口绑定成功");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"雷达控制端口绑定失败");
			return FALSE;
		}
	}

	m_Timer.Start(3*1000);

	return TRUE;
}

BOOL CRadarController::stopReceive()
{
	m_Timer.Stop();
	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();
		GetPlug()->m_UDPFactory.ReleaseInstance(m_udpHandler);
		m_udpHandler = NULL;
		return TRUE;
	}

	return FALSE;
}

bool CRadarController::sendDataToRadar(const char* cmd,UINT len,const char* dstIP,UINT port)
{
	if(m_udpHandler->IsWorking())
	{
		return m_udpHandler->SendData(cmd,len,dstIP,port);
	}
	return false;
}

INT CRadarController::parseStatusStr(std::map<wxString,wxString>& statusMap)
{
	auto prefixIndex =  m_previousStatus.Find(wxT(":")) + 1;
	auto previousStatus = m_previousStatus.Mid(prefixIndex ,m_previousStatus.Length() - prefixIndex - 1);
	previousStatus.Append(wxT(","));

	INT previousCommaIndex = 0;
	INT currentCommaIndex = 0;
	while((currentCommaIndex = previousStatus.find(wxT(","),previousCommaIndex)) != wxNOT_FOUND)
	{
		auto tmpStr = previousStatus.Mid(previousCommaIndex,currentCommaIndex - previousCommaIndex);
		INT equalIndex = 0;
		if((equalIndex = tmpStr.Find(wxT("="))) != wxNOT_FOUND)
		{
			statusMap[tmpStr.Left(equalIndex)] = tmpStr.Right(tmpStr.Length() - equalIndex - 1);
		}
		previousCommaIndex = currentCommaIndex + 1;
	}

	return EXIT_SUCCESS;
}

bool CRadarController::writeToDataBase(const std::map<wxString,wxString>& status)
{
	wxString sqlSentence;
	auto pDataBase = GetPlug()->getDatabase();


	if(pDataBase)
	{

			auto radarType = GetPlug()->getRadarTypeStr();


			for(auto item = status.cbegin(); item != status.cend(); item++)
			{
				sqlSentence.Printf(wxT("UPDATE radarcontrolinfo SET `value`='%s' WHERE `status`='%s' AND radarType='%s'"),item->second,item->first,radarType);
				if(!pDataBase->RunSQLNoQuery(sqlSentence))
				{
					break;
				}

			}
			return true;
	}


	return false;
}

BOOL CRadarController::reStartReceive()
{
	m_Timer.Stop();

	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();

		if (m_udpHandler->StartReceive(0, "0.0.0.0", GetPlug()->getConfig()->NetConfig.Local.port, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"雷达控制端口绑定成功");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"雷达控制端口绑定失败");
			return FALSE;
		}
	}

	m_Timer.Start(3*1000);

	return TRUE;
}

void CRadarController::onTimeout(wxTimerEvent& event)
{
	sendRequest();  //发送状态查询命令
}

bool CRadarController::sendRequest()
{
	auto result = GetPlug()->PingRadar();  //通过ping命令查询连接状态
	if(result == 1)
	{
		if(!m_radarConnectionPingFlag)  //若前一次ping不通，这次ping通后则将连接状态设为正常
		{
			m_radarConnectionPingFlag = TRUE;
			writePingToDataBase(result);

			m_DisconnectCount = 0;
			m_radarNOCmdFlag = FALSE;
		}
		else     
		{
			m_DisconnectCount++;
			if(m_DisconnectCount >= 10)  
			{
				if(!m_radarNOCmdFlag)  //在ping通的情况下，如果m_DisconnectCount大于阈值，表明没有收到状态返回值，则依然把连接状态设置为异常
				{
					m_radarNOCmdFlag = TRUE;
					writePingToDataBase(0);
				}

				if(m_DisconnectCount >= 1e6)
				{
					m_DisconnectCount = 10;
				}
			}
			else
			{
				if(m_radarNOCmdFlag)  //连续几次ping通后，m_DisconnectCount没有大于阈值，则表明接受到了状态信息，把连接状态设置为正常
				{
					m_radarNOCmdFlag = FALSE;
					writePingToDataBase(1);
				}
			}
		}
	
	}
	else
	{
		if(m_radarConnectionPingFlag) //若连接不通则写入相应状态
		{
			m_radarConnectionPingFlag = FALSE;
			writePingToDataBase(0);
			writeRemoteCtlToDataBase(0);
		}

	}
	

	char* cmd = "#RADARCTL:Type=1,Data=1&";  //发送状态查询命令

	if(m_udpHandler->IsWorking())
	{
		return m_udpHandler->SendData(cmd,std::strlen(cmd),GetPlug()->getConfig()->NetConfig.Radar.ip,GetPlug()->getConfig()->NetConfig.Radar.port);
	}

	return false;

}

bool CRadarController::writePingToDataBase(INT result)
{
	wxString sqlSentence;
	auto pDataBase = GetPlug()->getDatabase();	

	if(pDataBase)
	{

			auto radarType = GetPlug()->getRadarTypeStr();

			sqlSentence.Printf(wxT("UPDATE radarcontrolinfo SET `value`='%d' WHERE eName='InternetConnect' AND radarType='%s'"),result,radarType);
				
			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}
	}

	return true;
}

bool CRadarController::writeRemoteCtlToDataBase(INT result)
{
	wxString sqlSentence;
	auto pDataBase = GetPlug()->getDatabase();


	if(pDataBase)
	{
			auto radarType = GetPlug()->getRadarTypeStr();

			sqlSentence.Printf(wxT("UPDATE radarcontrolinfo SET `value`='%d' WHERE eName='remoteControl' AND radarType='%s'"),result,radarType);

			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}
	}

	return true;
}
