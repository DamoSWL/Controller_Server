
#pragma once

#include "radarStructure.h"


//该类负责计算健康指数
class CHealthIndex
{
public:
	CHealthIndex(void);
	~CHealthIndex(void);


public:
	float calHealthIndex(const RadarStructure::RadarStatus& radarData);
	BOOL calEmitterTubeFault(const RadarStructure::RadarStatus& radarData);
	BOOL calEmitter3A5Fault(const RadarStructure::RadarStatus& radarData);
	BOOL calPinzhongFault(const RadarStructure::RadarStatus& radarData);
	BOOL calReceiverADFrontFault(const RadarStructure::RadarStatus& radarData);
	BOOL calReceiveADEndFault(const RadarStructure::RadarStatus& radarData);
	BOOL calExternalFault(const RadarStructure::RadarStatus& radarData);
	BOOL calRefOnlineFault(const RadarStructure::RadarStatus& radarData);
	BOOL calServoPosFault();
	BOOL calAntennaBeamFault();
	BOOL cal96ClockFault();

	void getDynRange();
	BOOL getDynValue();
	void getNoiseLevelRange();
	BOOL getSunDegreeValue();

private:
	BOOL calKDAmp(const RadarStructure::RadarStatus& radarData,float standard);
	BOOL calPhaseNoise(const RadarStructure::RadarStatus& radarData);
	BOOL calEmitterPower(const RadarStructure::RadarStatus& radarData);
	BOOL calRFDSignal(const RadarStructure::RadarStatus& radarData,float standard);
	BOOL calSyscalDelta(const RadarStructure::RadarStatus& radarData,float standard);
	BOOL calCWSignal(const RadarStructure::RadarStatus& radarData,float standard);
	BOOL calDynRange();
	BOOL calNoiseCoefficient(const RadarStructure::RadarStatus& radarData,float standard);
	BOOL calNoiseLevel(const RadarStructure::RadarStatus& radarData);
	BOOL calNoiseLevel(const RadarStructure::RadarStatus& radarData,float standard);

private:
	static int cbSQLGetDynLowerRange( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetDynUpperRange( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetDynValue( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetLowerNoise( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetUpperNoise( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetSunAzimuth( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetSunElevation( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );


public:
	float m_dynUpperBound;
	float m_dynLowerBound;

	float m_dynValue;

	float m_noiseLevelUpperBound;
	float m_noiseLevelLowerBound;

	float m_sunAzimuth;
	float m_sunElevation;

private:
	wxString m_radarType;
	
	float m_emissionMax; //发射功率上下限
	float m_emissionMin;

	float m_noiseCofMax; //噪音系数上下限
	float m_noiseCofStd;

	float m_phaseNoiseMax;  //相位噪音上下限
	float m_phaseNoiseStd;

	float m_syscalMax;  
	float m_syscalStd;




};

