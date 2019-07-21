/* \file WRSOCcontrtolToolBar.h
	\brief WRSOCcontrtol工具条定义
	\ingroup WRSOCcontrtolGroup
	
	本文件定义了WRSOCcontrtol工具条。

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
#ifndef __HEADER_MY_TB_20170904
#define __HEADER_MY_TB_20170904

//! 工具栏定义
const char g_ToolBarXML[] ="<?xml version=\"1.0\" encoding=\"gb2312\" ?>"
	"<Plugin name=\"wxPlugin\">"
	"<ToolBar>"
	"<Button name=\"测试1\" pic=\"Pic1.bmp\" id=\"21\" tooltip=\"测试1\" separator=\"0\" />"
	"<Button separator=\"1\" />"
	"<Button name=\"测试2\" pic=\"Pic2.bmp\" id=\"22\" tooltip=\"测试2\" separator=\"0\" />"
	"</ToolBar>"
	"</Plugin>";

static const PL_TB_ID g_guid_toolbar = { 0x49785939, 0xD39A, 0x4FA1, { 0xA4, 0x3D, 0x07, 0x8A, 0x44, 0x7D, 0x28, 0x8D } };

#endif


