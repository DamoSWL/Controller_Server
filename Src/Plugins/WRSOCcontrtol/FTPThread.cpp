#include "FTPThread.h"
#include <wx/dir.h>

#define FTP_SUCCESS				0//����ɹ�
#define FTP_HOSTNULL			2//������Ϊ��
#define FTP_CONNECTFAILD		3//����������ʧ��
#define FTP_LOGINFAILD			4//��¼ʧ��
#define FTP_CNOTCHDIR			5//���ܸı�·��
#define FTP_GETFILEMESSFAILD	6//��ȡ�ļ���Ϣ����
#define FTP_FILEXFERFAILD		7//�ļ��������


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
			bool flag = fileDir.GetFirst(&fileName, wxEmptyString,wxDIR_FILES );  //�����ļ��У���ȡ�µ��ļ�
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
			if(GetPlug()->m_remotePath.IsEmpty())  //�������ļ�Ϊ�����ϴ�
			{
				break;
			}
			uploadResult = GetPlug()->m_CurlModule->FtpXfer2(GetPlug()->m_FTPIPAddress.ToStdString().c_str(),  //���ļ������ϴ�
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
				tmpFile.insert(*item); //�ϴ��ɹ������������
			}
		}

		for(auto item = tmpFile.cbegin(); item != tmpFile.cend(); item++)  //�ѳɹ��ϴ����ļ�ɾ��
		{
			m_uploadFiles.erase(*item);
			::wxRemoveFile(*item);
		}

		switch(uploadResult)
		{
		case FTP_HOSTNULL:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP������Ϊ��");
			break;
		case FTP_CONNECTFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP����������ʧ��");
			break;
		case FTP_LOGINFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"FTP��������¼ʧ��");
			break;
		case FTP_CNOTCHDIR:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"���ܸı�·��");
			break;
		case FTP_GETFILEMESSFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"��ȡ�ļ���Ϣ����");
			break;
		case FTP_FILEXFERFAILD:
			GetPlug()->AddLog(LOG_TYPE_ERROR,"�ļ��������");
			break;
		default:
			break;
		}


		wxString ftpAlarmContent = wxT("��׼���������FTP�ϴ������쳣");  //���ϴ��쳣���ļ�����
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
