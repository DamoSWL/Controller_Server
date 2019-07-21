#include "EnvironmentData.h"
#include <ctime>
#include <cstdlib>

static const int sensorType[] = {0x30,      0x31, 
								0x10,		0x20,
								0x21,	    0x40,
								0x41,		0x42,
								0x83,		0xE0,
								0xE1,		0xE3,
								0 };

static const int queryCmdLen = 10;
static const int expectedRetLen = 14;

CEnvironmentData::CEnvironmentData()
{
}


CEnvironmentData::~CEnvironmentData()
{
}

bool CEnvironmentData::isSensorATDataValid(const std::vector<FLOAT>& data)
{
	if (m_sensorATemperatureComparator.size() == 0)
	{
		m_sensorATemperatureComparator.push_back(data[0]);
		return true;
	}
	else
	{
		if ((data[0] > getMaxFromList(m_sensorATemperatureComparator) + 10) || (data[0] < getMinFromList(m_sensorATemperatureComparator) - 10))
		{
			return false;
		}
		else
		{
			if (m_sensorATemperatureComparator.size() > 5)
			{
				m_sensorATemperatureComparator.pop_front();
			}
			m_sensorATemperatureComparator.push_back(data[0]);
			return true;
		}
	}

	return false;

}

bool CEnvironmentData::isSensorAHDataValid(const std::vector<FLOAT>& data)
{
	if (m_sensorAHumidityComparator.size() == 0)
	{
		m_sensorAHumidityComparator.push_back(data[1]);
		return true;
	}
	else
	{
		if ((data[1] > getMaxFromList(m_sensorAHumidityComparator) + 10) || (data[1] < getMinFromList(m_sensorAHumidityComparator) - 10))
		{
			return false;
		}
		else
		{
			if (m_sensorAHumidityComparator.size() > 5)
			{
				m_sensorAHumidityComparator.pop_front();
			}
			m_sensorAHumidityComparator.push_back(data[1]);
			return true;
		}
	}
	return false;
}

bool CEnvironmentData::isSensorBTDataValid(const std::vector<FLOAT>& data)
{
	if (m_sensorBTemperatureComparator.size() == 0)
	{
		m_sensorBTemperatureComparator.push_back(data[0]);
		return true;
	}
	else
	{
		if ((data[0] > getMaxFromList(m_sensorBTemperatureComparator) + 10) || (data[0] < getMinFromList(m_sensorBTemperatureComparator) - 10))
		{
			return false;
		}
		else
		{
			if (m_sensorBTemperatureComparator.size() > 5)
			{
				m_sensorBTemperatureComparator.pop_front();
			}
			m_sensorBTemperatureComparator.push_back(data[0]);
			return true;
		}
	}
	return false;
}

bool CEnvironmentData::isSensorBHDataValid(const std::vector<FLOAT>& data)
{
	if (m_sensorBHumidityComparator.size() == 0)
	{
		m_sensorBHumidityComparator.push_back(data[1]);
		return true;
	}
	else
	{
		if ((data[1] > getMaxFromList(m_sensorBHumidityComparator) + 10) || (data[1] < getMinFromList(m_sensorBHumidityComparator) - 10))
		{
			return false;
		}
		else
		{
			if (m_sensorBHumidityComparator.size() > 5)
			{
				m_sensorBHumidityComparator.pop_front();
			}
			m_sensorBHumidityComparator.push_back(data[1]);
			return true;
		}
	}
	return false;
}


FLOAT CEnvironmentData::getMaxFromList(const std::list<FLOAT>& list)
{
	FLOAT maxValue = list.front();
	for each (auto val in list)
	{
		if (val > maxValue)
		{
			maxValue = val;
		}
	}
	return maxValue;
}

FLOAT CEnvironmentData::getMinFromList(const std::list<FLOAT>& list)
{
	FLOAT minValue = list.front();
	for each (auto val in list)
	{
		if (val < minValue)
		{
			minValue = val;
		}
	}
	return minValue;
}

