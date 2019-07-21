#pragma once

//该类负责解析java端发送的命令，并组建新的命令帧
class CAirHandler
{
public:
	CAirHandler(void);
	~CAirHandler(void);

public:
	enum   //空调控制行为定义
	{
		SWITCHOFF = 0, 
		SWITCHON,
		MODE,
		TEMPERATURE,
		WINDDIRECTION,
		WINDSPEED
	};
	 
	enum  //空调模式定义
	{
		MODEAUTO = 0,
		MODECOLD,
		MODEHUMIDITY,
		MODEWIND,
		MODEWARM,
	};

public:
	INT parseAirCmd( const char *pData, UINT Len, INT* pUserData);

	std::string fabricateSwitchOnCmd();  //组建空调开关 设置温湿度等命令帧
	std::string fabricateSwitchOffCmd();
	std::string fabricateModeCmd(INT param);
	std::string fabricateTemperatureCmd(INT param);
	std::string fabricateSpeedCmd(INT param);
	std::string fabricateDirectionCmd(INT param);

	std::string fabricateModeCmdForOpen();  //开启空调后发送前一次的设置信息
	std::string fabricateTemperatureCmdForOpen();
	std::string fabricateSpeedCmdForOpen();
	std::string fabricateDirectionCmdForOpen();

private:
	UCHAR getSwitchRegisterAddress();  //根据当前设备标号获取寄存器地址

private:
	UINT m_currentChannel;  //当前设备编号

	std::map<INT,INT> m_ModeValue;  //保存前一次空调设置的模式 温湿度等
	std::map<INT,INT> m_TemperatureValue;
	std::map<INT,INT> m_SpeedValue;
	std::map<INT,INT> m_DirectionValue;
	


};

