/* \file WRSOCcontrtolSet.h
	\brief WRSOCcontrtol参数设置对话框定义
	\ingroup WRSOCcontrtolGroup
	
	本文件定义了WRSOCcontrtolSet参数设置对话框类。

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

#ifndef __HEADER_WRSOCcontrtol_Set_20170904
#define __HEADER_WRSOCcontrtol_Set_20170904

#include "WRSOCcontrtol_gui.h"
#include "wxipctrl.hpp"
#include "RadarLocalController.h"
#include "cdyw/NewInputFilter.h"

using namespace CDYW::PluginSystem;
static const PL_GUID g_guid_Set = { 0x4A5DDEC2, 0x2840, 0x463B, { 0xA4, 0xBF, 0x91, 0x69, 0xDA, 0x81, 0x97, 0x93 } };

/*! \class WRSOCcontrtolSet
	\brief 参数设置对话框类
	\ingroup WRSOCcontrtolGroup
*/
class WRSOCcontrtolSet : 
	public WRSOCcontrtolPanelSet ,
	public IParaPageHelper< WRSOCcontrtolSet, wxWindow >
{
public:
	WRSOCcontrtolSet( wxWindow* parent);

	virtual ~WRSOCcontrtolSet();

	/* 重载自IDlgParapage*/
	void OnApply();
	int OnCheckApply();
	

private:
	void Init();
	void InitVideo();  //初始化视频服务配置
	void InitDir();  //初始化目录信息
	void initRadarController();
	void initSMSInfo();  //初始化短息配置
	void getDir(wxCommandEvent& event);
	void startReceive(wxCommandEvent& event);
	void sendRadarCmd(wxCommandEvent& event);
	void onStatusTimeout(wxTimerEvent& event);
	void clearText(wxCommandEvent& event);
	void FtpService(wxCommandEvent& event);
	void UPSControlModeShift(wxCommandEvent& event);
	BOOL onDirApply();
	BOOL onVideoApply();
	void initFtp();
	void onFTP();
	BOOL onSMSInfo();   //初始化短息配置
	void setRadarPartialityMode();
	void getAirConditionConfiguration();
	void setAirConditionConfiguration();

	void onComboboxAirTypeOneChanged(wxCommandEvent& event);
	void onComboboxAirTypeTwoChanged(wxCommandEvent& event);
	void onComboboxAirTypeThreeChanged(wxCommandEvent& event);
	void onComboboxAirTypeFourChanged(wxCommandEvent& event);
	void onComboboxAirTypeFiveChanged(wxCommandEvent& event);
	void onComboboxAirTypeSixChanged(wxCommandEvent& event);
	void onComboboxAirTypeSevenChanged(wxCommandEvent& event);

	

private:
	int m_SunButtonID;
	int m_RefButtonID;
	int m_DynButtonID;
	int m_PhaseNoiseButtonID;
	int m_SoftwareButtonID;
	int m_AdaptChgButtonID;
	int m_Para67ButtonID;
	int m_OnlineParaButtonID;
	int m_DiaryButtonID;
	int m_BackupButtonID;

	int m_StartReceiveID;
	int m_SendRadarCmdID;
	int m_ClearID;
	int m_FTPServiceID;
	int m_UPSControlModeID;

	int m_comboboxAirTypeID_1;
	int m_comboboxAirTypeID_2;
	int m_comboboxAirTypeID_3;
	int m_comboboxAirTypeID_4;
	int m_comboboxAirTypeID_5;
	int m_comboboxAirTypeID_6;
	int m_comboboxAirTypeID_7;

	CNewInputFliter m_InputFliter;
	void InitInputFilter();

};

#endif

