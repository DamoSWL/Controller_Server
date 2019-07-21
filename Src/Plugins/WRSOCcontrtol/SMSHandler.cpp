#include "SMSHandler.h"


SMSHandler::SMSHandler(void):m_SMSThread(NULL),m_replyReceiveFlag(FALSE)
{
}


SMSHandler::~SMSHandler(void)
{
}

void SMSHandler::CallBack(UINT msg, WPARAM chi, LPARAM port)
{
	if (msg == WM_COMM_RXCHAR)
	{
		unsigned char data = (unsigned char)(chi);
		m_replyEcho.append(1,data);
		
	}	
}

BOOL SMSHandler::StartCom()
{
	comConfig serialPortConfig;
	serialPortConfig.portnr = 1;
	serialPortConfig.baud = 9600;
	serialPortConfig.parity = 'N';
	serialPortConfig.databits = 8;
	serialPortConfig.stopbits = 1;

	if (m_serialPort.InitPort(this,
		serialPortConfig.portnr,
		serialPortConfig.baud,
		serialPortConfig.parity,
		serialPortConfig.databits,
		serialPortConfig.stopbits))
	{
		
		return m_serialPort.StartMonitoring();

	}
	return FALSE;
}

BOOL SMSHandler::StopCom()
{
	return m_serialPort.StopMonitoring();
}

const std::multimap<std::string,std::string>& SMSHandler::getSMSRawData()
{
	wxCriticalSectionLocker Locker(m_SMSCriticaSection);
	return m_SMSRawData;
}

void SMSHandler::addSMSData(const std::string& rawData)
{
	wxCriticalSectionLocker Locker(m_SMSCriticaSection);
	auto pos = rawData.find(',');
	if(pos != std::string::npos)
	{
		auto TelNum = rawData.substr(0,pos);
		auto message = rawData.substr(pos+1,rawData.length() - pos - 1);

		m_SMSRawData.insert(std::make_pair(TelNum,message));
	}
}

void SMSHandler::sendUartData(const char* data,UINT len)
{
	m_serialPort.WriteToPort((char*)data,len);
}

void SMSHandler::sendSMSText()
{
	m_SMSThread = NULL;
	m_SMSThread = new SMSThread(this,wxTHREAD_DETACHED);
	wxASSERT_MSG(m_SMSThread!=NULL,_("短信发送线程创建失败"));
	if(m_SMSThread)
	{
		if(m_SMSThread->Run() != wxTHREAD_NO_ERROR)   
		 {             
		  	delete m_SMSThread;       
			m_SMSThread = NULL;  
			GetPlug()->AddLog(LOG_TYPE_ERROR,"短信发送线程启动失败");
		 }
	}

	return;
}

void SMSHandler::clearReplyEcho()
{
	m_replyEcho.clear();
}

void SMSHandler::SMSAlarm()
{
// 	if(m_replyEcho.empty() || (m_replyEcho.find("ERROR") != std::string::npos))
// 	{
// 
// 	}
}

SMSThread::SMSThread(SMSHandler* Handler,wxThreadKind kind /*= wxTHREAD_DETACHED*/):m_SMSHandler(Handler),wxThread(kind)
{

}

SMSThread::~SMSThread()
{

}

