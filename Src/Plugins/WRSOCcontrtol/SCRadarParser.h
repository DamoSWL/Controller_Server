#pragma once

#include "radarStructure.h"
#include "SCAlarm.h"


//该类负责SC等雷达在线监测点 适配参数的解析
class CSCRadarParser
{
public:
	CSCRadarParser();
	~CSCRadarParser();

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
	time_t m_updateTime ;
	RadarStructure::SCStructure m_SCRadarData;
	RadarStructure::SCAdaptParam m_SCAdaptParam;
	CSCAlarm m_SCAlarm;
};

