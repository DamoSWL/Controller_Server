#include "EnvAlarm.h"
#include <ctime>
#include "mysqldatabase.h"



CEnvAlarm::CEnvAlarm():temperatureUpper(0.0),
temperatureLower(0.0),
humidityUpper(0.0),	
humidityLower(0.0),
m_isConnectedFlagA(true),
m_setFirstTimeFlagA(true),
m_isConnectedFlagB(true),
m_setFirstTimeFlagB(true),
m_sensorATemperatureExp(false),
m_sensorAHumidityExp(false),
m_sensorBTemperatureExp(false),
m_sensorBHumidityExp(false)
{

	m_EnvContent["engineRoomTempA"] = "机房温度";
	m_EnvContent["engineRoomHumidityA"] = "机房湿度";
	m_EnvContent["engineRoomTempB"] = "机房温度B";
	m_EnvContent["engineRoomHumidityB"] = "机房湿度B";

}


CEnvAlarm::~CEnvAlarm()
{
}


void CEnvAlarm::getAlarmType()
{
	/*wxString sqlError;
	MySqlDatabase* pDataBase = GetPlug()->getDatabase();
	wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
	try
	{
	char* sqlQuery = "SELECT type FROM thresholdparameter WHERE eName = 'engineRoomTempA'";
	int resultNum = pDataBase->ExecuteQuery(sqlQuery);

	if(resultNum != 0)
	{
	for(int index = 0; index < resultNum; index++)
	{
	int length = 0;
	char* sqlTmpStr = pDataBase->GetStringValue(index,"type",length);
	if(sqlTmpStr)
	{
	m_envType.assign(sqlTmpStr,strlen(sqlTmpStr));
	}

	}
	pDataBase->FreeResult(sqlError);
	}


	}
	catch (wxString& error)
	{
	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
	}

	}*/
	m_envType = "附属设备";
}

void CEnvAlarm::connectionAlarmA()
{
	m_isConnectedFlagA = false;
	if (m_setFirstTimeFlagA)
	{
		m_EnvEndureTime["ConnectA"].first = ::wxDateTime::GetTimeNow();
		m_setFirstTimeFlagA = false;
		m_EnvContent["ConnectA"] = "传感器A连接异常";
		m_EnvConcreteContent["ConnectA"] = m_EnvContent["ConnectA"];
		writeToDataBase("ConnectA");
	}
}

void CEnvAlarm::connectionAlarmB()
{
	m_isConnectedFlagB = false;
	if (m_setFirstTimeFlagB)
	{
		m_EnvEndureTime["ConnectB"].first = ::wxDateTime::GetTimeNow();
		m_setFirstTimeFlagB = false;
		m_EnvContent["ConnectB"] = "传感器B连接异常";
		m_EnvConcreteContent["ConnectB"] = m_EnvContent["ConnectB"];
		writeToDataBase("ConnectB");
	}
}

