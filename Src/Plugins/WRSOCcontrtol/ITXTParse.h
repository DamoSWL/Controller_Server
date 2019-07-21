#pragma once


class ITXTParse
{
public:
	 virtual ~ITXTParse() {}
	 virtual INT loadTXTFile(time_t* time) = 0;
	 virtual void parseFile() = 0;
	 virtual bool checkFileTime(time_t* time) = 0;
	 virtual bool writeToDataBase(time_t time) = 0;
	 virtual void setUpdateTime(time_t time) = 0;
	 virtual void updateTime(time_t time) = 0;
};
