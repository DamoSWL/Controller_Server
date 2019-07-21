#pragma once


namespace RadarStructure
{

	typedef struct _GENERICHEADER
	{
		int MagicNumber;	//�̶���־������ָʾ�״������ļ�0x4D545352
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
		//int SPARE_1;//����
		float ANT_PEAK_PWR;
		float XMTR_PEAK_PWR;
		float ANT_AVG_PWR;
		float XMTR_AVG_PWR;
		float POWER_METER_RATIO_DB;
		float ANT_PWR_MTR_ZERO;
		float XMTR_PWR_MTR_ZERO;
		int SPARE_2;//����
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
		short SPARE_3;//����
		float PDP;
		float TE_V;
		float ZDR;
		short CTL;
		short DT_V;
		short SPARE_4[10];//����
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
				unsigned int ps_rctrl:1;     //���ң��
				unsigned int xmt_rctrl:1;    //����ң��
				unsigned int RDA_rctrl:1;    //RDAң��
				unsigned int PwrSrc:1;       //�ܵ�Դ
				unsigned int low:1;          //�����ѹ
				unsigned int ready:1;        //����׼��
				unsigned int high:1;         //�����ѹ
				unsigned int wpluse:1;       //���������
	
				unsigned int WGS0:1;         //��������0��
				unsigned int WGS90:1;        //��������90��
				unsigned int antsafe:1;      //���߰�ȫ
				unsigned int ps_err:1;       //����ܹ���
				unsigned int xmt_err:1;      //�����ܹ���
				unsigned int recv_err:1;     //�����ܹ���
				unsigned int svo_err:1;      //�ŷ��ܹ���
				unsigned int mcu_err:1;      //������ܹ���
	
				unsigned int resrv:16;        //����
			};
			struct
			{
				unsigned int m_status;
			};
		};
		short  FdPress;     //������ѹ1(Kpa);20Kpa��55Kpa
		short  FdHumi;      //����ʪ��1(%);0��40%
		short  AntTemp;     //�������¶�(��);0�㡫35��
		short  AntHumi;     //������ʪ��(%);0��95%
		short  RoomTemp;    //�����¶�(��);0�㡫35��
		short  RoomHumi;    //����ʪ��(%);0��95%
		short  KlyTemp;     //�ٵ��ܷ���¶�(��);0�㡫35��
		short  KlyHumi;     //�ٵ��ܷ��ʪ��(%);0��95%
		short  MagTemp;     //�ų�����¶�:0.1(��)-SC���У�CD����;
		short  MagHumi;     //�ų����ʪ��:0.1(%)-SC���У�CD����;
		short  CaseTemp;    //�������¶�:(��);20�桫25��
		short  XmtPwrUp[3]; //�����Ϸ����ֵ����:0.1(kw);0:��;1:H;2:V;
		short  XmtPwrDn;    //�����·����ֵ����:0.1(kw);
		short  XmtRefDn;    //�����·��䷴�书��:0.1(kw);
		short PulseW;     //����:ns
		short  iresrv[13];  //����
	};

	struct PwrMcu_CMA
	{
		union {
			struct
			{
				unsigned int xmt_a:1;        //����A��
				unsigned int xmt_b:1;        //����B��
				unsigned int xmt_c:1;        //����C��
				unsigned int pwr_sv:1;       //�ŷ���Դ
				unsigned int pwr_rcv:1;      //���յ�Դ
				unsigned int pwr_mcu:1;      //��ص�Դ
				unsigned int ps_rctrl:1;     //���ң��
				unsigned int PwrSrc:1;       //�ܵ�Դ
	
				unsigned int XmtComm:1;      //������ͨ��
				unsigned int AmpComm:1;      //���书��ͨ��
				unsigned int RcvComm:1;      //���ռ��ͨ��
				unsigned int SvoComm:1;      //�ŷ����ͨ��
				unsigned int AzDComm:1;      //��λ������ͨ��
				unsigned int ElDComm:1;      //����������ͨ��
				unsigned int PwrVComm:1;     //����ѹͨ��
				unsigned int PwrAComm:1;     //������ͨ��
	
				unsigned int AntComm:1;      //��������ʪ��ͨ��
				unsigned int KlyComm:1;      //�ٵ�����ʪ��ͨ��
				unsigned int RoomComm:1;     //������ʪ��ͨ��
				unsigned int DiffComm:1;     //���ת��ͨ��
				unsigned int PowComm:1;      //����̽ͷͨ��
				unsigned int resrv2:11;        //����
			};
			struct
			{
				unsigned int m_status;
			};
		};
		short  AAmp;        //A�����
		short  BAmp;        //B�����
		short  CAmp;        //C�����
		short  AVolt;       //A���ѹ;200��240V AC
		short  BVolt;       //B���ѹ;200��240V AC
		short  CVolt;       //C���ѹ;200��240V AC
		short  iresrv[8];   //����
	};
	

	struct Rcv_CMA
	{
		union {
			struct
			{
				unsigned int rcv_excit:1;    //����Դ
				unsigned int rcv_test:1;     //����Դ
				unsigned int rcv_Hlosc:1;    //H����Դ
				unsigned int rcv_Vlosc:1;    //V����Դ
				unsigned int rcv_losc:1;     //����Դ
				unsigned int rcv_COHO:1;     //��ɻ�׼
				unsigned int rcv_HCOHO:1;    //H��ɻ�׼
				unsigned int rcv_VCOHO:1;    //V��ɻ�׼
	
				unsigned int rcv_12v:1;      //12V
				unsigned int rcv_5v:1;       //+5V
				unsigned int rcv__5v:1;      //-5v
				unsigned int rcv_15v:1;      //15V
				unsigned int rcv_24v:1;      //24v
				unsigned int resrv:3;        //����
	
				unsigned int rcv_Hchan:1;    //H����ͨ��
				unsigned int rcv_Vchan:1;    //V����ͨ��
				unsigned int WGS0:1;         //��������0��
				unsigned int WGS90:1;        //��������90��
				unsigned int dummy:12;       //����
			};
			struct
			{
				unsigned int m_status;
			};
	};
		short  ExcitOut;        //����Դ����ź�:0.01(dBm);+10��+13dBm
		short  TestOut;         //����Դ����ź�:0.01(dBm);-3��-6dBm
		short  LoscOut;         //����Դ����ź�:0.01(dBm);+10��+13dBm
		short  COHO;            //30MHz��ɻ�׼�ź�:0.01(dBm);0��+3dBm
		short  Rcv12V;          //+12Vֱ����ѹ��Դ:0.01(V);+12V��0.5V
		short  Rcv15V;          //+15Vֱ����ѹ��Դ:0.01(V);+15V��0.5V
		short  Rcv5V;           //+5Vֱ����ѹ��Դ:0.01(V);+5V��0.2V
		short  Rcv_5V;          //-5Vֱ����ѹ��Դ:0.01(V);-5V��0.2V
		short  Rcv24V;          //+24Vֱ����ѹ��Դ:0.01(V);+24V��0.5V
		short  HFieldAmp;       //H·���ŵ���:0.01(mA);50��70mA
		short  VFieldAmp;       //H·���ŵ���:0.01(mA);50��70mA
		short  iresrv[11];      //����
	};

	struct Svo_CMA
	{
		union {
			struct
			{
				unsigned int az_drihot:1;    //��λ����������
				unsigned int az_voltO:1;     //��λ��������ѹ
				unsigned int az_over:1;      //��λ����������
				unsigned int az_rota:1;      //��λ��ת��ѹ���߶Ͽ�
				unsigned int az_rdc:1;       //��λRDC
				unsigned int az_voltL:1;     //��λ������Ƿѹ
				unsigned int resrv:2;        //����
	
				unsigned int el_drihot:1;    //��������������
				unsigned int el_voltO:1;     //������������ѹ
				unsigned int el_over:1;      //��������������
				unsigned int el_rota:1;      //������ת��ѹ���߶Ͽ�
				unsigned int el_rdc:1;       //����RDC
				unsigned int el_voltL:1;     //����������Ƿѹ
				unsigned int resrv1:2;        //����
	
				unsigned int el__2deg:1;     //����-2��
				unsigned int el_90deg:1;     //����+90��
				unsigned int antsafe:1;      //���߰�ȫ
				unsigned int exciting:1;     //����
				unsigned int dummy:12;       //����
			};
			struct
			{
				unsigned int m_status;
			};
		};
		short  Sv12V;       //+12V��Դ:0.01(V);+12V��0.5V
		short  Sv24V;       //+24V��Դ:0.01(V);+24V��1V
		short  AzAmp;       //��λ����:0.01(A);0��3A
		short  AzCmdRpm;    //��λ����ת��:rpm;0��3800rpm
		short  AzCurRpm;    //��λ��ǰת��:rpm;0��3800rpm
		short  ElAmp;       //��������:0.01(A);0��3A
		short  ElCmdRpm;    //��������ת��:rpm;0��3800rpm
		short  ElCurRpm;    //������ǰת��:rpm;0��3800rpm
		short  iresrv[6];   //����
	};

	struct Xmt_CMA_SC
	{
		union {
			struct
			{
				ULONGLONG p4:1;           //4��Sָʾ
				ULONGLONG p1:1;           //1��Sָʾ
				ULONGLONG lctrl:1;        //����ָʾ
				ULONGLONG rctrl:1;        //ң��ָʾ
				ULONGLONG low:1;          //��ѹָʾ
				ULONGLONG delay:1;        //��ʱָʾ
				ULONGLONG ready:1;        //׼��ָʾ
				ULONGLONG high:1;         //��ѹָʾ
	
				ULONGLONG fan:1;          //�ٵ��ܷ�ѹ
				ULONGLONG plc:1;          //PLC��Դ
				ULONGLONG dtrig:1;        //�ŵ紥��
				ULONGLONG cooling:1;      //��ȴ����
				ULONGLONG oilhot:1;       //���¹���
				ULONGLONG igbt:1;         //IGBT����
				ULONGLONG vacion:1;       //�ѱù���
				ULONGLONG bias:1;         //ƫ��Ƿ��
	
				ULONGLONG filaover:1;     //��˿����
				ULONGLONG filalack:1;     //��˿Ƿ��
				ULONGLONG door:1;         //�ſ���
				ULONGLONG klyhot:1;       //�������
				ULONGLONG antsafe:1;      //���߰�ȫ
				ULONGLONG energy:1;       //���ܹ���
				ULONGLONG npeak:1;        //�������
				ULONGLONG collect:1;      //�ظ�ѹ���ϻ���
	
				ULONGLONG end_AmpSrc:1;   //ĩ�����ŵ�Դ
				ULONGLONG front_AmpSrc:1; //ǰ�����ŵ�Դ
				ULONGLONG front_Amp4us:1; //ǰ������4us
				ULONGLONG front_Amp1us:1; //ǰ������1us
				ULONGLONG front_AmpClk:1; //ǰ������ʱ��
				ULONGLONG front_AmpTrig:1;//ǰ�����Ŵ���
				ULONGLONG front_AmpPow:1; //ǰ�����Ź���
				ULONGLONG front_AmpExc:1; //ǰ�����ż���
	
				ULONGLONG coll:1;         //ע��������
				ULONGLONG fback:1;        //��������
				ULONGLONG ctrig:1;        //��紥��
				ULONGLONG mag1un:1;       //�ų�1Ƿ��--S
				ULONGLONG mag1over:1;     //�ų�1����--S
				ULONGLONG mag2un:1;       //�ų�2Ƿ��--S
				ULONGLONG mag2over:1;     //�ų�2����--S
				ULONGLONG fire:1;         //���ߴ��--S
	
				ULONGLONG coilfan:1;      //��Ȧ��ѹ--S
				ULONGLONG fanj:1;	     //���--S
				ULONGLONG pfn:1; 	     //PFN��ѹ--S
				ULONGLONG highall:1;      //��ѹȫ����  --S
				ULONGLONG tritrig:1;      //�ɿع败��  --S
				ULONGLONG work:1;         //����̬     --S
				ULONGLONG test:1;         //����̬     --S
				ULONGLONG tri:1;          //�ɿع�     --S
	
				ULONGLONG eampexc:1;      //ĩ�����ż���--S
				ULONGLONG eampclk:1;      //ĩ������ʱ��--S
				ULONGLONG eamptrig:1;     //ĩ�����Ŵ���--S
				ULONGLONG eamppow:1;      //ĩ�����Ź���--S
				ULONGLONG eamp1us:1;      //ĩ������1us--S
				ULONGLONG eamp4us:1;      //ĩ������4us--S
				ULONGLONG coilhot:1;      //��Ȧ����--S
				ULONGLONG losspha:1;      //ȱ��--S
	
				ULONGLONG highpd:1;        //��ѹ���--S
				ULONGLONG resrv:7;         //����
			};
			struct
			{
				ULONGLONG m_status;
			};
		};
		short  frontst;     //ǰ������״̬
		short  HighPwrAmp;  //�߹��ʵ���:0.01(A);2.0��6.0A
		short  FilaAmp;     //��˿����:0.01(A);5.0��7.5A
		short  BiasAmp;     //ƫ�ŵ���:0.01(A);2.0��8.0A
		short  Monit5V;     //������+5V:0.01(V);5��1V
		short  Monit24V;    //������+24V:0.01(V);24��1V
		short  Monit12V;    //������+12V:0.01(V);12��1V
		short  Monit_12V;   //������-12V:0.01(V);-12��1V
	
		short  CollAmp;     //ע����:0.01(mA);30��45mA
		short  nPeakAmp;    //�������:0.01(mA);��55mA
		short  VacionVolt;  //�ѱõ�ѹ:0.01(KV);2.8��4.5kv
		short  VacionAmp;   //�ѱõ���0.01(uA);��20uA
		short  Chg15V;      //������+15V:0.01(V);15��1V
		short  Chg_15V;     //������-15V:0.01(V);-15��1V
		short  Trig12V;     //��������+12V:0.01(V);12��1V
		short  Trig24V;     //��������+24V:0.01(V);24��2V
		
		short  Front12V;    //ǰ������+12V:0.01(V);12��1V
		short  Front24V;    //ǰ������+24V:0.01(V);12��1V
		short  Front36V;    //ǰ������+36V:0.01(V);36��1V
		short  eamp12V;     //ĩ������+12V:0.01(V);12��1V
		short  eamp24V;	//ĩ������+24V:0.01(V);24��1V
		short  eamp36V;     //ĩ������+36V:0.01(V);36��3V
		short  Filadisc;    //��˿���12(V):0.01(V);12��1V
		short  xmt510Volt;  //����510V��0.1V;
	
		short  Mang1Amp;    //�ų�1����:
		short  Mang2Amp;    //�ų�2����:
		short  Trifan24V;   //�ɿع�ֻ���ѹ24V:0.01(V);24��2V
	
		short  iresrv[9];  //����
	};
	
	


	typedef struct _SCStructure
	{
		struct Whole_CMA m_ws;      //����״̬�ֽ�
		struct PwrMcu_CMA m_pwrmcu; //���+�����
		struct Rcv_CMA m_recv;      //����
		struct Svo_CMA m_sv;        //�ŷ�
		struct Xmt_CMA_SC m_xmt;       //����
	}SCStructure;

