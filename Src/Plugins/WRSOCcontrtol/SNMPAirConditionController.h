#pragma once


//����ʹ��SNMPЭ����ƾ��ܿյ�

class CSNMPAirConditionController
{
public:
	CSNMPAirConditionController(void);
	~CSNMPAirConditionController(void);


public:
	typedef struct   //���ܿյ�����״̬
	{
		INT onoffStatus;
		FLOAT setTemperature;
		FLOAT setHumidity;
		FLOAT currentTemperature;
		FLOAT currentHumidity;
	}AirStatus;

public:
	bool parseAirCmd(const char *pData, UINT Len);   //��java��̨��ȡ���յ�����ָ���ʹ�øú�������ָ��
	bool sendSwitchCmd(INT flag);  //���Ϳ��ػ�����
	bool sendTemperatureCmd(INT temperature);  //�����¶ȿ�������
	bool sendHumidityCmd(INT humidity);   //����ʪ�ȿ�������

	bool AcquireStatus();  //��ȡ�յ�״̬��Ϣ

private:
	bool AcquireStatusByIP(INT index,const wxString& IP);  //���ж�����ܿյ�ʱ��ͨ��IP��ַ����ȡ״̬��Ϣ
	bool AcquireOnOffStatus(const wxString& IP);   //��ȡ���ػ�״̬
	bool AcquireSetTemperature(const wxString& IP);  //��ȡ�¶�״̬
	bool AcquireSetHumidity(const wxString& IP);   //��ȡʪ��״̬
	bool AcquireCurrentTemperature(INT index, const wxString& IP);  //��ȡ�����¶�
	bool AcquireCurrentHumidity(INT index, const wxString& IP);   //��ȡ����ʪ��

	bool writeAirStatusToDataBase(UINT index);  //��״̬��Ϣд�����ݿ���



public: 
	static DWORD WINAPI QueryStatus(LPVOID para);  //�߳���ں������ں�����ÿ��һ��ʱ�䷢�Ͳ�ѯ����


private:
	INT m_currentDeviceIndex;  //���ж�����ܿյ�ʱ�����ֵ�ǰ��յ�
	AirStatus m_AirStatus;
};

