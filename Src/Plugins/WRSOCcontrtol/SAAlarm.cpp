#include "SAAlarm.h"


CSAAlarm::CSAAlarm(void):m_polarType(UNIPOLE)
{

    m_SAExpFlag["rf_power_smoothing"] = false;
    m_SAExpFlag["hxmtlosssingle"] = false;
    m_SAExpFlag["hxmtlosshv"] = false;
    m_SAExpFlag["hrcvloss"] = false;
    m_SAExpFlag["htestloss"] = false;
    m_SAExpFlag["vxmtlosssingle"] = false;
    m_SAExpFlag["vxmtlosshv"] = false;
    m_SAExpFlag["vrcvloss"] = false;
    m_SAExpFlag["vtestloss"] = false;
    m_SAExpFlag["pathloss_noise_h"] = false;
    m_SAExpFlag["pathloss_noise_v"] = false;
    m_SAExpFlag["ts_cw"] = false;
    m_SAExpFlag["ts_noise"] = false;
    m_SAExpFlag["pathloss_rf"] = false;
    m_SAExpFlag["ts_rf"] = false;
    m_SAExpFlag["pathloss_kly"] = false;
    m_SAExpFlag["noise_smoothing"] = false;
    m_SAExpFlag["gas_loss"] = false;
    m_SAExpFlag["k1"] = false;
    m_SAExpFlag["k2"] = false;
    m_SAExpFlag["k3"] = false;
    m_SAExpFlag["k4"] = false;
	m_SAExpFlag["radome_loss"] = false;
	

	m_SAContent["rf_power_smoothing"] = "射频功率平滑系数";
	m_SAContent["hxmtlosssingle"] = "水平发射水平通道损耗";
	m_SAContent["hxmtlosshv"] = "水平发射双通道损耗";
	m_SAContent["hrcvloss"] = "水平通道接收支路损耗";
	m_SAContent["htestloss"] = "水平通道测试信号损耗";
	m_SAContent["vxmtlosssingle"] = "垂直发射垂直通道损耗";
	m_SAContent["vxmtlosshv"] = "垂直发射双通道损耗";
	m_SAContent["vrcvloss"] = "垂直通道接收支路损耗";
	m_SAContent["vtestloss"] = "垂直通道测试信号损耗";
	m_SAContent["pathloss_noise_h"] = "水平射频噪声信号损耗";
	m_SAContent["pathloss_noise_v"] = "垂直射频噪声信号损耗";
	m_SAContent["ts_cw"] = "连续波测试信号强度";
	m_SAContent["ts_noise"] = "噪声源测试信号超噪比";
	m_SAContent["pathloss_rf"] = "射频激励信号路径损耗";
	m_SAContent["ts_rf"] = "射频激励信号强度";
	m_SAContent["pathloss_kly"] = "速调管输出信号损耗";
	m_SAContent["noise_smoothing"] = "噪声标定平滑系数";
	m_SAContent["gas_loss"] = "大气衰减值";
	m_SAContent["k1"] = "方位位置增益因子";
	m_SAContent["k2"] = "方位驱动增益因子";
	m_SAContent["k3"] = "俯仰位置增益因子";
	m_SAContent["k4"] = "俯仰驱动增益因子";
	m_SAContent["radome_loss"] = "天线罩损耗";

	m_alarmType["rf_power_smoothing"] = std::string("");
	m_alarmType["hxmtlosssingle"] = std::string("");
	m_alarmType["hxmtlosshv"] = std::string("");
	m_alarmType["hrcvloss"] = std::string("");
	m_alarmType["htestloss"] = std::string("");
	m_alarmType["vxmtlosssingle"] = std::string("");
	m_alarmType["vxmtlosshv"] = std::string("");
	m_alarmType["vrcvloss"] = std::string("");
	m_alarmType["vtestloss"] = std::string("");
	m_alarmType["pathloss_noise_h"] = std::string("");
	m_alarmType["pathloss_noise_v"] = std::string("");
	m_alarmType["ts_cw"] = std::string("");
	m_alarmType["ts_noise"] = std::string("");
	m_alarmType["pathloss_rf"] = std::string("");
	m_alarmType["ts_rf"] = std::string("");
	m_alarmType["pathloss_kly"] = std::string("");
	m_alarmType["noise_smoothing"] = std::string("");
	m_alarmType["gas_loss"] = std::string("");
	m_alarmType["k1"] = std::string("");
	m_alarmType["k2"] = std::string("");
	m_alarmType["k3"] = std::string("");
	m_alarmType["k4"] = std::string("");
	m_alarmType["radome_loss"] = std::string("");

	m_ThreadholdValues["rf_power_smoothing"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hxmtlosssingle"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hxmtlosshv"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hrcvloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["htestloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vxmtlosssingle"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vxmtlosshv"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vrcvloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vtestloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_noise_h"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_noise_v"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ts_cw"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ts_noise"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_rf"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ts_rf"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_kly"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_smoothing"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["gas_loss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["k1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["k2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["k3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["k4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["radome_loss"] = std::make_pair(0.0,0.0);

}


CSAAlarm::~CSAAlarm(void)
{
}

void CSAAlarm::getAlarmType()
{
    // wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);

	if(pDataBase)
	{	
		// try
		// {

			
			for(auto item = m_alarmType.cbegin(); item != m_alarmType.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256,"SELECT type FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmType,wxT("type"),this);	
			}
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		int length = 0;
			// 		char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
			// 		if(sqlTmpStr)
			// 		{
			// 			std::string eName(sqlTmpStr,strlen(sqlTmpStr));
			// 			sqlTmpStr = pDataBase->GetStringValue(index,"type",length);
			// 			if(sqlTmpStr)
			// 			{
			// 				std::string type(sqlTmpStr,strlen(sqlTmpStr));
			// 				m_alarmType[eName] = type;
			// 			}
			// 		}
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

void CSAAlarm::getThresholdValue()
{
    // wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);

	if (pDataBase)
	{
		// try
		// {
			for(auto item = m_ThreadholdValues.cbegin(); item != m_ThreadholdValues.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256, "SELECT lowerLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetLowerValue,wxT("lowerLimit"),this);
	
	
				sprintf_s(sqlQuery,256, "SELECT upperLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetUpperValue,wxT("upperLimit"),this);
			}
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		int length = 0;
            //         char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
            //         {
            //             if(sqlTmpStr)
            //             {
            //                 std::string eName(sqlTmpStr,strlen(sqlTmpStr));
            //                 std::pair<FLOAT,FLOAT> pair;
            //                 pair.first = pDataBase->GetFloatValue(index,"lowerLimit");
            //                 pair.second = pDataBase->GetFloatValue(index,"upperLimit");
            //                 m_ThreadholdValues[eName] = pair;
            //             }
            //         }
					
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

void CSAAlarm::getSAValue(const RadarStructure::SAStructrue& value)
{
	m_SAValues.clear();

	if(::wxFinite (value.RF_POWER_SMOOTHING))
	{
		m_SAValues["rf_power_smoothing"] = value.RF_POWER_SMOOTHING;
	}
	else 
	{
		m_SAValues["rf_power_smoothing"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.HXMTLOSSSINGLE))
	{
		m_SAValues["hxmtlosssingle"] = value.HXMTLOSSSINGLE;
	}
	else
	{
		m_SAValues["hxmtlosssingle"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.HXMTLOSSHV))
	{
		m_SAValues["hxmtlosshv"] = value.HXMTLOSSHV;
	}
	else
	{
		m_SAValues["hxmtlosshv"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.HRCVLOSS))
	{
		m_SAValues["hrcvloss"] = value.HRCVLOSS;
	}
	else
	{
		m_SAValues["hrcvloss"] = EXCEPTIONDATA;
	}
   
	if(::wxFinite (value.HTESTLOSS))
	{
		m_SAValues["htestloss"] = value.HTESTLOSS;
	}
	else
	{
		m_SAValues["htestloss"] = EXCEPTIONDATA;
	}
	
	if(m_polarType == BIPOLE)
	{
		if(::wxFinite (value.VXMTLOSSSINGLE))
		{
			m_SAValues["vxmtlosssingle"] = value.VXMTLOSSSINGLE;
		}
		else
		{
			m_SAValues["vxmtlosssingle"] = EXCEPTIONDATA;
		}
	}

	if(m_polarType == BIPOLE)
	{
		if(::wxFinite (value.VXMTLOSSHV))
		{
			m_SAValues["vxmtlosshv"] = value.VXMTLOSSHV;
		}
		else
		{
			m_SAValues["vxmtlosshv"] = EXCEPTIONDATA;
		}
	}


	if(m_polarType == BIPOLE)
	{
		if(::wxFinite (value.VRCVLOSS))
		{
			m_SAValues["vrcvloss"] = value.VRCVLOSS;
		}
		else
		{
			m_SAValues["vrcvloss"] = EXCEPTIONDATA;
		}
	}
	

	if(m_polarType == BIPOLE)
	{
		if(::wxFinite (value.VTESTLOSS))
		{
			m_SAValues["vtestloss"] = value.VTESTLOSS;
		}
		else
		{
			m_SAValues["vtestloss"] = EXCEPTIONDATA;
		}
	}

  
	if(::wxFinite (value.PATHLOSS_NOISE_H))
	{
		m_SAValues["pathloss_noise_h"] = value.PATHLOSS_NOISE_H;
	}
	else
	{
		m_SAValues["pathloss_noise_h"] = EXCEPTIONDATA;
	}

	if(m_polarType == BIPOLE)
	{
		if(::wxFinite (value.PATHLOSS_NOISE_V))
		{
			m_SAValues["pathloss_noise_v"] = value.PATHLOSS_NOISE_V;
		}
		else
		{
			m_SAValues["pathloss_noise_v"] = EXCEPTIONDATA;
		}
	}


	if(::wxFinite (value.TS_CW))
	{
		m_SAValues["ts_cw"] = value.TS_CW;
	}
	else
	{
		m_SAValues["ts_cw"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.TS_NOISE))
	{
		m_SAValues["ts_noise"] = value.TS_NOISE;
	}
	else
	{
		m_SAValues["ts_noise"] = EXCEPTIONDATA;
	}
  
	if(::wxFinite (value.PATHLOSS_RF))
	{
		m_SAValues["pathloss_rf"] = value.PATHLOSS_RF;
	}
	else
	{
		m_SAValues["pathloss_rf"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.TS_RF))
	{
		m_SAValues["ts_rf"] = value.TS_RF;
	}
	else
	{
		m_SAValues["ts_rf"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.PATHLOSS_KLY))
	{
		m_SAValues["pathloss_kly"] = value.PATHLOSS_KLY;
	}
	else
	{
		m_SAValues["pathloss_kly"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.NOISE_SMOOTHING))
	{
		m_SAValues["noise_smoothing"] = value.NOISE_SMOOTHING;
	}
	else
	{
		m_SAValues["noise_smoothing"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.GAS_LOSS))
	{
		m_SAValues["gas_loss"] = value.GAS_LOSS;
	}
	else
	{
		m_SAValues["gas_loss"] = EXCEPTIONDATA;
	}
   
	if(::wxFinite (value.K1))
	{
		m_SAValues["k1"] = value.K1;
	}
	else
	{
		m_SAValues["k1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.K2))
	{
		m_SAValues["k2"] = value.K2;
	}
	else
	{
		m_SAValues["k2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.K3))
	{
		m_SAValues["k3"] = value.K3;
	}
	else
	{
		m_SAValues["k3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.K4))
	{
    	m_SAValues["k4"] = value.K4;
	}
	else
	{
		m_SAValues["k4"] = EXCEPTIONDATA;
	}

	if(::wxFinite (value.RADOME_LOSS))
	{
    	m_SAValues["radome_loss"] = value.RADOME_LOSS;
	}
	else
	{
		m_SAValues["radome_loss"] = EXCEPTIONDATA;
	}

}

void CSAAlarm::SACheck()
{
    for(auto item = m_SAValues.cbegin();item != m_SAValues.cend(); item++)
	{
		auto name = item->first;
		if(m_ThreadholdValues.find(name) != m_ThreadholdValues.cend())
		{
			auto value = m_SAValues[name];
			auto lowerLimit = m_ThreadholdValues[name].first;
			auto upperLimit = m_ThreadholdValues[name].second;


			if((std::fabs(lowerLimit) < ZEROBIAS) && (std::fabs(upperLimit) < ZEROBIAS))
			{
				continue;
			}

			if(m_SAExpFlag[name])
			{
				if((value <= upperLimit) || (value >= lowerLimit))
				{
					m_SAExpFlag[name] = false;
					m_SAEndureTime[name].second = ::wxDateTime::GetTimeNow();
					writeToDataBasewithEndTime(name);
				}

			}
			else
			{
				if(value > upperLimit)
				{
					m_SAExpFlag[name] = true;
					m_SAEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SAConcreteContent[name] = m_SAContent[name];
					m_SAConcreteContent[name]  += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),m_SAValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);

				}
				else if(value < lowerLimit)
				{
					m_SAExpFlag[name] = true;
					m_SAEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SAConcreteContent[name] = m_SAContent[name];
					m_SAConcreteContent[name] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),m_SAValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);
				}
			}
		}

	}
}

bool CSAAlarm::writeToDataBase(const std::string name)
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
	// try
	// {
		if(pDataBase)
		{
			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_SAEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "),  wxString(m_alarmType[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 1));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_SAConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));
		}



		return 	pDataBase->RunSQLNoQuery(sqlSentence);
	// }
	// catch (wxString& error)
	// {
	// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
	// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
	// }

	return false;
}

bool CSAAlarm::writeToDataBasewithEndTime(const std::string name)
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
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_SAEndureTime[name].second,
																										m_SAEndureTime[name].first,
																										wxString(m_SAConcreteContent[name].c_str(),wxConvLocal));


			return 	pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

int CSAAlarm::cbSQLGetAlarmType(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSAAlarm *pT = static_cast<CSAAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmType(std::string(argv[0]));
	}

	return 0;
}

int CSAAlarm::cbSQLGetLowerValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSAAlarm *pT = static_cast<CSAAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetLowerValue(std::string(argv[0]));
	}

	return 0;
}

int CSAAlarm::cbSQLGetUpperValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSAAlarm *pT = static_cast<CSAAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetUpperValue(std::string(argv[0]));
	}

	return 0;
}
