#include "RadarParser.h"
#include <wx/dir.h>
#include <ctime>
#include <cmath>



CRadarParser::CRadarParser():m_updateTime(0)
{
}


CRadarParser::~CRadarParser()
{
}

DWORD CRadarParser::exec(LPVOID para)
{
	CRadarParser* self = static_cast<CRadarParser*>(para);
	
	if(self)
	{
		self->setUpdateTime(GetPlug()->getfileConfig().radarStatustime);
		time_t newTime = 0;

		self->startRadarFileDetection();

		while (!GetPlug()->GetExit())
		{

 			if (self->loadFile(&newTime))
 			{
				
				if(self->writeSiteToDataBase(newTime) && self->writeRadarDataToBase(newTime))
				{				
					self->updateTime(newTime);
				}
				self->startRadarFileDetection();  
			}

			if(newTime > (time_t)(::wxDateTime::GetTimeNow()- 10*60))
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


BOOL CRadarParser::loadFile(time_t* newTime)
{
	auto fileName = getLatestFileName(newTime);
	if (fileName.IsEmpty())
	{
		return FALSE;
	}
 
 	wxFile file(fileName,wxFile::read);
	if (file.IsOpened())
 	{

 		GetPlug()->AddLog(LOG_TYPE_MESSAGE,fileName.ToStdString().c_str());
 		file.Read((void*)&m_geneHeader,sizeof(RadarStructure::GeneHeader));

		m_siteAlarm.getAlarmType();
		m_siteAlarm.getAlarmLevel();
		m_siteAlarm.getAlarmCNname();
		m_siteAlarm.getThresholdValue();


		file.Read((void*)&m_siteInfo,sizeof(RadarStructure::SiteInfo));

		m_siteAlarm.getSiteInfoData(m_siteInfo);
		m_siteAlarm.siteInfosCheck();


		m_radarStatusAlarm.getAlarmType();
		m_radarStatusAlarm.getAlarmLevel();
		m_radarStatusAlarm.getAlarmCNname();
		m_radarStatusAlarm.getThresholdValue();

		file.Read((void*)&m_radarStatus,sizeof(RadarStructure::RadarStatus));

		m_radarStatusAlarm.getRadarStatusData(m_radarStatus);
		m_radarStatusAlarm.RadarStatusCheck();


		file.Close();

	}
	else
	{
		file.Close();
		return FALSE;
	}

	return TRUE;
}

bool CRadarParser::writeSiteToDataBase(time_t newTime)
{
	wxString sqlSentence = wxT("INSERT INTO radarConstParameter(updateTime,  \
																			stationNum, \
																			stationName, \
																			latitude, \
																			longitude, \
																			antHeight, \
																			surfaceHeight, \
																			transmitFre,  \
																			HBeamWidth,  \
																			VBeamWidth,  \
																			RDAVersion,  \
																			radarType, \
																			antGain, \
																			Ltx,  \
																			Lrx,  \
																			Lx,  \
																			Atmos, \
																			Le,  \
																			pt) VALUES");

	auto pDataBase = GetPlug()->getDatabase();
	
	if (pDataBase)
	{


			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), newTime));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_siteInfo.SiteCode,wxConvLocal,8)));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_siteInfo.SiteName,wxConvLocal,32)));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.Latitude));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.Longitude));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_siteInfo.AntennaHeight));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_siteInfo.GroundHeight));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.Frequency));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.BeamWidthHori));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.BeamWidthVert));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_siteInfo.RDAVersion));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(getRadarTypeName().c_str())));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.AntennaGain * 0.01));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.TransmittingFeederLoss * 0.01));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.ReceivingFeederLoss * 0.01));
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_siteInfo.OtherLoss * 0.01));
			sqlSentence.Append(wxString::Format(wxT("%f, "), 0.0));
			sqlSentence.Append(wxString::Format(wxT("%f, "), 0.0));
			sqlSentence.Append(wxString::Format(wxT("%f)"), 0.0));


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
			
	}
	return true;
}

