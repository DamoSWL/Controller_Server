
#include "UPSAlarm.h"
#include <memory>
#include "mysqldatabase.h"




CUPSAlarm::CUPSAlarm()
{
	m_UPSExpFlag["inputVoltage"] = false;
	m_UPSExpFlag["outputVoltage"] = false;
	m_UPSExpFlag["inputFre"] = false;
	m_UPSExpFlag["outputFre"] = false;
	m_UPSExpFlag["batteryCellVoltage"] = false;
	m_UPSExpFlag["batteryCapacity"] = false;
	m_UPSExpFlag["batteryCurrent"] = false;
	
	m_UPSContent["inputVoltage"] = "ups输入电压";
	m_UPSContent["outputVoltage"] = "ups输出电压";
	m_UPSContent["inputFre"] = "ups输入频率";
	m_UPSContent["outputFre"] = "ups输出频率";
	m_UPSContent["batteryCellVoltage"] = "ups电池电压";
	m_UPSContent["batteryCapacity"] = "ups电池容量";
	m_UPSContent["batteryCurrent"] = "ups电池电流";
}



CUPSAlarm::~CUPSAlarm()
{
}


void CUPSAlarm::getAlarmType()
{
	/*wxString sqlError;
	MySqlDatabase* pDataBase = GetPlug()->getDatabase();
	wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
	try
	{

	char* sqlQuery = "SELECT type FROM thresholdparameter WHERE eName = 'inputVoltage'";
	int resultNum = pDataBase->ExecuteQuery(sqlQuery);

	if(resultNum != 0)
	{
	for(int index = 0; index < resultNum; index++)
	{
	int length = 0;
	char* sqlTmpStr = pDataBase->GetStringValue(index,"type",length);
	if(sqlTmpStr)
	{
	m_alarmType.assign(sqlTmpStr,strlen(sqlTmpStr));
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
}

void CUPSAlarm::getThresholdValue()
{
	/*wxString sqlError;
	MySqlDatabase* pDataBase = GetPlug()->getDatabase();
	wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	wxMutexLocker locker(*sqlLocker);

	if (pDataBase)
	{
	try
	{
	char* sqlQuery = "SELECT upperLimit, lowerLimit, eName FROM thresholdparameter WHERE tableName = radarrunenvirement'";
	int resultNum = pDataBase->ExecuteQuery(sqlQuery);
	if(resultNum != 0)
	{
	for(int index = 0; index < resultNum; index++)
	{
	int length = 0;
	char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
	{
	if(sqlTmpStr)
	{
	std::string eName(sqlTmpStr,strlen(sqlTmpStr));
	std::pair<FLOAT,FLOAT> pair;
	pair.first = pDataBase->GetFloatValue(index,"lowerLimit");
	pair.second = pDataBase->GetFloatValue(index,"upperLimit");
	m_ThreadholdValues[eName] = pair;
	}
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
	
}

void CUPSAlarm::getUPSValue(const UPSHandlerSNMP::tagUPSStatus& value)
{
	m_UPSValues["inputVoltage"] = value.inVoltage;
	m_UPSValues["outputVoltage"] =  value.outVoltage;
	m_UPSValues["inputFre"] = value.inFreq;
	m_UPSValues["batteryCellVoltage"] = value.batteryEleVoltage * 6;
	
}

void CUPSAlarm::UPSCheck(const UPSHandlerSNMP::tagUPSStatus & data)
{

	if (!m_isConnectedFlag)
	{
		m_isConnectedFlag = true;
		m_setFirstTimeFlag = true;
		m_UPSEndureTime["Connect"].second = ::wxDateTime::GetTimeNow();
		writeToDataBasewithEndTime("Connect");
	}


	for(auto item = m_UPSValues.cbegin();item != m_UPSValues.cend(); item++)
	{
		auto name = item->first;
		if(m_ThreadholdValues.find(name) != m_ThreadholdValues.cend())
		{
			auto value = m_UPSValues[name] * 1.0;
			auto lowerLimit = m_ThreadholdValues[name].first;
			auto upperLimit = m_ThreadholdValues[name].second;

			if(m_UPSExpFlag[name])
			{
				if((value <= upperLimit) || (value >= lowerLimit))
				{
					m_UPSExpFlag[name] = false;
					m_UPSEndureTime[name].second = ::wxDateTime::GetTimeNow();
					writeToDataBasewithEndTime(name);
				}

			}
			else
			{
				if(value > upperLimit)
				{
					m_UPSExpFlag[name] = true;
					m_UPSEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_UPSConcreteContent[name] = m_UPSContent[name];
					m_UPSConcreteContent[name].append("取样值过高");
					writeToDataBase(name);

				}
				else if(value < lowerLimit)
				{
					m_UPSExpFlag[name] = true;
					m_UPSEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_UPSConcreteContent[name] = m_UPSContent[name];
					m_UPSConcreteContent[name].append("取样值过低");
					writeToDataBase(name);
				}
			}
		}

	}

}

void CUPSAlarm::connectAlarm()
{
	m_isConnectedFlag = false;
	if(m_setFirstTimeFlag)
	{
		m_UPSEndureTime["Connect"].first = ::wxDateTime::GetTimeNow();
		m_setFirstTimeFlag = false;
		m_UPSContent["Connect"] = "UPS连接异常";
		m_UPSConcreteContent["Connect"] = m_UPSContent["Connect"];
		writeToDataBase("Connect");

	}
}


bool CUPSAlarm::writeToDataBase(const std::string name)
{
	/*wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
																	endTime, \
																	type, \
																	alarmName, \
																	level, \
																	mode, \
																	content, \
																	origin) VALUES");
	wxString sqlError;
	MySqlDatabase* pDataBase = GetPlug()->getDatabase();
	wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		try
		{

			sqlSentence.Append(wxString::Format(wxT("(%d, "), m_UPSEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_alarmType.c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 1));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(m_UPSConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 2));

			
			if(!pDataBase->ExecuteNoQuery(sqlSentence,sqlError))
			{
				throw sqlError;
			}
			return true;
		}
		catch (wxString& error)
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
			GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		}
	}*/
		return false;
}

bool CUPSAlarm::writeToDataBasewithEndTime(const std::string name)
{

	
	/*wxString sqlError;
	MySqlDatabase* pDataBase = GetPlug()->getDatabase();
	wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		try
		{
			wxString sqlSentence;
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%d WHERE endTime IS NULL AND time=%d AND content='%s'"), m_UPSEndureTime[name].second,
																															m_UPSEndureTime[name].first,
																															wxString(m_UPSConcreteContent[name].c_str(),wxConvLocal));
			if(!pDataBase->ExecuteNoQuery(sqlSentence,sqlError))
			{
				throw sqlError;
			}
			return true;
		}
		catch (wxString& error)
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
			GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		}
	}*/
	return false;
}
