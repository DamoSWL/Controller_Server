#pragma once


//负责发电机
class CGenerator : public wxEvtHandler
{
public:
	CGenerator(void);
	~CGenerator(void);


public:
	typedef struct _GenerateParam
	{
		WORD GeneratorVoltageL1;
		WORD GeneratorVoltageL2;
		WORD GeneratorVoltageL3;

		WORD GeneratorVoltageL1L2;
		WORD GeneratorVoltageL2L3;
		WORD GeneratorVoltageL3L1;


		WORD GeneratorCurrentL1;
		WORD GeneratorCurrentL2;
		WORD GeneratorCurrentL3;

		SHORT ActivePower;
		SHORT ActivePowerL1;
		SHORT ActivePowerL2;
		SHORT ActivePowerL3;

		SHORT ApparentPower;
		SHORT ApparentPowerL1;
		SHORT ApparentPowerL2;
		SHORT ApparentPowerL3;

		WORD GeneratorFrequency;
		WORD GeneratorLoad;
		WORD PowerFactor;

		LONG KWHours;
		LONG RunHours;
		WORD BatteryVoltage;
		WORD OilPress;
		WORD WaterTemperature;
		WORD FuelLevel;
		LONG KVArHours;

		WORD EngineState;

	}GeneratorParam;

	enum
	{
		GENERATORVOLTAGEL1 = 0,
		GENERATORVOLTAGEL2,
		GENERATORVOLTAGEL3,
		GENERATORVOLTAGEL1L2,
		GENERATORVOLTAGEL2L3,
		GENERATORVOLTAGEL3L1,
		GENERATORCURRENTL1,
		GENERATORCURRENTL2,
		GENERATORCURRENTL3,
		GENERATORFREQUENCY,

		ACTIVEPOWER,
		ACTIVEPOWERL1,
		ACTIVEPOWERL2,
		ACTIVEPOWERL3,
		APPARENTPOWER,
		APPARENTPOWERL1,
		APPARENTPOWERL2,
		APPARENTPOWERL3,
		BATTERYVOLTAGE,
		OILPRESS,
		WATERTEMPERATURE,
		FUELLEVEL,
		
		GENERATORLOAD,
		POWERFACTOR,

		KWHOURS,
		RUNHOURS,		
		KVARHOURS,

		ENGINESTATE
	};

public:
	BOOL startReceive();
	BOOL stopReceive();

	void startTimer();
	void stopTimer() {m_Timer.Stop();}

	void onGeneratorTimeout(wxTimerEvent& event);

public:
	static int processData(void *pHandle, const char *pData, UINT Len);
	static void processEvent(void *pHandle, int Opt, char *pData = nullptr, UINT Len = 0);


public:
	void handleTypeForUnsigned2(const char *pData, UINT Len);
	void handleTypeForInteger2(const char *pData, UINT Len);
	void handleTypeForChar(const char *pData, UINT Len);
	void handleTypeForInteger4(const char *pData, UINT Len);
	void handleTypeForEngineState(const char *pData, UINT Len);


private:
	void initParam();
	bool writeToDataBase();

	

private:
	ISocketNet* m_udpHandler;

	std::map<UINT,UINT> m_objectNum;
	std::map<UINT,FLOAT> m_objectCofficient;
	std::map<UINT,UINT> m_objectLength;
	std::map<UINT,FLOAT> m_objectValue;

	wxTimer m_Timer;
	
public:
	UINT m_State;

	DECLARE_EVENT_TABLE()
};

