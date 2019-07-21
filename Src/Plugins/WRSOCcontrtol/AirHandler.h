#pragma once

//���ฺ�����java�˷��͵�������齨�µ�����֡
class CAirHandler
{
public:
	CAirHandler(void);
	~CAirHandler(void);

public:
	enum   //�յ�������Ϊ����
	{
		SWITCHOFF = 0, 
		SWITCHON,
		MODE,
		TEMPERATURE,
		WINDDIRECTION,
		WINDSPEED
	};
	 
	enum  //�յ�ģʽ����
	{
		MODEAUTO = 0,
		MODECOLD,
		MODEHUMIDITY,
		MODEWIND,
		MODEWARM,
	};

public:
	INT parseAirCmd( const char *pData, UINT Len, INT* pUserData);

	std::string fabricateSwitchOnCmd();  //�齨�յ����� ������ʪ�ȵ�����֡
	std::string fabricateSwitchOffCmd();
	std::string fabricateModeCmd(INT param);
	std::string fabricateTemperatureCmd(INT param);
	std::string fabricateSpeedCmd(INT param);
	std::string fabricateDirectionCmd(INT param);

	std::string fabricateModeCmdForOpen();  //�����յ�����ǰһ�ε�������Ϣ
	std::string fabricateTemperatureCmdForOpen();
	std::string fabricateSpeedCmdForOpen();
	std::string fabricateDirectionCmdForOpen();

private:
	UCHAR getSwitchRegisterAddress();  //���ݵ�ǰ�豸��Ż�ȡ�Ĵ�����ַ

private:
	UINT m_currentChannel;  //��ǰ�豸���

	std::map<INT,INT> m_ModeValue;  //����ǰһ�οյ����õ�ģʽ ��ʪ�ȵ�
	std::map<INT,INT> m_TemperatureValue;
	std::map<INT,INT> m_SpeedValue;
	std::map<INT,INT> m_DirectionValue;
	


};

