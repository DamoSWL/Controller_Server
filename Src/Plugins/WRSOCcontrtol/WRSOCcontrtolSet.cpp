#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "WRSOCcontrtolSet.h"

// static const int STATUSTIMERID = 150;
// BEGIN_EVENT_TABLE(WRSOCcontrtolSet,wxEvtHandler)
// 	EVT_TIMER(STATUSTIMERID,WRSOCcontrtolSet::onStatusTimeout)
// END_EVENT_TABLE()

WRSOCcontrtolSet::WRSOCcontrtolSet(wxWindow* parent) :
	WRSOCcontrtolPanelSet(parent),
	//m_localRadarController(NULL),
	//m_cnt(0),
	m_SunButtonID(0),
	m_RefButtonID(0),
	m_DynButtonID(0),
	m_PhaseNoiseButtonID(0),
	m_SoftwareButtonID(0),
	m_AdaptChgButtonID(0),
	m_Para67ButtonID(0),
	m_OnlineParaButtonID(0),
	m_DiaryButtonID(0),
	m_BackupButtonID(0)
	//m_StatusTimer(this,STATUSTIMERID)
{
	InitInputFilter();
	Init();
	m_notebook->SetSelection(0);
	
}

WRSOCcontrtolSet::~WRSOCcontrtolSet()
{
//	m_StatusTimer.Stop();
// 	if(m_localRadarController)
// 	{
// 		m_localRadarController->stopReceive();
// 	}
	
};

/* 重载自IDlgParapage*/
void WRSOCcontrtolSet::OnApply()
{
	
}

