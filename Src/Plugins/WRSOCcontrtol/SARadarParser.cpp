
#include "SARadarParser.h"
#include <wx/dir.h>

CSARadarParser::CSARadarParser(void):m_updateTime(0),m_polarType(UNIPOLE)
{
}


CSARadarParser::~CSARadarParser(void)
{
}

DWORD WINAPI CSARadarParser::exec(LPVOID para)
{
    CSARadarParser* self = static_cast<CSARadarParser*>(para);
	
	if(self)
	{
		self->setUpdateTime(GetPlug()->getfileConfig().radarDatatime);
		time_t newTime = 0;

		while (!GetPlug()->GetExit())
		{

			if (self->loadFile(&newTime))
			{
				if(self->writeToDataBase(newTime))
				{
					self->updateTime(newTime);
				}
			}
			if(newTime > (time_t)(::wxDateTime::GetTimeNow() - 10*60))
			{
                for(int i = 0;i <= 30;i++)
				{
					if(GetPlug()->GetExit())
					{
						break;
					}
					::wxSleep(1);
				}
			}
			else
			{
				::wxSleep(1);
			}
			
		}

	}
	return EXIT_SUCCESS;
}

wxString CSARadarParser::getLatestFileName(time_t* newTime)
{
	wxString dirName(GetPlug()->getfileConfig().radarDataDir);
	if(dirName.IsEmpty())
	{
		return wxEmptyString;
	}
	if(!::wxDirExists(dirName))
	{
		return wxEmptyString;
	}


	wxDir fileDir(dirName);
	wxString fullFilePath;

	if ( !fileDir.IsOpened() )
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"雷达在线监测参数文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName, /*wxEmptyString*/wxT("Z_R_DWRN_SRSI*"), wxDIR_FILES );
		while( flag )
		{
            if(GetPlug()->GetExit())
			{
				break;
            }
            
			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			auto updateTime = GetPlug()->fileModificationTime(fileName);
			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				updateTime += 8*60*60;
			}

			if(m_updateTime < updateTime)
			{
				*newTime = updateTime;
				newFileFlag = true;
				break;
			}
			flag = fileDir.GetNext(&fileName);
		}

		if(!newFileFlag)
		{
			fullFilePath.clear();
		}
	}
	return fullFilePath;
}

BOOL CSARadarParser::loadFile(time_t* newTime)
{

    auto fileName = getLatestFileName(newTime);
	if (fileName.IsEmpty())
	{
		return FALSE;
	}

	wxFile file(fileName,wxFile::read);
	if (file.IsOpened())
	{
		m_SAAlarm.getAlarmType();
		m_SAAlarm.getThresholdValue();

		file.Seek(sizeof(RadarStructure::GeneHeader) + sizeof(RadarStructure::SiteInfo));	

		RadarStructure::RadarStatus radarStatus;
		file.Read((void*)&radarStatus,sizeof(RadarStructure::RadarStatus));
		m_polarType = radarStatus.POL;

		file.Read((void*)&m_SARadarData,sizeof(RadarStructure::SAStructrue));

		m_SAAlarm.setPolarType(m_polarType);
		m_SAAlarm.getSAValue(m_SARadarData);
		m_SAAlarm.SACheck();

		file.Close();

	}
	else
	{
		file.Close();
		return FALSE;
	}

	return TRUE;
}

