#include "SCRadarParser.h"
#include <wx/dir.h>




CSCRadarParser::CSCRadarParser():m_updateTime(0)
{
}


CSCRadarParser::~CSCRadarParser()
{
}

DWORD CSCRadarParser::exec(LPVOID para)
{
	CSCRadarParser* self = static_cast<CSCRadarParser*>(para);

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

void CSCRadarParser::updateTime(time_t newTime)
{
	m_updateTime = newTime;
	GetPlug()->setradarDatatime(newTime);
}

BOOL CSCRadarParser::loadFile(time_t* newTime)
{
	auto fileName = getLatestFileName(newTime);
	if (fileName.IsEmpty())
	{
		return FALSE;
	}

	wxFile file(fileName,wxFile::read);
	if (file.IsOpened())
	{
		m_SCAlarm.getAlarmType();
		m_SCAlarm.getThresholdValue();

		file.Seek(sizeof(RadarStructure::GeneHeader) + sizeof(RadarStructure::SiteInfo) + sizeof(RadarStructure::RadarStatus),wxFromStart);
		file.Read((void*)&m_SCRadarData,sizeof(RadarStructure::SCStructure));
		file.Read((void*)&m_SCAdaptParam,sizeof(RadarStructure::SCAdaptParam));

		m_SCAlarm.getSCValue(m_SCRadarData);
		m_SCAlarm.SCCheck();
		file.Close();

	}
	else
	{
		file.Close();
		return FALSE;
	}

	return TRUE;


}

BOOL CSCRadarParser::writeToDataBase(time_t newTime)
{

	// wxString sqlSentence = wxT("INSERT INTO onlinemonitorpoint_sc(updateTime,  \
	// 																		rda_ver_name, \
	// 																		rf_power_smoothing, \
	// 																		hxmtlosssingle, \
	// 																		hxmtlosshv, \
	// 																		hrcvloss, \
	// 																		htestloss, \
	// 																		vxmtlosshv, \
	// 																		vrcvloss, \
	// 																		vtestloss, \
	// 																		pathloss_noise_h, \
	// 																		pathloss_noise_v, \
	// 																		ts_cw, \
	// 																		ts_noise, \
	// 																		pathloss_rf, \
	// 																		ts_rf, \
	// 																		pathloss_kly, \
	// 																		gas_loss, \
	// 																		radome_loss, \
	// 																		feederPressure, \
	// 																		feederHumidity, \
	// 																		feederLoss, \
	// 																		xmtBit, \
	// 																		temp, \
	// 																		humi, \
	// 																		magn1Amp, \
	// 																		magn2Amp, \
	// 																		filaAmp, \
	// 																		biasAmp, \
	// 																		collAmp, \
	// 																		vacionAmp, \
	// 																		vacionVolt, \
	// 																		front12V, \
	// 																		front24V, \
	// 																		front36V, \
	// 																		end12V, \
	// 																		end24V, \
	// 																		end36V, \
	// 																		highPwrAmp, \
	// 																		volt510, \
	// 																		nPeakAmp, \
	// 																		volt24V, \
	// 																		volt15V, \
	// 																		volt_15V, \
	// 																		trig12V, \
	// 																		trig200V, \
	// 																		rcv5V, \
	// 																		rcv_5V, \
	// 																		rcv12V, \
	// 																		rcv15V, \
	// 																		rcv24V, \
	// 																		hFiledAmp, \
	// 																		vFiledAmp, \
	// 																		hPresel, \
	// 																		vPresel, \
	// 																		hMixer, \
	// 																		vMixer, \
	// 																		hFront, \
	// 																		vFront, \
	// 																		hDigit, \
	// 																		vDigit, \
	// 																		testSignalF, \
	// 																		testSignalB, \
	// 																		hFiledFront, \
	// 																		vFiledFront, \
	// 																		excitOut, \
	// 																		testOut, \
	// 																		intrinOut, \
	// 																		COHO, \
	// 																		RFD, \
	// 																		hKD, \
	// 																		vKD, \
	// 																		pAUp, \
	// 																		pADown, \
	// 																		hDischg, \
	// 																		vDischg, \
	// 																		upLIC, \
	// 																		upHLIC, \
	// 																		upVLIC, \
	// 																		downLIC, \
	// 																		refLIC, \
	// 																		caseTemp, \
	// 																		rcvBit, \
	// 																		sv5V, \
	// 																		sv24V, \
	// 																		azAmp, \
	// 																		azCmdRmp, \
	// 																		azCurRmp, \
	// 																		elAmp, \
	// 																		elCmdRmp, \
	// 																		elCurRmp, \
	// 																		svBit, \
	// 																		aVolt, \
	// 																		aAmp, \
	// 																		bVolt, \
	// 																		bAmp, \
	// 																		cVolt, \
	// 																		cAmp, \
	// 																		pwrBit, \
	// 																		ant_Temp, \
	// 																		ant_Humidity, \
	// 																		roomTemp, \
	// 																		roomHumidity) VALUES");


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
																	frontst, \
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
																	Front24V, \
																	Front36V, \
																	eamp12V, \
																	eamp24V, \
																	eamp36V, \
																	Filadisc, \
																	xmt510Volt, \
																	Mang1Amp, \
																	Mang2Amp, \
																	Trifan24V) VALUES");

	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			const double COF = 0.01;
			const double doucof = 1.0;


			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), newTime));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getWholeBit().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.FdPress * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.FdHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.AntTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.AntHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.RoomTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.RoomHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.KlyTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.KlyHumi * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.MagTemp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.MagHumi * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.CaseTemp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.XmtPwrUp[0] * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.XmtPwrUp[1] * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.XmtPwrUp[2] * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.XmtPwrDn * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.XmtRefDn * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_ws.PulseW * doucof));

			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getPwrBit().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_pwrmcu.AAmp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_pwrmcu.BAmp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_pwrmcu.CAmp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_pwrmcu.AVolt * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_pwrmcu.BVolt * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_pwrmcu.CVolt * doucof));

			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getRcvStr().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.ExcitOut * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.TestOut * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.LoscOut * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.COHO * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.Rcv12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.Rcv15V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.Rcv5V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.Rcv_5V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.Rcv24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.HFieldAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_recv.VFieldAmp * COF));

			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getSvoStr().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.Sv12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.Sv24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.AzAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.AzCmdRpm * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.AzCurRpm * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.ElAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.ElCmdRpm * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_sv.ElCurRpm * doucof));


			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getXmtStr().c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.frontst * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.HighPwrAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.FilaAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.BiasAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Monit5V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Monit24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Monit12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Monit_12V * COF));

			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.CollAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.nPeakAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.VacionVolt * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.VacionAmp * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Chg15V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Chg_15V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Trig12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Trig24V * COF));

			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Front12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Front24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Front36V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.eamp12V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.eamp24V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.eamp36V * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Filadisc * COF));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.xmt510Volt * 0.1));

			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Mang1Amp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.m_xmt.Mang2Amp * doucof));
			sqlSentence.Append(wxString::Format(wxT("%f)"), m_SCRadarData.m_xmt.Trifan24V * COF));

			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.FeederPressure / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.FeederHumidity / COF));
			// sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCRadarData.FeederLoss));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getSCXMTBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Temp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Humi / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Magn1Amp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Magn2Amp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.FilaAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.BiasAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.CollAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.VacionAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.VacionVolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Front12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Front24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Front36V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.End12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.End24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.End36V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.HighPwrAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Volt510 / VOLT510V));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.nPeakAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Volt24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Volt15V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Volt_15V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Trig12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Trig200V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Rcv5V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Rcv_5V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Rcv12V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Rcv15V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Rcv24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.HFiledAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.VFiledAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Hpresel / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Vpresel / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Hmixer / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Vmixer / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Hfront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Vfront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Hdigit / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Vdigit / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.TestSignalF / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.TestSignalB / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.HFiledFront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.VFiledFront / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.ExcitOut / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.TestOut / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.IntrinOut / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.COHO / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.RFD / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.HKD / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.VKD / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.PAUp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.PADown / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Hdischg / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Vdischg / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.UpLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.UpHLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.UpVLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.DownLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.RefLIC / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.CaseTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(getSCRCVBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Sv5V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Sv24V / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.AzAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.AzCmdRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.AzCurRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.ElAmp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.ElCmdRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.ElCurRmp / 1.0));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(getSCSVBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Avolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Aamp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Bvolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Bamp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Cvolt / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.Camp / COF));
			// sqlSentence.Append(wxString::Format(wxT("'%s', "),wxString(getSCPWRBitStr().c_str(),wxConvLocal)));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.AntTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.AntHumidity / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCRadarData.RoomTemp / COF));
			// sqlSentence.Append(wxString::Format(wxT("%f)"), m_SCRadarData.RoomHumidity / COF));
			

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


