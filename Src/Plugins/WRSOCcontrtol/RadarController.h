#pragma once

//该类负责雷达控制
class CRadarController :wxEvtHandler
{
public:
	CRadarController(void);
	~CRadarController(void);

public:
	static int processData(void *pHandle, const char *pData, UINT Len);  //处理雷达返回的状态信息
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT uiLen = 0);

	BOOL startReceive();
	BOOL stopReceive();
	bool sendDataToRadar(const char* cmd,UINT len,const char* dstIP,UINT port);

	void setNetService(CNetService* localNetService) {m_localNetService = localNetService;} 
	CNetService* getNetService() {return m_localNetService;}

	bool cmdCompare(const wxString& cmd) {m_DisconnectCount = 0; return m_previousStatus == cmd;}  //判断当前雷达状态信息是否与上次相同
	void setStatusStr(const wxString& cmd) {m_previousStatus = cmd;}  
	INT parseStatusStr(std::map<wxString,wxString>& statusMap);  //解析雷达状态信息
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
	UINT m_DisconnectCount;  //连接断开次数，收到命令后清0

	BOOL m_radarConnectionPingFlag;  //标识位，表明前一次使用过ping命令
	BOOL m_radarNOCmdFlag;  //标识位,表明没有数据返回

	DECLARE_EVENT_TABLE()

};