DWORD CEnvironmentData::getEnvFromSensorA(std::vector<FLOAT>& envDataA)
{

	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return EXIT_FAILURE;
	}

	SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockServer == INVALID_SOCKET)
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"env套接字创建失败");
		return EXIT_FAILURE;
	}
	SOCKADDR_IN severAddr;
	severAddr.sin_family = AF_INET;
	severAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	severAddr.sin_port = htons(3193);

	if (::bind(sockServer, (SOCKADDR*)&severAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"env套接字绑定失败");
		return EXIT_FAILURE;
	}

	UINT timeout = 100;
	setsockopt(sockServer, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(UINT));


	SOCKADDR_IN clientAddr;
	clientAddr.sin_family = AF_INET;

	bool temperatureUpdateFlag = false;
	bool humidityDataUpdateFlag = false;

	auto Count = 3;

	while (Count--)
	{

		std::string message("sensorA ");
	//	GetPlug()->AddLog(LOG_TYPE_MESSAGE,message.c_str());

		IN_ADDR addr;
		inet_pton(AF_INET, GetPlug()->getConfig()->NetConfig.Env.ip, (PVOID)&addr);
		clientAddr.sin_addr = addr;
		clientAddr.sin_port = htons(GetPlug()->getConfig()->NetConfig.Env.port);

		char queryCmd[queryCmdLen] = { 0 };
		queryCmd[0] = '\x7F';
		queryCmd[1] = '\x01';
		queryCmd[2] = '\x05';
		queryCmd[3] = '\x64';
		queryCmd[4] = '\x6D';

		try
		{

			queryCmd[5] = GetPlug()->getConfig()->HumidSensor[0].id;
			queryCmd[6] = sensorType[GetPlug()->getConfig()->HumidSensor[0].sensorType[0]];
			queryCmd[7] = GetPlug()->getConfig()->HumidSensor[0].attribute[0];
			queryCmd[8] = queryCmd[1] ^ queryCmd[2] ^ queryCmd[3] ^ queryCmd[4] ^ queryCmd[5] ^ queryCmd[6] ^ queryCmd[7];
			queryCmd[9] = '\xEF';

			SOCKADDR_IN client;
			int socklen = sizeof(SOCKADDR);

			if(!temperatureUpdateFlag)
			{
				sendto(sockServer, queryCmd, queryCmdLen, 0, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));
				char result[20] = { '\0' };

				int len = recvfrom(sockServer, result, 20, 0, (SOCKADDR*)&client, &socklen);

				if (len != expectedRetLen)
				{
					throw std::runtime_error(std::to_string((unsigned long long)len));
				}
				else
				{
					if(((unsigned char)result[5] == GetPlug()->getConfig()->HumidSensor[0].id) &&                
						((unsigned char)result[6] == sensorType[GetPlug()->getConfig()->HumidSensor[0].sensorType[0]]) && 
						((unsigned char)result[7] == GetPlug()->getConfig()->HumidSensor[0].attribute[0]))
					{
						FLOAT sensorATemperature = 0;
						memcpy(&sensorATemperature, result + 8, 4);
						if (sensorATemperature <= -990.0)
						{
							throw std::runtime_error("error");
						}
						else
						{
							envDataA[0] = sensorATemperature;
							temperatureUpdateFlag = true;

						}
					}	
					else
					{
						GetPlug()->AddLog(LOG_TYPE_ERROR,"传感器A返回温度地址异常");
						continue;
					}	

				}
			}


			queryCmd[6] = sensorType[GetPlug()->getConfig()->HumidSensor[0].sensorType[1]];
			queryCmd[7] = GetPlug()->getConfig()->HumidSensor[0].attribute[1];
			queryCmd[8] = queryCmd[1] ^ queryCmd[2] ^ queryCmd[3] ^ queryCmd[4] ^ queryCmd[5] ^ queryCmd[6] ^ queryCmd[7];

			if(!humidityDataUpdateFlag)
			{
				sendto(sockServer, queryCmd, queryCmdLen, 0, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));
				char result[20] = { '\0' };

				int len = recvfrom(sockServer, result, 20, 0, (SOCKADDR*)&client, &socklen);

				if (len != expectedRetLen)
				{
					throw std::runtime_error(std::to_string((unsigned long long)len));
				}
				else
				{
					if(((unsigned char)result[5] == GetPlug()->getConfig()->HumidSensor[0].id) &&                
						((unsigned char)result[6] == sensorType[GetPlug()->getConfig()->HumidSensor[0].sensorType[1]]) && 
						((unsigned char)result[7] == GetPlug()->getConfig()->HumidSensor[0].attribute[1]))
					{
						FLOAT sensorAHumidity = 0;
						memcpy(&sensorAHumidity, result + 8, 4);
						if (sensorAHumidity <= -990.0)
						{
							throw std::runtime_error("error");
						}
						else
						{
							envDataA[1] = sensorAHumidity;
							humidityDataUpdateFlag = true;

						}
					}
					else
					{
						GetPlug()->AddLog(LOG_TYPE_ERROR,"传感器A返回湿度地址异常");
						continue;
					}

				}
			}

			if (isSensorATDataValid(envDataA) && isSensorAHDataValid(envDataA))
			{
				temperatureUpdateFlag = false;
				humidityDataUpdateFlag = false;

				closesocket(sockServer);
				return EXIT_SUCCESS;
			}
			else
			{
				temperatureUpdateFlag = false;
				humidityDataUpdateFlag = false;
				continue;
			}

		}
		catch (std::runtime_error& e)
		{

			int len = std::atoi(e.what());
			if (len = -1)
			{
				continue;
			}
			else
			{
				SOCKADDR_IN client;
				int socklen = sizeof(SOCKADDR);
				char result[20] = { 0 };

				while (len != -1)
				{
					len = recvfrom(sockServer, result, 20, 0, (SOCKADDR*)&client, &socklen);

				}
			}
		}

	}

	closesocket(sockServer);
	return EXIT_FAILURE;
	
	

}

