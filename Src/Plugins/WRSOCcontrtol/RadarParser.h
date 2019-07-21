#pragma once


#include "radarStructure.h"
#include "SiteAlarm.h"
#include "RadarStatusAlarm.h"
#include "HealthIndex.h"


//���ฺ���״�ؼ���������
wxDECLARE_EVENT(RADARFILE_EVENT, wxCommandEvent);

class CRadarParser 
{
public:
	CRadarParser();
	~CRadarParser();

	static DWORD WINAPI exec(LPVOID para); //�߳���ں������ں����ڲ�����µ��ļ�������

public:
	wxString getLatestFileName(time_t* newTime); //��ȡ���µ��״�����ļ�

	BOOL loadFile(time_t* newTime);  //��ȡ�����ļ����ļ�����
	bool writeSiteToDataBase(time_t newTime);
	bool writeRadarDataToBase(time_t newTime);
	std::string getRadarTypeName();  
	void setUpdateTime(time_t time) {m_updateTime = time;}
	void updateTime(time_t newTime);
	void startRadarFileDetection();  //�����״��ļ�ȱʧ���


private:
	CSiteAlarm m_siteAlarm;  //�״�վ��ַ���ݱ���
	CRadarStatusAlarm m_radarStatusAlarm;
	CHealthIndex m_healthIndex;  //����ָ����

	time_t m_updateTime;
	RadarStructure::GeneHeader m_geneHeader;
	RadarStructure::SiteInfo m_siteInfo;
	RadarStructure::RadarStatus m_radarStatus;

	wxString m_VCPMode;
	


	

};

