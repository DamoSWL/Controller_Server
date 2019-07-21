#pragma once

#include "AirHandler.h"
class CNetService;

//负责空调控制
class CAirConditionerController
{
public:
	CAirConditionerController();
	~CAirConditionerController();

public:
	static int processData(void *pHandle, const char *pData, UINT Len); //处理空调控制器返回的数据
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT uiLen = 0);

	BOOL startReceive();
	BOOL stopReceive();
	bool sendDataToAirConditioner(const char* cmd,UINT len,const char* dstIP,UINT port);
	
	void setairConditionFlag(bool flag) {m_airConditionCmdFlag = flag;}
	bool getairConditonFlag(){return m_airConditionCmdFlag;}

	void setNetService(CNetService* localNetService) {m_localNetService = localNetService;}
	CNetService* getNetService() {return m_localNetService;}

	void parseAirCmd(const char *pData, UINT Len);  //解析java段发送的空调命令


private:
	ISocketNet* m_udpHandler;
	CNetService* m_localNetService;

	CAirHandler m_airHandler;  //该类负责解析命令，并组建空调命令帧
	bool m_airConditionCmdFlag;  //标志位，判断是否收到空调控制器回复


};

