#pragma once


//���ฺ������״ﱨ����־�ļ�
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

	} RADARALARM;   //SA���ͺ��״�ı�������

	typedef struct				//�����ṹ
	{	
		char  STATUS_DATA[9];
		char  STATUS_TIME[7];
		int   ALARM_NO;			//�״ﱨ����	
		int   MONITOR_ALARM_FLAG;	  //SC,CD���ͺ��״�İ�ŶŶ������	
		
	}RADAR_ALARM_DATA;  

	enum      //MONITOR_ALARM_FLAG�����ֶ��� 1��ʾ���� 0��ʾ��������
	{
		ALARMON = 1,
		ALARMOFF
	};

private:
	static int cbSQLGetAlarmNum( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );  //���ݿ�ص����� ��ȡ������subsystem content���ֶ�����
	static int cbSQLGetsubsystem( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAlarmContent( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );

public:
	static DWORD WINAPI alarmFileMonitor(LPVOID para);  //�߳���ں������ں����ڽ����ļ�

	void getAlarmType();  //��ȡ������Ϣ��������Ϣ
	void getLatestFileName(std::map<UINT,wxString>& fileInfo,UINT* newTime);  //��ȡ���µı�����־�ļ�
	void getAlarmInfo(const RADARALARM& data);   //��������Ϣ�����Ա������
	void getAlarmInfoEd2(const RADAR_ALARM_DATA& data);
	bool parseAlarmInfo(UINT newTime);  //����������Ϣ �����汾��Ӧ��ͬ�״��ͺ�
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
	UINT m_updateTime;  //���һ�α����ļ�������ʱ��
	std::vector<RADARALARM> m_alarmInfo;
	std::vector<RADAR_ALARM_DATA> m_alarmInfoEd2;
	std::set<INT> m_activeAlarmNum;  //δ���������ı�����ż���

};

