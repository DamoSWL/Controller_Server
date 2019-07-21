#include "CCRadarParser.h"
#include <wx/dir.h>

CCCRadarParser::CCCRadarParser(void)
{
}


CCCRadarParser::~CCCRadarParser(void)
{
}

DWORD WINAPI CCCRadarParser::exec(LPVOID para)
{
	
	CCCRadarParser* self = static_cast<CCCRadarParser*>(para);

	if(self)
	{
		self->setUpdateTime(GetPlug()->getfileConfig().radarDatatime);
		time_t newTime = 0;
		while (!GetPlug()->GetExit())
		{
			if (self->loadFile(&newTime))
			{
				if(self->writeToDataBase(newTime) && self->writeAdatpParamToDateBase(newTime))
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

wxString CCCRadarParser::getLatestFileName(time_t* newTime)
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
		bool flag = fileDir.GetFirst(&fileName, /*wxT("*_RadarStatus_*")*/wxT("Z_R_DWRN_SRSI*"), wxDIR_FILES );
		while( flag )
		{
			if(GetPlug()->GetExit())
			{
				break;
			}
			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			auto updateTime = GetPlug()->fileModificationTime(fullFilePath);
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

BOOL CCCRadarParser::loadFile(time_t* newTime)
{
	auto fileName = getLatestFileName(newTime);
	if (fileName.IsEmpty())
	{
		return FALSE;
	}

	wxFile file(fileName,wxFile::read);
	if (file.IsOpened())
	{	
		file.Seek(sizeof(RadarStructure::GeneHeader) + sizeof(RadarStructure::SiteInfo) + sizeof(RadarStructure::RadarStatus),wxFromStart);

		file.Read((void*)&m_CCRadarData,sizeof(RadarStructure::CCStructure));
		file.Read((void*)&m_CCAdapatParam,sizeof(RadarStructure::CCAdaptParam));

		file.Close();

	}
	else
	{
		file.Close();
		return FALSE;
	}

	return TRUE;
}

BOOL CCCRadarParser::writeAdatpParamToDateBase(time_t newTime)
{
	wxString sqlSentence = wxT("INSERT INTO adaptparameter (updateTIme, \
																Le_one, \
																cw2Power, \
																cw1_hPower, \
																cw_vPower, \
																us1RfdPower, \
																us2RfdPower, \
																kd1Power, \
																kd2Power, \
																InternalNoiseRatio, \
																cw_h_cwh_t, \
																cw_v_cwv_t, \
																cw_h_cwh_e, \
																cw_v_cwv_e, \
																rfd_h_1us, \
																rfd_h_2us, \
																noiseRatio_h, \
																noiseRatio_v, \
																two_way_airLoss) VALUES");
	
	auto pDataBase = GetPlug()->getDatabase();
	if(pDataBase)
	{
		sqlSentence.Append(wxString::Format(wxT("(%I64d, "), newTime));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.Le_one));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw2Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw1_hPower));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw_vPower));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.us1RfdPower));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.us2RfdPower));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.kd1Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.kd2Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.InternalNoiseRatio));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw_h_cwh_t));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw_v_cwv_t));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw_h_cwh_e));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.cw_v_cwv_e));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.rfd_h_1us));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.rfd_h_2us));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.noiseRatio_h));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCAdapatParam.noiseRatio_v));
		sqlSentence.Append(wxString::Format(wxT("%f)"), m_CCAdapatParam.two_way_airLoss));

		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}	
	}

	return TRUE;

}