bool CRadarParser::writeRadarDataToBase(time_t newTime)
{
	m_healthIndex.getDynRange();
	m_healthIndex.getDynValue();
	m_healthIndex.getNoiseLevelRange();
	m_healthIndex.getSunDegreeValue();

	wxString sqlSentence = wxT("INSERT INTO calibrationparameter (updateTime, \
							   sel_patt, \
							   ant_peak_pwr, \
							   xmtr_peak_pwr, \
							   ant_avg_pwr, \
							   xmtr_avg_pwr, \
							   power_meter_ratio_db, \
							   ant_pwr_mtr_zero, \
							   xmtr_pwr_mtr_zero, \
							   ant_pwr_mtr_zero_v, \
							   rnspln, \
							   ant_peak_pwr_v, \
							   rnlpln, \
							   ant_avg_pwr_v, \
							   te, \
							   pulse_width, \
							   sysstatus, \
							   noise_hchan_1, \
							   noise_hchan_2, \
							   noise_hchan_3, \
							   noise_hchan_4, \
							   noise_vchan_1, \
							   noise_vchan_2, \
							   noise_vchan_3, \
							   noise_vchan_4, \
							   rnoisevchan, \
							   rnoisehchan, \
							   pol, \
							   pdp, \
							   te_v, \
							   zdr, \
							   ctl, \
							   dt_v, \
							   syscal_1, \
							   syscal_2, \
							   syscal_3, \
							   syscal_4, \
							   expected_refl_amp_1, \
							   expected_refl_amp_2, \
							   expected_refl_amp_3, \
							   expected_refl_amp_4, \
							   measured_refl_amp_1, \
							   measured_refl_amp_2, \
							   measured_refl_amp_3, \
							   measured_refl_amp_4, \
							   sysspln, \
							   syslpln, \
							   expected_velocity_1, \
							   expected_velocity_2, \
							   expected_velocity_3, \
							   expected_velocity_4, \
							   measured_velocity_1, \
							   measured_velocity_2, \
							   measured_velocity_3, \
							   measured_velocity_4, \
							   expected_width_1, \
							   expected_width_2, \
							   expected_width_3, \
							   expected_width_4, \
							   measured_width_1, \
							   measured_width_2, \
							   measured_width_3, \
							   measured_width_4, \
							   shelter_temp, \
							   xmtr_air_temp, \
							   radome_temp, \
							   syscal_delta, \
							   expected_rf8_amp_1, \
							   expected_rf8_amp_2, \
							   expected_rf8_amp_3, \
							   shelter_humidity, \
							   xmtr_air_humidity, \
							   radome_humidity, \
							   measured_rf8_amp_1, \
							   measured_rf8_amp_2, \
							   measured_rf8_amp_3, \
							   phase_noise, \
							   phase_noise_v, \
							   unfiltered_v_chan_pwr, \
							   filtered_v_chan_pwr, \
							   unfiltered_lin_chan_pwr, \
							   filtered_lin_chan_pwr,  \
							   noise_coefficient_h, \
							   noise_coefficient_v, \
							   radar_health_score, \
							   transmitterKlystron, \
							   transmitter3A5, \
							   freSynthesizer, \
							   receiverForeend, \
							   receiverBackend, \
							   externalInterference, \
							   refCalibration, \
							   servoPositioning, \
							   antennaBeam, \
							   clockFaultOf96,\
							   ant_az_expected, \
							   ant_az_measured, \
							   ant_el_expected, \
							   ant_el_measured) VALUES ");

	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "),newTime));
			if(std::strlen(m_radarStatus.SEL_PATT) >= 3)
			{
				sqlSentence.Append(wxString::Format(wxT("'%s',"),wxString(m_radarStatus.SEL_PATT,wxConvLocal)));

				m_VCPMode.Append(m_radarStatus.SEL_PATT,8);
			}
			else
			{
				char sel_patt[8] = {0};
				sprintf_s(sel_patt,8,"%d",m_radarStatus.SEL_PATT[0]);
				std::string SEL_PATTStr = "VCP";
				SEL_PATTStr.append(sel_patt);
				sqlSentence.Append(wxString::Format(wxT("'%s',"),wxString(SEL_PATTStr)));
				
				m_VCPMode.Append(wxString(SEL_PATTStr));
			}
			
			if((::wxFinite (m_radarStatus.ANT_PEAK_PWR)) && (std::fabs(m_radarStatus.ANT_PEAK_PWR) <= 9000.0))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_PEAK_PWR));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
		
			if((::wxFinite (m_radarStatus.XMTR_PEAK_PWR)) && (std::fabs(m_radarStatus.XMTR_PEAK_PWR) <= 9000.0))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.XMTR_PEAK_PWR));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if((::wxFinite (m_radarStatus.ANT_AVG_PWR)) && (std::fabs(m_radarStatus.ANT_AVG_PWR) <= 9000.0))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_AVG_PWR)); 
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if((::wxFinite (m_radarStatus.XMTR_AVG_PWR))  && (std::fabs(m_radarStatus.XMTR_AVG_PWR) <= 9000.0))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.XMTR_AVG_PWR));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			if((m_radarStatus.XMTR_PEAK_PWR < 50.0) || (m_radarStatus.ANT_PEAK_PWR < 50.0) || (!::wxFinite(m_radarStatus.POWER_METER_RATIO_DB)) || (GetPlug()->CCExceptionCheck(m_radarStatus.POWER_METER_RATIO_DB)))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.POWER_METER_RATIO_DB));
			}


			if(::wxFinite (m_radarStatus.ANT_PWR_MTR_ZERO))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.ANT_PWR_MTR_ZERO))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_PWR_MTR_ZERO));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if(::wxFinite (m_radarStatus.XMTR_PWR_MTR_ZERO))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.XMTR_PWR_MTR_ZERO))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.XMTR_PWR_MTR_ZERO));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}


