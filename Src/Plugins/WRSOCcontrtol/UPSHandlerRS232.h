#pragma once

//该类用于处理无法通过SNMP协议读取状态信息的UPS设备

class UPSHandlerRS232
{
public:
	UPSHandlerRS232(void);
	~UPSHandlerRS232(void);

public:
	typedef struct _UPSDataRS232  //UPS状态信息，参见数据库表upsinfo，主要获取电压，电流，功率，负载，电池等信息，其余没有可以省略
	{
		FLOAT batCondition;  //当前电池健康状况
		FLOAT batStatus;    //当前电池状态
		FLOAT batChargeStatus;  //当前电池充电状态
		FLOAT estimatedMinutesOnBat;   //电池供电剩余时间
		FLOAT batteryVol;      //电池电压
		FLOAT batteryCurrent;   //电池电流
		FLOAT batteryTemperature;  //UPS电池温度
		FLOAT inputLine1Fre;    //输入频率1
		FLOAT inputLine1Vol;    //输入电压1
		FLOAT inputLine1Cur;    //输入电流1
		FLOAT inputLine1Power;  //输入功率1
		FLOAT inputLine2Fre;    //输入频率2
		FLOAT inputLine2Vol;    //输入电压2
		FLOAT inputLine2Cur;    //输入电流2
		FLOAT inputLine2Power;  //输入功率2
		FLOAT inputLine3Fre;    //输入频率3
		FLOAT inputLine3Vol;    //输入电压3
		FLOAT inputLine3Cur;    //输入电流3
		FLOAT inputLine3Power;  //输入功率3
		FLOAT outputSource;     //UPS输出模式
		FLOAT outputFre;      //输出频率
		FLOAT outputLines;    //UPS输出线路
		FLOAT outputLine1Vol;   //输出电压1
		FLOAT outputLine1Cur;   //输出电流1
		FLOAT outputLine1Power;  //输出功率1
		FLOAT outputLine1Load;  //输出负载1
		FLOAT outputLine2Vol;   //输出电压2
		FLOAT outputLine2Cur;   //输出电流2
		FLOAT outputLine2Power;  //输出功率2
		FLOAT outputLine2Load;   //输出负载2
		FLOAT outputLine3Vol;   //输出电压3
		FLOAT outputLine3Cur;   //输出电流3
		FLOAT outputLine3Power;  //输出功率3
		FLOAT outputLine3Load;   //输出负载3
		FLOAT bypassFre;   //旁路频率
		FLOAT bypassLines;   //旁路路线
		FLOAT bypassLine1Vol;   //旁路电压1
		FLOAT bypassLine1Cur;   //旁路电流1
		FLOAT bypassLine1Power;  //旁路功率1
		FLOAT bypassLine2Vol;   //旁路电压2
		FLOAT bypassLine2Cur;   //旁路电流2
		FLOAT bypassLine2Power;  //旁路功率2
		FLOAT bypassLine3Vol;   //旁路电压3
		FLOAT bypassLine3Cur;   //旁路电流3
		FLOAT bypassLine3Power;  //旁路功率3
		FLOAT outputTotalPower;   //UPS输出总功率
		FLOAT batteryCap;        //电池容量
		FLOAT upsFaultId;   //UPS错误信息ID
		FLOAT upsShutdown;  //UPS关机
		FLOAT chargeFault;  //充电故障
		FLOAT systemShutdown;  //系统关机
		FLOAT autoRestart;   //自动重启
		FLOAT shutdownDelay;  //停机延迟
		FLOAT shutdownAtonce;  //立即关机
		FLOAT uspsCommunication;  //通信状态
		FLOAT batteryFault;  //电池故障
		FLOAT batteryVolLow;  //电池电压过低
		FLOAT bypass;  //旁路状态【0：市电线路；1：旁路线路】
		FLOAT otherFault;  //其他故障
		FLOAT testInProgress;  //测试状态

	}UPSDataRS232;

public:
	static DWORD WINAPI exec(LPVOID para);  //线程入口函数，在线程中发送命令,并写入数据库中

	static int processData(void *pHandle, const char *pData, UINT Len);  //注册回调，用于处理返回的ups状态数据
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT Len = 0);

public:
	BOOL startReceive();  //打开网络端口
	BOOL stopReceive();   //关闭网络端口


	BOOL queryUPSData();  //发送UPS查询指令
	bool writeToDataBase();  //将状态信息写入数据库中


public:
	UPSDataRS232 m_UPSDataRS232;  //ups状态信息
	BOOL m_newDataFlag;  //标志位，表示接受到新的数据

private: 
	ISocketNet* m_udpHandler;  //基础库中udp套接字

	
};

