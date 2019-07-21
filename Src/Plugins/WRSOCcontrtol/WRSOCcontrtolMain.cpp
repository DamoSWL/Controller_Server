// PluginTest.cpp : Defines the entry point for the DLL application.
//

#ifdef WX_PRECOMP
	#include "wx_pch.h"
#endif

#include "HCNetSDK.h"
#include <wx/dir.h>
#include "Temperature.h"
#include "sdk/PluginSystem/GeneralFun.h"
#include <fstream>


wxDEFINE_EVENT(RADARFILE_EVENT, wxCommandEvent);


void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser)
{

	auto aa = dwDataType;

	auto len = dwBufSize;

	BYTE* data = new BYTE[dwBufSize];
	memcpy_s(data,dwBufSize,pBuffer,dwBufSize);

	auto cnt = 0;

// 	for(auto i=0;i<dwBufSize;i++)
// 	{
// 		if(data[i])
// 		{
// 			cnt++;
// 		}
// 	}

	delete[] data;

	
}


WRSOCcontrtolPlug* g_pMain = NULL;

// 全局函数
WRSOCcontrtolPlug* GetPlug() 
{ 
	return g_pMain;
};
PL_GUID	g_DependGuid[10]={0};
const char *g_szDependPluginName[10]={0};

static const int INITIALTIMERID = 110;
static const int CAPTUREJPEGTIMERID = 120;
static const int HDCFGTIMERID = 130;
static const int DVRLOGTIMERID = 140;
static const int RADARFILETIMERID = 150;
static const int RADARRECEIVETIMERID = 160;
static const int CONNECTIONTIMERID = 170;
static const int RADARFILEDETECTTIMERID = 300;

BEGIN_EVENT_TABLE(WRSOCcontrtolPlug,wxEvtHandler)
	EVT_TIMER(INITIALTIMERID,WRSOCcontrtolPlug::onInitialTimeout)
	EVT_TIMER(CAPTUREJPEGTIMERID,WRSOCcontrtolPlug::onCaptureJPEGTimeout)
	EVT_TIMER(HDCFGTIMERID,WRSOCcontrtolPlug::onHDCfgTimeout)
	EVT_TIMER(DVRLOGTIMERID,WRSOCcontrtolPlug::onDVRLogTimeout)
	EVT_TIMER(RADARFILETIMERID,WRSOCcontrtolPlug::onRadarFileTimeout)
	EVT_TIMER(RADARRECEIVETIMERID,WRSOCcontrtolPlug::onRadarReceiveTimeout)
	EVT_TIMER(CONNECTIONTIMERID,WRSOCcontrtolPlug::onConnectionTimeout)
	EVT_TIMER(RADARFILEDETECTTIMERID,WRSOCcontrtolPlug::onRadarFileDetectTimeout)
END_EVENT_TABLE()


WRSOCcontrtolPlug::WRSOCcontrtolPlug():
  m_IHost(NULL),m_IComm(NULL),
	  m_initialTimer(this,INITIALTIMERID),
	  m_captureJPEGTimer(this,CAPTUREJPEGTIMERID),
	  m_HDCfgTimer(this,HDCFGTIMERID),
	  m_DVRLogTimer(this,DVRLOGTIMERID),
	  m_RadarFileTimer(this,RADARFILETIMERID),
	  m_radarReceiveTimer(this,RADARRECEIVETIMERID),
	  m_ConnectionTimer(this,CONNECTIONTIMERID),
	  m_pEnvSocket(NULL),
	  m_pEnvHandler(NULL),
	  m_DiskExceptionFlag(FALSE),
	  m_VideoExceptionFlag(FALSE),
	  m_RadarExceptionFlag(FALSE),
	  m_UPSExceptionFlag(FALSE),
	  m_FTPThread(NULL),
	  m_UPSControlMode(UPS_NetControl),
	  m_RadarFileDetectTimer(this,RADARFILEDETECTTIMERID),
	  m_radarMode(SINGLE_POLARIZATION),
	  m_alarmStartTime(0),
	  m_alarmEndTime(0),
	  m_alarmLevel(0)
  {
	g_pMain = this;
	////////////////
	m_bExit = false;
	m_WorkPath = wxEmptyString;

#ifdef __TESTWRSOC__
	m_state14 = 0;
#endif
	//{	//济南协议测试 Modbus通信协议设计说明书_Ver 1.34_C.pdf
	//	////Modbus通信协议设计说明书_Ver 1.34_C(3).pdf 协议分析
	//	//unsigned char test[]={0x1,0x3,0x0,0x2,0x0,0x1,0xca,0x25};//查询,CRC=0x25ca,请求 1 号机的数据，位置为：寄存器起始地址 0002，寄存器个数为 1 个
	//	//unsigned char test[]={0x1,0x3,0x2,0x12,0x22};//响应，该响应CRC有问题
	//	//unsigned char test[]={0x12,0x3,0x0,0x5,0x0,0x2,0x96,0xad};//查询,CRC=0xd6a9,该命令CRC有问题
	//	unsigned char test[]={0x12,0x3,0x4,0x01,0xf6,0x01,0xf6,0xb8,0xea};//响应，CRC=0xb8ea，CRC低字节在前传输，其他值高字节在前传输
	//	int len=sizeof(test)-2;
	//	unsigned int ret= chkcrc(test,len);
	//	ret=ret;
	//}

	//{
	//	//重庆协议测试 RT RS232通讯协议(新）.pdf
	//	char cmd[50]={0};
	//	sprintf(cmd,"^P005Q3GS");
	//	int len=strlen(cmd);
	//	cmd[len]=0xd;
	//	len=strlen(cmd);
	//	//m_udpHandler->SendData(cmd,sizeof(cmd),GetPlug()->m_UPSIPAdress.c_str(),23);
	//	int aa=0;
	//	aa=1;
	//}
	
	
}


bool WRSOCcontrtolPlug::OnCommandHandle(
	PL_WORD wNotifyCode, 
	PL_WORD wID, 
	void * hWndCtl) 
{
	switch(wID)
	{
	case 11:
		if (m_IComm->VerifyRight())
		{
			m_IHost->ShowTheSetDlg( g_guid_Set );
		}
		
		break;
	default:
		break;
	}
	return true;
}

//! 初始化完毕后调用, 插件在此函数中进行UI初始化等操作
void WRSOCcontrtolPlug::OnInitialized()
{
	//// 获取需要的接口
	m_IHost = (IHostPlugin*)m_IFrame->QueryInterfaceP( g_PL_GRSFrame, g_I_Host );
	m_IComm = (CDYW::Plugin::ICommon *)m_IFrame->QueryInterfaceP( g_PL_GRSFrame, guid_I_Common );
	if( m_IComm ) {
		m_WorkPath = wxString(m_IComm->GetWorkPath(),wxConvLocal);
		m_IComm->AddLog( 0, "插件[  WRSOCcontrtol ]..." );
	}

		m_pDatabase = (IMySqlDB*)m_IFrame->QueryInterfaceP(g_PL_MySqlDB,g_I_MySqlDB);
		m_CurlModule = (ICurlModule*)m_IFrame->QueryInterfaceP(g_PL_CurlModule,g_I_CurlModule);
// 		m_pTCPServer = (ITCPServer *)m_IFrame->QueryInterfaceP(g_PL_TCPServer,g_I_TCPServer);
// 
// 		if (m_pTCPServer)
// 		{
// 			m_pTCPInstance=m_pTCPServer->GetInstance(1024*1024,1024*1024,1024*1000*20,1024*1000*20);
// 		}

	
	
	// 注册工具栏 和 菜单
 	if(  m_IHost ) 
 	{
 				//// 注册菜单
 		//memset( &m_menu, 0, sizeof(IPluginMenu));
 		m_menu.pluginGUID = g_PL_WRSOCcontrtol;
 		m_menu.parGUID = guid_SetConfig;
 		m_menu.selfGUID = g_guid_menu;
 		m_menu.menuDefine = (char*)g_MenuXML;
 		//m_menu.lSize = strlen(g_MenuXML);
 		m_IHost->RegisterMenu( m_menu );
 				
 				//// 注册工具栏
 		//memset( &m_toolbar, 0, sizeof(IPluginToolBar));
 // 		m_toolbar.bNewBar = true;
 // 		m_toolbar.pluginGUID = g_PL_WRSOCcontrtol;
 // 		m_toolbar.parGUID = guid_NULL;
 // 		m_toolbar.selfGUID = g_guid_toolbar;
 // 		strcpy( m_toolbar.name, "TB_WRSOCcontrtol" );
 // 		m_toolbar.barDefine = (char*)g_ToolBarXML;
 // 		m_toolbar.lSize = strlen(g_ToolBarXML);
 // 
 // 		m_IHost->RegisterToolBar( m_toolbar );
 				
 				//// 注册参数设置面板
 		//memset( &m_DlgPara, 0 , sizeof(IPluginDlgPara));
 		strcpy( m_DlgPara.cName, "系统配置" );
 		m_DlgPara.selfGUID = g_guid_Set;
 		m_DlgPara.parGUID = guid_NULL;
 		m_DlgPara.pluginGUID = g_PL_WRSOCcontrtol;
 		m_DlgPara.funCreate = WRSOCcontrtolSet::CreatePage;
 
 		m_IHost->RegisterSettingDialog(m_DlgPara);

 	}
}

void WRSOCcontrtolPlug::OnExit()
{
	m_bExit = true;

	AddLog(LOG_TYPE_MESSAGE,"程序退出中......");

	m_SMSHandler.StopCom();
	TXTparser.stopTimer();

	m_initialTimer.Stop();
	m_captureJPEGTimer.Stop();
	m_HDCfgTimer.Stop();
	m_DVRLogTimer.Stop();
	m_RadarFileTimer.Stop();
	m_ConnectionTimer.Stop();
	m_RadarFileDetectTimer.Stop();

	if(m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND)
	{
		m_Generator.stopTimer();
		m_Generator.stopReceive();
	}


	DVRLogout();

	delete m_sqlQueryLocker;
	delete m_xmlWriteLocker;


	if(m_pEnvHandler)
	{
		m_pEnvHandler->stopTimer();
		delete m_pEnvHandler;
		m_pEnvHandler = NULL;
	}

	if(m_pEnvSocket)
	{
		m_pEnvSocket->StopReceive();
		m_UDPFactory.ReleaseInstance(m_pEnvSocket);
		m_pEnvSocket = NULL;

	}

	m_netService.stopReceive();


	if(WaitForSingleObject(logHandler,INFINITE) == WAIT_OBJECT_0)
	{
		CloseHandle(logHandler);
	}

	if(WaitForSingleObject(radarHandler,INFINITE) == WAIT_OBJECT_0)
	{
		CloseHandle(radarHandler);
	}

	if(WaitForSingleObject(onlineRadarHandler,INFINITE) == WAIT_OBJECT_0)
	{
		CloseHandle(onlineRadarHandler);
	}

	if(WaitForSingleObject(envHandler,INFINITE) == WAIT_OBJECT_0)
	{
		CloseHandle(envHandler);
	}

	if(WaitForSingleObject(upsHandler,INFINITE) == WAIT_OBJECT_0)
	{
		CloseHandle(upsHandler);
	}

	if(m_FTPThread)
	{
		m_FTPThread->Delete();
		while(1)
		{
			if(m_FTPThread == NULL)
			{
				break;
			}
		}
	}


}

