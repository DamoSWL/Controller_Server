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
		bool bEnabled;				//使能标志，用于配置界面显示对应功能
		bool bValid;				//数据有效标志
		float inVoltage;		//输入电压MMM.M   M是一个整数，范围为0 – 9。单位V
		float lastBatteryVoltage;//上次转电池放电时电压NNN.N  N是一个整数，范围为0 – 9，有部分机种默认为0。单位是V
		float outVoltage;		//输出电压PPP.P  P是十进制的整数，范围为0 - 9。单位是V。三相输入时为R相电压
		short outLoadPercentage;	//输出负载百分比: QQQ   QQQ是W%或VA%的最大值。VA%是VA的最大值的百分比。W% 是有用功率的最大值的百分比
		float inFreq;			//输入频率: RR.R    R是一个整数，范围为0 – 9。单位是Hz
		float batteryEleVoltage;//电池单元电压: S.SS   S是一个整数，范围为0 – 9。单位是V。每节电池有6个电池单元，读到的电压要乘以6，得到单节电池电压。市电模式时，读到的是充电电压，电池模式时读到电池电压
		float temperature;		//温度: TT.T   T是一个整数，范围为0 – 9。单位是摄氏度
		union
		{
			unsigned short UPSData;
			struct
			{
				unsigned short reserve : 1;	//保留
				unsigned short bShutdown : 1;			// 1 : 关机
				unsigned short bTesting : 1;			// 1 : 测试进行中
				unsigned short bUPSMode : 1;			// 1 : UPS 后备式   0 ：在线式
				unsigned short bUPSStatus : 1;		// 1 : UPS 故障
				unsigned short bBypassModel : 1;		// 1 : 旁路模式
				unsigned short bBatteryVoltage : 1;	// 1 : 电池电压低
				unsigned short bElectricSupply : 1;	// 1 : 市电异常
				unsigned short Spa : 8;		//保留
			};
		};
	}tagUPSStatus;

public:
// 	BOOL StartCom();
// 	BOOL StopCom();
//	void sendUPSCmd();
//  virtual void CallBack(UINT /*msg*/, WPARAM /*chi*/, LPARAM /*port*/);

	void getUPSInfo();  //获取SNMP协议需要的配置信息

public:
	static int cbSQLGetID( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );	  //sql查询回调函数，获取SNMP协议需要的配置信息
	static int cbSQLGetName( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetOID( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetCoefficient( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );

	static DWORD WINAPI exec(LPVOID para);

private:
	BOOL queryUPSData();  //查询UPS状态信息
	bool writeToDataBase();

public:
	std::map<INT,std::string> m_UPSID;  //ups查询状态的命令编号
	std::map<std::string,std::string> m_UPSOID;  //ups查询状态的OID号码
	std::map<std::string,INT> m_UPSData;  //ups状态数据
	std::map<std::string,FLOAT> m_UPSCoefficient;  //ups状态参数的加权系数

	INT m_currentID;
	std::string m_currentName;




};

