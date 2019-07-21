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


	m_SCContent["rf_power_smoothing"] = "射频功率平滑系数";
	m_SCContent["hxmtlosssingle"] = "水平发射水平通道发射支路损耗";
	m_SCContent["hxmtlosshv"] = "双通道发射水平发射支路损耗";
	m_SCContent["hrcvloss"] = "水平通道接收支路损耗";
	m_SCContent["htestloss"] = "水平通道测试信号损耗";
	m_SCContent["vxmtlosshv"] = "双通道发射垂直发射支路损耗";
	m_SCContent["vrcvloss"] = "垂直通道接收支路损耗";
	m_SCContent["vtestloss"] = "垂直通道测试信号损耗";
	m_SCContent["pathloss_noise_h"] = "水平通道射频噪声测试信号损耗";
	m_SCContent["pathloss_noise_v"] = "垂直通道射频噪声测试信号损耗";
	m_SCContent["ts_cw"] = "连续波测试信号功率";
	m_SCContent["ts_noise"] = "噪声源测试信号超噪比";
	m_SCContent["pathloss_rf"] = "射频激励信号路径损耗";
	m_SCContent["ts_rf"] = "射频激励信号功率";
	m_SCContent["pathloss_kly"] = "速调管输出信号路径损耗";
	m_SCContent["gas_loss"] = "大气衰减值";
	m_SCContent["radome_loss"] = "天线罩双程损耗";
	m_SCContent["temp"] = "机柜风温度";
	m_SCContent["humi"] = "机柜风湿度";
	m_SCContent["magn1Amp"] = "磁场Ⅰ电流";
	m_SCContent["magn2Amp"] = "磁场Ⅱ电流";
	m_SCContent["filaAmp"] = "灯丝电流";
	m_SCContent["biasAmp"] = "偏磁电流";
	m_SCContent["collAmp"] = "注电流";
	m_SCContent["vacionAmp"] = "钛泵电流";
	m_SCContent["vacionVolt"] = "钛泵电压";
	m_SCContent["front12V"] = "前级功放+12V";
	m_SCContent["front24V"] = "前级功放+24V";
	m_SCContent["front36V"] = "前级功放+36V";
	m_SCContent["end12V"] = "末级功放+12V";
	m_SCContent["end24V"] = "末级功放+24V";
	m_SCContent["end36V"] = "末级功放+36V";
	m_SCContent["highPwrAmp"] = "高功率电流";
	m_SCContent["volt510"] = "+510V";
	m_SCContent["nPeakAmp"] = "反峰电流";
	m_SCContent["volt24V"] = "可控硅风机+24V";
	m_SCContent["volt15V"] = "充电+15V";
	m_SCContent["volt_15V"] = "充电-15V";
	m_SCContent["trig12V"] = "触发+12V";
	m_SCContent["trig200V"] = "触发+200V";
	m_SCContent["rcv5V"] = "+5V直流稳压电源";
	m_SCContent["rcv_5V"] = "-5V直流稳压电源";
	m_SCContent["rcv12V"] = "+12V直流稳压电源";
	m_SCContent["rcv15V"] = "+15V直流稳压电源";
	m_SCContent["rcv24V"] = "+24V直流稳压电源";
	m_SCContent["hFiledAmp"] = "H路场放电流";
	m_SCContent["vFiledAmp"] = "V路场放电流";
	m_SCContent["hPresel"] = "H路场放预选器";
	m_SCContent["vPresel"] = "V路场放预选器";
	m_SCContent["hMixer"] = "H路混频器";
	m_SCContent["vMixer"] = "V路混频器";
	m_SCContent["hFront"] = "H路前置中频放大器";
	m_SCContent["vFront"] = "V路前置中频放大器";
	m_SCContent["hDigit"] = "数字中频处理器H路";
	m_SCContent["vDigit"] = "数字中频处理器V路";
	m_SCContent["testSignalF"] ="定标衰减器前输入点测试信号";
	m_SCContent["testSignalB"] ="定标衰减器后输入点测试信号";
	m_SCContent["hFiledFront"] ="H路场放前输入点测试信号";
	m_SCContent["vFiledFront"] ="V路场放前输入点测试信号";
	m_SCContent["excitOut"] = "激励源输出信号";
	m_SCContent["testOut"] = "测试源输出信号";
	m_SCContent["intrinOut"] = "本振源输出信号";
	m_SCContent["COHO"] = "30MHz相干基准信号";
	m_SCContent["RFD"] = "RFD信号";
	m_SCContent["hKD"] = "H路KD信号";
	m_SCContent["vKD"] = "V路KD信号";
	m_SCContent["pAUp"] = "天线上功放信号";
	m_SCContent["pADown"] = "天线下功放信号";
	m_SCContent["hDischg"] = "H放电管信号";
	m_SCContent["vDischg"] = "V放电管信号";
	m_SCContent["upLIC"] = "天线上发射耦合信号";
	m_SCContent["upHLIC"] = "天线上H发射耦合信号";
	m_SCContent["upVLIC"] = "天线上V发射耦合信号";
	m_SCContent["downLIC"] = "天线下发射耦合信号";
	m_SCContent["refLIC"] = "天线下反射耦合信号";
	m_SCContent["caseTemp"] = "恒温箱温度";
	m_SCContent["sv5V"] = "+5V电源";
	m_SCContent["sv24V"] = "+24V电源";
	m_SCContent["azAmp"] = "方位电流";
	m_SCContent["azCmdRmp"] = "方位命令转速";
	m_SCContent["azCurRmp"] = "方位当前转速";
	m_SCContent["elAmp"] = "俯仰电流";
	m_SCContent["elCmdRmp"] = "俯仰命令转速";
	m_SCContent["elCurRmp"] ="俯仰当前转速";
	m_SCContent["aVolt"] = "A相电压";
	m_SCContent["aAmp"] = "A相电流";
	m_SCContent["bVolt"] = "B相电压";
	m_SCContent["bAmp"] = "B相电流";
	m_SCContent["cVolt"] = "C相电压";
	m_SCContent["cAmp"] = "C相电流";
	m_SCContent["feederPressure"] = "馈线腔内气压";
	m_SCContent["feederHumidity"] = "馈线腔内空气干燥度";
	m_SCContent["ant_temp"] = "天线罩温度";
	m_SCContent["ant_humidity"] = "天线罩湿度";
	m_SCContent["roomTemp"] = "机房温度";
	m_SCContent["roomHumidity"] = "机房湿度";

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
					m_SCConcreteContent[name]  += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),m_SCValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
					writeToDataBase(name);

				}
				else if(value < lowerLimit)
				{
					m_SCExpFlag[name] = true;
					m_SCEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_SCConcreteContent[name] = m_SCContent[name];
					m_SCConcreteContent[name] += wxString::Format(wxT("值为%.2f,超出阈值范围[%.2f,%.2f]"),m_SCValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
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

int CSCAlarm::cbSQLGetAlarmType(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSCAlarm *pT = static_cast<CSCAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmType(std::string(argv[0]));
	}

	return 0;
}

int CSCAlarm::cbSQLGetLowerValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSCAlarm *pT = static_cast<CSCAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetLowerValue(std::string(argv[0]));
	}

	return 0;
}

int CSCAlarm::cbSQLGetUpperValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CSCAlarm *pT = static_cast<CSCAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetUpperValue(std::string(argv[0]));
	}

	return 0;
}
