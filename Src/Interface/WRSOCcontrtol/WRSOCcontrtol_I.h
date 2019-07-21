/* \file IWRSOCcontrtol IWRSOCcontrtol.h
	\brief WRSOCcontrtol对外接口定义文件
	\ingroup PluginGroup
	
	本接口为其它插件提供...功能。

	\version 1.0
	\author 
	\date 20170904

	\b 修改记录：
	\arg \c version 1.0， <author>，20170904 创建

	\bug 
	
	<b>Copyright (c) 2008-2010,ChengDu YuanWang Science & Technology CO., LTD.
	All rights reserved.</b>

	\ingroup WRSOCcontrtolInterface
*/
#ifndef _INTERFACE_WRSOCcontrtol_20170904
#define _INTERFACE_WRSOCcontrtol_20170904

//! 插件的IDd
static const PL_GUID g_PL_WRSOCcontrtol = { 0x375AEAF9, 0x69B4, 0x4749, { 0xA3, 0x34, 0xFF, 0xCB, 0xCD, 0xA6, 0x5F, 0x79 } };

//! 插件的功能接口ID 
static const PL_GUID g_I_WRSOCcontrtol = { 0xC3E1910C, 0x393B, 0x4498, { 0x8D, 0xF1, 0x98, 0xA2, 0xBF, 0x41, 0xED, 0x3E } };

//! WRSOCcontrtol插件d的中文名称
static const char *g_Name_WRSOCcontrtol = "WRSOCcontrtol";

/*! \class IWRSOCcontrtol
	\brief WRSOCcontrtol接口类

	\ingroup WRSOCcontrtolInterface
*/
class IWRSOCcontrtol
{
public:

};

#endif