BOOL CSARadarParser::writeToDataBase(time_t newTime)
{
    wxString sqlSentence = wxT("INSERT INTO onlinemonitorpoint (updateTime, \
                                                                    rda_ver_name, \
                                                                    rf_power_smoothing, \
                                                                    hxmtlosssingle, \
                                                                    hxmtlosshv, \
                                                                    hrcvloss, \
                                                                    htestloss, \
                                                                    vxmtlosssingle, \
                                                                    vxmtlosshv, \
                                                                    vrcvloss, \
                                                                    vtestloss, \
                                                                    pathloss_noise_h, \
                                                                    pathloss_noise_v, \
                                                                    ts_cw, \
                                                                    ts_noise, \
                                                                    pathloss_rf, \
                                                                    ts_rf, \
                                                                    pathloss_kly, \
                                                                    noise_smoothing, \
                                                                    gas_loss, \
                                                                    k1, \
                                                                    k2, \
                                                                    k3, \
                                                                    k4, \
                                                                    radome_loss, \
                                                                    xmtBit, \
                                                                    rcvBit, \
                                                                    summary, \
                                                                    svBit) VALUES");

    // wxString sqlError;
    auto pDataBase = GetPlug()->getDatabase();
    // wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
    // wxMutexLocker locker(*sqlLocker);	

    if (pDataBase)
	{
		// try
		// {

            sqlSentence.Append(wxString::Format(wxT("(%I64d, "),newTime));
            sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(m_SARadarData.Rda_Version_Name)));

            if(::wxFinite (m_SARadarData.RF_POWER_SMOOTHING))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.RF_POWER_SMOOTHING));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.HXMTLOSSSINGLE))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.HXMTLOSSSINGLE));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.HXMTLOSSHV))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.HXMTLOSSHV));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.HRCVLOSS))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.HRCVLOSS));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.HTESTLOSS))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.HTESTLOSS));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }
        
			if(m_polarType == BIPOLE)
			{
				if(::wxFinite (m_SARadarData.VXMTLOSSSINGLE))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.VXMTLOSSSINGLE));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}


			if(m_polarType == BIPOLE)
			{
				if(::wxFinite (m_SARadarData.VXMTLOSSHV))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.VXMTLOSSHV));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}

          
			if(m_polarType == BIPOLE)
			{
				if(::wxFinite (m_SARadarData.VRCVLOSS))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.VRCVLOSS));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}
        
			if(m_polarType == BIPOLE)
			{

				if(::wxFinite (m_SARadarData.VTESTLOSS))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.VTESTLOSS));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}

            if(::wxFinite (m_SARadarData.PATHLOSS_NOISE_H))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.PATHLOSS_NOISE_H));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

			if(m_polarType == BIPOLE)
			{

				if(::wxFinite (m_SARadarData.PATHLOSS_NOISE_V))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.PATHLOSS_NOISE_V));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			}
    
            if(::wxFinite (m_SARadarData.TS_CW))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.TS_CW));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.TS_NOISE))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.TS_NOISE));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }
           
            if(::wxFinite (m_SARadarData.PATHLOSS_RF))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.PATHLOSS_RF));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }
            
            if(::wxFinite (m_SARadarData.TS_RF))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.TS_RF));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }
            
            if(::wxFinite (m_SARadarData.PATHLOSS_KLY))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.PATHLOSS_KLY));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.NOISE_SMOOTHING))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.NOISE_SMOOTHING));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.GAS_LOSS))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.GAS_LOSS));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.K1))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.K1));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.K2))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.K2));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.K3))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.K3));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }
           
            if(::wxFinite (m_SARadarData.K4))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.K4));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }

            if(::wxFinite (m_SARadarData.RADOME_LOSS))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_SARadarData.RADOME_LOSS));
            }
            else
            {
                sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
            }
            
            sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(getSAXMTBitStr().c_str(),wxConvLocal)));
            sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(getSARCVBitStr().c_str(),wxConvLocal)));
            sqlSentence.Append(wxString::Format(wxT("%d, "), m_SARadarData.SUMMARY));
            sqlSentence.Append(wxString::Format(wxT("'%s')"),wxString(getSASVBitStr().c_str(),wxConvLocal)));
            
#ifdef __TESTWRSOC__
			auto state14 = GetPlug()->getState14();
			if((state14 == 0) || (state14 == 5)) 
			{
				return TRUE;
			}
#endif

            bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
            if(!ret)
			{
				SHOWFUNCNAME();
			}


		// }
		// catch (wxString& error)
		// {
		// 	if(error.Find(wxT("Duplicate entry")) != wxNOT_FOUND)
		// 	{
		// 		return TRUE;
		// 	}
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return TRUE;

}

