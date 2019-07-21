#include "HealthIndex.h"
#include <cmath>
#include "mysqldatabase.h"



CHealthIndex::CHealthIndex(void): m_emissionMax(0.0),
	m_emissionMin(0.0),
	m_noiseCofMax(4.0),
	m_noiseCofStd(3.0),
	m_phaseNoiseMax(0.0),
	m_phaseNoiseStd(0.0),
	m_syscalMax(3.0),
	m_syscalStd(2.0),
	m_dynUpperBound(0.0),
	m_dynLowerBound(0.0),
	m_dynValue(0.0),
	m_noiseLevelUpperBound(0.0),
	m_noiseLevelLowerBound(0.0),
	m_sunAzimuth(0.0),
	m_sunElevation(0.0)
{
}


CHealthIndex::~CHealthIndex(void)
{
}

float CHealthIndex::calHealthIndex(const RadarStructure::RadarStatus& radarData)  //计算健康指数
{
	m_radarType = GetPlug()->getRadarTypeStr();
	WCHAR sType = L'S';
	WCHAR cType = L'C';
	if(m_radarType.Left(1) == wxT("S"))
	{
		m_emissionMax = 650.0;
		m_emissionMin = 450.0;
		
		m_phaseNoiseMax = (float)0.2;
		m_phaseNoiseStd = (float)0.1;
	}
	else if(m_radarType.Left(1) == wxT("C"))
	{
		m_emissionMax = 250.0;
		m_emissionMin = 200.0;

		m_phaseNoiseMax = (float)0.3;
		m_phaseNoiseStd = (float)0.15;
	}
	else
	{
		return 0.0;
	}

	float S1 = 0.0;
	float S2 = 0.0;
	float S3 = 0.0;
	float S4 = 0.0;

	if((::wxFinite (radarData.XMTR_PEAK_PWR)) && (std::fabs(radarData.XMTR_PEAK_PWR) <= 9000.0))
	{
		if(radarData.XMTR_PEAK_PWR >= m_emissionMax)
		{
			S1 = 25.0;
		}
		else if(radarData.XMTR_PEAK_PWR < m_emissionMin)
		{
			S1 = 0.0;
		}
		else 
		{
			S1 = 25.0 * radarData.XMTR_PEAK_PWR / m_emissionMax;
		}
	}
	else
	{
		S1 = 0.0;
	}


	auto noiseCof = 0.0;
	if((::wxFinite (radarData.TE)) && (radarData.TE > 0) && (radarData.TE < 1e3))
	{
		noiseCof = 10.0 * std::log10(radarData.TE / 290.0 + 1);
	}
	else
	{
		noiseCof = -1;
	}
	if(noiseCof > 0)
	{
		if(noiseCof <= m_noiseCofStd)
		{
			S2 = 25.0;
		}
		else if(noiseCof > m_noiseCofMax)
		{
			S2 = 0.0;
		}
		else 
		{
			S2 = (1.0 - (noiseCof - m_noiseCofStd)/ m_noiseCofStd) * 25.0;
		}
	}
	else
	{
		S2 = 0.0;
	}

	if(::wxFinite (radarData.PHASE_NOISE))
	{
		if(radarData.PHASE_NOISE <= m_phaseNoiseStd)
		{
			S3 = 25.0;
		}
		else if(radarData.PHASE_NOISE > m_phaseNoiseMax)
		{
			S3 = 0.0;
		}
		else
		{
			S3 = (1.0 - (radarData.PHASE_NOISE - m_phaseNoiseStd) / m_phaseNoiseStd) * 25.0;
		}
	}
	else
	{
		S3 = 0.0;
	}

	

	if(::wxFinite (radarData.SYSCAL_DELTA))
	{
		if(radarData.SYSCAL_DELTA <= m_syscalStd)
		{
			S4 = 25.0;
		}
		else if(radarData.SYSCAL_DELTA > m_syscalMax)
		{
			S4 = 0.0;
		}
		else
		{
			S4 = (1.0 - (radarData.SYSCAL_DELTA - m_syscalStd) / m_syscalStd) * 25.0;
		}
	}
	else
	{
		S4 = 0.0;
	}
	

	auto message = wxString::Format(wxT("S1=%f, S2=%f, S3=%f, S4=%f, Score=%f"),S1,S2,S3,S4,S1 + S2 + S3 + S4);
	GetPlug()->AddLog(LOG_TYPE_MESSAGE,message.mb_str(wxConvLocal));

	return S1 + S2 + S3 + S4;

}

