#pragma once


//该类主要负责FTP上传
class FTPThread :public wxThread
{
public:
	FTPThread(wxThreadKind kind = wxTHREAD_DETACHED) : wxThread(kind),m_FtpExceptionFlag(FALSE) {}
	~FTPThread(void);

protected:
	virtual void * Entry();  

private:
	std::set<wxString> m_uploadFiles;

	BOOL m_FtpExceptionFlag;  //FTP上传一场标志位
	std::pair<time_t,time_t> m_FtpvExceptionEndureTime;  //FTP上传异常持续时间

};

