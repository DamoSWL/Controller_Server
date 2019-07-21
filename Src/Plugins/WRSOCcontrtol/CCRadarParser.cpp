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
		GetPlug()->AddLog(LOG_TYPE_ERROR,"�״����߼������ļ�Ŀ¼�쳣");
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


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit20)); //����ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit21)); //����ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit22)); //�źŴ���ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit23)); //�ŷ�ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit24)); //���չ��ʼ��ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit25)); //KLY���ʼ��ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit26)); //���ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit27)); //���߹��ʼ��ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit28)); //ʱ�������ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit29)); //����������ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit30)); //UPS��Դͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit31)); //��ʪ����1-������ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit32)); //��ʪ����2-ת̨ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit33)); //��ʪ����3-����ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit34)); //��ʪ����4-����ͨ��
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit35)); //��ʪ����5-����ͨ��


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit38)); //��������ѹ�澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit39)); //�������¶ȸ澯


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit47)); //�������¶ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit48)); //������ʪ�ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit49)); //ת̨�¶ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit50)); //ת̨ʪ�ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit51)); //�����¶ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit52)); //����ʪ�ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit53)); //�ۺϻ����¶ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit54)); //�ۺϻ���ʪ�ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit55)); //��������¶ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit56)); //�������ʪ�ȸ澯
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit57)); //�������¶ȸ澯


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit62));


	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit65)); //�״ﳣ��C
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit66)); //���곣��SYSCAL
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit67)); //��λ����
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit68)); //�Ӳ�����
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit69)); //CW�������
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit70)); //RFD�������
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit71)); //KD�������
	pwrBitStr.append(std::to_string((LONGLONG)m_CCRadarData.pwrBit72)); //�ٶȲ������

	return pwrBitStr;
}

std::string CCCRadarParser::getRcvStr()
{
	std::string rcvBitStr;

	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit0)); //H·��ͨ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit1)); //H·��ͨ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit2)); //V·��ͨ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit3)); //V·��ͨ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit4)); //һ����
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit5)); //������
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit6)); //����
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit7)); //150Mzʱ��1
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit8)); //�ϱ�Ƶ
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit9)); //84MHzʱ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit10)); //150Mzʱ��2
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit11)); //����ֱ����Դ
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit12)); //DDSʱ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit13)); //AD����ʱ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit14)); //ϵͳʱ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit15)); //Burstͨ��
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit16)); //��ͨ��H1������ƽ�澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit17)); //��ͨ��H2������ƽ�澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit18)); //��ͨ��H1����澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit19)); //��ͨ��H2����澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit20)); //ͨ�������澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit21)); //����ϵ���澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit22)); //Burstͨ������澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit23)); //Burstͨ��������ƽ�澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit24)); //һ�����źŸ澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit25)); //�����źŸ澯
	rcvBitStr.append(std::to_string((LONGLONG)m_CCRadarData.rcvBit26)); //DDS�����źŸ澯

	return rcvBitStr;
}

std::string CCCRadarParser::getSvoStr()
{
	std::string svBitStr;

	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit0)); //��IQ����
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit1)); //���ݶ���
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit2)); //�����������ʧ��
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit3)); //�޲������
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit4)); //�洢�ռ����


	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit7)); //��λ������
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit8)); //����������
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit9)); //R/D����
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit10)); //ң/����
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit11)); //+15V����
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit12)); //-15V����
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit13)); //������λ
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit14)); //��λ��λ
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit15)); //�嶥����
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit16)); //��׹���
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit17)); //���߷�λ��������
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit18)); //���߸�����������
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit19)); //��λ��λ���
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit20)); //������λ���
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit21)); //���߶�λ����


	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit26)); //��������
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit27)); //�ŵ紥���ź�
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit28)); //��紥���ź�
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit29)); //��ǰ�����ź�
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit30)); //���������ź�
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit31)); //ͨ�絥Ԫ1
	svBitStr.append(std::to_string((LONGLONG)m_CCRadarData.svBit32)); //ͨ�絥Ԫ2

	return svBitStr;

}

std::string CCCRadarParser::getXmtStr()
{
	std::string xmtBitStr;

	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit0));			//��ȴ����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit1));			//�ų�����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit2));			//��ɨ����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit3));			//�޳�紥��
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit4));			//�޷ŵ紥��
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit5));			//��ռ�ձ�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit6));			//��̬����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit7));			//��̬Ƿ���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit8));			//�ų���Դ1
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit9));			//�ų���Դ2
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit10));			//��˿��Դ
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit11));			//��Դͨ��
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit12));			//��ɨ��Դ
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit13));			//������
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit14));			//�˹��߹�ѹ
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit15));			//�˹��ߵ�ѹ��
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit16));			//����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit17));			//�ɿع�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit18));			//�ɿع���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit19));			//KLY����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit20));			//KLY����
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit21));			//KLY�����ڵ�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit22));			//KLY����ڵ�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit23));			//KLY�¶�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit24));			//�߰��¶�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit25));			//�ѱ�Ƿѹ
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit26));			//�ѱù���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit27));			//+15V��Դ����ѹ��Դ��
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit28));			//+15V��Դ����������
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit29));			//+24V��Դ����أ�
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit30));			//+24V��Դ����������
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit31));			//+60V��Դ����������
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit32));			//�����¶ȹ���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit33));			//�����¶ȹ���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit34));			//�����ſ���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit35));			//�������ſ���
	xmtBitStr.append(std::to_string((LONGLONG)m_CCRadarData.xmtBit36));			//��������

	return xmtBitStr;
}