// 			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CB")))
// 			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite (m_radarStatus.ANT_PWR_MTR_ZERO_V))
					{
						if(GetPlug()->CCExceptionCheck(m_radarStatus.ANT_PWR_MTR_ZERO_V))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_PWR_MTR_ZERO_V));
						}
						
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
//			}
// 			else
// 			{
// 				if(::wxFinite (m_radarStatus.ANT_PWR_MTR_ZERO_V))
// 				{
// 					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_PWR_MTR_ZERO_V));
// 				}
// 				else
// 				{
// 					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
// 				}
// 			}
				

			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(::wxFinite (m_radarStatus.RNSPLN))
				{
					if(m_radarStatus.RNSPLN > 0.0)
					{
						auto temp = 10.0 * std::log10(m_radarStatus.RNSPLN);
						if(std::fabs(temp) < 1e5)
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), temp));
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
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else 
			{
				if(::wxFinite (m_radarStatus.RNSPLN))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.RNSPLN))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.RNSPLN));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				
			}
					
			
// 			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")))
// 			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if((::wxFinite (m_radarStatus.ANT_PEAK_PWR_V))  && (std::fabs(m_radarStatus.ANT_PEAK_PWR_V) <= 9000.0))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_PEAK_PWR_V));
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
// 			}
// 			else
// 			{
// 				if((::wxFinite (m_radarStatus.ANT_PEAK_PWR_V))  && (std::fabs(m_radarStatus.ANT_PEAK_PWR_V) <= 9000.0))
// 				{
// 					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_PEAK_PWR_V));
// 				}
// 				else
// 				{
// 					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
// 				}
// 			}
				



			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(::wxFinite (m_radarStatus.RNLPLN))
				{
					if(m_radarStatus.RNLPLN > 0.0)
					{ 

						auto temp = 10.0 * std::log10(m_radarStatus.RNLPLN);
						if(std::fabs(temp) < 1e5)
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), temp));
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
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				if(::wxFinite (m_radarStatus.RNLPLN))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.RNLPLN))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.RNLPLN));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}


// 			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")))
// 			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if((::wxFinite (m_radarStatus.ANT_AVG_PWR_V)) && (std::fabs(m_radarStatus.ANT_PEAK_PWR_V) <= 9000.0))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_AVG_PWR_V));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
				}
