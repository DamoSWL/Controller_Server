#pragma once
#include "ITXTParse.h"
#include <ctime>
#include <Windows.h>

//���ฺ�������λ�����ļ�
class CPhaseNoiseResult : public ITXTParse
{
public:
	CPhaseNoiseResult();
	virtual ~CPhaseNoiseResult();
public:
	virtual INT loadTXTFile(time_t* time) override;
	virtual void parseFile() override;
	virtual bool checkFileTime(time_t* time) override;
	virtual bool writeToDataBase(time_t time) override;
	virtual void setUpdateTime(time_t time) override {m_updateTime = time;}
	virtual void updateTime(time_t time);

private:
	wxString getLatestFileName(time_t* time);


public:
	static int cbSQLGetID( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );

public:
	UINT m_currentID;

private:
	std::string m_contents;
	FLOAT m_phaseNoise;
	FLOAT m_unfiltered;
	FLOAT m_cluterSuppersion;
	FLOAT m_filtered ;
	std::multimap<INT,std::vector<double> > m_NoiseData;

	time_t m_updateTime;



};