#pragma pack(1)

	struct PulseParam
	{
		unsigned int ipw;                //����(ns)
		unsigned short itpower;		//��������ֵ����(kW)
		short  iRadarConst[2];     //�״ﳣ��0.01dB;0:H,1:V
		short  iNoiseLevel[2];     //ͨ������0.01dB;0:H,1:V
		short  iKDSingal;          //KD�ź�ǿ��(0.01dB)
		unsigned short iDistBin[2];        //�źž����;0:RFD;1:KD
		unsigned short iMinFreq;
		unsigned short iMaxFreq;  //��С������ظ�Ƶ�ʱ�
		unsigned short freq[10];           //�ظ�Ƶ�ʱ�
		short  idBz0[4];           //dBz0��Уֵ0.01dB;0:��H;1:��V;2:˫H;3:˫V
	};

	struct WarnSet
	{
		short  iMin;               //����/ά��
		short  iMax;               //����/�仵
	};

	struct SCAdaptParam
	{
		char    sSiteNum[8];        //0-7�״�վ��
		char    sSiteName[32];      //8-39վ������
		float   fLatitude;          //40-43γ��(��)
		float   fLongtitude;        //44-47����(��)
		int     iAntHeight;         //48-51���߸߶�(m)
		int     iAltitude;          //52-55����߶�(m)
		char    iTimeZone;          //56ʱ��-12��12
		short  iAMT;               //57-58��ƽ���¶�0.01��
		unsigned short iAMP;               //59-60��ƽ����ѹmb(����)
		float   fAirRefract;        //61-64�ճ�/����ʱ�Ĵ���������(0.5667)
		float   fXmtFreq;           //65-68���������Ƶ��(MHz)
		float   fHBWidth;           //69-72ˮƽ�������
		float   fVBWidth;           //73-76��ֱ�������
		int     iRDAVersion;        //77-80�״����ݲɼ�����汾��
		short   RadarType;          //81-82:1-SA;2-SB;3-SC;33-CA;34-CB;35-CC;36-CCJ;37-CD;65-XA
		short   iAntGain;           //83-84��������0.01dB;

		unsigned short iGasAttenu;         //85-86����˥��(0.00001dB/km)
		unsigned short iXmtLoss[2][2];     //87-94ˮƽ����-����֧·���(0.01dB);00:��H,01:��V,10:˫H,11:˫V
		unsigned short iXmtPwrLoss[5];     //95-104���书�����(0.01dB);0:��������,1:������H,0:������V,3:��������,3:�����·�

		unsigned short iRecWorkLoss[2];    //105-108 ���չ���֧·���(0.01dB);0:H,1:V
		unsigned short iRecTestLoss[2];    //109-112 ���ղ���֧·���(0.01dB);0:H,1:V
		unsigned short iNSrcLoss[2];       //113-116 ��������Դ���(0.01dB);0:H,1:V
		short  iCWSingal;          //117-118 �������ź�ǿ��(0.01dBm)
		short  iRFDSingal;         //119-120 RFD�ź�ǿ��(0.01dB)
		short  iNoiseENR;          //121-122 ����Դ�ĳ����(0.01dB)
		short  iZdrBias;           //123-124 Zdr����0.01dB
		short  iPdpBias;           //125-126 Pdp����0.01

		unsigned char  bAutoCalib;         //127��ɨ�Զ��궨����
		unsigned char  iCheckTime;         //128�����ʼ����Ӳ����Ƽ����ʱ��(hour)
		char    iDyncStart;         //129��̬������ʼ(1dBm);-120
		char    iDyncEnd;           //130��̬���Խ���(1dBm);-10
		unsigned char iDyncStep;          //131��̬���Բ���(1dBm);1
		unsigned char iNSF;               //132�����궨ƽ��ϵ��(0.01);1-100
		char    iRFDAttenu[3];      //133-135 RFD����ǿ��(1dBm);0:С,1:��,2:��
		char    iKDAttenu[3];       //136-138 KD����ǿ��(1dBm);0:С,1:��,2:��
		char    iCWAttenu;          //139 CW����ǿ��(1dBm);
		char    iCSAttenu;          //140�Ӳ����Ʋ���ǿ��(1dBm);

		WarnSet iresv[13];          //141-192 ����
		WarnSet iNLevelThres;       //193-196���ջ�������ƽ����(0.1);min,max��
		WarnSet iNTempThres;        //197-200�����¶�����(0.1);
		WarnSet iTgtThres[2];       //201-208Ŀ��һ��������(0.1dB);0:RFD,1:KD��
		WarnSet iReflCheckThres;    //209-212�����ʱ궨�������(0.1dB);
		WarnSet iRadConstThres;     //213-216ϵͳ�궨��������(0.1dB);
		WarnSet iClutSupprThres;    //217-220�Ӳ���������(0.1dB);
		WarnSet iVelCheckThres;     //221-224�ٶȼ������(0.1m/s);
		WarnSet iSWCheckThres;      //225-228�׿�������(0.1m/s);
		WarnSet iZdrThres;          //229-232 Zdr�������(0.1dB);
		PulseParam pulse[2];        //233-284�������
	};

