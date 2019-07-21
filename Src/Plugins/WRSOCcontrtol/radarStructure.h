#pragma once


namespace RadarStructure
{

	typedef struct _GENERICHEADER
	{
		int MagicNumber;	//固定标志，用来指示雷达数据文件0x4D545352
		short MajorVersion;
		short MinorVersion;
		int GenericType;
		int ProductType;
		char Reserved[16];
	}GeneHeader;

	typedef struct _RADARSITE  
	{
		char SiteCode[8];
		char SiteName[32];
		float Latitude;
		float Longitude;
		int AntennaHeight;
		int GroundHeight;
		float Frequency;
		float BeamWidthHori;
		float BeamWidthVert;
		int RDAVersion;
		short RadarType;
		short AntennaGain;
		short TransmittingFeederLoss;
		short ReceivingFeederLoss;
		short OtherLoss;
		char Reserved[46];
	}SiteInfo;

	typedef struct _RADARSTATUS  
	{
		char STATUS_DATA[9];
		char STATUS_TIME[7];
		char SEL_PATT[8];
		//int SPARE_1;//多余
		float ANT_PEAK_PWR;
		float XMTR_PEAK_PWR;
		float ANT_AVG_PWR;
		float XMTR_AVG_PWR;
		float POWER_METER_RATIO_DB;
		float ANT_PWR_MTR_ZERO;
		float XMTR_PWR_MTR_ZERO;
		int SPARE_2;//多余
		float ANT_PWR_MTR_ZERO_V;
		float RNSPLN;
		float ANT_PEAK_PWR_V;
		float RNLPLN;
		float ANT_AVG_PWR_V;
		float TE;
		short PULSE_WIDTH;
		short SysStatus;
		float NOISE_HCHAN[4];
		float NOISE_VCHAN[4];
		float RNOISEVCHAN;
		float RNOISEHCHAN;
		short POL;
		short SPARE_3;//多余
		float PDP;
		float TE_V;
		float ZDR;
		short CTL;
		short DT_V;
		short SPARE_4[10];//多余
		float SYSCAL[4];
		float EXPECTED_REFL_AMP[4];
		float SPARE_5[4];
		float MEASURED_REFL_AMP[4];
		float ANT_AZ_EXPECTED;
		float ANT_AZ_MEASURED;
		float ANT_EL_EXPECTED;
		float ANT_EL_MEASURED;
		float SYSSPLN;
		float SPARE_7;
		float SYSLPLN;
		float SPARE_8;
		float EXPECTED_VELOCITY[4];
		float MEASURED_VELOCITY[4];
		float EXPECTED_WIDTH[4];
		float MEASURED_WIDTH[4];
		float SHELTER_TEMP;
		float XMTR_AIR_TEMP;
		float RADOME_TEMP;
		float SYSCAL_DELTA;
		short SPARE_9[10];
		float EXPECTED_RF8_AMP[3];
		float SHELTER_HUMIDITY;
		float XMTR_AIR_HUMIDITY;
		float RADOME_HUMIDITY;
		float MEASURED_RF8_AMP[3];
		float SPARE_10[3];
		float PHASE_NOISE;
		float PHASE_NOISE_V;
		short SPARE_11[8];
		float UNFILTERED_V_CHAN_PWR;
		float FILTERED_V_CHAN_PWR;
		float UNFILTERED_LIN_CHAN_PWR;
		float FILTERED_LIN_CHAN_PWR;
	}RadarStatus;


	struct Whole_CMA
	{
		union {
			struct
			{
				unsigned int ps_rctrl:1;     //配电遥控
				unsigned int xmt_rctrl:1;    //发射遥控
				unsigned int RDA_rctrl:1;    //RDA遥控
				unsigned int PwrSrc:1;       //总电源
				unsigned int low:1;          //发射低压
				unsigned int ready:1;        //发射准加
				unsigned int high:1;         //发射高压
				unsigned int wpluse:1;       //发射宽脉冲
	
				unsigned int WGS0:1;         //波导开关0°
				unsigned int WGS90:1;        //波导开关90°
				unsigned int antsafe:1;      //天线安全
				unsigned int ps_err:1;       //配电总故障
				unsigned int xmt_err:1;      //发射总故障
				unsigned int recv_err:1;     //接收总故障
				unsigned int svo_err:1;      //伺服总故障
				unsigned int mcu_err:1;      //主监控总故障
	
				unsigned int resrv:16;        //保留
			};
			struct
			{
				unsigned int m_status;
			};
		};
		short  FdPress;     //馈线气压1(Kpa);20Kpa～55Kpa
		short  FdHumi;      //馈线湿度1(%);0～40%
		short  AntTemp;     //天线罩温度(℃);0°～35℃
		short  AntHumi;     //天线罩湿度(%);0～95%
		short  RoomTemp;    //机房温度(℃);0°～35℃
		short  RoomHumi;    //机房湿度(%);0～95%
		short  KlyTemp;     //速调管风道温度(℃);0°～35℃
		short  KlyHumi;     //速调管风道湿度(%);0～95%
		short  MagTemp;     //磁场风道温度:0.1(℃)-SC独有，CD保留;
		short  MagHumi;     //磁场风道湿度:0.1(%)-SC独有，CD保留;
		short  CaseTemp;    //恒温箱温度:(℃);20℃～25℃
		short  XmtPwrUp[3]; //天线上发射峰值功率:0.1(kw);0:总;1:H;2:V;
		short  XmtPwrDn;    //天线下发射峰值功率:0.1(kw);
		short  XmtRefDn;    //天线下发射反射功率:0.1(kw);
		short PulseW;     //脉宽:ns
		short  iresrv[13];  //保留
	};

	struct PwrMcu_CMA
	{
		union {
			struct
			{
				unsigned int xmt_a:1;        //发射A相
				unsigned int xmt_b:1;        //发射B相
				unsigned int xmt_c:1;        //发射C相
				unsigned int pwr_sv:1;       //伺服电源
				unsigned int pwr_rcv:1;      //接收电源
				unsigned int pwr_mcu:1;      //监控电源
				unsigned int ps_rctrl:1;     //配电遥控
				unsigned int PwrSrc:1;       //总电源
	
				unsigned int XmtComm:1;      //发射监控通信
				unsigned int AmpComm:1;      //发射功放通信
				unsigned int RcvComm:1;      //接收监控通信
				unsigned int SvoComm:1;      //伺服监控通信
				unsigned int AzDComm:1;      //方位驱动器通信
				unsigned int ElDComm:1;      //俯仰驱动器通信
				unsigned int PwrVComm:1;     //配电电压通信
				unsigned int PwrAComm:1;     //配电电流通信
	
				unsigned int AntComm:1;      //天线罩温湿度通信
				unsigned int KlyComm:1;      //速调管温湿度通信
				unsigned int RoomComm:1;     //机房温湿度通信
				unsigned int DiffComm:1;     //差分转换通信
				unsigned int PowComm:1;      //功率探头通信
				unsigned int resrv2:11;        //保留
			};
			struct
			{
				unsigned int m_status;
			};
		};
		short  AAmp;        //A相电流
		short  BAmp;        //B相电流
		short  CAmp;        //C相电流
		short  AVolt;       //A相电压;200～240V AC
		short  BVolt;       //B相电压;200～240V AC
		short  CVolt;       //C相电压;200～240V AC
		short  iresrv[8];   //保留
	};
	

