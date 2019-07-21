#ifndef _INTERFACE_TCPServer_20120626
#define _INTERFACE_TCPServer_20120626

static const PL_GUID g_PL_TCPServer = { 0x51e4a6c1, 0x4991, 0x4c57, { 0xb2, 0xc, 0x28, 0x6d, 0x2e, 0x3f, 0x1b, 0xbd } };		//插件的ID
static const PL_GUID g_I_TCPServer = { 0x8d13701c, 0xc92c, 0x4451, { 0xa6, 0xec, 0xae, 0x45, 0x62, 0x82, 0xda, 0x77 } };	//插件的功能接口ID 

static PLUGIN_NAME TCPServerName("TCP","服务");
static const char *g_Name_TCPServer = TCPServerName.name;	//TCPServer插件的中文名称

enum UserLogoinStatus
{
	NOLOGOIN	=	-1,		//没有登录
	LOGOINCLOSE	=	0,		//已关闭
	LOGOINFALSE	=	1,		//登录错误，可能用户名密码
	LOGOINREPEAT=	2,		//该用户已经存在
	LOGOINTRUE	=	3		//登录正确	
};


#pragma pack(push, 1)

typedef struct _SENDDATATYPE_
{
	union
	{
		unsigned char cTypeBuf;
		struct
		{	
			unsigned char all:1;			//向所有的客户端发送
			unsigned char Socket:1;		//发送指定的数据到socket
			unsigned char Web:1;			//向所有WEB的客户端发送
			unsigned char iPad:1;		//向所有iPad的客户端发送
			unsigned char UT:1;			//向所有UT的客户端发送
			unsigned char cTmp:3;
		};
	};	
}tagSendDataType;

typedef struct _UserLogoinStatus_
{
	union
	{
		int STBuf;
		struct
		{
			char LogoinStatus;
			char WebSocket;
			tagSendDataType SendUserType;
			char tmp1;
		};
	};	
}tagUserLogoinStatus;
#pragma pack( pop )

class  ITCPSocketNet
{
public:
	virtual void OnReceiveData(const char *pData,unsigned int len,int Socket,const char *IP)=0;//数据接收响应
	virtual void OnConnected(int Socket,const char *IP)=0;		//用户退出登录
	virtual void OnDisconnected(int Socket)=0;	//用户退出登录	
};
class  ITCPInstance
{
public:
	virtual bool StartServer(const char *IP,		/*服务IP地址，为NULL或指向空字符串时不绑定指定IP*/
		const unsigned short Port,					/*绑定服务端口*/
		const unsigned int iMaxNumConnections = 1,	/*允许最大连接数*/
		const bool bNeedPack = false,				/*接收是否需要组包cdyw网络包格式,true 需要组包 */
		const bool bNeedLogoin = false				/*客户端是否需要登录*/
		)=0;
	virtual void StopServer()=0;
	virtual bool IsWork()=0;
	virtual bool SendData(
		const char* pData,					/*缓冲区地址*/ 
		const unsigned int len,				/*发送缓冲区长度*/ 
		int Socket,							/*发送的Socket，非指定的客户端，可以填写0*/ 
		unsigned int iSendUserType=1)=0;	/*发送数据到指定的客服端，默认发送到指定socket中*/
	virtual void RegHandle(ITCPSocketNet *pHandle)=0;	//注册ut命令处理过程
	virtual void DisconnectUser(int Socket,bool bAll=false)=0;				//主动断开指定Socket连接
	virtual void SetLoginSuccess(int Socket,int status)=0;	//设置登录状态，status见enum UserLogoinStatus
};

class ITCPServer
{
public:
	virtual ITCPInstance *GetInstance(
		const unsigned int RecePackSize,		/*接收包最大值*/
		const unsigned int SendPackSize,		/*发送包最大值*/
		const unsigned int ReceBuffSize,		/*接收缓冲区大小*/
		const unsigned int SendBuffSize,			/*发送缓冲区大小*/
		const string InstanceName = ""/*实例名：可为空*/) = 0;					//获取TCP服务实例，可以获取多个
	virtual void ReleaseInstance( ITCPInstance *pInstance) = 0;	//释放TCP服务实例
};

#endif

