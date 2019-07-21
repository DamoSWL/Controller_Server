/* \file IMySqlDB IMySqlDB.h
	\brief MySqlDB对外接口定义文件
	\ingroup PluginGroup
	
	本接口为其它插件提供...功能。

	\version 1.0
	\author 
	\date 20180613

	\b 修改记录：
	\arg \c version 1.0， <author>，20180613 创建

	\bug 
	
	<b>Copyright (c) 2008-2010,ChengDu YuanWang Science & Technology CO., LTD.
	All rights reserved.</b>

	\ingroup MySqlDBInterface
*/
#ifndef _INTERFACE_MySqlDB_20180613
#define _INTERFACE_MySqlDB_20180613

//! 插件的ID
static const PL_GUID g_PL_MySqlDB = { 0x3c3f61d, 0x7b1c, 0x4bf7, { 0x9b, 0x46, 0x5a, 0xe6, 0xdf, 0x5f, 0xb4, 0xe5 } };

//! 插件的功能接口ID 
static const PL_GUID g_I_MySqlDB = { 0xc50a1c36, 0x37e4, 0x4243, { 0x9a, 0xa2, 0x38, 0x46, 0x0, 0x60, 0x71, 0x22 } };


//! MySqlDB插件d的中文名称
static const char *g_Name_MySqlDB = "数据库管理";

/*! \class IMySqlDB
	\brief MySqlDB接口类

	\ingroup MySqlDBInterface
*/

typedef int (*cbSQL)( void *pIn/*传入的参数*/, int argc, char **argv, char **azColName );
class IMySqlDB
{
public:

	virtual BOOL IsDataBaseConnected() = 0;

	virtual bool RunSQLNoQuery(const wxArrayString &SQL)=0;
	virtual bool RunSQLNoQuery(const wxString &SQL)=0;
	virtual bool RunSQLNoQuery(const char* SQL)=0;

	virtual bool RunSQLQuery( const char* szSQL, cbSQL cb,wxString strfilter, void* pIn ) = 0;						//直接调用运行SQL语句
	virtual int RunSQLQueryReturnCont(const char* szSQL)=0;  //只返回查询结果行数 -1为执行错误
};

#endif