void WRSOCcontrtolPlug::OnActived()
{
	m_bExit = false;

	m_sqlQueryLocker = new wxMutex(wxMUTEX_DEFAULT);
	m_xmlWriteLocker = new wxMutex(wxMUTEX_DEFAULT);

	memset(&m_SysConfig, 0, sizeof(tagSysConfig));
	if(!loadXMLFile())
	{
		writeXMLFile();
	}

	if(!loadDirConfigFile())
	{
		return;
	}

	//测试代码
	{		
		//removeExcessiveVideo();

		//unsigned char data[]={
		//	0x7F, 0x7E, 0xFF, 0xFF, 0x61, 0xF7, 0x83, 0x00, 0x7F, 0x7E, 0x98, 0x6F, 0x79, 0xBB, 0xEF, 0xC7, 0xEF, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD9, 0xF7, 0xD9, 0xF7, 0xD9, 0xF7, 0xFF, 0x37, 0xFF, 0x37, 0xFF, 0x37, 0xEF, 0xB9, 0xEF, 0xB7, 0xEF, 0xBD, 0xFF, 0x97, 0xFF, 0x95, 0xFF, 0x91, 0xD9, 0xF5, 0xD9, 0xF7, 0xD9, 0xF5, 0xFF, 0xA3, 0xFF, 0x9F, 0xFF, 0xA9, 0xEF, 0xC7, 0xEF, 0xC7, 0xEF, 0xC9, 0xFF, 0xDD, 0xFF, 0xDF, 0xFF, 0xC9, 0xD9, 0xF7, 0xD9, 0xF7, 0xD9, 0xF7, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0xFF, 0x0B, 0xEF, 0x0F, 0xEF, 0x13, 0xFF, 0xF9, 0xFF, 0xF9, 0xFF, 0x0B, 0x01, 0x0F, 0xF9, 0x2F, 0xDF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x91, 0xFF, 0xFD, 0xFF, 0xB1, 0xFF, 0xE1, 0xEF, 0xBF, 0xEF, 0xBD, 0xEF, 0xBB, 0xEF, 0xB9, 0xEF, 0xC3, 0xEF, 0xBD, 0xFF, 0xFD, 0x4D, 0xC9, 0x00
		//	//0x7F, 0x7E, 0xFF, 0xFF, 0x8D, 0xF5, 0x4F, 0x00, 0x7F, 0x7E, 0xA3, 0x6F, 0x79, 0xBB, 0xEF, 0xC3, 0xEF, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD9, 0xE9, 0xD9, 0xE9, 0xD9, 0xE9, 0xFF, 0x37, 0xFF, 0x37, 0xFF, 0x37, 0xEF, 0xB9, 0xEF, 0xB1, 0xEF, 0xBD, 0xFF, 0x47, 0xFF, 0x47, 0xFF, 0x47, 0xD9, 0xEB, 0xD9, 0xE9, 0xD9, 0xEB, 0xFF, 0x81, 0xFF, 0x85, 0xFF, 0x83, 0xEF, 0xC5, 0xEF, 0xC5, 0xEF, 0xC7, 0xFF, 0x9F, 0xFF, 0xA1, 0xFF, 0x9D, 0xD9, 0xED, 0xD9, 0xED, 0xD9, 0xED, 0xFF, 0x75, 0xFF, 0x51, 0xFF, 0x55, 0xFF, 0xF1, 0xFF, 0xED, 0xFF, 0xED, 0xFF, 0xF1, 0xFF, 0xED, 0xFF, 0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF, 0x0B, 0xEF, 0x11, 0xEF, 0x15, 0xFF, 0xF9, 0xFF, 0xF9, 0xFF, 0x0B, 0xB1, 0x4F, 0xF9, 0x2F, 0xA7, 0x5D, 0x00
		//};
		//unsigned char data1[]={
		//	0x7F, 0x7E, 0xFF, 0xFF, 0x8D, 0xF5, 0x4F, 0x00, 0x7F, 0x7E, 0xA3, 0x6F, 0x79, 0xBB, 0xEF, 0xC3, 0xEF, 0xB3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD9, 0xE9, 0xD9, 0xE9, 0xD9, 0xE9, 0xFF, 0x37, 0xFF, 0x37, 0xFF, 0x37, 0xEF, 0xB9, 0xEF, 0xB1, 0xEF, 0xBD, 0xFF, 0x47, 0xFF, 0x47, 0xFF, 0x47, 0xD9, 0xEB, 0xD9, 0xE9, 0xD9, 0xEB, 0xFF, 0x81, 0xFF, 0x85, 0xFF, 0x83, 0xEF, 0xC5, 0xEF, 0xC5, 0xEF, 0xC7, 0xFF, 0x9F, 0xFF, 0xA1, 0xFF, 0x9D, 0xD9, 0xED, 0xD9, 0xED, 0xD9, 0xED, 0xFF, 0x75, 0xFF, 0x51, 0xFF, 0x55, 0xFF, 0xF1, 0xFF, 0xED, 0xFF, 0xED, 0xFF, 0xF1, 0xFF, 0xED, 0xFF, 0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF, 0x0B, 0xEF, 0x11, 0xEF, 0x15, 0xFF, 0xF9, 0xFF, 0xF9, 0xFF, 0x0B, 0xB1, 0x4F, 0xF9, 0x2F, 0xA7, 0x5D, 0x00
		//};
		//int len=sizeof(data);//79
		//int len1=sizeof(data1);//57

		//CRCData CRCCheckData;
		//CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)data,len-2);

		//CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)data1,len1-2);

		

		{//济南站测试数据
			//unsigned char cmd[8] = {0};
			//cmd[0] = 0x01;//设备地址(需要修改)
			//cmd[1] = 0x03;//功能码，0x03遥测量获取
			//cmd[2] = 0x00;
			//cmd[3] = 0x00;//寄存器起始地址
			//cmd[4] = 0x00;
			//cmd[5] = 0x39;//寄存器个数 57
			//CRCData CRCCheckData;
			//CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)cmd,6);
			//cmd[6] = CRCCheckData.crcByte[1];
			//cmd[7] = CRCCheckData.crcByte[0];
			////01 03 00 00 00 39 85 D8
			//CRCCheckData.crcData=CRCCheckData.crcData;

			/*char data[] = {0x01, 0x03, 0x72, 0x08, 0x92, 0x08, 0x92, 0x08, 0x8D, 0x08, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x85, 0x13, 0x85, 0x13, 0x85, 0x00, 0x64, 0x00, 0x64, 0x00, 0x64, 0x08, 0x90, 0x08, 0x91, 0x08, 0x8E, 0x00, 0x99, 0x00, 0x97, 0x00, 0x97, 0x13, 0x85, 0x13, 0x85, 0x13, 0x85, 0x00, 0x5F, 0x00, 0x5D, 0x00, 0x5D, 0x08, 0x9C, 0x08, 0x9C, 0x08, 0x9B, 0x00, 0x88, 0x00, 0x8F, 0x00, 0x8D, 0x13, 0x85, 0x13, 0x85, 0x13, 0x85, 0x00, 0x5E, 0x00, 0x60, 0x00, 0x60, 0x00, 0x1D, 0x00, 0x1F, 0x00, 0x1E, 0x00, 0x1C, 0x00, 0x1E, 0x00, 0x1D, 0x00, 0x07, 0x00, 0x08, 0x00, 0x07, 0x00, 0x5D, 0x00, 0x65, 0x00, 0x61, 0x00, 0xFA, 0x08, 0x77, 0x08, 0x75, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFA, 0x06, 0xE6, 0x03, 0xE8, 0x67, 0xA0};
			m_Radarcd=wxT("Z9531");
			m_UPSHandlerRS232.processData(&m_UPSHandlerRS232,data,sizeof(data));*/
		}
		
	}
	
	m_initialTimer.Start(1000);
}

void WRSOCcontrtolPlug::OnDestroy()
{

}

void * WRSOCcontrtolPlug::QueryInterfaceP(PL_IID guid)
{
	if( IsEqualID( guid, g_I_WRSOCcontrtol ) )
	{
		return (IWRSOCcontrtol*)(g_pMain); 
	}
	return NULL;  
}

PL_GUID * WRSOCcontrtolPlug::GetDependenceList(int &cnt ,const char ***ppszDepNames)
{
	g_DependGuid[0]			= g_PL_MySqlDB;
	g_szDependPluginName[0] = g_Name_MySqlDB;

	cnt = 1;
	if( ppszDepNames != NULL )
	{
		*ppszDepNames = g_szDependPluginName;
	}
	return g_DependGuid;
}


std::string WRSOCcontrtolPlug::UnicodeToUtf8(const wchar_t* unicode)
{
	int len = 0;
	std::string utf8Str;
	if (unicode)
	{
		len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
		PCHAR szUtf8 = new CHAR[len + 1];
		memset(szUtf8, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
		utf8Str.assign(szUtf8);
		delete[] szUtf8;
	}

	return utf8Str;
}

std::wstring WRSOCcontrtolPlug::Utf8ToUnicode(const char* utf8Str)
{
	int len = 0;
	std::wstring unicodeStr;
	if(utf8Str)
	{
		len = MultiByteToWideChar(CP_UTF8, NULL, utf8Str, strlen(utf8Str), NULL, 0);
		PWCHAR wszString = new WCHAR[len + 1];
		MultiByteToWideChar(CP_UTF8, NULL, utf8Str, strlen(utf8Str), wszString, len);
		wszString[len] = '\0';
		unicodeStr.assign(wszString);
		delete[] wszString;
	}

	return unicodeStr;
}

std::wstring WRSOCcontrtolPlug::GbkToUnicode(const std::string& strGbk)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, NULL, 0);
	wchar_t *strUnicode = new wchar_t[len];
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, strUnicode, len);

	std::wstring strTemp(strUnicode);//此时的strTemp是Unicode编码
	delete[] strUnicode;
	strUnicode = NULL;
	return strTemp;

}

tagSysConfig* WRSOCcontrtolPlug::getConfig()
{
	return &m_SysConfig;
}

void WRSOCcontrtolPlug::setConfig(tagSysConfig& SysConfig)
{
	memcpy(&m_SysConfig, &SysConfig, sizeof(tagSysConfig));
	writeXMLFile();
}

bool WRSOCcontrtolPlug::InitToolbarData()
{
	wxString FileName;
	FileName = wxString( m_IComm->GetWorkPath(),wxConvLocal);
	FileName += wxT("\\Config\\WRSOCcontrtol\\WRSOCcontrtolToolbar.xml");
	//if (!wxFileExists(FileName))
	{
		return false;
	}
	//return InitMenuData(g_ToolBarXML,MAX_TOOL_CON,FileName);
}

bool WRSOCcontrtolPlug::InitMenuData(char* pData,int DataSize,wxString FileName)
{
	memset(pData,0,sizeof(char)*DataSize);
	FILE *fp = fopen(FileName.mb_str(wxConvLocal),"rb");
	if(fp!=NULL)
	{
		fseek(fp,0,SEEK_END);
		int Size = ftell(fp);
		if (Size>=0)
		{
			fseek(fp,0,SEEK_SET);
			fread(pData,Size,1,fp);
		}
		fclose(fp);
	}
	else
	{
		return false;
	}
	if (strlen(pData) <= 0)
	{
		return false;
	}
	return true;
}