BOOL CSCRadarParser::writeAdatpParamToDateBase(time_t newTime)
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
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_SCAdaptParam.sSiteNum,wxConvLocal)));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_SCAdaptParam.sSiteName,wxConvLocal)));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.fLatitude));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.fLongtitude));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iAntHeight));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iAltitude));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iTimeZone));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iAMT * COF));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iAMP));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.fAirRefract));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.fXmtFreq));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.fHBWidth));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.fVBWidth));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iRDAVersion));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.RadarType));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iAntGain * COF));

		std::vector<FLOAT> adaptParam;
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iGasAttenu * 0.00001));

		adaptParam.push_back(m_SCAdaptParam.iXmtLoss[0][0] * COF);
		adaptParam.push_back(m_SCAdaptParam.iXmtLoss[0][1] * COF);
		adaptParam.push_back(m_SCAdaptParam.iXmtLoss[1][0] * COF);
		adaptParam.push_back(m_SCAdaptParam.iXmtLoss[1][1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 5;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.iXmtPwrLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 2;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.iRecWorkLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 2;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.iRecTestLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 2;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.iNSrcLoss[index] * COF);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iCWSingal * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iRFDSingal * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iNoiseENR * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iZdrBias * COF));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iPdpBias * COF));

		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.bAutoCalib));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iCheckTime));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iDyncStart));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iDyncEnd));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iDyncStep));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.iNSF * COF));

		adaptParam.clear();
		for(auto index = 0; index < 3;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.iRFDAttenu[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 3;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.iKDAttenu[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iCWAttenu));
		sqlSentence.Append(wxString::Format(wxT("%d, "), m_SCAdaptParam.iCSAttenu));


		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iNLevelThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iNLevelThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iNTempThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iNTempThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iTgtThres[0].iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iTgtThres[0].iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iTgtThres[1].iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iTgtThres[1].iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iReflCheckThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iReflCheckThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iRadConstThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iRadConstThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iClutSupprThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iClutSupprThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iVelCheckThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iVelCheckThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iSWCheckThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iSWCheckThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.iZdrThres.iMin * COF2);
		adaptParam.push_back(m_SCAdaptParam.iZdrThres.iMax * COF2);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[0].ipw * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[0].itpower * 1.0));
	
		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.pulse[0].iRadarConst[0] * COF);
		adaptParam.push_back(m_SCAdaptParam.pulse[0].iRadarConst[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.pulse[0].iNoiseLevel[0] * COF);
		adaptParam.push_back(m_SCAdaptParam.pulse[0].iNoiseLevel[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[0].iKDSingal * COF));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.pulse[0].iDistBin[0] * 1.0);
		adaptParam.push_back(m_SCAdaptParam.pulse[0].iDistBin[1] * 1.0);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[0].iMinFreq * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[0].iMaxFreq * 1.0));

		adaptParam.clear();
		for(auto index = 0; index < 10;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.pulse[0].freq[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 4;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.pulse[0].idBz0[index] * 0.01);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));


		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[1].ipw * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[1].itpower * 1.0));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.pulse[1].iRadarConst[0] * COF);
		adaptParam.push_back(m_SCAdaptParam.pulse[1].iRadarConst[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.pulse[1].iNoiseLevel[0] * COF);
		adaptParam.push_back(m_SCAdaptParam.pulse[1].iNoiseLevel[1] * COF);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[0].iKDSingal * COF));

		adaptParam.clear();
		adaptParam.push_back(m_SCAdaptParam.pulse[1].iDistBin[0] * 1.0);
		adaptParam.push_back(m_SCAdaptParam.pulse[1].iDistBin[1] * 1.0);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[1].iMinFreq * 1.0));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_SCAdaptParam.pulse[1].iMaxFreq * 1.0));

		adaptParam.clear();
		for(auto index = 0; index < 10;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.pulse[1].freq[index] * 1.0);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s', "), getAdaptParamStr(adaptParam)));

		adaptParam.clear();
		for(auto index = 0; index < 4;index++)
		{
			adaptParam.push_back(m_SCAdaptParam.pulse[1].idBz0[index] * 0.01);
		}
		sqlSentence.Append(wxString::Format(wxT("'%s')"), getAdaptParamStr(adaptParam)));

		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}

	}

	return TRUE;
}