void CEnvAlarm::checkSensorA(const std::vector<FLOAT>& data)
{

// 	if (!m_isConnectedFlagA)
// 	{
// 		m_isConnectedFlagA = true;
// 		m_setFirstTimeFlagA = true;
// 		m_EnvEndureTime["ConnectA"].second = ::wxDateTime::GetTimeNow();
// 		writeToDataBasewithEndTime("ConnectA");
// 	}

	if(m_envLevel < 0)
	{
		return;
	}

	if (m_sensorATemperatureExp)
	{
		if ((data[0] <= temperatureUpper) && (data[0] >= temperatureLower))
		{
			m_sensorATemperatureExp = false;
			m_EnvEndureTime["engineRoomTempA"].second = ::wxDateTime::GetTimeNow();
			writeToDataBasewithEndTime("engineRoomTempA");
		}
	}
	else
	{
		if (data[0] > temperatureUpper)
		{
			m_sensorATemperatureExp = true;
			m_EnvEndureTime["engineRoomTempA"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomTempA"] = m_EnvContent["engineRoomTempA"];
			m_EnvConcreteContent["engineRoomTempA"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[0],temperatureLower,temperatureUpper).ToStdString();
			writeToDataBase("engineRoomTempA");

		}
		else if (data[0] < temperatureLower)
		{
			m_sensorATemperatureExp = true;
			m_EnvEndureTime["engineRoomTempA"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomTempA"] = m_EnvContent["engineRoomTempA"];
			m_EnvConcreteContent["engineRoomTempA"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[0],temperatureLower,temperatureUpper).ToStdString();
			writeToDataBase("engineRoomTempA");

		}
		
	}

	if (m_sensorAHumidityExp)
	{
		if ((data[1] <= humidityUpper) && (data[1] >= humidityLower))
		{
			m_sensorAHumidityExp = false;
			m_EnvEndureTime["engineRoomHumidityA"].second = ::wxDateTime::GetTimeNow();
			writeToDataBasewithEndTime("engineRoomHumidityA");
		}
	}
	else
	{
		if (data[1] > humidityUpper)
		{
			m_sensorAHumidityExp = true;
			m_EnvEndureTime["engineRoomHumidityA"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomHumidityA"] = m_EnvContent["engineRoomHumidityA"];
			m_EnvConcreteContent["engineRoomHumidityA"]+= wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[1],humidityLower,humidityUpper).ToStdString();
			writeToDataBase("engineRoomHumidityA");

		}
		else if (data[1] < humidityLower)
		{
			m_sensorAHumidityExp = true;
			m_EnvEndureTime["engineRoomHumidityA"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomHumidityA"] = m_EnvContent["engineRoomHumidityA"];
			m_EnvConcreteContent["engineRoomHumidityA"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[1],humidityLower,humidityUpper).ToStdString();
			writeToDataBase("engineRoomHumidityA");

		}
	
	}

}

void CEnvAlarm::checkSensorB(const std::vector<FLOAT>& data)
{
// 	if (!m_isConnectedFlagB)
// 	{
// 		m_isConnectedFlagB = true;
// 		m_setFirstTimeFlagB = true;
// 		m_EnvEndureTime["ConnectA"].second = ::wxDateTime::GetTimeNow();
// 		writeToDataBasewithEndTime("ConnectB");
// 	}

	if (m_sensorBTemperatureExp)
	{
		if ((data[0] <= temperatureUpper) && (data[0] >= temperatureLower))
		{
			m_sensorBTemperatureExp = false;
			m_EnvEndureTime["engineRoomTempB"].second = ::wxDateTime::GetTimeNow();
			writeToDataBasewithEndTime("engineRoomTempB");
			
		}
	}
	else
	{
		if (data[0] > temperatureUpper)
		{
			m_sensorBTemperatureExp = true;
			m_EnvEndureTime["engineRoomTempB"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomTempB"] = m_EnvContent["engineRoomTempB"];
			m_EnvConcreteContent["engineRoomTempB"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[0],temperatureLower,temperatureUpper).ToStdString();
			writeToDataBase("engineRoomTempB");

		}
		else if (data[0] < temperatureLower)
		{
			m_sensorBTemperatureExp = true;
			m_EnvEndureTime["engineRoomTempB"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomTempB"] = m_EnvContent["engineRoomTempB"];
			m_EnvConcreteContent["engineRoomTempB"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[0],temperatureLower,temperatureUpper).ToStdString();
			writeToDataBase("engineRoomTempB");

		}
		
	}

	if (m_sensorBHumidityExp)
	{
		if ((data[1] <= humidityUpper) && (data[1] >= humidityLower))
		{
			m_sensorBHumidityExp = false;
			m_EnvEndureTime["engineRoomHumidityB"].second = ::wxDateTime::GetTimeNow();
			writeToDataBasewithEndTime("engineRoomHumidityB");
		}
	}
	else
	{
		if (data[1] > humidityUpper)
		{
			m_sensorBHumidityExp = true;
			m_EnvEndureTime["engineRoomHumidityB"].first = ::wxDateTime::GetTimeNow();
			m_EnvConcreteContent["engineRoomHumidityB"] = m_EnvContent["engineRoomHumidityB"];
			m_EnvConcreteContent["engineRoomHumidityB"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[1],humidityLower,humidityUpper).ToStdString();
			writeToDataBase("engineRoomHumidityB");

		}
		else if (data[1] < humidityLower)
		{
			m_sensorBHumidityExp = true;
			m_EnvConcreteContent["engineRoomHumidityB"] = m_EnvContent["engineRoomHumidityB"];
			m_EnvConcreteContent["engineRoomHumidityB"] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),data[1],humidityLower,humidityUpper).ToStdString();
			writeToDataBase("engineRoomHumidityB");

		}

	}

}

bool CEnvAlarm::writeToDataBase(const std::string name)
{

	wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
																	endTime, \
																	type, \
																	alarmName, \
																	level, \
																	mode, \
																	content, \
																	origin) VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_EnvEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_envType)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_envLevel));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_EnvConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));

			return pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