bool WRSOCcontrtolPlug::loadDirConfigFile()
{
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");

	pugi::xml_parse_result result = m_doc.load_file(fileName.wc_str());
	if(result.status !=pugi::status_ok)
	{
		AddLog(LOG_TYPE_ERROR,"读取DirConfig.xml配置文件失败!");
		return false;
	}

	pugi::xml_node root = m_doc.child("config");

	pugi::xml_node database = root.child("database");
	const char* hostname = database.child("hostname").text().as_string();
	m_databaseConfig.hostname.assign(hostname);


	const char* user = database.child("user").text().as_string();
	m_databaseConfig.user.assign(user);


	const char* password = database.child("password").text().as_string();
	m_databaseConfig.password.assign(password);


	const char* databasename = database.child("databasename").text().as_string();
	m_databaseConfig.databasename.assign(databasename);


	pugi::xml_node txtfile = root.child("txtfile");
	const char* SunResultdir = txtfile.child("SunResultdir").text().as_string();
	m_fileConfig.SunResultdir.assign(SunResultdir);


	m_fileConfig.SunResulttime = txtfile.child("SunResulttime").text().as_llong();

	const char* RefCalibResultdir = txtfile.child("RefCalibResultdir").text().as_string();
	m_fileConfig.RefCalibResultdir.assign(RefCalibResultdir);


	m_fileConfig.RefCalibResulttime = txtfile.child("RefCalibResulttime").text().as_llong();

	const char* DynTestResultdir = txtfile.child("DynTestResultdir").text().as_string();
	m_fileConfig.DynTestResultdir.assign(DynTestResultdir);


	m_fileConfig.DynTestResulttime = txtfile.child("DynTestResulttime").text().as_llong();

	const char* PhaseNoiseResultdir = txtfile.child("PhaseNoiseResultdir").text().as_string();
	m_fileConfig.PhaseNoiseResultdir.assign(PhaseNoiseResultdir);


	m_fileConfig.PhaseNoiseResulttime = txtfile.child("PhaseNoiseResulttime").text().as_llong();

	const char* alarmLogDir = txtfile.child("alarmLogDir").text().as_string();
	m_fileConfig.alarmLogDir.assign(alarmLogDir);

	m_fileConfig.alarmtime = txtfile.child("alarmtime").text().as_uint();

	const char* adaptchgdir = txtfile.child("adaptchgdir").text().as_string();
	m_fileConfig.adaptchgdir.assign(adaptchgdir);

	m_fileConfig.adaptchgtime = txtfile.child("adaptchgtime").text().as_llong();

	m_fileConfig.adaptchgreadsize = txtfile.child("adaptchgreadsize").text().as_uint();

	const char* radarexeDir = txtfile.child("radarexeDir").text().as_string();
	m_fileConfig.radarexeDir.assign(radarexeDir);

	m_fileConfig.radarexetime = txtfile.child("radarexetime").text().as_llong();

	const char* radarexefilename = txtfile.child("radarexefilename").text().as_string();
	m_fileConfig.radarexefilename.assign(radarexefilename);

	pugi::xml_node radar = root.child("radar");
	const char* radarStatusDir = radar.child("radarStatusDir").text().as_string();
	m_fileConfig.radarStatusDir.assign(radarStatusDir);

	m_fileConfig.radarStatustime = radar.child("radarStatustime").text().as_llong();

	const char* radarDataDir = radar.child("radarDataDir").text().as_string();
	m_fileConfig.radarDataDir.assign(radarDataDir);

	m_fileConfig.radarDatatime = radar.child("radarDatatime").text().as_llong();

	pugi::xml_node video = root.child("video");
	const char* videodir = video.child("videodir").text().as_string();
	//m_fileConfig.videoDir.assign(videodir);

	m_fileConfig.videoInterval = video.child("videointerval").text().as_uint();
	if(m_fileConfig.videoInterval>60) m_fileConfig.videoInterval = 60;
	else if(m_fileConfig.videoInterval<1) m_fileConfig.videoInterval = 1;
	m_fileConfig.videoSave = video.child("videosave").text().as_uint();
	if(m_fileConfig.videoSave>12) m_fileConfig.videoInterval = 12;
	else if(m_fileConfig.videoSave<1) m_fileConfig.videoInterval = 1;

	pugi::xml_node UPS = root.child("UPS");
	m_fileConfig.UPSInterval = UPS.child("interval").text().as_uint();

	const char* upsIP = UPS.child("IP").text().as_string();
	m_UPSIPAdress.assign(upsIP);

	m_UPSControlMode = UPS.child("ControlMode").text().as_uint();

	pugi::xml_node FTP = root.child("FTP");

	const char* FTPIP = FTP.child("IP").text().as_string();
	m_FTPIPAddress.Append(FTPIP);

	const char* FTPport = FTP.child("port").text().as_string();
	m_FTPPort.Append(FTPport);

	const char* FTPUsername = FTP.child("username").text().as_string();
	m_FTPUsername.Append(FTPUsername);

	const char* FTPPassword = FTP.child("password").text().as_string();
	m_FTPPassword.Append(FTPPassword);

	const char* FTPRemotepath = FTP.child("remotepath").text().as_string();
	m_remotePath.Append(FTPRemotepath);

	const char* backUpPath = root.child("BackupPath").text().as_string();
	m_BackupFilePath.Append(wxString(backUpPath));



	AddLog(LOG_TYPE_MESSAGE,"DirConfig.xml配置文件读取成功");

	return true;
}

void WRSOCcontrtolPlug::onInitialTimeout(wxTimerEvent& event)
{
	if(m_pDatabase->IsDataBaseConnected())
	{
		m_initialTimer.Stop();


		tagReserveFunPar Par;
		Par.FunCode = RESFUNCODE_QUEREG;
		unsigned int RegStatus;
		Par.pPar = (void*)&RegStatus;
		m_IHost->ReserveFun((void*)&Par); //获取软件注册状态

		auto fullPath = GetWorkPath() + wxT("\\Plugin\\WRSOCcontrtol\\WRSOCcontrtol.dll");
		auto fileTime = wxString::Format(wxT("%I64d"),GetFileModificationTime(fullPath));

		wxString registerStatus;

		if(RegStatus==REGSTATUS_CIVILREG)//已注册状态
		{
			registerStatus = wxT("1001-");
		}
		else
		{
			registerStatus = wxT("1000-");
		}

		fileTime.Prepend(registerStatus);

		writeRegisterInfoToDataBase(fileTime); //将注册信息写入数据库中

		writeH5streamToDataBase();

		Bind(RADARFILE_EVENT,&WRSOCcontrtolPlug::radarFileDetectionHandler,this); //自定义事件绑定

		getRadarType();
		getRadarLocation();
		getRadarcd();
		getClientIPAddress();
		getSMSAlarmInterval();
		getAirConditionerStr();  //获取空调配置信息
		parseAirConditionStr();

		writeAlarmEndTimeToDataBase();

		getVideoNameAndPassword(); 

		if(m_radarType.Right(1) == wxT("0"))
		{
			m_radarMode = SINGLE_POLARIZATION;
		}
		else
		{
			m_radarMode = DUAL_POLARIZATION;
		}

		m_radarType.Trim();
		m_radarType = m_radarType.Left(2);

		radarHandler = CreateThread(nullptr,0,CRadarParser::exec,&radarParser,0,nullptr);  //开启雷达参数文件解析线程

		if(m_radarType.Contains(wxT("SA")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CSARadarParser::exec, &SARadarParser, 0, nullptr);
		}
		else if(m_radarType.Contains(wxT("SB")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CSARadarParser::exec, &SARadarParser, 0, nullptr);
		}
		else if(m_radarType.Contains(wxT("CA")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CSARadarParser::exec, &SARadarParser, 0, nullptr);
		}
		else if(m_radarType.Contains(wxT("CB")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CSARadarParser::exec, &SARadarParser, 0, nullptr);
		}
		else if(m_radarType.Contains(wxT("SC")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CSCRadarParser::exec, &SCRadarParser, 0, nullptr);
		}
		else if(m_radarType.Contains(wxT("CD")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CCDRadarParser::exec, &CDRadarParser, 0, nullptr);
		}
		else if(m_radarType.Contains(wxT("CC")))
		{
			onlineRadarHandler = CreateThread(nullptr, 0, CCCRadarParser::exec, &CCRadarParser, 0, nullptr);
		}
		else{}

		m_RadarFileTimer.Start(30*60*1000);
		logHandler = CreateThread(nullptr, 0, CLogAlarm::alarmFileMonitor, &logAlarm, 0, nullptr);  //开启雷达报警日志文件监控进程

		m_pEnvSocket = m_UDPFactory.GetInstance();  //定时采集温湿度
		if(m_pEnvSocket)
		{
			std::set<UINT> id;
			id.insert(0x11);

			m_pEnvHandler = new CTemperature;
			m_pEnvHandler->setID(id);
			m_pEnvSocket->RegHandle(m_pEnvHandler,CTemperature::processData,CTemperature::processEvent);
			m_pEnvSocket->StartReceive(0,"0.0.0.0",3152,NULL,0,8192,8192,1000000);
			m_pEnvHandler->startTimer(10*1000);
		}


		if(m_SMSHandler.StartCom())
		{
			AddLog(LOG_TYPE_MESSAGE,"串口连接正常");
		}
		else
		{
			AddLog(LOG_TYPE_ERROR,"串口连接异常");	
		}

		m_netService.startReceive();

		if(m_UPSControlMode == UPS_NetControl)  //开启UPS采集线程
		{
			upsHandler = CreateThread(nullptr,0,UPSHandlerSNMP::exec,&m_UPSHandlerSNMP,0,nullptr);
		}
		else if(m_UPSControlMode == UPS_ComControl)
		{
			upsHandler = CreateThread(nullptr,0,UPSHandlerRS232::exec,&m_UPSHandlerRS232,0,nullptr);
		}
		else{}


		TXTparser.setUpdateTime();
		TXTparser.startTimer();  //定时解析离线标定文件

		m_fileConfig.videoDir = GetWorkPath().ToStdString();
		m_fileConfig.videoDir += "\\tomcat\\tomcat\\webapps\\Snapshot";  

		m_DVRLogTimer.Start(10*1000,true);  //定时器，定时到达后登陆视频服务器

		m_ConnectionTimer.Start(60*1000);

		m_uploadPath = GetWorkPath() + wxT("\\upload");
		if(!::wxDirExists(m_uploadPath))
		{
			::wxMkDir(m_uploadPath);
		}

		m_FTPThread = new FTPThread(wxTHREAD_DETACHED);  //FTP 上传服务
		wxASSERT_MSG(m_FTPThread!=NULL,_("FTP上传线程创建失败"));
		if(m_FTPThread)
		{
			if(m_FTPThread->Run() != wxTHREAD_NO_ERROR ) 
			{
				delete m_FTPThread;        
				m_FTPThread = NULL;
				GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP上传线程启动失败");
			}
			else
			{
				GetPlug()->AddLog(LOG_TYPE_MESSAGE,"FTP上传服务线程启动成功");
			}
		}

		::wxMilliSleep(100);

		if(m_FTPThread->IsRunning())
		{
			if(GetPlug()->m_FTPThread->Pause() != wxTHREAD_NO_ERROR)
			{
				::wxMessageBox(wxT("FTP上传服务停止失败"),wxT("报警"));
			}
			else
			{
				GetPlug()->AddLog(LOG_TYPE_MESSAGE,"FTP上传服务停止");
			}

		}

		if(m_Radarcd.Find(wxT("Z9376")) != wxNOT_FOUND)
		{
			m_Generator.startReceive();
			m_Generator.startTimer();
		}
	}
}