int WRSOCcontrtolSet::OnCheckApply()
{
	tagSysConfig SysConfig;
	memset(&SysConfig, 0, sizeof(tagSysConfig));
	/*SysConfig.HumidSensor[0].id = m_comboBox_HS0_id->GetSelection();
	SysConfig.HumidSensor[1].id = m_comboBox_HS1_id->GetSelection();
	if (SysConfig.HumidSensor[0].id == SysConfig.HumidSensor[1].id)
	{
	wxMessageBox(wxT("温湿度传感器ID不能相同！"), wxT("提示"));
	return -1;
	}
	SysConfig.HumidSensor[0].attribute[0] = m_comboBox_HS0_att0->GetSelection();
	SysConfig.HumidSensor[0].attribute[1] = m_comboBox_HS0_att1->GetSelection();
	if (SysConfig.HumidSensor[0].attribute[0] == SysConfig.HumidSensor[0].attribute[1])
	{
	wxMessageBox(wxT("同一温湿度传感器属性不能相同！"), wxT("提示"));
	return -1;
	}
	SysConfig.HumidSensor[1].attribute[0] = m_comboBox_HS1_att0->GetSelection();
	SysConfig.HumidSensor[1].attribute[1] = m_comboBox_HS1_att1->GetSelection();
	if (SysConfig.HumidSensor[1].attribute[0] == SysConfig.HumidSensor[1].attribute[1])
	{
	wxMessageBox(wxT("同一温湿度传感器属性不能相同！"), wxT("提示"));
	return -1;
	}
	SysConfig.HumidSensor[0].sensorType[0] = m_comboBox_HS0_type0->GetSelection();
	SysConfig.HumidSensor[0].sensorType[1] = m_comboBox_HS0_type1->GetSelection();
	SysConfig.HumidSensor[1].sensorType[0] = m_comboBox_HS1_type0->GetSelection();
	SysConfig.HumidSensor[1].sensorType[1] = m_comboBox_HS1_type1->GetSelection();*/

	tagNetConfig &NetConfig = SysConfig.NetConfig;
	strcpy(NetConfig.Local.ip, m_panel_localIP->GetValue().mb_str(wxConvLocal));
	NetConfig.Local.port = atoi(m_textCtrl_localPort->GetValue().mb_str(wxConvLocal));
	if(NetConfig.Local.port <= 0)
	{
		::wxMessageBox(wxT("端口号输入异常"),wxT("报警"));
		m_textCtrl_localPort->SetSelection(-1,-1);
		return -1;
	}

	strcpy(NetConfig.Radar.ip, m_panel_RadarIP->GetValue().mb_str(wxConvLocal));
	NetConfig.Radar.port = atoi(m_textCtrl_RadarPort->GetValue().mb_str(wxConvLocal));
	if(NetConfig.Radar.port <= 0)
	{
		::wxMessageBox(wxT("端口号输入异常"),wxT("报警"));
		m_textCtrl_RadarPort->SetSelection(-1,-1);
		return -1;
	}

	strcpy(NetConfig.Env.ip, m_panel_envIP->GetValue().mb_str(wxConvLocal));
	NetConfig.Env.port = atoi(m_textCtrl_envPort->GetValue().mb_str(wxConvLocal));
	if(NetConfig.Env.port <= 0)
	{
		::wxMessageBox(wxT("端口号输入异常"),wxT("报警"));
		m_textCtrl_envPort->SetSelection(-1,-1);
		return -1;
	}

	strcpy(NetConfig.Video.ip, m_panel_videoIP->GetValue().mb_str(wxConvLocal));
	NetConfig.Video.port = atoi(m_textCtrl_videoPort->GetValue().mb_str(wxConvLocal));
	if(NetConfig.Video.port <= 0)
	{
		::wxMessageBox(wxT("端口号输入异常"),wxT("报警"));
		m_textCtrl_videoPort->SetSelection(-1,-1); 
		return -1;
	}

	NetConfig.videoHttpPort = atoi(m_textCtrl_VideoHttpPort->GetValue().mb_str(wxConvLocal));
	if(NetConfig.videoHttpPort <= 0)
	{
		::wxMessageBox(wxT("端口号输入异常"),wxT("报警"));
		m_textCtrl_VideoHttpPort->SetSelection(-1,-1); 
		return -1;
	}

	auto tmpConfig = GetPlug()->getConfig();
	BOOL videoAlteredFlag = FALSE;
	if((strcmp(tmpConfig->NetConfig.Video.ip,NetConfig.Video.ip) != 0) || (tmpConfig->NetConfig.Video.port != NetConfig.Video.port))
	{
		videoAlteredFlag = TRUE;
	}

	if(tmpConfig->NetConfig.Local.port != NetConfig.Local.port)
	{
		GetPlug()->startRadarReceiveTimer();
	}


	GetPlug()->m_RadarLocation = m_textCtrl_siteName->GetValue();
	GetPlug()->m_Radarcd = m_textCtrl_siteIndex->GetValue();
	GetPlug()->writeRadarLocationToDataBase();  //将雷达站址信息写入数据库中
	GetPlug()->writeRadarcdToDataBase();
	

	GetPlug()->setConfig(SysConfig);  //设置网络IP配置

	GetPlug()->writeVideoIPAdressToDataBase();

	GetPlug()->m_UPSIPAdress = m_panel_upsIP->GetValue();
	GetPlug()->setUPSIPAddress();  //设置UPS信息
	GetPlug()->writeUPSAdressToDataBase();

	GetPlug()->m_UPSControlMode = m_comboBox_upsControl->GetSelection();
	GetPlug()->setUPSControlMode();

	setRadarPartialityMode();  //设置雷达偏振信息

	if(videoAlteredFlag == TRUE)
	{
		GetPlug()->startDVRLogTimer();
	}

	if(!onDirApply())
	{
		return EXIT_FAILURE;
	}

	if(!onVideoApply())
	{
		return EXIT_FAILURE;
	}

	if(!onSMSInfo())
	{
		return EXIT_FAILURE;
	}

	setAirConditionConfiguration();
	onFTP();

	return EXIT_SUCCESS;
}

