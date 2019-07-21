
#include "CDRadarParser.h"
#include <wx/dir.h>

CCDRadarParser::CCDRadarParser(void):m_updateTime(0)
{
}


CCDRadarParser::~CCDRadarParser(void)
{
}

DWORD WINAPI CCDRadarParser::exec(LPVOID para)
{
	CCDRadarParser* self = static_cast<CCDRadarParser*>(para);

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

wxString CCDRadarParser::getLatestFileName(time_t* newTime)
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
		bool flag = fileDir.GetFirst(&fileName, wxT("Z_R_DWRN_SRSI*"), wxDIR_FILES );
		while( flag )
		{
			if(GetPlug()->GetExit())
			{
				break;
			}

			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			wxFileName fileAtrribute(fullFilePath);
			if(!fileAtrribute.IsOk())
			{
				GetPlug()->AddLog(LOG_TYPE_ERROR,fullFilePath.mb_str(wxConvLocal));
				break;
			}
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

BOOL CCDRadarParser::loadFile(time_t* newTime)
{
	auto fileName = getLatestFileName(newTime);
	if (fileName.IsEmpty())
	{
		return FALSE;
	}

	wxFile file(fileName,wxFile::read);
	if (file.IsOpened())
	{
		m_CDAlarm.getAlarmType();
		m_CDAlarm.getThresholdValue();

		file.Seek(sizeof(RadarStructure::GeneHeader) + sizeof(RadarStructure::SiteInfo) + sizeof(RadarStructure::RadarStatus),wxFromStart);
		file.Read((void*)&m_CDRadarData,sizeof(RadarStructure::CDStructure));
		file.Read((void*)&m_CDAdaptParam,sizeof(RadarStructure::SCAdaptParam));

		m_CDAlarm.getCDValue(m_CDRadarData);
		m_CDAlarm.CDCheck();

		file.Close();

	}
	else
	{
		file.Close();
		return FALSE;
	}

	return TRUE;
}

BOOL CCDRadarParser::writeToDataBase(time_t newTime)
{
	// wxString sqlSentence = wxT("INSERT INTO onlinemonitorpoint_cd(updateTime,  \
	// 							rda_ver_name, \
	// 							rf_power_smoothing, \
	// 							hxmtlosssingle, \
	// 							hxmtlosshv, \
	// 							hrcvloss, \
	// 							htestloss, \
	// 							vxmtlosshv, \
	// 							vrcvloss, \
	// 							vtestloss, \
	// 							pathloss_noise_h, \
	// 							pathloss_noise_v, \
	// 							ts_cw, \
	// 							ts_noise, \
	// 							pathloss_rf, \
	// 							ts_rf, \
	// 							pathloss_kly, \
	// 							gas_loss, \
	// 							radome_loss, \
	// 						   feederPressure, \
	// 						   feederHumidity, \
	// 						   feederLoss, \
	// 						   xmtBit_cd, \
	// 						   pwr12V, \
	// 						   chg15V, \
	// 						   chg_15V, \
	// 						   trig12V, \
	// 						   trig24V, \
	// 						   monit12V, \
	// 						   filaVolt, \
	// 						   filaAmp, \
	// 						   biasAmp, \
	// 						   vacionVolt, \
	// 						   vacionAmp, \
	// 						   klyTemp, \
	// 						   rectVolt, \
	// 						   highPwrAmp, \
	// 						   collAmp, \
	// 						   nPeakAmp, \
	// 						   rcv5V, \
	// 						   rcv_5V, \
	// 						   rcv12V, \
	// 						   rcv15V, \
	// 						   rcv24V, \
	// 						   hFiledAmp, \
	// 						   vFiledAmp, \
	// 						   hPresel, \
	// 						   vPresel, \
	// 						   hMixer, \
	// 						   vMixer, \
	// 						   hFront, \
	// 						   vFront, \
	// 						   hDigit, \
	// 						   vDigit, \
	// 						   testSignalF, \
	// 						   testSignalB, \
	// 						   hFiledFront, \
	// 						   vFiledFront, \
	// 						   excitOut, \
	// 						   testOut, \
	// 						   intrinOut, \
	// 						   COHO, \
	// 						   RFD, \
	// 						   hKD, \
	// 						   vKD, \
	// 						   pAUp, \
	// 						   pADown, \
	// 						   hDischg, \
	// 						   vDischg, \
	// 						   upLIC, \
	// 						   upHLIC, \
	// 						   upVLIC, \
	// 						   downLIC, \
	// 						   refLIC, \
	// 						   caseTemp, \
	// 						   rcvBit, \
	// 						   sv5V, \
	// 						   sv24V, \
	// 						   azAmp, \
	// 						   azCmdRmp, \
	// 						   azCurRmp, \
	// 						   elAmp, \
	// 						   elCmdRmp, \
	// 						   elCurRmp, \
	// 						   svBit, \
	// 						   aVolt, \
	// 						   aAmp, \
	// 						   bVolt, \
	// 						   bAmp, \
	// 						   cVolt, \
	// 						   cAmp, \
	// 						   pwrBit, \
	// 						   ant_temp, \
	// 						   ant_humidity, \
	// 						   roomTemp, \
	// 						   roomHumidity) VALUES");


	wxString sqlSentence = wxT("INSERT INTO onlinemonitorpoint (updateTime,  \
																		wholeBit, \
																		FdPress, \
																		FdHumi, \
																		AntTemp, \
																		AntHumi, \
																		RoomTemp, \
																		RoomHumi, \
																		KlyTemp, \
																		KlyHumi, \
																		MagTemp, \
																		MagHumi, \
																		CaseTemp, \
																		XmtPwrUp0, \
																		XmtPwrUp1, \
																		XmtPwrUp2, \
																		XmtPwrDn, \
																		XmtRefDn, \
																		PulseW, \
																		pwrBit, \
																		AAmp, \
																		BAmp, \
																		CAmp, \
																		AVolt, \
																		BVolt, \
																		CVolt, \
																		rcvBit, \
																		ExcitOut, \
																		TestOut, \
																		LoscOut, \
																		COHO, \
																		Rcv12V, \
																		Rcv15V, \
																		Rcv5V, \
																		Rcv_5V, \
																		Rcv24V, \
																		HFieldAmp, \
																		VFieldAmp, \
																		svBit, \
																		Sv12V, \
																		Sv24V, \
																		AzAmp, \
																		AzCmdRpm, \
																		AzCurRpm, \
																		ElAmp, \
																		ElCmdRpm, \
																		ElCurRpm, \
																		xmtBit, \
																		RectVolt, \
																		HighPwrAmp, \
																		FilaAmp, \
																		BiasAmp, \
																		Monit5V, \
																		Monit24V, \
																		Monit12V, \
																		Monit_12V, \
																		CollAmp, \
																		nPeakAmp, \
																		VacionVolt, \
																		VacionAmp, \
																		Chg15V, \
																		Chg_15V, \
																		Trig12V, \
																		Trig24V, \
																		Front12V, \
																		Front24V) VALUES");
	// wxString sqlError;
	auto  pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {

			const double COF = 0.01;
			const double doucof = 1.0;

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "),newTime));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getWholeBit().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.FdPress * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.FdHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.AntTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.AntHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.RoomTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.RoomHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.KlyTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.KlyHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.MagTemp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.MagHumi * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.CaseTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.XmtPwrUp[0] * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.XmtPwrUp[1] * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.XmtPwrUp[2] * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.XmtPwrDn * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.XmtRefDn * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_ws.PulseW * doucof));

			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getPwrBit().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_pwrmcu.AAmp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_pwrmcu.BAmp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_pwrmcu.CAmp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_pwrmcu.AVolt * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_pwrmcu.BVolt * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_pwrmcu.CVolt * doucof));

			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getRcvStr().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.ExcitOut * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.TestOut * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.LoscOut * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.COHO * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.Rcv12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.Rcv15V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.Rcv5V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.Rcv_5V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.Rcv24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.HFieldAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_recv.VFieldAmp * COF));

			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getSvoStr().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.Sv12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.Sv24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.AzAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.AzCmdRpm * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.AzCurRpm * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.ElAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.ElCmdRpm * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_sv.ElCurRpm * doucof));


			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getXmtStr().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.RectVolt * 0.1));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.HighPwrAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.FilaAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.BiasAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Monit5V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Monit24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Monit12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Monit_12V * COF));

			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.CollAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.nPeakAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.VacionVolt * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.VacionAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Chg15V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Chg_15V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Trig12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Trig24V * COF));

			sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.m_xmt.Front12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f)"), m_CDRadarData.m_xmt.Front24V * COF));


			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.FeederPressure / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.FeederHumidity / COF));
			// sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDRadarData.FeederLoss));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getCDXMTBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Pwr12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Chg15V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Chg_15V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Trig12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Trig24V /COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Monit12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.FilaVolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.FilaAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.BiasAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.VacionVolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.VacionAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.KlyTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.RectVolt / RECTVOLT));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.HighPwrAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.CollAmp/ COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.nPeakAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Rcv5V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Rcv_5V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Rcv12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Rcv15V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Rcv24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.HFiledAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.VFiledAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Hpresel / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Vpresel / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Hmixer / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Vmixer / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Hfront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Vfront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Hdigit / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Vdigit / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.TestSignalF / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.TestSignalB / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.HFiledFront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.VFiledFront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.ExcitOut / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.TestOut / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.IntrinOut / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.COHO / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.RFD / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.HKD / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.VKD / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.PAUp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.PADown / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Hdischg / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Vdischg / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.UpLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.UpHLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.UpVLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.DownLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.RefLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.CaseTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getCDRCVBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Sv5V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Sv24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.AzAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.AzCmdRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.AzCurRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.ElAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.ElCmdRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.ElCurRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getCDSVBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Avolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Aamp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Bvolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Bamp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Cvolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.Camp / COF));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getCDPWRBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.AntTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.AntHumidity / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDRadarData.RoomTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f)"), m_CDRadarData.RoomHumidity / COF));


