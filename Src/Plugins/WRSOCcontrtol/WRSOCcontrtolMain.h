
/* \file WRSOCcontrtolMain.h
	\brief WRSOCcontrtol插件类定义
	\ingroup WRSOCcontrtolGroup
	
	本文件定义了WRSOCcontrtolPlug 类。

	\version 1.0
	\author 
	\date 20170904

	\b 修改记录：
	\arg \c version 1.0， <author>，20170904 创建

	\bug 
	
	<b>Copyright (c) 2008-2010,ChengDu YuanWang Science & Technology CO., LTD.
	All rights reserved.</b>

	\ingroup WRSOCcontrtolGroup
*/
#ifndef _HEADER_WRSOCcontrtol_MAIN_20170904
#define _HEADER_WRSOCcontrtol_MAIN_20170904

#define IP_LENGTH 16
#define HUMID_SENSOR_MAX_NUM 2

typedef struct IP_CONFIG
{
	char ip[IP_LENGTH];
	int port;
}tagIPConfig;

typedef struct NET_CONFIG //网络配置结构体定义，包括本地，雷达，环境，视频等信息
{
	tagIPConfig Local;
	tagIPConfig Radar;
	tagIPConfig Env;
	tagIPConfig Video;
	INT videoHttpPort;
}tagNetConfig;

typedef struct HUMID_SENSOR
{
	int id;
	int sensorType[2];
	int attribute[2];
}tagHumidSensor;

typedef struct SYS_CONFIG
{
	tagNetConfig NetConfig;
	tagHumidSensor HumidSensor[HUMID_SENSOR_MAX_NUM];
}tagSysConfig;

struct databaseConfig
{
	std::string hostname;
	std::string user;
	std::string password;
	std::string databasename;	
};

struct FileConfig  //雷达参数文件和离线标定文件的更新时间和路径的结构体定义
{
	std::string SunResultdir;
	time_t SunResulttime;
	std::string RefCalibResultdir;
	time_t RefCalibResulttime;
	std::string DynTestResultdir;
	time_t DynTestResulttime;
	std::string PhaseNoiseResultdir;
	time_t PhaseNoiseResulttime;

	std::string alarmLogDir;
	time_t alarmtime;

	std::string adaptchgdir;
	time_t adaptchgtime;
	UINT adaptchgreadsize;

	std::string radarStatusDir;
	time_t radarStatustime;
	std::string radarDataDir;
	time_t radarDatatime;
	
	std::string radarexeDir;
	time_t radarexetime;
	std::string radarexefilename;

	std::string videoDir;
	UINT videoInterval;//视频保存间隔，分钟
	UINT videoSave;	//视频清理时间，月

	UINT UPSInterval;

};

enum  //UPS查询模式，SNMP协议或串口协议
{
	UPS_NetControl = 0,
	UPS_ComControl
};

enum  //雷达双偏正或但偏正
{
	SINGLE_POLARIZATION = 0,
	DUAL_POLARIZATION
};

enum //空调定义，普通空调或精密空调
{
	NONE = 0,
	ORDINARYAIRCONDITION,
	ADVANCEDAIRCONDITION
};

enum  //雷达文件缺失的时间段定义
{
	NOPROBE = 0,
	FIRSTSTAGE,  //5-10min
	SECONDSTAGE, //30min
	THIRDSTAGE   //60min
};

typedef union
{
	unsigned short crcData;
	unsigned char crcByte[2];
}CRCData;

const FLOAT CCEXCEPTION = 10000.0;
const FLOAT EXCEPTIONDATA = -9999.0;
const double ZEROBIAS = 1e-7;

const INT UNIPOLE = 0;
const INT BIPOLE = 1;

#include <process.h>
#include <cmath>
#include "WRSOCcontrtolToolBar.h"
#include "WRSOCcontrtolMenu.h"
#include "WRSOCcontrtolSet.h"

#include "mysqldatabase.h"
#include "NetService.h"

#include "RadarRunEnvironment.h"
#include "UPSHandlerSNMP.h"
#include "Parser.h"

