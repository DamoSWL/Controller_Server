#pragma once

#include "ITXTParse.h"
#include <ctime>

//该类负责动态范围文件解析
class CDynTestResult :	public ITXTParse
{
public:
	CDynTestResult();
	virtual ~CDynTestResult();

	virtual INT loadTXTFile(time_t* time) override;  //检测新的离线标定文件
	virtual void parseFile() override;   //读取离线标定文件内容并解析
	virtual bool checkFileTime(time_t* time) override;  //检测离线标定文件的修改时间
	virtual bool writeToDataBase(time_t time) override; //将解析内容存放如数据库中
	virtual void setUpdateTime(time_t time) override {m_updateTime = time;}
	virtual void updateTime(time_t time);

public:
	static int cbSQLGetID( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );

private:
	wxString getLatestFileName(time_t* time);

public:
	UINT m_currentID;

private:
	std::string m_contents;
	std::set<std::vector<FLOAT> > m_dynamicData;

	FLOAT m_Hnoise;
	FLOAT m_dynamicRange;
	FLOAT m_slope;
	FLOAT m_dynamicStart;
	FLOAT m_dynamicEnd;

	BOOL m_newNoiseFlag;
	BOOL m_newRangeFlag;
	BOOL m_newSlopFlag;
	BOOL m_newStartFlag;
	BOOL m_newEndFlag;

	time_t m_updateTime;

	  


};

