#pragma once

#include "IProcess.h"
#include <tuple>
#include "EnvAlarm.h"

//该类负责获取温湿度
class CTemperature : public wxEvtHandler,
	public IProcess
{
public:
	CTemperature(void);
	virtual ~CTemperature(void);

public:
	virtual void setID(const std::set<UINT>& id);  //设置环境主机ID
	virtual void setIPAddress(const std::string& IPStr) {}  //设置发送的IP地址
	virtual void processCmd(const char* pData,unsigned int len);  //处理环境主机返回的命令
	virtual void startTimer(int interval);  //开启定时器，定时到达后发送命令
	virtual void stopTimer();
	virtual void setLocationType(UINT type) {}

public:
	static int processData(void *pHandle, const char *pData, UINT Len);  //处理环境主机返回的命令
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT Len = 0);

private:
	void startWriteDataTimer();
	void sendTemperatureCmd(UINT id);
	void onwriteDataTimeout(wxTimerEvent& event);
	void onCmdTimeout(wxTimerEvent& event);

	bool writeToDataBase();

private:
	wxTimer m_cmdTimer;
	wxTimer m_writeDataTimer;

	std::set<UINT> m_IDSet;
	std::map<UINT,std::tuple<FLOAT,FLOAT> > m_TemperatureParam;  //获取到的温湿度
	std::map<UINT,bool> m_newTemperatureFlag; //标志位，判断是否收到数据

	BOOL m_envExceptionFlag;  //标识位，判断环境主机连接是否异常
	std::pair<time_t,time_t> m_EnvExceptionEndureTime;  //环境主机连接异常时间

	CEnvAlarm m_EnvAlarm;

	DECLARE_EVENT_TABLE()

};

