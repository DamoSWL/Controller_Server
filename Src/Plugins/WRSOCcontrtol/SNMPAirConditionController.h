#pragma once


//该类使用SNMP协议控制精密空调

class CSNMPAirConditionController
{
public:
	CSNMPAirConditionController(void);
	~CSNMPAirConditionController(void);


public:
	typedef struct   //精密空调返回状态
	{
		INT onoffStatus;
		FLOAT setTemperature;
		FLOAT setHumidity;
		FLOAT currentTemperature;
		FLOAT currentHumidity;
	}AirStatus;

public:
	bool parseAirCmd(const char *pData, UINT Len);   //从java后台获取到空调控制指令后，使用该函数解析指令
	bool sendSwitchCmd(INT flag);  //发送开关机命令
	bool sendTemperatureCmd(INT temperature);  //发送温度控制命令
	bool sendHumidityCmd(INT humidity);   //发送湿度控制命令

	bool AcquireStatus();  //获取空调状态信息

private:
	bool AcquireStatusByIP(INT index,const wxString& IP);  //当有多个精密空调时，通过IP地址来获取状态信息
	bool AcquireOnOffStatus(const wxString& IP);   //获取开关机状态
	bool AcquireSetTemperature(const wxString& IP);  //获取温度状态
	bool AcquireSetHumidity(const wxString& IP);   //获取湿度状态
	bool AcquireCurrentTemperature(INT index, const wxString& IP);  //获取室内温度
	bool AcquireCurrentHumidity(INT index, const wxString& IP);   //获取室内湿度

	bool writeAirStatusToDataBase(UINT index);  //将状态信息写入数据库中



public: 
	static DWORD WINAPI QueryStatus(LPVOID para);  //线程入口函数，在函数中每隔一段时间发送查询命令


private:
	INT m_currentDeviceIndex;  //当有多个精密空调时，区分当前活动空调
	AirStatus m_AirStatus;
};