	struct Rcv_CMA
	{
		union {
			struct
			{
				unsigned int rcv_excit:1;    //激励源
				unsigned int rcv_test:1;     //测试源
				unsigned int rcv_Hlosc:1;    //H本振源
				unsigned int rcv_Vlosc:1;    //V本振源
				unsigned int rcv_losc:1;     //本振源
				unsigned int rcv_COHO:1;     //相干基准
				unsigned int rcv_HCOHO:1;    //H相干基准
				unsigned int rcv_VCOHO:1;    //V相干基准
	
				unsigned int rcv_12v:1;      //12V
				unsigned int rcv_5v:1;       //+5V
				unsigned int rcv__5v:1;      //-5v
				unsigned int rcv_15v:1;      //15V
				unsigned int rcv_24v:1;      //24v
				unsigned int resrv:3;        //保留
	
				unsigned int rcv_Hchan:1;    //H接收通道
				unsigned int rcv_Vchan:1;    //V接收通道
				unsigned int WGS0:1;         //波导开关0°
				unsigned int WGS90:1;        //波导开关90°
				unsigned int dummy:12;       //保留
			};
			struct
			{
				unsigned int m_status;
			};
	};
		short  ExcitOut;        //激励源输出信号:0.01(dBm);+10～+13dBm
		short  TestOut;         //测试源输出信号:0.01(dBm);-3～-6dBm
		short  LoscOut;         //本振源输出信号:0.01(dBm);+10～+13dBm
		short  COHO;            //30MHz相干基准信号:0.01(dBm);0～+3dBm
		short  Rcv12V;          //+12V直流稳压电源:0.01(V);+12V±0.5V
		short  Rcv15V;          //+15V直流稳压电源:0.01(V);+15V±0.5V
		short  Rcv5V;           //+5V直流稳压电源:0.01(V);+5V±0.2V
		short  Rcv_5V;          //-5V直流稳压电源:0.01(V);-5V±0.2V
		short  Rcv24V;          //+24V直流稳压电源:0.01(V);+24V±0.5V
		short  HFieldAmp;       //H路场放电流:0.01(mA);50～70mA
		short  VFieldAmp;       //H路场放电流:0.01(mA);50～70mA
		short  iresrv[11];      //保留
	};

	struct Svo_CMA
	{
		union {
			struct
			{
				unsigned int az_drihot:1;    //方位驱动器过热
				unsigned int az_voltO:1;     //方位驱动器过压
				unsigned int az_over:1;      //方位驱动器过流
				unsigned int az_rota:1;      //方位旋转变压器线断开
				unsigned int az_rdc:1;       //方位RDC
				unsigned int az_voltL:1;     //方位驱动器欠压
				unsigned int resrv:2;        //保留
	
				unsigned int el_drihot:1;    //俯仰驱动器过热
				unsigned int el_voltO:1;     //俯仰驱动器过压
				unsigned int el_over:1;      //俯仰驱动器过流
				unsigned int el_rota:1;      //俯仰旋转变压器线断开
				unsigned int el_rdc:1;       //俯仰RDC
				unsigned int el_voltL:1;     //俯仰驱动器欠压
				unsigned int resrv1:2;        //保留
	
				unsigned int el__2deg:1;     //俯仰-2°
				unsigned int el_90deg:1;     //俯仰+90°
				unsigned int antsafe:1;      //天线安全
				unsigned int exciting:1;     //激磁
				unsigned int dummy:12;       //保留
			};
			struct
			{
				unsigned int m_status;
			};
		};
		short  Sv12V;       //+12V电源:0.01(V);+12V±0.5V
		short  Sv24V;       //+24V电源:0.01(V);+24V±1V
		short  AzAmp;       //方位电流:0.01(A);0～3A
		short  AzCmdRpm;    //方位命令转速:rpm;0～3800rpm
		short  AzCurRpm;    //方位当前转速:rpm;0～3800rpm
		short  ElAmp;       //俯仰电流:0.01(A);0～3A
		short  ElCmdRpm;    //俯仰命令转速:rpm;0～3800rpm
		short  ElCurRpm;    //俯仰当前转速:rpm;0～3800rpm
		short  iresrv[6];   //保留
	};

	struct Xmt_CMA_SC
	{
		union {
			struct
			{
				ULONGLONG p4:1;           //4ｕS指示
				ULONGLONG p1:1;           //1ｕS指示
				ULONGLONG lctrl:1;        //本控指示
				ULONGLONG rctrl:1;        //遥控指示
				ULONGLONG low:1;          //低压指示
				ULONGLONG delay:1;        //延时指示
				ULONGLONG ready:1;        //准加指示
				ULONGLONG high:1;         //高压指示
	
				ULONGLONG fan:1;          //速调管风压
				ULONGLONG plc:1;          //PLC电源
				ULONGLONG dtrig:1;        //放电触发
				ULONGLONG cooling:1;      //冷却启动
				ULONGLONG oilhot:1;       //油温过热
				ULONGLONG igbt:1;         //IGBT过流
				ULONGLONG vacion:1;       //钛泵过流
				ULONGLONG bias:1;         //偏磁欠流
	
				ULONGLONG filaover:1;     //灯丝过流
				ULONGLONG filalack:1;     //灯丝欠流
				ULONGLONG door:1;         //门开关
				ULONGLONG klyhot:1;       //管体过热
				ULONGLONG antsafe:1;      //天线安全
				ULONGLONG energy:1;       //赋能过流
				ULONGLONG npeak:1;        //反峰电流
				ULONGLONG collect:1;      //关高压故障汇总
	
				ULONGLONG end_AmpSrc:1;   //末级功放电源
				ULONGLONG front_AmpSrc:1; //前级功放电源
				ULONGLONG front_Amp4us:1; //前级功放4us
				ULONGLONG front_Amp1us:1; //前级功放1us
				ULONGLONG front_AmpClk:1; //前级功放时钟
				ULONGLONG front_AmpTrig:1;//前级功放触发
				ULONGLONG front_AmpPow:1; //前级功放功率
				ULONGLONG front_AmpExc:1; //前级功放激励
	
				ULONGLONG coll:1;         //注电流过流
				ULONGLONG fback:1;        //反馈过流
				ULONGLONG ctrig:1;        //充电触发
				ULONGLONG mag1un:1;       //磁场1欠流--S
				ULONGLONG mag1over:1;     //磁场1过流--S
				ULONGLONG mag2un:1;       //磁场2欠流--S
				ULONGLONG mag2over:1;     //磁场2过流--S
				ULONGLONG fire:1;         //馈线打火--S
	
				ULONGLONG coilfan:1;      //线圈风压--S
				ULONGLONG fanj:1;	     //风机--S
				ULONGLONG pfn:1; 	     //PFN过压--S
				ULONGLONG highall:1;      //高压全启动  --S
				ULONGLONG tritrig:1;      //可控硅触发  --S
				ULONGLONG work:1;         //工作态     --S
				ULONGLONG test:1;         //测试态     --S
				ULONGLONG tri:1;          //可控硅     --S
	
				ULONGLONG eampexc:1;      //末级功放激励--S
				ULONGLONG eampclk:1;      //末级功放时钟--S
				ULONGLONG eamptrig:1;     //末级功放触发--S
				ULONGLONG eamppow:1;      //末级功放功率--S
				ULONGLONG eamp1us:1;      //末级功放1us--S
				ULONGLONG eamp4us:1;      //末级功放4us--S
				ULONGLONG coilhot:1;      //线圈过温--S
				ULONGLONG losspha:1;      //缺相--S
	
