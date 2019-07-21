#pragma once

#include "radarStructure.h"


//对站台地址数据进行报警
class CSiteAlarm 
{
public:
	CSiteAlarm();
	~CSiteAlarm();

public:
	void getAlarmType();
	void getAlarmLevel();
	void getAlarmCNname();
	void getThresholdValue();
	void getSiteInfoData(const RadarStructure::SiteInfo& siteInfo);
	void siteInfosCheck();

private:
	bool writeToDataBase(const std::string name);
	bool writeToDataBasewithEndTime(const std::string name);

private:
	static int cbSQLGetAlarmType( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetLowerValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetUpperValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAlarmCNname(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName);
	static int cbSQLGetAlarmLevel(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName);

public:
	void cbGetAlarmType(const std::string& type) {m_alarmType[m_currenteName] = type;}
	void cbGetAlarmCNname(const std::string& name) {m_SiteContent[m_currenteName] = name;}
	void cbGetAlarmLevel(const std::string& level) {m_alarmLevel[m_currenteName] = std::stoi(level);}
	void cbGetLowerValue(const std::string lowerValue) {m_ThreadholdValues[m_currenteName].first = std::stof(lowerValue);}
	void cbGetUpperValue(const std::string upperValue) {m_ThreadholdValues[m_currenteName].second = std::stof(upperValue);}


private:

	std::map<std::string,std::pair<FLOAT,FLOAT> > m_ThreadholdValues;
	std::map<std::string,FLOAT> m_SiteValues;
	std::map<std::string,bool> m_SiteExpFlag;
	std::map<std::string,std::pair<time_t,time_t> > m_SiteEndureTime;
	std::map<std::string,std::string> m_SiteContent;
	std::map<std::string,std::string> m_SiteConcreteContent;
	std::map<std::string,std::string> m_alarmType;
	std::map<std::string,INT> m_alarmLevel;

	std::string m_currenteName;

};