wxString CSCRadarParser::getLatestFileName(time_t* newTime)
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

std::string CSCRadarParser::getWholeBit()
{
	std::string wholeBitStr;
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.ps_rctrl));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.xmt_rctrl));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.RDA_rctrl));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.PwrSrc));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.low));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.ready));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.high));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.wpluse));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.WGS0));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.WGS90));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.antsafe));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.ps_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.xmt_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.recv_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.svo_err));
	wholeBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_ws.mcu_err));
	return wholeBitStr;
}


std::string CSCRadarParser::getPwrBit()
{
	std::string pwerBitStr;
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.xmt_a));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.xmt_b));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.xmt_c));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.pwr_sv));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.pwr_rcv));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.pwr_mcu));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.ps_rctrl));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.PwrSrc));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.XmtComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.AmpComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.RcvComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.SvoComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.AzDComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.ElDComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.PwrVComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.PwrAComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.AntComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.KlyComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.RoomComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.DiffComm));
	pwerBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_pwrmcu.PowComm));

	return pwerBitStr;
}


std::string CSCRadarParser::getRcvStr()
{
	std::string rcvBitStr;
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_excit));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_test));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_Hlosc));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_Vlosc));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_losc));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_COHO));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_HCOHO));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_VCOHO));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_12v));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_5v));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv__5v));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_15v));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_24v));
	rcvBitStr.append(std::to_string(0LL));
	rcvBitStr.append(std::to_string(0LL));
	rcvBitStr.append(std::to_string(0LL));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_Hchan));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.rcv_Vchan));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.WGS0));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.WGS90));
	rcvBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_recv.dummy));
	return rcvBitStr;
}



