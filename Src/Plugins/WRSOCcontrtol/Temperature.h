#pragma once

#include "IProcess.h"
#include <tuple>
#include "EnvAlarm.h"

//���ฺ���ȡ��ʪ��
class CTemperature : public wxEvtHandler,
	public IProcess
{
public:
	CTemperature(void);
	virtual ~CTemperature(void);

public:
	virtual void setID(const std::set<UINT>& id);  //���û�������ID
	virtual void setIPAddress(const std::string& IPStr) {}  //���÷��͵�IP��ַ
	virtual void processCmd(const char* pData,unsigned int len);  //�������������ص�����
	virtual void startTimer(int interval);  //������ʱ������ʱ�����������
	virtual void stopTimer();
	virtual void setLocationType(UINT type) {}

public:
	static int processData(void *pHandle, const char *pData, UINT Len);  //�������������ص�����
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
	std::map<UINT,std::tuple<FLOAT,FLOAT> > m_TemperatureParam;  //��ȡ������ʪ��
	std::map<UINT,bool> m_newTemperatureFlag; //��־λ���ж��Ƿ��յ�����

	BOOL m_envExceptionFlag;  //��ʶλ���жϻ������������Ƿ��쳣
	std::pair<time_t,time_t> m_EnvExceptionEndureTime;  //�������������쳣ʱ��

	CEnvAlarm m_EnvAlarm;

	DECLARE_EVENT_TABLE()

};

