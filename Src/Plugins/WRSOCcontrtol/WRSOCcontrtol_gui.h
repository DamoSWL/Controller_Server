///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WRSOCCONTRTOL_GUI_H__
#define __WRSOCCONTRTOL_GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class wxipctrl;

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DEFAULT wxID_ANY // Default
#define ID_MAIN_OK 1000

///////////////////////////////////////////////////////////////////////////////
/// Class WRSOCContrtolPackPanel
///////////////////////////////////////////////////////////////////////////////
class WRSOCContrtolPackPanel : public wxPanel 
{
	private:
	
	protected:
		wxButton* m_OK;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
	
	public:
		
		WRSOCContrtolPackPanel( wxWindow* parent, wxWindowID id = ID_DEFAULT, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~WRSOCContrtolPackPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WRSOCcontrtolPanelSet
///////////////////////////////////////////////////////////////////////////////
class WRSOCcontrtolPanelSet : public wxPanel 
{
	private:
	
	protected:
		wxNotebook* m_notebook;
		wxPanel* m_panel1;
		wxStaticText* m_staticText3221;
		wxStaticText* m_staticText3321;
		wxipctrl* m_panel_localIP;
		wxStaticText* m_staticText3421;
		wxTextCtrl* m_textCtrl_localPort;
		wxStaticText* m_staticText32121;
		wxStaticText* m_staticText33121;
		wxipctrl* m_panel_RadarIP;
		wxStaticText* m_staticText34121;
		wxTextCtrl* m_textCtrl_RadarPort;
		wxStaticText* m_staticText321121;
		wxStaticText* m_staticText331121;
		wxipctrl* m_panel_envIP;
		wxStaticText* m_staticText341121;
		wxTextCtrl* m_textCtrl_envPort;
		wxStaticText* m_staticText3211111;
		wxStaticText* m_staticText3311111;
		wxipctrl* m_panel_videoIP;
		wxStaticText* m_staticText3411111;
		wxTextCtrl* m_textCtrl_videoPort;
		wxStaticText* m_staticText_httpPort;
		wxTextCtrl* m_textCtrl_VideoHttpPort;
		wxStaticText* m_staticText32111111;
		wxStaticText* m_staticText33111111;
		wxipctrl* m_panel_upsIP;
		wxStaticText* m_staticText34111111;
		wxTextCtrl* m_textCtrl_upsPort;
		wxStaticText* m_staticText130;
		wxComboBox* m_comboBox_upsControl;
		wxStaticText* m_staticText64;
		wxChoice* m_choice_UPSType;
		wxPanel* m_panel2;
		wxScrolledWindow* m_scrolledWindow;
		wxStaticText* m_staticText26411;
		wxTextCtrl* m_textCtrl_sun;
		wxButton* m_button_sun;
		wxStaticText* m_staticText261111;
		wxTextCtrl* m_textCtrl_ref;
		wxButton* m_button_ref;
		wxStaticText* m_staticText262111;
		wxTextCtrl* m_textCtrl_dyn;
		wxButton* m_button_dyn;
		wxStaticText* m_staticText263115;
		wxTextCtrl* m_textCtrl_noise;
		wxButton* m_button_noise;
		wxStaticText* m_staticText2631131;
		wxTextCtrl* m_textCtrl_67data;
		wxButton* m_button_67data;
		wxStaticText* m_staticText26311412;
		wxTextCtrl* m_textCtrl_diary;
		wxButton* m_button_diary;
		wxStaticText* m_staticText263114111;
		wxTextCtrl* m_textCtrl_backup;
		wxButton* m_button_backUp;
		wxPanel* m_panel3;
		wxStaticText* m_staticText436;
		wxComboBox* m_comboBox_airType1;
		wxComboBox* m_comboBox_airChannel1;
		wxStaticText* m_staticText446;
		wxTextCtrl* m_textCtrl_airDevice1;
		wxStaticText* m_staticText4311;
		wxComboBox* m_comboBox_airType2;
		wxComboBox* m_comboBox_airChannel2;
		wxStaticText* m_staticText4411;
		wxTextCtrl* m_textCtrl_airDevice2;
		wxStaticText* m_staticText4321;
		wxComboBox* m_comboBox_airType3;
		wxComboBox* m_comboBox_airChannel3;
		wxStaticText* m_staticText4421;
		wxTextCtrl* m_textCtrl_airDevice3;
		wxStaticText* m_staticText4331;
		wxComboBox* m_comboBox_airType4;
		wxComboBox* m_comboBox_airChannel4;
		wxStaticText* m_staticText4431;
		wxTextCtrl* m_textCtrl_airDevice4;
		wxStaticText* m_staticText4341;
		wxComboBox* m_comboBox_airType5;
		wxComboBox* m_comboBox_airChannel5;
		wxStaticText* m_staticText4441;
		wxTextCtrl* m_textCtrl_airDevice5;
		wxStaticText* m_staticText4351;
		wxComboBox* m_comboBox_airType6;
		wxComboBox* m_comboBox_airChannel6;
		wxStaticText* m_staticText4451;
		wxTextCtrl* m_textCtrl_airDevice6;
		wxStaticText* m_staticText43511;
		wxComboBox* m_comboBox_airType7;
		wxComboBox* m_comboBox_airChannel7;
		wxStaticText* m_staticText44511;
		wxTextCtrl* m_textCtrl_airDevice7;
		wxPanel* m_panel4;
		wxStaticText* m_staticText42;
		wxComboBox* m_comboBox_partialMode;
		wxStaticText* m_staticText421;
		wxTextCtrl* m_textCtrl_siteName;
		wxStaticText* m_staticText4211;
		wxTextCtrl* m_textCtrl_siteIndex;
		wxStaticText* m_staticText59;
		wxSpinCtrl* m_spinCtrl_startTime;
		wxStaticText* m_staticText621;
		wxStaticText* m_staticText61;
		wxSpinCtrl* m_spinCtrl_endTime;
		wxStaticText* m_staticText63;
		wxStaticText* m_staticText62;
		wxComboBox* m_comboBox_alarmLevel;
		wxStaticText* m_staticText117;
		wxTextCtrl* m_textCtrl_videointerval;
		wxStaticText* m_staticText119;
		wxStaticText* m_staticText1171;
		wxTextCtrl* m_textCtrl_videosavemonth;
		wxStaticText* m_staticText1191;
		wxPanel* m_panel5;
		wxStaticText* m_staticText32212;
		wxStaticText* m_staticText33211;
		wxipctrl* m_panel_FTPIP;
		wxStaticText* m_staticText34212;
		wxTextCtrl* m_textCtrl_FTPPort;
		wxStaticText* m_staticText322121;
		wxStaticText* m_staticText332111;
		wxTextCtrl* m_textCtrl_FTPUsername;
		wxStaticText* m_staticText342121;
		wxTextCtrl* m_textCtrl_FTPPassword;
		wxStaticText* m_staticText3221211;
		wxStaticText* m_staticText3321111;
		wxTextCtrl* m_textCtrl_FTPLocalpath;
		wxStaticText* m_staticText32212111;
		wxStaticText* m_staticText33211111;
		wxTextCtrl* m_textCtrl_FTPRemotepath;
		wxButton* m_button_FTPService;
	
	public:
		
		WRSOCcontrtolPanelSet( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 834,587 ), long style = wxTAB_TRAVERSAL ); 
		~WRSOCcontrtolPanelSet();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyPanel4
///////////////////////////////////////////////////////////////////////////////
class MyPanel4 : public wxPanel 
{
	private:
	
	protected:
		wxNotebook* m_notebook3;
	
	public:
		
		MyPanel4( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 820,524 ), long style = wxTAB_TRAVERSAL ); 
		~MyPanel4();
	
};

#endif //__WRSOCCONTRTOL_GUI_H__
