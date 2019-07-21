
#include "RadarRunEnvironment.h"
#include "mysqldatabase.h"


DWORD WINAPI CRadarRunEnvironment::EnvironmentExec(LPVOID para)
{
	CRadarRunEnvironment* self = static_cast<CRadarRunEnvironment*>(para);
	if(self)
	{
		while(!GetPlug()->GetExit())
		{
			self->setUpdateTime();
			if(self->getEnvFromSensorA() == EXIT_SUCCESS)
			{
				self->setEnvADataFlag(true);
				self->resetSensorADisconnectCnt();

				GetPlug()->AddLog(LOG_TYPE_MESSAGE,"sensorA insert");
			}
			else
			{
				self->setEnvADataFlag(false);
				self->addDisconnectionA();
			}

			if(self->getEnvFromSensorB() == EXIT_SUCCESS)
			{
				self->setEnvBDataFlag(true);
				self->resetSensorBDisconnectCnt();

				GetPlug()->AddLog(LOG_TYPE_MESSAGE,"sensorB insert");
			}
			else
			{
				self->setEnvBDataFlag(false);
				self->addDisconnectionB();
			}

			self->processEnvData();
			
			for(auto index = 0; (index <= 10) && (!GetPlug()->GetExit()); index++)
			{
				::wxSleep(6);
			}
		}

	}

	return EXIT_SUCCESS;
}

CRadarRunEnvironment::CRadarRunEnvironment():
											m_updateTime(0),
											m_sensorADisconnectedCount(0),
											m_sensorBDisconnectedCount(0),
											m_UPSDisconnectedCount(0),
											m_newEnvADataFlag(false),
											m_newEnvBDataFlag(false),
											m_newUPSDataFlag(false),
											m_envDataA(2,0),
											m_envDataB(2,0)
{

}


CRadarRunEnvironment::~CRadarRunEnvironment()
{

}




	//m_UPSController.sendUPSCmd();

	

bool CRadarRunEnvironment::writeToDataBase()
{
	wxString sqlSentence = wxT("INSERT INTO radarrunenvirement (updatetime, \
																engineRoomTempA, \
																engineRoomHumidityA, \
																engineRoomTempB, \
																engineRoomHumidityB,  \
																inputVoltage, \
																dischargedVoltage, \
																outputVoltage,  \
																loads, \
																inputFre, \
																batteryCellVoltage,\
																temperature,  \
																batteryCapacity,  \
																batteryCurrent,  \
																upsElecAbnormal, \
																upsLowBV, \
																bypassMode,\
																upsFault,  \
																upsMode, \
																upsTesting, \
																upsShutdown) VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			
			sqlSentence.Append(wxString::Format(wxT("(%d, "),static_cast<UINT>(m_updateTime)));
			
			if(m_newEnvADataFlag)
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_envDataA.at(0)));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_envDataA.at(1)));
			}
			else
			{
				return false;
			}
		
			if(m_newEnvBDataFlag)
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_envDataB.at(0)));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_envDataB.at(1)));
			}
			else
			{
				return false;
			}
	
			if(m_newUPSDataFlag)
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.inVoltage));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.lastBatteryVoltage));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.outVoltage));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.outLoadPercentage*1.0));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.inFreq));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.batteryEleVoltage));
				sqlSentence.Append(wxString::Format(wxT("%f, "),m_upsData.temperature));
				sqlSentence.Append(wxString::Format(wxT("%f, "),0.0));
				sqlSentence.Append(wxString::Format(wxT("%f, "),0.0));
				sqlSentence.Append(wxString::Format(wxT("%d, "),m_upsData.bElectricSupply));
				sqlSentence.Append(wxString::Format(wxT("%d, "),m_upsData.bBatteryVoltage));
				sqlSentence.Append(wxString::Format(wxT("%d, "),m_upsData.bBypassModel));
				sqlSentence.Append(wxString::Format(wxT("%d, "),m_upsData.bUPSStatus));
				sqlSentence.Append(wxString::Format(wxT("%d, "),m_upsData.bUPSMode));
				sqlSentence.Append(wxString::Format(wxT("%d, "),m_upsData.bTesting));
				sqlSentence.Append(wxString::Format(wxT("%d)"),m_upsData.bShutdown));
			}
			else
			{
				for(int index = 0;index < 15; index++)
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "),0.0));
				}
				sqlSentence.Append(wxString::Format(wxT("%f)"),0.0));
			}


			return 	pDataBase->RunSQLNoQuery(sqlSentence);


		// }
		// catch (wxString &error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}

	return false;
}


//void CRadarRunEnvironment::insertUPSData(const UPSHandler::tagUPSStatus & data)
//{
//	m_upsData = data;
//	m_newUPSDataFlag = true;
//
//	GetPlug()->AddLog(LOG_TYPE_MESSAGE,"UPS insert");
//
//
//}

void CRadarRunEnvironment::addDisconnectionA()
{
	m_sensorADisconnectedCount++;
}

void CRadarRunEnvironment::addDisconnectionB()
{
	m_sensorBDisconnectedCount++;

}

//BOOL CRadarRunEnvironment::startCom()
//{
//	return m_UPSController.StartCom();
//}
//
//BOOL CRadarRunEnvironment::stopCom()
//{
//	return m_UPSController.StopCom();
//}

void CRadarRunEnvironment::processEnvData()
{

	writeToDataBase();
	m_UPSAlarm.getAlarmType();
	m_UPSAlarm.getThresholdValue();

	if(m_newUPSDataFlag)
	{
		//m_UPSAlarm.UPSCheck(m_upsData);
	}
	else
	{
		//m_UPSAlarm.connectAlarm();
	}

	m_envAlarm.getAlarmType();
	m_envAlarm.getThresholdValue();

	if(m_newEnvADataFlag)
	{
		m_envAlarm.checkSensorA(m_envDataA);
	}
	else
	{
		if(m_sensorADisconnectedCount >= 3)
		{
			m_envAlarm.connectionAlarmA();
		}
		
	}

	if(m_newEnvBDataFlag)
	{
		m_envAlarm.checkSensorB(m_envDataB);
	}
	else
	{
		if(m_sensorBDisconnectedCount >= 3)
		{
			m_envAlarm.connectionAlarmB();
		}
	}

	m_newEnvADataFlag = false;
	m_newEnvBDataFlag = false;
	m_newUPSDataFlag = false;
}

