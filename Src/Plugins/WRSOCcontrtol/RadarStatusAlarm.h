#pragma once

//���ฺ����״��������ֵ�ж�
class CRadarStatusAlarm
{
public:
	CRadarStatusAlarm();
	~CRadarStatusAlarm();


public:
	void getAlarmType();  //�⼸�����������ݿ��ж�ȡ��������
	void getAlarmLevel();
	void getAlarmCNname();
	void getThresholdValue();
	void getRadarStatusData(const RadarStructure::RadarStatus& radarStatus);
	void RadarStatusCheck();  //���״����������ֵ�ж�

private:
	bool writeToDataBase(const std::string name);
	bool writeToDataBasewithEndTime(const std::string name);

private:
	static int cbSQLGetAlarmType( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );  //���ݿ��ѯ�ص�����
	static int cbSQLGetLowerValue( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );
	static int cbSQLGetUpperValue( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAlarmCNname(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName);
	static int cbSQLGetAlarmLevel(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName);

public:
	void cbGetAlarmType(const std::string& type) {m_alarmType[m_currenteName] = type;}
	void cbGetAlarmCNname(const std::string& name) {m_RadarStatusContent[m_currenteName] = name;}
	void cbGetAlarmLevel(const std::string& level) {m_alarmLevel[m_currenteName] = std::stoi(level);}
	void cbGetLowerValue(const std::string lowerValue) {m_ThreadholdValues[m_currenteName].first = std::stof(lowerValue);}
	void cbGetUpperValue(const std::string upperValue) {m_ThreadholdValues[m_currenteName].second = std::stof(upperValue);}


private:
	std::map<std::string,std::string> m_alarmType;  //����type�������
	std::map<std::string,INT> m_alarmLevel;       //�����ȼ�
	std::map<std::string,std::pair<FLOAT,FLOAT> > m_ThreadholdValues;  //������ֵ
	std::map<std::string,FLOAT> m_RadarStatusValues;  //��ǰ�״����
	std::map<std::string,bool> m_RadarStatusExpFlag;   //��־λ��true���Ӧ����������ֵ��
	std::map<std::string,std::pair<time_t,time_t> > m_RadarStatusEndureTime;  //���汨���Ŀ�ʼʱ�������ʱ��
	std::map<std::string,std::string> m_RadarStatusContent;   //����ԭʼ����
	std::map<std::string,std::string> m_RadarStatusConcreteContent;  //������������

	std::string m_currenteName;
		
}; 

