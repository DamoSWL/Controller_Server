///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxipctrl.hpp"

#include "WRSOCcontrtol_gui.h"

///////////////////////////////////////////////////////////////////////////

WRSOCContrtolPackPanel::WRSOCContrtolPackPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	
	mainSizer->Add( 0, 0, 1, 0, 0 );
	
	m_OK = new wxButton( this, ID_MAIN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	mainSizer->Add( m_OK, 0, wxALL|wxALIGN_RIGHT|wxALIGN_BOTTOM, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	mainSizer->Add( m_staticText2, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	mainSizer->Add( m_staticText3, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	mainSizer->Add( m_staticText4, 0, wxALL, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
}

WRSOCContrtolPackPanel::~WRSOCContrtolPackPanel()
{
}

WRSOCcontrtolPanelSet::WRSOCcontrtolPanelSet( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_notebook->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	m_panel1 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer58;
	bSizer58 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer58->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer211;
	sbSizer211 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("网络配置") ), wxVERTICAL );
	
	wxBoxSizer* bSizer3821;
	bSizer3821 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3221 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("本地网络配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3221->Wrap( -1 );
	bSizer3821->Add( m_staticText3221, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText3321 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3321->Wrap( -1 );
	bSizer3821->Add( m_staticText3321, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel_localIP = new wxipctrl( sbSizer211->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), wxTAB_TRAVERSAL );
	bSizer3821->Add( m_panel_localIP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText3421 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("端口"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3421->Wrap( -1 );
	bSizer3821->Add( m_staticText3421, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl_localPort = new wxTextCtrl( sbSizer211->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer3821->Add( m_textCtrl_localPort, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer211->Add( bSizer3821, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer38121;
	bSizer38121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText32121 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("雷达网络配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32121->Wrap( -1 );
	bSizer38121->Add( m_staticText32121, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText33121 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33121->Wrap( -1 );
	bSizer38121->Add( m_staticText33121, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel_RadarIP = new wxipctrl( sbSizer211->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), wxTAB_TRAVERSAL );
	bSizer38121->Add( m_panel_RadarIP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText34121 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("端口"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34121->Wrap( -1 );
	bSizer38121->Add( m_staticText34121, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl_RadarPort = new wxTextCtrl( sbSizer211->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer38121->Add( m_textCtrl_RadarPort, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer211->Add( bSizer38121, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer381121;
	bSizer381121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText321121 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("环境网络配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText321121->Wrap( -1 );
	bSizer381121->Add( m_staticText321121, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText331121 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText331121->Wrap( -1 );
	bSizer381121->Add( m_staticText331121, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel_envIP = new wxipctrl( sbSizer211->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), wxTAB_TRAVERSAL );
	bSizer381121->Add( m_panel_envIP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText341121 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("端口"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText341121->Wrap( -1 );
	bSizer381121->Add( m_staticText341121, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl_envPort = new wxTextCtrl( sbSizer211->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer381121->Add( m_textCtrl_envPort, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer211->Add( bSizer381121, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3811111;
	bSizer3811111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3211111 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("视频网络配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3211111->Wrap( -1 );
	bSizer3811111->Add( m_staticText3211111, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText3311111 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3311111->Wrap( -1 );
	bSizer3811111->Add( m_staticText3311111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel_videoIP = new wxipctrl( sbSizer211->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), wxTAB_TRAVERSAL );
	bSizer3811111->Add( m_panel_videoIP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText3411111 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("端口"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3411111->Wrap( -1 );
	bSizer3811111->Add( m_staticText3411111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl_videoPort = new wxTextCtrl( sbSizer211->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer3811111->Add( m_textCtrl_videoPort, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer3811111->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_staticText_httpPort = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("Http端口"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText_httpPort->Wrap( -1 );
	bSizer3811111->Add( m_staticText_httpPort, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_VideoHttpPort = new wxTextCtrl( sbSizer211->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer3811111->Add( m_textCtrl_VideoHttpPort, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	sbSizer211->Add( bSizer3811111, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer38111111;
	bSizer38111111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText32111111 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("UPS网络配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32111111->Wrap( -1 );
	bSizer38111111->Add( m_staticText32111111, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText33111111 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33111111->Wrap( -1 );
	bSizer38111111->Add( m_staticText33111111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel_upsIP = new wxipctrl( sbSizer211->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), wxTAB_TRAVERSAL );
	bSizer38111111->Add( m_panel_upsIP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText34111111 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("端口"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34111111->Wrap( -1 );
	bSizer38111111->Add( m_staticText34111111, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrl_upsPort = new wxTextCtrl( sbSizer211->GetStaticBox(), wxID_ANY, wxT("161"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizer38111111->Add( m_textCtrl_upsPort, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer38111111->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_staticText130 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("控制方式"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText130->Wrap( -1 );
	bSizer38111111->Add( m_staticText130, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_comboBox_upsControl = new wxComboBox( sbSizer211->GetStaticBox(), wxID_ANY, wxT("网络"), wxDefaultPosition, wxSize( 50,-1 ), 0, NULL, wxCB_READONLY );
	m_comboBox_upsControl->Append( wxT("网络(SNMP)") );
	m_comboBox_upsControl->Append( wxT("网络(串口转网络)") );
	m_comboBox_upsControl->SetSelection( 0 );
	m_comboBox_upsControl->SetMinSize( wxSize( 112,-1 ) );
	
	bSizer38111111->Add( m_comboBox_upsControl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText64 = new wxStaticText( sbSizer211->GetStaticBox(), wxID_ANY, wxT("型号"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	m_staticText64->Enable( false );
	m_staticText64->Hide();
	
	bSizer38111111->Add( m_staticText64, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choice_UPSTypeChoices[] = { wxT("EP60") };
	int m_choice_UPSTypeNChoices = sizeof( m_choice_UPSTypeChoices ) / sizeof( wxString );
	m_choice_UPSType = new wxChoice( sbSizer211->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_UPSTypeNChoices, m_choice_UPSTypeChoices, 0 );
	m_choice_UPSType->SetSelection( 0 );
	m_choice_UPSType->Enable( false );
	m_choice_UPSType->Hide();
	
	bSizer38111111->Add( m_choice_UPSType, 0, wxALL, 5 );
	
	
	sbSizer211->Add( bSizer38111111, 0, wxEXPAND, 5 );
	
	
	bSizer58->Add( sbSizer211, 1, wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( bSizer58 );
	m_panel1->Layout();
	bSizer58->Fit( m_panel1 );
	m_notebook->AddPage( m_panel1, wxT("网络配置"), true );
	m_panel2 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetMinSize( wxSize( 140,-1 ) );
	
	wxBoxSizer* bSizer93;
	bSizer93 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer93->Add( 0, 10, 0, wxEXPAND, 5 );
	
	m_scrolledWindow = new wxScrolledWindow( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow->SetScrollRate( 5, 5 );
	wxStaticBoxSizer* sbSizer411;
	sbSizer411 = new wxStaticBoxSizer( new wxStaticBox( m_scrolledWindow, wxID_ANY, wxT("文件路径配置") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer54;
	bSizer54 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer15411;
	bSizer15411 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText26411 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("太阳法文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26411->Wrap( -1 );
	m_staticText26411->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer15411->Add( m_staticText26411, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_sun = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_sun->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer15411->Add( m_textCtrl_sun, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer15411->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_sun = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15411->Add( m_button_sun, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer15411, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer151111;
	bSizer151111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText261111 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("发射率文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText261111->Wrap( -1 );
	m_staticText261111->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer151111->Add( m_staticText261111, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_ref = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_ref->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer151111->Add( m_textCtrl_ref, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer151111->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_ref = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer151111->Add( m_button_ref, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer151111, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer152111;
	bSizer152111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText262111 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("动态范围文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText262111->Wrap( -1 );
	m_staticText262111->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer152111->Add( m_staticText262111, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_dyn = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_dyn->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer152111->Add( m_textCtrl_dyn, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer152111->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_dyn = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer152111->Add( m_button_dyn, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer152111, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer153115;
	bSizer153115 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText263115 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("相位噪音文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText263115->Wrap( -1 );
	m_staticText263115->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer153115->Add( m_staticText263115, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_noise = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_noise->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer153115->Add( m_textCtrl_noise, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer153115->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_noise = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153115->Add( m_button_noise, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer153115, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1531131;
	bSizer1531131 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2631131 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("66项参数文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2631131->Wrap( -1 );
	m_staticText2631131->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer1531131->Add( m_staticText2631131, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_67data = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_67data->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer1531131->Add( m_textCtrl_67data, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer1531131->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_67data = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1531131->Add( m_button_67data, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer1531131, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15311412;
	bSizer15311412 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText26311412 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("雷达报警文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26311412->Wrap( -1 );
	m_staticText26311412->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer15311412->Add( m_staticText26311412, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_diary = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_diary->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer15311412->Add( m_textCtrl_diary, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer15311412->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_diary = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15311412->Add( m_button_diary, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer15311412, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer153114111;
	bSizer153114111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText263114111 = new wxStaticText( sbSizer411->GetStaticBox(), wxID_ANY, wxT("备份文件路径："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText263114111->Wrap( -1 );
	m_staticText263114111->SetMinSize( wxSize( 140,-1 ) );
	
	bSizer153114111->Add( m_staticText263114111, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_backup = new wxTextCtrl( sbSizer411->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_backup->SetMaxSize( wxSize( 320,-1 ) );
	
	bSizer153114111->Add( m_textCtrl_backup, 1, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer153114111->Add( 30, 0, 0, wxEXPAND, 5 );
	
	m_button_backUp = new wxButton( sbSizer411->GetStaticBox(), wxID_ANY, wxT("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer153114111->Add( m_button_backUp, 0, wxALL, 5 );
	
	
	bSizer54->Add( bSizer153114111, 0, wxEXPAND, 5 );
	
	
	sbSizer411->Add( bSizer54, 1, wxEXPAND, 5 );
	
	
	m_scrolledWindow->SetSizer( sbSizer411 );
	m_scrolledWindow->Layout();
	sbSizer411->Fit( m_scrolledWindow );
	bSizer93->Add( m_scrolledWindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel2->SetSizer( bSizer93 );
	m_panel2->Layout();
	bSizer93->Fit( m_panel2 );
	m_notebook->AddPage( m_panel2, wxT("文件路径配置"), false );
	m_panel3 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer291;
	bSizer291 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer291->Add( 20, 20, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer306;
	bSizer306 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText436 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText436->Wrap( -1 );
	bSizer306->Add( m_staticText436, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer306->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType1 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType1->Append( wxT("无") );
	m_comboBox_airType1->Append( wxT("普通空调") );
	m_comboBox_airType1->Append( wxT("精密空调") );
	m_comboBox_airType1->SetSelection( 0 );
	m_comboBox_airType1->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer306->Add( m_comboBox_airType1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer306->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel1 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel1->Append( wxT("第一支路") );
	m_comboBox_airChannel1->Append( wxT("第二支路") );
	m_comboBox_airChannel1->Append( wxT("第三支路") );
	m_comboBox_airChannel1->Append( wxT("第四支路") );
	m_comboBox_airChannel1->SetSelection( 0 );
	m_comboBox_airChannel1->Enable( false );
	m_comboBox_airChannel1->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer306->Add( m_comboBox_airChannel1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer306->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText446 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText446->Wrap( -1 );
	bSizer306->Add( m_staticText446, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice1 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice1->Enable( false );
	
	bSizer306->Add( m_textCtrl_airDevice1, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer306, 0, wxEXPAND, 5 );
	
	
	bSizer291->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3011;
	bSizer3011 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4311 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4311->Wrap( -1 );
	bSizer3011->Add( m_staticText4311, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3011->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType2 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType2->Append( wxT("无") );
	m_comboBox_airType2->Append( wxT("普通空调") );
	m_comboBox_airType2->Append( wxT("精密空调") );
	m_comboBox_airType2->SetSelection( 0 );
	m_comboBox_airType2->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3011->Add( m_comboBox_airType2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3011->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel2 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel2->Append( wxT("第一支路") );
	m_comboBox_airChannel2->Append( wxT("第二支路") );
	m_comboBox_airChannel2->Append( wxT("第三支路") );
	m_comboBox_airChannel2->Append( wxT("第四支路") );
	m_comboBox_airChannel2->SetSelection( 0 );
	m_comboBox_airChannel2->Enable( false );
	m_comboBox_airChannel2->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3011->Add( m_comboBox_airChannel2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3011->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText4411 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4411->Wrap( -1 );
	bSizer3011->Add( m_staticText4411, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice2 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice2->Enable( false );
	
	bSizer3011->Add( m_textCtrl_airDevice2, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer3011, 0, wxEXPAND, 5 );
	
	
	bSizer291->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3021;
	bSizer3021 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4321 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4321->Wrap( -1 );
	bSizer3021->Add( m_staticText4321, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3021->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType3 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType3->Append( wxT("无") );
	m_comboBox_airType3->Append( wxT("普通空调") );
	m_comboBox_airType3->Append( wxT("精密空调") );
	m_comboBox_airType3->SetSelection( 0 );
	m_comboBox_airType3->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3021->Add( m_comboBox_airType3, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3021->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel3 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel3->Append( wxT("第一支路") );
	m_comboBox_airChannel3->Append( wxT("第二支路") );
	m_comboBox_airChannel3->Append( wxT("第三支路") );
	m_comboBox_airChannel3->Append( wxT("第四支路") );
	m_comboBox_airChannel3->SetSelection( 0 );
	m_comboBox_airChannel3->Enable( false );
	m_comboBox_airChannel3->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3021->Add( m_comboBox_airChannel3, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3021->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText4421 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4421->Wrap( -1 );
	bSizer3021->Add( m_staticText4421, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice3 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice3->Enable( false );
	
	bSizer3021->Add( m_textCtrl_airDevice3, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer3021, 0, wxEXPAND, 5 );
	
	
	bSizer291->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3031;
	bSizer3031 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4331 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4331->Wrap( -1 );
	bSizer3031->Add( m_staticText4331, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3031->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType4 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType4->Append( wxT("无") );
	m_comboBox_airType4->Append( wxT("普通空调") );
	m_comboBox_airType4->Append( wxT("精密空调") );
	m_comboBox_airType4->SetSelection( 0 );
	m_comboBox_airType4->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3031->Add( m_comboBox_airType4, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3031->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel4 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel4->Append( wxT("第一支路") );
	m_comboBox_airChannel4->Append( wxT("第二支路") );
	m_comboBox_airChannel4->Append( wxT("第三支路") );
	m_comboBox_airChannel4->Append( wxT("第四支路") );
	m_comboBox_airChannel4->SetSelection( 0 );
	m_comboBox_airChannel4->Enable( false );
	m_comboBox_airChannel4->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3031->Add( m_comboBox_airChannel4, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3031->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText4431 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4431->Wrap( -1 );
	bSizer3031->Add( m_staticText4431, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice4 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice4->Enable( false );
	
	bSizer3031->Add( m_textCtrl_airDevice4, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer3031, 0, wxEXPAND, 5 );
	
	
	bSizer291->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3041;
	bSizer3041 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4341 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4341->Wrap( -1 );
	bSizer3041->Add( m_staticText4341, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3041->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType5 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType5->Append( wxT("无") );
	m_comboBox_airType5->Append( wxT("普通空调") );
	m_comboBox_airType5->Append( wxT("精密空调") );
	m_comboBox_airType5->SetSelection( 0 );
	m_comboBox_airType5->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3041->Add( m_comboBox_airType5, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3041->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel5 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel5->Append( wxT("第一支路") );
	m_comboBox_airChannel5->Append( wxT("第二支路") );
	m_comboBox_airChannel5->Append( wxT("第三支路") );
	m_comboBox_airChannel5->Append( wxT("第四支路") );
	m_comboBox_airChannel5->SetSelection( 0 );
	m_comboBox_airChannel5->Enable( false );
	m_comboBox_airChannel5->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3041->Add( m_comboBox_airChannel5, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3041->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText4441 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4441->Wrap( -1 );
	bSizer3041->Add( m_staticText4441, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice5 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice5->Enable( false );
	
	bSizer3041->Add( m_textCtrl_airDevice5, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer3041, 0, wxEXPAND, 5 );
	
	
	bSizer291->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3051;
	bSizer3051 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4351 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调6"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4351->Wrap( -1 );
	bSizer3051->Add( m_staticText4351, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3051->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType6 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType6->Append( wxT("无") );
	m_comboBox_airType6->Append( wxT("普通空调") );
	m_comboBox_airType6->Append( wxT("精密空调") );
	m_comboBox_airType6->SetSelection( 0 );
	m_comboBox_airType6->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3051->Add( m_comboBox_airType6, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3051->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel6 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel6->Append( wxT("第一支路") );
	m_comboBox_airChannel6->Append( wxT("第二支路") );
	m_comboBox_airChannel6->Append( wxT("第三支路") );
	m_comboBox_airChannel6->Append( wxT("第四支路") );
	m_comboBox_airChannel6->SetSelection( 0 );
	m_comboBox_airChannel6->Enable( false );
	m_comboBox_airChannel6->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer3051->Add( m_comboBox_airChannel6, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer3051->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText4451 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4451->Wrap( -1 );
	bSizer3051->Add( m_staticText4451, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice6 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice6->Enable( false );
	
	bSizer3051->Add( m_textCtrl_airDevice6, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer3051, 0, wxEXPAND, 5 );
	
	
	bSizer291->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer30511;
	bSizer30511 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText43511 = new wxStaticText( m_panel3, wxID_ANY, wxT("空调7"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43511->Wrap( -1 );
	bSizer30511->Add( m_staticText43511, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer30511->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airType7 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airType7->Append( wxT("无") );
	m_comboBox_airType7->Append( wxT("普通空调") );
	m_comboBox_airType7->Append( wxT("精密空调") );
	m_comboBox_airType7->SetSelection( 0 );
	m_comboBox_airType7->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer30511->Add( m_comboBox_airType7, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer30511->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_airChannel7 = new wxComboBox( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_airChannel7->Append( wxT("第一支路") );
	m_comboBox_airChannel7->Append( wxT("第二支路") );
	m_comboBox_airChannel7->Append( wxT("第三支路") );
	m_comboBox_airChannel7->Append( wxT("第四支路") );
	m_comboBox_airChannel7->SetSelection( 0 );
	m_comboBox_airChannel7->Enable( false );
	m_comboBox_airChannel7->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer30511->Add( m_comboBox_airChannel7, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer30511->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText44511 = new wxStaticText( m_panel3, wxID_ANY, wxT("设备地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44511->Wrap( -1 );
	bSizer30511->Add( m_staticText44511, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_airDevice7 = new wxTextCtrl( m_panel3, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_airDevice7->Enable( false );
	
	bSizer30511->Add( m_textCtrl_airDevice7, 0, wxALL, 5 );
	
	
	bSizer291->Add( bSizer30511, 0, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( bSizer291 );
	m_panel3->Layout();
	bSizer291->Fit( m_panel3 );
	m_notebook->AddPage( m_panel3, wxT("空调配置"), false );
	m_panel4 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer83;
	bSizer83 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer83->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("雷达偏振设置") ), wxHORIZONTAL );
	
	m_staticText42 = new wxStaticText( sbSizer6->GetStaticBox(), wxID_ANY, wxT("雷达偏振类型："), wxDefaultPosition, wxSize( 90,-1 ), 0 );
	m_staticText42->Wrap( -1 );
	sbSizer6->Add( m_staticText42, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_comboBox_partialMode = new wxComboBox( sbSizer6->GetStaticBox(), wxID_ANY, wxT("单偏振"), wxDefaultPosition, wxSize( 100,-1 ), 0, NULL, wxCB_READONLY );
	m_comboBox_partialMode->Append( wxT("单偏振") );
	m_comboBox_partialMode->Append( wxT("双偏振") );
	m_comboBox_partialMode->SetSelection( 0 );
	sbSizer6->Add( m_comboBox_partialMode, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	bSizer83->Add( sbSizer6, 0, wxEXPAND, 5 );
	
	
	bSizer83->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("气象站点配置") ), wxHORIZONTAL );
	
	m_staticText421 = new wxStaticText( sbSizer61->GetStaticBox(), wxID_ANY, wxT("气象站点名称："), wxDefaultPosition, wxSize( 90,-1 ), 0 );
	m_staticText421->Wrap( -1 );
	sbSizer61->Add( m_staticText421, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_siteName = new wxTextCtrl( sbSizer61->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	sbSizer61->Add( m_textCtrl_siteName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer61->Add( 20, 0, 0, wxEXPAND, 5 );
	
	m_staticText4211 = new wxStaticText( sbSizer61->GetStaticBox(), wxID_ANY, wxT("气象站点编号："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4211->Wrap( -1 );
	sbSizer61->Add( m_staticText4211, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	sbSizer61->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_textCtrl_siteIndex = new wxTextCtrl( sbSizer61->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	sbSizer61->Add( m_textCtrl_siteIndex, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer83->Add( sbSizer61, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer611;
	sbSizer611 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("短信告警时间段") ), wxHORIZONTAL );
	
	m_staticText59 = new wxStaticText( sbSizer611->GetStaticBox(), wxID_ANY, wxT("起始时间："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	sbSizer611->Add( m_staticText59, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer611->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_spinCtrl_startTime = new wxSpinCtrl( sbSizer611->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 23, 0 );
	m_spinCtrl_startTime->SetMaxSize( wxSize( 80,-1 ) );
	
	sbSizer611->Add( m_spinCtrl_startTime, 0, wxALL, 5 );
	
	m_staticText621 = new wxStaticText( sbSizer611->GetStaticBox(), wxID_ANY, wxT("时"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText621->Wrap( -1 );
	sbSizer611->Add( m_staticText621, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sbSizer611->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_staticText61 = new wxStaticText( sbSizer611->GetStaticBox(), wxID_ANY, wxT("结束时间："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	sbSizer611->Add( m_staticText61, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer611->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_spinCtrl_endTime = new wxSpinCtrl( sbSizer611->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 23, 23 );
	m_spinCtrl_endTime->SetMaxSize( wxSize( 80,-1 ) );
	
	sbSizer611->Add( m_spinCtrl_endTime, 0, wxALL, 5 );
	
	m_staticText63 = new wxStaticText( sbSizer611->GetStaticBox(), wxID_ANY, wxT("时"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	sbSizer611->Add( m_staticText63, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer611->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_staticText62 = new wxStaticText( sbSizer611->GetStaticBox(), wxID_ANY, wxT("报警等级"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	sbSizer611->Add( m_staticText62, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	sbSizer611->Add( 10, 0, 0, wxEXPAND, 5 );
	
	m_comboBox_alarmLevel = new wxComboBox( sbSizer611->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox_alarmLevel->Append( wxT("取消告警") );
	m_comboBox_alarmLevel->Append( wxT("一般") );
	m_comboBox_alarmLevel->Append( wxT("重要") );
	m_comboBox_alarmLevel->Append( wxT("严重") );
	m_comboBox_alarmLevel->SetSelection( 0 );
	m_comboBox_alarmLevel->SetMinSize( wxSize( 120,-1 ) );
	
	sbSizer611->Add( m_comboBox_alarmLevel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer83->Add( sbSizer611, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer311;
	sbSizer311 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("视频快照配置") ), wxVERTICAL );
	
	wxBoxSizer* bSizer100;
	bSizer100 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText117 = new wxStaticText( sbSizer311->GetStaticBox(), wxID_ANY, wxT("视频快照采样间隔时间："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText117->Wrap( -1 );
	bSizer100->Add( m_staticText117, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_videointerval = new wxTextCtrl( sbSizer311->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer100->Add( m_textCtrl_videointerval, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText119 = new wxStaticText( sbSizer311->GetStaticBox(), wxID_ANY, wxT("分钟"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText119->Wrap( -1 );
	bSizer100->Add( m_staticText119, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sbSizer311->Add( bSizer100, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1001;
	bSizer1001 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1171 = new wxStaticText( sbSizer311->GetStaticBox(), wxID_ANY, wxT("视频快照采样保存时间："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1171->Wrap( -1 );
	bSizer1001->Add( m_staticText1171, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrl_videosavemonth = new wxTextCtrl( sbSizer311->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1001->Add( m_textCtrl_videosavemonth, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText1191 = new wxStaticText( sbSizer311->GetStaticBox(), wxID_ANY, wxT("月"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1191->Wrap( -1 );
	bSizer1001->Add( m_staticText1191, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	sbSizer311->Add( bSizer1001, 1, wxEXPAND, 5 );
	
	
	bSizer83->Add( sbSizer311, 0, wxEXPAND, 20 );
	
	
	m_panel4->SetSizer( bSizer83 );
	m_panel4->Layout();
	bSizer83->Fit( m_panel4 );
	m_notebook->AddPage( m_panel4, wxT("其它配置"), false );
	m_panel5 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer583;
	bSizer583 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer583->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2112;
	sbSizer2112 = new wxStaticBoxSizer( new wxStaticBox( m_panel5, wxID_ANY, wxT("FTP配置") ), wxVERTICAL );
	
	wxBoxSizer* bSizer38212;
	bSizer38212 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText32212 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("FTP服务器网络配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32212->Wrap( -1 );
	bSizer38212->Add( m_staticText32212, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText33211 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33211->Wrap( -1 );
	bSizer38212->Add( m_staticText33211, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_panel_FTPIP = new wxipctrl( sbSizer2112->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 140,-1 ), wxTAB_TRAVERSAL );
	bSizer38212->Add( m_panel_FTPIP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText34212 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("端口"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34212->Wrap( -1 );
	bSizer38212->Add( m_staticText34212, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl_FTPPort = new wxTextCtrl( sbSizer2112->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38212->Add( m_textCtrl_FTPPort, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer2112->Add( bSizer38212, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer382121;
	bSizer382121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText322121 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("FTP服务器用户配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText322121->Wrap( -1 );
	bSizer382121->Add( m_staticText322121, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText332111 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("用户名"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText332111->Wrap( -1 );
	bSizer382121->Add( m_staticText332111, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl_FTPUsername = new wxTextCtrl( sbSizer2112->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer382121->Add( m_textCtrl_FTPUsername, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText342121 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("密码"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText342121->Wrap( -1 );
	bSizer382121->Add( m_staticText342121, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl_FTPPassword = new wxTextCtrl( sbSizer2112->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	bSizer382121->Add( m_textCtrl_FTPPassword, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer2112->Add( bSizer382121, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3821211;
	bSizer3821211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3221211 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("FTP服务器用户配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3221211->Wrap( -1 );
	bSizer3821211->Add( m_staticText3221211, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText3321111 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("本地路径"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3321111->Wrap( -1 );
	bSizer3821211->Add( m_staticText3321111, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl_FTPLocalpath = new wxTextCtrl( sbSizer2112->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_FTPLocalpath->SetMinSize( wxSize( 255,-1 ) );
	
	bSizer3821211->Add( m_textCtrl_FTPLocalpath, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer2112->Add( bSizer3821211, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer38212111;
	bSizer38212111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText32212111 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("FTP服务器用户配置："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32212111->Wrap( -1 );
	bSizer38212111->Add( m_staticText32212111, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText33211111 = new wxStaticText( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("远程路径"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33211111->Wrap( -1 );
	bSizer38212111->Add( m_staticText33211111, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl_FTPRemotepath = new wxTextCtrl( sbSizer2112->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_FTPRemotepath->SetMinSize( wxSize( 255,-1 ) );
	
	bSizer38212111->Add( m_textCtrl_FTPRemotepath, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizer2112->Add( bSizer38212111, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer104->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button_FTPService = new wxButton( sbSizer2112->GetStaticBox(), wxID_ANY, wxT("FTP上传启动"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer104->Add( m_button_FTPService, 0, wxALL, 5 );
	
	
	bSizer104->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	sbSizer2112->Add( bSizer104, 8, wxEXPAND, 5 );
	
	
	bSizer583->Add( sbSizer2112, 1, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( bSizer583 );
	m_panel5->Layout();
	bSizer583->Fit( m_panel5 );
	m_notebook->AddPage( m_panel5, wxT("FTP上传设置"), false );
	
	bSizer37->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer37->Add( 0, 10, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer37 );
	this->Layout();
}

WRSOCcontrtolPanelSet::~WRSOCcontrtolPanelSet()
{
}

MyPanel4::MyPanel4( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer37->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer37 );
	this->Layout();
}

MyPanel4::~MyPanel4()
{
}