void WRSOCcontrtolPlug::onCaptureJPEGTimeout(wxTimerEvent& event)
{

	if(m_fileConfig.videoDir.empty())
	{
		return;
	}

	if(!::wxDirExists(wxString(m_fileConfig.videoDir)))
	{
		//AddLog(LOG_TYPE_ERROR,"视频快照目录不存在，请重新选择目录");
// 		m_fileConfig.videoDir = GetWorkPath().ToStdString();
// 		m_fileConfig.videoDir += "\\tomcat\\tomcat\\webapps\\Snapshot";  
		auto dir = GetWorkPath();

		dir += wxT("\\tomcat");
		::wxMkDir(dir);

		dir += wxT("\\tomcat");
		::wxMkDir(dir);

		dir += wxT("\\webapps");
		::wxMkDir(dir);

		dir += wxT("\\Snapshot");
		::wxMkDir(dir);
	}

	if(m_videoChan.empty())
	{
		return;
	}

	NET_DVR_JPEGPARA JPEGPara;
	JPEGPara.wPicSize = 2;
	JPEGPara.wPicQuality = 0;

	for(auto chan = m_videoChan.cbegin(); chan != m_videoChan.cend(); chan++)
	{
		
		wxDateTime dateTime = wxDateTime::Now();
		auto yearDir = wxString::Format(wxT("%d"),dateTime.GetYear());
		auto monthDir = wxString::Format(wxT("%d"),dateTime.GetMonth() + 1);  //创建目录

		auto partialVideoDir = yearDir;
		partialVideoDir.Append(wxT("/"));
		partialVideoDir.Append(monthDir);
		partialVideoDir.Append(wxT("/"));

		yearDir.Prepend(wxT('/'));
		yearDir.Prepend(wxString(m_fileConfig.videoDir));
		yearDir.Replace(wxT("\\"),wxT("/"));

		monthDir.Prepend(wxT('/'));
		monthDir.Prepend(yearDir);

		if(!::wxDirExists(yearDir))
		{
			if(!::wxMkdir(yearDir))
			{
				AddLog(LOG_TYPE_ERROR,"视频快照年份目录创建失败");
				break;
			}
		}

		if(!::wxDirExists(monthDir))
		{
			if(!::wxMkdir(monthDir))
			{
				AddLog(LOG_TYPE_ERROR,"视频快照月份目录创建失败");
				break;
			}
		}

		wxString videoFullPath;
		videoFullPath = dateTime.Format(wxT("%Y%m%d%H%M%S"));
		videoFullPath.Append(wxString::Format(wxT("_C%d.jpg"),*chan));

		partialVideoDir.Append(videoFullPath);

		videoFullPath.Prepend(wxT('/'));
		videoFullPath.Prepend(monthDir);

		std::string snapShotPath(videoFullPath.ToStdString());

		if(::NET_DVR_CaptureJPEGPicture(m_NET_DVR_USERID,*chan,&JPEGPara,(char*)snapShotPath.c_str()))  //抓取图片
		{
			writeVideoToDataBase(partialVideoDir,*chan);
		}
	}

	removeExcessiveVideo();  //删除过时图片
}


void WRSOCcontrtolPlug::onHDCfgTimeout(wxTimerEvent& event)
{
	ULARGE_INTEGER totalNum = { 0 };
	ULARGE_INTEGER freeNUm = { 0 };
	BOOL diskCHealthFlag  = 1;
	BOOL diskDHealthFlag  = 1;

	GetDiskFreeSpaceExA("C:\\", NULL, &totalNum, &freeNUm);  //判断磁盘剩余大小
	if(freeNUm.QuadPart * 100 / totalNum.QuadPart < 10)
	{
		diskCHealthFlag = 0;
	}

	GetDiskFreeSpaceExA("D:\\", NULL, &totalNum, &freeNUm);
	if(freeNUm.QuadPart * 100 / totalNum.QuadPart < 10)
	{
		diskDHealthFlag = 0;
	
	}

	auto sqlSentence = wxString::Format(wxT("UPDATE radarrunstatus SET disk_status = %d"),diskCHealthFlag & diskDHealthFlag);
	m_pDatabase->RunSQLNoQuery(sqlSentence);

	wxString diskAlarmContent = wxT("标准输出控制器磁盘剩余空间不足");
	if(((diskCHealthFlag == 0) || (diskDHealthFlag == 0)) && !m_DiskExceptionFlag)
	{
		
		m_DiskExceptionEndureTime.first = wxDateTime::GetTimeNow();

		wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
								   endTime, \
								   type, \
								   alarmName, \
								   level, \
								   mode, \
								   content, \
								   origin) VALUES");
		if (m_pDatabase)
		{

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_DiskExceptionEndureTime.first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("主机系统")));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), diskAlarmContent));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


			 m_pDatabase->RunSQLNoQuery(sqlSentence);
			 m_DiskExceptionFlag = TRUE;
		}

	}

	if(((diskCHealthFlag == 1) || (diskDHealthFlag == 1)) && m_DiskExceptionFlag)
	{
		m_DiskExceptionEndureTime.second = wxDateTime::GetTimeNow();

		if (m_pDatabase)
		{

			wxString sqlSentence;
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_DiskExceptionEndureTime.second,
				m_DiskExceptionEndureTime.first,
				diskAlarmContent);

			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_DiskExceptionFlag = FALSE;

		}
	}
 

	auto currentHour = wxDateTime::Now().GetHour();
	if(currentHour % 12 == 0)
	{

		NET_DVR_HDCFG hdconfig;
		DWORD len = 0;

		if(::NET_DVR_GetDVRConfig(m_NET_DVR_USERID,NET_DVR_GET_HDCFG,0xFFFFFFFF,(void*)&hdconfig,sizeof(NET_DVR_HDCFG),&len) == TRUE)  //判断视频服务器磁盘状态
		{
			std::map<wxString,INT> HDInfo;
			HDInfo[wxT("dwHDNo")] = hdconfig.struHDInfo[0].dwHDNo;
			HDInfo[wxT("dwCapacity")] =  hdconfig.struHDInfo[0].dwCapacity;
			HDInfo[wxT("dwFreeSpace")] =  hdconfig.struHDInfo[0].dwFreeSpace;
			HDInfo[wxT("dwHdStatus")] =  hdconfig.struHDInfo[0].dwHdStatus;
			HDInfo[wxT("byHDAttr")] =  hdconfig.struHDInfo[0].byHDAttr;
			HDInfo[wxT("byHDType")] =  hdconfig.struHDInfo[0].byHDType;
			HDInfo[wxT("byDiskDriver ")] =  hdconfig.struHDInfo[0].byDiskDriver;
			HDInfo[wxT("dwHdGroup")] =  hdconfig.struHDInfo[0].dwHdGroup;
			HDInfo[wxT("byRecycling")] =  hdconfig.struHDInfo[0].byRecycling;

			wxString sqlSentence;
			// wxString sqlError;
			// wxMutexLocker locker(*m_sqlQueryLocker);
			if(m_pDatabase)
			{
				// try
				// {
					for(auto item = HDInfo.cbegin(); item != HDInfo.cend(); item++)
					{
						sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%d' WHERE channel='%s'"),item->second,item->first);

						if(m_pDatabase->RunSQLNoQuery(sqlSentence))
						{
							break;
						}
					}		

				// }
				// catch (wxString& error)
				// {
				// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
				// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
				// }
			}
			
		}
	}
}

void WRSOCcontrtolPlug::onDVRLogTimeout(wxTimerEvent& event)
{
	DVRLogout();
	DVRLogin();
}

void WRSOCcontrtolPlug::onRadarFileTimeout(wxTimerEvent& event)
{
	if(m_BackupFilePath.IsEmpty())
	{
		return;
	}


	std::set<wxString> transferFileName;

	wxString dirName(m_fileConfig.radarStatusDir);
	if(dirName.IsEmpty())
	{
		return;
	}
	if(!::wxDirExists(dirName))
	{
		return;
	}
	wxDir fileDir(dirName);
	wxString fullFilePath;

	if ( !fileDir.IsOpened() )  //备份文件
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"文件目录异常");
	}
	else
	{
		wxString fileName;
		bool newFileFlag = false;
		bool flag = fileDir.GetFirst(&fileName, wxEmptyString,wxDIR_FILES );
		while( flag )
		{
			if(GetPlug()->GetExit())
			{
				break;
			}
			fullFilePath = dirName;
			fullFilePath += wxT("\\");
			fullFilePath += fileName;

			auto updateTime = GetPlug()->fileModificationTime(fileName);
			if(updateTime < ::wxDateTime::GetTimeNow() - 24*3600)
			{
				if((fileName.Find(wxT(".bin")) != wxNOT_FOUND) || (fileName.Find(wxT(".dat")) != wxNOT_FOUND) || (fileName.Find(wxT(".txt")) != wxNOT_FOUND))
				{
					transferFileName.insert(fullFilePath);
				}

			}
			flag = fileDir.GetNext(&fileName);
		}

	}

	for(auto item = transferFileName.cbegin(); item != transferFileName.cend(); item++)
	{
		auto yearDir = m_BackupFilePath + wxT("\\") + getTransferFileYear(*item);
		if(!::wxDirExists(yearDir))
		{
			::wxMkDir(yearDir);
		}

		auto monthDir = yearDir + wxT("\\") + getTransferFileMonth(*item);
		if(!::wxDirExists(monthDir))
		{
			::wxMkDir(monthDir);
		}

		auto RadarFileDir = monthDir + wxT("\\radarFile");
		if(!::wxDirExists(RadarFileDir))
		{
			::wxMkDir(RadarFileDir);
		}

		auto TxtFileDir = monthDir + wxT("\\txtFile");
		if(!::wxDirExists(TxtFileDir))
		{
			::wxMkDir(TxtFileDir);
		}

		auto AlarmFileDir = monthDir + wxT("\\alarmFile");
		if(!::wxDirExists(AlarmFileDir))
		{
			::wxMkDir(AlarmFileDir);
		}


		auto Pos = item->Find(wxT('\\'),true) + 1;
		auto fileName = item->Right(item->Length() - Pos);
	 

		if(fileName.Find(wxT("Z_R_DWRN_SRSI")) != wxNOT_FOUND)
		{
			if(::wxDirExists(RadarFileDir))
			{
				auto copyFilePath = RadarFileDir + wxT("\\") + fileName;
				::wxCopyFile(*item,copyFilePath);
				::wxRemoveFile(*item);
			}
			
		}
		else if((fileName.Find(wxT("RadarAlarm")) != wxNOT_FOUND) || (fileName.Find(wxT("Z_A_DWRN_ALM")) != wxNOT_FOUND))
		{
			if(::wxDirExists(AlarmFileDir))
			{
				auto copyFilePath = AlarmFileDir + wxT("\\") + fileName;
				::wxCopyFile(*item,copyFilePath);
				::wxRemoveFile(*item);
			}
		}
		else
		{
			if(::wxDirExists(TxtFileDir))
			{
				auto copyFilePath = TxtFileDir + wxT("\\") + fileName;
				::wxCopyFile(*item,copyFilePath);
				::wxRemoveFile(*item);
			}
		}
	}
	
	
}

void WRSOCcontrtolPlug::onRadarReceiveTimeout(wxTimerEvent& event)
{
	m_netService.reStartReceive();
}

void WRSOCcontrtolPlug::onConnectionTimeout(wxTimerEvent& event)
{
	videoConnectionCheck();
	UPSConnectionCheck();

	INT pingResult = PingRadar();
	writeRadarConnectionToDataBase(pingResult); //将雷达连接状态写入数据库中
	writeRDAConnectionAlarmToDataBase(pingResult);

}

void WRSOCcontrtolPlug::onRadarFileDetectTimeout(wxTimerEvent& event)
{ 
	if((m_RadarFileDetectTimer.GetInterval() == 12*60*1000) || (m_RadarFileDetectTimer.GetInterval() == 20*60*1000))  //根据不同时间段判断文件缺失
	{
		writeRadarFileMissingToDataBase(m_RadarFileDetectTimer.GetInterval()/60/1000);
		m_RadarFileDetectTimer.StartOnce(30*60*1000 - m_RadarFileDetectTimer.GetInterval());
	}
	else if((m_RadarFileDetectTimer.GetInterval() == 18*60*1000) || (m_RadarFileDetectTimer.GetInterval() == 10*60*1000))
	{
		writeRadarFileMissingToDataBase(30);
		m_RadarFileDetectTimer.StartOnce(30*60*1000);
	}
	else 
	{
		writeRadarFileMissingToDataBase(60);
	}
}