DWORD CEnvironmentData::getEnvFromSensorB(std::vector<FLOAT>& envDataB)
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return EXIT_FAILURE;
	}

	SOCKET sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockServer == INVALID_SOCKET)
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"env套接字创建失败");
		return EXIT_FAILURE;
	}
	SOCKADDR_IN severAddr;
	severAddr.sin_family = AF_INET;
	severAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	severAddr.sin_port = htons(3193);

	if (::bind(sockServer, (SOCKADDR*)&severAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"env套接字绑定失败");
		return EXIT_FAILURE;
	}

	UINT timeout = 100;
	setsockopt(sockServer, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(UINT));


	SOCKADDR_IN clientAddr;
	clientAddr.sin_family = AF_INET;

	bool temperatureUpdateFlag = false;
	bool humidityDataUpdateFlag = false;

	auto Count = 3;

	while (Count--)
	{
		std::string message("sensorB ");
	//	GetPlug()->AddLog(LOG_TYPE_MESSAGE,message.c_str());

		IN_ADDR addr;
		inet_pton(AF_INET, GetPlug()->getConfig()->NetConfig.Env.ip, (PVOID)&addr);
		clientAddr.sin_addr = addr;
		clientAddr.sin_port = htons(GetPlug()->getConfig()->NetConfig.Env.port);

		char queryCmd[queryCmdLen] = { 0 };
		queryCmd[0] = '\x7F';
		queryCmd[1] = '\x01';
		queryCmd[2] = '\x05';
		queryCmd[3] = '\x64';
		queryCmd[4] = '\x6D';

		try
		{


			queryCmd[5] = GetPlug()->getConfig()->HumidSensor[1].id;
			queryCmd[6] = sensorType[GetPlug()->getConfig()->HumidSensor[1].sensorType[0]];
			queryCmd[7] = GetPlug()->getConfig()->HumidSensor[1].attribute[0];
			queryCmd[8] = queryCmd[1] ^ queryCmd[2] ^ queryCmd[3] ^ queryCmd[4] ^ queryCmd[5] ^ queryCmd[6] ^ queryCmd[7];
			queryCmd[9] = '\xEF';

			SOCKADDR_IN client;
			int socklen = sizeof(SOCKADDR);

			if(!temperatureUpdateFlag)
			{
				sendto(sockServer, queryCmd, queryCmdLen, 0, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));
				char result[20] = { '\0' };

				int len = recvfrom(sockServer, result, 20, 0, (SOCKADDR*)&client, &socklen);
				if (len != expectedRetLen)
				{
					throw std::runtime_error(std::to_string((unsigned long long)len));
				}
				else
				{
					if(((unsigned char)result[5] == GetPlug()->getConfig()->HumidSensor[1].id) &&                
						((unsigned char)result[6] == sensorType[GetPlug()->getConfig()->HumidSensor[1].sensorType[0]]) && 
						((unsigned char)result[7] == GetPlug()->getConfig()->HumidSensor[1].attribute[0]))
					{
						FLOAT sensorBTemperature = 0;
						memcpy(&sensorBTemperature, result + 8, 4);
						if (sensorBTemperature <= -990.0)
						{
							throw std::runtime_error("error");
						}
						else
						{
							envDataB[0] = sensorBTemperature;
							temperatureUpdateFlag = true;

						}
					}
					else
					{
						GetPlug()->AddLog(LOG_TYPE_ERROR,"传感器B返回温度地址异常");
						continue;
					}

				}
			}


			queryCmd[6] = sensorType[GetPlug()->getConfig()->HumidSensor[1].sensorType[1]];
			queryCmd[7] = GetPlug()->getConfig()->HumidSensor[1].attribute[1];
			queryCmd[8] = queryCmd[1] ^ queryCmd[2] ^ queryCmd[3] ^ queryCmd[4] ^ queryCmd[5] ^ queryCmd[6] ^ queryCmd[7];

			if(!humidityDataUpdateFlag)
			{
				sendto(sockServer, queryCmd, queryCmdLen, 0, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));
				char result[20] = { '\0' };

				int len = recvfrom(sockServer, result, 20, 0, (SOCKADDR*)&client, &socklen);
				if (len != expectedRetLen)
				{
					throw std::runtime_error(std::to_string((unsigned long long)len));
				}
				else
				{
					if(((unsigned char)result[5] == GetPlug()->getConfig()->HumidSensor[1].id) &&                
						((unsigned char)result[6] == sensorType[GetPlug()->getConfig()->HumidSensor[1].sensorType[1]]) && 
						((unsigned char)result[7] == GetPlug()->getConfig()->HumidSensor[1].attribute[1]))
					{
						FLOAT sensorBHumidity = 0;
						memcpy(&sensorBHumidity, result + 8, 4);
						if (sensorBHumidity <= -990.0)
						{
							throw std::runtime_error("error");
						}
						else
						{
							envDataB[1] = sensorBHumidity;
							humidityDataUpdateFlag = true;

						}
					}
					else
					{
						GetPlug()->AddLog(LOG_TYPE_ERROR,"传感器B返回湿度地址异常");
						continue;
					}

				}					
			}


			if (isSensorBTDataValid(envDataB) && isSensorBHDataValid(envDataB))
			{
				temperatureUpdateFlag = false;
				humidityDataUpdateFlag = false;
				
				closesocket(sockServer);
				return EXIT_SUCCESS;

			}
			else
			{
				temperatureUpdateFlag = false;
				humidityDataUpdateFlag = false;
				continue;
			}

		}
		catch (std::runtime_error& e)
		{
			int len = std::atoi(e.what());
			if (len = -1)
			{
				continue;
			}
			else
			{
				SOCKADDR_IN client;
				int socklen = sizeof(SOCKADDR);
				char result[20] = { 0 };

				while (len != -1)
				{
					len = recvfrom(sockServer, result, 20, 0, (SOCKADDR*)&client, &socklen);
				}
			}

		}
	}

		closesocket(sockServer);
		return EXIT_FAILURE;

}



