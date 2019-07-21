/* \file ICurlModule ICurlModule.h
	\brief CurlModule对外接口定义文件
	\ingroup PluginGroup
	
	本接口为其它插件提供...功能。

	\version 1.0
	\author 
	\date 20170320

	\b 修改记录：
	\arg \c version 1.0， <author>，20170320 创建

	\bug 
	
	<b>Copyright (c) 2008-2010,ChengDu YuanWang Science & Technology CO., LTD.
	All rights reserved.</b>

	\ingroup CurlModuleInterface
*/
#ifndef _INTERFACE_CurlModule_20170320
#define _INTERFACE_CurlModule_20170320

//! 插件的ID
static const PL_GUID g_PL_CurlModule = { 0x463F376E, 0x847E, 0x4D5E, { 0xA3, 0x9E, 0xC7, 0x93, 0xF7, 0x48, 0x17, 0x81 } };

//! 插件的功能接口ID 
static const PL_GUID g_I_CurlModule = { 0x8F44B91B, 0x65AB, 0x4717, { 0x93, 0xBE, 0xA3, 0x51, 0x74, 0x14, 0x30, 0x60 } };

//! CurlModule插件d的中文名称
static PLUGIN_NAME CurlModuleName("FTP","组件");
static const char *g_Name_CurlModule = CurlModuleName.name;

/*! \class ICurlModule
	\brief CurlModule接口类

	\ingroup CurlModuleInterface
*/

#define  HTTPMEMORY_MAX_FILES 10
struct tagHTTPMemory {
	char *memory;
	size_t size;
};

class ICurlModule
{
public:
	//transmode 为空表示被动模式(passive)，主动模式下可以指定端口，或端口范围，或不指定端口。
	//端口指令告诉远程服务器连接到指定的IP地址。
	//字符串可以是普通的IP地址、主机名、网络接口名称（UNIX下）或只是一个“ - ”符号，以允许库使用您的系统的默认IP地址。
	//默认FTP操作是被动的，因此不会使用端口。
	//该地址后面可以有一个“：”指定一个端口，可选地后跟一个“ - ”来指定一个端口范围。
	//如果指定的端口是0，操作系统将选择一个空闲端口。
	//如果一个范围提供范围内的所有端口不可用，libcurl会报告curle_ftp_port_failed的处理。
	//无效的端口 / 范围设置将被忽略。
	//192.168.1.2 : 32000 - 33000
	//curl.se : 32123
	//注意： 在主动模式下，第一次连接完成后，本地已经创建一个TCP Server 并FTP服务器已经连接上，不传输数据会导致检测会出现问题，传输数据没问题

	virtual int FtpConnectionCheck(const char *szIP, const char* szPort, const char* szUser, const char* szPass, const char* szPath, const char*  transmode = NULL) = 0;// transmode 为空表示被动模式
	virtual int FtpXfer2(const char *szIP, const char* szPort, const char* szUser, const char* szPass, const char* szPath, const char* szFile, const char*  transmode = NULL) = 0;// transmode 为空表示被动模式
	virtual int FtpXferBuff(const char *szIP, const char* szPort, const char* szUser, const char* szPass, const char*szPath, const char* szFileName, const char* pBuff, unsigned int Len, const char*  transmode = NULL) = 0;// transmode 为空表示被动模式
	virtual int FtpGetLastFileList(const char *szIP, const char* szPort, const char* szUser, const char* szPass, const char* szPath, const int&iMaxOutCnt, const char* szMatch, OUT vector<string> &strOufFiles, const char* transmode = NULL)=0;// transmode 为空表示被动模式
	virtual int FtpDownloadFile(const char *szIP, const char* szPort, const char* szUser, const char* szPass, const char* szSrcFile, const char* szDesSaveFile, const char* transmode = NULL)=0;//获取最新的文件
	
	//HTTP
	virtual int HttpPushInMemory(const char* szURL, vector<tagHTTPMemory> &vecHTTPMemory)=0;//返回HTTP的内容，URL必须是GB2312，返回的内容可能是二进制流，UTF8的字符串
	//根据返回的错误信息，查找对应的错误消息
	virtual const string FtpGetErrorMessage(int iErr) = 0;


};

#endif

