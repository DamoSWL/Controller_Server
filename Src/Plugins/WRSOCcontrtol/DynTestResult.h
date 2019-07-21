#pragma once

#include "ITXTParse.h"
#include <ctime>

//���ฺ��̬��Χ�ļ�����
class CDynTestResult :	public ITXTParse
{
public:
	CDynTestResult();
	virtual ~CDynTestResult();

	virtual INT loadTXTFile(time_t* time) override;  //����µ����߱궨�ļ�
	virtual void parseFile() override;   //��ȡ���߱궨�ļ����ݲ�����
	virtual bool checkFileTime(time_t* time) override;  //������߱궨�ļ����޸�ʱ��
	virtual bool writeToDataBase(time_t time) override; //���������ݴ�������ݿ���
	virtual void setUpdateTime(time_t time) override {m_updateTime = time;}
	virtual void updateTime(time_t time);

public:
	static int cbSQLGetID( void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName );

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

