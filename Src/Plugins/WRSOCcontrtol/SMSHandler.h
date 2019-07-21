#pragma once
#include "serialport.h"



class SMSThread;

//�������ڽ��ܴ�java�˻�ȡ�Ķ�����ϵ����Ϣ�Ͷ�Ϣ���ݣ�����������
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
    const std::multimap<std::string,std::string>& getSMSRawData();  //��ȡ����ԭʼ��Ϣ
    void clearSMSData() {wxCriticalSectionLocker Locker(m_SMSCriticaSection);m_SMSRawData.clear();}
    void addSMSData(const std::string& rawData);  //��Ӷ�����Ϣ
    void sendUartData(const char* data,UINT len);
    void sendSMSText();  //�����̣߳����Ͷ���
	void clearReplyEcho();  //��ջظ���Ϣ
	void SMSAlarm();

    
private:
    CSerialPort m_serialPort;
    std::multimap<std::string,std::string> m_SMSRawData;  //ԭʼ������Ϣ�������ֻ��źͶ�������
    wxCriticalSection m_SMSCriticaSection;
    SMSThread* m_SMSThread;

    BOOL m_replyReceiveFlag;

public:
	std::string m_replyEcho;


};


//���ฺ���������Ϣ��ת��Ϊ���ű��벢���ͳ�ȥ
class SMSThread : public wxThread
{
public:

    SMSThread(SMSHandler* Handler,wxThreadKind kind = wxTHREAD_DETACHED);
    ~SMSThread();

private:
    SMSHandler* m_SMSHandler;
    std::multimap<std::string,UINT> m_SMSRefinedData;  //ת�������Ϣ���������ͱ��볤��


protected:
    virtual void * Entry();
    void processSMSData(const std::multimap<std::string,std::string>& rawData);  //���������Ϣ��ת���ɱ���

    void transformTelNum(std::string& TelNum);  //���ֻ��������ת��
    UINT transformContent(std::string& content);  //���������ݽ���ת��

};
