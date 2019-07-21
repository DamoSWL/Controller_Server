/* \file WRSOCcontrtolPlug.cpp
	\brief WRSOCcontrtol 插件实现文件
	\ingroup WRSOCcontrtolGroup
	
	本文件定义了WRSOCcontrtol插件的实体。

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

// PluginTest.cpp : Defines the entry point for the DLL application.
//

#ifdef WX_PRECOMP
	#include "wx_pch.h"
#endif

#include "WRSOCcontrtolMain.h"

static const PL_PID g_guid = g_PL_WRSOCcontrtol;

namespace WRSOCcontrtol
{
	//! 插件测试类实例化
	PluginRegistrant< WRSOCcontrtolPlug > plugin( 
			g_Name_WRSOCcontrtol, 
			g_PL_WRSOCcontrtol 
	);
}

