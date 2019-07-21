#pragma once


#include "radarStructure.h"
#include "CDAlarm.h"

//���ฺ��CD�ͺ� ���߼��� �״������������
class CCDRadarParser
{
public:
	CCDRadarParser(void);
	~CCDRadarParser(void);

public:
	static DWORD WINAPI exec(LPVOID para);

	wxString getLatestFileName(time_t* newTime);
	BOOL loadFile(time_t* newTime);
	BOOL writeToDataBase(time_t newTime);
	BOOL writeAdatpParamToDateBase(time_t newTime);
	void setUpdateTime(time_t time) {m_updateTime = time;}
	void updateTime(time_t newTime);

private:
	std::string getWholeBit();
	std::string getPwrBit();
	std::string getRcvStr();
	std::string getSvoStr();
	std::string getXmtStr();

	wxString getAdaptParamStr(const std::vector<FLOAT>& adaptParam);

private:
	time_t m_updateTime;
	RadarStructure::CDStructure m_CDRadarData;
	RadarStructure::SCAdaptParam m_CDAdaptParam;
	CCDAlarm m_CDAlarm;

};

