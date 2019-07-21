#include "SpecAirConditionController.h"
#include "ywsocketdefine.h"
#include "NetService.h"


static const int SPECAIRCONTROLLERTIMERID = 280;

BEGIN_EVENT_TABLE(CSpecAirConditionController,wxEvtHandler)
	EVT_TIMER(SPECAIRCONTROLLERTIMERID,CSpecAirConditionController::onAirStatusTimeout)
END_EVENT_TABLE()


CSpecAirConditionController::CSpecAirConditionController(void):m_currentDeviceIndex(0),
																	m_udpHandler(NULL),
																	m_localNetService(NULL),
																	m_airConditionCmdFlag(false),
																	m_Timer(this,SPECAIRCONTROLLERTIMERID),
																	m_SpecialAirConditionerCmdNO(0)
{
}


CSpecAirConditionController::~CSpecAirConditionController(void)
{
}

int CSpecAirConditionController::processData(void *pHandle, const char *pData, UINT Len)
{
	CSpecAirConditionController* self = static_cast<CSpecAirConditionController*>(pHandle);
	if(self)
	{
		if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))  //���ݲ�ͬվ̨����ͬ������Ϣ
		{
			if(Len == 8)
			{
				if((UCHAR)pData[1] == 0x06)
				{
					self->setairConditionFlag(false);  //����ʶλ����Ϊfalse���������յ�����

					CNetService* localNetService = self->getNetService();
					if(localNetService)
					{
						localNetService->sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);  //��java�˷���success���������Ƴɹ�
					}

				}
			}
			else if(Len == 37)
			{
				if((UCHAR)pData[1] == 0x03)
				{
					self->parseAirStatus(pData,Len);  //�����ص�״̬��Ϣ
				}
			}
			else{}
		}

	}

	return 0;
}

void CSpecAirConditionController::processEvent(void *pHandle, int Opt, char *pData /*= nullptr*/, UINT uiLen /*= 0*/)  //�ص�����������ʱû���յ��ظ�������øú���������java�˷���fail
{
	CSpecAirConditionController* self = static_cast<CSpecAirConditionController*>(pHandle);

	if(self)
	{
		switch(Opt)
		{
		case YW_EVENT_RECEIVE_THREAD_TIMEOUT:
			if(self->getairConditonFlag())
			{
				self->setairConditionFlag(false);

				CNetService* localNetService = self->getNetService();
				if(localNetService)
				{
					localNetService->sendDataToFront("FAIL",4,GetPlug()->m_ClientIPAddress.c_str(),9898);//��java�˷���fail
				}					
			}				
			break;
		default:
			break;

		}
	}
}

