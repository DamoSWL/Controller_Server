#pragma once

//���ฺ���״����
class CRadarController :wxEvtHandler
{
public:
	CRadarController(void);
	~CRadarController(void);

public:
	static int processData(void *pHandle, const char *pData, UINT Len);  //�����״ﷵ�ص�״̬��Ϣ
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT uiLen = 0);

	BOOL startReceive();
	BOOL stopReceive();
	bool sendDataToRadar(const char* cmd,UINT len,const char* dstIP,UINT port);

	void setNetService(CNetService* localNetService) {m_localNetService = localNetService;} 
	CNetService* getNetService() {return m_localNetService;}

	bool cmdCompare(const wxString& cmd) {m_DisconnectCount = 0; return m_previousStatus == cmd;}  //�жϵ�ǰ�״�״̬��Ϣ�Ƿ����ϴ���ͬ
	void setStatusStr(const wxString& cmd) {m_previousStatus = cmd;}  
	INT parseStatusStr(std::map<wxString,wxString>& statusMap);  //�����״�״̬��Ϣ
	bool writeToDataBase(const std::map<wxString,wxString>& status);
	
	BOOL reStartReceive();

private:
	void onTimeout(wxTimerEvent& event);
	bool sendRequest();
	bool writePingToDataBase(INT result);
	bool writeRemoteCtlToDataBase(INT result);

private:
	ISocketNet* m_udpHandler;
	CNetService* m_localNetService;
	wxTimer m_Timer;
	wxString m_previousStatus;
	UINT m_DisconnectCount;  //���ӶϿ��������յ��������0

	BOOL m_radarConnectionPingFlag;  //��ʶλ������ǰһ��ʹ�ù�ping����
	BOOL m_radarNOCmdFlag;  //��ʶλ,����û�����ݷ���

	DECLARE_EVENT_TABLE()

};