void WRSOCcontrtolPlug::videoConnectionCheck()
{
	HANDLE hIcmpFile;
	ULONG ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	INT result = 0;


	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) 
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"invalid ping handle");
		return;
	}  


	inet_pton(AF_INET, m_SysConfig.NetConfig.Video.ip, &ipaddr);
	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);

	if(ReplyBuffer)
	{
		dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),NULL, ReplyBuffer, ReplySize, 2000);
		if(dwRetVal > 0)
		{
			PICMP_ECHO_REPLY replyInfo = static_cast<PICMP_ECHO_REPLY>(ReplyBuffer);
			if(replyInfo)
			{
				if(replyInfo->Status == IP_SUCCESS)
				{
					result = 1;
				}
				else
				{
					result = 0;
				}
				writeVideoConnectionToDataBase(result);
			}
		}
	}

	free(ReplyBuffer);

	wxString videoAlarmContent = wxT("标准输出控制器和视频服务器连接异常");
	if((result == 0) && !m_VideoExceptionFlag)
	{
		m_VideoExceptionEndureTime.first = wxDateTime::GetTimeNow();

		wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
								   endTime, \
								   type, \
								   alarmName, \
								   level, \
								   mode, \
								   content, \
								   origin) VALUES");
		if (m_pDatabase)
		{

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_VideoExceptionEndureTime.first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("设备连接")));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), videoAlarmContent));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_VideoExceptionFlag = TRUE;
		}
	}

	if((result == 1) && m_VideoExceptionFlag)
	{
		m_VideoExceptionEndureTime.second = wxDateTime::GetTimeNow();

		if (m_pDatabase)
		{

			wxString sqlSentence;
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_VideoExceptionEndureTime.second,
				m_VideoExceptionEndureTime.first,
				videoAlarmContent);

			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_VideoExceptionFlag = FALSE;

		}

	}
	IcmpCloseHandle(hIcmpFile);
}

void WRSOCcontrtolPlug::UPSConnectionCheck()
{
	HANDLE hIcmpFile;
	ULONG ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	INT result = 0;


	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) 
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"invalid ping handle");
		return;
	}  


	inet_pton(AF_INET, m_UPSIPAdress.c_str(), &ipaddr);
	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);

	if(ReplyBuffer)
	{
		dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),NULL, ReplyBuffer, ReplySize, 2000);
		if(dwRetVal > 0)
		{
			PICMP_ECHO_REPLY replyInfo = static_cast<PICMP_ECHO_REPLY>(ReplyBuffer);
			if(replyInfo)
			{
				if(replyInfo->Status == IP_SUCCESS)
				{
					result = 1;
				}
				else
				{
					result = 0;
				}
			
			}
		}
	}

	free(ReplyBuffer);

	wxString UPSAlarmContent = wxT("标准输出控制器和UPS设备连接异常");
	if((result == 0) && !m_UPSExceptionFlag)
	{
		m_UPSExceptionEndureTime.first = wxDateTime::GetTimeNow();

		wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
								   endTime, \
								   type, \
								   alarmName, \
								   level, \
								   mode, \
								   content, \
								   origin) VALUES");
		if (m_pDatabase)
		{

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_UPSExceptionEndureTime.first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("设备连接")));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), UPSAlarmContent));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_UPSExceptionFlag = TRUE;
		}
	}

	if((result == 1) && m_UPSExceptionFlag)
	{
		m_UPSExceptionEndureTime.second = wxDateTime::GetTimeNow();

		if (m_pDatabase)
		{

			wxString sqlSentence;
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_UPSExceptionEndureTime.second,
				m_UPSExceptionEndureTime.first,
				UPSAlarmContent);

			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_UPSExceptionFlag = FALSE;

		}

	}
	IcmpCloseHandle(hIcmpFile);
}

int WRSOCcontrtolPlug::cbSQLGetRadarType(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{	
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_radarType = wxString::FromAscii(argv[0]);
	}
	return 0;

}

int WRSOCcontrtolPlug::cbSQLGetRadarcd(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_Radarcd = wxString::FromAscii(argv[0]);
	}
	return 0;
}

int WRSOCcontrtolPlug::cbSQLGetRadarLocation( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName )
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_RadarLocation = wxString(argv[0]);
	}
	return 0;
}

int WRSOCcontrtolPlug::cbSQLGetServerIPAddress(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_ClientIPAddress = std::string(argv[0]);
	}
	return 0;
}

int WRSOCcontrtolPlug::cbSQLGetAirConditionStr(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_airConditionerStr = std::string(argv[0]);
	}
	return 0;
}

int WRSOCcontrtolPlug::cbSQLGetVideoName(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_videoName = std::string(argv[0]);
	}
	return 0;
}

int WRSOCcontrtolPlug::cbSQLGetVideoPassword(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_videoPassword = std::string(argv[0]);
	}
	return 0;
}

int WRSOCcontrtolPlug::cbSQLGetSMSWarnInterval(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	WRSOCcontrtolPlug *pT = static_cast<WRSOCcontrtolPlug*>(pIn);
	if(pT)
	{
		pT->m_SMSAlarmInterval = wxString::FromAscii(argv[0]);
	}
	return 0;
}

void WRSOCcontrtolPlug::getRadarType()
{
	 wxString sqlError;
	if (m_pDatabase)
	{
		char sqlQuery[128] = {'\0'};
		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'radarType'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetRadarType,wxT("content"),this);
	}

}

void WRSOCcontrtolPlug::getRadarcd()
{
	wxString sqlError;

	if (m_pDatabase)
	{
		char sqlQuery[128] = {'\0'};
		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'radarcd'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetRadarcd,wxT("content"),this);
	}


}

void WRSOCcontrtolPlug::getRadarLocation()
{
	wxString sqlError;

	if (m_pDatabase)
	{
		char sqlQuery[128] = {'\0'};
		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'locationName'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetRadarLocation,wxT("content"),this);
	}
}

void WRSOCcontrtolPlug::getAirConditionerStr()
{
	wxString sqlError;

	if (m_pDatabase)
	{
		char sqlQuery[128] = {'\0'};
		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'airConditioner'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetAirConditionStr,wxT("content"),this);
	}
}

void WRSOCcontrtolPlug::parseAirConditionStr()  //解析空调配置信息
{
	int index = -1;

	if(m_airConditionerStr.empty())
	{
		return;
	}

	do
	{
		index++;

		int nextIndex = m_airConditionerStr.find(";",index);
		if(nextIndex == std::string::npos)
		{
			nextIndex = m_airConditionerStr.length();
		}

		auto subStr = m_airConditionerStr.substr(index,nextIndex - index);
		if(subStr.length() < 5)
		{
			continue;
		}
		
		auto indexSubStr = subStr.substr(0,1);
		auto typeSubStr = subStr.substr(2,1);
		auto attributeSubStr = subStr.substr(4,subStr.length() - 4);

		if(std::stoul(typeSubStr) == 2)
		{
			m_ordinaryAirConfiguration.insert(std::make_pair(std::stoul(indexSubStr),std::stoul(attributeSubStr)));
		}
		else if(std::stoul(typeSubStr) == 1)
		{
			m_advancedAirConfiguration.insert(std::make_pair(std::stoul(indexSubStr),std::stoul(attributeSubStr,NULL,16)));
		}
		else{}

	}while((index = m_airConditionerStr.find(";",index)) != std::string::npos);
	
}


void WRSOCcontrtolPlug::getSMSAlarmInterval()
{
	if (m_pDatabase)
	{
		std::string radarType;
		char sqlQuery[128] = {'\0'};
		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'SMSWarnInterval'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetSMSWarnInterval,wxT("content"),this);

		auto linePos = m_SMSAlarmInterval.Find(wxT("-"));
		auto colonPos = m_SMSAlarmInterval.Find(wxT(":"));

		if((linePos != wxNOT_FOUND) && (colonPos != wxNOT_FOUND))
		{
			auto tempStr = m_SMSAlarmInterval.Left(linePos);
			tempStr.ToULong(&m_alarmStartTime);

			tempStr = m_SMSAlarmInterval.Mid(linePos+1, colonPos - linePos - 1);
			tempStr.ToULong(&m_alarmEndTime);

			tempStr = m_SMSAlarmInterval.Right(m_SMSAlarmInterval.Length() - colonPos - 1);
			tempStr.ToLong(&m_alarmLevel);
		}

	}
}

bool WRSOCcontrtolPlug::setSMSAlarmInterval()
{
	m_SMSAlarmInterval = wxString::Format(wxT("%d-%d:%d"),m_alarmStartTime,m_alarmEndTime,m_alarmLevel);
	if(m_pDatabase)
	{
		wxString sqlSentence;

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='SMSWarnInterval'"),m_SMSAlarmInterval);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);
	}
	return false;
}

void WRSOCcontrtolPlug::getVideoNameAndPassword()
{
	if (m_pDatabase)
	{
		std::string radarType;
		char sqlQuery[128] = {'\0'};
		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'videoName'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetVideoName,wxT("content"),this);

		sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'videoPasswd'");	
		m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetVideoPassword,wxT("content"),this);

	}
}

bool WRSOCcontrtolPlug::writeRegisterInfoToDataBase(const wxString& registerNum)
{
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;			
	ZeroMemory(&si,sizeof(STARTUPINFOA));
	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;


	wxFile cmdFile(wxT("register.bat"),wxFile::write);
	if(cmdFile.IsOpened())
	{
		cmdFile.Write(wxString::Format(wxT("\"%s\\tomcat\\jre\\bin\\java\" -jar DesCode.jar  %s > encrypt.txt"),GetPlug()->GetWorkPath(),registerNum));
	}
	cmdFile.Close();

	char cmdLine[] = {"register.bat"};

	if(::CreateProcessA(NULL,
		cmdLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi))
	{
		::WaitForSingleObject(pi.hProcess,INFINITE);
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}
	else
	{
		return false;
	}

	std::string encryptStr;

	std::ifstream file("encrypt.txt");
	if(file.is_open())
	{
		std::getline(file,encryptStr,'\n');
	}

	file.close();
	::wxRemoveFile(wxT("register.bat"));
	::wxRemoveFile(wxT("encrypt.txt"));

	if (m_pDatabase)
	{
		wxString sqlSentence;
		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='SoftwareDeadline'"),wxString(encryptStr));
		return m_pDatabase->RunSQLNoQuery(sqlSentence);
	}

	return false;

}

bool WRSOCcontrtolPlug::writeH5streamToDataBase()
{
	auto H5streamPath = GetWorkPath() + wxT("\\h5stream");

	H5streamPath.Replace(wxT("\\"),wxT("\\\\"));

	if (m_pDatabase)
	{
		wxString sqlSentence;
		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='VideoServerPath'"),H5streamPath);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);

	}
	return false;
}

void WRSOCcontrtolPlug::getClientIPAddress()
{
	wxString sqlError;
	if (m_pDatabase)
	{

			std::string radarType;
			char sqlQuery[128] = {'\0'};
			sprintf_s(sqlQuery,128,"SELECT content FROM systeminfo WHERE channel = 'ClientIPAddress'");	
			m_pDatabase->RunSQLQuery(sqlQuery,cbSQLGetServerIPAddress,wxT("content"),this);
	}
}

BOOL WRSOCcontrtolPlug::writeVideoToDataBase(const wxString& path,UINT chan)
{
	wxString sqlSentence = wxT("INSERT INTO videopicture (updateTime,\
																filePath,\
														        type) VALUES");


	if (m_pDatabase)
	{

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "),::wxDateTime::GetTimeNow()));
			sqlSentence.Append(wxString::Format(wxT("'%s', "),path));
			sqlSentence.Append(wxString::Format(wxT("%d)"), chan));

			bool ret = m_pDatabase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}
	}
	return TRUE;
}

