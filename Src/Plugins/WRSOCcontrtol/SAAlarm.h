#pragma once

#include "radarStructure.h"

class CSAAlarm
{
public:
	CSAAlarm(void);
	~CSAAlarm(void);

public:
	void getAlarmType();
	void getThresholdValue();
	void getSAValue(const RadarStructure::SAStructrue& value);
	void SACheck();

	void setPolarType(UINT type) {m_polarType = type;}
	
private:
	bool writeToDataBase(const std::string name);
	bool writeToDataBasewithEndTime(const std::string name);

private:
	static int cbSQLGetAlarmType( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetLowerValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetUpperValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );

public:
	void cbGetAlarmType(const std::string& type) {m_alarmType[m_currenteName] = type;}
	void cbGetLowerValue(const std::string lowerValue) {m_ThreadholdValues[m_currenteName].first = std::stof(lowerValue);}
	void cbGetUpperValue(const std::string upperValue) {m_ThreadholdValues[m_currenteName].second = std::stof(upperValue);}
	
private:
	std::map<std::string,std::pair<FLOAT,FLOAT> > m_ThreadholdValues;
	std::map<std::string,FLOAT> m_SAValues;
	std::map<std::string,bool> m_SAExpFlag;
	std::map<std::string,std::pair<time_t,time_t> > m_SAEndureTime;
	std::map<std::string,std::string> m_SAContent;
	std::map<std::string,std::string> m_SAConcreteContent;
	std::map<std::string,std::string> m_alarmType;

	std::string m_currenteName;

	UINT m_polarType;

};