				ULONGLONG highpd:1;        //高压配电--S
				ULONGLONG resrv:7;         //保留
			};
			struct
			{
				ULONGLONG m_status;
			};
		};
		short  frontst;     //前级功放状态
		short  HighPwrAmp;  //高功率电流:0.01(A);2.0～6.0A
		short  FilaAmp;     //灯丝电流:0.01(A);5.0～7.5A
		short  BiasAmp;     //偏磁电流:0.01(A);2.0～8.0A
		short  Monit5V;     //发射监控+5V:0.01(V);5±1V
		short  Monit24V;    //发射监控+24V:0.01(V);24±1V
		short  Monit12V;    //发射监控+12V:0.01(V);12±1V
		short  Monit_12V;   //发射监控-12V:0.01(V);-12±1V
	
		short  CollAmp;     //注电流:0.01(mA);30～45mA
		short  nPeakAmp;    //反峰电流:0.01(mA);≤55mA
		short  VacionVolt;  //钛泵电压:0.01(KV);2.8～4.5kv
		short  VacionAmp;   //钛泵电流0.01(uA);≤20uA
		short  Chg15V;      //充电控制+15V:0.01(V);15±1V
		short  Chg_15V;     //充电控制-15V:0.01(V);-15±1V
		short  Trig12V;     //触发产生+12V:0.01(V);12±1V
		short  Trig24V;     //触发产生+24V:0.01(V);24±2V
		
		short  Front12V;    //前级功放+12V:0.01(V);12±1V
		short  Front24V;    //前级功放+24V:0.01(V);12±1V
		short  Front36V;    //前级功放+36V:0.01(V);36±1V
		short  eamp12V;     //末级功放+12V:0.01(V);12±1V
		short  eamp24V;	//末级功放+24V:0.01(V);24±1V
		short  eamp36V;     //末级功放+36V:0.01(V);36±3V
		short  Filadisc;    //灯丝监测12(V):0.01(V);12±1V
		short  xmt510Volt;  //发射510V：0.1V;
	
		short  Mang1Amp;    //磁场1电流:
		short  Mang2Amp;    //磁场2电流:
		short  Trifan24V;   //可控硅分机电压24V:0.01(V);24±2V
	
		short  iresrv[9];  //保留
	};
	
	


	typedef struct _SCStructure
	{
		struct Whole_CMA m_ws;      //整机状态字节
		struct PwrMcu_CMA m_pwrmcu; //配电+主监控
		struct Rcv_CMA m_recv;      //接收
		struct Svo_CMA m_sv;        //伺服
		struct Xmt_CMA_SC m_xmt;       //发射
	}SCStructure;

#pragma pack(1)

	struct PulseParam
	{
		unsigned int ipw;                //脉宽(ns)
		unsigned short itpower;		//发射机额定峰值功率(kW)
		short  iRadarConst[2];     //雷达常数0.01dB;0:H,1:V
		short  iNoiseLevel[2];     //通道噪声0.01dB;0:H,1:V
		short  iKDSingal;          //KD信号强度(0.01dB)
		unsigned short iDistBin[2];        //信号距离库;0:RFD;1:KD
		unsigned short iMinFreq;
		unsigned short iMaxFreq;  //最小和最大重复频率表
		unsigned short freq[10];           //重复频率表
		short  idBz0[4];           //dBz0标校值0.01dB;0:单H;1:单V;2:双H;3:双V
	};

	struct WarnSet
	{
		short  iMin;               //下限/维护
		short  iMax;               //上限/变坏
	};

	struct SCAdaptParam
	{
		char    sSiteNum[8];        //0-7雷达站号
		char    sSiteName[32];      //8-39站点名称
		float   fLatitude;          //40-43纬度(度)
		float   fLongtitude;        //44-47经度(度)
		int     iAntHeight;         //48-51天线高度(m)
		int     iAltitude;          //52-55地面高度(m)
		char    iTimeZone;          //56时区-12到12
		short  iAMT;               //57-58年平均温度0.01度
		unsigned short iAMP;               //59-60年平均气压mb(毫巴)
		float   fAirRefract;        //61-64日出/日落时的大气折射率(0.5667)
		float   fXmtFreq;           //65-68发射机工作频率(MHz)
		float   fHBWidth;           //69-72水平波束宽度
		float   fVBWidth;           //73-76垂直波束宽度
		int     iRDAVersion;        //77-80雷达数据采集软件版本号
		short   RadarType;          //81-82:1-SA;2-SB;3-SC;33-CA;34-CB;35-CC;36-CCJ;37-CD;65-XA
		short   iAntGain;           //83-84天线增益0.01dB;

		unsigned short iGasAttenu;         //85-86大气衰减(0.00001dB/km)
		unsigned short iXmtLoss[2][2];     //87-94水平发射-发射支路损耗(0.01dB);00:单H,01:单V,10:双H,11:双V
		unsigned short iXmtPwrLoss[5];     //95-104发射功率损耗(0.01dB);0:天线上总,1:天线上H,0:天线上V,3:天线下总,3:天线下反

		unsigned short iRecWorkLoss[2];    //105-108 接收工作支路损耗(0.01dB);0:H,1:V
		unsigned short iRecTestLoss[2];    //109-112 接收测试支路损耗(0.01dB);0:H,1:V
		unsigned short iNSrcLoss[2];       //113-116 接收噪声源损耗(0.01dB);0:H,1:V
		short  iCWSingal;          //117-118 连续波信号强度(0.01dBm)
		short  iRFDSingal;         //119-120 RFD信号强度(0.01dB)
		short  iNoiseENR;          //121-122 噪声源的超噪比(0.01dB)
		short  iZdrBias;           //123-124 Zdr补偿0.01dB
		short  iPdpBias;           //125-126 Pdp补偿0.01

		unsigned char  bAutoCalib;         //127体扫自动标定功能
		unsigned char  iCheckTime;         //128反射率检查和杂波抑制检查间隔时间(hour)
		char    iDyncStart;         //129动态测试起始(1dBm);-120
		char    iDyncEnd;           //130动态测试结束(1dBm);-10
		unsigned char iDyncStep;          //131动态测试步进(1dBm);1
		unsigned char iNSF;               //132噪声标定平滑系数(0.01);1-100
		char    iRFDAttenu[3];      //133-135 RFD测试强度(1dBm);0:小,1:中,2:大
		char    iKDAttenu[3];       //136-138 KD测试强度(1dBm);0:小,1:中,2:大
		char    iCWAttenu;          //139 CW测试强度(1dBm);
		char    iCSAttenu;          //140杂波抑制测试强度(1dBm);

		WarnSet iresv[13];          //141-192 保留
		WarnSet iNLevelThres;       //193-196接收机噪声电平门限(0.1);min,max。
		WarnSet iNTempThres;        //197-200噪声温度门限(0.1);
		WarnSet iTgtThres[2];       //201-208目标一致性门限(0.1dB);0:RFD,1:KD。
		WarnSet iReflCheckThres;    //209-212反射率标定检查门限(0.1dB);
		WarnSet iRadConstThres;     //213-216系统标定常数门限(0.1dB);
		WarnSet iClutSupprThres;    //217-220杂波抑制门限(0.1dB);
		WarnSet iVelCheckThres;     //221-224速度检查门限(0.1m/s);
		WarnSet iSWCheckThres;      //225-228谱宽检查门限(0.1m/s);
		WarnSet iZdrThres;          //229-232 Zdr检查门限(0.1dB);
		PulseParam pulse[2];        //233-284脉宽参数
	};

#pragma pack()


	struct Xmt_CMA_CD
	{
		union {
			struct
			{
				ULONGLONG p2:1;           //2ｕS指示
				ULONGLONG p1:1;           //1ｕS指示
				ULONGLONG lctrl:1;        //本控指示
				ULONGLONG rctrl:1;        //遥控指示
				ULONGLONG low:1;          //低压指示
				ULONGLONG delay:1;        //延时指示
				ULONGLONG ready:1;        //准加指示
				ULONGLONG high:1;         //高压指示
	