void WRSOCcontrtolPlug::removeExcessiveVideo()
{
	//m_fileConfig.videoDir = "E:\\Work\\国家局大探中心项目\\天气雷达标准输出控制器WRSOC\\WRSOC_Server\\Build\\Debug\\tomcat\\tomcat\\webapps\\Snapshot";

	wxDateTime nowtime = wxDateTime::Now();
	wxDateSpan sp(0,m_fileConfig.videoSave,0);
	wxDateTime EndTime = nowtime-sp;
	wxDateTime filetime = nowtime;

	wxString video_path(wxString(m_fileConfig.videoDir));
	wxDir videoDir(video_path);
	if ( !videoDir.IsOpened() )
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"视频快照文件目录异常");
	}
	else
	{
		wxString year_path;
		bool yearDirflag = videoDir.GetFirst(&year_path, wxEmptyString, wxDIR_DIRS );
		while( yearDirflag )//快照目录遍历
		{
			int year = wxAtoi(year_path);
			if(year>2000 && year<4000 && year_path.length()==4)//满足年格式要求
			{
				filetime.SetYear(year);
				//年目录
				year_path = video_path + wxT("\\") +year_path;
				wxDir yearDir(year_path);
				wxString month_path;
				bool monthDirflag = yearDir.GetFirst(&month_path, wxEmptyString, wxDIR_DIRS );
				while(monthDirflag)//年目录遍历
				{
					int month = wxAtoi(month_path);
					if(month>0 && month<13 && month_path.length()<3)//满足月格式要求
					{
						filetime.SetMonth((wxDateTime::Month)(month-1));
						//月目录
						if(filetime<EndTime)//超出时间范围的月目录
						{
							//遍历删除月目录下文件
							month_path = year_path + wxT("\\") +month_path;
							wxDir monthDir(month_path);
							wxString filename;
							bool fileflag = monthDir.GetFirst(&filename, wxEmptyString, wxDIR_FILES );
							while(fileflag)//月目录遍历删除
							{
								wxRemoveFile(month_path+wxT("\\")+filename);	//删除文件					
								fileflag = monthDir.GetNext(&filename);
							}
							if(monthDir.HasFiles()==false)//判断月目录是否为空
							{
								wxRmdir(month_path);//删除月目录
								AddLogF(LOG_TYPE_MESSAGE,"自动清理快照目录，月目录：%s",(const char*)month_path.mb_str(wxConvLocal));
							}
						}	
					}
					
					monthDirflag = yearDir.GetNext(&month_path);				
				}
				if(yearDir.HasFiles()==false && yearDir.HasSubDirs()==false)//判断年目录是否为空
				{
					wxRmdir(year_path);//删除年目录
					AddLogF(LOG_TYPE_MESSAGE,"自动清理快照目录，年目录：%s",(const char*)year_path.mb_str(wxConvLocal));
				}
			}
			
			yearDirflag = videoDir.GetNext(&year_path);
		}		
	}
}

void WRSOCcontrtolPlug::startJPEGTimer()
{
	m_captureJPEGTimer.Stop();
	m_HDCfgTimer.Stop();
	m_captureJPEGTimer.Start(m_fileConfig.videoInterval * 60 * 1000);
	m_HDCfgTimer.Start(15*60*1000);

}

void WRSOCcontrtolPlug::startDVRLogTimer()
{
	m_DVRLogTimer.Start(3000,true);
}

void WRSOCcontrtolPlug::startRadarReceiveTimer()
{
	m_radarReceiveTimer.Start(2000,true);
}

void WRSOCcontrtolPlug::setradarexetime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node radarexetime = txtFile.child("radarexetime");

	radarexetime.text().set(time);

	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarexetime失败");
	}
}
void WRSOCcontrtolPlug::setradarexefilename(std::string radarfilename)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node radarexefilename = txtFile.child("radarexefilename");

	radarexefilename.text().set(radarfilename.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarexefilename失败");
	}
}

void WRSOCcontrtolPlug::setsunfilename(std::string filename)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node sunfileName = txtFile.child("SunResultdir");

	sunfileName.text().set(filename.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存SunResultdir失败");
	}
}

void WRSOCcontrtolPlug::setreffilename(std::string filename)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node reffileName = txtFile.child("RefCalibResultdir");

	reffileName.text().set(filename.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存RefCalibResultdir失败");
	}
}

void WRSOCcontrtolPlug::setdynfilename(std::string filename)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node dynfileName = txtFile.child("DynTestResultdir");

	dynfileName.text().set(filename.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存DynTestResultdir失败");
	}
}

void WRSOCcontrtolPlug::setnoisefilename(std::string filename)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node noisefileName = txtFile.child("PhaseNoiseResultdir");

	noisefileName.text().set(filename.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存PhaseNoiseResultdir失败");
	}
}

void WRSOCcontrtolPlug::setadaptfilename(std::string filename)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node adaptfileName = txtFile.child("adaptchgdir");

	adaptfileName.text().set(filename.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存adaptchgdir失败");
	}
}

void WRSOCcontrtolPlug::setsoftwaredir(std::string dirname)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node radarexeDir = txtFile.child("radarexeDir");

	radarexeDir.text().set(dirname.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarexeDir失败");
	}
}

void WRSOCcontrtolPlug::setradarstatusdir(std::string dirname)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node radar = root.child("radar");
	pugi::xml_node radarstatusDir = radar.child("radarStatusDir");

	radarstatusDir.text().set(dirname.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarStatusDir失败");
	}
}

void WRSOCcontrtolPlug::setradardatadir(std::string dirname)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node radar = root.child("radar");
	pugi::xml_node radardataDir = radar.child("radarDataDir");

	radardataDir.text().set(dirname.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarDataDir失败");
	}
}

void WRSOCcontrtolPlug::setlogdir(std::string dirname)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtfile = root.child("txtfile");
	pugi::xml_node logfileName = txtfile.child("alarmLogDir");

	logfileName.text().set(dirname.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存alarmLogDir失败");
	}
}

void WRSOCcontrtolPlug::setBackupPathDir(std::string dirname)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node BackupPath = root.child("BackupPath");

	BackupPath.text().set(dirname.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存BackupPath失败");
	}

}

void WRSOCcontrtolPlug::setftpIPAddress(std::string ipAddress)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node ftp = root.child("FTP");
	pugi::xml_node ftpIP = ftp.child("IP");

	ftpIP.text().set(ipAddress.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存ftpIPAddress失败");
	}
}

void WRSOCcontrtolPlug::setftpUsername(std::string username)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node ftp = root.child("FTP");
	pugi::xml_node ftpusername = ftp.child("username");

	ftpusername.text().set(username.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存ftpusername失败");
	}
}

void WRSOCcontrtolPlug::setftpPassword(std::string password)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node ftp = root.child("FTP");
	pugi::xml_node ftppassword = ftp.child("password");

	ftppassword.text().set(password.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存ftppassword失败");
	}
}

void WRSOCcontrtolPlug::setftpRemotepath(std::string remotePath)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node ftp = root.child("FTP");
	pugi::xml_node ftpremotepath = ftp.child("remotepath");

	ftpremotepath.text().set(remotePath.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存ftpremotepath失败");
	}
}

void WRSOCcontrtolPlug::setUPSIPAddress()
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node UPS = root.child("UPS");
	pugi::xml_node IP = UPS.child("IP");

	IP.text().set(m_UPSIPAdress.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存UPSIP失败");
	}
}

void WRSOCcontrtolPlug::setUPSControlMode()
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node UPS = root.child("UPS");
	pugi::xml_node ControlMode = UPS.child("ControlMode");

	ControlMode.text().set(m_UPSControlMode);
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存ControlMode失败");
	}
}

void WRSOCcontrtolPlug::setvideodir(std::string dirname)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node video = root.child("video");
	pugi::xml_node videodir = video.child("videodir");

	videodir.text().set(dirname.c_str());
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存videodir失败");
	}
}

BOOL WRSOCcontrtolPlug::setvideodirToDataBase()
{
	wxString sqlSentence;
	// wxString sqlError;
	// wxMutexLocker locker(*m_sqlQueryLocker);	
	if (m_pDatabase)
	{
		// try
		// {
			wxString path(m_fileConfig.videoDir.c_str(),wxConvLocal);
			path.Replace(wxT("\\"),wxT("/"));
			sqlSentence = wxString::Format(wxT("UPDATE systeminfo SET content = '%s' WHERE channel = 'videoPicPath'"),path);
	
			return m_pDatabase->RunSQLNoQuery(sqlSentence);


		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return FALSE;
}

void WRSOCcontrtolPlug::setvideointerval(UINT interval)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node video = root.child("video");
	pugi::xml_node videointerval = video.child("videointerval");

	videointerval.text().set(interval);
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存videointerval失败");
	}
}

void WRSOCcontrtolPlug::setvideosave(UINT saveyear)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node video = root.child("video");
	pugi::xml_node videosave = video.child("videosave");

	videosave.text().set(saveyear);
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存videosave失败");
	}
}

wxString WRSOCcontrtolPlug::getRadarTypeStr()
{
	auto index = m_radarType.Find(wxT("-"));
	if(index != wxNOT_FOUND)
	{
		return m_radarType.Left(index);
	}

	return m_radarType;
	

}

INT WRSOCcontrtolPlug::PingRadar()
{
	
	HANDLE hIcmpFile;
	ULONG ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	INT result = 0;

	inet_pton(AF_INET, m_SysConfig.NetConfig.Radar.ip, &ipaddr);

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) 
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR,"invalid ping handle");
		return 0;
	}   
	else
	{
		ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
		ReplyBuffer = (VOID*)malloc(ReplySize);

		if(ReplyBuffer)
		{
			dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),NULL, ReplyBuffer, ReplySize, 1000);
			if(dwRetVal > 0)
			{
				PICMP_ECHO_REPLY replyInfo = static_cast<PICMP_ECHO_REPLY>(ReplyBuffer);
				if(replyInfo)
				{
					if(replyInfo->Status == IP_SUCCESS)
					{
						result = 1;
					}
				}
			}

			free(ReplyBuffer);
		}

	}

	IcmpCloseHandle(hIcmpFile);

	return result;
	
}

bool WRSOCcontrtolPlug::sendEnvCmd(const char *pData, UINT Len)
{
	if(m_pEnvSocket && m_pEnvSocket->IsWorking())
	{
		return m_pEnvSocket->SendData(pData,Len,m_SysConfig.NetConfig.Env.ip,m_SysConfig.NetConfig.Env.port);
	}

	return true;
}

BOOL WRSOCcontrtolPlug::DVRLogout()
{
	m_captureJPEGTimer.Stop();
	m_HDCfgTimer.Stop();
	
	return ::NET_DVR_Logout(m_NET_DVR_USERID) && ::NET_DVR_Cleanup();
}