bool CEnvAlarm::writeToDataBasewithEndTime(const std::string name)
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			wxString sqlSentence; 
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_EnvEndureTime[name].second,
																															m_EnvEndureTime[name].first,
																															wxString(m_EnvConcreteContent[name].c_str(),wxConvLocal));

			return pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

int CEnvAlarm::cbSQLGetUpperTemperature(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CEnvAlarm *pT = static_cast<CEnvAlarm*>(pIn);
	if(pT)
	{
		pT->temperatureUpper = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CEnvAlarm::cbSQLGetLowerTemperature(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CEnvAlarm *pT = static_cast<CEnvAlarm*>(pIn);
	if(pT)
	{
		pT->temperatureLower = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CEnvAlarm::cbSQLGetUpperHumidity(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CEnvAlarm *pT = static_cast<CEnvAlarm*>(pIn);
	if(pT)
	{
		pT->humidityUpper = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CEnvAlarm::cbSQLGetLowerHumidity(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CEnvAlarm *pT = static_cast<CEnvAlarm*>(pIn);
	if(pT)
	{
		pT->humidityLower = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CEnvAlarm::cbSQLGetAlarmLevel(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CEnvAlarm *pT = static_cast<CEnvAlarm*>(pIn);
	if(pT)
	{
		pT->m_envLevel = std::stoi(std::string(argv[0]));
	}

	return 0;
}

void CEnvAlarm::getThresholdValue()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {

			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256,"SELECT lowerLimit FROM thresholdparameter WHERE eName = 'engineRoomTempA'");
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetLowerTemperature,wxT("lowerLimit"),this);
			
			sprintf_s(sqlQuery,256,"SELECT upperLimit FROM thresholdparameter WHERE eName = 'engineRoomTempA'");
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetUpperTemperature,wxT("upperLimit"),this);
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		temperatureUpper = pDataBase->GetFloatValue(index,"upperLimit");
			// 		temperatureLower = pDataBase->GetFloatValue(index,"lowerLimit");
			// 	}
			// 	pDataBase->FreeResult(sqlError);
			// }
			sprintf_s(sqlQuery,256,"SELECT lowerLimit FROM thresholdparameter WHERE eName = 'engineRoomHumidityA'");
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetLowerHumidity,wxT("lowerLimit"),this);
			
			sprintf_s(sqlQuery,256,"SELECT upperLimit FROM thresholdparameter WHERE eName = 'engineRoomHumidityA'");
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetUpperHumidity,wxT("upperLimit"),this);

			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		humidityUpper = pDataBase->GetFloatValue(index,"upperLimit");
			// 		humidityLower = pDataBase->GetFloatValue(index,"lowerLimit");
			// 	}
			// 	pDataBase->FreeResult(sqlError);
			// }

		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }

	}

}

void CEnvAlarm::getAlarmLevel()
{
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{

			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256,"SELECT level FROM thresholdparameter WHERE eName = 'engineRoomTempA'");
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmLevel,wxT("level"),this);	

	}
}
