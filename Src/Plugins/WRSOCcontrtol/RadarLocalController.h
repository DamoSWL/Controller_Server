#pragma once

#include "UDP.h"


class CRadarLocalController 
{
public:
	CRadarLocalController(void);
	~CRadarLocalController(void);

public:
	typedef struct _radarStatus
	{
		UINT remoteControl;
		UINT powerSupply;
		UINT RDAPowerSupply;
		UINT recvPowerSupply;
		UINT RDAComputerStatus;
		UINT emitterLowPressure;
		UINT emitterHighPressure;
		UINT servoPower;
		UINT emitterHeat;
		UINT RDAOperation;
		wxString VCP;
		wxString SCHD;
		wxString VCPS;
		wxString SCHDS;
	}radarStatus;

public:
	bool startReceive(INT localPort);
	bool stopReceive();
	bool sendData(wxString& wxRadarCmd,UINT type,UINT data);
	bool isWorking();
	void setDst(std::string& remoteIP,INT remotePort);

	void setRadarStatus(const CRadarLocalController::radarStatus& status) {m_radarStatus = status;}
	CRadarLocalController::radarStatus getRadarStatus() {return m_radarStatus;}
	void setRadarStatusFlag(bool flag) {m_newStatusFlag = flag;}
	bool getRadarStatusFlag() {return m_newStatusFlag;}

	std::vector<wxString>& getRecvData() {return m_recvData;}
	void appendRecvData(const wxString& data) {m_recvData.push_back(data);}
	void clearRecvData() {m_recvData.clear();}

	static int processData(void *pHandle, const char *pData, UINT Len);
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT Len = 0);



private:
	CUDP m_udpFactory;
	ISocketNet* m_udpHandler;

	INT m_localPort;
	std::string m_remoteIP;
	INT m_remotePort;

	radarStatus m_radarStatus;
	std::vector<wxString> m_recvData;
	bool m_newStatusFlag;



	

	
};

