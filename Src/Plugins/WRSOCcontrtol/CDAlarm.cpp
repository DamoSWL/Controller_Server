#include "CDAlarm.h"
#include "mysqldatabase.h"


CCDAlarm::CCDAlarm(void)
{
    
    m_CDExpFlag["rf_power_smoothing"] = false;
    m_CDExpFlag["hxmtlosssingle"] = false;
    m_CDExpFlag["hxmtlosshv"] = false;
    m_CDExpFlag["hrcvloss"] = false;
    m_CDExpFlag["htestloss"] = false;
    m_CDExpFlag["vxmtlosshv"] = false;
    m_CDExpFlag["vrcvloss"] = false;
    m_CDExpFlag["vtestloss"] = false;
    m_CDExpFlag["pathloss_noise_h"] = false;
    m_CDExpFlag["pathloss_noise_v"] = false;
    m_CDExpFlag["ts_cw"] = false;
    m_CDExpFlag["ts_noise"] = false;
    m_CDExpFlag["pathloss_rf"] = false;
    m_CDExpFlag["ts_rf"] = false;
    m_CDExpFlag["pathloss_kly"] = false;
    m_CDExpFlag["gas_loss"] = false;
    m_CDExpFlag["radome_loss"] = false;
    m_CDExpFlag["feederPressure"] = false;
    m_CDExpFlag["feederHumidity"] = false;
    m_CDExpFlag["pwr12V"] = false;
    m_CDExpFlag["chg15V"] = false;
    m_CDExpFlag["chg_15V"] = false;
    m_CDExpFlag["trig12V"] = false;
    m_CDExpFlag["trig24V"] = false;
    m_CDExpFlag["monit12V"] = false;
    m_CDExpFlag["filaVolt"] = false;
    m_CDExpFlag["filaAmp"] = false;
    m_CDExpFlag["biasAmp"] = false;
    m_CDExpFlag["vacionVolt"] = false;
    m_CDExpFlag["vacionAmp"] = false;
    m_CDExpFlag["klyTemp"] = false;
    m_CDExpFlag["rectVolt"] = false;
    m_CDExpFlag["highPwrAmp"] = false;
    m_CDExpFlag["collAmp"] = false;
    m_CDExpFlag["nPeakAmp"] = false;
    m_CDExpFlag["rcv5V"] = false;
    m_CDExpFlag["rcv_5V"] = false;
    m_CDExpFlag["rcv12V"] = false;
    m_CDExpFlag["rcv15V"] = false;
    m_CDExpFlag["rcv24V"] = false;
    m_CDExpFlag["hFiledAmp"] = false;
    m_CDExpFlag["vFiledAmp"] = false;
    m_CDExpFlag["hPresel"] = false;
    m_CDExpFlag["vPresel"] = false;
    m_CDExpFlag["hMixer"] = false;
    m_CDExpFlag["vMixer"] = false;
    m_CDExpFlag["hFront"] = false;
    m_CDExpFlag["vFront"] = false;
    m_CDExpFlag["hDigit"] = false;
    m_CDExpFlag["vDigit"] = false;
    m_CDExpFlag["testSignalF"] = false;
    m_CDExpFlag["testSignalB"] = false;
    m_CDExpFlag["hFiledFront"] = false;
    m_CDExpFlag["vFiledFront"] = false;
    m_CDExpFlag["excitOut"] = false;
    m_CDExpFlag["testOut"] = false;
    m_CDExpFlag["intrinOut"] = false;
    m_CDExpFlag["COHO"] = false;
    m_CDExpFlag["RFD"] = false;
    m_CDExpFlag["hKD"] = false;
    m_CDExpFlag["vKD"] = false;
    m_CDExpFlag["pAUp"] = false;
    m_CDExpFlag["pADown"] = false;
    m_CDExpFlag["hDischg"] = false;
    m_CDExpFlag["vDischg"] = false;
    m_CDExpFlag["upLIC"] = false;
    m_CDExpFlag["upHLIC"] = false;
    m_CDExpFlag["upVLIC"] = false;
    m_CDExpFlag["downLIC"] = false;
    m_CDExpFlag["refLIC"] = false;
    m_CDExpFlag["caseTemp"] = false;
    m_CDExpFlag["sv5V"] = false;
    m_CDExpFlag["sv24V"] = false;
    m_CDExpFlag["azAmp"] = false;
    m_CDExpFlag["azCmdRmp"] = false;
    m_CDExpFlag["azCurRmp"] = false;
    m_CDExpFlag["elAmp"] = false;
    m_CDExpFlag["elCmdRmp"] = false;
    m_CDExpFlag["elCurRmp"] = false;
    m_CDExpFlag["aVolt"] = false;
    m_CDExpFlag["aAmp"] = false;
    m_CDExpFlag["bVolt"] = false;
    m_CDExpFlag["bAmp"] = false;
    m_CDExpFlag["cVolt"] = false;
    m_CDExpFlag["cAmp"] = false;
    m_CDExpFlag["ant_temp"] = false;
    m_CDExpFlag["ant_humidity"] = false;
    m_CDExpFlag["roomTemp"] = false;
    m_CDExpFlag["roomHumidity"] = false;

    m_CDContent["rf_power_smoothing"] = "射频功率平滑系数";
    m_CDContent["hxmtlosssingle"] = "水平发射水平通道发射支路损耗";
    m_CDContent["hxmtlosshv"] = "双通道发射水平发射支路损耗";
    m_CDContent["hrcvloss"] = "水平通道接收支路损耗";
    m_CDContent["htestloss"] = "水平通道测试信号损耗";
    m_CDContent["vxmtlosshv"] = "双通道发射垂直发射支路损耗";
    m_CDContent["vrcvloss"] = "垂直通道接收支路损耗";
    m_CDContent["vtestloss"] = "垂直通道测试信号损耗";
    m_CDContent["pathloss_noise_h"] = "水平通道射频噪声测试信号损耗";
    m_CDContent["pathloss_noise_v"] = "垂直通道射频噪声测试信号损耗";
    m_CDContent["ts_cw"] = "连续波测试信号功率";
    m_CDContent["ts_noise"] = "噪声源测试信号超噪比";
    m_CDContent["pathloss_rf"] = "射频激励信号路径损耗";
    m_CDContent["ts_rf"] = "射频激励信号功率";
    m_CDContent["pathloss_kly"] = "速调管输出信号路径损耗";
    m_CDContent["gas_loss"] = "大气衰减值";
    m_CDContent["radome_loss"] = "天线罩双程损耗";
    m_CDContent["feederPressure"] = "馈线气压";
    m_CDContent["feederHumidity"] = "馈线干燥度";
    m_CDContent["pwr12V"] = "功放电源+12V";
    m_CDContent["chg15V"] = "充电控制+15V故障";
    m_CDContent["chg_15V"] = "充电控制-15V故障";
    m_CDContent["trig12V"] = "触发产生+12V故障";
    m_CDContent["trig24V"] = "触发产生+24V故障";
    m_CDContent["monit12V"] = "监控+12V故障";
    m_CDContent["filaVolt"] = "灯丝电压";
    m_CDContent["filaAmp"] = "灯丝电流";
    m_CDContent["biasAmp"] = "偏磁电流";
    m_CDContent["vacionVolt"] = "钛泵电压";
    m_CDContent["vacionAmp"] = "钛泵电流";
    m_CDContent["klyTemp"] = "KLY管温";
    m_CDContent["rectVolt"] = "整流电压";
    m_CDContent["highPwrAmp"] = "高功率电流";
    m_CDContent["collAmp"] = "注电流";
    m_CDContent["nPeakAmp"] = "反峰电流";
    m_CDContent["rcv5V"] = "+5V直流稳压电源";
    m_CDContent["rcv_5V"] = "-5V直流稳压电源";
    m_CDContent["rcv12V"] = "+12V直流稳压电源";
    m_CDContent["rcv15V"] = "+15V直流稳压电源";
    m_CDContent["rcv24V"] = "+24V直流稳压电源";
    m_CDContent["hFiledAmp"] = "H路场放电流";
    m_CDContent["vFiledAmp"] = "V路场放电流";
    m_CDContent["hPresel"] = "H路场放预选器";
    m_CDContent["vPresel"] = "V路场放预选器";
    m_CDContent["hMixer"] = "H路混频器";
    m_CDContent["vMixer"] = "V路混频器";
    m_CDContent["hFront"] = "H路前置中频放大器";
    m_CDContent["vFront"] = "V路前置中频放大器";
    m_CDContent["hDigit"] = "H路数字中频处理器";
    m_CDContent["vDigit"] = "V路数字中频处理器";
    m_CDContent["testSignalF"] = "定标衰减器前测试信号";
    m_CDContent["testSignalB"] = "定标衰减器后测试信号";
    m_CDContent["hFiledFront"] = "H路场放前测试信号";
    m_CDContent["vFiledFront"] = "V路场放前测试信号";
    m_CDContent["excitOut"] = "激励源输出信号";
    m_CDContent["testOut"] = "测试源输出信号";
    m_CDContent["intrinOut"] = "本振源输出信号";
    m_CDContent["COHO"] = "30MHz相干基准信号";
    m_CDContent["RFD"] = "RFD信号";
    m_CDContent["hKD"] = "H路KD信号";
    m_CDContent["vKD"] = "V路KD信号";
    m_CDContent["pAUp"] = "天线上功放信号";
    m_CDContent["pADown"] = "天线下功放信号";
    m_CDContent["hDischg"] = "H放电管信号";
    m_CDContent["vDischg"] = "V放电管信号";
    m_CDContent["upLIC"] = "天线上发射耦合信号";
    m_CDContent["upHLIC"] = "天线上H发射耦合信号";
    m_CDContent["upVLIC"] = "天线上V发射耦合信号";
    m_CDContent["downLIC"] = "天线下发射耦合信号";
    m_CDContent["refLIC"] = "天线下反射耦合信号";
    m_CDContent["caseTemp"] = "恒温箱温度";
    m_CDContent["sv5V"] = "+5V电源";
    m_CDContent["sv24V"] = "+24V电源";
    m_CDContent["azAmp"] = "方位电流";
    m_CDContent["azCmdRmp"] = "方位命令转速";
    m_CDContent["azCurRmp"] = "方位当前转速";
    m_CDContent["elAmp"] = "俯仰电流";
    m_CDContent["elCmdRmp"] = "俯仰命令转速";
    m_CDContent["elCurRmp"] = "俯仰当前转速";
    m_CDContent["aVolt"] = "A相电压";
    m_CDContent["aAmp"] = "A相电流";
    m_CDContent["bVolt"] = "B相电压";
    m_CDContent["bAmp"] = "B相电流";
    m_CDContent["cVolt"] = "C相电压";
    m_CDContent["cAmp"] = "C相电流";
    m_CDContent["ant_temp"] = "天线罩温度";
    m_CDContent["ant_humidity"] = "天线罩湿度";
    m_CDContent["roomTemp"] = "机房温度";
	m_CDContent["roomHumidity"] = "机房湿度";
	
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
    m_alarmType["feederPressure"] = std::string("");
    m_alarmType["feederHumidity"] = std::string("");
    m_alarmType["pwr12V"] = std::string("");
    m_alarmType["chg15V"] = std::string("");
    m_alarmType["chg_15V"] = std::string("");
    m_alarmType["trig12V"] = std::string("");
    m_alarmType["trig24V"] = std::string("");
    m_alarmType["monit12V"] = std::string("");
    m_alarmType["filaVolt"] = std::string("");
    m_alarmType["filaAmp"] = std::string("");
    m_alarmType["biasAmp"] = std::string("");
    m_alarmType["vacionVolt"] = std::string("");
    m_alarmType["vacionAmp"] = std::string("");
    m_alarmType["klyTemp"] = std::string("");
    m_alarmType["rectVolt"] = std::string("");
    m_alarmType["highPwrAmp"] = std::string("");
    m_alarmType["collAmp"] = std::string("");
    m_alarmType["nPeakAmp"] = std::string("");
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
    m_ThreadholdValues["feederPressure"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["feederHumidity"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["pwr12V"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["chg15V"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["chg_15V"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["trig12V"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["trig24V"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["monit12V"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["filaVolt"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["filaAmp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["biasAmp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["vacionVolt"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["vacionAmp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["klyTemp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["rectVolt"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["highPwrAmp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["collAmp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["nPeakAmp"] = std::make_pair(0.0,0.0);
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
    m_ThreadholdValues["ant_temp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["ant_humidity"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["roomTemp"] = std::make_pair(0.0,0.0);
    m_ThreadholdValues["roomHumidity"] = std::make_pair(0.0,0.0);



}


CCDAlarm::~CCDAlarm(void)
{
}

void CCDAlarm::getAlarmType()
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

void CCDAlarm::getThresholdValue()
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

void CCDAlarm::getCDValue(const RadarStructure::CDStructure& value)
{
 /*   m_CDValues.clear();
    m_CDValues["rf_power_smoothing"] = value.RF_POWER_SMOOTHING * 1.0;
	m_CDValues["hxmtlosssingle"] = value.HXMTLOSSSINGLE * 1.0;
	m_CDValues["hxmtlosshv"] = value.HXMTLOSSHV * 1.0;
	m_CDValues["hrcvloss"] = value.HRCVLOSS * 1.0;
	m_CDValues["htestloss"] = value.HTESTLOSS * 1.0;
	m_CDValues["vxmtlosshv"] = value.VXMTLOSSHV * 1.0;
	m_CDValues["vrcvloss"] = value.VRCVLOSS * 1.0;
	m_CDValues["vtestloss"] = value.VTESTLOSS * 1.0;
	m_CDValues["pathloss_noise_h"] = value.PATHLOSS_NOISE_H * 1.0;
	m_CDValues["pathloss_noise_v"] = value.PATHLOSS_NOISE_V * 1.0;
	m_CDValues["ts_cw"] = value.TS_CW * 1.0;
	m_CDValues["ts_noise"] = value.TS_NOISE * 1.0;
	m_CDValues["pathloss_rf"] = value.PATHLOSS_RF * 1.0;
	m_CDValues["ts_rf"] = value.TS_RF * 1.0;
	m_CDValues["pathloss_kly"] = value.PATHLOSS_KLY * 1.0;
	m_CDValues["gas_loss"] = value.GAS_LOSS * 1.0;
	m_CDValues["radome_loss"] = value.RADOME_LOSS * 1.0;
	m_CDValues["pwr12V"] = value.Pwr12V * 1.0;
	m_CDValues["chg15V"] = value.Chg15V * 1.0;
	m_CDValues["chg_15V"] = value.Chg_15V * 1.0;
	m_CDValues["trig12V"] = value.Trig12V * 1.0;
	m_CDValues["trig24V"] = value.Trig24V * 1.0;
	m_CDValues["monit12V"] = value.Monit12V * 1.0;
	m_CDValues["filaVolt"] = value.FilaVolt * 1.0;
	m_CDValues["filaAmp"] = value.FilaAmp * 1.0;
	m_CDValues["biasAmp"] = value.BiasAmp * 1.0;
	m_CDValues["vacionVolt"] = value.VacionVolt * 1.0;
	m_CDValues["vacionAmp"] = value.VacionAmp * 1.0;
	m_CDValues["klyTemp"] = value.KlyTemp * 1.0;
	m_CDValues["rectVolt"] = value.RectVolt * 1.0;
	m_CDValues["highPwrAmp"] = value.HighPwrAmp * 1.0;
	m_CDValues["collAmp"] = value.CollAmp * 1.0;
	m_CDValues["nPeakAmp"] = value.nPeakAmp * 1.0;
	m_CDValues["rcv5V"] = value.Rcv5V * 1.0;
	m_CDValues["rcv_5V"] = value.Rcv_5V * 1.0;
	m_CDValues["rcv12V"] = value.Rcv12V * 1.0;
	m_CDValues["rcv15V"] = value.Rcv15V * 1.0;
	m_CDValues["rcv24V"] = value.Rcv24V * 1.0;
	m_CDValues["hFiledAmp"] = value.HFiledAmp * 1.0;
	m_CDValues["vFiledAmp"] = value.VFiledAmp * 1.0;
	m_CDValues["hPresel"] = value.Hpresel * 1.0;
	m_CDValues["vPresel"] = value.Vpresel * 1.0;
	m_CDValues["hMixer"] = value.Hmixer * 1.0;
	m_CDValues["vMixer"] = value.Vmixer * 1.0;
	m_CDValues["hFront"] = value.Hfront * 1.0;
	m_CDValues["vFront"] = value.Vfront * 1.0;
	m_CDValues["hDigit"] = value.Hdigit * 1.0;
	m_CDValues["vDigit"] = value.Vdigit * 1.0;
	m_CDValues["testSignalF"] = value.TestSignalF * 1.0;
	m_CDValues["testSignalB"] = value.TestSignalB * 1.0;
	m_CDValues["hFiledFront"] = value.HFiledFront * 1.0;
	m_CDValues["vFiledFront"] = value.VFiledFront * 1.0;
	m_CDValues["excitOut"] = value.ExcitOut * 1.0;
	m_CDValues["testOut"] = value.TestOut * 1.0;
	m_CDValues["intrinOut"] = value.IntrinOut * 1.0;
	m_CDValues["COHO"] = value.COHO * 1.0;
	m_CDValues["RFD"] = value.RFD * 1.0;
	m_CDValues["hKD"] = value.HKD * 1.0;
	m_CDValues["vKD"] = value.VKD * 1.0;
	m_CDValues["pAUp"] = value.PAUp * 1.0;
	m_CDValues["pADown"] = value.PADown * 1.0;
	m_CDValues["hDischg"] = value.Hdischg * 1.0;
	m_CDValues["vDischg"] = value.Vdischg * 1.0;
	m_CDValues["upLIC"] = value.UpLIC * 1.0;
	m_CDValues["upHLIC"] = value.UpHLIC * 1.0;
	m_CDValues["upVLIC"] = value.UpVLIC * 1.0;
	m_CDValues["downLIC"] = value.DownLIC * 1.0;
	m_CDValues["refLIC"] = value.RefLIC * 1.0;
	m_CDValues["caseTemp"] = value.CaseTemp * 1.0;
	m_CDValues["sv5V"] = value.Sv5V * 1.0;
	m_CDValues["sv24V"] = value.Sv24V * 1.0;
	m_CDValues["azAmp"] = value.AzAmp * 1.0;
	m_CDValues["azCmdRmp"] = value.AzCmdRmp * 1.0;
	m_CDValues["azCurRmp"] = value.AzCurRmp * 1.0;
	m_CDValues["elAmp"] = value.ElAmp * 1.0;
	m_CDValues["elCmdRmp"] = value.ElCmdRmp * 1.0;
	m_CDValues["elCurRmp"] = value.ElCurRmp * 1.0;
	m_CDValues["aVolt"] = value.Avolt * 1.0;
	m_CDValues["aAmp"] = value.Aamp * 1.0;
	m_CDValues["bVolt"] = value.Bvolt * 1.0;
	m_CDValues["bAmp"] = value.Bamp * 1.0;
	m_CDValues["cVolt"] = value.Cvolt * 1.0;
	m_CDValues["cAmp"] = value.Camp * 1.0;
	m_CDValues["feederPressure"] = value.FeederPressure * 1.0;
	m_CDValues["feederHumidity"] = value.FeederHumidity * 1.0;
	m_CDValues["ant_temp"] = value.AntTemp * 1.0;
	m_CDValues["ant_humidity"] = value.AntHumidity * 1.0;
	m_CDValues["roomTemp"] = value.RoomTemp * 1.0;
	m_CDValues["roomHumidity"] = value.RoomHumidity * 1.0;*/
}

void CCDAlarm::CDCheck()
{
    for(auto item = m_CDValues.cbegin();item != m_CDValues.cend(); item++)
	{
		auto name = item->first;
		if(m_ThreadholdValues.find(name) != m_ThreadholdValues.cend())
		{
			FLOAT value = 0;
			FLOAT coefficient = 0;
			if(name == "RectVolt")
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

			value = m_CDValues[name] / coefficient;
			auto lowerLimit = m_ThreadholdValues[name].first;
			auto upperLimit = m_ThreadholdValues[name].second;

			if((std::fabs(lowerLimit) < ZEROBIAS) && (std::fabs(upperLimit) < ZEROBIAS))
			{
				continue;
			}

			if(m_CDExpFlag[name])
			{
				if((value <= upperLimit) || (value >= lowerLimit))
				{
					m_CDExpFlag[name] = false;
					m_CDEndureTime[name].second = ::wxDateTime::GetTimeNow();
					writeToDataBasewithEndTime(name);
				}

			}
			else
			{
				if(value > upperLimit)
				{
					m_CDExpFlag[name] = true;
					m_CDEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_CDConcreteContent[name] = m_CDContent[name];
					m_CDConcreteContent[name].append("取样值过高");
					writeToDataBase(name);

				}
				else if(value < lowerLimit)
				{
					m_CDExpFlag[name] = true;
					m_CDEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_CDConcreteContent[name] = m_CDContent[name];
					m_CDConcreteContent[name].append("取样值过低");
					writeToDataBase(name);
				}
			}
		}

	}
}

bool CCDAlarm::writeToDataBase(const std::string name)
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
			sqlSentence.Append(wxString::Format(wxT("(%d, "), m_CDEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "),  wxString(m_alarmType[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), 1));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_CDConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 2));
		}



		return pDataBase->RunSQLNoQuery(sqlSentence);
	// }
	// catch (wxString& error)
	// {
	// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
	// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
	// }

	return false;
}

bool CCDAlarm::writeToDataBasewithEndTime(const std::string name)
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
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%d WHERE endTime IS NULL AND time=%d  AND content='%s'"), m_CDEndureTime[name].second,
																										m_CDEndureTime[name].first,
																										wxString(m_CDConcreteContent[name].c_str(),wxConvLocal));
	

			return pDataBase->RunSQLNoQuery(sqlSentence);
		// }
		// catch (wxString& error)
		// {
		// 	GetPlug()->AddLog(LOG_TYPE_MESSAGE,__FUNCTION__);
		// 	GetPlug()->AddLog(LOG_TYPE_ERROR,error.mb_str(wxConvLocal));
		// }
	}
	return false;
}

int CCDAlarm::cbSQLGetAlarmType(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CCDAlarm *pT = static_cast<CCDAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmType(std::string(argv[0]));
	}

	return 0;

}

int CCDAlarm::cbSQLGetLowerValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CCDAlarm *pT = static_cast<CCDAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetLowerValue(std::string(argv[0]));
	}

	return 0;
}

int CCDAlarm::cbSQLGetUpperValue(void *pIn/*传入的参数*/, int argc, char **argv, char **azColName)
{
	CCDAlarm *pT = static_cast<CCDAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetUpperValue(std::string(argv[0]));
	}

	return 0;
}
