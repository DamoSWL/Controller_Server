#pragma once

class CRadarExeParser
{
public:
	CRadarExeParser();
	~CRadarExeParser();

	bool checkFileTime(time_t* newTime,wxString& newFileName);
	bool writeToDataBase(time_t newTime,wxString& newFileName);

	void setUpdateTime();
	void setPreviousFileName();

	void updateTime(time_t newTime);
	void updateFileName(const wxString& newFileName);

private:
	std::time_t m_updateTime;
	UINT m_fileSize;
	std::string m_previousFileName;
};