				ULONGLONG fan:1;          //速调管风压
				ULONGLONG plc:1;          //PLC电源
				ULONGLONG dtrig:1;        //放电触发
				ULONGLONG cooling:1;      //冷却启动
				ULONGLONG oilhot:1;       //油温过热
				ULONGLONG igbt:1;         //IGBT过流
				ULONGLONG vacion:1;       //钛泵过流
				ULONGLONG bias:1;         //偏磁欠流
	
				ULONGLONG filaover:1;     //灯丝过流
				ULONGLONG filalack:1;     //灯丝欠流
				ULONGLONG door:1;         //门开关
				ULONGLONG klyhot:1;       //管体过热
				ULONGLONG antsafe:1;      //天线安全
				ULONGLONG energy:1;       //赋能过流
				ULONGLONG npeak:1;        //反峰电流
				ULONGLONG collect:1;      //关高压故障汇总
	
				ULONGLONG end_AmpSrc:1;   //末级功放电源
				ULONGLONG front_AmpSrc:1; //前级功放电源
				ULONGLONG front_Amp2us:1; //前级功放2us
				ULONGLONG front_Amp1us:1; //前级功放1us
				ULONGLONG front_AmpClk:1; //前级功放时钟
				ULONGLONG front_AmpTrig:1;//前级功放触发
				ULONGLONG front_AmpPow:1; //前级功放功率
				ULONGLONG front_AmpExc:1; //前级功放激励
	
				ULONGLONG coll:1;         //注电流过流
				ULONGLONG fback:1;        //反馈过流
				ULONGLONG ctrig:1;        //充电触发
				ULONGLONG tri1:1;         //可控硅1
				ULONGLONG tri2:1;         //可控硅2
				ULONGLONG tri3:1;         //可控硅3
				ULONGLONG tri4:1;         //可控硅4
				ULONGLONG tri5:1;         //可控硅5
	
				ULONGLONG tri6:1;         //可控硅6
				ULONGLONG trifan:1;       //可控硅风机
				ULONGLONG resrv:22;       //保留
			};
			struct
			{
				ULONGLONG m_status;
			};
		};
		short  RectVolt;    //整流电压:0.1(V);480～520V
		short  HighPwrAmp;  //高功率电流:0.01(A);2.0～6.0A
		short  FilaAmp;     //灯丝电流:0.01(A);5.0～7.5A
		short  BiasAmp;     //偏磁电流:0.01(A);2.0～8.0A
		short  Monit5V;     //发射监控+5V:0.01(V);5±1V
		short  Monit24V;    //发射监控+24V:0.01(V);24±1V
		short  Monit12V;    //发射监控+12V:0.01(V);12±1V
		short  Monit_12V;   //发射监控-12V:0.01(V);-12±1V
		short  CollAmp;     //注电流:0.01(mA);30～45mA
		short  nPeakAmp;    //反峰电流:0.01(mA);≤55mA
		short  VacionVolt;  //钛泵电压:0.01(KV);2.8～4.5kv
		short  VacionAmp;   //钛泵电流0.01(uA);≤20uA
		short  Chg15V;      //充电控制+15V:0.01(V);15±1V
		short  Chg_15V;     //充电控制-15V:0.01(V);-15±1V
		short  Trig12V;     //触发产生+12V:0.01(V);12±1V
		short  Trig24V;     //触发产生+24V:0.01(V);24±2V
		short  Front12V;    //前级功放+12V:0.01(V);12±1V
		short  Front24V;    //前级功放+24V:0.01(V);12±1V
		short  iresrv[18];  //保留
	};
	
	typedef struct _CDStructure
	{
		struct Whole_CMA m_ws;      //整机状态字节
		struct PwrMcu_CMA m_pwrmcu; //配电+主监控
		struct Rcv_CMA m_recv;      //接收
		struct Svo_CMA m_sv;        //伺服
		struct Xmt_CMA_CD m_xmt;       //发射
	}CDStructure;

