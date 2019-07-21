#include "Generator.h"


static const int GENERATORTIMERID = 240;


BEGIN_EVENT_TABLE(CGenerator,wxEvtHandler)
	EVT_TIMER(GENERATORTIMERID,CGenerator::onGeneratorTimeout)
END_EVENT_TABLE()


CGenerator::CGenerator(void):m_Timer(this,GENERATORTIMERID),m_State(0)
{
}


CGenerator::~CGenerator(void)
{
}

BOOL CGenerator::startReceive()
{
	m_udpHandler = GetPlug()->m_UDPFactory.GetInstance();

	if(m_udpHandler)
	{
		m_udpHandler->RegHandle(this, CGenerator::processData,CGenerator::processEvent);

		if (m_udpHandler->StartReceive(0, "0.0.0.0", 9376, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"端口9376绑定成功");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"端口9376绑定失败");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CGenerator::stopReceive()
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

void CGenerator::startTimer()
{
	initParam();
	m_Timer.Start(3000);
}

void CGenerator::onGeneratorTimeout(wxTimerEvent& event)
{
	m_Timer.Stop();

	if(m_State > ENGINESTATE)
	{
		m_State = 0;
		writeToDataBase();

		m_Timer.Start(1*60*1000);
	}
	else
	{
		UCHAR cmd[8] = {0};
		cmd[0] = 0x01;
		cmd[1] = 0x03;
		cmd[2] = m_objectNum[m_State] / 256;
		cmd[3] = m_objectNum[m_State] % 256;
		cmd[4] = 0;
		cmd[5] = m_objectLength[m_State];

		CRCData CRCCheckData;
		CRCCheckData.crcData = GetPlug()->chkcrc(cmd,6);

		cmd[6] = CRCCheckData.crcByte[1];
		cmd[7] = CRCCheckData.crcByte[0];

		m_udpHandler->SendData((const char*)cmd,8,"172.18.190.38",23);

		m_Timer.Start(3000);

		
	}

}

int CGenerator::processData(void *pHandle, const char *pData, UINT Len)
{
	CGenerator* self = static_cast<CGenerator*>(pHandle);
	if(self)
	{		
		if((Len >= 7) && ((UCHAR)pData[0] == 0x01) && ((UCHAR)pData[1] == 0x03))
		{		
			if(self->m_State <= GENERATORFREQUENCY)
			{
				self->handleTypeForUnsigned2(pData,Len);
			}
			else if((self->m_State >= ACTIVEPOWER) && (self->m_State <= FUELLEVEL))
			{
				self->handleTypeForInteger2(pData,Len);
			}
			else if((self->m_State >= GENERATORLOAD) && (self->m_State <= POWERFACTOR))
			{
				self->handleTypeForChar(pData,Len);
			}
			else if((self->m_State >= KWHOURS) && (self->m_State <= KVARHOURS))
			{
				self->handleTypeForInteger4(pData,Len);
			}
			else if(self->m_State == ENGINESTATE)
			{
				self->handleTypeForEngineState(pData,Len);
			}
			else{}

			self->m_State++;

		}
	}

	return 0;
}

void CGenerator::processEvent(void *pHandle, int Opt, char *pData /*= nullptr*/, UINT Len /*= 0*/)
{

}

void CGenerator::handleTypeForUnsigned2(const char *pData, UINT Len)
{
	WORD value = 0;
	std::memcpy((void*)&value,(void*)(pData + 3),2);
	value = ntohs(value);

	m_objectValue[m_State] = value / m_objectCofficient[m_State];

}

void CGenerator::handleTypeForChar(const char *pData, UINT Len)
{
	m_objectValue[m_State] = (UCHAR)pData[4] / m_objectCofficient[m_State];

}

void CGenerator::handleTypeForInteger4(const char *pData, UINT Len)
{
	LONG value = 0;
	std::memcpy((void*)&value,(void*)(pData + 3),4);
	value = ntohl(value);

	m_objectValue[m_State] = value / m_objectCofficient[m_State];
}

void CGenerator::handleTypeForEngineState(const char *pData, UINT Len)
{
	m_objectValue[m_State] = (UCHAR)pData[4] + 445;
}

void CGenerator::handleTypeForInteger2(const char *pData, UINT Len)
{
	SHORT value = 0;
	std::memcpy((void*)&value,(void*)(pData + 3),2);
	value = ntohs(value);

	if((m_State >= OILPRESS) && (m_State <= FUELLEVEL))
	{
		if(((UCHAR)pData[3] == 0x80) && ((UCHAR)pData[4] == 0))
		{
			m_objectValue[m_State] = -1e6;
		}
		else
		{
			m_objectValue[m_State] = value / m_objectCofficient[m_State];
		}
	}
	else
	{
		m_objectValue[m_State] = value / m_objectCofficient[m_State];
	}


}

void CGenerator::initParam()
{
	m_objectNum[GENERATORVOLTAGEL1] = 8192;
	m_objectNum[GENERATORVOLTAGEL2] = 8193;
	m_objectNum[GENERATORVOLTAGEL3] = 8194;
	m_objectNum[GENERATORVOLTAGEL1L2] = 9628;
	m_objectNum[GENERATORVOLTAGEL2L3] = 9629;
	m_objectNum[GENERATORVOLTAGEL3L1] = 9630;
	m_objectNum[GENERATORCURRENTL1] = 8198;
	m_objectNum[GENERATORCURRENTL2] = 8199;
	m_objectNum[GENERATORCURRENTL3] = 8200;
	m_objectNum[ACTIVEPOWER] = 8202;
	m_objectNum[ACTIVEPOWERL1] = 8524;
	m_objectNum[ACTIVEPOWERL2] = 8525;
	m_objectNum[ACTIVEPOWERL3] = 8526;
	m_objectNum[APPARENTPOWER] = 8565;
	m_objectNum[APPARENTPOWERL1] = 8530;
	m_objectNum[APPARENTPOWERL2] = 8531;
	m_objectNum[APPARENTPOWERL3] = 8532;
	m_objectNum[GENERATORFREQUENCY] = 8210;
	m_objectNum[GENERATORLOAD] = 8395;
	m_objectNum[POWERFACTOR] = 8204;
	m_objectNum[KWHOURS] = 8205;
	m_objectNum[RUNHOURS] = 8206;
	m_objectNum[BATTERYVOLTAGE] = 8213;
	m_objectNum[OILPRESS] = 8227;
	m_objectNum[WATERTEMPERATURE] = 8228;
	m_objectNum[FUELLEVEL] = 8229;
	m_objectNum[KVARHOURS] = 8539;
	m_objectNum[ENGINESTATE] = 8330;


	m_objectCofficient[GENERATORVOLTAGEL1] = 1.0;
	m_objectCofficient[GENERATORVOLTAGEL2] = 1.0;
	m_objectCofficient[GENERATORVOLTAGEL3] = 1.0;
	m_objectCofficient[GENERATORVOLTAGEL1L2] = 1.0;
	m_objectCofficient[GENERATORVOLTAGEL2L3] = 1.0;
	m_objectCofficient[GENERATORVOLTAGEL3L1] = 1.0;
	m_objectCofficient[GENERATORCURRENTL1] = 1.0;
	m_objectCofficient[GENERATORCURRENTL2] = 1.0;
	m_objectCofficient[GENERATORCURRENTL3] = 1.0;
	m_objectCofficient[ACTIVEPOWER] = 1.0;
	m_objectCofficient[ACTIVEPOWERL1] = 1.0;
	m_objectCofficient[ACTIVEPOWERL2] = 1.0;
	m_objectCofficient[ACTIVEPOWERL3] = 1.0;
	m_objectCofficient[APPARENTPOWER] = 1.0;
	m_objectCofficient[APPARENTPOWERL1] = 1.0;
	m_objectCofficient[APPARENTPOWERL2] = 1.0;
	m_objectCofficient[APPARENTPOWERL3] = 1.0;
	m_objectCofficient[GENERATORFREQUENCY] = 10.0;
	m_objectCofficient[GENERATORLOAD] = 1.0;
	m_objectCofficient[POWERFACTOR] = 100.0;
	m_objectCofficient[KWHOURS] = 1.0;
	m_objectCofficient[RUNHOURS] = 1.0;
	m_objectCofficient[BATTERYVOLTAGE] = 10.0;
	m_objectCofficient[OILPRESS] = 10.0;
	m_objectCofficient[WATERTEMPERATURE] = 1.0;
	m_objectCofficient[FUELLEVEL] = 1.0;
	m_objectCofficient[KVARHOURS] = 1.0;
	m_objectCofficient[ENGINESTATE] = 1.0;


	m_objectLength[GENERATORVOLTAGEL1] = 1;
	m_objectLength[GENERATORVOLTAGEL2] = 1;
	m_objectLength[GENERATORVOLTAGEL3] = 1;
	m_objectLength[GENERATORVOLTAGEL1L2] = 1;
	m_objectLength[GENERATORVOLTAGEL2L3] = 1;
	m_objectLength[GENERATORVOLTAGEL3L1] = 1;
	m_objectLength[GENERATORCURRENTL1] = 1;
	m_objectLength[GENERATORCURRENTL2] = 1;
	m_objectLength[GENERATORCURRENTL3] = 1;
	m_objectLength[ACTIVEPOWER] = 1;
	m_objectLength[ACTIVEPOWERL1] = 1;
	m_objectLength[ACTIVEPOWERL2] = 1;
	m_objectLength[ACTIVEPOWERL3] = 1;
	m_objectLength[APPARENTPOWER] = 1;
	m_objectLength[APPARENTPOWERL1] = 1;
	m_objectLength[APPARENTPOWERL2] = 1;
	m_objectLength[APPARENTPOWERL3] = 1;
	m_objectLength[GENERATORFREQUENCY] = 1;
	m_objectLength[GENERATORLOAD] = 1;
	m_objectLength[POWERFACTOR] = 1;
	m_objectLength[KWHOURS] = 2;
	m_objectLength[RUNHOURS] = 2;
	m_objectLength[BATTERYVOLTAGE] = 1;
	m_objectLength[OILPRESS] = 1;
	m_objectLength[WATERTEMPERATURE] = 1;
	m_objectLength[FUELLEVEL] = 1;
	m_objectLength[KVARHOURS] = 2;
	m_objectLength[ENGINESTATE] = 1;

	m_objectValue[GENERATORVOLTAGEL1] = 0;
	m_objectValue[GENERATORVOLTAGEL2] = 0;
	m_objectValue[GENERATORVOLTAGEL3] = 0;
	m_objectValue[GENERATORVOLTAGEL1L2] = 0;
	m_objectValue[GENERATORVOLTAGEL2L3] = 0;
	m_objectValue[GENERATORVOLTAGEL3L1] = 0;
	m_objectValue[GENERATORCURRENTL1] = 0;
	m_objectValue[GENERATORCURRENTL2] = 0;
	m_objectValue[GENERATORCURRENTL3] = 0;
	m_objectValue[GENERATORFREQUENCY] = 0;
	m_objectValue[ACTIVEPOWER] = 0;
	m_objectValue[ACTIVEPOWERL1] = 0;
	m_objectValue[ACTIVEPOWERL2] = 0;
	m_objectValue[ACTIVEPOWERL3] = 0;
	m_objectValue[APPARENTPOWER] = 0;
	m_objectValue[APPARENTPOWERL1] = 0;
	m_objectValue[APPARENTPOWERL2] = 0;
	m_objectValue[APPARENTPOWERL3] = 0;
	m_objectValue[BATTERYVOLTAGE] = 0;
	m_objectValue[OILPRESS] = 0;
	m_objectValue[WATERTEMPERATURE] = 0;
	m_objectValue[FUELLEVEL] = 0;
	m_objectValue[GENERATORLOAD] = 0;
	m_objectValue[POWERFACTOR] = 0;
	m_objectValue[KWHOURS] = 0;
	m_objectValue[RUNHOURS	] = 0;
	m_objectValue[KVARHOURS] = 0;
	m_objectValue[ENGINESTATE] = 0;
}

bool CGenerator::writeToDataBase()
{

		wxString sqlSentence = wxT("INSERT INTO generatorinfo (updateTime, \
			genVL1, \
			genVL2, \
			genVL3, \
			genVL1L2, \
			genVL2L3, \
			genVL3L1, \
			genCurrL1, \
			genCurrL2, \
			genCurrL3, \
			genFreq, \
			actPower, \
			actPwrL1, \
			actPwrL2, \
			actPwrL3, \
			apparPower, \
			apparPwrL1, \
			apparPwrL2, \
			apparPwrL3, \
			battVolt, \
			oilPress, \
			waterTemp, \
			fuelLevel, \
			genLoad, \
			pwrFactor, \
			kWh, \
			runHours, \
			kVArh, \
			genStatus) VALUES");
	


		auto pDataBase = GetPlug()->getDatabase();
		if (pDataBase)
		{
			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), wxDateTime::GetTimeNow()));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORVOLTAGEL1]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORVOLTAGEL2]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORVOLTAGEL3]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORVOLTAGEL1L2]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORVOLTAGEL2L3]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORVOLTAGEL3L1]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORCURRENTL1]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORCURRENTL2]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORCURRENTL3]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORFREQUENCY]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[ACTIVEPOWER]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[ACTIVEPOWERL1]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[ACTIVEPOWERL2]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[ACTIVEPOWERL3]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[APPARENTPOWER]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[APPARENTPOWERL1]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[APPARENTPOWERL2]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[APPARENTPOWERL3]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[BATTERYVOLTAGE]));

			if(m_objectValue[OILPRESS] < -1e5)
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[OILPRESS]));
			}

			if(m_objectValue[WATERTEMPERATURE] < -1e5)
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[WATERTEMPERATURE]));
			}
			
			if(m_objectValue[FUELLEVEL] < -1e5)
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[FUELLEVEL]));
			}
			
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[GENERATORLOAD]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[POWERFACTOR]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[KWHOURS]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[RUNHOURS]));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_objectValue[KVARHOURS]));
			sqlSentence.Append(wxString::Format(wxT("%d)"), (INT)m_objectValue[ENGINESTATE]));

			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}
		}

	return true;
}

