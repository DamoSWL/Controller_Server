#include "Parser.h"

static const int TXTPARSERTIMERID = 125;
static const int CHGPARSERTIMERID = 127;


BEGIN_EVENT_TABLE(CParser,wxEvtHandler)
	EVT_TIMER(TXTPARSERTIMERID,CParser::onTxtParserTimeout)
	EVT_TIMER(CHGPARSERTIMERID,CParser::onChgParserTimeout)
END_EVENT_TABLE()

CParser::CParser():m_txtParserTimer(this,TXTPARSERTIMERID),
m_chgParserTimer(this,CHGPARSERTIMERID),
m_DynTestParser(new CDynTestResult),
m_PhaseNoiseParser(new CPhaseNoiseResult),
m_RefCalibParser(new CRefCalibResult),
m_SunResultParser(new CSunResultParse),
m_AdaptChgParser(new CAdaptChg)
{

}


CParser::~CParser()
{
}

void CParser::setUpdateTime()
{
	m_DynTestParser->setUpdateTime(GetPlug()->getfileConfig().DynTestResulttime);
	m_PhaseNoiseParser->setUpdateTime(GetPlug()->getfileConfig().PhaseNoiseResulttime);
	m_RefCalibParser->setUpdateTime(GetPlug()->getfileConfig().RefCalibResulttime);
	m_SunResultParser->setUpdateTime(GetPlug()->getfileConfig().SunResulttime);

// 	m_RadarExeParser.setUpdateTime();
// 	m_RadarExeParser.setPreviousFileName();
// 
// 	m_AdaptChgParser->setUpdateTime(GetPlug()->getfileConfig().adaptchgtime);
}

void CParser::startTimer()
{
	m_txtParserTimer.Start(10*1000,true);
	//m_chgParserTimer.Start(10*1000,true);
}

void CParser::stopTimer()
{
	m_txtParserTimer.Stop();
	//m_chgParserTimer.Stop();
}

void CParser::onTxtParserTimeout(wxTimerEvent& event)
{
	wxDateTime currentDateTime((time_t)::wxDateTime::GetTimeNow());

	auto weekDay = currentDateTime.GetWeekDay();
	auto monthDay = currentDateTime.GetDay();



	time_t time = 0;
//	if(weekDay == wxDateTime::Mon)
//	{

		if (m_DynTestParser->loadTXTFile(&time) == EXIT_SUCCESS)
		{
			if (m_DynTestParser->checkFileTime(&time))
			{
				m_DynTestParser->parseFile();
				if(m_DynTestParser->writeToDataBase(time))
				{
					m_DynTestParser->updateTime(time);
				}
			}

		}
		else
		{
		//	GetPlug()->AddLog(LOG_TYPE_ERROR,"dyntestresult.txt打开失败");
		}

		if (m_PhaseNoiseParser->loadTXTFile(&time) == EXIT_SUCCESS)
		{
			if (m_PhaseNoiseParser->checkFileTime(&time))
			{
				m_PhaseNoiseParser->parseFile();
				if(m_PhaseNoiseParser->writeToDataBase(time))
				{
					m_PhaseNoiseParser->updateTime(time);
				}
			}

		}
		else
		{
			//GetPlug()->AddLog(LOG_TYPE_ERROR,"phasenoiseresult.txt打开失败");
		}

		if (m_RefCalibParser->loadTXTFile(&time) == EXIT_SUCCESS)
		{
			if (m_RefCalibParser->checkFileTime(&time))
			{
				m_RefCalibParser->parseFile();
				if(m_RefCalibParser->writeToDataBase(time))
				{
					m_RefCalibParser->updateTime(time);
				}
			}

		}
		else
		{
			//GetPlug()->AddLog(LOG_TYPE_ERROR,"refcalibresult.txt打开失败");
		}

//	}

//	if(monthDay == 1)
//	{
		if (m_SunResultParser->loadTXTFile(&time) == EXIT_SUCCESS)
		{
			if (m_SunResultParser->checkFileTime(&time))
			{
			//	m_SunResultParser->parseFile();
			//	if(m_SunResultParser->writeToDataBase(time))
			//	{
			//		m_SunResultParser->updateTime(time);
			//	}
			}

		}
		else
		{
			//GetPlug()->AddLog(LOG_TYPE_ERROR,"sunresult.txt打开失败");
		}
//	}

		m_txtParserTimer.Start(1*1000,true);
}

void CParser::onChgParserTimeout(wxTimerEvent& event)
{
	time_t newTime = 0;
	wxString newFileName;
	if(m_RadarExeParser.checkFileTime(&newTime,newFileName))
	{
		if(m_RadarExeParser.writeToDataBase(newTime,newFileName))
		{
			m_RadarExeParser.updateTime(newTime);
			m_RadarExeParser.updateFileName(newFileName);
		}
	}


	if(m_AdaptChgParser->loadTXTFile(&newTime) == EXIT_SUCCESS)
	{
		if(m_AdaptChgParser->checkFileTime(&newTime))
		{
			m_AdaptChgParser->parseFile();
			if(m_AdaptChgParser->writeToDataBase(newTime))
			{
				m_AdaptChgParser->updateTime(newTime);
			}
		}

	}
	else
	{
		//GetPlug()->AddLog(LOG_TYPE_ERROR,"adaptchg.txt打开失败");
	}

	m_chgParserTimer.Start(10*1000,true);
	
}