BOOL CHealthIndex::calEmitterTubeFault(const RadarStructure::RadarStatus& radarData)
{
	const float KDCONST = 2.0;
	BOOL KDCheckFlag = calKDAmp(radarData,KDCONST);
	BOOL phaseNoiseCheckFlag = calPhaseNoise(radarData);
	BOOL EmitterPowerCheckFlag = calEmitterPower(radarData);

	return KDCheckFlag && phaseNoiseCheckFlag && EmitterPowerCheckFlag;
	
}

BOOL CHealthIndex::calEmitter3A5Fault(const RadarStructure::RadarStatus& radarData)
{
	const float RFDCONST = 3.0;
	const float DELTACONST = 3.0;
	const float KDCONST = 3.0;

	BOOL RFDCheckFlag = calRFDSignal(radarData,RFDCONST);
	BOOL DeltaCheckFlag = calSyscalDelta(radarData,DELTACONST);
	BOOL EmitterPowerCheckFlag = calEmitterPower(radarData);
	BOOL KDCheckFlag = calKDAmp(radarData,KDCONST);
	BOOL phaseNoiseCheckFlag = calPhaseNoise(radarData);

	return RFDCheckFlag && DeltaCheckFlag && EmitterPowerCheckFlag && KDCheckFlag && phaseNoiseCheckFlag;
	//return EmitterPowerCheckFlag;

}

BOOL CHealthIndex::calPinzhongFault(const RadarStructure::RadarStatus& radarData)
{
	const float CWCONST = 3.0;
	const float DELTACONST = 3.0;
	const float RFDCONST = 3.0;
	const float KDCONST = 3.0;

	BOOL CWCheckFlag = calCWSignal(radarData,CWCONST);
	BOOL DeltaCheckFlag = calSyscalDelta(radarData,DELTACONST);
	BOOL RFDCheckFlag = calRFDSignal(radarData,RFDCONST);
	BOOL EmitterPowerCheckFlag = calEmitterPower(radarData);
	BOOL KDCheckFlag = calKDAmp(radarData,KDCONST);
	BOOL phaseNoiseCheckFlag = calPhaseNoise(radarData);

	return CWCheckFlag && DeltaCheckFlag && RFDCheckFlag && EmitterPowerCheckFlag && KDCheckFlag && phaseNoiseCheckFlag;
	
}

BOOL CHealthIndex::calReceiverADFrontFault(const RadarStructure::RadarStatus& radarData)
{
	const float DELTACONST = 3.0;
	const float NOISECONST = 3.0;

	BOOL DeltaCheckFlag = calSyscalDelta(radarData,DELTACONST);
	BOOL noiseCheckFlag = calNoiseCoefficient(radarData,NOISECONST);
	BOOL receiverCheckFlag = calDynRange();

	return DeltaCheckFlag && noiseCheckFlag && receiverCheckFlag;
	
}

BOOL CHealthIndex::calReceiveADEndFault(const RadarStructure::RadarStatus& radarData)
{
	const float DELTACONST = 3.0;
	const float NOISECONST = 3.0;

	BOOL DeltaCheckFlag = calSyscalDelta(radarData,DELTACONST);
	BOOL noiseCofCheckFlag = calNoiseCoefficient(radarData,NOISECONST);	
	BOOL receiverCheckFlag = calDynRange();
	BOOL noiseLevelCheckFlag = calNoiseLevel(radarData);

	return DeltaCheckFlag && noiseCofCheckFlag && receiverCheckFlag && noiseLevelCheckFlag;

}

