#pragma once
#include "ITXTParse.h"
#include <ctime>


namespace Month
{
	enum
	{
		Jan = 1,
		Feb,
		Mar,
		Apr,
		May,
		Jun,
		Jul,
		Aug,
		Sept,
		Oct,
		Nov,
		Dec,
	};
}

//该类负责解析反射率文件

class CRefCalibResult : public ITXTParse
{
public:
	CRefCalibResult();
	virtual ~CRefCalibResult();

	virtual INT loadTXTFile(time_t* time) override;
	virtual void parseFile() override;
	virtual bool checkFileTime(time_t* time) override;
	virtual bool writeToDataBase(time_t time) override;
	virtual void setUpdateTime(time_t time) override {m_updateTime = time;}
	virtual void updateTime(time_t time);

private: 
	UINT getMonthNum(std::string& monthStr);
	wxString getLatestFileName(time_t* time);

private:
	std::string m_contents;
	std::map<UINT, std::map<UINT, std::vector<FLOAT> > >m_refCalibResult;
	std::map<UINT,FLOAT> m_expectedPower;
	std::map<UINT,FLOAT> m_measuredPower;
	std::map<UINT,bool> m_expectedFlag ;
	std::map<UINT,bool> m_measuredFlag;
	std::time_t m_updateTime;

};

