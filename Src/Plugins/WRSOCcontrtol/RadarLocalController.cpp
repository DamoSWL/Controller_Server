#include "RadarLocalController.h"




CRadarLocalController::CRadarLocalController(void):m_udpHandler(NULL),
	m_localPort(0),
	m_remotePort(0),
	m_newStatusFlag(false)
{
}


CRadarLocalController::~CRadarLocalController(void)
{
	stopReceive();
}


bool CRadarLocalController::startReceive(INT localPort)
{
    m_localPort = localPort;

    m_udpHandler = m_udpFactory.GetInstance();
    if(m_udpHandler)
    {
        m_udpHandler->RegHandle(this,CRadarLocalController::processData,CRadarLocalController::processEvent);
    }

    if (m_udpHandler->StartReceive(0, "0.0.0.0", m_localPort, nullptr, 0, 8192, 8192, 1000000))
	{
		return true;
	}
	return false;
}

bool CRadarLocalController::stopReceive()
{
	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();
		m_udpFactory.ReleaseInstance(m_udpHandler);
		m_udpHandler = NULL;
		return true;
	}

	return false;
	
}

bool CRadarLocalController::sendData(wxString& wxRadarCmd,UINT type,UINT data)
{
	wxRadarCmd = wxString::Format(wxT("#RADARCTL:Type=%d,Data=%d&"),type,data);
	std::string radarCmd(wxRadarCmd.mb_str(wxConvLocal));
	return m_udpHandler->SendData(radarCmd.c_str(),radarCmd.length(),m_remoteIP.c_str(),m_remotePort);
}

bool CRadarLocalController::isWorking()
{
	if(m_udpHandler)
	{
		return m_udpHandler->IsWorking();
	}
	return false;
}

void CRadarLocalController::setDst(std::string& remoteIP,INT remotePort)
{
	m_remoteIP = remoteIP;
	m_remotePort = remotePort;
}

int CRadarLocalController::processData(void *pHandle, const char *pData, UINT Len)
{
	
	CRadarLocalController* self = static_cast<CRadarLocalController*>(pHandle);
	if(self)
	{
		wxString statusCmd;

        statusCmd = wxString(pData,wxConvLocal,Len);
		self->appendRecvData(statusCmd);

		radarStatus radarStatus;
		
		auto index = statusCmd.Find(wxT("S2"));
		if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto remoteControlStr = wxString(statusCmd[index + 3]);
           remoteControlStr.ToULong((PULONG)&radarStatus.remoteControl);
        }
        
        index = statusCmd.Find(wxT("S3"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto powerSupplyStr = wxString(statusCmd[index + 3]);
           powerSupplyStr.ToULong((PULONG)&radarStatus.powerSupply);
        }

        index = statusCmd.Find(wxT("S4"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto RDAPowerSupplyStr = wxString(statusCmd[index + 3]);
           RDAPowerSupplyStr.ToULong((PULONG)&radarStatus.RDAPowerSupply);
        }

        index = statusCmd.Find(wxT("S5"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto recvPowerSupplyStr = wxString(statusCmd[index + 3]);
           recvPowerSupplyStr.ToULong((PULONG)&radarStatus.recvPowerSupply);
        }

        index = statusCmd.Find(wxT("S8"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto RDAComputerStatusStr = wxString(statusCmd[index + 3]);
           RDAComputerStatusStr.ToULong((PULONG)&radarStatus.RDAComputerStatus);
        }

        index = statusCmd.Find(wxT("S9"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto emitterLowPressureStr = wxString(statusCmd[index + 3]);
           emitterLowPressureStr.ToULong((PULONG)&radarStatus.emitterLowPressure);
        }

        index = statusCmd.Find(wxT("S10"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto emitterHighPressureStr = wxString(statusCmd[index + 4]);
           emitterHighPressureStr.ToULong((PULONG)&radarStatus.emitterHighPressure);
        }

        index = statusCmd.Find(wxT("S11"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto servoPowerStr = wxString(statusCmd[index + 4]);
           servoPowerStr.ToULong((PULONG)&radarStatus.servoPower);
        }

        index = statusCmd.Find(wxT("S13"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto emitterHeatStr = wxString(statusCmd[index + 4]);
           emitterHeatStr.ToULong((PULONG)&radarStatus.emitterHeat);
        }

        index = statusCmd.Find(wxT("S14"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
           auto RDAOperationStr = wxString(statusCmd[index + 4]);
           RDAOperationStr.ToULong((PULONG)&radarStatus.RDAOperation);
        }

        index = statusCmd.Find(wxT("S15"));
        if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
        }
        else
        {
			auto tmpIndex = statusCmd.find(wxT(','),index);
            radarStatus.VCP = statusCmd.Mid(index + 4,tmpIndex - index - 4);
        }

        index = statusCmd.Find(wxT("S16"));
        if(index == wxNOT_FOUND)
        {
            //return EXIT_FAILURE;
        }
        else
        {
			auto tmpIndex = statusCmd.find(wxT(','),index);
            radarStatus.SCHD = statusCmd.Mid(index + 4,tmpIndex - index - 4);
        }

		index = statusCmd.Find(wxT("S17"));
		if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
			auto tmpIndex = statusCmd.find(wxT(','),index);
			radarStatus.VCPS = statusCmd.Mid(index + 4,tmpIndex - index - 4);
		}

		index = statusCmd.Find(wxT("S18"));
		if(index == wxNOT_FOUND)
		{
			//return EXIT_FAILURE;
		}
		else
		{
			auto tmpIndex = statusCmd.find(wxT('&'),index);
			radarStatus.SCHDS = statusCmd.Mid(index + 4,tmpIndex - index - 4);
		}

        self->setRadarStatus(radarStatus);
        self->setRadarStatusFlag(true);
		return EXIT_SUCCESS;

	}
	
	return EXIT_FAILURE;
}

void CRadarLocalController::processEvent(void *pHandle, int Opt , char *pData,UINT Len)
{
    
}
