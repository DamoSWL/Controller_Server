#pragma once
#include "serialport.h"



class SMSThread;

//该类用于接受从java端获取的短信联系人信息和短息内容，并保存下来
class SMSHandler :
	public ISerialPortCalBack
{
public:
	SMSHandler(void);
    ~SMSHandler(void);
    

	virtual void CallBack(UINT msg, WPARAM chi, LPARAM port);

public:
	struct comConfig
	{
		UINT portnr;
		UINT baud;
		char parity;
		UINT databits;
		UINT stopbits;
    };
    
public:
    BOOL StartCom();
    BOOL StopCom();
    const std::multimap<std::string,std::string>& getSMSRawData();  //获取短信原始信息
    void clearSMSData() {wxCriticalSectionLocker Locker(m_SMSCriticaSection);m_SMSRawData.clear();}
    void addSMSData(const std::string& rawData);  //添加短信信息
    void sendUartData(const char* data,UINT len);
    void sendSMSText();  //开启线程，发送短信
	void clearReplyEcho();  //清空回复信息
	void SMSAlarm();

    
private:
    CSerialPort m_serialPort;
    std::multimap<std::string,std::string> m_SMSRawData;  //原始短信信息，保存手机号和短信内容
    wxCriticalSection m_SMSCriticaSection;
    SMSThread* m_SMSThread;

    BOOL m_replyReceiveFlag;

public:
	std::string m_replyEcho;


};


//该类负责处理短信信息，转换为短信编码并发送楚去
class SMSThread : public wxThread
{
public:

    SMSThread(SMSHandler* Handler,wxThreadKind kind = wxTHREAD_DETACHED);
    ~SMSThread();

private:
    SMSHandler* m_SMSHandler;
    std::multimap<std::string,UINT> m_SMSRefinedData;  //转换后的信息，保存编码和编码长度


protected:
    virtual void * Entry();
    void processSMSData(const std::multimap<std::string,std::string>& rawData);  //处理短信信息，转换成编码

    void transformTelNum(std::string& TelNum);  //将手机号码进行转换
    UINT transformContent(std::string& content);  //将短信内容进行转换

};