void WRSOCcontrtolSet::Init()
{
	tagSysConfig *SysConfig = GetPlug()->getConfig();

	tagNetConfig &NetConfig = SysConfig->NetConfig;
	if (!m_panel_localIP->SetValue(wxString(NetConfig.Local.ip, wxConvLocal)))
		GetPlug()->AddLog(LOG_TYPE_ERROR, "本地网络IP不符合规定，使用默认IP");
	m_textCtrl_localPort->SetValue(wxString::Format(wxT("%d"), NetConfig.Local.port));
	if (!m_panel_RadarIP->SetValue(wxString(NetConfig.Radar.ip, wxConvLocal)))
		GetPlug()->AddLog(LOG_TYPE_ERROR, "本地网络IP不符合规定，使用默认IP");
	m_textCtrl_RadarPort->SetValue(wxString::Format(wxT("%d"), NetConfig.Radar.port));
	if (!m_panel_envIP->SetValue(wxString(NetConfig.Env.ip, wxConvLocal)))
		GetPlug()->AddLog(LOG_TYPE_ERROR, "本地网络IP不符合规定，使用默认IP");
	m_textCtrl_envPort->SetValue(wxString::Format(wxT("%d"), NetConfig.Env.port));
	if (!m_panel_videoIP->SetValue(wxString(NetConfig.Video.ip, wxConvLocal)))
		GetPlug()->AddLog(LOG_TYPE_ERROR, "本地网络IP不符合规定，使用默认IP");
	m_textCtrl_videoPort->SetValue(wxString::Format(wxT("%d"), NetConfig.Video.port));
	m_textCtrl_VideoHttpPort->SetValue(wxString::Format(wxT("%d"), NetConfig.videoHttpPort));

	//tagHumidSensor &HumidSensor0 = SysConfig->HumidSensor[0];
	//m_comboBox_HS0_id->SetSelection(HumidSensor0.id);
	//m_comboBox_HS0_type0->SetSelection(HumidSensor0.sensorType[0]);
	//m_comboBox_HS0_att0->SetSelection(HumidSensor0.attribute[0]);
	//m_comboBox_HS0_type1->SetSelection(HumidSensor0.sensorType[1]);
	//m_comboBox_HS0_att1->SetSelection(HumidSensor0.attribute[1]);

	//tagHumidSensor &HumidSensor1 = SysConfig->HumidSensor[1];
	//m_comboBox_HS1_id->SetSelection(HumidSensor1.id);
	//m_comboBox_HS1_type0->SetSelection(HumidSensor1.sensorType[0]);
	//m_comboBox_HS1_att0->SetSelection(HumidSensor1.attribute[0]);
	//m_comboBox_HS1_type1->SetSelection(HumidSensor1.sensorType[1]);
	//m_comboBox_HS1_att1->SetSelection(HumidSensor1.attribute[1]);

	if(GetPlug()->m_radarMode == SINGLE_POLARIZATION)
	{
		m_comboBox_partialMode->SetSelection(0);
	}
	else
	{
		m_comboBox_partialMode->SetSelection(1);
	}
	
	m_textCtrl_siteName->SetValue(GetPlug()->m_RadarLocation);
	m_textCtrl_siteIndex->SetValue(GetPlug()->m_Radarcd);


	m_panel_upsIP->SetValue(wxString(GetPlug()->m_UPSIPAdress));
	m_comboBox_upsControl->SetSelection(GetPlug()->m_UPSControlMode);

	if(m_comboBox_upsControl->GetSelection() == UPS_NetControl)
	{
		m_textCtrl_upsPort->SetValue(wxT("161"));
	}
	else if(m_comboBox_upsControl->GetSelection() == UPS_ComControl)
	{
		m_textCtrl_upsPort->SetValue(wxT("23"));
	}
	else{}

	getAirConditionConfiguration();
	InitDir();
	InitVideo();
	initRadarController();
	initFtp();
	initSMSInfo();

}

void WRSOCcontrtolSet::InitVideo()
{
	const FileConfig& fileConfig = GetPlug()->getfileConfig();
	m_textCtrl_videointerval->SetValue(wxString::Format(wxT("%d"),fileConfig.videoInterval));
	m_textCtrl_videosavemonth->SetValue(wxString::Format( wxT("%d"),fileConfig.videoSave));
}

