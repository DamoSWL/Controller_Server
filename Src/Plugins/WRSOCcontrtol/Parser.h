#pragma once

#include "DynTestResult.h"
#include "PhaseNoiseResult.h"
#include "RefCalibResult.h"
#include "SunResultParse.h"
#include "AdaptChg.h"
#include "RadarExeParser.h"
#include <memory>


//该类负责对离线文件进行解析
class CParser : public wxEvtHandler
{
public:
	CParser();
	~CParser();

public:
	void setUpdateTime();
	void startTimer();
	void stopTimer();

private:
	void onTxtParserTimeout(wxTimerEvent& event);
	void onChgParserTimeout(wxTimerEvent& event);

private:
	wxTimer m_txtParserTimer;
	wxTimer m_chgParserTimer;

	std::shared_ptr<ITXTParse> m_DynTestParser;
	std::shared_ptr<ITXTParse> m_PhaseNoiseParser;
	std::shared_ptr<ITXTParse> m_RefCalibParser;
	std::shared_ptr<ITXTParse> m_SunResultParser;
	

	CRadarExeParser m_RadarExeParser;
	std::shared_ptr<ITXTParse> m_AdaptChgParser;


	DECLARE_EVENT_TABLE()

};

