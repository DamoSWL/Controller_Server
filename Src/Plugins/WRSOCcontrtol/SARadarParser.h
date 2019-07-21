#pragma once

#include "radarStructure.h"
#include "SAAlarm.h"


//该类负责SA等雷达在线监测点 适配参数等解析
class CSARadarParser
{
public:
	CSARadarParser(void);
	~CSARadarParser(void);

public:
	static DWORD WINAPI exec(LPVOID para);

	wxString getLatestFileName(time_t* newTime);
	
	BOOL loadFile(time_t* newTime);
	BOOL writeToDataBase(time_t newTime);
	void setUpdateTime(time_t time) {m_updateTime = time;}
	void updateTime(time_t newTime);
	
	std::string getSAXMTBitStr();
	std::string getSARCVBitStr();
	std::string getSASVBitStr();


private:
	time_t m_updateTime;
	RadarStructure::SAStructrue m_SARadarData;
	CSAAlarm m_SAAlarm;

	UINT m_polarType;


};