// 			}
// 			else
// 			{
// 				if((::wxFinite (m_radarStatus.ANT_AVG_PWR_V)) && (std::fabs(m_radarStatus.ANT_PEAK_PWR_V) <= 9000.0))
// 				{
// 					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_AVG_PWR_V));
// 				}
// 				else
// 				{
// 					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
// 				}
// 			}

			if(::wxFinite (m_radarStatus.TE))
			{
				if((m_radarStatus.TE >= 0.0) && (m_radarStatus.TE < 1e3))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.TE));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if(!GetPlug()->CCExceptionCheck(m_radarStatus.PULSE_WIDTH))
			{
				if(GetPlug()->m_radarType == wxT("CC"))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PULSE_WIDTH * 1.0));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PULSE_WIDTH * 0.001));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_radarStatus.SysStatus));


			for(auto index = 0; index <= 3; index++)
			{
				if(::wxFinite (m_radarStatus.NOISE_HCHAN[index]))
				{
					if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
					{
						if(m_radarStatus.NOISE_HCHAN[index] > 0.0)
						{

							auto temp = 10.0 * std::log10(m_radarStatus.NOISE_HCHAN[index]);
							if(std::fabs(temp) < 1e5)
							{
								sqlSentence.Append(wxString::Format(wxT("%f, "), temp));
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
					}
					else 
					{
						if(std::fabs(m_radarStatus.NOISE_HCHAN[index]) > 0)
						{
							if(GetPlug()->CCExceptionCheck(m_radarStatus.NOISE_HCHAN[index]))
							{
								sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
							}
							else
							{
								sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.NOISE_HCHAN[index]));
							}
						}	
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
						}
						
					}
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}

			}

			for(auto index = 0; index <= 3; index++)
			{
					if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
					{
						if(m_radarStatus.POL == BIPOLE)
						{
							if(::wxFinite (m_radarStatus.NOISE_VCHAN[index]))
							{
								if(m_radarStatus.NOISE_VCHAN[index] > 0.0)
								{
									auto temp = 10.0 * std::log10(m_radarStatus.NOISE_VCHAN[index]);
									if(std::fabs(temp) < 1e5)
									{
										sqlSentence.Append(wxString::Format(wxT("%f, "), temp));
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
					}
					else 
					{
						if(m_radarStatus.POL == BIPOLE)
						{
							if(::wxFinite (m_radarStatus.NOISE_VCHAN[index]))
							{
								if(std::fabs(m_radarStatus.NOISE_VCHAN[index]) > 0)
								{
									if(GetPlug()->CCExceptionCheck(m_radarStatus.NOISE_VCHAN[index]))
									{
										sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
									}
									else
									{
										sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.NOISE_VCHAN[index]));
									}
									
								}
								else
								{
									sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
								}
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
					}


			}



			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite (m_radarStatus.RNOISEVCHAN))
					{
						if(m_radarStatus.RNOISEVCHAN > 0.0)
						{
							
							auto temp = 10.0 * std::log10(m_radarStatus.RNOISEVCHAN);
							if(std::fabs(temp) < 1e5)
							{
								sqlSentence.Append(wxString::Format(wxT("%f, "), temp));
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

			}
			else 
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite (m_radarStatus.RNOISEVCHAN))
					{
						if(std::fabs(m_radarStatus.RNOISEVCHAN) > 0)
						{
							if(GetPlug()->CCExceptionCheck(m_radarStatus.RNOISEVCHAN))
							{
								sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
							}
							else
							{
								sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.RNOISEVCHAN));
							}
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
						}
						
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

					
			}

			if(::wxFinite (m_radarStatus.RNOISEHCHAN))
			{
				if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
				{
					if(m_radarStatus.RNOISEHCHAN > 0.0)
					{
						auto temp = 10.0 * std::log10(m_radarStatus.RNOISEHCHAN);
						if(std::fabs(temp) < 1e5)
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), temp));
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
				}
				else 
				{
					if(std::fabs( m_radarStatus.RNOISEHCHAN) > 0)
					{
						if(GetPlug()->CCExceptionCheck( m_radarStatus.RNOISEHCHAN))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.RNOISEHCHAN));
						}
						
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
					}
					
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}


			if(!GetPlug()->CCExceptionCheck(m_radarStatus.POL))
			{
				 sqlSentence.Append(wxString::Format(wxT("%d, "), m_radarStatus.POL));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}


			 if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			 {
				 if(m_radarStatus.POL == BIPOLE)
				 {
					 sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PDP));
				 }
				 else
				 {
					 sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				 }
			 }
			 else
			 {
				 if(GetPlug()->CCExceptionCheck(m_radarStatus.PDP))
				 {
					 sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				 }
				 else
				 {
					 sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PDP));
				 }				
			 }

			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite (m_radarStatus.TE_V))
					{
						if((m_radarStatus.TE_V > 0.0) && (m_radarStatus.TE_V < 1e3))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.TE_V));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
						}
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
			}
			else
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite (m_radarStatus.TE_V))
					{
						if(GetPlug()->CCExceptionCheck(m_radarStatus.TE_V))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.TE_V));
						}
						
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
	
			}


			 if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ZDR));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				}
			}
			else
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.ZDR))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ZDR));
				}
				
			}

			sqlSentence.Append(wxString::Format(wxT("%d, "), m_radarStatus.CTL));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_radarStatus.DT_V));

			for(auto index = 0; index < 4; index++)
			{

				if(::wxFinite (m_radarStatus.SYSCAL[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.SYSCAL[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.SYSCAL[index]));
					}
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}

			}
			
			for(auto index = 0; index < 4; index++)
			{
				if(::wxFinite (m_radarStatus.EXPECTED_REFL_AMP[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.EXPECTED_REFL_AMP[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.EXPECTED_REFL_AMP[index]));
					}
					}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}

			for(auto index = 0; index < 4; index++)
			{
				if(::wxFinite (m_radarStatus.MEASURED_REFL_AMP[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.MEASURED_REFL_AMP[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.MEASURED_REFL_AMP[index]));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
		

			if(::wxFinite (m_radarStatus.SYSSPLN))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.SYSSPLN))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.SYSSPLN));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}


			if(::wxFinite (m_radarStatus.SYSLPLN))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.SYSLPLN))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.SYSLPLN));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			

			for(auto index = 0; index < 4; index++)
			{
				if(::wxFinite (m_radarStatus.EXPECTED_VELOCITY[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.EXPECTED_VELOCITY[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.EXPECTED_VELOCITY[index]));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}

			for(auto index = 0; index < 4; index++)
			{
				if(::wxFinite (m_radarStatus.MEASURED_VELOCITY[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.MEASURED_VELOCITY[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.MEASURED_VELOCITY[index]));
					}
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}

			for(auto index = 0; index < 4; index++)
			{
				if(::wxFinite (m_radarStatus.EXPECTED_WIDTH[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.EXPECTED_WIDTH[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.EXPECTED_WIDTH[index]));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
	

			for(auto index = 0; index < 4; index++)
			{
				if(::wxFinite (m_radarStatus.MEASURED_WIDTH[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.MEASURED_WIDTH[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.MEASURED_WIDTH[index]));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
	
			if(::wxFinite (m_radarStatus.SHELTER_TEMP))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.SHELTER_TEMP))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.SHELTER_TEMP));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
 		
			if(::wxFinite (m_radarStatus.XMTR_AIR_TEMP))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.XMTR_AIR_TEMP))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.XMTR_AIR_TEMP));
				}
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			if(::wxFinite (m_radarStatus.RADOME_TEMP))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.RADOME_TEMP))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.RADOME_TEMP));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
		
			if(::wxFinite (m_radarStatus.SYSCAL_DELTA))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.SYSCAL_DELTA))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.SYSCAL_DELTA));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			for(auto index = 0; index < 3; index++)
			{
				if(::wxFinite (m_radarStatus.EXPECTED_RF8_AMP[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.EXPECTED_RF8_AMP[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));				
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.EXPECTED_RF8_AMP[index]));
					}
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}
	
			if(::wxFinite (m_radarStatus.SHELTER_HUMIDITY))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.SHELTER_HUMIDITY))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.SHELTER_HUMIDITY));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if(::wxFinite (m_radarStatus.XMTR_AIR_HUMIDITY))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.XMTR_AIR_HUMIDITY))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.XMTR_AIR_HUMIDITY));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if(::wxFinite (m_radarStatus.RADOME_HUMIDITY))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.RADOME_HUMIDITY))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.RADOME_HUMIDITY));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			for(auto index = 0; index < 3; index++)
			{
				if(::wxFinite (m_radarStatus.MEASURED_RF8_AMP[index]))
				{
					if(GetPlug()->CCExceptionCheck(m_radarStatus.MEASURED_RF8_AMP[index]))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.MEASURED_RF8_AMP[index]));
					}
					
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
			}

			if(::wxFinite (m_radarStatus.PHASE_NOISE))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.PHASE_NOISE))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PHASE_NOISE));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite( m_radarStatus.PHASE_NOISE_V))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PHASE_NOISE_V));	
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
			}
			else
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite( m_radarStatus.PHASE_NOISE_V))
					{
						if(GetPlug()->CCExceptionCheck(m_radarStatus.PHASE_NOISE_V))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.PHASE_NOISE_V));	
						}
						
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

			}


			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite( m_radarStatus.UNFILTERED_V_CHAN_PWR))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.UNFILTERED_V_CHAN_PWR));
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
			}
			else
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite( m_radarStatus.UNFILTERED_V_CHAN_PWR))
					{
						if(GetPlug()->CCExceptionCheck(m_radarStatus.UNFILTERED_V_CHAN_PWR))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.UNFILTERED_V_CHAN_PWR));
						}
						
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

			}



			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite( m_radarStatus.FILTERED_V_CHAN_PWR))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.FILTERED_V_CHAN_PWR));
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
			}
			else
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if(::wxFinite( m_radarStatus.FILTERED_V_CHAN_PWR))
					{
						if(GetPlug()->CCExceptionCheck(m_radarStatus.FILTERED_V_CHAN_PWR))
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
						}
						else
						{
							sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.FILTERED_V_CHAN_PWR));
						}
						
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

			}

			
			if(::wxFinite (m_radarStatus.UNFILTERED_LIN_CHAN_PWR))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.UNFILTERED_LIN_CHAN_PWR))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.UNFILTERED_LIN_CHAN_PWR));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			
			if(::wxFinite (m_radarStatus.FILTERED_LIN_CHAN_PWR))
			{
				if(GetPlug()->CCExceptionCheck(m_radarStatus.FILTERED_LIN_CHAN_PWR))
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.FILTERED_LIN_CHAN_PWR));
				}
				
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}


			if((m_radarStatus.TE >= 0.0) && (m_radarStatus.TE < 1e3))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), 10 * std::log10(m_radarStatus.TE / 290.0 + 1)));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "),EXCEPTIONDATA));
			}
	



			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if((m_radarStatus.TE_V > 0.0) && (m_radarStatus.TE_V < 1e3))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), 10 * std::log10(m_radarStatus.TE_V / 290.0 + 1)));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
					}

				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				}
			}
			else
			{
				if(m_radarStatus.POL == BIPOLE)
				{
					if((m_radarStatus.TE_V > 0.0) && (m_radarStatus.TE_V < 1e3))
					{
						sqlSentence.Append(wxString::Format(wxT("%f, "), 10 * std::log10(m_radarStatus.TE_V / 290.0 + 1)));
					}
					else
					{
						sqlSentence.Append(wxString::Format(wxT("%s, "),wxT("NULL")));
					}
				}
				else
				{
					sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				}

			}

		}	
	
			sqlSentence.Append(wxString::Format(wxT("%f, "), m_healthIndex.calHealthIndex(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calEmitterTubeFault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calEmitter3A5Fault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calPinzhongFault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calReceiverADFrontFault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calReceiveADEndFault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calExternalFault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calRefOnlineFault(m_radarStatus)));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calServoPosFault()));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.calAntennaBeamFault()));
			sqlSentence.Append(wxString::Format(wxT("%d, "),(INT)m_healthIndex.cal96ClockFault()));

			if(::wxFinite (m_radarStatus.ANT_AZ_EXPECTED))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_AZ_EXPECTED));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}
			if(::wxFinite (m_radarStatus.ANT_AZ_MEASURED))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_AZ_MEASURED));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			if(::wxFinite (m_radarStatus.ANT_EL_EXPECTED))
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), m_radarStatus.ANT_EL_EXPECTED));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f, "), EXCEPTIONDATA));
			}

			if(::wxFinite (m_radarStatus.ANT_EL_MEASURED))
			{
				sqlSentence.Append(wxString::Format(wxT("%f)"), m_radarStatus.ANT_EL_MEASURED));
			}
			else
			{
				sqlSentence.Append(wxString::Format(wxT("%f)"), EXCEPTIONDATA));
			}
			//double NoiseT = (pow(10.0, NoiseF / 10.0)) * 290.0;

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
		

		return true;
}


