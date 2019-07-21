#pragma once

#include <ctime>
#include "ITXTParse.h"



class CAdaptChg : public ITXTParse
{
public:
	CAdaptChg();
	virtual ~CAdaptChg(void);

public:
	virtual INT loadTXTFile(time_t* time) override;
	virtual void parseFile() override;
	virtual bool checkFileTime(time_t* time) override;
	virtual bool writeToDataBase(time_t time) override;
	virtual void setUpdateTime(time_t time) override;
	virtual void updateTime(time_t time);

private:
	time_t getRecordTime(const std::string& dateStr,const std::string& timeStr);
	wxString getLatestFileName(time_t* time);

private:
	wxString m_fileName;
	std::vector<std::string> m_contents;
	UINT m_updateTime;
	UINT m_previousReadSize;
	std::map<INT,std::pair<std::string,std::string> > m_alterRecord;

};