BOOL WRSOCcontrtolPlug::DVRLogin()  
{  
	if(!::NET_DVR_Init())
	{
		AddLog(LOG_TYPE_ERROR,"视频客户端初始化失败");
		::NET_DVR_Cleanup();
		return FALSE;
	}

	::NET_DVR_SetReconnect(1000,1);
	NET_DVR_LOCAL_LOG_CFG logCfg;
	logCfg.wSDKLogNum = 500;
	::NET_DVR_SetSDKLocalCfg(NET_DVR_LOCAL_CFG_TYPE_LOG,(void*)&logCfg);
	::NET_DVR_SetLogToFile(3,".\\Log\\NET_DVR_LOG\\",TRUE);

	NET_DVR_DEVICEINFO_V30 deviceInfo;
	m_NET_DVR_USERID = ::NET_DVR_Login_V30(m_SysConfig.NetConfig.Video.ip,m_SysConfig.NetConfig.Video.port,(char*)m_videoName.c_str(),(char*)m_videoPassword.c_str(),&deviceInfo);  //登录视频服务器
	if(m_NET_DVR_USERID >= 0)
	{
		AddLogF(LOG_TYPE_MESSAGE,"视频服务器登陆成功,数字通道(%d),模拟通道(%d)",deviceInfo.byIPChanNum,deviceInfo.byChanNum);
		if( (deviceInfo.byIPChanNum+deviceInfo.byChanNum)==0)
		{
			AddLogF(LOG_TYPE_WARNING,"无可用视频通道!");
			DVRLogout();
			return FALSE;
		}

		for(auto chan = 0; chan < deviceInfo.byIPChanNum; chan++)
		{
			m_videoChan.insert(deviceInfo.byStartDChan + chan);
		}
		for(auto chan = 0; chan < deviceInfo.byChanNum; chan++)
		{
			m_videoChan.insert(deviceInfo.byStartChan + chan);
		}
	}
	else
	{
		AddLog(LOG_TYPE_ERROR,"视频服务器登陆失败");
		::NET_DVR_Logout(m_NET_DVR_USERID);
		::NET_DVR_Cleanup();
		return FALSE;
	}

	startJPEGTimer();

	/////////////////////////////////////////
// 	HWND hWnd = GetConsoleWindow(); 
// 	NET_DVR_PREVIEWINFO struPlayInfo = {0};
// 	struPlayInfo.hPlayWnd = NULL;
// 
// 	struPlayInfo.lChannel = 34; 
// 	struPlayInfo.dwStreamType = 0;
// 	struPlayInfo.dwLinkMode = 0;
// 	struPlayInfo.bBlocked = 1; 
// 
// 	LONG lRealPlayHandle = ::NET_DVR_RealPlay_V40(m_NET_DVR_USERID, &struPlayInfo, g_RealDataCallBack_V30, NULL);
// 	if(lRealPlayHandle < 0)
// 	{
// 		AddLog(LOG_TYPE_MESSAGE,"realplay fail");
// 	}
	

	return TRUE;
}

wxString WRSOCcontrtolPlug::getTransferFileYear(const wxString& fileName)
{
	auto indexStart = fileName.Find(wxT('_'),true);
	indexStart++;

	auto indexEnd= fileName.Find(wxT('.'));
	auto fileTimeStr = fileName.Mid(indexStart,indexEnd - indexStart);
	auto yearStr = fileTimeStr.Left(4);

	return yearStr;	
}

wxString WRSOCcontrtolPlug::getTransferFileMonth(const wxString& fileName)
{
	auto indexStart = fileName.Find(wxT('_'),true);
	indexStart++;

	auto indexEnd= fileName.Find(wxT('.'));
	auto fileTimeStr = fileName.Mid(indexStart,indexEnd - indexStart);
	auto yearStr = fileTimeStr.Left(4);
	auto monthStr = fileTimeStr.Mid(4,2);

	return monthStr;	
}

void WRSOCcontrtolPlug::writeAlarmEndTimeToDataBase()
{ 
	wxString sqlSentence;
	sqlSentence.Printf(wxT(" UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND origin=0"),wxDateTime::GetTimeNow());

	if (m_pDatabase)
	{
		bool ret = m_pDatabase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}
	}
}

bool WRSOCcontrtolPlug::writeRadarFileMissingToDataBase(INT interval)
{
	wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
							   endTime, \
							   type, \
							   alarmName, \
							   level, \
							   mode, \
							   content, \
							   origin) VALUES");

	if (m_pDatabase)
	{
		sqlSentence.Append(wxString::Format(wxT("(%I64d, "), wxDateTime::GetTimeNow()));
		sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("文件缺失")));
		sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
		sqlSentence.Append(wxString::Format(wxT("%d, "), 1));
		sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));

		wxString content = wxString::Format(wxT("雷达%d分钟没有生成66项关键性能参数文件"),interval);
		sqlSentence.Append(wxString::Format(wxT("'%s', "), content));
		sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


		bool ret =  m_pDatabase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}

	}
	return true;
}

bool WRSOCcontrtolPlug::writeRadarFileMissingEndTimeToDataBase()
{

	if (m_pDatabase)
	{
		wxString sqlSentence;
		sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND type='%s'"), wxDateTime::GetTimeNow() ,wxT("文件缺失"));

		bool ret = m_pDatabase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}

	}

	return true;
}

bool WRSOCcontrtolPlug::writeRDAConnectionAlarmToDataBase(INT result)
{
	wxString radarAlarmContent = wxT("标准输出控制器和雷达RDA电脑连接异常");
	if((result == 0) && !m_RadarExceptionFlag)
	{
		m_RadarExceptionEndureTime.first = wxDateTime::GetTimeNow();

		wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
								   endTime, \
								   type, \
								   alarmName, \
								   level, \
								   mode, \
								   content, \
								   origin) VALUES");
		if (m_pDatabase)
		{

			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_RadarExceptionEndureTime.first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("设备连接")));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), radarAlarmContent));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_RadarExceptionFlag = TRUE;
		}
	}

	if((result == 1) && m_RadarExceptionFlag)
	{
		m_RadarExceptionEndureTime.second = wxDateTime::GetTimeNow();

		if (m_pDatabase)
		{

			wxString sqlSentence;
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_RadarExceptionEndureTime.second,
				m_RadarExceptionEndureTime.first,
				radarAlarmContent);

			m_pDatabase->RunSQLNoQuery(sqlSentence);
			m_RadarExceptionFlag = FALSE;

		}

	}

	return true;
}

bool WRSOCcontrtolPlug::writeRadarConnectionToDataBase(INT result)
{
	wxString sqlSentence = wxString::Format(wxT("UPDATE radarrunstatus SET software_status = %d"),result);
	auto pDataBase = GetPlug()->getDatabase();	
	if (pDataBase)
	{
		return pDataBase->RunSQLNoQuery(sqlSentence);
	}

	return false;
}

bool WRSOCcontrtolPlug::writeEnvConnectionToDataBase(INT result)
{
	wxString sqlSentence = wxString::Format(wxT("UPDATE radarrunstatus SET env_status = %d"),result);
	auto pDataBase = GetPlug()->getDatabase();	
	if (pDataBase)
	{
		return pDataBase->RunSQLNoQuery(sqlSentence);
	}

	return false;
}

bool WRSOCcontrtolPlug::writeVideoConnectionToDataBase(INT result)
{
	wxString sqlSentence = wxString::Format(wxT("UPDATE radarrunstatus SET video_status = %d"),result);
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{
		return pDataBase->RunSQLNoQuery(sqlSentence);
	}

	return false;
}

unsigned int WRSOCcontrtolPlug::chkcrc(unsigned char *buf,unsigned char len)
{
	unsigned char hi,lo; 
	unsigned int i;  
	unsigned int crc;  
	crc=0xFFFF; 
	for(i=0;i<len;i++) 
	{ 
		crc=calccrc(*buf,crc); 
		buf++; 
	} 
	hi=( unsigned char)(crc%256); 
	lo=( unsigned char)(crc/256);
	crc=(((unsigned int)(hi))<<8)|lo; 
	return crc;
}

unsigned int WRSOCcontrtolPlug::calccrc(unsigned char crcbuf,unsigned int crc)
{
	unsigned char i; 
	unsigned char chk; 
	crc=crc ^ crcbuf; 
	for(i=0;i<8;i++) 
	{ 		
		chk=( unsigned char)(crc&1); 
		crc=crc>>1; 
		crc=crc&0x7fff;  
		if (chk==1)
			crc=crc^0xa001;  
		crc=crc&0xffff;  
	}
	return crc; 
}

BOOL WRSOCcontrtolPlug::CCExceptionCheck(float data)
{
	if(std::fabs(data) > CCEXCEPTION)
	{
		return TRUE;
	}
	
	return FALSE;
}

bool WRSOCcontrtolPlug::writeVideoIPAdressToDataBase()
{
	wxString videoIPAddress;

	videoIPAddress.Printf(wxT("%s:%d"),wxString(m_SysConfig.NetConfig.Video.ip,16),m_SysConfig.NetConfig.videoHttpPort);

	wxString sqlSentence;
	if(m_pDatabase)
	{

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='videoIPAddress'"),videoIPAddress);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);	
	}

	return false;

}

bool WRSOCcontrtolPlug::writeUPSAdressToDataBase()
{

	wxString sqlSentence;
	if(m_pDatabase)
	{

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='UPSAddress'"),wxString(m_UPSIPAdress));
		return m_pDatabase->RunSQLNoQuery(sqlSentence);	
	}

	return false;
}

bool WRSOCcontrtolPlug::writeRadarPartialityModeToDataBase(INT mode)
{
	wxString radarMode;
	radarMode.Printf(wxT("%s-%d"),m_radarType,mode);

	wxString sqlSentence;
	if(m_pDatabase)
	{

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='radarType'"),radarMode);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);	
	}

	return false;


}

bool WRSOCcontrtolPlug::writeAirConditionConfigurationToDataBase(const wxString& configuration)
{
	wxString sqlSentence;
	if(m_pDatabase)
	{

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='airConditioner'"),configuration);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);	
	}

	return false;

}

bool WRSOCcontrtolPlug::writeRadarLocationToDataBase()
{
	wxString sqlSentence;
	if(m_pDatabase)
	{

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='locationName'"),m_RadarLocation);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);	
	}

	return false;
}

bool WRSOCcontrtolPlug::writeRadarcdToDataBase()
{
	wxString sqlSentence;
	if(m_pDatabase)
	{

		sqlSentence.Printf(wxT("UPDATE systeminfo SET content='%s' WHERE channel='radarcd'"),m_Radarcd);
		return m_pDatabase->RunSQLNoQuery(sqlSentence);	
	}

	return false;
}

void WRSOCcontrtolPlug::radarFileDetectionHandler(wxCommandEvent& event)
{
	m_RadarFileDetectTimer.Stop();
	
	ULONG interval = 0;
	event.GetString().ToULong(&interval);
	interval *= 60*1000;

	m_RadarFileDetectTimer.StartOnce(interval);

	writeRadarFileMissingEndTimeToDataBase();
	
}

time_t WRSOCcontrtolPlug::GetFileModificationTime(const wxString &filepath)
{
	FILETIME ftCreate, ftModify, ftAccess,localTime;
	HANDLE hFile = CreateFileA((const char*)filepath.ToStdString().c_str(), GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                            // default security
		OPEN_EXISTING,          // existing file only
		FILE_FLAG_BACKUP_SEMANTICS , // normal file
		NULL);



	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
	{
		return -1;
	}
	SYSTEMTIME stLocal;
	FileTimeToLocalFileTime(&ftModify,&localTime);
	FileTimeToSystemTime(&localTime, &stLocal);

	time_t tmpT = GeneralFun::GetTicks(stLocal.wYear, stLocal.wMonth, stLocal.wDay,  stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

	CloseHandle(hFile);

	return tmpT;	
}

time_t WRSOCcontrtolPlug::fileModificationTime(const wxString& fileName)
{
	auto indexStart = fileName.Find(wxT('_'),true);
	indexStart++;

	auto indexEnd= fileName.Find(wxT('.'));

	auto fileTimeStr = fileName.Mid(indexStart,indexEnd - indexStart);

	auto yearStr = fileTimeStr.Left(4);
	auto monthStr = fileTimeStr.Mid(4,2);
	auto dayStr = fileTimeStr.Mid(6,2);
	auto hourStr = fileTimeStr.Mid(8,2);
	auto minStr = fileTimeStr.Mid(10,2);
	auto secStr= fileTimeStr.Mid(12,2);

	ULONG year = 0;
	ULONG month = 0;
	ULONG day = 0;
	ULONG hour = 0;
	ULONG min = 0;
	ULONG sec = 0;

	yearStr.ToULong(&year);
	monthStr.ToULong(&month);
	dayStr.ToULong(&day);
	hourStr.ToULong(&hour);
	minStr.ToULong(&min);
	secStr.ToULong(&sec);

	struct tm fileTime;
	fileTime.tm_year = year - 1900;
	fileTime.tm_mon = month - 1;
	fileTime.tm_mday = day;
	fileTime.tm_hour = hour;
	fileTime.tm_min = min;
	fileTime.tm_sec = sec;

	return std::mktime(&fileTime);	
}

void WRSOCcontrtolPlug::setSunResulttime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node SunResulttime = txtFile.child("SunResulttime");

	SunResulttime.text().set(time);

	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存SunResulttime失败");
	}

}

