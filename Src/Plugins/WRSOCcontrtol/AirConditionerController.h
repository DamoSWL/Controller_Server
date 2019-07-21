#pragma once

#include "AirHandler.h"
class CNetService;

//����յ�����
class CAirConditionerController
{
public:
	CAirConditionerController();
	~CAirConditionerController();

public:
	static int processData(void *pHandle, const char *pData, UINT Len); //����յ����������ص�����
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT uiLen = 0);

	BOOL startReceive();
	BOOL stopReceive();
	bool sendDataToAirConditioner(const char* cmd,UINT len,const char* dstIP,UINT port);
	
	void setairConditionFlag(bool flag) {m_airConditionCmdFlag = flag;}
	bool getairConditonFlag(){return m_airConditionCmdFlag;}

	void setNetService(CNetService* localNetService) {m_localNetService = localNetService;}
	CNetService* getNetService() {return m_localNetService;}

	void parseAirCmd(const char *pData, UINT Len);  //����java�η��͵Ŀյ�����


private:
	ISocketNet* m_udpHandler;
	CNetService* m_localNetService;

	CAirHandler m_airHandler;  //���ฺ�����������齨�յ�����֡
	bool m_airConditionCmdFlag;  //��־λ���ж��Ƿ��յ��յ��������ظ�


};

