#pragma once

#include "radarStructure.h"

//���ฺ��CC���״����߼��� �����������
class CCCRadarParser
{
public:
	CCCRadarParser(void);
	~CCCRadarParser(void);

public:
	static DWORD WINAPI exec(LPVOID para);

	wxString getLatestFileName(time_t* newTime);
	BOOL loadFile(time_t* newTime);
	BOOL writeToDataBase(time_t newTime);
	BOOL writeAdatpParamToDateBase(time_t newTime);
	void setUpdateTime(time_t time) {m_updateTime = time;}
	void updateTime(time_t newTime);

private:
	std::string getPwrBit();
	std::string getRcvStr();
	std::string getSvoStr();
	std::string getXmtStr();

private:	
	time_t m_updateTime ;
	RadarStructure::CCStructure m_CCRadarData;
	RadarStructure::CCAdaptParam m_CCAdapatParam;

};

