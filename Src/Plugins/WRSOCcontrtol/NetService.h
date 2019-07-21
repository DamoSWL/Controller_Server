#pragma once

#include "AirConditionerController.h"
#include "RadarController.h"
#include "SpecAirConditionController.h"
#include "SNMPAirConditionController.h"

//该类负责与java端进行通信
class CNetService
{
public:
	CNetService();
	~CNetService();

public:
	BOOL startReceive();
	BOOL stopReceive();
	bool sendDataToFront(const char* cmd,UINT len,const char* dstIP,UINT port);  //将数据发送给java后端
	bool sendDataToAirConditioner(const char* cmd,UINT len,const char* dstIP,UINT port);
	bool sendDataToRadar(const char* cmd,UINT len,const char* dstIP,UINT port);

	void setAirConditionerCmdFlag(bool flag) {m_AirConditionerController.setairConditionFlag(true);}
	

	static int processData(void *pHandle, const char *pData, UINT Len);  //处理从java后端发送的命令
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT Len = 0);

	BOOL reStartReceive() {return m_radarController.reStartReceive();}

	bool is_str_gbk(const char* str);



private:
	ISocketNet* m_udpHandler;
	CRadarController m_radarController;
	HANDLE m_SNMPAirHandler;

public:
	CAirConditionerController m_AirConditionerController;
	CSpecAirConditionController m_SpecAirConditionerController;
	CSNMPAirConditionController m_SNMPAirConditionerController;
};