std::string CSCRadarParser::getSvoStr()
{
	std::string svoBitStr;
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.az_drihot));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.az_voltO));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.az_over));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.az_rota));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.az_rdc));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.az_voltL));
	svoBitStr.append(std::to_string(0LL));
	svoBitStr.append(std::to_string(0LL));

	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_drihot));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_voltO));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_over));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_rota));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_rdc));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_voltL));
	svoBitStr.append(std::to_string(0LL));
	svoBitStr.append(std::to_string(0LL));

	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el__2deg));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.el_90deg));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.antsafe));
	svoBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_sv.exciting));
	
	return svoBitStr;
}


std::string CSCRadarParser::getXmtStr()
{
	std::string xmtBitStr;
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.p4));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.p1));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.lctrl));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.rctrl));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.low));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.delay));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.ready));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.high));


	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.fan));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.plc));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.dtrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.cooling));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.oilhot));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.igbt));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.vacion));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.bias));

	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.filaover));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.filalack));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.door));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.klyhot));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.antsafe));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.energy));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.npeak));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.collect));

	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.end_AmpSrc));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_AmpSrc));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_Amp4us));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_Amp1us));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_AmpClk));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_AmpTrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_AmpPow));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.front_AmpExc));

	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.coll));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.fback));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.ctrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.mag1un));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.mag1over));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.mag2un));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.mag2over));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.fire));

	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.coilfan));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.fanj));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.pfn));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.highall));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.tritrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.work));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.test));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.tri));

	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.eampexc));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.eampclk));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.eamptrig));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.eamppow));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.eamp1us));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.eamp4us));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.coilhot));
	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.losspha));

	xmtBitStr.append(std::to_string((LONGLONG)m_SCRadarData.m_xmt.highpd));
	
	return xmtBitStr;
}

wxString CSCRadarParser::getAdaptParamStr(const std::vector<FLOAT>& adaptParam)
{
	wxString adaptParamStr;
	for(auto item = adaptParam.cbegin(); item != adaptParam.cend(); item++)
	{
		adaptParamStr.Append(wxString::Format(wxT("%.2f#"),*item));
	}

	return adaptParamStr.Left(adaptParamStr.Length()-1);
}

