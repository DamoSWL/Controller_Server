#pragma once

#include <list>

class CEnvironmentData
{
public:
	CEnvironmentData();
	~CEnvironmentData();

public:
	void setCRadarRunEnvironment();
	DWORD getEnvFromSensorA(std::vector<FLOAT>& envDataA);
	DWORD getEnvFromSensorB(std::vector<FLOAT>& envDataB);


private:
	bool isSensorATDataValid( const std::vector<FLOAT>& data);
	bool isSensorAHDataValid(const std::vector<FLOAT>& data);
	bool isSensorBTDataValid(const std::vector<FLOAT>& data);
	bool isSensorBHDataValid(const std::vector<FLOAT>& data);
	FLOAT getMaxFromList(const std::list<FLOAT>& list);
	FLOAT getMinFromList(const std::list<FLOAT>& list);

private:
	std::list<FLOAT> m_sensorATemperatureComparator;
	std::list<FLOAT> m_sensorAHumidityComparator;
	std::list<FLOAT> m_sensorBTemperatureComparator;
	std::list<FLOAT> m_sensorBHumidityComparator;



};