void WRSOCcontrtolSet::InitDir()
{
	m_SunButtonID = m_button_sun->GetId();
	m_RefButtonID = m_button_ref->GetId();
	m_DynButtonID = m_button_dyn->GetId();
	m_PhaseNoiseButtonID = m_button_noise->GetId();
	m_Para67ButtonID = m_button_67data->GetId();
	m_DiaryButtonID = m_button_diary->GetId();
	m_BackupButtonID = m_button_backUp->GetId();
	m_UPSControlModeID = m_comboBox_upsControl->GetId();

	m_comboboxAirTypeID_1 = m_comboBox_airType1->GetId();
	m_comboboxAirTypeID_2 = m_comboBox_airType2->GetId();
	m_comboboxAirTypeID_3 = m_comboBox_airType3->GetId();
	m_comboboxAirTypeID_4 = m_comboBox_airType4->GetId();
	m_comboboxAirTypeID_5 = m_comboBox_airType5->GetId();
	m_comboboxAirTypeID_6 = m_comboBox_airType6->GetId();
	m_comboboxAirTypeID_7 = m_comboBox_airType7->GetId();


	auto fileConfig = GetPlug()->getfileConfig();
	m_textCtrl_sun->SetValue(wxString(fileConfig.SunResultdir));
	m_textCtrl_ref->SetValue(wxString(fileConfig.RefCalibResultdir));
	m_textCtrl_dyn->SetValue(wxString(fileConfig.DynTestResultdir));
	m_textCtrl_noise->SetValue(wxString(fileConfig.PhaseNoiseResultdir));
	m_textCtrl_67data->SetValue(wxString(fileConfig.radarStatusDir));
	m_textCtrl_diary->SetValue(wxString(fileConfig.alarmLogDir));
	m_textCtrl_backup->SetValue(GetPlug()->m_BackupFilePath);

	Connect(m_SunButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_RefButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_DynButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_PhaseNoiseButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_Para67ButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_DiaryButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_BackupButtonID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::getDir));
	Connect(m_UPSControlModeID, wxEVT_COMBOBOX , wxCommandEventHandler(WRSOCcontrtolSet::UPSControlModeShift));

	Connect(m_comboboxAirTypeID_1, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeOneChanged));
	Connect(m_comboboxAirTypeID_2, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeTwoChanged));
	Connect(m_comboboxAirTypeID_3, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeThreeChanged));
	Connect(m_comboboxAirTypeID_4, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeFourChanged));
	Connect(m_comboboxAirTypeID_5, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeFiveChanged));
	Connect(m_comboboxAirTypeID_6, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeSixChanged));
	Connect(m_comboboxAirTypeID_7, wxEVT_COMBOBOX, wxCommandEventHandler(WRSOCcontrtolSet::onComboboxAirTypeSevenChanged));

}

void WRSOCcontrtolSet::initRadarController()
{
}

void WRSOCcontrtolSet::initSMSInfo()
{
	m_spinCtrl_startTime->SetValue(GetPlug()->m_alarmStartTime);
	m_spinCtrl_endTime->SetValue(GetPlug()->m_alarmEndTime);
	m_comboBox_alarmLevel->SetSelection(GetPlug()->m_alarmLevel + 1);
}

void WRSOCcontrtolSet::getDir(wxCommandEvent& event)
{
	wxString path;
	wxDirDialog dialog(this,wxT("选择文件路径"),wxT("."));
	dialog.Center();
	if(dialog.ShowModal() == wxID_OK)
	{
		path = dialog.GetPath();
		auto id = event.GetId();
		
		if(id == m_SunButtonID)
		{
			m_textCtrl_sun->SetValue(path);
		}
		else if(id == m_RefButtonID)
		{
			m_textCtrl_ref->SetValue(path);
		}
		else if(id == m_DynButtonID)
		{
			m_textCtrl_dyn->SetValue(path);
		}
		else if(id == m_PhaseNoiseButtonID)
		{
			m_textCtrl_noise->SetValue(path);
		}
		else if(id == m_AdaptChgButtonID)
		{
//			m_textCtrl_adapt->SetValue(path);
		}
		if(id == m_Para67ButtonID)
		{
			m_textCtrl_67data->SetValue(path);
		}
		else if(id == m_OnlineParaButtonID)
		{
//			m_textCtrl_onlinedata->SetValue(path);
		}
		else if(id == m_DiaryButtonID)
		{
			m_textCtrl_diary->SetValue(path);
		}
		else if(id == m_SoftwareButtonID)
		{
//			m_textCtrl_software->SetValue(path);
		}
		else if(id == m_BackupButtonID)
		{
			m_textCtrl_backup->SetValue(path);
		}

		else{}	

	}
}


void WRSOCcontrtolSet::startReceive(wxCommandEvent& event)
{



}

void WRSOCcontrtolSet::sendRadarCmd(wxCommandEvent& event)
{

	
}