BOOL CHealthIndex::calExternalFault(const RadarStructure::RadarStatus& radarData)
{
	const float NOISELEVELCONST = 3.0;
	BOOL noiseLevelCheckFlag = calNoiseLevel(radarData,NOISELEVELCONST);

	return noiseLevelCheckFlag;

}

BOOL CHealthIndex::calRefOnlineFault(const RadarStructure::RadarStatus& radarData)
{
	const float DELTACONST = 2.0;
	BOOL DeltaCheckFlag = calSyscalDelta(radarData,DELTACONST);
	BOOL emitterPowerCheckFlag;
	if(std::fabs(radarData.XMTR_PEAK_PWR) < 50)
	{
		emitterPowerCheckFlag = TRUE;
	}
	else
	{
		emitterPowerCheckFlag = FALSE;
	}

	return DeltaCheckFlag && emitterPowerCheckFlag;

}

BOOL CHealthIndex::calServoPosFault()
{
	return FALSE;
}

BOOL CHealthIndex::calAntennaBeamFault()
{
	const float SUNCONST = 0.3f;
	if((m_sunAzimuth > SUNCONST) || (m_sunElevation > SUNCONST))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CHealthIndex::cal96ClockFault()
{
	return FALSE;
}

BOOL CHealthIndex::calPhaseNoise(const RadarStructure::RadarStatus& radarData)
{
	if(radarData.PHASE_NOISE > m_phaseNoiseStd)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CHealthIndex::calEmitterPower(const RadarStructure::RadarStatus& radarData)
{
	if((radarData.XMTR_PEAK_PWR < m_emissionMin) || (radarData.XMTR_PEAK_PWR > m_emissionMax))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CHealthIndex::calRFDSignal(const RadarStructure::RadarStatus& radarData,float standard)
{
	for(auto index = 1; index < 4; index++)
	{
		if(std::fabs(radarData.MEASURED_REFL_AMP[index] - radarData.EXPECTED_REFL_AMP[index]) > standard)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CHealthIndex::calSyscalDelta(const RadarStructure::RadarStatus& radarData,float standard)
{
	if(radarData.SYSCAL_DELTA > standard)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CHealthIndex::calCWSignal(const RadarStructure::RadarStatus& radarData,float standard)
{
	if(std::fabs(radarData.MEASURED_REFL_AMP[0] - radarData.EXPECTED_REFL_AMP[0]) > standard)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CHealthIndex::calDynRange()
{
	if((m_dynValue > m_dynUpperBound) && (m_dynValue < m_dynLowerBound))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CHealthIndex::calNoiseCoefficient(const RadarStructure::RadarStatus& radarData,float standard)
{
	auto noiseCof = 0.0;
	if((radarData.TE > 0) && (radarData.TE < 1e3))
	{
		noiseCof = 10.0 * std::log10(radarData.TE / 290.0 + 1);
	}
	else
	{
		noiseCof = EXCEPTIONDATA;
	}
	if(noiseCof > standard)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CHealthIndex::calNoiseLevel(const RadarStructure::RadarStatus& radarData)
{
	if((radarData.RNOISEVCHAN > m_noiseLevelUpperBound) && (radarData.RNOISEVCHAN < m_noiseLevelLowerBound))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CHealthIndex::calNoiseLevel(const RadarStructure::RadarStatus& radarData,float standard)
{
	if(radarData.RNOISEVCHAN > standard)
	{
		return TRUE;
	}
	return FALSE;
}

int CHealthIndex::cbSQLGetDynLowerRange(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_dynLowerBound = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CHealthIndex::cbSQLGetDynUpperRange(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_dynUpperBound = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CHealthIndex::cbSQLGetDynValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_dynValue = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CHealthIndex::cbSQLGetLowerNoise(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_noiseLevelLowerBound = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CHealthIndex::cbSQLGetUpperNoise(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_noiseLevelUpperBound = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CHealthIndex::cbSQLGetSunAzimuth(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_sunAzimuth = std::stof(std::string(argv[0]));
	}

	return 0;
}

int CHealthIndex::cbSQLGetSunElevation(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CHealthIndex *pT = static_cast<CHealthIndex*>(pIn);
	if(pT)
	{
		pT->m_sunElevation = std::stof(std::string(argv[0]));
	}

	return 0;
}

void CHealthIndex::getDynRange()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			char queryStr[256] = {'\0'};
			sprintf_s(queryStr,256,"SELECT lowerLimit FROM thresholdparameter WHERE eName = 'dynamicRange'");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetDynLowerRange,wxT("lowerLimit"),this);
			
			sprintf_s(queryStr,256,"SELECT upperLimit FROM thresholdparameter WHERE eName = 'dynamicRange'");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetDynUpperRange,wxT("upperLimit"),this);
		
		// 	if(resultNum != 0)
		// 	{
		// 		for(int index = 0; index < resultNum; index++)
		// 		{
		// 			m_dynUpperBound = pDataBase->GetFloatValue(index,"upperLimit");
		// 			m_dynLowerBound = pDataBase->GetFloatValue(index,"lowerLimit");
		// 		}
		// 	}
			
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}


}

BOOL CHealthIndex::getDynValue()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			char queryStr[256] = {'\0'};
			sprintf_s(queryStr,256,"SELECT slope FROM dyntestinfo ORDER BY updateTime desc LIMIT 1");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetDynValue,wxT("slope"),this);
			
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		m_dynValue = pDataBase->GetFloatValue(index,"dynamicRange");
			// 	}
			// 	return TRUE;

			// }
			
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}

	return FALSE;
}

void CHealthIndex::getNoiseLevelRange()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			char queryStr[128] = {'\0'};
			sprintf_s(queryStr,128,"SELECT lowerLimit FROM thresholdparameter WHERE eName = 'rnoisehchan'");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetLowerNoise,wxT("lowerLimit"),this);

			sprintf_s(queryStr,128,"SELECT upperLimit FROM thresholdparameter WHERE eName = 'rnoisehchan'");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetUpperNoise,wxT("upperLimit"),this);

			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		m_noiseLevelUpperBound = pDataBase->GetFloatValue(index,"upperLimit");
			// 		m_noiseLevelLowerBound = pDataBase->GetFloatValue(index,"lowerLimit");
			// 	}
			// }

		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
}

BOOL CHealthIndex::getSunDegreeValue()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			char queryStr[128] = {'\0'};
			sprintf_s(queryStr,128,"SELECT newACF FROM sunresult ORDER BY updateTime desc LIMIT 1");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetSunAzimuth,wxT("newACF"),this);

			sprintf_s(queryStr,128,"SELECT  newECF FROM sunresult ORDER BY updateTime desc LIMIT 1");
			pDataBase->RunSQLQuery(queryStr,cbSQLGetSunElevation,wxT("newECF"),this);
			
		// 	if(resultNum != 0)
		// 	{
		// 		for(int index = 0; index < resultNum; index++)
		// 		{
		// 			m_sunAzimuth = pDataBase->GetFloatValue(index,"newACF");
		// 			m_sunElevation = pDataBase->GetFloatValue(index,"newECF");
		// 		}
		// 		return TRUE;

		// 	}
			
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}

	return FALSE;
}

BOOL CHealthIndex::calKDAmp(const RadarStructure::RadarStatus& radarData,float standard)
{
	for(auto index = 0; index < 3; index++)
	{
		
		if(std::fabs(radarData.MEASURED_RF8_AMP[index]  - radarData.EXPECTED_RF8_AMP[index]) >= standard)
		{
			return TRUE;
		}

	}

	return FALSE;
}