#pragma pack()


	struct Xmt_CMA_CD
	{
		union {
			struct
			{
				ULONGLONG p2:1;           //2��Sָʾ
				ULONGLONG p1:1;           //1��Sָʾ
				ULONGLONG lctrl:1;        //����ָʾ
				ULONGLONG rctrl:1;        //ң��ָʾ
				ULONGLONG low:1;          //��ѹָʾ
				ULONGLONG delay:1;        //��ʱָʾ
				ULONGLONG ready:1;        //׼��ָʾ
				ULONGLONG high:1;         //��ѹָʾ
	
				ULONGLONG fan:1;          //�ٵ��ܷ�ѹ
				ULONGLONG plc:1;          //PLC��Դ
				ULONGLONG dtrig:1;        //�ŵ紥��
				ULONGLONG cooling:1;      //��ȴ����
				ULONGLONG oilhot:1;       //���¹���
				ULONGLONG igbt:1;         //IGBT����
				ULONGLONG vacion:1;       //�ѱù���
				ULONGLONG bias:1;         //ƫ��Ƿ��
	
				ULONGLONG filaover:1;     //��˿����
				ULONGLONG filalack:1;     //��˿Ƿ��
				ULONGLONG door:1;         //�ſ���
				ULONGLONG klyhot:1;       //�������
				ULONGLONG antsafe:1;      //���߰�ȫ
				ULONGLONG energy:1;       //���ܹ���
				ULONGLONG npeak:1;        //�������
				ULONGLONG collect:1;      //�ظ�ѹ���ϻ���
	
				ULONGLONG end_AmpSrc:1;   //ĩ�����ŵ�Դ
				ULONGLONG front_AmpSrc:1; //ǰ�����ŵ�Դ
				ULONGLONG front_Amp2us:1; //ǰ������2us
				ULONGLONG front_Amp1us:1; //ǰ������1us
				ULONGLONG front_AmpClk:1; //ǰ������ʱ��
				ULONGLONG front_AmpTrig:1;//ǰ�����Ŵ���
				ULONGLONG front_AmpPow:1; //ǰ�����Ź���
				ULONGLONG front_AmpExc:1; //ǰ�����ż���
	
				ULONGLONG coll:1;         //ע��������
				ULONGLONG fback:1;        //��������
				ULONGLONG ctrig:1;        //��紥��
				ULONGLONG tri1:1;         //�ɿع�1
				ULONGLONG tri2:1;         //�ɿع�2
				ULONGLONG tri3:1;         //�ɿع�3
				ULONGLONG tri4:1;         //�ɿع�4
				ULONGLONG tri5:1;         //�ɿع�5
	
				ULONGLONG tri6:1;         //�ɿع�6
				ULONGLONG trifan:1;       //�ɿع���
				ULONGLONG resrv:22;       //����
			};
			struct
			{
				ULONGLONG m_status;
			};
		};
		short  RectVolt;    //������ѹ:0.1(V);480��520V
		short  HighPwrAmp;  //�߹��ʵ���:0.01(A);2.0��6.0A
		short  FilaAmp;     //��˿����:0.01(A);5.0��7.5A
		short  BiasAmp;     //ƫ�ŵ���:0.01(A);2.0��8.0A
		short  Monit5V;     //������+5V:0.01(V);5��1V
		short  Monit24V;    //������+24V:0.01(V);24��1V
		short  Monit12V;    //������+12V:0.01(V);12��1V
		short  Monit_12V;   //������-12V:0.01(V);-12��1V
		short  CollAmp;     //ע����:0.01(mA);30��45mA
		short  nPeakAmp;    //�������:0.01(mA);��55mA
		short  VacionVolt;  //�ѱõ�ѹ:0.01(KV);2.8��4.5kv
		short  VacionAmp;   //�ѱõ���0.01(uA);��20uA
		short  Chg15V;      //������+15V:0.01(V);15��1V
		short  Chg_15V;     //������-15V:0.01(V);-15��1V
		short  Trig12V;     //��������+12V:0.01(V);12��1V
		short  Trig24V;     //��������+24V:0.01(V);24��2V
		short  Front12V;    //ǰ������+12V:0.01(V);12��1V
		short  Front24V;    //ǰ������+24V:0.01(V);12��1V
		short  iresrv[18];  //����
	};
	
	typedef struct _CDStructure
	{
		struct Whole_CMA m_ws;      //����״̬�ֽ�
		struct PwrMcu_CMA m_pwrmcu; //���+�����
		struct Rcv_CMA m_recv;      //����
		struct Svo_CMA m_sv;        //�ŷ�
		struct Xmt_CMA_CD m_xmt;       //����
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

 //		unsigned int xmtBit0 : 1;			//��ȴ����
 //		unsigned int xmtBit1 : 1;			//�ų�����
 //		unsigned int xmtBit2 : 1;			//��ɨ����
 //		unsigned int xmtBit3 : 1;			//�޳�紥��
 //		unsigned int xmtBit4 : 1;			//�޷ŵ紥��
 //		unsigned int xmtBit5 : 1;			//��ռ�ձ�
 //		unsigned int xmtBit6 : 1;			//��̬����
 //		unsigned int xmtBit7 : 1;			//��̬Ƿ���
 //		unsigned int xmtBit8 : 1;			//�ų���Դ1
 //		unsigned int xmtBit9 : 1;			//�ų���Դ2
 //		unsigned int xmtBit10 : 1;			//��˿��Դ
 //		unsigned int xmtBit11 : 1;			//��Դͨ��
 //		unsigned int xmtBit12 : 1;			//��ɨ��Դ
 //		unsigned int xmtBit13 : 1;			//������
 //		unsigned int xmtBit14 : 1;			//�˹��߹�ѹ
 //		unsigned int xmtBit15 : 1;			//�˹��ߵ�ѹ��
 //		unsigned int xmtBit16 : 1;			//����
 //		unsigned int xmtBit17 : 1;			//�ɿع�
 //		unsigned int xmtBit18 : 1;			//�ɿع���
 //		unsigned int xmtBit19 : 1;			//KLY����
 //		unsigned int xmtBit20 : 1;			//KLY����
 //		unsigned int xmtBit21 : 1;			//KLY�����ڵ�
 //		unsigned int xmtBit22 : 1;			//KLY����ڵ�
 //		unsigned int xmtBit23 : 1;			//KLY�¶�
 //		unsigned int xmtBit24 : 1;			//�߰��¶�
 //		unsigned int xmtBit25 : 1;			//�ѱ�Ƿѹ
 //		unsigned int xmtBit26 : 1;			//�ѱù���
 //		unsigned int xmtBit27 : 1;			//+15V��Դ����ѹ��Դ��
 //		unsigned int xmtBit28 : 1;			//+15V��Դ����������
 //		unsigned int xmtBit29 : 1;			//+24V��Դ����أ�
 //		unsigned int xmtBit30 : 1;			//+24V��Դ����������
 //		unsigned int xmtBit31 : 1;			//+60V��Դ����������
 //		unsigned int xmtBit32 : 1;			//�����¶ȹ���
 //		unsigned int xmtBit33 : 1;			//�����¶ȹ���
 //		unsigned int xmtBit34 : 1;			//�����ſ���
 //		unsigned int xmtBit35 : 1;			//�������ſ���
 //		unsigned int xmtBit36 : 1;			//��������
 //		unsigned int xmtBit37 : 11;			//Ԥ����38-63
	//	unsigned int xmtBit38 : 16;			//Ԥ����38-63

 //		unsigned int rcvBit0 : 1; //H·��ͨ��
 //		unsigned int rcvBit1 : 1; //H·��ͨ��
 //		unsigned int rcvBit2 :  1; //V·��ͨ��
 //		unsigned int rcvBit3 : 1; //V·��ͨ��
 //		unsigned int rcvBit4 : 1; //һ����
 //		unsigned int rcvBit5 : 1; //������
 //		unsigned int rcvBit6 : 1; //����
 //		unsigned int rcvBit7 : 1; //150Mzʱ��1
 //		unsigned int rcvBit8 : 1; //�ϱ�Ƶ
 //		unsigned int rcvBit9 : 1; //84MHzʱ��
 //		unsigned int rcvBit10 : 1; //150Mzʱ��2
 //		unsigned int rcvBit11 : 1; //����ֱ����Դ
 //		unsigned int rcvBit12 : 1; //DDSʱ��
 //		unsigned int rcvBit13 : 1; //AD����ʱ��
 //		unsigned int rcvBit14 : 1; //ϵͳʱ��
 //		unsigned int rcvBit15 : 1; //Burstͨ��
 //		unsigned int rcvBit16 : 1; //��ͨ��H1������ƽ�澯
 //		unsigned int rcvBit17 : 1; //��ͨ��H2������ƽ�澯
 //		unsigned int rcvBit18 : 1; //��ͨ��H1����澯
 //		unsigned int rcvBit19 : 1; //��ͨ��H2����澯
 //		unsigned int rcvBit20 : 1; //ͨ�������澯
 //		unsigned int rcvBit21 : 1; //����ϵ���澯
 //		unsigned int rcvBit22 : 1; //Burstͨ������澯
 //		unsigned int rcvBit23 : 1; //Burstͨ��������ƽ�澯
 //		unsigned int rcvBit24 : 1; //һ�����źŸ澯
 //		unsigned int rcvBit25 : 1; //�����źŸ澯
 //		unsigned int rcvBit26 : 1; //DDS�����źŸ澯
 //		unsigned int rcvBit27 : 5; //Ԥ����28-47
 //		unsigned int rcvBit28 : 16; //Ԥ����28-47
 //
 //		unsigned int svBit0 : 1; //��IQ����
 //		unsigned int svBit1 : 1; //���ݶ���
 //		unsigned int svBit2 : 1; //�����������ʧ��
 //		unsigned int svBit3 : 1; //�޲������
 //		unsigned int svBit4 : 1; //�洢�ռ����
 //		unsigned int svBit5 : 11; //Ԥ��:27��bit
 //		unsigned int svBit6 : 16; //Ԥ��:27��bit
 //
 //		unsigned int svBit7 : 1; //��λ������
 //		unsigned int svBit8 : 1; //����������
 //		unsigned int svBit9 : 1; //R/D����
 //		unsigned int svBit10 : 1; //ң/����
 //		unsigned int svBit11 : 1; //+15V����
 //		unsigned int svBit12 : 1; //-15V����
 //		unsigned int svBit13 : 1; //������λ
 //		unsigned int svBit14 : 1; //��λ��λ
 //		unsigned int svBit15 : 1; //�嶥����
 //		unsigned int svBit16 : 1; //��׹���
 //		unsigned int svBit17 : 1; //���߷�λ��������
 //		unsigned int svBit18 : 1; //���߸�����������
 //		unsigned int svBit19 : 1; //��λ��λ���
 //		unsigned int svBit20 : 1; //������λ���
 //		unsigned int svBit21 : 1; //���߶�λ����
 //		unsigned int svBit22 : 1; //Ԥ����49��bit
 //		unsigned int svBit23 : 16; //Ԥ����49��bit
 //		unsigned int svBit24 : 16; //Ԥ����49��bit
 //		unsigned int svBit25 : 16; //Ԥ����49��bit
 //
 //		unsigned int svBit26 : 1; //��������
 //		unsigned int svBit27 : 1; //�ŵ紥���ź�
 //		unsigned int svBit28 : 1; //��紥���ź�
 //		unsigned int svBit29 : 1; //��ǰ�����ź�
 //		unsigned int svBit30 : 1; //���������ź�
 //		unsigned int svBit31 : 1; //ͨ�絥Ԫ1
 //		unsigned int svBit32 : 1; //ͨ�絥Ԫ2
 //		unsigned int svBit33 : 9; //Ԥ����25��bit
 //		unsigned int svBit34 : 16; //Ԥ����25��bit
 //
 //		unsigned int pwrBit0 : 1; //380V����
 //		unsigned int pwrBit1 : 1; //�����Դ
 //		unsigned int pwrBit2 : 1; //UPS��Դ
 //		unsigned int pwrBit3 : 1; //���յ�Դ
 //		unsigned int pwrBit4 : 1; //�ŷ���Դ
 //		unsigned int pwrBit5 : 1; //��Դת��
 //		unsigned int pwrBit6 : 1; //���ʼ��
 //		unsigned int pwrBit7 : 1; //Ԥ����1��bit
 //		unsigned int pwrBit8 : 1; //A���ѹ
 //		unsigned int pwrBit9 : 1; //B���ѹ
 //		unsigned int pwrBit10 : 1; //C���ѹ
 //		unsigned int pwrBit11 : 1; //A�����
 //		unsigned int pwrBit12: 1; //B�����
 //		unsigned int pwrBit13 : 1; //C�����
 //		unsigned int pwrBit14 : 2; //Ԥ����34��bit
 //		unsigned int pwrBit15 : 16; //Ԥ����34��bit
 //
 //		unsigned int pwrBit17 : 1; //��ǰ�����ź�
 //		unsigned int pwrBit18 : 15; //Ԥ����31��bit
 //		unsigned int pwrBit19 : 16; //Ԥ����31��bit
 //
 //		unsigned int pwrBit20 : 1; //����ͨ��
 //		unsigned int pwrBit21 : 1; //����ͨ��
 //		unsigned int pwrBit22 : 1; //�źŴ���ͨ��
 //		unsigned int pwrBit23 : 1; //�ŷ�ͨ��
 //		unsigned int pwrBit24 : 1; //���չ��ʼ��ͨ��
 //		unsigned int pwrBit25 : 1; //KLY���ʼ��ͨ��
 //		unsigned int pwrBit26 : 1; //���ͨ��
 //		unsigned int pwrBit27 : 1; //���߹��ʼ��ͨ��
 //		unsigned int pwrBit28 : 1; //ʱ�������ͨ��
 //		unsigned int pwrBit29 : 1; //����������ͨ��
 //		unsigned int pwrBit30 : 1; //UPS��Դͨ��
 //		unsigned int pwrBit31 : 1; //��ʪ����1-������ͨ��
 //		unsigned int pwrBit32 : 1; //��ʪ����2-ת̨ͨ��
 //		unsigned int pwrBit33 : 1; //��ʪ����3-����ͨ��
 //		unsigned int pwrBit34 : 1; //��ʪ����4-����ͨ��
 //		unsigned int pwrBit35 : 1; //��ʪ����5-����ͨ��
 //		unsigned int pwrBit37 : 16; //Ԥ����30��bit
 //
 //
 //		unsigned int pwrBit38 : 1; //��������ѹ�澯
 //		unsigned int pwrBit39 : 1; //�������¶ȸ澯
 //		unsigned int pwrBit40 : 14; //Ԥ����35��bit
 //		unsigned int pwrBit41 : 16; //Ԥ����35��bit
 //		unsigned int pwrBit42 : 1; //Ԥ����35��bit
 //		unsigned int pwrBit43 : 1; //Ԥ����35��bit
 //		unsigned int pwrBit44 : 1; //Ԥ����35��bit
 //		unsigned int pwrBit45 : 1; //Ԥ����35��bit
 //		unsigned int pwrBit46 : 1; //Ԥ����35��bit
 //
 //		unsigned int pwrBit47 : 1; //�������¶ȸ澯
 //		unsigned int pwrBit48 : 1; //������ʪ�ȸ澯
 //		unsigned int pwrBit49 : 1; //ת̨�¶ȸ澯
 //		unsigned int pwrBit50 : 1; //ת̨ʪ�ȸ澯
 //		unsigned int pwrBit51 : 1; //�����¶ȸ澯
 //		unsigned int pwrBit52 : 1; //����ʪ�ȸ澯
 //		unsigned int pwrBit53 : 1; //�ۺϻ����¶ȸ澯
 //		unsigned int pwrBit54 : 1; //�ۺϻ���ʪ�ȸ澯
 //		unsigned int pwrBit55 : 1; //��������¶ȸ澯
 //		unsigned int pwrBit56 : 1; //�������ʪ�ȸ澯
 //		unsigned int pwrBit57 : 1; //�������¶ȸ澯
 //		unsigned int pwrBit58 : 16; //Ԥ����62��bit
 //		unsigned int pwrBit59 : 16; //Ԥ����62��bit
 //		unsigned int pwrBit60 : 16; //Ԥ����62��bit
 //		unsigned int pwrBit62 : 1; //ʱ�����������
 //		unsigned int pwrBit63 : 15; //Ԥ����31��bit
 //		unsigned int pwrBit64 : 16; //Ԥ����31��bit
 //
 //		unsigned int pwrBit65 : 1; //�״ﳣ��C
 //		unsigned int pwrBit66 : 1; //���곣��SYSCAL
 //		unsigned int pwrBit67 : 1; //��λ����
 //		unsigned int pwrBit68 : 1; //�Ӳ�����
 //		unsigned int pwrBit69 : 1; //CW�������
 //		unsigned int pwrBit70 : 1; //RFD�������
 //		unsigned int pwrBit71 : 1; //KD�������
 //		unsigned int pwrBit72 : 1; //�ٶȲ������
 //		unsigned int pwrBit73 : 8; //Ԥ����24��bit
 //		unsigned int pwrBit74 : 16; //Ԥ����24��bit

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
				
 				short xmtBit0 : 1;			//��ȴ����
 				short xmtBit1 : 1;			//�ų�����
 				short xmtBit2 : 1;			//��ɨ����
 				short xmtBit3 : 1;			//�޳�紥��
 				short xmtBit4 : 1;			//�޷ŵ紥��
 				short xmtBit5 : 1;			//��ռ�ձ�
 				short xmtBit6 : 1;			//��̬����
 				short xmtBit7 : 1;			//��̬Ƿ���
  				short xmtBit8 : 1;			//�ų���Դ1
  				short xmtBit9 : 1;			//�ų���Դ2
  				short xmtBit10 : 1;			//��˿��Դ
  				short xmtBit11 : 1;			//��Դͨ��
  				short xmtBit12 : 1;			//��ɨ��Դ
  				short xmtBit13 : 1;			//������
  				short xmtBit14 : 1;			//�˹��߹�ѹ
  				short xmtBit15 : 1;			//�˹��ߵ�ѹ��
 			};
				
		};	

		union 
		{
			short bitset2;
			struct  
			{
				short xmtBit16 : 1;			//����
				short xmtBit17 : 1;			//�ɿع�
				short xmtBit18 : 1;			//�ɿع���
				short xmtBit19 : 1;			//KLY����
				short xmtBit20 : 1;			//KLY����
				short xmtBit21 : 1;			//KLY�����ڵ�
				short xmtBit22 : 1;			//KLY����ڵ�
				short xmtBit23 : 1;			//KLY�¶�
				short xmtBit24 : 1;			//�߰��¶�
				short xmtBit25 : 1;			//�ѱ�Ƿѹ
				short xmtBit26 : 1;			//�ѱù���
				short xmtBit27 : 1;			//+15V��Դ����ѹ��Դ��
				short xmtBit28 : 1;			//+15V��Դ����������
				short xmtBit29 : 1;			//+24V��Դ����أ�
				short xmtBit30 : 1;			//+24V��Դ����������
				short xmtBit31 : 1;			//+60V��Դ����������
			};
		};

	
		union
		{
			short bitset3;
			struct  
			{
				short xmtBit32 : 1;			//�����¶ȹ���
				short xmtBit33 : 1;			//�����¶ȹ���
				short xmtBit34 : 1;			//�����ſ���
				short xmtBit35 : 1;			//�������ſ���
				short xmtBit36 : 1;			//��������
				short xmtBit37 : 11;			//Ԥ����38-63
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
 				short rcvBit0 : 1; //H·��ͨ��
 				short rcvBit1 : 1; //H·��ͨ��
 				short rcvBit2 : 1; //V·��ͨ��
 				short rcvBit3 : 1; //V·��ͨ��
 				short rcvBit4 : 1; //һ����
 				short rcvBit5 : 1; //������
 				short rcvBit6 : 1; //����
 				short rcvBit7 : 1; //150Mzʱ��1
 				short rcvBit8 : 1; //�ϱ�Ƶ
 				short rcvBit9 : 1; //84MHzʱ��
 				short rcvBit10 : 1; //150Mzʱ��2
 				short rcvBit11 : 1; //����ֱ����Դ
 				short rcvBit12 : 1; //DDSʱ��
 				short rcvBit13 : 1; //AD����ʱ��
 				short rcvBit14 : 1; //ϵͳʱ��
 				short rcvBit15 : 1; //Burstͨ��
 			};
 		};

 		union
 		{
 			short bitset6;
 			struct 
 			{
 				short rcvBit16 : 1; //��ͨ��H1������ƽ�澯
 				short rcvBit17 : 1; //��ͨ��H2������ƽ�澯
 				short rcvBit18 : 1; //��ͨ��H1����澯
 				short rcvBit19 : 1; //��ͨ��H2����澯
 				short rcvBit20 : 1; //ͨ�������澯
 				short rcvBit21 : 1; //����ϵ���澯
 				short rcvBit22 : 1; //Burstͨ������澯
 				short rcvBit23 : 1; //Burstͨ��������ƽ�澯
 				short rcvBit24 : 1; //һ�����źŸ澯
 				short rcvBit25 : 1; //�����źŸ澯
 				short rcvBit26 : 1; //DDS�����źŸ澯
 				short rcvBit27 : 5; //Ԥ����28-47
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
					short svBit0 : 1; //��IQ����
					short svBit1 : 1; //���ݶ���
					short svBit2 : 1; //�����������ʧ��
					short svBit3 : 1; //�޲������
					short svBit4 : 1; //�洢�ռ����
					short svBit5 : 11; //Ԥ��:27��bit
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
				short svBit7 : 1; //��λ������
				short svBit8 : 1; //����������
				short svBit9 : 1; //R/D����
				short svBit10 : 1; //ң/����
				short svBit11 : 1; //+15V����
				short svBit12 : 1; //-15V����
				short svBit13 : 1; //������λ
				short svBit14 : 1; //��λ��λ
				short svBit15 : 1; //�嶥����
				short svBit16 : 1; //��׹���
				short svBit17 : 1; //���߷�λ��������
				short svBit18 : 1; //���߸�����������
				short svBit19 : 1; //��λ��λ���
				short svBit20 : 1; //������λ���
				short svBit21 : 1; //���߶�λ����
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
				short svBit26 : 1; //��������
				short svBit27 : 1; //�ŵ紥���ź�
				short svBit28 : 1; //��紥���ź�
				short svBit29 : 1; //��ǰ�����ź�
				short svBit30 : 1; //���������ź�
				short svBit31 : 1; //ͨ�絥Ԫ1
				short svBit32 : 1; //ͨ�絥Ԫ2
				short svBit33 : 9; //Ԥ����25��bit
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
				short pwrBit0 : 1; //380V����
				short pwrBit1 : 1; //�����Դ
				short pwrBit2 : 1; //UPS��Դ
				short pwrBit3 : 1; //���յ�Դ
				short pwrBit4 : 1; //�ŷ���Դ
				short pwrBit5 : 1; //��Դת��
				short pwrBit6 : 1; //���ʼ��
				short pwrBit7 : 1; //Ԥ����1��bit
				short pwrBit8 : 1; //A���ѹ
				short pwrBit9 : 1; //B���ѹ
				short pwrBit10 : 1; //C���ѹ
				short pwrBit11 : 1; //A�����
				short pwrBit12: 1; //B�����
				short pwrBit13 : 1; //C�����
				short pwrBit14 : 2; //Ԥ����34��bit
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
				short pwrBit17 : 1; //��ǰ�����ź�
				short pwrBit18 : 15; //Ԥ����31��bit
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
				short pwrBit20 : 1; //����ͨ��
				short pwrBit21 : 1; //����ͨ��
				short pwrBit22 : 1; //�źŴ���ͨ��
				short pwrBit23 : 1; //�ŷ�ͨ��
				short pwrBit24 : 1; //���չ��ʼ��ͨ��
				short pwrBit25 : 1; //KLY���ʼ��ͨ��
				short pwrBit26 : 1; //���ͨ��
				short pwrBit27 : 1; //���߹��ʼ��ͨ��
				short pwrBit28 : 1; //ʱ�������ͨ��
				short pwrBit29 : 1; //����������ͨ��
				short pwrBit30 : 1; //UPS��Դͨ��
				short pwrBit31 : 1; //��ʪ����1-������ͨ��
				short pwrBit32 : 1; //��ʪ����2-ת̨ͨ��
				short pwrBit33 : 1; //��ʪ����3-����ͨ��
				short pwrBit34 : 1; //��ʪ����4-����ͨ��
				short pwrBit35 : 1; //��ʪ����5-����ͨ��
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
				short pwrBit38 : 1; //��������ѹ�澯
				short pwrBit39 : 1; //�������¶ȸ澯
				short pwrBit40 : 14; //Ԥ����35��bit
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
				short pwrBit42 : 1; //Ԥ����35��bit
				short pwrBit43 : 1; //Ԥ����35��bit
				short pwrBit44 : 1; //Ԥ����35��bit
				short pwrBit45 : 1; //Ԥ����35��bit
				short pwrBit46 : 1; //Ԥ����35��bit
				short pwrBit47 : 1; //�������¶ȸ澯
				short pwrBit48 : 1; //������ʪ�ȸ澯
				short pwrBit49 : 1; //ת̨�¶ȸ澯
				short pwrBit50 : 1; //ת̨ʪ�ȸ澯
				short pwrBit51 : 1; //�����¶ȸ澯
				short pwrBit52 : 1; //����ʪ�ȸ澯
				short pwrBit53 : 1; //�ۺϻ����¶ȸ澯
				short pwrBit54 : 1; //�ۺϻ���ʪ�ȸ澯
				short pwrBit55 : 1; //��������¶ȸ澯
				short pwrBit56 : 1; //�������ʪ�ȸ澯
				short pwrBit57 : 1; //�������¶ȸ澯
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
				short pwrBit62 : 1; //ʱ�����������
				short pwrBit63 : 15; //Ԥ����31��bit
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
				short pwrBit65 : 1; //�״ﳣ��C
				short pwrBit66 : 1; //���곣��SYSCAL
				short pwrBit67 : 1; //��λ����
				short pwrBit68 : 1; //�Ӳ�����
				short pwrBit69 : 1; //CW�������
				short pwrBit70 : 1; //RFD�������
				short pwrBit71 : 1; //KD�������
				short pwrBit72 : 1; //�ٶȲ������
				short pwrBit73 : 8; //Ԥ����24��bit
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

