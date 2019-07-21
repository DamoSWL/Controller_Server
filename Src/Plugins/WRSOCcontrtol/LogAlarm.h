#pragma once


//该类负责解析雷达报警日志文件
class CLogAlarm 
{
public:
	CLogAlarm();
	~CLogAlarm();

public:
	typedef struct _RADARALARM
	{
		char STATUS_DATA[9];
		char STATUS_TIME[7];
		char MONITOR_ALARM_FLAG[8];
		int ALARM_NO;
		char SPARE[4];

	} RADARALARM;   //SA等型号雷达的报警定义

	typedef struct				//报警结构
	{	
		char  STATUS_DATA[9];
		char  STATUS_TIME[7];
		int   ALARM_NO;			//雷达报警码	
		int   MONITOR_ALARM_FLAG;	  //SC,CD等型号雷达的把哦哦经定义	
		
	}RADAR_ALARM_DATA;  

	enum      //MONITOR_ALARM_FLAG的数字定义 1表示报警 0表示消除报警
	{
		ALARMON = 1,
		ALARMOFF
	};

private:
	static int cbSQLGetAlarmNum( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );  //数据库回调函数 获取报警的subsystem content等字段内容
	static int cbSQLGetsubsystem( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAlarmContent( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );

public:
	static DWORD WINAPI alarmFileMonitor(LPVOID para);  //线程入口函数，在函数内解析文件

	void getAlarmType();  //获取报警信息的配置信息
	void getLatestFileName(std::map<UINT,wxString>& fileInfo,UINT* newTime);  //获取最新的报警日志文件
	void getAlarmInfo(const RADARALARM& data);   //将报警信息存入成员变量中
	void getAlarmInfoEd2(const RADAR_ALARM_DATA& data);
	bool parseAlarmInfo(UINT newTime);  //解析报警信息 两个版本对应不同雷达型号
	bool parseAlarmInfoEd2(UINT newTime);
	void setUpdateTime(UINT time){m_updateTime = time;}

private:
	time_t getDateTime(const RADARALARM& alarmInfo);
	time_t getDateTimeEd2(const RADAR_ALARM_DATA& alarmInfo);
	bool writeToDataBase(const RADARALARM& alarmInfo);
	bool writeToDataBaseEd2(const RADAR_ALARM_DATA& alarmInfo);

public:
	std::map<UINT,std::string> m_alarmSubSystem;
	std::map<UINT,std::string> m_alarmContent;
	UINT m_currentAlarmNum;

private:
	UINT m_updateTime;  //最近一次报警文件的生成时间
	std::vector<RADARALARM> m_alarmInfo;
	std::vector<RADAR_ALARM_DATA> m_alarmInfoEd2;
	std::set<INT> m_activeAlarmNum;  //未消除报警的报警编号集合

};