BOOL WRSOCcontrtolSet::onDirApply()
{

	FileConfig& fileConfig = GetPlug()->getfileConfig();

	auto tmpDir = m_textCtrl_sun->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("太阳法测试结果文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}
	}

	fileConfig.SunResultdir = tmpDir.ToStdString();

	tmpDir = m_textCtrl_ref->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("反射率标定结果文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}

	}

	fileConfig.RefCalibResultdir = tmpDir.ToStdString();


	tmpDir = m_textCtrl_dyn->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("动态范围测试结果文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}
	}
	fileConfig.DynTestResultdir = tmpDir.ToStdString();


	tmpDir = m_textCtrl_noise->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("相位噪音测试结果文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}

	}

	fileConfig.PhaseNoiseResultdir = tmpDir.ToStdString();
	fileConfig.adaptchgdir = tmpDir.ToStdString();
	fileConfig.radarexeDir = tmpDir.ToStdString();



	tmpDir = m_textCtrl_67data->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("雷达67项关键参数文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}

	}

	fileConfig.radarStatusDir = tmpDir.ToStdString();
	fileConfig.radarDataDir =  fileConfig.radarStatusDir;



	tmpDir = m_textCtrl_diary->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("日志文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}

	}

	fileConfig.alarmLogDir = tmpDir.ToStdString();


	tmpDir = m_textCtrl_backup->GetValue();
	if(!tmpDir.IsEmpty())
	{
		if(!::wxDirExists(tmpDir))
		{
			::wxMessageBox(wxT("备份文件目录不存在，请另外选择目录"),wxT("报警"));
			return FALSE;
		}

	}

	GetPlug()->m_BackupFilePath = tmpDir;


	GetPlug()->setsunfilename(fileConfig.SunResultdir);
	GetPlug()->setreffilename(fileConfig.RefCalibResultdir);
	GetPlug()->setdynfilename(fileConfig.DynTestResultdir);
	GetPlug()->setnoisefilename(fileConfig.PhaseNoiseResultdir);
	GetPlug()->setradarstatusdir(fileConfig.radarStatusDir);
	GetPlug()->setradardatadir(fileConfig.radarDataDir);
	GetPlug()->setlogdir(fileConfig.alarmLogDir);
	GetPlug()->setBackupPathDir(GetPlug()->m_BackupFilePath.ToStdString());
	GetPlug()->setvideodirToDataBase();

	return TRUE;
}

BOOL WRSOCcontrtolSet::onVideoApply()
{
	FileConfig& fileConfig = GetPlug()->getfileConfig();
	LONG interval = 0;
	m_textCtrl_videointerval->GetValue().ToLong(&interval);
	LONG saveYear = 0;
	m_textCtrl_videosavemonth->GetValue().ToLong(&saveYear);

	if(interval <= 0)
	{
		::wxMessageBox(wxT("视频快照采样时间小于零，请重新设置"),wxT("报警"));
		return FALSE;
	}
	if(saveYear <= 0)
	{
		::wxMessageBox(wxT("视频快照保存时间小于零，请重新设置"),wxT("报警"));
		return FALSE;
	}

	fileConfig.videoInterval = interval;
	GetPlug()->setvideointerval(interval);
	fileConfig.videoSave = saveYear;
	GetPlug()->setvideosave(saveYear);
	GetPlug()->startJPEGTimer();
	
	return TRUE;

}

void WRSOCcontrtolSet::initFtp()
{
	m_panel_FTPIP->SetValue(GetPlug()->m_FTPIPAddress);
	m_textCtrl_FTPPort->SetValue(GetPlug()->m_FTPPort);
	m_textCtrl_FTPUsername->SetValue(GetPlug()->m_FTPUsername);
	m_textCtrl_FTPPassword->SetValue(GetPlug()->m_FTPPassword);
	m_textCtrl_FTPLocalpath->SetValue(GetPlug()->m_uploadPath);
	m_textCtrl_FTPRemotepath->SetValue(GetPlug()->m_remotePath);

	m_FTPServiceID = m_button_FTPService->GetId();

	Connect(m_FTPServiceID,wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WRSOCcontrtolSet::FtpService));


}