BOOL CSpecAirConditionController::startReceive()
{
	m_udpHandler = GetPlug()->m_UDPFactory.GetInstance();

	if(m_udpHandler)
	{
		m_udpHandler->RegHandle(this, CSpecAirConditionController::processData,CSpecAirConditionController::processEvent);

		if (m_udpHandler->StartReceive(0, "0.0.0.0", 7878, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"�˿�7878�󶨳ɹ�");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"�˿�7878��ʧ��");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSpecAirConditionController::stopReceive()
{
	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();
		GetPlug()->m_UDPFactory.ReleaseInstance(m_udpHandler);
		m_udpHandler = NULL;
		return TRUE;
	}

	return FALSE;
}

void CSpecAirConditionController::parseAirCmd(const char *pData, UINT Len)  //������java�˷��͵Ŀ���ָ��
{
	std::string cmd(pData,Len);
	INT pos = 0;

	if(cmd.find(":") != std::string::npos)
	{
		m_currentDeviceIndex = std::stoi(cmd.substr(std::strlen("#PATCTL:"),1));  //��ȡ���ܿյ����
	}
	else
	{
		return;
	}

	setairConditionFlag(true);  //��λ���ձ�־λ

	if(cmd.find("Power") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto tmpStr = cmd.substr(pos,cmd.length() - 1 - pos);
		INT flag = std::stoi(tmpStr);

		sendSwitchCmd(flag);  //���Ϳ��ػ�
	}
	else if(cmd.find("Temp") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto tmpStr = cmd.substr(pos,cmd.length() - 1 - pos);
		INT value = std::stoi(tmpStr);

		sendTemperatureCmd(value);  //�����¶�
	}
	else if(cmd.find("Humi") != std::string::npos)
	{
		pos = cmd.find("=") + 1;
		auto tmpStr = cmd.substr(pos,cmd.length() - 1 - pos);
		INT value = std::stoi(tmpStr);

		sendHumidityCmd(value);  //����ʪ��
	}
	else{}

}

void CSpecAirConditionController::parseAirStatus(const char *pData, UINT Len)  //�������ص�״̬��Ϣ�����������ݿ���
{
	auto id = pData[0];

	m_SpecAirControllerStatus.switchStatus = pData[4] & 0x01;
	m_SpecAirControllerStatus.temperature = ((UCHAR)pData[5] * 256 + (UCHAR)pData[6]) / 10;
	m_SpecAirControllerStatus.humidity = ((UCHAR)pData[7] * 256 + (UCHAR)pData[8]) / 10;
	m_SpecAirControllerStatus.status = (UCHAR)pData[10];
	m_SpecAirControllerStatus.alarmInfoOne = (UCHAR)pData[15] * 256 + (UCHAR)pData[16];
	m_SpecAirControllerStatus.alarmInfoTwo = (UCHAR)pData[17] * 256 + (UCHAR)pData[18];

	m_SpecAirControllerStatus.windTemperature = ((UCHAR)pData[11] * 256 + (UCHAR)pData[12]) / 10;
	m_SpecAirControllerStatus.windHumidity = ((UCHAR)pData[13] * 256 + (UCHAR)pData[14]) / 10;

	writeToDataBase(id);
}

void CSpecAirConditionController::onAirStatusTimeout(wxTimerEvent& event)  //��ʱ������
{
	m_Timer.Stop();

	if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))  //���ݲ�̨ͬվ���Ͳ�ͬ�Ĳ�ѯ����
	{
		UCHAR statusCmd[8] = {0};

		auto item = GetPlug()->m_advancedAirConfiguration.cbegin(); //��������Ϣ�л�ȡ���ܿյ��ı�ź���ͨ�����õ�ID��
		if(m_SpecialAirConditionerCmdNO >= GetPlug()->m_advancedAirConfiguration.size())
		{
			m_SpecialAirConditionerCmdNO = 0;
		}

		for(UINT i = 0; i < m_SpecialAirConditionerCmdNO; i++)  //���Ͷ�̨���ܿյ�ʱ��ͨ��m_SpecialAirConditionerCmdNO���ʹ������ж���ڼ����յ�����״̬��ѯ����
		{
			item++;
		}
		

		statusCmd[0] = item->second;
		statusCmd[1] = 0x03;
		statusCmd[2] = 0;
		statusCmd[3] = 0;
		statusCmd[4] = 0;
		statusCmd[5] = 0x10;

		CRCData CRCCheckData;
		CRCCheckData.crcData = GetPlug()->chkcrc(statusCmd,6);  //CRCУ��

		statusCmd[6] = CRCCheckData.crcByte[1];
		statusCmd[7] = CRCCheckData.crcByte[0];

		m_udpHandler->SendData((const char*)statusCmd,8,GetPlug()->getConfig()->NetConfig.Env.ip,GetPlug()->getConfig()->NetConfig.Env.port);

		m_SpecialAirConditionerCmdNO++;

	}

	if(m_SpecialAirConditionerCmdNO >= GetPlug()->m_advancedAirConfiguration.size())  //������ʱ��
	{
		startTimer();
	}
	else
	{
		m_Timer.Start(5000);
	}

}

bool CSpecAirConditionController::sendSwitchCmd(INT flag)
{
	if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))  //���ݲ�̨ͬվ���Ϳ��ػ�����
	{
		UCHAR airCmd[8] = {0};
		airCmd[0] = GetPlug()->m_advancedAirConfiguration[m_currentDeviceIndex];
		airCmd[1] = 0x06;
		airCmd[2] = 0;
		airCmd[3] = 0;
		airCmd[4] = 0;
		airCmd[5] = flag;

		CRCData CRCCheckData;
		CRCCheckData.crcData = GetPlug()->chkcrc(airCmd,6);

		airCmd[6] = CRCCheckData.crcByte[1];
		airCmd[7] = CRCCheckData.crcByte[0];

		return m_udpHandler->SendData((const char*)airCmd,8,GetPlug()->getConfig()->NetConfig.Env.ip,GetPlug()->getConfig()->NetConfig.Env.port);
	}

	return false;

}