	typedef struct _SAStructrue
	{
		char Rda_Version_Name[16];
		float RF_POWER_SMOOTHING;
		float HXMTLOSSSINGLE;
		float HXMTLOSSHV;
		float HRCVLOSS;
		float HTESTLOSS;
		float VXMTLOSSSINGLE;
		float VXMTLOSSHV;
		float VRCVLOSS;
		float VTESTLOSS;
		float PATHLOSS_NOISE_H;
		float PATHLOSS_NOISE_V;
		float TS_CW;
		float TS_NOISE;
		float PATHLOSS_RF;
		float TS_RF;
		float PATHLOSS_KLY;
		float NOISE_SMOOTHING;
		float GAS_LOSS;
		float K1;
		float K2;
		float K3;
		float K4;
		float RADOME_LOSS;
		char RESERVEDA[64];
		unsigned int PFN_HIGH_BIT : 1;
		unsigned int MAINT_REQD : 1;
		unsigned int MIANT_MODE : 1;
		unsigned int WG1_PFN_XFER_INTLK : 1;
		unsigned int WG1_SWITCH_POS : 1;
		unsigned int XMTR_AVAILABLE : 1;
		unsigned int KLYSTRON_WARMUP : 1;
		unsigned int FILAMENT_PS : 1;
		unsigned int FOCUS_COIL_PS : 1;
		unsigned int VAC_PUMP_PS : 1;
		unsigned int FILAMENT_PS_VOL : 1;
		unsigned int P40V_PS : 1;
		unsigned int N15V_PS : 1;
		unsigned int P28V_PS : 1;
		unsigned int P15V_PS : 1;
		unsigned int P5V_PS : 1;
		unsigned int SPARE1 : 1;
		unsigned int SPARE2 : 1;
		unsigned int CABINET_AIR_FLOW : 1;
		unsigned int CABINET_AIR_TEMP : 1;
		unsigned int CABINET_INTLK : 1;
		unsigned int WG_ARC_VCWR : 1;
		unsigned int WG_PRESS : 1;
		unsigned int CIRCULATOR_TEMP : 1;
		unsigned int PFN_LOW_BIT : 1;
		unsigned int TRIGGER_AMP : 1;
		unsigned int INV_DIODE : 1;
		unsigned int FLYBACK_CHARGER : 1;
		unsigned int MAIN_PWR_VOLTAGE : 1;
		unsigned int MOD_SWITCH_FAIL : 1;
		unsigned int MOD_INV_CURRENT : 1;
		unsigned int MOD_OVERLOAD : 1;
		unsigned int XMTR_BATTERY_CHRG : 1;
		unsigned int OIL_LEVEL : 1;
		unsigned int PRF_LIMIT : 1;
		unsigned int OIL_TEMP : 1;
		unsigned int FOCUS_COIL_AIR : 1;
		unsigned int FOCUS_COIL_CUR : 1;
		unsigned int XMTR_OVERCURRENT : 1;
		unsigned int XMTR_OVERVOLTAGE : 1;
		unsigned int ONE_TEST_BIT7 : 1;
		unsigned int ONE_TEST_BIT : 1;
		unsigned int ONE_TEST_BIT5 : 1;
		unsigned int KLYS_AIR_FLOW : 1;
		unsigned int KLYS_AIR_TEMP : 1;
		unsigned int KLYS_VACION_CUR : 1;
		unsigned int KLYS_FILAMENT_CUR : 1;
		unsigned int KLYS_CURRENT : 1;
		unsigned int WG_HMD : 1;
		unsigned int POST_CHG_REG : 1;
		unsigned int MOD_SWITCH_MAINT : 1;
		unsigned int ONE_TEST_BIT4 : 1;
		unsigned int ONE_TEST_BIT3 : 1;
		unsigned int ONE_TEST_BIT2 : 1;
		unsigned int ONE_TEST_BIT1 : 1;
		unsigned int ONE_TEST_BIT0 : 1;
		unsigned int ZERO_TEST_BIT7 : 1;
		unsigned int ZERO_TEST_BIT6 : 1;
		unsigned int ZERO_TEST_BIT5 : 1;
		unsigned int ZERO_TEST_BIT4 : 1;
		unsigned int ZERO_TEST_BIT3  : 1;
		unsigned int ZERO_TEST_BIT2  : 1;
		unsigned int ZERO_TEST_BIT1  : 1;
		unsigned int ZERO_TEST_BIT0  : 1;
		unsigned int SPARE3 : 1;
		unsigned int SPARE4 : 1;
		unsigned int SPARE5 : 1;
		unsigned int UART  : 1;
		unsigned int SPARE6 : 1;
		unsigned int XMTR_INOP  : 1;
		unsigned int TX_RECYCLE  : 1;
		unsigned int HIGH_VOLTAGE  : 1;
		unsigned int SPARE7_7 : 1;
		unsigned int SPARE7_6 : 1;
		unsigned int SPARE7_5 : 1;
		unsigned int SPARE7_4 : 1;
		unsigned int SPARE7_3 : 1;
		unsigned int SPARE7_2 : 1;
		unsigned int SPARE7_1 : 1;
		unsigned int SPARE7_0 : 1;
		unsigned int PS2_N9V_PS  : 1;
		unsigned int PS1_PN18V_PS  : 1;
		unsigned int SPAREH : 1;
		unsigned int PS2_P5V_PS  : 1;
		unsigned int SPARE8 : 1;
		unsigned int SPARE9 : 1;
		unsigned int SPARE10 : 1;
		unsigned int SPARE11 : 1;
		unsigned int PS3_N5PT2V_PS  : 1;
		unsigned int SPARE12 : 1;
		unsigned int PS1_P5V_PS  : 1;
		unsigned int PS4_P6V_PS  : 1;
		unsigned int PS2_P9V_PS  : 1;
		unsigned int SPARE13 : 1;
		unsigned int SPARE14 : 1;
		unsigned int SPARE15 : 1;
		unsigned int RADOME_HATCH  : 1;
		unsigned int SPARE16 : 1;
		unsigned int SPARE17 : 1;
		unsigned int SPARE18 : 1;
		unsigned int SPARE19 : 1;
		unsigned int SPARE20 : 1;
		unsigned int SPARE21 : 1;
		unsigned int SPARE22 : 1;
		unsigned int SPARE23 : 1;
		unsigned int SPARE24 : 1;
		unsigned int SPARE25 : 1;
		unsigned int SPARE26 : 1;
		unsigned int SPARE27 : 1;
		unsigned int WG2_PFN_XFER_INTLK  : 1;
		unsigned int WG2_SWITCH_POS  : 1;
		unsigned int SPARE28 : 1;
		unsigned int SUMMARY : 16;
		char RESERVEDB[64];
		unsigned int EL_GEARBOX_OIL  : 1;
		unsigned int EL_ENCODER_LIGHT  : 1;
		unsigned int SPARE29 : 1;
		unsigned int EL_N_LIMIT : 1;
		unsigned int EL_P_LIMIT  : 1;
		unsigned int SPARE30 : 1;
		unsigned int EL_DEAD_LIMIT : 1;
		unsigned int EL_PCU_PARITY : 1;
		unsigned int EL_STOW_PIN : 1;
		unsigned int EL_MOTOR : 1;
		unsigned int PED_150V_UNDER : 1;
		unsigned int PED_150V_OVER : 1;
		unsigned int EL_AMP_OVRTEMP : 1;
		unsigned int EL_AMP_SHT_CKT : 1;
		unsigned int EL_AMP_INHIBIT  : 1;
		unsigned int PED_BIT_ST1 :1;
		unsigned int SPARE31 : 1;
		unsigned int AZ_HANDWHELL :1;
		unsigned int EL_HANDWHELL :1;
		unsigned int SPARE32 : 1;
		unsigned int AZ_BULLGEAR_OIL :1;
		unsigned int AZ_GEARBOX_OIL :1;
		unsigned int AZ_ENCODER_LIGHT :1;
		unsigned int AZ_PCU_PARITY :1;
		unsigned int AZ_STOW_PIN :1;
		unsigned int AZ_MOTOR :1;
		unsigned int SPARE33 : 1;
		unsigned int SPARE34 : 1;
		unsigned int AZ_AMP_OVRTEMP :1;
		unsigned int AZ_AMP_SHT_CKT :1;
		unsigned int AZ_AMP_INHIBIT :1;
		unsigned int PED_BIT_ST2 :1;
		unsigned int SPARE35_15 : 1;
		unsigned int SPARE35_14 : 1;
		unsigned int SPARE35_13 : 1;
		unsigned int SPARE35_12 : 1;
		unsigned int SPARE35_11 : 1;
		unsigned int SPARE35_10 : 1;
		unsigned int SPARE35_9 : 1;
		unsigned int SPARE35_8 : 1;
		unsigned int PED_INTLK_SWITCH :1;
		unsigned int SERVO :1;
		unsigned int EL_AMP_PS :1;
		unsigned int AZ_AMP_PS :1;
		unsigned int SPARE36 : 1;
		unsigned int SPARE37 : 1;
		unsigned int SPARE38 : 1;
		unsigned int PED_BIT_ST3 :1;
		char RESERVEDD[766];
	}SAStructrue;


	//typedef struct _CCStructure
	//{
	//	float KLY_InputPWR;
	//	float RECTVOLT;
	//	float HIGHPWRAMP;
	//	float PFN_VOLT;
	//	float NPEAKAMP;
	//	float KLY_ALLAMP;
	//	float KLY_TBAMP;
	//	float VacionAMP;
	//	float FilaAMP;
	//	float Magn1AMP;
	//	float Magn2AMP;
	//	float LO1_PWR;
	//	float TestPWR;
	//	float ExcitPWR;
	//	float Gain;
	//	float SWR;
	//	float NOISE_COEFFICIENT;
	//	float DYNAMIC_RANGE;
	//	float ANT_InTemp;
	//	float ANT_InHumidity;
	//	float Cab_Temp;
	//	float Cab_Humidity;
	//	float Feeder_Pressure;
	//	float Uab_Volt;
	//	float Ubc_Volt;
	//	float Uca_Volt;
	//	float AVolt;
	//	float AAmp;
	//	float BVolt;
	//	float BAmp;
	//	float CVolt;
	//	float CAmp;

