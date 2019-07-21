#pragma once



class CEnvAlarm
{
public:
	CEnvAlarm();
	~CEnvAlarm();

public:
	void getAlarmType();
	void getThresholdValue();
	void getAlarmLevel();

public:
	void connectionAlarmA();
	void connectionAlarmB();
	void checkSensorA(const std::vector<FLOAT>& data);
	void checkSensorB(const std::vector<FLOAT>& data);


private:
	bool writeToDataBase(const std::string name);
	bool writeToDataBasewithEndTime(const std::string name);

private:
	static int cbSQLGetUpperTemperature( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetLowerTemperature( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetUpperHumidity( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetLowerHumidity( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAlarmLevel(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName);

public:
	 FLOAT temperatureUpper;
	 FLOAT temperatureLower;
	 FLOAT humidityUpper;
	 FLOAT humidityLower;

	 INT m_envLevel;

private:
	std::string m_envType;
	

	bool m_isConnectedFlagA;
	bool m_setFirstTimeFlagA;


	bool m_isConnectedFlagB;
	bool m_setFirstTimeFlagB;


	bool m_sensorATemperatureExp;
	bool m_sensorAHumidityExp ;

	bool m_sensorBTemperatureExp ;
	bool m_sensorBHumidityExp ;

	std::map<std::string,std::pair<time_t,time_t> > m_EnvEndureTime;
	std::map<std::string,std::string> m_EnvContent;
	std::map<std::string,std::string> m_EnvConcreteContent;



};