BOOL CCCRadarParser::writeToDataBase(time_t newTime)
{
	wxString sqlSentence = wxT("INSERT INTO onlinemonitorpoint (updateTime, \
																	KLY_INPUTPWR, \
																	RECTVOLT, \
																	HIGHPWRAMP, \
																	PFN_VOLT, \
																	NPEAKAMP, \
																	KLY_ALLAMP, \
																	KLY_TBAMP, \
																	VACIONAMP, \
																	FILAAMP, \
																	MAGN1AMP, \
																	MAGN2AMP, \
																	LO1_PWR, \
																	TESTPWR, \
																	EXCITPWR, \
																	GAIN, \
																	SWR, \
																	NOISE_COEFFICIENT, \
																	DYNAMIC_RANGE, \
																	ANT_INTEMP, \
																	ANT_INHUMIDITY, \
																	CAB_TEMP, \
																	CAB_HUMIDITY, \
																	FEEDER_PRESSURE, \
																	Uab_Volt, \
																	Ubc_Volt, \
																	Uca_Volt, \
																	AVOLT, \
																	AAMP, \
																	BVOLT, \
																	BAMP, \
																	CVOLT, \
																	CAMP, \
																	pwrBit, \
																	rcvBit, \
																	svBit, \
																	xmtBit) VALUES");
	
	auto pDataBase = GetPlug()->getDatabase();
	if(pDataBase)
	{
		sqlSentence.Append(wxString::Format(wxT("(%I64d, "), newTime));
		if(!::wxFinite (m_CCRadarData.KLY_InputPWR) || GetPlug()->CCExceptionCheck(m_CCRadarData.KLY_InputPWR))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.KLY_InputPWR));
		}
		
		if(!::wxFinite (m_CCRadarData.RECTVOLT) || GetPlug()->CCExceptionCheck(m_CCRadarData.RECTVOLT))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.RECTVOLT));
		}
		
		if(!::wxFinite (m_CCRadarData.HIGHPWRAMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.HIGHPWRAMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.HIGHPWRAMP));
		}
		
		if(!::wxFinite (m_CCRadarData.PFN_VOLT) || GetPlug()->CCExceptionCheck(m_CCRadarData.PFN_VOLT))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.PFN_VOLT));
		}
		
		if(!::wxFinite (m_CCRadarData.NPEAKAMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.NPEAKAMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.NPEAKAMP));
		}

		if(!::wxFinite (m_CCRadarData.KLY_ALLAMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.KLY_ALLAMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.KLY_ALLAMP));
		}

		if(!::wxFinite (m_CCRadarData.KLY_TBAMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.KLY_TBAMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.KLY_TBAMP));
		}
		
		if(!::wxFinite (m_CCRadarData.VacionAMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.VacionAMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.VacionAMP));
		}

		if(!::wxFinite (m_CCRadarData.FilaAMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.FilaAMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.FilaAMP));
		}

		if(!::wxFinite (m_CCRadarData.Magn1AMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.Magn1AMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Magn1AMP));
		}
		
		if(!::wxFinite (m_CCRadarData.Magn2AMP) || GetPlug()->CCExceptionCheck(m_CCRadarData.Magn2AMP))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Magn2AMP));
		}

		if(!::wxFinite (m_CCRadarData.LO1_PWR) || GetPlug()->CCExceptionCheck(m_CCRadarData.LO1_PWR))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.LO1_PWR));
		}

		if(!::wxFinite (m_CCRadarData.TestPWR) || GetPlug()->CCExceptionCheck(m_CCRadarData.TestPWR))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.TestPWR));
		}

		if(!::wxFinite (m_CCRadarData.ExcitPWR) || GetPlug()->CCExceptionCheck(m_CCRadarData.ExcitPWR))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.ExcitPWR));
		}

		if(!::wxFinite (m_CCRadarData.Gain) || GetPlug()->CCExceptionCheck(m_CCRadarData.Gain))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Gain));
		}
		
		if(!::wxFinite (m_CCRadarData.SWR) || GetPlug()->CCExceptionCheck(m_CCRadarData.SWR))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.SWR));
		}
		
		if(!::wxFinite (m_CCRadarData.NOISE_COEFFICIENT) || GetPlug()->CCExceptionCheck(m_CCRadarData.NOISE_COEFFICIENT))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.NOISE_COEFFICIENT));
		}

		if(!::wxFinite (m_CCRadarData.DYNAMIC_RANGE) || GetPlug()->CCExceptionCheck(m_CCRadarData.DYNAMIC_RANGE))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.DYNAMIC_RANGE));
		}
		
		if(!::wxFinite (m_CCRadarData.ANT_InTemp) || GetPlug()->CCExceptionCheck(m_CCRadarData.ANT_InTemp))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.ANT_InTemp));
		}
		
		if(!::wxFinite (m_CCRadarData.ANT_InHumidity) || GetPlug()->CCExceptionCheck(m_CCRadarData.ANT_InHumidity))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.ANT_InHumidity));
		}
		
		if(!::wxFinite (m_CCRadarData.Cab_Temp) || GetPlug()->CCExceptionCheck(m_CCRadarData.Cab_Temp))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Cab_Temp));
		}

		if(!::wxFinite (m_CCRadarData.Cab_Humidity) || GetPlug()->CCExceptionCheck(m_CCRadarData.Cab_Humidity))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Cab_Humidity));
		}

		if(!::wxFinite (m_CCRadarData.Feeder_Pressure) || GetPlug()->CCExceptionCheck(m_CCRadarData.Feeder_Pressure))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Feeder_Pressure));
		}


		if(!::wxFinite (m_CCRadarData.Uab_Volt) || GetPlug()->CCExceptionCheck(m_CCRadarData.Uab_Volt))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Uab_Volt));
		}
		
		if(!::wxFinite (m_CCRadarData.Ubc_Volt) || GetPlug()->CCExceptionCheck(m_CCRadarData.Ubc_Volt))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Ubc_Volt));
		}

		if(!::wxFinite (m_CCRadarData.Uca_Volt) || GetPlug()->CCExceptionCheck(m_CCRadarData.Uca_Volt))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.Uca_Volt));
		}

		if(!::wxFinite (m_CCRadarData.AVolt) || GetPlug()->CCExceptionCheck(m_CCRadarData.AVolt))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.AVolt));
		}
		
		if(!::wxFinite (m_CCRadarData.AAmp) || GetPlug()->CCExceptionCheck(m_CCRadarData.AAmp))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.AAmp));
		}

		if(!::wxFinite (m_CCRadarData.BVolt) || GetPlug()->CCExceptionCheck(m_CCRadarData.BVolt))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.BVolt));
		}

		if(!::wxFinite (m_CCRadarData.BAmp) || GetPlug()->CCExceptionCheck(m_CCRadarData.BAmp))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.BAmp));
		}
		
		
		if(!::wxFinite (m_CCRadarData.CVolt) || GetPlug()->CCExceptionCheck(m_CCRadarData.CVolt))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.CVolt));
		}

		if(!::wxFinite (m_CCRadarData.CAmp) || GetPlug()->CCExceptionCheck(m_CCRadarData.CAmp))
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
		}
		else
		{
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CCRadarData.CAmp));
		}

		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getPwrBit())));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getRcvStr())));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getSvoStr())));
		sqlSentence.Append(wxString::Format(wxT("'%s')"), wxString(getXmtStr())));

		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}

	}

	return TRUE;
	
}