 //		unsigned int xmtBit0 : 1;			//冷却开关
 //		unsigned int xmtBit1 : 1;			//磁场开关
 //		unsigned int xmtBit2 : 1;			//回扫开关
 //		unsigned int xmtBit3 : 1;			//无充电触发
 //		unsigned int xmtBit4 : 1;			//无放电触发
 //		unsigned int xmtBit5 : 1;			//超占空比
 //		unsigned int xmtBit6 : 1;			//固态功放
 //		unsigned int xmtBit7 : 1;			//固态欠输出
 //		unsigned int xmtBit8 : 1;			//磁场电源1
 //		unsigned int xmtBit9 : 1;			//磁场电源2
 //		unsigned int xmtBit10 : 1;			//灯丝电源
 //		unsigned int xmtBit11 : 1;			//电源通风
 //		unsigned int xmtBit12 : 1;			//回扫电源
 //		unsigned int xmtBit13 : 1;			//充电过荷
 //		unsigned int xmtBit14 : 1;			//人工线过压
 //		unsigned int xmtBit15 : 1;			//人工线电压低
 //		unsigned int xmtBit16 : 1;			//反峰
 //		unsigned int xmtBit17 : 1;			//可控硅
 //		unsigned int xmtBit18 : 1;			//可控硅风机
 //		unsigned int xmtBit19 : 1;			//KLY总流
 //		unsigned int xmtBit20 : 1;			//KLY管体
 //		unsigned int xmtBit21 : 1;			//KLY总流节点
 //		unsigned int xmtBit22 : 1;			//KLY管体节点
 //		unsigned int xmtBit23 : 1;			//KLY温度
 //		unsigned int xmtBit24 : 1;			//线包温度
 //		unsigned int xmtBit25 : 1;			//钛泵欠压
 //		unsigned int xmtBit26 : 1;			//钛泵过流
 //		unsigned int xmtBit27 : 1;			//+15V电源（高压电源）
 //		unsigned int xmtBit28 : 1;			//+15V电源（调制器）
 //		unsigned int xmtBit29 : 1;			//+24V电源（监控）
 //		unsigned int xmtBit30 : 1;			//+24V电源（调制器）
 //		unsigned int xmtBit31 : 1;			//+60V电源（调制器）
 //		unsigned int xmtBit32 : 1;			//机内温度过高
 //		unsigned int xmtBit33 : 1;			//机内温度过低
 //		unsigned int xmtBit34 : 1;			//机柜门开启
 //		unsigned int xmtBit35 : 1;			//天线罩门开启
 //		unsigned int xmtBit36 : 1;			//紧急开关
 //		unsigned int xmtBit37 : 11;			//预留：38-63
	//	unsigned int xmtBit38 : 16;			//预留：38-63

 //		unsigned int rcvBit0 : 1; //H路主通道
 //		unsigned int rcvBit1 : 1; //H路副通道
 //		unsigned int rcvBit2 :  1; //V路主通道
 //		unsigned int rcvBit3 : 1; //V路副通道
 //		unsigned int rcvBit4 : 1; //一本振
 //		unsigned int rcvBit5 : 1; //二本振
 //		unsigned int rcvBit6 : 1; //晶振
 //		unsigned int rcvBit7 : 1; //150Mz时钟1
 //		unsigned int rcvBit8 : 1; //上变频
 //		unsigned int rcvBit9 : 1; //84MHz时钟
 //		unsigned int rcvBit10 : 1; //150Mz时钟2
 //		unsigned int rcvBit11 : 1; //接收直流电源
 //		unsigned int rcvBit12 : 1; //DDS时钟
 //		unsigned int rcvBit13 : 1; //AD采样时钟
 //		unsigned int rcvBit14 : 1; //系统时钟
 //		unsigned int rcvBit15 : 1; //Burst通道
 //		unsigned int rcvBit16 : 1; //主通道H1噪声电平告警
 //		unsigned int rcvBit17 : 1; //副通道H2噪声电平告警
 //		unsigned int rcvBit18 : 1; //主通道H1增益告警
 //		unsigned int rcvBit19 : 1; //副通道H2增益告警
 //		unsigned int rcvBit20 : 1; //通道增益差告警
 //		unsigned int rcvBit21 : 1; //噪声系数告警
 //		unsigned int rcvBit22 : 1; //Burst通道增益告警
 //		unsigned int rcvBit23 : 1; //Burst通道噪声电平告警
 //		unsigned int rcvBit24 : 1; //一本振信号告警
 //		unsigned int rcvBit25 : 1; //激励信号告警
 //		unsigned int rcvBit26 : 1; //DDS测试信号告警
 //		unsigned int rcvBit27 : 5; //预留：28-47
 //		unsigned int rcvBit28 : 16; //预留：28-47
 //
 //		unsigned int svBit0 : 1; //无IQ数据
 //		unsigned int svBit1 : 1; //数据丢包
 //		unsigned int svBit2 : 1; //适配参数加载失败
 //		unsigned int svBit3 : 1; //无参数输出
 //		unsigned int svBit4 : 1; //存储空间过低
 //		unsigned int svBit5 : 11; //预留:27个bit
 //		unsigned int svBit6 : 16; //预留:27个bit
 //
 //		unsigned int svBit7 : 1; //方位驱动器
 //		unsigned int svBit8 : 1; //俯仰驱动器
 //		unsigned int svBit9 : 1; //R/D故障
 //		unsigned int svBit10 : 1; //遥/本控
 //		unsigned int svBit11 : 1; //+15V故障
 //		unsigned int svBit12 : 1; //-15V故障
 //		unsigned int svBit13 : 1; //俯仰定位
 //		unsigned int svBit14 : 1; //方位定位
 //		unsigned int svBit15 : 1; //冲顶故障
 //		unsigned int svBit16 : 1; //打底故障
 //		unsigned int svBit17 : 1; //天线方位角码跳变
 //		unsigned int svBit18 : 1; //天线俯仰角码跳变
 //		unsigned int svBit19 : 1; //方位定位误差
 //		unsigned int svBit20 : 1; //俯仰定位误差
 //		unsigned int svBit21 : 1; //天线定位精度
 //		unsigned int svBit22 : 1; //预留：49个bit
 //		unsigned int svBit23 : 16; //预留：49个bit
 //		unsigned int svBit24 : 16; //预留：49个bit
 //		unsigned int svBit25 : 16; //预留：49个bit
 //
 //		unsigned int svBit26 : 1; //波导开关
 //		unsigned int svBit27 : 1; //放电触发信号
 //		unsigned int svBit28 : 1; //充电触发信号
 //		unsigned int svBit29 : 1; //导前触发信号
 //		unsigned int svBit30 : 1; //功放门套信号
 //		unsigned int svBit31 : 1; //通风单元1
 //		unsigned int svBit32 : 1; //通风单元2
 //		unsigned int svBit33 : 9; //预留：25个bit
 //		unsigned int svBit34 : 16; //预留：25个bit
 //
 //		unsigned int pwrBit0 : 1; //380V备份
 //		unsigned int pwrBit1 : 1; //发射电源
 //		unsigned int pwrBit2 : 1; //UPS电源
 //		unsigned int pwrBit3 : 1; //接收电源
 //		unsigned int pwrBit4 : 1; //伺服电源
 //		unsigned int pwrBit5 : 1; //电源转接
 //		unsigned int pwrBit6 : 1; //功率检测
 //		unsigned int pwrBit7 : 1; //预留：1个bit
 //		unsigned int pwrBit8 : 1; //A相电压
 //		unsigned int pwrBit9 : 1; //B相电压
 //		unsigned int pwrBit10 : 1; //C相电压
 //		unsigned int pwrBit11 : 1; //A相电流
 //		unsigned int pwrBit12: 1; //B相电流
 //		unsigned int pwrBit13 : 1; //C相电流
 //		unsigned int pwrBit14 : 2; //预留：34个bit
 //		unsigned int pwrBit15 : 16; //预留：34个bit
 //
 //		unsigned int pwrBit17 : 1; //导前触发信号
 //		unsigned int pwrBit18 : 15; //预留：31个bit
 //		unsigned int pwrBit19 : 16; //预留：31个bit
 //
 //		unsigned int pwrBit20 : 1; //发射通信
 //		unsigned int pwrBit21 : 1; //接收通信
 //		unsigned int pwrBit22 : 1; //信号处理通信
 //		unsigned int pwrBit23 : 1; //伺服通信
 //		unsigned int pwrBit24 : 1; //接收功率检测通信
 //		unsigned int pwrBit25 : 1; //KLY功率检测通信
 //		unsigned int pwrBit26 : 1; //配电通信
 //		unsigned int pwrBit27 : 1; //天线功率检测通信
 //		unsigned int pwrBit28 : 1; //时间服务器通信
 //		unsigned int pwrBit29 : 1; //波导充气机通信
 //		unsigned int pwrBit30 : 1; //UPS电源通信
 //		unsigned int pwrBit31 : 1; //温湿度仪1-天线罩通信
 //		unsigned int pwrBit32 : 1; //温湿度仪2-转台通信
 //		unsigned int pwrBit33 : 1; //温湿度仪3-机房通信
 //		unsigned int pwrBit34 : 1; //温湿度仪4-机柜通信
 //		unsigned int pwrBit35 : 1; //温湿度仪5-发射通信
 //		unsigned int pwrBit37 : 16; //预留：30个bit
 //
 //
 //		unsigned int pwrBit38 : 1; //充气机气压告警
 //		unsigned int pwrBit39 : 1; //充气机温度告警
 //		unsigned int pwrBit40 : 14; //预留：35个bit
 //		unsigned int pwrBit41 : 16; //预留：35个bit
 //		unsigned int pwrBit42 : 1; //预留：35个bit
 //		unsigned int pwrBit43 : 1; //预留：35个bit
 //		unsigned int pwrBit44 : 1; //预留：35个bit
 //		unsigned int pwrBit45 : 1; //预留：35个bit
 //		unsigned int pwrBit46 : 1; //预留：35个bit
 //
 //		unsigned int pwrBit47 : 1; //天线罩温度告警
 //		unsigned int pwrBit48 : 1; //天线罩湿度告警
 //		unsigned int pwrBit49 : 1; //转台温度告警
 //		unsigned int pwrBit50 : 1; //转台湿度告警
 //		unsigned int pwrBit51 : 1; //机房温度告警
 //		unsigned int pwrBit52 : 1; //机房湿度告警
 //		unsigned int pwrBit53 : 1; //综合机柜温度告警
 //		unsigned int pwrBit54 : 1; //综合机柜湿度告警
 //		unsigned int pwrBit55 : 1; //发射机柜温度告警
 //		unsigned int pwrBit56 : 1; //发射机柜湿度告警
 //		unsigned int pwrBit57 : 1; //发射监控温度告警
 //		unsigned int pwrBit58 : 16; //预留：62个bit
 //		unsigned int pwrBit59 : 16; //预留：62个bit
 //		unsigned int pwrBit60 : 16; //预留：62个bit
 //		unsigned int pwrBit62 : 1; //时间服务器故障
 //		unsigned int pwrBit63 : 15; //预留：31个bit
 //		unsigned int pwrBit64 : 16; //预留：31个bit
 //
 //		unsigned int pwrBit65 : 1; //雷达常数C
 //		unsigned int pwrBit66 : 1; //定标常数SYSCAL
 //		unsigned int pwrBit67 : 1; //相位噪声
 //		unsigned int pwrBit68 : 1; //杂波抑制
 //		unsigned int pwrBit69 : 1; //CW测试误差
 //		unsigned int pwrBit70 : 1; //RFD测试误差
 //		unsigned int pwrBit71 : 1; //KD测试误差
 //		unsigned int pwrBit72 : 1; //速度测试误差
 //		unsigned int pwrBit73 : 8; //预留：24个bit
 //		unsigned int pwrBit74 : 16; //预留：24个bit