void * SMSThread::Entry()
{
	if(m_SMSHandler)
	{
		UINT repeatSndCnt = 0;
		BOOL sndFlag = TRUE;

		m_SMSHandler->clearReplyEcho();  //清空返回内容
		auto rawSMSData = m_SMSHandler->getSMSRawData();  //获取原始信息
		m_SMSHandler->clearSMSData();
		processSMSData(rawSMSData);  //对原始信息进行处理
		

		m_SMSHandler->sendUartData("AT+CMGF=0\x0D", 10);  //发送命令
		::wxSleep(1);
		m_SMSHandler->clearReplyEcho();

		for(auto item = m_SMSRefinedData.cbegin(); item != m_SMSRefinedData.cend(); ) //对多条短信逐个发送
		{
			m_SMSHandler->clearReplyEcho();

			char CMGSCmd[32] = {0};
			sprintf_s(CMGSCmd,32,"AT+CMGS=%d\x0D",item->second);  //发送命令
			m_SMSHandler->sendUartData(CMGSCmd, strlen(CMGSCmd));

			wxThread::Sleep(500);
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,m_SMSHandler->m_replyEcho.c_str());
			m_SMSHandler->clearReplyEcho();

			m_SMSHandler->sendUartData(item->first.c_str(), strlen(item->first.c_str()));  //发送短信内容
			wxThread::Sleep(9*1000);
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,m_SMSHandler->m_replyEcho.c_str());

			if(m_SMSHandler->m_replyEcho.find("CMGS:") == std::string::npos)  
			{
				repeatSndCnt++;
				if(repeatSndCnt >= 5)  //若回复不正确们继续发送，超过5次则发送下一条
				{
					item++;
					repeatSndCnt = 0;
					sndFlag = FALSE;
				}
			}
			else
			{
				item++;  //若收到正确回复，则发送吓一跳
				repeatSndCnt = 0;
			}
		}

		if(sndFlag)
		{
			GetPlug()->m_netService.sendDataToFront("SUCCESS",7,GetPlug()->m_ClientIPAddress.c_str(),9898);
		}
		else
		{
			GetPlug()->m_netService.sendDataToFront("FAIL",4,GetPlug()->m_ClientIPAddress.c_str(),9898);
		}

		m_SMSHandler->clearReplyEcho();
		m_SMSRefinedData.clear();

	}

	return (wxThread::ExitCode)0;
}

void SMSThread::processSMSData(const std::multimap<std::string,std::string>& rawData)
{
	for(auto item = rawData.cbegin(); item != rawData.cend(); item++)
	{
		std::string SMSTextInPDU;
		std::string TelNum = item->first;
		std::string content = item->second;

		transformTelNum(TelNum);
		auto len = transformContent(content);

		SMSTextInPDU.append("0011000D91");
		SMSTextInPDU.append(TelNum);
		SMSTextInPDU.append("0008A7");
		SMSTextInPDU.append(content);
		SMSTextInPDU.append("\x1A\x0D");

		m_SMSRefinedData.insert(std::make_pair(SMSTextInPDU,len + 15));

	}
}

void SMSThread::transformTelNum(std::string& TelNum)
{
	int index = 0;
	TelNum.append(1,'F');

	for(auto item = TelNum.begin(); item != TelNum.end(); item++)
	{
		if(index % 2 == 0)
		{
			auto tmpNum = *item;
			*item = *(item+1);
			*(item+1) = tmpNum;

		}

		index++;
	}
}

UINT SMSThread::transformContent(std::string& content)
{
	auto wContent = GetPlug()->GbkToUnicode(content);
	content.clear();

	auto len = 2 * wcslen(wContent.c_str());
	char lenStr[8] = {0};
	sprintf_s(lenStr,8,"%02X",len);
	content.append(lenStr);

	for(auto item = wContent.cbegin(); item != wContent.cend(); item++)
	{
		auto singleWCharacter = *item;
		UCHAR singleCharacter[4] = {0};
		memmove_s((void*)singleCharacter,2,(void*)&singleWCharacter,2);

		auto tmpCharacter = singleCharacter[0];
		singleCharacter[0] = singleCharacter[1];
		singleCharacter[1] = tmpCharacter;

		content.append(1,(singleCharacter[0] / 16 < 10)? ('0' + singleCharacter[0] / 16):('A' + singleCharacter[0] / 16 - 10)) ;
		content.append(1,(singleCharacter[0] % 16 < 10)? ('0' + singleCharacter[0] % 16):('A' + singleCharacter[0] % 16 - 10)) ;
		content.append(1,(singleCharacter[1] / 16 < 10)? ('0' + singleCharacter[1] / 16):('A' + singleCharacter[1] / 16 - 10)) ;
		content.append(1,(singleCharacter[1] % 16 < 10)? ('0' + singleCharacter[1] % 16):('A' + singleCharacter[1] % 16 - 10)) ;
	}

	return len;
}