void WRSOCcontrtolSet::onFTP()
{
	GetPlug()->m_FTPIPAddress = m_panel_FTPIP->GetValue();
	GetPlug()->m_FTPUsername = m_textCtrl_FTPUsername->GetValue();
	GetPlug()->m_FTPPassword = m_textCtrl_FTPPassword->GetValue();
	GetPlug()->m_remotePath = m_textCtrl_FTPRemotepath->GetValue();

	GetPlug()->setftpIPAddress(GetPlug()->m_FTPIPAddress.ToStdString());
	GetPlug()->setftpUsername(GetPlug()->m_FTPUsername.ToStdString());
	GetPlug()->setftpPassword(GetPlug()->m_FTPPassword.ToStdString());
	GetPlug()->setftpRemotepath(GetPlug()->m_remotePath.ToStdString());
}

BOOL WRSOCcontrtolSet::onSMSInfo()
{
	auto alarmStartTime = m_spinCtrl_startTime->GetValue();
	auto alarmEndTime = m_spinCtrl_endTime->GetValue();
	auto alarmLevel = m_comboBox_alarmLevel->GetCurrentSelection() - 1;

	if(alarmStartTime >= alarmEndTime )
	{
		::wxMessageBox(wxT("短信告警时间异常，请重新设置"),wxT("报警"));
		return FALSE;
	}


	GetPlug()->m_alarmStartTime = alarmStartTime;
	GetPlug()->m_alarmEndTime = alarmEndTime;
	GetPlug()->m_alarmLevel = alarmLevel;

	GetPlug()->setSMSAlarmInterval();

	return TRUE;
}

void WRSOCcontrtolSet::setRadarPartialityMode()
{
	auto index = m_comboBox_partialMode->GetSelection();
	switch(index)
	{
	case SINGLE_POLARIZATION:
		GetPlug()->m_radarMode = SINGLE_POLARIZATION;
		GetPlug()->writeRadarPartialityModeToDataBase(SINGLE_POLARIZATION);
		break;
	case DUAL_POLARIZATION:
		GetPlug()->m_radarMode = DUAL_POLARIZATION;
		GetPlug()->writeRadarPartialityModeToDataBase(DUAL_POLARIZATION);
		break;
	default:
		break;
	}
}


void WRSOCcontrtolSet::getAirConditionConfiguration()
{
	
	for(auto item = GetPlug()->m_ordinaryAirConfiguration.cbegin(); item != GetPlug()->m_ordinaryAirConfiguration.cend(); item++)
	{
		switch(item->first)
		{
		case 1:
			m_comboBox_airType1->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel1->SetSelection(item->second - 1);
			m_comboBox_airChannel1->Enable(true);
			break;
		case 2:
			m_comboBox_airType2->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel2->SetSelection(item->second - 1);
			m_comboBox_airChannel2->Enable(true);
			break;
		case 3:
			m_comboBox_airType3->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel3->SetSelection(item->second - 1);
			m_comboBox_airChannel3->Enable(true);
			break;
		case 4:
			m_comboBox_airType4->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel4->SetSelection(item->second - 1);
			m_comboBox_airChannel4->Enable(true);
			break;
		case 5:
			m_comboBox_airType5->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel5->SetSelection(item->second - 1);
			m_comboBox_airChannel5->Enable(true);
			break;
		case 6:
			m_comboBox_airType6->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel6->SetSelection(item->second - 1);
			m_comboBox_airChannel6->Enable(true);
			break;
		case 7:
			m_comboBox_airType7->SetSelection(ORDINARYAIRCONDITION);
			m_comboBox_airChannel7->SetSelection(item->second - 1);
			m_comboBox_airChannel7->Enable(true);
			break;
		default:
			break;

		}
	}

	for(auto item = GetPlug()->m_advancedAirConfiguration.cbegin(); item != GetPlug()->m_advancedAirConfiguration.cend(); item++)  //从界面将空调配置信息组成字符串
	{
		switch(item->first)
		{
		case 1:
			m_comboBox_airType1->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice1->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice1->Enable(true);
			break;
		case 2:
			m_comboBox_airType2->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice2->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice2->Enable(true);
			break;
		case 3:
			m_comboBox_airType3->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice3->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice3->Enable(true);
			break;
		case 4:
			m_comboBox_airType4->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice4->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice4->Enable(true);
			break;
		case 5:
			m_comboBox_airType5->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice5->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice5->Enable(true);
			break;
		case 6:
			m_comboBox_airType6->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice6->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice6->Enable(true);
			break;
		case 7:
			m_comboBox_airType7->SetSelection(ADVANCEDAIRCONDITION);
			m_textCtrl_airDevice7->SetValue(wxString::Format(wxT("%02X"),item->second));
			m_textCtrl_airDevice7->Enable(true);
			break;
		default:
			break;

		}
	}



}

