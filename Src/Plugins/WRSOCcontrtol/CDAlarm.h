
#pragma once

#include "radarStructure.h"



class CCDAlarm
{
public:
	CCDAlarm(void);
	 ~CCDAlarm(void);

public:
	void getAlarmType();
	void getThresholdValue();
	void getCDValue(const RadarStructure::CDStructure& value);
    void CDCheck();

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
    std::map<std::string,FLOAT> m_CDValues; 
    std::map<std::string,bool> m_CDExpFlag;
    std::map<std::string,std::pair<time_t,time_t> > m_CDEndureTime;
    std::map<std::string,std::string> m_CDContent;
    std::map<std::string,std::string> m_CDConcreteContent;
	std::map<std::string,std::string> m_alarmType;

	std::string m_currenteName;
};