#include "RadarParser.h"
#include "LogAlarm.h"
#include "SARadarParser.h"
#include "SCRadarParser.h"
#include "CDRadarParser.h"
#include "CCRadarParser.h"
#include "pugixml.hpp"
#include "RadarLocalController.h"
#include <iphlpapi.h>
#include <icmpapi.h>
#include <WS2tcpip.h>
#include "SMSHandler.h"
#include "UPSHandlerSNMP.h"
#include "UPSHandlerRS232.h"
#include "UDP.h"
#include "IProcess.h"
#include "FTPThread.h"
#include "Generator.h"



#define SHOWFUNCNAME()  GetPlug()->AddLog(LOG_TYPE_ERROR,__FUNCTION__)


using namespace CDYW::PluginSystem;

/*! \class WRSOCcontrtolPlug
	\brief WRSOCcontrtol插件。
	\ingroup WRSOCcontrtolGroup
*/
class WRSOCcontrtolPlug : public wxEvtHandler,
	public IPluginHelper< WRSOCcontrtolPlug >,
	public IWRSOCcontrtol
/*	public ITCPSocketNet*/
{
	CDYW::Plugin::ICommon *m_IComm;
	bool m_bExit;
	wxString m_WorkPath;
public:
	// 接口定义
	IHostPlugin *m_IHost;
		// 成员变量定义
		IPluginToolBar m_toolbar;
			IPluginMenu m_menu;
			IPluginDlgPara m_DlgPara;
			IPluginDlgPara m_panel;

// 			virtual void OnReceiveData(const char *pData,unsigned int len,int Socket,const char *IP);
// 
// 
// 			virtual void OnConnected(int Socket,const char *IP);
// 
// 
// 			virtual void OnDisconnected(int Socket);

public:
	WRSOCcontrtolPlug();

	bool OnCommandHandle(PL_WORD wNotifyCode,PL_WORD wID,void * hWndCtl);

	//! 初始化完毕后调用, 插件在此函数中进行UI初始化等操作
	void OnInitialized();
	//! 获取插件的GUID
	PL_PID GetGUID()		{return g_PL_WRSOCcontrtol;}
	//! 获取插件名称
	const char *GetName()	{return ("WRSOCcontrtol");}
	//! 获取插件版本信息
	const char *GetVersion();
	//! 获取作者
	const char *GetAuthor()	{return ("CDYW");}
	//! 程序退出
	void OnExit();
	//! 程序是否退出
	bool GetExit()			{return m_bExit;}
	//! 初始化变量
	void OnActived();
	//! 释放资源
	void OnDestroy();
	//! 获取程序路径
	const wxString GetWorkPath()	{return m_WorkPath;}
		//! 从文件初始化工具栏
	bool InitToolbarData();
	bool InitMenuData(char* pData,int DataSize,wxString FileName);
		void *QueryInterfaceP( PL_IID guid );
	PL_GUID *GetDependenceList( int &cnt ,const char ***ppszDepNames );
	CDYW::Plugin::ICommon *GetComm() { return m_IComm; };
	//////////////////////////////////////////////////////////////////////////

public:
	std::string UnicodeToUtf8(const wchar_t* unicode);
	std::wstring Utf8ToUnicode(const char* utf8Str);
	std::wstring GbkToUnicode(const std::string& strGbk);


public:  
	tagSysConfig* getConfig();  //设置 获取网络配置信息
	void setConfig(tagSysConfig& SysConfig);
private:
	bool loadXMLFile();  //读取 设置config.xml内容
	void writeXMLFile();
	tagSysConfig m_SysConfig;

private:
	IMySqlDB* m_pDatabase;
	wxMutex* m_sqlQueryLocker;
	wxMutex* m_xmlWriteLocker;

	LONG m_NET_DVR_USERID;  //登录视频服务器返回ID号
	std::set<UINT> m_videoChan;  //视频信号频道

	wxTimer m_initialTimer;  //用于模块初始化
	wxTimer m_captureJPEGTimer;  //用于抓取图片
	wxTimer m_HDCfgTimer;  //用于检测磁盘大小
	wxTimer m_DVRLogTimer;  //用于视频服务器登录
	wxTimer m_RadarFileTimer;  //用于雷达文件备份
	wxTimer m_radarReceiveTimer;  //用于网络服务重启
	wxTimer m_ConnectionTimer;  //用于判断周围设备连接情况
	wxTimer m_RadarFileDetectTimer;  //用于判断雷达文件缺失

	//CRadarRunEnvironment radarRunEnvironment;
	CLogAlarm logAlarm;

	CParser TXTparser;
	CRadarParser radarParser;
	CSARadarParser SARadarParser;
	CSCRadarParser SCRadarParser;
	CCDRadarParser CDRadarParser;
	CCCRadarParser CCRadarParser;
	UPSHandlerSNMP m_UPSHandlerSNMP;
	UPSHandlerRS232 m_UPSHandlerRS232;
	CGenerator m_Generator;

	HANDLE logHandler;   //报警日志文件线程句柄
	HANDLE radarHandler; //雷达关键参数文件线程句柄
	HANDLE onlineRadarHandler; //雷达在线监测点线程句柄
	HANDLE envHandler;  //获取环境温湿度线程句柄
	HANDLE upsHandler;  //ups线程句柄

	FileConfig m_fileConfig;  
	databaseConfig m_databaseConfig;
	
	pugi::xml_document m_doc;

	SMSHandler m_SMSHandler;  //发送短信线程句柄

	ISocketNet* m_pEnvSocket;  //环境发送套接字
	IProcess* m_pEnvHandler;  //环境数据处理类

	BOOL m_DiskExceptionFlag; //磁盘连接异常信息
	std::pair<time_t,time_t> m_DiskExceptionEndureTime;

	BOOL m_VideoExceptionFlag;  //视频连接异常信息
	std::pair<time_t,time_t> m_VideoExceptionEndureTime;

	BOOL m_RadarExceptionFlag;  //雷达连接异常信息
	std::pair<time_t,time_t> m_RadarExceptionEndureTime;

	BOOL m_UPSExceptionFlag;  //ups设备连接异常信息
	std::pair<time_t,time_t> m_UPSExceptionEndureTime;

public:
	wxString m_radarType;  //雷达类型 SA SB等
	wxString m_Radarcd; //站台编号 
	wxString m_RadarLocation;  //站台地址
	INT m_radarMode; //雷达偏正类型

	std::string m_ClientIPAddress;  //java端IP地址

	wxString m_BackupFilePath;	//备份文件路径

	CUDP m_UDPFactory;  //UDP 套接字工厂类
	std::string m_UPSIPAdress; //upsIP地址
	INT m_UPSControlMode;  //ups查询方式

	wxString m_FTPIPAddress;
	wxString m_FTPPort;
	wxString m_FTPUsername;
	wxString m_FTPPassword;

	wxString m_uploadPath;
	wxString m_remotePath;

	ICurlModule* m_CurlModule;
	FTPThread* m_FTPThread;

	std::string m_airConditionerStr;  //原始空调配置信息
	std::map<INT,INT> m_ordinaryAirConfiguration;  //普通空调配置信息
	std::map<INT,INT> m_advancedAirConfiguration;  //精密空调配置信息

	CNetService m_netService;

	std::string m_videoName;  //视频登录用户名
	std::string m_videoPassword;  //视频登录密码


	wxString m_SMSAlarmInterval;
	ULONG m_alarmStartTime;
	ULONG m_alarmEndTime;
	LONG m_alarmLevel;

public:
	time_t GetFileModificationTime( const wxString &filepath);

	wxMutex* getSqlQueryMutexLocker() {return m_sqlQueryLocker;}
	IMySqlDB* getDatabase() const {return m_pDatabase;}

	databaseConfig& getdataBaseConfig() {return m_databaseConfig;}
	FileConfig& getfileConfig() {return m_fileConfig;}

	time_t fileModificationTime(const wxString& fileName);
	wxString getRadarTypeStr();

	void setSunResulttime(time_t time);  //这几个函数将文件最新更新时间写入xml中
	void setrefcalibresulttime(time_t time);
	void setdyntestresulttime(time_t time);
	void setphasenoiseresulttime(time_t time);
	void setradarStatustime(time_t time);
	void setalarmtime(time_t time);
	void setadaptchgtime(time_t time);
	void setadaptchgreadsize(unsigned int length);
	void setradarDatatime(time_t time);
	void setradarexetime(time_t time);
	void setradarexefilename(std::string radarfilename);

	void setsunfilename(std::string filename);
	void setreffilename(std::string filename);
	void setdynfilename(std::string filename);
	void setnoisefilename(std::string filename);
	void setadaptfilename(std::string filename);

	void setsoftwaredir(std::string dirname);  //将文件监控目录写入xml中
	void setradarstatusdir(std::string dirname);
	void setradardatadir(std::string dirname);
	void setlogdir(std::string dirname);
	void setBackupPathDir(std::string dirname);

	void setftpIPAddress(std::string ipAddress);
	void setftpUsername(std::string username);
	void setftpPassword(std::string password);
	void setftpRemotepath(std::string remotePath);

	void setUPSIPAddress();
	void setUPSControlMode();

	void setvideodir(std::string dirname);
	BOOL setvideodirToDataBase();
	void setvideointerval(UINT interval);
	void setvideosave(UINT saveyear);
	void startJPEGTimer();
	void startDVRLogTimer();
	void startRadarReceiveTimer();

	INT PingRadar();

	void addSMSData(const std::string& rawData) {m_SMSHandler.addSMSData(rawData);} //添加短信内容
	void sendSMSText() {m_SMSHandler.sendSMSText();}  //发送短信

	bool sendEnvCmd(const char *pData, UINT Len);
	BOOL reStartReceive() {return m_netService.reStartReceive();}

	bool writeRadarConnectionToDataBase(INT result);  //将设备连接状态写入数据库中
	bool writeEnvConnectionToDataBase(INT result);
	bool writeVideoConnectionToDataBase(INT result);

	unsigned int chkcrc(unsigned char *buf,unsigned char len);
	unsigned int calccrc(unsigned char crcbuf,unsigned int crc);

	BOOL CCExceptionCheck(float data);

	bool writeVideoIPAdressToDataBase();
	bool writeUPSAdressToDataBase();

	bool writeRadarPartialityModeToDataBase(INT mode);  //将雷达偏正状态写入数据库中
	bool writeAirConditionConfigurationToDataBase(const wxString& configuration);  //将空调配置信息写入数据库中

	bool writeRadarLocationToDataBase();
	bool writeRadarcdToDataBase();

	bool setSMSAlarmInterval();


#ifdef __TESTWRSOC__
	UINT getState14() {return m_state14;}
	void setState14(UINT state) {m_state14 = state;} 
#endif

private:
	static int cbSQLGetRadarType( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );  //这几个函数是数据库查询回调函数
	static int cbSQLGetRadarcd( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetRadarLocation( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetServerIPAddress( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetAirConditionStr( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetVideoName( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetVideoPassword( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
	static int cbSQLGetSMSWarnInterval( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );

private:
	bool loadDirConfigFile();

	void onInitialTimeout(wxTimerEvent& event);  //每个定时器对应的定时函数
	void onCaptureJPEGTimeout(wxTimerEvent& event);
	void onHDCfgTimeout(wxTimerEvent& event);
	void onDVRLogTimeout(wxTimerEvent& event);
	void onRadarFileTimeout(wxTimerEvent& event);
	void onRadarReceiveTimeout(wxTimerEvent& event);
	void onConnectionTimeout(wxTimerEvent& event);
	void onRadarFileDetectTimeout(wxTimerEvent& event);

	void videoConnectionCheck();  //监测视频主机连接状态
	void UPSConnectionCheck();  //监测UPS链接状态

	void radarFileDetectionHandler(wxCommandEvent& event);  //监测雷达文件缺失

	BOOL writeVideoToDataBase(const wxString& path,UINT chan);
	void removeExcessiveVideo();  //删除过时的抓图图片

	void getClientIPAddress();
	void getRadarType();
	void getRadarcd();
	void getRadarLocation();
	void getAirConditionerStr();
	void parseAirConditionStr();

	void getSMSAlarmInterval();


	void getVideoNameAndPassword();
	bool writeRegisterInfoToDataBase(const wxString& registerNum);
	bool writeH5streamToDataBase();

	BOOL DVRLogout();  //视频主机登录退出
	BOOL DVRLogin();   //视频主机登录

	wxString getTransferFileYear(const wxString& fileName);
	wxString getTransferFileMonth(const wxString& fileName);

	void writeAlarmEndTimeToDataBase();
	bool writeRadarFileMissingToDataBase(INT interval);  //将雷达文件缺失报警写入数据库
	bool writeRadarFileMissingEndTimeToDataBase();       //消除雷达文件缺失报警
	bool writeRDAConnectionAlarmToDataBase(INT result);


#ifdef __TESTWRSOC__
	UINT m_state14;
#endif

	DECLARE_EVENT_TABLE()


};


#endif