void WRSOCcontrtolSet::setAirConditionConfiguration()  //根据空调配置信息绘制界面
{
	GetPlug()->m_ordinaryAirConfiguration.clear();
	GetPlug()->m_advancedAirConfiguration.clear();

	auto index = m_comboBox_airType1->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(1,m_comboBox_airChannel1->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice1->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(1,address));
	}
	else{}

	index = m_comboBox_airType2->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(2,m_comboBox_airChannel2->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice2->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(2,address));
	}
	else{}

	index = m_comboBox_airType3->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(3,m_comboBox_airChannel3->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice3->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(3,address));
	}
	else{}

	index = m_comboBox_airType4->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(4,m_comboBox_airChannel4->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice4->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(4,address));
	}
	else{}

	index = m_comboBox_airType5->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(5,m_comboBox_airChannel5->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice5->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(5,address));
	}
	else{}

	index = m_comboBox_airType6->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(6,m_comboBox_airChannel6->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice6->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(6,address));
	}
	else{}

	index = m_comboBox_airType7->GetSelection();
	if(index == ORDINARYAIRCONDITION)
	{
		GetPlug()->m_ordinaryAirConfiguration.insert(std::make_pair(7,m_comboBox_airChannel7->GetSelection() + 1));
	}
	else if(index == ADVANCEDAIRCONDITION)
	{
		auto tmp = m_textCtrl_airDevice7->GetValue();
		long address  = 0;
		tmp.ToLong(&address,16);
		GetPlug()->m_advancedAirConfiguration.insert(std::make_pair(7,address));
	}
	else{}

	wxString ordinaryAirStr;
	wxString advancedAirStr;
	wxString totalStr;

	for(auto item = GetPlug()->m_ordinaryAirConfiguration.cbegin(); item != GetPlug()->m_ordinaryAirConfiguration.cend(); item++)
	{
		ordinaryAirStr.Append(wxString::Format(wxT("%d:2:%d"),item->first,item->second));
		ordinaryAirStr.Append(wxT(";"));
	}

	for(auto item = GetPlug()->m_advancedAirConfiguration.cbegin(); item != GetPlug()->m_advancedAirConfiguration.cend(); item++)
	{
		advancedAirStr.Append(wxString::Format(wxT("%d:1:%02X"),item->first,item->second));
		advancedAirStr.Append(wxT(";"));
	}

	totalStr.Append(ordinaryAirStr);
	totalStr.Append(advancedAirStr);
	totalStr.RemoveLast();

	GetPlug()->writeAirConditionConfigurationToDataBase(totalStr);
}

