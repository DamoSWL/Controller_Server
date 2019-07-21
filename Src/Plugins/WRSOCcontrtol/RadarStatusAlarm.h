#pragma once

//该类负责对雷达参数做阈值判断
class CRadarStatusAlarm
{
public:
	CRadarStatusAlarm();
	~CRadarStatusAlarm();


public:
	void getAlarmType();  //这几个函数从数据库中读取报警配置
	void getAlarmLevel();
	void getAlarmCNname();
	void getThresholdValue();
	void getRadarStatusData(const RadarStructure::RadarStatus& radarStatus);
	void RadarStatusCheck();  //对雷达参数进行阈值判断

private:
	bool writeToDataBase(const std::string name);
	bool writeToDataBasewithEndTime(const std::string name);

private:
	static int cbSQLGetAlarmType( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );  //数据库查询回调函数
	static int cbSQLGetLowerValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetUpperValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAlarmCNname(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName);
	static int cbSQLGetAlarmLevel(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName);

public:
	void cbGetAlarmType(const std::string& type) {m_alarmType[m_currenteName] = type;}
	void cbGetAlarmCNname(const std::string& name) {m_RadarStatusContent[m_currenteName] = name;}
	void cbGetAlarmLevel(const std::string& level) {m_alarmLevel[m_currenteName] = std::stoi(level);}
	void cbGetLowerValue(const std::string lowerValue) {m_ThreadholdValues[m_currenteName].first = std::stof(lowerValue);}
	void cbGetUpperValue(const std::string upperValue) {m_ThreadholdValues[m_currenteName].second = std::stof(upperValue);}


private:
	std::map<std::string,std::string> m_alarmType;  //报警type类别容器
	std::map<std::string,INT> m_alarmLevel;       //报警等级
	std::map<std::string,std::pair<FLOAT,FLOAT> > m_ThreadholdValues;  //报警阈值
	std::map<std::string,FLOAT> m_RadarStatusValues;  //当前雷达参数
	std::map<std::string,bool> m_RadarStatusExpFlag;   //标志位，true则对应参数超出阈值，
	std::map<std::string,std::pair<time_t,time_t> > m_RadarStatusEndureTime;  //保存报警的开始时间和消除时间
	std::map<std::string,std::string> m_RadarStatusContent;   //报警原始内容
	std::map<std::string,std::string> m_RadarStatusConcreteContent;  //报警最终内容

	std::string m_currenteName;
		
}; 

