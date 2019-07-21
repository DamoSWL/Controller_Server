#include "SCAlarm.h"
#include "mysqldatabase.h"

CSCAlarm::CSCAlarm(void)
{

	m_SCExpFlag["rf_power_smoothing"] = false;
	m_SCExpFlag["hxmtlosssingle"] = false;
	m_SCExpFlag["hxmtlosshv"] = false;
	m_SCExpFlag["hrcvloss"] = false;
	m_SCExpFlag["htestloss"] = false;
	m_SCExpFlag["vxmtlosshv"] = false;
	m_SCExpFlag["vrcvloss"] = false;
	m_SCExpFlag["vtestloss"] = false;
	m_SCExpFlag["pathloss_noise_h"] = false;
	m_SCExpFlag["pathloss_noise_v"] = false;
	m_SCExpFlag["ts_cw"] = false;
	m_SCExpFlag["ts_noise"] = false;
	m_SCExpFlag["pathloss_rf"] = false;
	m_SCExpFlag["ts_rf"] = false;
	m_SCExpFlag["pathloss_kly"] = false;
	m_SCExpFlag["gas_loss"] = false;
	m_SCExpFlag["radome_loss"] = false;
	m_SCExpFlag["feederPressure"] = false;
	m_SCExpFlag["feederHumidity"] = false;
	m_SCExpFlag["temp"] = false;
	m_SCExpFlag["humi"] = false;
	m_SCExpFlag["magn1Amp"] = false;
	m_SCExpFlag["magn2Amp"] = false;
	m_SCExpFlag["filaAmp"] = false;
	m_SCExpFlag["biasAmp"] = false;
	m_SCExpFlag["collAmp"] = false;
	m_SCExpFlag["vacionAmp"] = false;
	m_SCExpFlag["vacionVolt"] = false;
	m_SCExpFlag["front12V"] = false;
	m_SCExpFlag["front24V"] = false;
	m_SCExpFlag["front36V"] = false;
	m_SCExpFlag["end12V"] = false;
	m_SCExpFlag["end24V"] = false;
	m_SCExpFlag["end36V"] = false;
	m_SCExpFlag["highPwrAmp"] = false;
	m_SCExpFlag["volt510"] = false;
	m_SCExpFlag["nPeakAmp"] =  false;
	m_SCExpFlag["volt24V"] = false;
	m_SCExpFlag["volt15V"] = false;
	m_SCExpFlag["volt_15V"] =  false;
	m_SCExpFlag["trig12V"] = false;
	m_SCExpFlag["trig200V"] =  false;
	m_SCExpFlag["rcv5V"] = false;
	m_SCExpFlag["rcv_5V"] = false;
	m_SCExpFlag["rcv12V"] = false;
	m_SCExpFlag["rcv15V"] = false;
	m_SCExpFlag["rcv24V"] = false;
	m_SCExpFlag["hFiledAmp"] = false;
	m_SCExpFlag["vFiledAmp"] = false;
	m_SCExpFlag["hPresel"] = false;
	m_SCExpFlag["vPresel"] = false;
	m_SCExpFlag["hMixer"] = false;
	m_SCExpFlag["vMixer"] = false;
	m_SCExpFlag["hFront"] = false;
	m_SCExpFlag["vFront"] = false;
	m_SCExpFlag["hDigit"] = false;
	m_SCExpFlag["vDigit"] = false;
	m_SCExpFlag["testSignalF"] = false;
	m_SCExpFlag["testSignalB"] = false;
	m_SCExpFlag["hFiledFront"] = false;
	m_SCExpFlag["vFiledFront"] = false;
	m_SCExpFlag["excitOut"] = false;
	m_SCExpFlag["testOut"] = false;
	m_SCExpFlag["intrinOut"] = false;
	m_SCExpFlag["COHO"] = false;
	m_SCExpFlag["RFD"] = false;
	m_SCExpFlag["hKD"] = false;
	m_SCExpFlag["vKD"] = false;
	m_SCExpFlag["pAUp"] = false;
	m_SCExpFlag["pADown"] = false;
	m_SCExpFlag["hDischg"] = false;
	m_SCExpFlag["vDischg"] = false;
	m_SCExpFlag["upLIC"] = false;
	m_SCExpFlag["upHLIC"] = false;
	m_SCExpFlag["upVLIC"] = false;
	m_SCExpFlag["downLIC"] = false;
	m_SCExpFlag["refLIC"] = false;
	m_SCExpFlag["caseTemp"] = false;
	m_SCExpFlag["sv5V"] = false;
	m_SCExpFlag["sv24V"] =  false;
	m_SCExpFlag["azAmp"] =  false;
	m_SCExpFlag["azCmdRmp"] = false;
	m_SCExpFlag["azCurRmp"] = false;
	m_SCExpFlag["elAmp"] = false;
	m_SCExpFlag["elCmdRmp"] = false;
	m_SCExpFlag["elCurRmp"] = false;
	m_SCExpFlag["aVolt"] =  false;
	m_SCExpFlag["aAmp"] =  false;
	m_SCExpFlag["bVolt"] =  false;
	m_SCExpFlag["bAmp"] =  false;
	m_SCExpFlag["cVolt"] =  false;
	m_SCExpFlag["cAmp"] =  false;
	m_SCExpFlag["feederPressure"] = false;
	m_SCExpFlag["feederHumidity"] = false;
	m_SCExpFlag["ant_temp"] = false;
	m_SCExpFlag["ant_humidity"] = false;
	m_SCExpFlag["roomTemp"] = false;
	m_SCExpFlag["roomHumidity"] = false;


	m_SCContent["rf_power_smoothing"] = "��Ƶ����ƽ��ϵ��";
	m_SCContent["hxmtlosssingle"] = "ˮƽ����ˮƽͨ������֧·���";
	m_SCContent["hxmtlosshv"] = "˫ͨ������ˮƽ����֧·���";
	m_SCContent["hrcvloss"] = "ˮƽͨ������֧·���";
	m_SCContent["htestloss"] = "ˮƽͨ�������ź����";
	m_SCContent["vxmtlosshv"] = "˫ͨ�����䴹ֱ����֧·���";
	m_SCContent["vrcvloss"] = "��ֱͨ������֧·���";
	m_SCContent["vtestloss"] = "��ֱͨ�������ź����";
	m_SCContent["pathloss_noise_h"] = "ˮƽͨ����Ƶ���������ź����";
	m_SCContent["pathloss_noise_v"] = "��ֱͨ����Ƶ���������ź����";
	m_SCContent["ts_cw"] = "�����������źŹ���";
	m_SCContent["ts_noise"] = "����Դ�����źų����";
	m_SCContent["pathloss_rf"] = "��Ƶ�����ź�·�����";
	m_SCContent["ts_rf"] = "��Ƶ�����źŹ���";
	m_SCContent["pathloss_kly"] = "�ٵ�������ź�·�����";
	m_SCContent["gas_loss"] = "����˥��ֵ";
	m_SCContent["radome_loss"] = "������˫�����";
	m_SCContent["temp"] = "������¶�";
	m_SCContent["humi"] = "�����ʪ��";
	m_SCContent["magn1Amp"] = "�ų������";
	m_SCContent["magn2Amp"] = "�ų������";
	m_SCContent["filaAmp"] = "��˿����";
	m_SCContent["biasAmp"] = "ƫ�ŵ���";
	m_SCContent["collAmp"] = "ע����";
	m_SCContent["vacionAmp"] = "�ѱõ���";
	m_SCContent["vacionVolt"] = "�ѱõ�ѹ";
	m_SCContent["front12V"] = "ǰ������+12V";
	m_SCContent["front24V"] = "ǰ������+24V";
	m_SCContent["front36V"] = "ǰ������+36V";
	m_SCContent["end12V"] = "ĩ������+12V";
	m_SCContent["end24V"] = "ĩ������+24V";
	m_SCContent["end36V"] = "ĩ������+36V";
	m_SCContent["highPwrAmp"] = "�߹��ʵ���";
	m_SCContent["volt510"] = "+510V";
	m_SCContent["nPeakAmp"] = "�������";
	m_SCContent["volt24V"] = "�ɿع���+24V";
	m_SCContent["volt15V"] = "���+15V";
	m_SCContent["volt_15V"] = "���-15V";
	m_SCContent["trig12V"] = "����+12V";
	m_SCContent["trig200V"] = "����+200V";
	m_SCContent["rcv5V"] = "+5Vֱ����ѹ��Դ";
	m_SCContent["rcv_5V"] = "-5Vֱ����ѹ��Դ";
	m_SCContent["rcv12V"] = "+12Vֱ����ѹ��Դ";
	m_SCContent["rcv15V"] = "+15Vֱ����ѹ��Դ";
	m_SCContent["rcv24V"] = "+24Vֱ����ѹ��Դ";
	m_SCContent["hFiledAmp"] = "H·���ŵ���";
	m_SCContent["vFiledAmp"] = "V·���ŵ���";
	m_SCContent["hPresel"] = "H·����Ԥѡ��";
	m_SCContent["vPresel"] = "V·����Ԥѡ��";
	m_SCContent["hMixer"] = "H·��Ƶ��";
	m_SCContent["vMixer"] = "V·��Ƶ��";
	m_SCContent["hFront"] = "H·ǰ����Ƶ�Ŵ���";
	m_SCContent["vFront"] = "V·ǰ����Ƶ�Ŵ���";
	m_SCContent["hDigit"] = "������Ƶ������H·";
	m_SCContent["vDigit"] = "������Ƶ������V·";
	m_SCContent["testSignalF"] ="����˥����ǰ���������ź�";
	m_SCContent["testSignalB"] ="����˥���������������ź�";
	m_SCContent["hFiledFront"] ="H·����ǰ���������ź�";
	m_SCContent["vFiledFront"] ="V·����ǰ���������ź�";
	m_SCContent["excitOut"] = "����Դ����ź�";
	m_SCContent["testOut"] = "����Դ����ź�";
	m_SCContent["intrinOut"] = "����Դ����ź�";
	m_SCContent["COHO"] = "30MHz��ɻ�׼�ź�";
	m_SCContent["RFD"] = "RFD�ź�";
	m_SCContent["hKD"] = "H·KD�ź�";
	m_SCContent["vKD"] = "V·KD�ź�";
	m_SCContent["pAUp"] = "�����Ϲ����ź�";
	m_SCContent["pADown"] = "�����¹����ź�";
	m_SCContent["hDischg"] = "H�ŵ���ź�";
	m_SCContent["vDischg"] = "V�ŵ���ź�";
	m_SCContent["upLIC"] = "�����Ϸ�������ź�";
	m_SCContent["upHLIC"] = "������H��������ź�";
	m_SCContent["upVLIC"] = "������V��������ź�";
	m_SCContent["downLIC"] = "�����·�������ź�";
	m_SCContent["refLIC"] = "�����·�������ź�";
	m_SCContent["caseTemp"] = "�������¶�";
	m_SCContent["sv5V"] = "+5V��Դ";
	m_SCContent["sv24V"] = "+24V��Դ";
	m_SCContent["azAmp"] = "��λ����";
	m_SCContent["azCmdRmp"] = "��λ����ת��";
	m_SCContent["azCurRmp"] = "��λ��ǰת��";
	m_SCContent["elAmp"] = "��������";
	m_SCContent["elCmdRmp"] = "��������ת��";
	m_SCContent["elCurRmp"] ="������ǰת��";
	m_SCContent["aVolt"] = "A���ѹ";
	m_SCContent["aAmp"] = "A�����";
	m_SCContent["bVolt"] = "B���ѹ";
	m_SCContent["bAmp"] = "B�����";
	m_SCContent["cVolt"] = "C���ѹ";
	m_SCContent["cAmp"] = "C�����";
	m_SCContent["feederPressure"] = "����ǻ����ѹ";
	m_SCContent["feederHumidity"] = "����ǻ�ڿ��������";
	m_SCContent["ant_temp"] = "�������¶�";
	m_SCContent["ant_humidity"] = "������ʪ��";
	m_SCContent["roomTemp"] = "�����¶�";
	m_SCContent["roomHumidity"] = "����ʪ��";

	m_alarmType["rf_power_smoothing"] = std::string("");
	m_alarmType["hxmtlosssingle"] = std::string("");
	m_alarmType["hxmtlosshv"] = std::string("");
	m_alarmType["hrcvloss"] = std::string("");
	m_alarmType["htestloss"] = std::string("");
	m_alarmType["vxmtlosshv"] = std::string("");
	m_alarmType["vrcvloss"] = std::string("");
	m_alarmType["vtestloss"] = std::string("");
	m_alarmType["pathloss_noise_h"] = std::string("");
	m_alarmType["pathloss_noise_v"] = std::string("");
	m_alarmType["ts_cw"] = std::string("");
	m_alarmType["ts_noise"] = std::string("");
	m_alarmType["pathloss_rf"] = std::string("");
	m_alarmType["ts_rf"] = std::string("");
	m_alarmType["pathloss_kly"] = std::string("");
	m_alarmType["gas_loss"] = std::string("");
	m_alarmType["radome_loss"] = std::string("");
	m_alarmType["temp"] = std::string("");
	m_alarmType["humi"] = std::string("");
	m_alarmType["magn1Amp"] = std::string("");
	m_alarmType["magn2Amp"] = std::string("");
	m_alarmType["filaAmp"] = std::string("");
	m_alarmType["biasAmp"] = std::string("");
	m_alarmType["collAmp"] = std::string("");
	m_alarmType["vacionAmp"] = std::string("");
	m_alarmType["vacionVolt"] = std::string("");
	m_alarmType["front12V"] = std::string("");
	m_alarmType["front24V"] = std::string("");
	m_alarmType["front36V"] = std::string("");
	m_alarmType["end12V"] = std::string("");
	m_alarmType["end24V"] = std::string("");
	m_alarmType["end36V"] = std::string("");
	m_alarmType["highPwrAmp"] = std::string("");
	m_alarmType["volt510"] = std::string("");
	m_alarmType["nPeakAmp"] = std::string("");
	m_alarmType["volt24V"] = std::string("");
	m_alarmType["volt15V"] = std::string("");
	m_alarmType["volt_15V"] = std::string("");
	m_alarmType["trig12V"] = std::string("");
	m_alarmType["trig200V"] = std::string("");
	m_alarmType["rcv5V"] = std::string("");
	m_alarmType["rcv_5V"] = std::string("");
	m_alarmType["rcv12V"] = std::string("");
	m_alarmType["rcv15V"] = std::string("");
	m_alarmType["rcv24V"] = std::string("");
	m_alarmType["hFiledAmp"] = std::string("");
	m_alarmType["vFiledAmp"] = std::string("");
	m_alarmType["hPresel"] = std::string("");
	m_alarmType["vPresel"] = std::string("");
	m_alarmType["hMixer"] = std::string("");
	m_alarmType["vMixer"] = std::string("");
	m_alarmType["hFront"] = std::string("");
	m_alarmType["vFront"] = std::string("");
	m_alarmType["hDigit"] = std::string("");
	m_alarmType["vDigit"] = std::string("");
	m_alarmType["testSignalF"] = std::string("");
	m_alarmType["testSignalB"] = std::string("");
	m_alarmType["hFiledFront"] = std::string("");
	m_alarmType["vFiledFront"] = std::string("");
	m_alarmType["excitOut"] = std::string("");
	m_alarmType["testOut"] = std::string("");
	m_alarmType["intrinOut"] = std::string("");
	m_alarmType["COHO"] = std::string("");
	m_alarmType["RFD"] = std::string("");
	m_alarmType["hKD"] = std::string("");
	m_alarmType["vKD"] = std::string("");
	m_alarmType["pAUp"] = std::string("");
	m_alarmType["pADown"] = std::string("");
	m_alarmType["hDischg"] = std::string("");
	m_alarmType["vDischg"] = std::string("");
	m_alarmType["upLIC"] = std::string("");
	m_alarmType["upHLIC"] = std::string("");
	m_alarmType["upVLIC"] = std::string("");
	m_alarmType["downLIC"] = std::string("");
	m_alarmType["refLIC"] = std::string("");
	m_alarmType["caseTemp"] = std::string("");
	m_alarmType["sv5V"] = std::string("");
	m_alarmType["sv24V"] = std::string("");
	m_alarmType["azAmp"] = std::string("");
	m_alarmType["azCmdRmp"] = std::string("");
	m_alarmType["azCurRmp"] = std::string("");
	m_alarmType["elAmp"] = std::string("");
	m_alarmType["elCmdRmp"] = std::string("");
	m_alarmType["elCurRmp"] = std::string("");
	m_alarmType["aVolt"] = std::string("");
	m_alarmType["aAmp"] = std::string("");
	m_alarmType["bVolt"] = std::string("");
	m_alarmType["bAmp"] = std::string("");
	m_alarmType["cVolt"] = std::string("");
	m_alarmType["cAmp"] = std::string("");
	m_alarmType["feederPressure"] = std::string("");
	m_alarmType["feederHumidity"] = std::string("");
	m_alarmType["ant_temp"] = std::string("");
	m_alarmType["ant_humidity"] = std::string("");
	m_alarmType["roomTemp"] = std::string("");
	m_alarmType["roomHumidity"] = std::string("");

	m_ThreadholdValues["rf_power_smoothing"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hxmtlosssingle"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hxmtlosshv"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hrcvloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["htestloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vxmtlosshv"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vrcvloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vtestloss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_noise_h"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_noise_v"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ts_cw"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ts_noise"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_rf"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ts_rf"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pathloss_kly"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["gas_loss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["radome_loss"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["temp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["humi"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["magn1Amp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["magn2Amp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["filaAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["biasAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["collAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vacionAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vacionVolt"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["front12V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["front24V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["front36V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["end12V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["end24V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["end36V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["highPwrAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["volt510"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["nPeakAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["volt24V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["volt15V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["volt_15V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["trig12V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["trig200V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rcv5V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rcv_5V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rcv12V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rcv15V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rcv24V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hFiledAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vFiledAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hPresel"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vPresel"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hMixer"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vMixer"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hFront"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vFront"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hDigit"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vDigit"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["testSignalF"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["testSignalB"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hFiledFront"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vFiledFront"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["excitOut"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["testOut"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["intrinOut"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["COHO"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["RFD"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hKD"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vKD"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pAUp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pADown"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["hDischg"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["vDischg"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["upLIC"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["upHLIC"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["upVLIC"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["downLIC"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["refLIC"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["caseTemp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["sv5V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["sv24V"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["azAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["azCmdRmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["azCurRmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["elAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["elCmdRmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["elCurRmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["aVolt"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["aAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["bVolt"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["bAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["cVolt"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["cAmp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["feederPressure"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["feederHumidity"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_temp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_humidity"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["roomTemp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["roomHumidity"] = std::make_pair(0.0,0.0);

}

CSCAlarm::~CSCAlarm(void)
{
}


void CSCAlarm::getAlarmType()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);

	if(pDataBase)
	{	
		// try
		// {
			for(auto item = m_alarmType.cbegin(); item != m_alarmType.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256,"SELECT type FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmType,wxT("type"),this);	
			}
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		int length = 0;
			// 		char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
			// 		if(sqlTmpStr)
			// 		{
			// 			std::string eName(sqlTmpStr,strlen(sqlTmpStr));
			// 			sqlTmpStr = pDataBase->GetStringValue(index,"type",length);
			// 			if(sqlTmpStr)
			// 			{
			// 				std::string type(sqlTmpStr,strlen(sqlTmpStr));
			// 				m_alarmType[eName] = type;
			// 			}
			// 		}
			// 	}

			// 	pDataBase->FreeResult(sqlError);
			// }
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }

	}
}

void CSCAlarm::getThresholdValue()
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);

	if (pDataBase)
	{
		// try
		// {
			for(auto item = m_ThreadholdValues.cbegin(); item != m_ThreadholdValues.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256, "SELECT lowerLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetLowerValue,wxT("lowerLimit"),this);
	
	
				sprintf_s(sqlQuery,256, "SELECT upperLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetUpperValue,wxT("upperLimit"),this);
			}
			// if(resultNum != 0)
			// {
			// 	for(int index = 0; index < resultNum; index++)
			// 	{
			// 		int length = 0;
			// 		char* sqlTmpStr = pDataBase->GetStringValue(index,"eName",length);
			// 		{
			// 			if(sqlTmpStr)
			// 			{
			// 				std::string eName(sqlTmpStr,strlen(sqlTmpStr));
			// 				std::pair<FLOAT,FLOAT> pair;
			// 				pair.first = pDataBase->GetFloatValue(index,"lowerLimit");
			// 				pair.second = pDataBase->GetFloatValue(index,"upperLimit");
			// 				m_ThreadholdValues[eName] = pair;
			// 			}
			// 		}
			// 	}
			// }
			// 	pDataBase->FreeResult(sqlError);

		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
}

void CSCAlarm::SCCheck()
{

	for(auto item = m_SCValues.cbegin();item != m_SCValues.cend(); item++)
	{
		auto name = item->first;
		if(m_ThreadholdValues.find(name) != m_ThreadholdValues.cend())
		{
			FLOAT value = 0;
			FLOAT coefficient = 0;
			if((name == "volt510") || (name == "RectVolt"))
			{
				coefficient = 10.0;
			}
			else if (name == "AzCmdRmp" || name == "AzCurRmp" || name == "ElCmdRmp" || name == "ElCurRmp")
			{
				coefficient = 1.0;
			}
			else
			{
				coefficient = 100.0;
			}

			value = m_SCValues[name] / coefficient;
			auto lowerLimit = m_ThreadholdValues[name].first;
			auto upperLimit = m_ThreadholdValues[name].second;


			if((std::fabs(lowerLimit) < ZEROBIAS) && (std::fabs(upperLimit) < ZEROBIAS))
			{
				continue;
			}

			if(m_SCExpFlag[name])
			{
				if((value <= upperLimit) || (value >= lowerLimit))
				{
					m_SCExpFlag[name] = false;
					m_SCEndureTime[name].second = ::wxDateTime::GetTimeNow();
					writeToDataBasewithEndTime(name);
				}

			}
			else
			{
				if(value > upperLimit)
				{
					m_SCExpFlag[name] = true;
					m_SCEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SCConcreteContent[name] = m_SCContent[name];
					m_SCConcreteContent[name]  += wxString::Format(wxT("ֵΪ%.2f,������ֵ��Χ[%.2f,%.2f]"),m_SCValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);

				}
				else if(value < lowerLimit)
				{
					m_SCExpFlag[name] = true;
					m_SCEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SCConcreteContent[name] = m_SCContent[name];
					m_SCConcreteContent[name] += wxString::Format(wxT("ֵΪ%.2f,������ֵ��Χ[%.2f,%.2f]"),m_SCValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);
				}
			}
		}

	}
	
}

void CSCAlarm::getSCValue(const RadarStructure::SCStructure& value)
{
	/*m_SCValues.clear();
	m_SCValues["rf_power_smoothing"] = value.RF_POWER_SMOOTHING * 1.0;
	m_SCValues["hxmtlosssingle"] = value.HXMTLOSSSINGLE * 1.0;
	m_SCValues["hxmtlosshv"] = value.HXMTLOSSHV * 1.0;
	m_SCValues["hrcvloss"] = value.HRCVLOSS * 1.0;
	m_SCValues["htestloss"] = value.HTESTLOSS * 1.0;
	m_SCValues["vxmtlosshv"] = value.VXMTLOSSHV * 1.0;
	m_SCValues["vrcvloss"] = value.VRCVLOSS * 1.0;
	m_SCValues["vtestloss"] = value.VTESTLOSS * 1.0;
	m_SCValues["pathloss_noise_h"] = value.PATHLOSS_NOISE_H * 1.0;
	m_SCValues["pathloss_noise_v"] = value.PATHLOSS_NOISE_V * 1.0;
	m_SCValues["ts_cw"] = value.TS_CW * 1.0;
	m_SCValues["ts_noise"] = value.TS_NOISE * 1.0;
	m_SCValues["pathloss_rf"] = value.PATHLOSS_RF * 1.0;
	m_SCValues["ts_rf"] = value.TS_RF * 1.0;
	m_SCValues["pathloss_kly"] = value.PATHLOSS_KLY * 1.0;
	m_SCValues["gas_loss"] = value.GAS_LOSS * 1.0;
	m_SCValues["radome_loss"] = value.RADOME_LOSS * 1.0;
	m_SCValues["temp"] = value.Temp * 1.0;
	m_SCValues["humi"] = value.Humi * 1.0;
	m_SCValues["magn1Amp"] = value.Magn1Amp * 1.0;
	m_SCValues["magn2Amp"] = value.Magn2Amp * 1.0;
	m_SCValues["filaAmp"] = value.FilaAmp * 1.0;
	m_SCValues["biasAmp"] = value.BiasAmp * 1.0;
	m_SCValues["collAmp"] = value.CollAmp * 1.0;
	m_SCValues["vacionAmp"] = value.VacionAmp * 1.0;
	m_SCValues["vacionVolt"] = value.VacionVolt * 1.0;
	m_SCValues["front12V"] = value.Front12V * 1.0;
	m_SCValues["front24V"] = value.Front24V * 1.0;
	m_SCValues["front36V"] = value.Front36V * 1.0;
	m_SCValues["end12V"] = value.End12V * 1.0;
	m_SCValues["end24V"] = value.End24V * 1.0;
	m_SCValues["end36V"] = value.End36V * 1.0;
	m_SCValues["highPwrAmp"] = value.HighPwrAmp * 1.0;
	m_SCValues["volt510"] = value.Volt510 * 1.0;
	m_SCValues["nPeakAmp"] = value.nPeakAmp * 1.0;
	m_SCValues["volt24V"] = value.Volt24V * 1.0;
	m_SCValues["volt15V"] = value.Volt15V * 1.0;
	m_SCValues["volt_15V"] = value.Volt_15V * 1.0;
	m_SCValues["trig12V"] = value.Trig12V * 1.0;
	m_SCValues["trig200V"] = value.Trig200V * 1.0;
	m_SCValues["rcv5V"] = value.Rcv5V * 1.0;
	m_SCValues["rcv_5V"] = value.Rcv_5V * 1.0;
	m_SCValues["rcv12V"] = value.Rcv12V * 1.0;
	m_SCValues["rcv15V"] = value.Rcv15V * 1.0;
	m_SCValues["rcv24V"] = value.Rcv24V * 1.0;
	m_SCValues["hFiledAmp"] = value.HFiledAmp * 1.0;
	m_SCValues["vFiledAmp"] = value.VFiledAmp * 1.0;
	m_SCValues["hPresel"] = value.Hpresel * 1.0;
	m_SCValues["vPresel"] = value.Vpresel * 1.0;
	m_SCValues["hMixer"] = value.Hmixer * 1.0;
	m_SCValues["vMixer"] = value.Vmixer * 1.0;
	m_SCValues["hFront"] = value.Hfront * 1.0;
	m_SCValues["vFront"] = value.Vfront * 1.0;
	m_SCValues["hDigit"] = value.Hdigit * 1.0;
	m_SCValues["vDigit"] = value.Vdigit * 1.0;
	m_SCValues["testSignalF"] = value.TestSignalF * 1.0;
	m_SCValues["testSignalB"] = value.TestSignalB * 1.0;
	m_SCValues["hFiledFront"] = value.HFiledFront * 1.0;
	m_SCValues["vFiledFront"] = value.VFiledFront * 1.0;
	m_SCValues["excitOut"] = value.ExcitOut * 1.0;
	m_SCValues["testOut"] = value.TestOut * 1.0;
	m_SCValues["intrinOut"] = value.IntrinOut * 1.0;
	m_SCValues["COHO"] = value.COHO * 1.0;
	m_SCValues["RFD"] = value.RFD * 1.0;
	m_SCValues["hKD"] = value.HKD * 1.0;
	m_SCValues["vKD"] = value.VKD * 1.0;
	m_SCValues["pAUp"] = value.PAUp * 1.0;
	m_SCValues["pADown"] = value.PADown * 1.0;
	m_SCValues["hDischg"] = value.Hdischg * 1.0;
	m_SCValues["vDischg"] = value.Vdischg * 1.0;
	m_SCValues["upLIC"] = value.UpLIC * 1.0;
	m_SCValues["upHLIC"] = value.UpHLIC * 1.0;
	m_SCValues["upVLIC"] = value.UpVLIC * 1.0;
	m_SCValues["downLIC"] = value.DownLIC * 1.0;
	m_SCValues["refLIC"] = value.RefLIC * 1.0;
	m_SCValues["caseTemp"] = value.CaseTemp * 1.0;
	m_SCValues["sv5V"] = value.Sv5V * 1.0;
	m_SCValues["sv24V"] = value.Sv24V * 1.0;
	m_SCValues["azAmp"] = value.AzAmp * 1.0;
	m_SCValues["azCmdRmp"] = value.AzCmdRmp * 1.0;
	m_SCValues["azCurRmp"] = value.AzCurRmp * 1.0;
	m_SCValues["elAmp"] = value.ElAmp * 1.0;
	m_SCValues["elCmdRmp"] = value.ElCmdRmp * 1.0;
	m_SCValues["elCurRmp"] = value.ElCurRmp * 1.0;
	m_SCValues["aVolt"] = value.Avolt * 1.0;
	m_SCValues["aAmp"] = value.Aamp * 1.0;
	m_SCValues["bVolt"] = value.Bvolt * 1.0;
	m_SCValues["bAmp"] = value.Bamp * 1.0;
	m_SCValues["cVolt"] = value.Cvolt * 1.0;
	m_SCValues["cAmp"] = value.Camp * 1.0;
	m_SCValues["feederPressure"] = value.FeederPressure * 1.0;
	m_SCValues["feederHumidity"] = value.FeederHumidity * 1.0;
	m_SCValues["ant_temp"] = value.AntTemp * 1.0;
	m_SCValues["ant_humidity"] = value.AntHumidity * 1.0;
	m_SCValues["roomTemp"] = value.RoomTemp * 1.0;
	m_SCValues["roomHumidity"] = value.RoomHumidity * 1.0;*/

}

bool CSCAlarm::writeToDataBase(const std::string name)
{
	wxString sqlSentence = wxT("INSERT INTO alarminfo (time, \
														endTime, \
														type, \
														alarmName, \
														level, \
														mode, \
														content, \
														origin) VALUES");
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	// try
	// {
		if(pDataBase)
		{
			sqlSentence.Append(wxString::Format(wxT("(%d, "), m_SCEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "),  wxString(m_alarmType[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 1));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_SCConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));
		}
		return 	pDataBase->RunSQLNoQuery(sqlSentence);
	// }
	// catch (wxString& error)
	// {
	// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
	// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
	// }

	return false;
}



bool CSCAlarm::writeToDataBasewithEndTime(const std::string name)
{
	// wxString sqlError;
	auto pDataBase = GetPlug()->getDatabase();
	// wxMutex* sqlLocker = GetPlug()->getSqlQueryMutexLocker();
	// wxMutexLocker locker(*sqlLocker);	
	if (pDataBase)
	{
		// try
		// {
			wxString sqlSentence; 
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%d WHERE endTime IS NULL AND time=%d  AND content='%s'"), m_SCEndureTime[name].second,
																										m_SCEndureTime[name].first,
																										wxString(m_SCConcreteContent[name].c_str(),wxConvLocal));


			return 	pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

int CSCAlarm::cbSQLGetAlarmType(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CSCAlarm *pT = static_cast<CSCAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmType(std::string(argv[0]));
	}

	return 0;
}

int CSCAlarm::cbSQLGetLowerValue(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CSCAlarm *pT = static_cast<CSCAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetLowerValue(std::string(argv[0]));
	}

	return 0;
}

int CSCAlarm::cbSQLGetUpperValue(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CSCAlarm *pT = static_cast<CSCAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetUpperValue(std::string(argv[0]));
	}

	return 0;
}
