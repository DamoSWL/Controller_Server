#pragma once


#include "UPSAlarm.h"
#include "EnvAlarm.h"
#include <wx/timer.h>
#include "EnvironmentData.h"



class CRadarRunEnvironment
{

public:
	static DWORD WINAPI EnvironmentExec(LPVOID para);

public:
	CRadarRunEnvironment();
	virtual ~CRadarRunEnvironment();

// 	BOOL startCom();
// 	BOOL stopCom();

	//void insertUPSData(const UPSHandler::tagUPSStatus& data);

	void addDisconnectionA();
	void addDisconnectionB();

	void setUpdateTime() {m_updateTime = ::wxDateTime::GetTimeNow();}
	DWORD getEnvFromSensorA() {return m_envDataAcquisition.getEnvFromSensorA(m_envDataA);}
	DWORD getEnvFromSensorB() {return m_envDataAcquisition.getEnvFromSensorB(m_envDataB);}

	void setEnvADataFlag(bool flag) {m_newEnvADataFlag = flag;}
	void setEnvBDataFlag(bool flag) {m_newEnvBDataFlag = flag;}
	void resetSensorADisconnectCnt() {m_sensorADisconnectedCount = 0;}
	void resetSensorBDisconnectCnt() {m_sensorBDisconnectedCount = 0;}

	void processEnvData();

private:
	bool writeToDataBase();
	
private:
	CEnvironmentData m_envDataAcquisition;
	//UPSHandler m_UPSController;

	UPSHandlerSNMP::tagUPSStatus m_upsData;
	std::vector<FLOAT> m_envDataA;
	std::vector<FLOAT> m_envDataB;

	wxTimer m_CmdTimer;
	wxTimer m_WriteTimer;

	bool m_newEnvADataFlag;
	bool m_newEnvBDataFlag;
	bool m_newUPSDataFlag;

	UINT m_updateTime;

	CUPSAlarm m_UPSAlarm;
	CEnvAlarm m_envAlarm;

	UINT m_sensorADisconnectedCount;
	UINT m_sensorBDisconnectedCount;

	UINT m_UPSDisconnectedCount;

};

