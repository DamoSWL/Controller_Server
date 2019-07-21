#pragma once


#include "ITXTParse.h"
#include <ctime>

//该类负责解析太阳法文件
class CSunResultParse : public ITXTParse
{
public:
	CSunResultParse();
	virtual ~CSunResultParse();

	virtual INT loadTXTFile(time_t* time) override;
	virtual void parseFile() override;
	virtual bool writeToDataBase(time_t time) override;
	virtual bool checkFileTime(time_t* time) override;
	virtual void setUpdateTime(time_t time) override {m_updateTime = time;}
	virtual void updateTime(time_t time);

private:
	wxString getLatestFileName(time_t* time);
	time_t getDateTime(const std::string& dateTimeStr);
	time_t getDateTime2(const std::string& dateStr,const std::string& timeStr);

private:
	std::string m_contents;
	FLOAT m_oldAzimuthConnectionFactor;
	FLOAT m_newAzimuthConnectionFactor;
	FLOAT m_peakPowrFromAzimuthScan;
	FLOAT m_oldElevationConnectionFactor;
	FLOAT m_newElevationConnectionFactor;
	FLOAT m_peakPowerFromElevationScan;
	FLOAT m_meanReceiverNoise;
	FLOAT m_beamWidth;
	FLOAT m_beamWidth_V;
	
	BOOL m_meanReceiverNoiseFlag;
	BOOL m_beamWidth_VFlag;

	std::time_t m_updateTime;
	std::time_t m_tmpUpdateTime;
	std::time_t m_produceTime;
	
	

};