void CCCRadarParser::updateTime(time_t newTime)
{
	m_updateTime = newTime;
	GetPlug()->setradarDatatime(newTime);
}

std::string CCCRadarParser::getPwrBit()
{
	std::string pwrBitStr;
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit0));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit1));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit2));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit3));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit4));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit5));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit6));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit7));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit8));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit9));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit10));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit11));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit12));
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit13));


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit17));


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit20)); //发射通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit21)); //接收通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit22)); //信号处理通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit23)); //伺服通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit24)); //接收功率检测通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit25)); //KLY功率检测通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit26)); //配电通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit27)); //天线功率检测通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit28)); //时间服务器通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit29)); //波导充气机通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit30)); //UPS电源通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit31)); //温湿度仪1-天线罩通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit32)); //温湿度仪2-转台通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit33)); //温湿度仪3-机房通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit34)); //温湿度仪4-机柜通信
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit35)); //温湿度仪5-发射通信


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit38)); //充气机气压告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit39)); //充气机温度告警


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit47)); //天线罩温度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit48)); //天线罩湿度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit49)); //转台温度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit50)); //转台湿度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit51)); //机房温度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit52)); //机房湿度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit53)); //综合机柜温度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit54)); //综合机柜湿度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit55)); //发射机柜温度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit56)); //发射机柜湿度告警
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit57)); //发射监控温度告警


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit62));


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit65)); //雷达常数C
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit66)); //定标常数SYSCAL
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit67)); //相位噪声
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit68)); //杂波抑制
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit69)); //CW测试误差
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit70)); //RFD测试误差
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit71)); //KD测试误差
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit72)); //速度测试误差

	return pwrBitStr;
}

