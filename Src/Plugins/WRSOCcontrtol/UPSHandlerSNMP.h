#pragma once
//#include "SerialPort.h"

class UPSHandlerSNMP/* : public ISerialPortCalBack*/
{
public:
	UPSHandlerSNMP();
	~UPSHandlerSNMP();

public:
	struct comConfig
	{
		UINT portnr;
		UINT baud;
		char parity;
		UINT databits;
		UINT stopbits;
	};

	typedef struct UPS_STATUS
	{
		bool bEnabled;				//ʹ�ܱ�־���������ý�����ʾ��Ӧ����
		bool bValid;				//������Ч��־
		float inVoltage;		//�����ѹMMM.M   M��һ����������ΧΪ0 �C 9����λV
		float lastBatteryVoltage;//�ϴ�ת��طŵ�ʱ��ѹNNN.N  N��һ����������ΧΪ0 �C 9���в��ֻ���Ĭ��Ϊ0����λ��V
		float outVoltage;		//�����ѹPPP.P  P��ʮ���Ƶ���������ΧΪ0 - 9����λ��V����������ʱΪR���ѹ
		short outLoadPercentage;	//������ذٷֱ�: QQQ   QQQ��W%��VA%�����ֵ��VA%��VA�����ֵ�İٷֱȡ�W% �����ù��ʵ����ֵ�İٷֱ�
		float inFreq;			//����Ƶ��: RR.R    R��һ����������ΧΪ0 �C 9����λ��Hz
		float batteryEleVoltage;//��ص�Ԫ��ѹ: S.SS   S��һ����������ΧΪ0 �C 9����λ��V��ÿ�ڵ����6����ص�Ԫ�������ĵ�ѹҪ����6���õ����ڵ�ص�ѹ���е�ģʽʱ���������ǳ���ѹ�����ģʽʱ������ص�ѹ
		float temperature;		//�¶�: TT.T   T��һ����������ΧΪ0 �C 9����λ�����϶�
		union
		{
			unsigned short UPSData;
			struct
			{
				unsigned short reserve : 1;	//����
				unsigned short bShutdown : 1;			// 1 : �ػ�
				unsigned short bTesting : 1;			// 1 : ���Խ�����
				unsigned short bUPSMode : 1;			// 1 : UPS ��ʽ   0 ������ʽ
				unsigned short bUPSStatus : 1;		// 1 : UPS ����
				unsigned short bBypassModel : 1;		// 1 : ��·ģʽ
				unsigned short bBatteryVoltage : 1;	// 1 : ��ص�ѹ��
				unsigned short bElectricSupply : 1;	// 1 : �е��쳣
				unsigned short Spa : 8;		//����
			};
		};
	}tagUPSStatus;

public:
// 	BOOL StartCom();
// 	BOOL StopCom();
//	void sendUPSCmd();
//  virtual void CallBack(UINT /*msg*/, WPARAM /*chi*/, LPARAM /*port*/);

	void getUPSInfo();  //��ȡSNMPЭ����Ҫ��������Ϣ

public:
	static int cbSQLGetID( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );	  //sql��ѯ�ص���������ȡSNMPЭ����Ҫ��������Ϣ
	static int cbSQLGetName( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );
	static int cbSQLGetOID( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );
	static int cbSQLGetCoefficient( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );

	static DWORD WINAPI exec(LPVOID para);

private:
	BOOL queryUPSData();  //��ѯUPS״̬��Ϣ
	bool writeToDataBase();

public:
	std::map<INT,std::string> m_UPSID;  //ups��ѯ״̬��������
	std::map<std::string,std::string> m_UPSOID;  //ups��ѯ״̬��OID����
	std::map<std::string,INT> m_UPSData;  //ups״̬����
	std::map<std::string,FLOAT> m_UPSCoefficient;  //ups״̬�����ļ�Ȩϵ��

	INT m_currentID;
	std::string m_currentName;




};