std::string CRadarParser::getRadarTypeName()
{
	switch (m_siteInfo.RadarType)
	{
	case 1:
		return "SA";
	case 2:
		return "SB";
	case 3:
		return "SC";
	case 4:
		return "SAD";
	case 33:
		return "CA";
	case 34:
		return "CB";
	case 35:
		return "CC";
	case 36:
		return "CCJ";
	case 37:
		return "CD";
	case 38:
		return "CAD";
	case 65:
		return "XA";
	case 66:
		return "XAD";

	default:
		break;
	}
	return std::string("Unknown");
}



void CRadarParser::updateTime(time_t newTime)
{
	m_updateTime = newTime;
	GetPlug()->setradarStatustime(newTime);
}


void CRadarParser::startRadarFileDetection()  //发送文件缺失检测
{
	wxCommandEvent detectionEvent(RADARFILE_EVENT);
	wxString interval; 

	if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
	{
		if(m_VCPMode.Find(wxT("VCP11")) != wxNOT_FOUND)  //根据不同雷达类型和扫面模式，设置时间间隔，超过时间间隔没有新文件则报警
		{
			interval = wxT("12"); 
		}
		else if(m_VCPMode.Find(wxT("VCP21")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else if(m_VCPMode.Find(wxT("VCP31")) != wxNOT_FOUND)
		{
			interval = wxT("20");
		}
		else if(m_VCPMode.Find(wxT("VCP41")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else
		{
			interval = wxT("12");
		}
	}
	else if((GetPlug()->m_radarType == wxT("SC")) || (GetPlug()->m_radarType == wxT("CD")) )
	{
		if(m_VCPMode.Find(wxT("VCP11")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else if(m_VCPMode.Find(wxT("VCP21")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else if(m_VCPMode.Find(wxT("VCP31")) != wxNOT_FOUND)
		{
			interval = wxT("20");
		}
		else if(m_VCPMode.Find(wxT("VCP41")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else
		{
			interval = wxT("12");
		}
	}
	else if(GetPlug()->m_radarType == wxT("CC"))
	{
		if(m_VCPMode.Find(wxT("VCP11")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else if(m_VCPMode.Find(wxT("VCP21")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else if(m_VCPMode.Find(wxT("VCP31")) != wxNOT_FOUND)
		{
			interval = wxT("20");
		}
		else if(m_VCPMode.Find(wxT("VCP41")) != wxNOT_FOUND)
		{
			interval = wxT("12");
		}
		else
		{
			interval = wxT("12");
		}
	}
	else{}



	detectionEvent.SetString(interval);
	::wxQueueEvent(GetPlug(),detectionEvent.Clone());  //发送自定义事件


}

wxString CRadarParser::getLatestFileName(time_t* newTime)
{
	wxString dirName(GetPlug()->getfileConfig().radarStatusDir);

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
		GetPlug()->AddLog(LOG_TYPE_ERROR,"雷达67项关键参数文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false; 
		bool flag = fileDir.GetFirst(&fileName, /*wxT("*_RadarStatus_*")*/wxT("Z_R_DWRN_SRSI*"),wxDIR_FILES );  //遍历文件夹
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
		else
		{	

			if(::wxDirExists(GetPlug()->m_uploadPath))
			{
				auto copyFilePath = GetPlug()->m_uploadPath + wxT("\\") + fileName;  //将新文件复制到FTP上传文件夹中
				::wxCopyFile(fullFilePath,copyFilePath);
			}

		}
	}

	return fullFilePath;
}