std::string CCCRadarParser::getRcvStr()
{
	std::string rcvBitStr;

	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit0)); //H路主通道
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit1)); //H路副通道
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit2)); //V路主通道
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit3)); //V路副通道
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit4)); //一本振
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit5)); //二本振
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit6)); //晶振
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit7)); //150Mz时钟1
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit8)); //上变频
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit9)); //84MHz时钟
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit10)); //150Mz时钟2
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit11)); //接收直流电源
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit12)); //DDS时钟
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit13)); //AD采样时钟
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit14)); //系统时钟
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit15)); //Burst通道
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit16)); //主通道H1噪声电平告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit17)); //副通道H2噪声电平告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit18)); //主通道H1增益告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit19)); //副通道H2增益告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit20)); //通道增益差告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit21)); //噪声系数告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit22)); //Burst通道增益告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit23)); //Burst通道噪声电平告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit24)); //一本振信号告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit25)); //激励信号告警
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit26)); //DDS测试信号告警

	return rcvBitStr;
}

std::string CCCRadarParser::getSvoStr()
{
	std::string svBitStr;

	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit0)); //无IQ数据
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit1)); //数据丢包
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit2)); //适配参数加载失败
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit3)); //无参数输出
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit4)); //存储空间过低


	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit7)); //方位驱动器
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit8)); //俯仰驱动器
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit9)); //R/D故障
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit10)); //遥/本控
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit11)); //+15V故障
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit12)); //-15V故障
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit13)); //俯仰定位
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit14)); //方位定位
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit15)); //冲顶故障
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit16)); //打底故障
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit17)); //天线方位角码跳变
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit18)); //天线俯仰角码跳变
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit19)); //方位定位误差
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit20)); //俯仰定位误差
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit21)); //天线定位精度


	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit26)); //波导开关
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit27)); //放电触发信号
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit28)); //充电触发信号
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit29)); //导前触发信号
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit30)); //功放门套信号
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit31)); //通风单元1
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit32)); //通风单元2

	return svBitStr;

}

std::string CCCRadarParser::getXmtStr()
{
	std::string xmtBitStr;

	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit0));			//冷却开关
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit1));			//磁场开关
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit2));			//回扫开关
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit3));			//无充电触发
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit4));			//无放电触发
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit5));			//超占空比
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit6));			//固态功放
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit7));			//固态欠输出
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit8));			//磁场电源1
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit9));			//磁场电源2
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit10));			//灯丝电源
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit11));			//电源通风
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit12));			//回扫电源
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit13));			//充电过荷
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit14));			//人工线过压
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit15));			//人工线电压低
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit16));			//反峰
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit17));			//可控硅
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit18));			//可控硅风机
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit19));			//KLY总流
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit20));			//KLY管体
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit21));			//KLY总流节点
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit22));			//KLY管体节点
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit23));			//KLY温度
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit24));			//线包温度
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit25));			//钛泵欠压
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit26));			//钛泵过流
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit27));			//+15V电源（高压电源）
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit28));			//+15V电源（调制器）
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit29));			//+24V电源（监控）
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit30));			//+24V电源（调制器）
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit31));			//+60V电源（调制器）
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit32));			//机内温度过高
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit33));			//机内温度过低
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit34));			//机柜门开启
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit35));			//天线罩门开启
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit36));			//紧急开关

	return xmtBitStr;
}
