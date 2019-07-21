#include "FTPThread.h"
#include <wx/dir.h>

#define FTP_SUCCESS				0//传输成功
#define FTP_HOSTNULL			2//服务器为空
#define FTP_CONNECTFAILD		3//服务器连接失败
#define FTP_LOGINFAILD			4//登录失败
#define FTP_CNOTCHDIR			5//不能改变路径
#define FTP_GETFILEMESSFAILD	6//获取文件信息错误
#define FTP_FILEXFERFAILD		7//文件传输错误


FTPThread::~FTPThread(void)
{
	GetPlug()->m_FTPThread = NULL;
}

void * FTPThread::Entry()
{
	while(!TestDestroy())
	{

		::wxMilliSleep(200);

		wxString dirName(GetPlug()->m_uploadPath);


		wxDir fileDir(dirName);
		wxString fullFilePath;
		wxString fileName;
		INT uploadResult = 0;
		std::set<wxString> tmpFile;	


		if ( fileDir.IsOpened() )
		{
			bool flag = fileDir.GetFirst(&fileName, wxEmptyString,wxDIR_FILES );  //遍历文件夹，获取新的文件
			while( flag )
			{
				if(GetPlug()->GetExit())
				{
					break;
				}
				fullFilePath = dirName;
				fullFilePath += wxT("\\");
				fullFilePath += fileName;

				m_uploadFiles.insert(fullFilePath);
				if(m_uploadFiles.size() >= 20)
				{
					break;
				}

				flag = fileDir.GetNext(&fileName);
			}
		}


		for(auto item = m_uploadFiles.cbegin(); item != m_uploadFiles.cend(); item++)
		{
			if(GetPlug()->m_remotePath.IsEmpty())  //若备份文件为空则不上传
			{
				break;
			}
			uploadResult = GetPlug()->m_CurlModule->FtpXfer2(GetPlug()->m_FTPIPAddress.ToStdString().c_str(),  //把文件进行上传
												GetPlug()->m_FTPPort.ToStdString().c_str(),
												GetPlug()->m_FTPUsername.ToStdString().c_str(),
												GetPlug()->m_FTPPassword.ToStdString().c_str(),
												GetPlug()->m_remotePath.ToStdString().c_str(),
												item->ToStdString().c_str());

			if(uploadResult != FTP_SUCCESS)
			{
				break;
			}
			else
			{
				tmpFile.insert(*item); //上传成功则放入容器中
			}
		}

		for(auto item = tmpFile.cbegin(); item != tmpFile.cend(); item++)  //把成功上传的文件删除
		{
			m_uploadFiles.erase(*item);
			::wxRemoveFile(*item);
		}

		switch(uploadResult)
		{
		case FTP_HOSTNULL:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP服务器为空");
			break;
		case FTP_CONNECTFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP服务器连接失败");
			break;
		case FTP_LOGINFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP服务器登录失败");
			break;
		case FTP_CNOTCHDIR:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"不能改变路径");
			break;
		case FTP_GETFILEMESSFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"获取文件信息错误");
			break;
		case FTP_FILEXFERFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"文件传输错误");
			break;
		default:
			break;
		}


		wxString ftpAlarmContent = wxT("标准输出控制器FTP上传服务异常");  //对上传异常的文件报警
		if((uploadResult != FTP_SUCCESS) && !m_FtpExceptionFlag)
		{
			m_FtpvExceptionEndureTime.first = wxDateTime::GetTimeNow();

			auto pDatabase = GetPlug()->getDatabase();
			wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
									   endTime, \
									   type, \
									   alarmName, \
									   level, \
									   mode, \
									   content, \
									   origin) VALUES");
			if (pDatabase)
			{

				sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_FtpvExceptionEndureTime.first));
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
				sqlSentence.Append(wxString::Format(wxT("%d, "), 2));
				sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
				sqlSentence.Append(wxString::Format(wxT("'%s', "), ftpAlarmContent));
				sqlSentence.Append(wxString::Format(wxT("%d)"), 0));


				pDatabase->RunSQLNoQuery(sqlSentence);
				m_FtpExceptionFlag = TRUE;
			}
		}
		else if((uploadResult == FTP_SUCCESS) && m_FtpExceptionFlag)
		{
			m_FtpvExceptionEndureTime.second = wxDateTime::GetTimeNow();

			auto pDatabase = GetPlug()->getDatabase();
			if (pDatabase)
			{

				wxString sqlSentence;
				sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_FtpvExceptionEndureTime.second,
					m_FtpvExceptionEndureTime.first,
					ftpAlarmContent);

				pDatabase->RunSQLNoQuery(sqlSentence);
				m_FtpExceptionFlag = FALSE;
			}
		}
		else{}


		for(auto i = 0; i< 3*60; i++)
		{
			if(TestDestroy())
			{
				return (wxThread::ExitCode)0;
			}
			wxThread::Sleep(1000);
		}

	}

	return (wxThread::ExitCode)0;
}