void WRSOCcontrtolSet::onComboboxAirTypeOneChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType1->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel1->Enable(false);
		m_textCtrl_airDevice1->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel1->Enable(true);
		m_textCtrl_airDevice1->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel1->Enable(false);
		m_textCtrl_airDevice1->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::onComboboxAirTypeTwoChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType2->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel2->Enable(false);
		m_textCtrl_airDevice2->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel2->Enable(true);
		m_textCtrl_airDevice2->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel2->Enable(false);
		m_textCtrl_airDevice2->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::onComboboxAirTypeThreeChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType3->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel3->Enable(false);
		m_textCtrl_airDevice3->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel3->Enable(true);
		m_textCtrl_airDevice3->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel3->Enable(false);
		m_textCtrl_airDevice3->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::onComboboxAirTypeFourChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType4->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel4->Enable(false);
		m_textCtrl_airDevice4->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel4->Enable(true);
		m_textCtrl_airDevice4->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel4->Enable(false);
		m_textCtrl_airDevice4->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::onComboboxAirTypeFiveChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType5->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel5->Enable(false);
		m_textCtrl_airDevice5->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel5->Enable(true);
		m_textCtrl_airDevice5->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel5->Enable(false);
		m_textCtrl_airDevice5->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::onComboboxAirTypeSixChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType6->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel6->Enable(false);
		m_textCtrl_airDevice6->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel6->Enable(true);
		m_textCtrl_airDevice6->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel6->Enable(false);
		m_textCtrl_airDevice6->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::onComboboxAirTypeSevenChanged(wxCommandEvent& event)
{
	auto index = m_comboBox_airType7->GetSelection();
	switch(index)
	{
	case NONE:
		m_comboBox_airChannel7->Enable(false);
		m_textCtrl_airDevice7->Enable(false);
		break;
	case ORDINARYAIRCONDITION:
		m_comboBox_airChannel7->Enable(true);
		m_textCtrl_airDevice7->Enable(false);
		break;
	case ADVANCEDAIRCONDITION:
		m_comboBox_airChannel7->Enable(false);
		m_textCtrl_airDevice7->Enable(true);
		break;
	default:
		break;
	}
}

void WRSOCcontrtolSet::InitInputFilter()
{
	m_InputFliter.InputFilterConnect(m_textCtrl_localPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);
	m_InputFliter.InputFilterConnect(m_textCtrl_RadarPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);
	m_InputFliter.InputFilterConnect(m_textCtrl_envPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);
	m_InputFliter.InputFilterConnect(m_textCtrl_videoPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);
	m_InputFliter.InputFilterConnect(m_textCtrl_VideoHttpPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);
	m_InputFliter.InputFilterConnect(m_textCtrl_upsPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);

	m_InputFliter.InputFilterConnect(m_textCtrl_videointerval,FILTERTYPE_PNUM,0,1,60,AllClosedInterval);
	m_InputFliter.InputFilterConnect(m_textCtrl_videosavemonth,FILTERTYPE_PNUM,0,1,12,AllClosedInterval);

	m_InputFliter.InputFilterConnect(m_textCtrl_FTPPort,FILTERTYPE_PNUM,0,1,65535,AllClosedInterval);
}

void WRSOCcontrtolSet::onStatusTimeout(wxTimerEvent& event)
{

}

void WRSOCcontrtolSet::clearText(wxCommandEvent& event)
{

}

void WRSOCcontrtolSet::FtpService(wxCommandEvent& event)
{
	if(m_button_FTPService->GetLabel() == wxT("FTP上传停止"))
	{
		if(GetPlug()->m_FTPThread)
		{
			if(GetPlug()->m_FTPThread->IsRunning())
			{
				if(GetPlug()->m_FTPThread->Pause() != wxTHREAD_NO_ERROR)
				{
					::wxMessageBox(wxT("FTP上传服务停止失败"),wxT("报警"));
				}
				else
				{
					m_button_FTPService->SetLabel(wxT("FTP上传启动"));
					GetPlug()->AddLog(LOG_TYPE_MESSAGE,"FTP上传服务停止");
				}
					         
			}
		}
	}
	else if(m_button_FTPService->GetLabel() == wxT("FTP上传启动"))
	{
		if(GetPlug()->m_FTPThread)
		{
			if(GetPlug()->m_FTPThread->IsPaused())
			{
				if(GetPlug()->m_FTPThread->Resume() != wxTHREAD_NO_ERROR)
				{
					::wxMessageBox(wxT("FTP上传服务启动失败"),wxT("报警"));
				}
				else
				{
					m_button_FTPService->SetLabel(wxT("FTP上传停止"));
					GetPlug()->AddLog(LOG_TYPE_MESSAGE,"FTP上传服务启动");
				}
			}
		}
	}
	else{}
}

void WRSOCcontrtolSet::UPSControlModeShift(wxCommandEvent& event)
{
	auto index = m_comboBox_upsControl->GetSelection();
	
	switch(index)
	{
	case UPS_NetControl:
		m_textCtrl_upsPort->SetValue(wxT("161"));
		break;
	case UPS_ComControl:
		m_textCtrl_upsPort->SetValue(wxT("23"));
		break;
	default:
		break;
	}

}