#ifdef __TESTWRSOC__
			auto state14 = GetPlug()->getState14();
			if((state14 == 0) || (state14 == 5)) 
			{
				return TRUE;
			}
#endif
			

			bool ret =  pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}


		}
		// catch (wxString& error)
		// {
		// 	if(error.Find(wxT("Duplicate entry")) != wxNOT_FOUND)
		// 	{
		// 		return TRUE;
		// 	}
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	
	return TRUE;
}

BOOL CCDRadarParser::writeAdatpParamToDateBase(time_t newTime)
{
	wxString sqlSentence = wxT("INSERT INTO adaptparameter (updateTime,  \
							   sSiteNum, \
							   sSiteName, \
							   fLatitude, \
							   fLongtitude, \
							   iAntHeight, \
							   iAltitude, \
							   iTimeZone, \
							   iAMT, \
							   iAMP, \
							   fAirRefract, \
							   fXmtFreq, \
							   fHBWidth, \
							   fVBWidth, \
							   iRDAVersion, \
							   RadarType, \
							   iAntGain, \
							   iGasAttenu, \
							   iXmtLoss, \
							   iXmtPwrLoss, \
							   iRecWorkLoss, \
							   iRecTestLoss, \
							   iNSrcLoss, \
							   iCWSingal, \
							   iRFDSingal, \
							   iNoiseENR, \
							   iZdrBias, \
							   iPdpBias, \
							   bAutoCalib, \
							   iCheckTime, \
							   iDyncStart, \
							   iDyncEnd, \
							   iDyncStep, \
							   iNSF, \
							   iRFDAttenu, \
							   iKDAttenu, \
							   iCWAttenu, \
							   iCSAttenu, \
							   iNLevelThres, \
							   iNTempThres, \
							   iTgtThres_rfd, \
							   iTgtThres_kd, \
							   iReflCheckThres, \
							   iRadConstThres, \
							   iClutSupprThres, \
							   iVelCheckThres, \
							   iSWCheckThres, \
							   iZdrThres, \
							   ipw, \
							   itpower, \
							   iRadarConst, \
							   iNoiseLevel, \
							   iKDSingal, \
							   iDistBin, \
							   iMinFreq, \
							   iMaxFreq, \
							   freq, \
							   idBz0, \
							   ipw_w, \
							   itpower_w, \
							   iRadarConst_w, \
							   iNoiseLevel_w, \
							   iKDSingal_w, \
							   iDistBin_w, \
							   iMinFreq_w, \
							   iMaxFreq_w, \
							   freq_w, \
							   idBz0_w) VALUES");

	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		const double COF = 0.01;
		const double COF2 = 0.1;

		sqlSentence.Append(wxString::Format(wxT("(%I64d, "), newTime));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_CDAdaptParam.sSiteNum,wxConvLocal)));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_CDAdaptParam.sSiteName,wxConvLocal)));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.fLatitude));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.fLongtitude));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iAntHeight));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iAltitude));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iTimeZone));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iAMT * COF));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iAMP));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.fAirRefract));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.fXmtFreq));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.fHBWidth));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.fVBWidth));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iRDAVersion));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.RadarType));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iAntGain * COF));

		std::vector<FLOAT> adaptParam;
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iGasAttenu * 0.00001));

		adaptParam.push_back(m_CDAdaptParam.iXmtLoss[0][0] * COF);
		adaptParam.push_back(m_CDAdaptParam.iXmtLoss[0][1] * COF);
		adaptParam.push_back(m_CDAdaptParam.iXmtLoss[1][0] * COF);
		adaptParam.push_back(m_CDAdaptParam.iXmtLoss[1][1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 5;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.iXmtPwrLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 2;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.iRecWorkLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 2;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.iRecTestLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 2;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.iNSrcLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iCWSingal * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iRFDSingal * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iNoiseENR * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iZdrBias * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iPdpBias * COF));

		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.bAutoCalib));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iCheckTime));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iDyncStart));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iDyncEnd));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iDyncStep));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.iNSF * COF));

		adaptParam.clear();
		for(auto index = 0; index < 3;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.iRFDAttenu[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 3;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.iKDAttenu[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iCWAttenu));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_CDAdaptParam.iCSAttenu));


		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iNLevelThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iNLevelThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iNTempThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iNTempThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iTgtThres[0].iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iTgtThres[0].iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iTgtThres[1].iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iTgtThres[1].iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iReflCheckThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iReflCheckThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iRadConstThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iRadConstThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iClutSupprThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iClutSupprThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iVelCheckThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iVelCheckThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iSWCheckThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iSWCheckThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.iZdrThres.iMin * COF2);
		adaptParam.push_back(m_CDAdaptParam.iZdrThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[0].ipw * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[0].itpower * 1.0));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.pulse[0].iRadarConst[0] * COF);
		adaptParam.push_back(m_CDAdaptParam.pulse[0].iRadarConst[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.pulse[0].iNoiseLevel[0] * COF);
		adaptParam.push_back(m_CDAdaptParam.pulse[0].iNoiseLevel[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[0].iKDSingal * COF));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.pulse[0].iDistBin[0] * 1.0);
		adaptParam.push_back(m_CDAdaptParam.pulse[0].iDistBin[1] * 1.0);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[0].iMinFreq * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[0].iMaxFreq * 1.0));

		adaptParam.clear();
		for(auto index = 0; index < 10;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.pulse[0].freq[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 4;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.pulse[0].idBz0[index] * 0.01);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));


		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[1].ipw * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[1].itpower * 1.0));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.pulse[1].iRadarConst[0] * COF);
		adaptParam.push_back(m_CDAdaptParam.pulse[1].iRadarConst[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.pulse[1].iNoiseLevel[0] * COF);
		adaptParam.push_back(m_CDAdaptParam.pulse[1].iNoiseLevel[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[0].iKDSingal * COF));

		adaptParam.clear();
		adaptParam.push_back(m_CDAdaptParam.pulse[1].iDistBin[0] * 1.0);
		adaptParam.push_back(m_CDAdaptParam.pulse[1].iDistBin[1] * 1.0);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[1].iMinFreq * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_CDAdaptParam.pulse[1].iMaxFreq * 1.0));

		adaptParam.clear();
		for(auto index = 0; index < 10;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.pulse[1].freq[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 4;index++)
		{
			adaptParam.push_back(m_CDAdaptParam.pulse[1].idBz0[index] * 0.01);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s')"), getAdaptParamStr(adaptParam)));

		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}
		return TRUE;

	}

	return FALSE;
}

