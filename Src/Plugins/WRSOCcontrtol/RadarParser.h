#pragma once


#include "radarStructure.h"
#include "SiteAlarm.h"
#include "RadarStatusAlarm.h"
#include "HealthIndex.h"


//该类负责雷达关键参数解析
wxDECLARE_EVENT(RADARFILE_EVENT, wxCommandEvent);

class CRadarParser 
{
public:
	CRadarParser();
	~CRadarParser();

	static DWORD WINAPI exec(LPVOID para); //线程入口函数，在函数内部检测新的文件并解析

public:
	wxString getLatestFileName(time_t* newTime); //获取最新的雷达参数文件

	BOOL loadFile(time_t* newTime);  //读取最新文件的文件内容
	bool writeSiteToDataBase(time_t newTime);
	bool writeRadarDataToBase(time_t newTime);
	std::string getRadarTypeName();  
	void setUpdateTime(time_t time) {m_updateTime = time;}
	void updateTime(time_t newTime);
	void startRadarFileDetection();  //启动雷达文件缺失监测


private:
	CSiteAlarm m_siteAlarm;  //雷达站地址数据报警
	CRadarStatusAlarm m_radarStatusAlarm;
	CHealthIndex m_healthIndex;  //健康指数类

	time_t m_updateTime;
	RadarStructure::GeneHeader m_geneHeader;
	RadarStructure::SiteInfo m_siteInfo;
	RadarStructure::RadarStatus m_radarStatus;

	wxString m_VCPMode;
	


	

};