void CSARadarParser::updateTime(time_t newTime)
{
	m_updateTime = newTime;
	GetPlug()->setradarDatatime(newTime);
}

std::string CSARadarParser::getSAXMTBitStr()
{
    std::string xmtBitStr;
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.PFN_HIGH_BIT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MAINT_REQD));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MIANT_MODE));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG1_PFN_XFER_INTLK));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG1_SWITCH_POS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.XMTR_AVAILABLE));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.KLYSTRON_WARMUP));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.FILAMENT_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.FOCUS_COIL_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.VAC_PUMP_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.FILAMENT_PS_VOL));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.P40V_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.N15V_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.P28V_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.P15V_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.P5V_PS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE1));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE2));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.CABINET_AIR_FLOW));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.CABINET_AIR_TEMP));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.CABINET_INTLK));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG_ARC_VCWR));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG_PRESS));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.CIRCULATOR_TEMP));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.PFN_LOW_BIT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.TRIGGER_AMP));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.INV_DIODE));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.FLYBACK_CHARGER));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MAIN_PWR_VOLTAGE));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MOD_SWITCH_FAIL));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MOD_INV_CURRENT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MOD_OVERLOAD));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.XMTR_BATTERY_CHRG));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.OIL_LEVEL));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.PRF_LIMIT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.OIL_TEMP));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.FOCUS_COIL_AIR));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.FOCUS_COIL_CUR));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.XMTR_OVERCURRENT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.XMTR_OVERVOLTAGE));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT7));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT5));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.KLYS_AIR_FLOW));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.KLYS_AIR_TEMP));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.KLYS_VACION_CUR));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.KLYS_FILAMENT_CUR));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.KLYS_CURRENT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG_HMD));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.POST_CHG_REG));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.MOD_SWITCH_MAINT));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT4));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT3));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT2));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT1));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ONE_TEST_BIT0));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT7));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT6));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT5));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT4));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT3 ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT2 ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT1 ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.ZERO_TEST_BIT0 ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE3));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE4));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE5));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.UART ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE6));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.XMTR_INOP ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.TX_RECYCLE ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.HIGH_VOLTAGE ));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_7));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_6));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_5));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_4));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_3));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_2));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_1));
    xmtBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE7_0));

    return xmtBitStr;

}

std::string CSARadarParser::getSARCVBitStr()
{
    std::string rcvBitStr;
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS2_N9V_PS));
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS1_PN18V_PS));
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS2_P5V_PS));
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS3_N5PT2V_PS));
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS1_P5V_PS));
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS4_P6V_PS));
    rcvBitStr.append(std::to_string((unsigned long long)m_SARadarData.PS2_P9V_PS));
    return rcvBitStr;

}

std::string CSARadarParser::getSASVBitStr()
{
    std::string svBitStr;
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_GEARBOX_OIL  ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_ENCODER_LIGHT  ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE29 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_N_LIMIT ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_P_LIMIT  ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE30 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_DEAD_LIMIT ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_PCU_PARITY ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_STOW_PIN ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_MOTOR ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.PED_150V_UNDER ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.PED_150V_OVER ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_AMP_OVRTEMP ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_AMP_SHT_CKT ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_AMP_INHIBIT  ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.PED_BIT_ST1));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE31 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_HANDWHELL));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_HANDWHELL));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE32 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_BULLGEAR_OIL));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_GEARBOX_OIL));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_ENCODER_LIGHT));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_PCU_PARITY));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_STOW_PIN));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_MOTOR));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE33 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE34 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_AMP_OVRTEMP));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_AMP_SHT_CKT));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_AMP_INHIBIT));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.PED_BIT_ST2));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_15 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_14 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_13 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_12 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_11 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_10 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_9 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE35_8 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.PED_INTLK_SWITCH));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SERVO));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.EL_AMP_PS));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.AZ_AMP_PS));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE36 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE37 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.SPARE38 ));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.PED_BIT_ST3));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.RADOME_HATCH));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG2_PFN_XFER_INTLK));
    svBitStr.append(std::to_string((unsigned long long)m_SARadarData.WG2_SWITCH_POS));

    return svBitStr;
}
