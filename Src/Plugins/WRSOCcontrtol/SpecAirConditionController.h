#pragma once

class CNetService;


//该类用于处理精密空调，发送空调指令并处理空调返回的状态
class CSpecAirConditionController : public wxEvtHandler
{

public:
	CSpecAirConditionController(void);
	~CSpecAirConditionController(void);

public:
	typedef struct _SpecAirStatus  //精密空调状态返回结构体定义，每个精密空调或许不同
	{
		INT switchStatus;
		FLOAT temperature;
		FLOAT humidity;
		FLOAT windTemperature;
		FLOAT windHumidity;

		union 
		{
			WORD status;
			struct  
			{
				WORD statusBit : 1;			
				WORD windBit : 1;			
				WORD heatBit : 1;			
				WORD freezeBit : 1;			
				WORD humidityUpBit : 1;			
				WORD humidityDownBit : 1;			
				WORD otherBit1 : 1;			
				WORD otherBit2 : 1;		

				WORD otherBits : 8;
			};
		};

		union 
		{
			WORD alarmInfoOne;
			struct  
			{
				WORD otherBit3 : 1;		
				WORD ohterBit4 : 1;		
				WORD Outdoor_temp_probe_alarmBit : 1;			
				WORD Indoor_humidity_probe_alarmBit : 1;			
				WORD Supply_air_temp_probe_alarmBit : 1;			
				WORD Indoor_temp_probe_alarmBit : 1;			
				WORD Low_humidity_aLarmBit : 1;		
				WORD High_humidity_aLarmBit : 1;			

				WORD Low_temp_alarmBit : 1;			
				WORD High_temp_alarmBit : 1;			
				WORD Air_filter_alarmBit : 1;			
				WORD Heater_overloadBit : 1;		
				WORD Fan_overloadBit : 1;			
				WORD Airflow_alarmBit : 1;		
				WORD Compressor_low_pressBit : 1;			
				WORD Compressor_high_pressBit : 1;			
			};
		};

		union 
		{
			WORD alarmInfoTwo;
			struct  
			{
				WORD otherBit5 : 1;		
				WORD Compressor_low_pressBit : 1;		
				WORD Compressor_high_pressBit : 1;			
				WORD Power_alarmBit : 1;			
				WORD ohterBit6 : 1;			
				WORD Condenser_fan_1_alarmBit : 1;			
				WORD Condenser_fan_2_alarmBit : 1;		
				WORD Smoke_alarmBit : 1;			

				WORD User_alarmBit : 1;			
				WORD Waterleak_alarmBit : 1;			
				WORD otherBit7: 1;			
				WORD otherBit8 : 1;		
				WORD Phase_alarmBit : 1;			
				WORD Humidifier_no_currentBit : 1;		
				WORD Humidifier_no_waterBit : 1;			
				WORD Humidifier_high_currentBit : 1;			
			};
		};
	}SpecAirStatus;

public:

	static int processData(void *pHandle, const char *pData, UINT Len);  //回调函数，处理精密空调返回的信息
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT uiLen = 0);

	BOOL startReceive();  //打开网络端口
	BOOL stopReceive();   //关闭网络端口

	void startTimer() {m_Timer.Start(60*1000);}  //定时器函数，用于定时发送空调状态查询指令
	void stopTImer() {m_Timer.Stop();}

	void parseAirCmd(const char *pData, UINT Len);  //从java后台获取到空调控制指令后，使用该函数解析指令
	void parseAirStatus(const char *pData, UINT Len);  //解析从空调返回的状态信息

	void setNetService(CNetService* localNetService) {m_localNetService = localNetService;}  //获取到CNetService类的指针，用于向java端发送命令
	CNetService* getNetService() {return m_localNetService;}

	void setairConditionFlag(bool flag) {m_airConditionCmdFlag = flag;}  //标识位，用于判断是否接受到空调回复
	bool getairConditonFlag(){return m_airConditionCmdFlag;}

	void onAirStatusTimeout(wxTimerEvent& event);  //定时器函数，定时发送指令



private:
	bool sendSwitchCmd(INT flag);  //发送开关命令
	bool sendTemperatureCmd(INT temperature);  //控制温度
	bool sendHumidityCmd(INT humidity);  // 控制湿度
	bool writeToDataBase(UCHAR id);  //将状态信息写入数据库中

	std::string getStatusStr();     //这三个函数将获取的状态信息组成字符串，写入数据库中，每个空调组成方式或许不一样
	std::string getAlarmStatusOneStr();
	std::string getAlarmStatusTwoStr();


private:
	ISocketNet* m_udpHandler;  //基础库的UDP套接字
	INT m_currentDeviceIndex;  //空调编号，用于区分多个精密空调，该编号从java后台获得
	CNetService* m_localNetService;  //CNetService类，用于向java端发数据

	bool m_airConditionCmdFlag;  //标识位，用于判断时候接受到数据

	wxTimer m_Timer;  //定时器，用于定时发送状态命令

	SpecAirStatus m_SpecAirControllerStatus;  //精密空调返回的状态信息结构体

	UINT m_SpecialAirConditionerCmdNO;  //发送状态查询命令次数，当有多个精密空调时，该变量用于区分是向第几个空调发送命令

	DECLARE_EVENT_TABLE()
};

