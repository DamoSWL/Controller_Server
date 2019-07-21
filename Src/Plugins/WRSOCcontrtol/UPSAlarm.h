#pragma once


#include "UPSHandlerSNMP.h"


class CUPSAlarm 
{
public:
	CUPSAlarm();
	~CUPSAlarm();

	void getAlarmType();
	void getThresholdValue();
	

public:
	void getUPSValue(const UPSHandlerSNMP::tagUPSStatus& value);
	void UPSCheck(const UPSHandlerSNMP::tagUPSStatus&  data);
	void connectAlarm();

private:
	bool writeToDataBase(const std::string name);
	bool writeToDataBasewithEndTime(const std::string name);

private:
	bool m_isConnectedFlag;
	bool m_setFirstTimeFlag;

	std::map<std::string,std::pair<FLOAT,FLOAT> > m_ThreadholdValues;
	std::map<std::string,FLOAT> m_UPSValues;
	std::map<std::string,bool> m_UPSExpFlag;
	std::map<std::string,std::pair<time_t,time_t> > m_UPSEndureTime;
	std::map<std::string,std::string> m_UPSContent;
	std::map<std::string,std::string> m_UPSConcreteContent;
	std::string m_alarmType;

	

};

