#pragma once

class CNetService;


//�������ڴ����ܿյ������Ϳյ�ָ�����յ����ص�״̬
class CSpecAirConditionController : public wxEvtHandler
{

public:
	CSpecAirConditionController(void);
	~CSpecAirConditionController(void);

public:
	typedef struct _SpecAirStatus  //���ܿյ�״̬���ؽṹ�嶨�壬ÿ�����ܿյ�����ͬ
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

	static int processData(void *pHandle, const char *pData, UINT Len);  //�ص������������ܿյ����ص���Ϣ
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT uiLen = 0);

	BOOL startReceive();  //������˿�
	BOOL stopReceive();   //�ر�����˿�

	void startTimer() {m_Timer.Start(60*1000);}  //��ʱ�����������ڶ�ʱ���Ϳյ�״̬��ѯָ��
	void stopTImer() {m_Timer.Stop();}

	void parseAirCmd(const char *pData, UINT Len);  //��java��̨��ȡ���յ�����ָ���ʹ�øú�������ָ��
	void parseAirStatus(const char *pData, UINT Len);  //�����ӿյ����ص�״̬��Ϣ

	void setNetService(CNetService* localNetService) {m_localNetService = localNetService;}  //��ȡ��CNetService���ָ�룬������java�˷�������
	CNetService* getNetService() {return m_localNetService;}

	void setairConditionFlag(bool flag) {m_airConditionCmdFlag = flag;}  //��ʶλ�������ж��Ƿ���ܵ��յ��ظ�
	bool getairConditonFlag(){return m_airConditionCmdFlag;}

	void onAirStatusTimeout(wxTimerEvent& event);  //��ʱ����������ʱ����ָ��



private:
	bool sendSwitchCmd(INT flag);  //���Ϳ�������
	bool sendTemperatureCmd(INT temperature);  //�����¶�
	bool sendHumidityCmd(INT humidity);  // ����ʪ��
	bool writeToDataBase(UCHAR id);  //��״̬��Ϣд�����ݿ���

	std::string getStatusStr();     //��������������ȡ��״̬��Ϣ����ַ�����д�����ݿ��У�ÿ���յ���ɷ�ʽ����һ��
	std::string getAlarmStatusOneStr();
	std::string getAlarmStatusTwoStr();


private:
	ISocketNet* m_udpHandler;  //�������UDP�׽���
	INT m_currentDeviceIndex;  //�յ���ţ��������ֶ�����ܿյ����ñ�Ŵ�java��̨���
	CNetService* m_localNetService;  //CNetService�࣬������java�˷�����

	bool m_airConditionCmdFlag;  //��ʶλ�������ж�ʱ����ܵ�����

	wxTimer m_Timer;  //��ʱ�������ڶ�ʱ����״̬����

	SpecAirStatus m_SpecAirControllerStatus;  //���ܿյ����ص�״̬��Ϣ�ṹ��

	UINT m_SpecialAirConditionerCmdNO;  //����״̬��ѯ������������ж�����ܿյ�ʱ���ñ���������������ڼ����յ���������

	DECLARE_EVENT_TABLE()
};