bool CSpecAirConditionController::sendTemperatureCmd(INT temperature)
{

	if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))  //���ݲ�̨ͬվ�����¶ȿ�������
	{
		temperature *= 10;

		UCHAR airCmd[8] = {0};
		airCmd[0] = GetPlug()->m_advancedAirConfiguration[m_currentDeviceIndex];
		airCmd[1] = 0x06;
		airCmd[2] = 0;
		airCmd[3] = 0x01;
		airCmd[4] = temperature / 256;
		airCmd[5] = temperature % 256;

		CRCData CRCCheckData;
		CRCCheckData.crcData = GetPlug()->chkcrc(airCmd,6);

		airCmd[6] = CRCCheckData.crcByte[1];
		airCmd[7] = CRCCheckData.crcByte[0];

		return m_udpHandler->SendData((const char*)airCmd,8,GetPlug()->getConfig()->NetConfig.Env.ip,GetPlug()->getConfig()->NetConfig.Env.port);
	}

	return false;
}

bool CSpecAirConditionController::sendHumidityCmd(INT humidity)
{
	if((GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND))  //���ݲ�̨ͬվ����ʪ�ȿ�������
	{
		humidity *= 10;

		UCHAR airCmd[8] = {0};
		airCmd[0] = GetPlug()->m_advancedAirConfiguration[m_currentDeviceIndex];
		airCmd[1] = 0x06;
		airCmd[2] = 0;
		airCmd[3] = 0x02;
		airCmd[4] = humidity / 256;
		airCmd[5] = humidity % 256;

		CRCData CRCCheckData;
		CRCCheckData.crcData = GetPlug()->chkcrc(airCmd,6);

		airCmd[6] = CRCCheckData.crcByte[1];
		airCmd[7] = CRCCheckData.crcByte[0];

		return m_udpHandler->SendData((const char*)airCmd,8,GetPlug()->getConfig()->NetConfig.Env.ip,GetPlug()->getConfig()->NetConfig.Env.port);
	}

	return false;
}

bool CSpecAirConditionController::writeToDataBase(UCHAR id)
{
	wxString sqlSentence = wxT("INSERT INTO airconditioninginfo (updateTime,atcNum,onOffStatus,setTemp,setHumidity,RATemp,RAHumi,status,alarm1,alarm2) VALUES");
	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{
		sqlSentence.Append(wxString::Format(wxT("(%I64d, "), wxDateTime::GetTimeNow()));
		sqlSentence.Append(wxString::Format(wxT("%d, "), id));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SpecAirControllerStatus.switchStatus));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SpecAirControllerStatus.temperature));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SpecAirControllerStatus.humidity));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SpecAirControllerStatus.windTemperature));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SpecAirControllerStatus.windHumidity));
		sqlSentence.Append(wxString::Format(wxT("%s, "), wxString(getStatusStr())));
		sqlSentence.Append(wxString::Format(wxT("%s, "), wxString(getAlarmStatusOneStr())));
		sqlSentence.Append(wxString::Format(wxT("%s)"), wxString(getAlarmStatusTwoStr())));


		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}
	}

	return true;
}

std::string CSpecAirConditionController::getStatusStr()
{
	std::string tmpStr;
	tmpStr.append("'");
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.humidityDownBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.humidityUpBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.freezeBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.heatBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.windBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.statusBit));
	tmpStr.append("'");

	return tmpStr;

}

std::string CSpecAirConditionController::getAlarmStatusOneStr()
{
	std::string tmpStr;
	tmpStr.append("'");
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Compressor_high_pressBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Compressor_low_pressBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Airflow_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Fan_overloadBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Heater_overloadBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Air_filter_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.High_temp_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Low_temp_alarmBit));

	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.High_humidity_aLarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Low_humidity_aLarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Indoor_temp_probe_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Supply_air_temp_probe_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Indoor_humidity_probe_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Outdoor_temp_probe_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)0));
	tmpStr.append(std::to_string((LONGLONG)0));
	tmpStr.append("'");

	return tmpStr;
}

std::string CSpecAirConditionController::getAlarmStatusTwoStr()
{
	std::string tmpStr;
	tmpStr.append("'");
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Humidifier_high_currentBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Humidifier_no_waterBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Humidifier_no_currentBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Phase_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)0));
	tmpStr.append(std::to_string((LONGLONG)0));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Waterleak_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.User_alarmBit));

	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Smoke_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Condenser_fan_2_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Condenser_fan_1_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)0));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Power_alarmBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Compressor_high_pressBit));
	tmpStr.append(std::to_string((LONGLONG)m_SpecAirControllerStatus.Compressor_low_pressBit));
	tmpStr.append(std::to_string((LONGLONG)0));
	tmpStr.append("'");

	return tmpStr;
}