void WRSOCcontrtolPlug::setrefcalibresulttime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node refcalibresulttime = txtFile.child("RefCalibResulttime");

	refcalibresulttime.text().set(time);
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存refcalibresulttime失败");
	}

}

void WRSOCcontrtolPlug::setdyntestresulttime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node dyntestresulttime = txtFile.child("DynTestResulttime");

	dyntestresulttime.text().set(time);
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存dyntestresulttime失败");
	}

}


void WRSOCcontrtolPlug::setphasenoiseresulttime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node phasenoiseresulttime = txtFile.child("PhaseNoiseResulttime");

	phasenoiseresulttime.text().set(time);
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存phasenoiseresulttime失败");
	}


}

void WRSOCcontrtolPlug::setradarStatustime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node radar = root.child("radar");
	pugi::xml_node radarStatustime = radar.child("radarStatustime");

	radarStatustime.text().set(time);
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarStatustime失败");
	}



}

void WRSOCcontrtolPlug::setalarmtime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node alarmtime = txtFile.child("alarmtime");

	alarmtime.text().set(time);
	wxString fileName = GetWorkPath() + wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存alarmtime失败");
	}
	

}

void WRSOCcontrtolPlug::setadaptchgtime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node adaptchgtime = txtFile.child("adaptchgtime");

	adaptchgtime.text().set(time);
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存adaptchgtime失败");
	}

}

void WRSOCcontrtolPlug::setadaptchgreadsize(unsigned int length)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node txtFile = root.child("txtfile");
	pugi::xml_node adaptchgreadsize = txtFile.child("adaptchgreadsize");

	adaptchgreadsize.text().set(length);
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存adaptchgreadsize失败");
	}

}

void WRSOCcontrtolPlug::setradarDatatime(time_t time)
{
	wxMutexLocker locker(*m_xmlWriteLocker);
	pugi::xml_node root = m_doc.child("config");
	pugi::xml_node radar = root.child("radar");
	pugi::xml_node radarDatatime = radar.child("radarDatatime");

	radarDatatime.text().set(time);
	wxString fileName = GetWorkPath();
	fileName += wxT("\\Config\\DirConfig.xml");
	if(!m_doc.save_file(fileName.wc_str(),"\t",1U,pugi::encoding_utf8))
	{
		GetPlug()->AddLog(LOG_TYPE_MESSAGE, "DirConfig.xml配置文件保存radarDatatime失败");
	}

}


bool WRSOCcontrtolPlug::loadXMLFile()
{
	wxString FileName = GetWorkPath() + wxT("\\Config\\Config.xml");
	try
	{
		ticpp::Document doc;
		doc.LoadFile(FileName.ToStdString());
		ticpp::Element *root = doc.FirstChildElement("Config");

		ticpp::Element *NetConfigEle = root->FirstChildElement("NetConfig");
		if (NetConfigEle != NULL)
		{
			tagNetConfig &NetConfig = m_SysConfig.NetConfig;
			ticpp::Element *localEle = NetConfigEle->FirstChildElement("Local");
			if (localEle != NULL)
			{
				strcpy(NetConfig.Local.ip, localEle->GetAttribute("ip").c_str());
				NetConfig.Local.port = atoi(localEle->GetAttribute("port").c_str());
			}

			ticpp::Element *RadarEle = NetConfigEle->FirstChildElement("Radar");
			if (RadarEle != NULL)
			{
				strcpy(NetConfig.Radar.ip, RadarEle->GetAttribute("ip").c_str());
				NetConfig.Radar.port = atoi(RadarEle->GetAttribute("port").c_str());
			}

			ticpp::Element *EnvEle = NetConfigEle->FirstChildElement("Env");
			if (EnvEle != NULL)
			{
				strcpy(NetConfig.Env.ip, EnvEle->GetAttribute("ip").c_str());
				NetConfig.Env.port = atoi(EnvEle->GetAttribute("port").c_str());
			}

			ticpp::Element *VideoEle = NetConfigEle->FirstChildElement("Video");
			if (VideoEle != NULL)
			{
				strcpy(NetConfig.Video.ip, VideoEle->GetAttribute("ip").c_str());
				NetConfig.Video.port = atoi(VideoEle->GetAttribute("port").c_str());
				NetConfig.videoHttpPort = atoi(VideoEle->GetAttribute("httpPort").c_str());
			}
		}

		ticpp::Element *HumidSensorEle = root->FirstChildElement("HumidSensor");
		if (HumidSensorEle != NULL)
		{
			ticpp::Iterator<ticpp::Element> ProductIt(HumidSensorEle->FirstChildElement("OneHS"));
			int num = 0;
			while (ProductIt != ProductIt.end())
			{
				if(num >= HUMID_SENSOR_MAX_NUM)
					break;
				tagHumidSensor &HumidSensor = m_SysConfig.HumidSensor[num];
				HumidSensor.id = atoi(ProductIt->GetAttribute("id").c_str());
				{
					ticpp::Element *Item0Ele = ProductIt->FirstChildElement("Item0");
					if (Item0Ele != NULL)
					{
						HumidSensor.sensorType[0] = atoi(Item0Ele->GetAttribute("sensorType").c_str());
						HumidSensor.attribute[0] = atoi(Item0Ele->GetAttribute("attribute").c_str());
					}
					ticpp::Element *Item1Ele = ProductIt->FirstChildElement("Item1");
					if (Item1Ele != NULL)
					{
						HumidSensor.sensorType[1] = atoi(Item1Ele->GetAttribute("sensorType").c_str());
						HumidSensor.attribute[1] = atoi(Item1Ele->GetAttribute("attribute").c_str());
					}
				}
				ProductIt++;num++;
			}
		}

		AddLog(LOG_TYPE_MESSAGE,"Config.xml配置文件读取成功");
	}
	catch (ticpp::Exception&)
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR, "读取Config.xml配置文件失败！使用默认配置");
		strcpy_s(m_SysConfig.NetConfig.Local.ip, IP_LENGTH, "127.0.0.1");
		m_SysConfig.NetConfig.Local.port = 9001;
		strcpy_s(m_SysConfig.NetConfig.Radar.ip, IP_LENGTH, "127.0.0.1");
		m_SysConfig.NetConfig.Radar.port = 5000;
		strcpy_s(m_SysConfig.NetConfig.Env.ip, IP_LENGTH, "222.18.149.229");
		m_SysConfig.NetConfig.Env.port = 3152;
		strcpy_s(m_SysConfig.NetConfig.Video.ip, IP_LENGTH, "127.0.0.1");
		m_SysConfig.NetConfig.Video.port = 5005;

		m_SysConfig.HumidSensor[0].id = 2;
		m_SysConfig.HumidSensor[0].sensorType[0] = 8;
		m_SysConfig.HumidSensor[0].sensorType[1] = 8;
		m_SysConfig.HumidSensor[0].attribute[0] = 0;
		m_SysConfig.HumidSensor[0].attribute[1] = 1;

		m_SysConfig.HumidSensor[1].id = 3;
		m_SysConfig.HumidSensor[1].sensorType[0] = 8;
		m_SysConfig.HumidSensor[1].sensorType[1] = 8;
		m_SysConfig.HumidSensor[1].attribute[0] = 0;
		m_SysConfig.HumidSensor[1].attribute[1] = 1;
		return false;
	}
	return true;
}

void WRSOCcontrtolPlug::writeXMLFile()
{
	wxString FileName = GetWorkPath() + wxT("\\Config\\Config.xml");
	wxFile file;
	if (file.Open(FileName, wxFile::write))
	{
		wxString wContent = wxT("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<!-- 传感器类型 0：30H 温度传感器			1：31H 湿度传感器：\n\
					 2：10H 开关量传感器			3：20H 4 - 20mA模拟量传感器\n\
					 4：21H 0 - 20mA模拟量传感器	5：40H HT485温度传感器\n\
					 6：41H HT485湿度传感器		7：42H HT485开关量传感器\n\
					 8：83H Modubus传感器		9：E0H 精密空调\n\
					 10：E1H UPS					11：E3H 开关量扩展模块\n\
					 12：00H 错误传感器值\n\
					 !-- > ");
		auto content = UnicodeToUtf8(wContent.c_str());

		file.Write(content.c_str(),content.length());
		file.Close();
	}
	else
	{
		GetPlug()->AddLog(LOG_TYPE_ERROR, "写配置文件失败！");
		return;
	}
	ticpp::Document doc;
	doc.LoadFile(FileName.ToStdString());
	ticpp::Element root("Config");

	ticpp::Element NetConfigEle("NetConfig");
	ticpp::Element localEle("Local");
	localEle.SetAttribute("ip",m_SysConfig.NetConfig.Local.ip);
	localEle.SetAttribute("port",m_SysConfig.NetConfig.Local.port);
	NetConfigEle.InsertEndChild(localEle);

	ticpp::Element RadarEle("Radar");
	RadarEle.SetAttribute("ip", m_SysConfig.NetConfig.Radar.ip);
	RadarEle.SetAttribute("port", m_SysConfig.NetConfig.Radar.port);
	NetConfigEle.InsertEndChild(RadarEle);

	ticpp::Element EnvEle("Env");
	EnvEle.SetAttribute("ip", m_SysConfig.NetConfig.Env.ip);
	EnvEle.SetAttribute("port", m_SysConfig.NetConfig.Env.port);
	NetConfigEle.InsertEndChild(EnvEle);

	ticpp::Element VideoEle("Video");
	VideoEle.SetAttribute("ip", m_SysConfig.NetConfig.Video.ip);
	VideoEle.SetAttribute("port", m_SysConfig.NetConfig.Video.port);
	VideoEle.SetAttribute("httpPort",m_SysConfig.NetConfig.videoHttpPort);
	NetConfigEle.InsertEndChild(VideoEle);

	ticpp::Element HumidSensorEle("HumidSensor");
	for (int i = 0; i < HUMID_SENSOR_MAX_NUM; i++)
	{
		ticpp::Element Item0Ele("Item0");
		Item0Ele.SetAttribute("sensorType", m_SysConfig.HumidSensor[i].sensorType[0]);
		Item0Ele.SetAttribute("attribute", m_SysConfig.HumidSensor[i].attribute[0]);

		ticpp::Element Item1Ele("Item1");
		Item1Ele.SetAttribute("sensorType", m_SysConfig.HumidSensor[i].sensorType[1]);
		Item1Ele.SetAttribute("attribute", m_SysConfig.HumidSensor[i].attribute[1]);

		ticpp::Element OneHSEle("OneHS");
		OneHSEle.SetAttribute("id", m_SysConfig.HumidSensor[i].id);
		OneHSEle.InsertEndChild(Item0Ele);
		OneHSEle.InsertEndChild(Item1Ele);

		HumidSensorEle.InsertEndChild(OneHSEle);
	}

	root.InsertEndChild(NetConfigEle);
	root.InsertEndChild(HumidSensorEle);
	doc.InsertEndChild(root);
	doc.SaveFile();
}