void CCDRadarParser::updateTime(time_t newTime)
{
	m_updateTime = newTime;
	GetPlug()->setradarDatatime(newTime);
}


std::string CCDRadarParser::getWholeBit()
{
	std::string wholeBitStr;
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.ps_rctrl));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.xmt_rctrl));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.RDA_rctrl));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.PwrSrc));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.low));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.ready));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.high));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.wpluse));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.WGS0));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.WGS90));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.antsafe));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.ps_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.xmt_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.recv_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.svo_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_ws.mcu_err));
	return wholeBitStr;
}


std::string CCDRadarParser::getPwrBit()
{
	std::string pwerBitStr;
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.xmt_a));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.xmt_b));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.xmt_c));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.pwr_sv));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.pwr_rcv));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.pwr_mcu));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.ps_rctrl));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.PwrSrc));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.XmtComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.AmpComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.RcvComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.SvoComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.AzDComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.ElDComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.PwrVComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.PwrAComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.AntComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.KlyComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.RoomComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.DiffComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_pwrmcu.PowComm));

	return pwerBitStr;
}


std::string CCDRadarParser::getRcvStr()
{
	std::string rcvBitStr;
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_excit));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_test));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_Hlosc));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_Vlosc));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_losc));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_COHO));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_HCOHO));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_VCOHO));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_12v));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_5v));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv__5v));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_15v));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_24v));
	rcvBitStr.append(std::to_string(0LL));
	rcvBitStr.append(std::to_string(0LL));
	rcvBitStr.append(std::to_string(0LL));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_Hchan));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.rcv_Vchan));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.WGS0));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.WGS90));
	rcvBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_recv.dummy));
	return rcvBitStr;
}



