#pragma once


//������Ҫ����FTP�ϴ�
class FTPThread :public wxThread
{
public:
	FTPThread(wxThreadKind kind = wxTHREAD_DETACHED) : wxThread(kind),m_FtpExceptionFlag(FALSE) {}
	~FTPThread(void);

protected:
	virtual void * Entry();  

private:
	std::set<wxString> m_uploadFiles;

	BOOL m_FtpExceptionFlag;  //FTP�ϴ�һ����־λ
	std::pair<time_t,time_t> m_FtpvExceptionEndureTime;  //FTP�ϴ��쳣����ʱ��

};

