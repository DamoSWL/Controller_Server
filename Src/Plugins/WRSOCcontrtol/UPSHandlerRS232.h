#pragma once

//�������ڴ����޷�ͨ��SNMPЭ���ȡ״̬��Ϣ��UPS�豸

class UPSHandlerRS232
{
public:
	UPSHandlerRS232(void);
	~UPSHandlerRS232(void);

public:
	typedef struct _UPSDataRS232  //UPS״̬��Ϣ���μ����ݿ��upsinfo����Ҫ��ȡ��ѹ�����������ʣ����أ���ص���Ϣ������û�п���ʡ��
	{
		FLOAT batCondition;  //��ǰ��ؽ���״��
		FLOAT batStatus;    //��ǰ���״̬
		FLOAT batChargeStatus;  //��ǰ��س��״̬
		FLOAT estimatedMinutesOnBat;   //��ع���ʣ��ʱ��
		FLOAT batteryVol;      //��ص�ѹ
		FLOAT batteryCurrent;   //��ص���
		FLOAT batteryTemperature;  //UPS����¶�
		FLOAT inputLine1Fre;    //����Ƶ��1
		FLOAT inputLine1Vol;    //�����ѹ1
		FLOAT inputLine1Cur;    //�������1
		FLOAT inputLine1Power;  //���빦��1
		FLOAT inputLine2Fre;    //����Ƶ��2
		FLOAT inputLine2Vol;    //�����ѹ2
		FLOAT inputLine2Cur;    //�������2
		FLOAT inputLine2Power;  //���빦��2
		FLOAT inputLine3Fre;    //����Ƶ��3
		FLOAT inputLine3Vol;    //�����ѹ3
		FLOAT inputLine3Cur;    //�������3
		FLOAT inputLine3Power;  //���빦��3
		FLOAT outputSource;     //UPS���ģʽ
		FLOAT outputFre;      //���Ƶ��
		FLOAT outputLines;    //UPS�����·
		FLOAT outputLine1Vol;   //�����ѹ1
		FLOAT outputLine1Cur;   //�������1
		FLOAT outputLine1Power;  //�������1
		FLOAT outputLine1Load;  //�������1
		FLOAT outputLine2Vol;   //�����ѹ2
		FLOAT outputLine2Cur;   //�������2
		FLOAT outputLine2Power;  //�������2
		FLOAT outputLine2Load;   //�������2
		FLOAT outputLine3Vol;   //�����ѹ3
		FLOAT outputLine3Cur;   //�������3
		FLOAT outputLine3Power;  //�������3
		FLOAT outputLine3Load;   //�������3
		FLOAT bypassFre;   //��·Ƶ��
		FLOAT bypassLines;   //��··��
		FLOAT bypassLine1Vol;   //��·��ѹ1
		FLOAT bypassLine1Cur;   //��·����1
		FLOAT bypassLine1Power;  //��·����1
		FLOAT bypassLine2Vol;   //��·��ѹ2
		FLOAT bypassLine2Cur;   //��·����2
		FLOAT bypassLine2Power;  //��·����2
		FLOAT bypassLine3Vol;   //��·��ѹ3
		FLOAT bypassLine3Cur;   //��·����3
		FLOAT bypassLine3Power;  //��·����3
		FLOAT outputTotalPower;   //UPS����ܹ���
		FLOAT batteryCap;        //�������
		FLOAT upsFaultId;   //UPS������ϢID
		FLOAT upsShutdown;  //UPS�ػ�
		FLOAT chargeFault;  //������
		FLOAT systemShutdown;  //ϵͳ�ػ�
		FLOAT autoRestart;   //�Զ�����
		FLOAT shutdownDelay;  //ͣ���ӳ�
		FLOAT shutdownAtonce;  //�����ػ�
		FLOAT uspsCommunication;  //ͨ��״̬
		FLOAT batteryFault;  //��ع���
		FLOAT batteryVolLow;  //��ص�ѹ����
		FLOAT bypass;  //��·״̬��0���е���·��1����·��·��
		FLOAT otherFault;  //��������
		FLOAT testInProgress;  //����״̬

	}UPSDataRS232;

public:
	static DWORD WINAPI exec(LPVOID para);  //�߳���ں��������߳��з�������,��д�����ݿ���

	static int processData(void *pHandle, const char *pData, UINT Len);  //ע��ص������ڴ����ص�ups״̬����
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT Len = 0);

public:
	BOOL startReceive();  //������˿�
	BOOL stopReceive();   //�ر�����˿�


	BOOL queryUPSData();  //����UPS��ѯָ��
	bool writeToDataBase();  //��״̬��Ϣд�����ݿ���


public:
	UPSDataRS232 m_UPSDataRS232;  //ups״̬��Ϣ
	BOOL m_newDataFlag;  //��־λ����ʾ���ܵ��µ�����

private: 
	ISocketNet* m_udpHandler;  //��������udp�׽���

	
};