	//}CCStructure;

#pragma pack(1)
	typedef struct _CCStructure
	{
		float KLY_InputPWR;
		float RECTVOLT;
		float HIGHPWRAMP;
		float PFN_VOLT;
		float NPEAKAMP;
		float KLY_ALLAMP;
		float KLY_TBAMP;
		float VacionAMP;
		float FilaAMP;
		float Magn1AMP;
		float Magn2AMP;
		float LO1_PWR;
		float TestPWR;
		float ExcitPWR;
		float Gain;
		float SWR;
		float NOISE_COEFFICIENT;
		float DYNAMIC_RANGE;
		float ANT_InTemp;
		float ANT_InHumidity;
		float Cab_Temp;
		float Cab_Humidity;
		float Feeder_Pressure;
		float Uab_Volt;
		float Ubc_Volt;
		float Uca_Volt;
		float AVolt;
		float AAmp;
		float BVolt;
		float BAmp;
		float CVolt;
		float CAmp;

		union
		{	
			short bitset1;
			struct 
 			{
				
 				short xmtBit0 : 1;			//冷却开关
 				short xmtBit1 : 1;			//磁场开关
 				short xmtBit2 : 1;			//回扫开关
 				short xmtBit3 : 1;			//无充电触发
 				short xmtBit4 : 1;			//无放电触发
 				short xmtBit5 : 1;			//超占空比
 				short xmtBit6 : 1;			//固态功放
 				short xmtBit7 : 1;			//固态欠输出
  				short xmtBit8 : 1;			//磁场电源1
  				short xmtBit9 : 1;			//磁场电源2
  				short xmtBit10 : 1;			//灯丝电源
  				short xmtBit11 : 1;			//电源通风
  				short xmtBit12 : 1;			//回扫电源
  				short xmtBit13 : 1;			//充电过荷
  				short xmtBit14 : 1;			//人工线过压
  				short xmtBit15 : 1;			//人工线电压低
 			};
				
		};	

		union 
		{
			short bitset2;
			struct  
			{
				short xmtBit16 : 1;			//反峰
				short xmtBit17 : 1;			//可控硅
				short xmtBit18 : 1;			//可控硅风机
				short xmtBit19 : 1;			//KLY总流
				short xmtBit20 : 1;			//KLY管体
				short xmtBit21 : 1;			//KLY总流节点
				short xmtBit22 : 1;			//KLY管体节点
				short xmtBit23 : 1;			//KLY温度
				short xmtBit24 : 1;			//线包温度
				short xmtBit25 : 1;			//钛泵欠压
				short xmtBit26 : 1;			//钛泵过流
				short xmtBit27 : 1;			//+15V电源（高压电源）
				short xmtBit28 : 1;			//+15V电源（调制器）
				short xmtBit29 : 1;			//+24V电源（监控）
				short xmtBit30 : 1;			//+24V电源（调制器）
				short xmtBit31 : 1;			//+60V电源（调制器）
			};
		};

	
		union
		{
			short bitset3;
			struct  
			{
				short xmtBit32 : 1;			//机内温度过高
				short xmtBit33 : 1;			//机内温度过低
				short xmtBit34 : 1;			//机柜门开启
				short xmtBit35 : 1;			//天线罩门开启
				short xmtBit36 : 1;			//紧急开关
				short xmtBit37 : 11;			//预留：38-63
			};
		};

		union
		{
			short bitset4;
		};


 		union
 		{
 			short bitset5;
 			struct  
 			{
 				short rcvBit0 : 1; //H路主通道
 				short rcvBit1 : 1; //H路副通道
 				short rcvBit2 : 1; //V路主通道
 				short rcvBit3 : 1; //V路副通道
 				short rcvBit4 : 1; //一本振
 				short rcvBit5 : 1; //二本振
 				short rcvBit6 : 1; //晶振
 				short rcvBit7 : 1; //150Mz时钟1
 				short rcvBit8 : 1; //上变频
 				short rcvBit9 : 1; //84MHz时钟
 				short rcvBit10 : 1; //150Mz时钟2
 				short rcvBit11 : 1; //接收直流电源
 				short rcvBit12 : 1; //DDS时钟
 				short rcvBit13 : 1; //AD采样时钟
 				short rcvBit14 : 1; //系统时钟
 				short rcvBit15 : 1; //Burst通道
 			};
 		};

 		union
 		{
 			short bitset6;
 			struct 
 			{
 				short rcvBit16 : 1; //主通道H1噪声电平告警
 				short rcvBit17 : 1; //副通道H2噪声电平告警
 				short rcvBit18 : 1; //主通道H1增益告警
 				short rcvBit19 : 1; //副通道H2增益告警
 				short rcvBit20 : 1; //通道增益差告警
 				short rcvBit21 : 1; //噪声系数告警
 				short rcvBit22 : 1; //Burst通道增益告警
 				short rcvBit23 : 1; //Burst通道噪声电平告警
 				short rcvBit24 : 1; //一本振信号告警
 				short rcvBit25 : 1; //激励信号告警
 				short rcvBit26 : 1; //DDS测试信号告警
 				short rcvBit27 : 5; //预留：28-47
 			};
 		};

		union
		{
			short bitset7;
		};

		union
		{
			short bitset8;
			struct  
			{
					short svBit0 : 1; //无IQ数据
					short svBit1 : 1; //数据丢包
					short svBit2 : 1; //适配参数加载失败
					short svBit3 : 1; //无参数输出
					short svBit4 : 1; //存储空间过低
					short svBit5 : 11; //预留:27个bit
			};
		};

		union
		{
			short bitset9;
		};

		union
		{
			short bitset10;
			struct  
			{
				short svBit7 : 1; //方位驱动器
				short svBit8 : 1; //俯仰驱动器
				short svBit9 : 1; //R/D故障
				short svBit10 : 1; //遥/本控
				short svBit11 : 1; //+15V故障
				short svBit12 : 1; //-15V故障
				short svBit13 : 1; //俯仰定位
				short svBit14 : 1; //方位定位
				short svBit15 : 1; //冲顶故障
				short svBit16 : 1; //打底故障
				short svBit17 : 1; //天线方位角码跳变
				short svBit18 : 1; //天线俯仰角码跳变
				short svBit19 : 1; //方位定位误差
				short svBit20 : 1; //俯仰定位误差
				short svBit21 : 1; //天线定位精度
			};
		};

		union
		{
			short bitset11;
		};

		union
		{
			short bitset12;
		};

		union
		{
			short bitset13;
		};

		union
		{
			short bitset14;
			struct  
			{
				short svBit26 : 1; //波导开关
				short svBit27 : 1; //放电触发信号
				short svBit28 : 1; //充电触发信号
				short svBit29 : 1; //导前触发信号
				short svBit30 : 1; //功放门套信号
				short svBit31 : 1; //通风单元1
				short svBit32 : 1; //通风单元2
				short svBit33 : 9; //预留：25个bit
			};
		};
 	
		union
		{
			short bitset15;
		};

		union
		{
			short bitset16;
			struct  
			{
				short pwrBit0 : 1; //380V备份
				short pwrBit1 : 1; //发射电源
				short pwrBit2 : 1; //UPS电源
				short pwrBit3 : 1; //接收电源
				short pwrBit4 : 1; //伺服电源
				short pwrBit5 : 1; //电源转接
				short pwrBit6 : 1; //功率检测
				short pwrBit7 : 1; //预留：1个bit
				short pwrBit8 : 1; //A相电压
				short pwrBit9 : 1; //B相电压
				short pwrBit10 : 1; //C相电压
				short pwrBit11 : 1; //A相电流
				short pwrBit12: 1; //B相电流
				short pwrBit13 : 1; //C相电流
				short pwrBit14 : 2; //预留：34个bit
			};
		};

		union
		{
			short bitset17;
		};


		union
		{
			short bitset18;
			struct  
			{
				short pwrBit17 : 1; //导前触发信号
				short pwrBit18 : 15; //预留：31个bit
			};
		};

		union
		{
			short bitset19;
		};

		union
		{
			short bitset20;
			struct  
			{
				short pwrBit20 : 1; //发射通信
				short pwrBit21 : 1; //接收通信
				short pwrBit22 : 1; //信号处理通信
				short pwrBit23 : 1; //伺服通信
				short pwrBit24 : 1; //接收功率检测通信
				short pwrBit25 : 1; //KLY功率检测通信
				short pwrBit26 : 1; //配电通信
				short pwrBit27 : 1; //天线功率检测通信
				short pwrBit28 : 1; //时间服务器通信
				short pwrBit29 : 1; //波导充气机通信
				short pwrBit30 : 1; //UPS电源通信
				short pwrBit31 : 1; //温湿度仪1-天线罩通信
				short pwrBit32 : 1; //温湿度仪2-转台通信
				short pwrBit33 : 1; //温湿度仪3-机房通信
				short pwrBit34 : 1; //温湿度仪4-机柜通信
				short pwrBit35 : 1; //温湿度仪5-发射通信
			};
		};

		union
		{
			short bitset21;
		};

		union
		{
			short bitset22;
			struct
			{
				short pwrBit38 : 1; //充气机气压告警
				short pwrBit39 : 1; //充气机温度告警
				short pwrBit40 : 14; //预留：35个bit
			};
		};

		union
		{
			short bitset23;
		};

		union
		{
			short bitset24;
			struct
			{
				short pwrBit42 : 1; //预留：35个bit
				short pwrBit43 : 1; //预留：35个bit
				short pwrBit44 : 1; //预留：35个bit
				short pwrBit45 : 1; //预留：35个bit
				short pwrBit46 : 1; //预留：35个bit
				short pwrBit47 : 1; //天线罩温度告警
				short pwrBit48 : 1; //天线罩湿度告警
				short pwrBit49 : 1; //转台温度告警
				short pwrBit50 : 1; //转台湿度告警
				short pwrBit51 : 1; //机房温度告警
				short pwrBit52 : 1; //机房湿度告警
				short pwrBit53 : 1; //综合机柜温度告警
				short pwrBit54 : 1; //综合机柜湿度告警
				short pwrBit55 : 1; //发射机柜温度告警
				short pwrBit56 : 1; //发射机柜湿度告警
				short pwrBit57 : 1; //发射监控温度告警
			};
		};

		union
		{
			short bitset25;
		};

		union
		{
			short bitset26;
		};

		union
		{
			short bitset27;
		};

		union
		{
			short bitset28;
			struct  
			{
				short pwrBit62 : 1; //时间服务器故障
				short pwrBit63 : 15; //预留：31个bit
			};
		};

		union
		{
			short bitset29;
		};

		union
		{
			short bitset30;
			struct  
			{
				short pwrBit65 : 1; //雷达常数C
				short pwrBit66 : 1; //定标常数SYSCAL
				short pwrBit67 : 1; //相位噪声
				short pwrBit68 : 1; //杂波抑制
				short pwrBit69 : 1; //CW测试误差
				short pwrBit70 : 1; //RFD测试误差
				short pwrBit71 : 1; //KD测试误差
				short pwrBit72 : 1; //速度测试误差
				short pwrBit73 : 8; //预留：24个bit
			};
		};


		union
		{
			short bitset31;
		};


	}CCStructure;
#pragma pack()


	struct CCAdaptParam
	{
		float Le_one;
		float cw2Power;
		float cw1_hPower;
		float cw_vPower;
		float us1RfdPower;
		float us2RfdPower;
		float kd1Power;
		float kd2Power;
		float InternalNoiseRatio;
		float cw_h_cwh_t;
		float cw_v_cwv_t;
		float cw_h_cwh_e;
		float cw_v_cwv_e;
		float rfd_h_1us;
		float rfd_h_2us;
		float noiseRatio_h;
		float noiseRatio_v;
		float two_way_airLoss;
		
	};

}