std::string CCDRadarParser::getSvoStr()
{
	std::string svoBitStr;
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.az_drihot));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.az_voltO));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.az_over));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.az_rota));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.az_rdc));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.az_voltL));
	svoBitStr.append(std::to_string(0LL));
	svoBitStr.append(std::to_string(0LL));

	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_drihot));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_voltO));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_over));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_rota));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_rdc));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_voltL));
	svoBitStr.append(std::to_string(0LL));
	svoBitStr.append(std::to_string(0LL));

	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el__2deg));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.el_90deg));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.antsafe));
	svoBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_sv.exciting));
	
	return svoBitStr;
}


std::string CCDRadarParser::getXmtStr()
{
	std::string xmtBitStr;
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.p2));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.p1));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.lctrl));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.rctrl));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.low));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.delay));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.ready));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.high));


	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.fan));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.plc));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.dtrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.cooling));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.oilhot));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.igbt));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.vacion));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.bias));

	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.filaover));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.filalack));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.door));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.klyhot));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.antsafe));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.energy));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.npeak));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.collect));

	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.end_AmpSrc));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_AmpSrc));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_Amp2us));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_Amp1us));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_AmpClk));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_AmpTrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_AmpPow));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.front_AmpExc));

	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.coll));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.fback));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.ctrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.tri1));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.tri2));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.tri3));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.tri4));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.tri5));

	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.tri6));
	xmtBitStr.append(std::to_string((LONGLONG)m_CDRadarData.m_xmt.trifan));
	
	return xmtBitStr;
}

wxString CCDRadarParser::getAdaptParamStr(const std::vector<FLOAT>& adaptParam)
{
	wxString adaptParamStr;
	for(auto item = adaptParam.cbegin(); item != adaptParam.cend(); item++)
	{
		adaptParamStr.Append(wxString::Format(wxT("%.2f#"),*item));
	}

	return adaptParamStr.Left(adaptParamStr.Length()-1);
}
