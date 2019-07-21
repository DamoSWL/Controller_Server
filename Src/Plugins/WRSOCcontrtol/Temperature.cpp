#include "Temperature.h"


static const int CMDTIMERID = 225;
static const int WRITETIMERID = 227; 


BEGIN_EVENT_TABLE(CTemperature,wxEvtHandler)
	EVT_TIMER(WRITETIMERID,CTemperature::onwriteDataTimeout)
	EVT_TIMER(CMDTIMERID,CTemperature::onCmdTimeout)
END_EVENT_TABLE()

CTemperature::CTemperature(void) : 
m_cmdTimer(this,CMDTIMERID),
m_writeDataTimer(this,WRITETIMERID),
m_envExceptionFlag(FALSE)
{

}

CTemperature::~CTemperature(void)
{

}

void CTemperature::setID(const std::set<UINT>& id)
{
    m_IDSet = id;
	for(auto index = m_IDSet.cbegin(); index != m_IDSet.cend(); index++)
	{
		m_newTemperatureFlag[*index] = false;
	}
}

void CTemperature::processCmd(const char* pData,unsigned int len)
{
	if((len == 13) && ((UCHAR)pData[1] == 0x03))  
	{
		UINT id = (UCHAR)pData[0];
		
		if(m_IDSet.find(id) != m_IDSet.end())
		{
			m_newTemperatureFlag[id] = true;
			UINT tmptemperature = (UCHAR)(pData[7]) * 256 + (UCHAR)pData[8];

			float sign = -1.0;
			if((tmptemperature & (1 << 15)) == 0)
			{
				sign = 1.0;
			}
			tmptemperature &= 0x7FFF;
			FLOAT temperature = tmptemperature * sign / 10.0;
			temperature += (CHAR)pData[5] / 10.0;

			UINT tmphumidity = (UCHAR)(pData[9]) * 256 + (UCHAR)pData[10];
			FLOAT humidity = tmphumidity / 10.0;
				
			m_TemperatureParam[id] = make_tuple(temperature,humidity);
		}
	}

}

int CTemperature::processData(void *pHandle, const char *pData, UINT Len)
{
    IProcess* self = static_cast<IProcess*>(pHandle);
	if(self)
	{
		self->processCmd(pData,Len);
	}
	return 0;
}

void CTemperature::processEvent(void *pHandle, int Opt, char *pData /*= nullptr*/, UINT Len /*= 0*/)
{

}


void CTemperature::startTimer(int interval)
{
	m_cmdTimer.Start(interval);
}

void CTemperature::stopTimer()
{
	m_cmdTimer.Stop();
	m_writeDataTimer.Stop();
}

void CTemperature::startWriteDataTimer()
{
	if(!m_writeDataTimer.IsRunning())
	{
		m_writeDataTimer.Start(60*1000,wxTIMER_ONE_SHOT);
	}
}

void CTemperature::sendTemperatureCmd(UINT id)
{
    UCHAR TemperatureCmd[8] = {'\0'};
	TemperatureCmd[0] = id;
	TemperatureCmd[1] = '\x03';
	TemperatureCmd[2] = '\x00';
	TemperatureCmd[3] = '\x00';
	TemperatureCmd[4] = '\x00';
	TemperatureCmd[5] = '\x05';

	//CRCData CRCCheckData;
	//CRCCheckData.crcData = GetPlug()->chkcrc(TemperatureCmd,6);

	TemperatureCmd[6] = '\x87';
	TemperatureCmd[7] = '\x59';

	GetPlug()->sendEnvCmd((const char*)&TemperatureCmd,8);


}

void CTemperature::onwriteDataTimeout(wxTimerEvent& event)
{
	m_cmdTimer.Stop();
	writeToDataBase();


	wxString envAlarmContent = wxT("标准输出控制器和环境主机连接异常");

	for(auto item = m_newTemperatureFlag.begin(); item != m_newTemperatureFlag.end(); item++)
	{
		if(item->second)
		{
			GetPlug()->writeEnvConnectionToDataBase(1);  //设置换环境主机连接正常

			if(m_envExceptionFlag)
			{
				m_EnvExceptionEndureTime.second = wxDateTime::GetTimeNow();

				auto pDatabase = GetPlug()->getDatabase();
				if (pDatabase)
				{

					wxString sqlSentence;
					sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_EnvExceptionEndureTime.second,
						m_EnvExceptionEndureTime.first,
						envAlarmContent);

					pDatabase->RunSQLNoQuery(sqlSentence);
					m_envExceptionFlag = FALSE;
				}
			}

			m_EnvAlarm.getAlarmType();
			m_EnvAlarm.getThresholdValue();
			m_EnvAlarm.getAlarmLevel();

			std::vector<FLOAT> envData(2,0.0);
			envData[0] = std::get<0>(m_TemperatureParam[item->first]);
			envData[1] = std::get<1>(m_TemperatureParam[item->first]);

			m_EnvAlarm.checkSensorA(envData);  //温湿度阈值判断

		}
		else
		{
			GetPlug()->writeEnvConnectionToDataBase(0);  //设置环境主机连接异常

			if(!m_envExceptionFlag)
			{
				m_EnvExceptionEndureTime.first = wxDateTime::GetTimeNow();

				auto pDatabase = GetPlug()->getDatabase();
				wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
										   endTime, \
										   type, \
										   alarmName, \
										   level, \
										   mode, \
										   content, \
										   origin) VALUES");
				if (pDatabase)
				{

					sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_EnvExceptionEndureTime.first));
					sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
					sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("设备连接")));
					sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
					sqlSentence.Append(wxString::Format(wxT("%d, "), 1));
					sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
					sqlSentence.Append(wxString::Format(wxT("'%s', "), envAlarmContent));
					sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


					pDatabase->RunSQLNoQuery(sqlSentence);
					m_envExceptionFlag = TRUE;
				}
			}
		}

		break;
	}	

	m_cmdTimer.Start(3*60*1000 - 60*1000);
	for(auto item = m_newTemperatureFlag.begin(); item != m_newTemperatureFlag.end(); item++)
	{
		item->second = false;
	}	
}

void CTemperature::onCmdTimeout(wxTimerEvent& event)
{
	m_cmdTimer.Stop();
	
	for(auto item = m_newTemperatureFlag.cbegin(); item != m_newTemperatureFlag.cend(); item++)
	{
		if(!item->second)
		{
			sendTemperatureCmd(item->first);
		}
	
	}

	m_cmdTimer.Start(10*1000);
	startWriteDataTimer();	
}

bool CTemperature::writeToDataBase()
{
    wxString sqlSentence = wxT("INSERT INTO radarrunenvirement (updateTime,\
        engineRoomTempA, \
        engineRoomHumidityA) VALUES");


    auto pDataBase = GetPlug()->getDatabase();


    if(pDataBase)
    {


            for(auto id = m_IDSet.cbegin(); id != m_IDSet.cend(); id++)
            {
                if(m_newTemperatureFlag[*id])
                {
                    auto sqlSentenceSingle = sqlSentence;
                    sqlSentenceSingle.Append(wxString::Format(wxT("(%I64d, "),wxDateTime::GetTimeNow()));

                    sqlSentenceSingle.Append(wxString::Format(wxT("%f, "),std::get<0>(m_TemperatureParam[*id])));
					sqlSentenceSingle.Append(wxString::Format(wxT("%f)"),std::get<1>(m_TemperatureParam[*id])));


					bool ret = pDataBase->RunSQLNoQuery(sqlSentenceSingle);
					if(!ret)
					{
						SHOWFUNCNAME();
					}
                }
            }

            
      }


    return true;
}
