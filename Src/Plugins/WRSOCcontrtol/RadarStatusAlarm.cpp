#include "RadarStatusAlarm.h"
#include "mysqldatabase.h"




							

CRadarStatusAlarm::CRadarStatusAlarm()
{
	m_RadarStatusExpFlag["sel_patt"] = false;   //�Լ����������г�ʼ��
	m_RadarStatusExpFlag["ant_peak_pwr"] = false;
	m_RadarStatusExpFlag["xmtr_peak_pwr"] = false;
	m_RadarStatusExpFlag["ant_avg_pwr"] = false;
	m_RadarStatusExpFlag["xmtr_avg_pwr"] = false;
	m_RadarStatusExpFlag["power_meter_ratio_db"] = false;
	m_RadarStatusExpFlag["ant_pwr_mtr_zero"] = false;
	m_RadarStatusExpFlag["xmtr_pwr_mtr_zero"] = false;
	m_RadarStatusExpFlag["ant_pwr_mtr_zero_v"] = false;
	m_RadarStatusExpFlag["rnspln"] =false;
	m_RadarStatusExpFlag["ant_peak_pwr_v"] = false;
	m_RadarStatusExpFlag["rnlpln"] =false;
	m_RadarStatusExpFlag["ant_avg_pwr_v"] = false;
	m_RadarStatusExpFlag["te"] = false;
	m_RadarStatusExpFlag["pulse_width"] = false;
	m_RadarStatusExpFlag["sysstatus"] =false;
	m_RadarStatusExpFlag["noise_hchan_1"] = false;
	m_RadarStatusExpFlag["noise_hchan_2"] = false;
	m_RadarStatusExpFlag["noise_hchan_3"] = false;
	m_RadarStatusExpFlag["noise_hchan_4"] = false;
	m_RadarStatusExpFlag["noise_vchan_1"] = false;
	m_RadarStatusExpFlag["noise_vchan_2"] = false;
	m_RadarStatusExpFlag["noise_vchan_3"] = false;
	m_RadarStatusExpFlag["noise_vchan_4"] = false;
	m_RadarStatusExpFlag["rnoisehchan"] = false;
	m_RadarStatusExpFlag["rnoisevchan"] = false;
	m_RadarStatusExpFlag["pol"] = false;
	m_RadarStatusExpFlag["pdp"] = false;
	m_RadarStatusExpFlag["te_v"] = false;
	m_RadarStatusExpFlag["zdr"] = false;
	m_RadarStatusExpFlag["syscal_1"] = false;
	m_RadarStatusExpFlag["syscal_2"] = false;
	m_RadarStatusExpFlag["syscal_3"] = false;
	m_RadarStatusExpFlag["syscal_4"] = false;
	m_RadarStatusExpFlag["expected_refl_amp_1"] = false;
	m_RadarStatusExpFlag["expected_refl_amp_2"] = false;
	m_RadarStatusExpFlag["expected_refl_amp_3"] = false;
	m_RadarStatusExpFlag["expected_refl_amp_4"] = false;
	m_RadarStatusExpFlag["measured_refl_amp_1"] = false;
	m_RadarStatusExpFlag["measured_refl_amp_2"] = false;
	m_RadarStatusExpFlag["measured_refl_amp_3"] = false;
	m_RadarStatusExpFlag["measured_refl_amp_4"] = false;
	m_RadarStatusExpFlag["sysspln"] = false;
	m_RadarStatusExpFlag["syslpln"] = false;
	m_RadarStatusExpFlag["expected_velocity_1"] = false;
	m_RadarStatusExpFlag["expected_velocity_2"] = false;
	m_RadarStatusExpFlag["expected_velocity_3"] = false;
	m_RadarStatusExpFlag["expected_velocity_4"] = false;
	m_RadarStatusExpFlag["measured_velocity_1"] = false;
	m_RadarStatusExpFlag["measured_velocity_2"] = false;
	m_RadarStatusExpFlag["measured_velocity_3"] = false;
	m_RadarStatusExpFlag["measured_velocity_4"] = false;
	m_RadarStatusExpFlag["expected_width_1"] = false;
	m_RadarStatusExpFlag["expected_width_2"] = false;
	m_RadarStatusExpFlag["expected_width_3"] = false;
	m_RadarStatusExpFlag["expected_width_4"] = false;
	m_RadarStatusExpFlag["measured_width_1"] = false;
	m_RadarStatusExpFlag["measured_width_2"] = false;
	m_RadarStatusExpFlag["measured_width_3"] = false;
	m_RadarStatusExpFlag["measured_width_4"] = false;
	m_RadarStatusExpFlag["shelter_temp"] = false;
	m_RadarStatusExpFlag["xmtr_air_temp"] = false;
	m_RadarStatusExpFlag["radome_temp"] = false;
	m_RadarStatusExpFlag["expected_rf8_amp_1"] = false;
	m_RadarStatusExpFlag["expected_rf8_amp_2"] = false;
	m_RadarStatusExpFlag["expected_rf8_amp_3"] = false;
	m_RadarStatusExpFlag["shelter_humidity"] = false;
	m_RadarStatusExpFlag["xmtr_air_humidity"] = false;
	m_RadarStatusExpFlag["radome_humidity"] = false;
	m_RadarStatusExpFlag["measured_rf8_amp_1"] = false;
	m_RadarStatusExpFlag["measured_rf8_amp_2"] = false;
	m_RadarStatusExpFlag["measured_rf8_amp_3"] = false;
	m_RadarStatusExpFlag["phase_noise"] = false;
	m_RadarStatusExpFlag["phase_noise_v"] = false;
	m_RadarStatusExpFlag["unfiltered_v_chan_pwr"] = false;
	m_RadarStatusExpFlag["filtered_v_chan_pwr"] = false;
	m_RadarStatusExpFlag["unfiltered_lin_chan_pwr"] = false;
	m_RadarStatusExpFlag["filtered_lin_chan_pwr"] = false;
	m_RadarStatusExpFlag["syscal_delta"] = false;
	m_RadarStatusExpFlag["noise_coefficient_h"] = false;
	m_RadarStatusExpFlag["noise_coefficient_v"] = false;

	m_alarmLevel["sel_patt"] = 0;
	m_alarmLevel["ant_peak_pwr"] = 0;
	m_alarmLevel["xmtr_peak_pwr"] = 0;
	m_alarmLevel["ant_avg_pwr"] = 0;
	m_alarmLevel["xmtr_avg_pwr"] = 0;
	m_alarmLevel["power_meter_ratio_db"] = 0;
	m_alarmLevel["ant_pwr_mtr_zero"] = 0;
	m_alarmLevel["xmtr_pwr_mtr_zero"] = 0;
	m_alarmLevel["ant_pwr_mtr_zero_v"] = 0;
	m_alarmLevel["rnspln"] =0;
	m_alarmLevel["ant_peak_pwr_v"] = 0;
	m_alarmLevel["rnlpln"] =0;
	m_alarmLevel["ant_avg_pwr_v"] = 0;
	m_alarmLevel["te"] = 0;
	m_alarmLevel["pulse_width"] = 0;
	m_alarmLevel["sysstatus"] =0;
	m_alarmLevel["noise_hchan_1"] = 0;
	m_alarmLevel["noise_hchan_2"] = 0;
	m_alarmLevel["noise_hchan_3"] = 0;
	m_alarmLevel["noise_hchan_4"] = 0;
	m_alarmLevel["noise_vchan_1"] = 0;
	m_alarmLevel["noise_vchan_2"] = 0;
	m_alarmLevel["noise_vchan_3"] = 0;
	m_alarmLevel["noise_vchan_4"] = 0;
	m_alarmLevel["rnoisehchan"] = 0;
	m_alarmLevel["rnoisevchan"] = 0;
	m_alarmLevel["pol"] = 0;
	m_alarmLevel["pdp"] = 0;
	m_alarmLevel["te_v"] = 0;
	m_alarmLevel["zdr"] = 0;
	m_alarmLevel["syscal_1"] = 0;
	m_alarmLevel["syscal_2"] = 0;
	m_alarmLevel["syscal_3"] = 0;
	m_alarmLevel["syscal_4"] = 0;
	m_alarmLevel["expected_refl_amp_1"] = 0;
	m_alarmLevel["expected_refl_amp_2"] = 0;
	m_alarmLevel["expected_refl_amp_3"] = 0;
	m_alarmLevel["expected_refl_amp_4"] = 0;
	m_alarmLevel["measured_refl_amp_1"] = 0;
	m_alarmLevel["measured_refl_amp_2"] = 0;
	m_alarmLevel["measured_refl_amp_3"] = 0;
	m_alarmLevel["measured_refl_amp_4"] = 0;
	m_alarmLevel["sysspln"] = 0;
	m_alarmLevel["syslpln"] = 0;
	m_alarmLevel["expected_velocity_1"] = 0;
	m_alarmLevel["expected_velocity_2"] = 0;
	m_alarmLevel["expected_velocity_3"] = 0;
	m_alarmLevel["expected_velocity_4"] = 0;
	m_alarmLevel["measured_velocity_1"] = 0;
	m_alarmLevel["measured_velocity_2"] = 0;
	m_alarmLevel["measured_velocity_3"] = 0;
	m_alarmLevel["measured_velocity_4"] = 0;
	m_alarmLevel["expected_width_1"] = 0;
	m_alarmLevel["expected_width_2"] = 0;
	m_alarmLevel["expected_width_3"] = 0;
	m_alarmLevel["expected_width_4"] = 0;
	m_alarmLevel["measured_width_1"] = 0;
	m_alarmLevel["measured_width_2"] = 0;
	m_alarmLevel["measured_width_3"] = 0;
	m_alarmLevel["measured_width_4"] = 0;
	m_alarmLevel["shelter_temp"] = 0;
	m_alarmLevel["xmtr_air_temp"] = 0;
	m_alarmLevel["radome_temp"] = 0;
	m_alarmLevel["expected_rf8_amp_1"] = 0;
	m_alarmLevel["expected_rf8_amp_2"] = 0;
	m_alarmLevel["expected_rf8_amp_3"] = 0;
	m_alarmLevel["shelter_humidity"] = 0;
	m_alarmLevel["xmtr_air_humidity"] = 0;
	m_alarmLevel["radome_humidity"] = 0;
	m_alarmLevel["measured_rf8_amp_1"] = 0;
	m_alarmLevel["measured_rf8_amp_2"] = 0;
	m_alarmLevel["measured_rf8_amp_3"] = 0;
	m_alarmLevel["phase_noise"] = 0;
	m_alarmLevel["phase_noise_v"] = 0;
	m_alarmLevel["unfiltered_v_chan_pwr"] = 0;
	m_alarmLevel["filtered_v_chan_pwr"] = 0;
	m_alarmLevel["unfiltered_lin_chan_pwr"] = 0;
	m_alarmLevel["filtered_lin_chan_pwr"] = 0;
	m_alarmLevel["syscal_delta"] = 0;
	m_alarmLevel["noise_coefficient_h"] = 0;
	m_alarmLevel["noise_coefficient_v"] = 0;

	m_RadarStatusContent["sel_patt"] = "��ɨ��ģʽ";
	m_RadarStatusContent["ant_peak_pwr"] = "ˮƽͨ�����߷�ֵ��";
	m_RadarStatusContent["xmtr_peak_pwr"] = "�������ֵ����";
	m_RadarStatusContent["ant_avg_pwr"] = "ˮƽͨ������ƽ������";
	m_RadarStatusContent["xmtr_avg_pwr"] = "�����ƽ������";
	m_RadarStatusContent["power_meter_ratio_db"] = "����������߹��ʱ�";
	m_RadarStatusContent["ant_pwr_mtr_zero"] = "ˮƽͨ�����߹��ʵ���";
	m_RadarStatusContent["xmtr_pwr_mtr_zero"] = "��������ʵ���";
	m_RadarStatusContent["ant_pwr_mtr_zero_v"] = "��ֱͨ�����߹��ʵ���";
	m_RadarStatusContent["rnspln"] = "������������ƽ";
	m_RadarStatusContent["ant_peak_pwr_v"] = "��ֱͨ�����߷�ֵ����";
	m_RadarStatusContent["rnlpln"] = "������������ƽ";
	m_RadarStatusContent["ant_avg_pwr_v"] = "��ֱͨ������ƽ������";
	m_RadarStatusContent["te"] = "ˮƽͨ�������¶�";
	m_RadarStatusContent["pulse_width"] = "��ǰ���������";
	m_RadarStatusContent["sysstatus"] = "ϵͳ״̬";
	m_RadarStatusContent["noise_hchan_1"] = "ˮƽͨ������������ƽ1";
	m_RadarStatusContent["noise_hchan_2"] = "ˮƽͨ������������ƽ2";
	m_RadarStatusContent["noise_hchan_3"] = "ˮƽͨ������������ƽ3";
	m_RadarStatusContent["noise_hchan_4"] = "ˮƽͨ������������ƽ4";
	m_RadarStatusContent["noise_vchan_1"] = "��ֱͨ������������ƽ1";
	m_RadarStatusContent["noise_vchan_2"] = "��ֱͨ������������ƽ2";
	m_RadarStatusContent["noise_vchan_3"] = "��ֱͨ������������ƽ3";
	m_RadarStatusContent["noise_vchan_3"] = "��ֱͨ������������ƽ4";
	m_RadarStatusContent["rnoisehchan"] = "��ǰˮƽͨ��������ƽ";
	m_RadarStatusContent["rnoisevchan"] = "��ǰ��ֱͨ��������ƽ";
	m_RadarStatusContent["pol"] = "˫�����״���";
	m_RadarStatusContent["pdp"] = "PDP�궨ֵ";
	m_RadarStatusContent["te_v"] = "��ֱͨ�������¶�";
	m_RadarStatusContent["zdr"] = "ZDR�궨ֵ";
	m_RadarStatusContent["syscal_1"] = "����ϵͳ�궨����1";
	m_RadarStatusContent["syscal_2"] = "����ϵͳ�궨����2";
	m_RadarStatusContent["syscal_3"] = "����ϵͳ�궨����3";
	m_RadarStatusContent["syscal_4"] = "����ϵͳ�궨����4";
	m_RadarStatusContent["expected_refl_amp_1"] = "����������ֵ";
	m_RadarStatusContent["expected_refl_amp_2"] = "RFD����ֵ1";
	m_RadarStatusContent["expected_refl_amp_3"] = "RFD����ֵ2";
	m_RadarStatusContent["expected_refl_amp_4"] = "RFD����ֵ3";
	m_RadarStatusContent["measured_refl_amp_1"] = "�����ʲ���ֵ";
	m_RadarStatusContent["measured_refl_amp_2"] = "RFD����ֵ1";
	m_RadarStatusContent["measured_refl_amp_3"] = "RFD����ֵ2";
	m_RadarStatusContent["measured_refl_amp_4"] = "RFD����ֵ3";
	m_RadarStatusContent["sysspln"] = "������ϵͳ�궨����";
	m_RadarStatusContent["syslpln"] = "������ϵͳ�궨����";
	m_RadarStatusContent["expected_velocity_1"] = "�ٶ�����ֵ1";
	m_RadarStatusContent["expected_velocity_2"] = "�ٶ�����ֵ2";
	m_RadarStatusContent["expected_velocity_3"] = "�ٶ�����ֵ3";
	m_RadarStatusContent["expected_velocity_4"] = "�ٶ�����ֵ4";
	m_RadarStatusContent["measured_velocity_1"] = "�ٶȲ���ֵ1";
	m_RadarStatusContent["measured_velocity_2"] = "�ٶȲ���ֵ2";
	m_RadarStatusContent["measured_velocity_3"] = "�ٶȲ���ֵ3";
	m_RadarStatusContent["measured_velocity_4"] = "�ٶȲ���ֵ4";
	m_RadarStatusContent["expected_width_1"] = "�׿�����ֵ1";
	m_RadarStatusContent["expected_width_2"] = "�׿�����ֵ2";
	m_RadarStatusContent["expected_width_3"] = "�׿�����ֵ3";
	m_RadarStatusContent["expected_width_4"] = "�׿�����ֵ4";
	m_RadarStatusContent["measured_width_1"] = "�׿����ֵ1";
	m_RadarStatusContent["measured_width_2"] = "�׿����ֵ2";
	m_RadarStatusContent["measured_width_3"] = "�׿����ֵ3";
	m_RadarStatusContent["measured_width_4"] = "�׿����ֵ4";
	m_RadarStatusContent["shelter_temp"] = "�����¶�";
	m_RadarStatusContent["xmtr_air_temp"] = "������¶�";
	m_RadarStatusContent["radome_temp"] = "�������¶�";
	m_RadarStatusContent["expected_rf8_amp_1"] = "KD�궨����ֵ1";
	m_RadarStatusContent["expected_rf8_amp_2"] = "KD�궨����ֵ2";
	m_RadarStatusContent["expected_rf8_amp_3"] = "KD�궨����ֵ3";
	m_RadarStatusContent["shelter_humidity"] = "����ʪ��";
	m_RadarStatusContent["xmtr_air_humidity"] = "�����ʪ��";
	m_RadarStatusContent["radome_humidity"] = "������ʪ��";
	m_RadarStatusContent["measured_rf8_amp_1"] = "KD�궨����ֵ1";
	m_RadarStatusContent["measured_rf8_amp_2"] = "KD�궨����ֵ2";
	m_RadarStatusContent["measured_rf8_amp_3"] = "KD�궨����ֵ3";
	m_RadarStatusContent["phase_noise"] = "ˮƽͨ����λ����";
	m_RadarStatusContent["phase_noise_v"] = "��ֱͨ����λ����";
	m_RadarStatusContent["unfiltered_v_chan_pwr"] = "��ֱͨ���˲�ǰ����";
	m_RadarStatusContent["filtered_v_chan_pwr"] = "��ֱͨ���˲�����";
	m_RadarStatusContent["unfiltered_lin_chan_pwr"] = "ˮƽͨ���˲�ǰ����";
	m_RadarStatusContent["filtered_lin_chan_pwr"] = "ˮƽͨ���˲�����";
	m_RadarStatusContent["syscal_delta"] = "ϵͳ�궨�����仯";
	m_RadarStatusContent["noise_coefficient_h"] = "ˮƽͨ������ϵ��";
	m_RadarStatusContent["noise_coefficient_v"] = "��ֱͨ������ϵ��";

	m_alarmType["ant_peak_pwr"] = std::string("");
	m_alarmType["xmtr_peak_pwr"] = std::string("");
	m_alarmType["ant_avg_pwr"] = std::string("");
	m_alarmType["xmtr_avg_pwr"] = std::string("");
	m_alarmType["power_meter_ratio_db"] = std::string("");
	m_alarmType["ant_pwr_mtr_zero"] = std::string("");
	m_alarmType["xmtr_pwr_mtr_zero"] = std::string("");
	m_alarmType["ant_pwr_mtr_zero_v"] = std::string("");
	m_alarmType["rnspln"] = std::string("");
	m_alarmType["ant_peak_pwr_v"] = std::string("");
	m_alarmType["rnlpln"] = std::string("");
	m_alarmType["ant_avg_pwr_v"] = std::string("");
	m_alarmType["te"] = std::string("");
	m_alarmType["pulse_width"] = std::string("");
	m_alarmType["noise_hchan_1"] = std::string("");
	m_alarmType["noise_hchan_2"] = std::string("");
	m_alarmType["noise_hchan_3"] = std::string("");
	m_alarmType["noise_hchan_4"] = std::string("");
	m_alarmType["noise_vchan_1"] = std::string("");
	m_alarmType["noise_vchan_2"] = std::string("");
	m_alarmType["noise_vchan_3"] = std::string("");
	m_alarmType["noise_vchan_3"] = std::string("");
	m_alarmType["rnoisehchan"] = std::string("");
	m_alarmType["rnoisevchan"] = std::string("");
	m_alarmType["pdp"] = std::string("");
	m_alarmType["te_v"] = std::string("");
	m_alarmType["zdr"] = std::string("");
	m_alarmType["syscal_1"] = std::string("");
	m_alarmType["syscal_2"] = std::string("");
	m_alarmType["syscal_3"] = std::string("");
	m_alarmType["syscal_4"] = std::string("");
	m_alarmType["expected_refl_amp_1"] = std::string("");
	m_alarmType["expected_refl_amp_2"] = std::string("");
	m_alarmType["expected_refl_amp_3"] = std::string("");
	m_alarmType["expected_refl_amp_4"] = std::string("");
	m_alarmType["measured_refl_amp_1"] = std::string("");
	m_alarmType["measured_refl_amp_2"] = std::string("");
	m_alarmType["measured_refl_amp_3"] = std::string("");
	m_alarmType["measured_refl_amp_4"] = std::string("");
	m_alarmType["sysspln"] = std::string("");
	m_alarmType["syslpln"] = std::string("");
	m_alarmType["expected_velocity_1"] = std::string("");
	m_alarmType["expected_velocity_2"] = std::string("");
	m_alarmType["expected_velocity_3"] = std::string("");
	m_alarmType["expected_velocity_4"] = std::string("");
	m_alarmType["measured_velocity_1"] = std::string("");
	m_alarmType["measured_velocity_2"] = std::string("");
	m_alarmType["measured_velocity_3"] = std::string("");
	m_alarmType["measured_velocity_4"] = std::string("");
	m_alarmType["expected_width_1"] = std::string("");
	m_alarmType["expected_width_2"] = std::string("");
	m_alarmType["expected_width_3"] = std::string("");
	m_alarmType["expected_width_4"] = std::string("");
	m_alarmType["measured_width_1"] = std::string("");
	m_alarmType["measured_width_2"] = std::string("");
	m_alarmType["measured_width_3"] = std::string("");
	m_alarmType["measured_width_4"] = std::string("");
	m_alarmType["shelter_temp"] = std::string("");
	m_alarmType["xmtr_air_temp"] = std::string("");
	m_alarmType["radome_temp"] = std::string("");
	m_alarmType["expected_rf8_amp_1"] = std::string("");
	m_alarmType["expected_rf8_amp_2"] = std::string("");
	m_alarmType["expected_rf8_amp_3"] = std::string("");
	m_alarmType["shelter_humidity"] = std::string("");
	m_alarmType["xmtr_air_humidity"] = std::string("");
	m_alarmType["radome_humidity"] = std::string("");
	m_alarmType["measured_rf8_amp_1"] = std::string("");
	m_alarmType["measured_rf8_amp_2"] = std::string("");
	m_alarmType["measured_rf8_amp_3"] = std::string("");
	m_alarmType["phase_noise"] = std::string("");
	m_alarmType["phase_noise_v"] = std::string("");
	m_alarmType["unfiltered_v_chan_pwr"] = std::string("");
	m_alarmType["filtered_v_chan_pwr"] = std::string("");
	m_alarmType["unfiltered_lin_chan_pwr"] = std::string("");
	m_alarmType["filtered_lin_chan_pwr"] = std::string("");
	m_alarmType["syscal_delta"] = std::string("");
	m_alarmType["noise_coefficient_h"] = std::string("");
	m_alarmType["noise_coefficient_v"] = std::string("");

	m_ThreadholdValues["ant_peak_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["xmtr_peak_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_avg_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["xmtr_avg_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["power_meter_ratio_db"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_pwr_mtr_zero"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["xmtr_pwr_mtr_zero"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_pwr_mtr_zero_v"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rnspln"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_peak_pwr_v"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rnlpln"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["ant_avg_pwr_v"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["te"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pulse_width"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_hchan_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_hchan_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_hchan_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_hchan_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_vchan_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_vchan_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_vchan_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_vchan_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rnoisehchan"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["rnoisevchan"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["pdp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["te_v"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["zdr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["syscal_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["syscal_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["syscal_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["syscal_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_refl_amp_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_refl_amp_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_refl_amp_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_refl_amp_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_refl_amp_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_refl_amp_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_refl_amp_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_refl_amp_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["sysspln"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["syslpln"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_velocity_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_velocity_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_velocity_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_velocity_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_velocity_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_velocity_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_velocity_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_velocity_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_width_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_width_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_width_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_width_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_width_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_width_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_width_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_width_4"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["shelter_temp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["xmtr_air_temp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["radome_temp"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_rf8_amp_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_rf8_amp_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["expected_rf8_amp_3"] = std::make_pair(0.0,0.);
	m_ThreadholdValues["shelter_humidity"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["xmtr_air_humidity"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["radome_humidity"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_rf8_amp_1"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_rf8_amp_2"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["measured_rf8_amp_3"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["phase_noise"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["phase_noise_v"] = std::make_pair(0.0,0.0) ;
	m_ThreadholdValues["unfiltered_v_chan_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["filtered_v_chan_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["unfiltered_lin_chan_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["filtered_lin_chan_pwr"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["syscal_delta"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_coefficient_h"] = std::make_pair(0.0,0.0);
	m_ThreadholdValues["noise_coefficient_v"] = std::make_pair(0.0,0.0);
	
}


CRadarStatusAlarm::~CRadarStatusAlarm()
{
}


void CRadarStatusAlarm::getAlarmType()
{

	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{

			for(auto item = m_alarmType.cbegin(); item != m_alarmType.cend(); item++)
			{
				m_currenteName = item->first;
				char sqlQuery[256] = {'\0'};
				sprintf_s(sqlQuery,256,"SELECT type FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
				pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmType,wxT("type"),this);	
			}
	}
}

void CRadarStatusAlarm::getAlarmLevel()
{
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{
		for(auto item = m_alarmLevel.cbegin(); item != m_alarmLevel.cend(); item++)
		{
			m_currenteName = item->first;
			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256,"SELECT level FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmLevel,wxT("level"),this);	
		}

	}
}

void CRadarStatusAlarm::getAlarmCNname()
{
	auto pDataBase = GetPlug()->getDatabase();
	if (pDataBase)
	{
		for(auto item = m_RadarStatusContent.cbegin(); item != m_RadarStatusContent.cend(); item++)
		{
			m_currenteName = item->first;
			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256,"SELECT cName FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetAlarmCNname,wxT("cName"),this);	
		}

	}
}

void CRadarStatusAlarm::getThresholdValue()
{

	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{
		
		for(auto item = m_ThreadholdValues.cbegin(); item != m_ThreadholdValues.cend(); item++)
		{
			m_currenteName = item->first;
			char sqlQuery[256] = {'\0'};
			sprintf_s(sqlQuery,256, "SELECT lowerLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetLowerValue,wxT("lowerLimit"),this);


			sprintf_s(sqlQuery,256, "SELECT upperLimit FROM thresholdparameter WHERE eName = '%s'",item->first.c_str());
			pDataBase->RunSQLQuery(sqlQuery,cbSQLGetUpperValue,wxT("upperLimit"),this);
		}


	}
}

void CRadarStatusAlarm::getRadarStatusData(const RadarStructure::RadarStatus& radarStatus)
{
	m_RadarStatusValues.clear();
	if(::wxFinite (radarStatus.ANT_PEAK_PWR))
	{
		m_RadarStatusValues["ant_peak_pwr"] = radarStatus.ANT_PEAK_PWR;
	}
	else
	{
		m_RadarStatusValues["ant_peak_pwr"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.XMTR_PEAK_PWR))
	{
		m_RadarStatusValues["xmtr_peak_pwr"] = radarStatus.XMTR_PEAK_PWR;
	}
	else
	{
		m_RadarStatusValues["xmtr_peak_pwr"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.ANT_AVG_PWR))
	{
		m_RadarStatusValues["ant_avg_pwr"] = radarStatus.ANT_AVG_PWR;
	}
	else
	{
		m_RadarStatusValues["ant_avg_pwr"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.XMTR_AVG_PWR))
	{
		m_RadarStatusValues["xmtr_avg_pwr"] = radarStatus.XMTR_AVG_PWR;
	}
	else
	{
		m_RadarStatusValues["xmtr_avg_pwr"] = EXCEPTIONDATA;
	}


	if((::wxFinite (radarStatus.POWER_METER_RATIO_DB)) && (radarStatus.XMTR_PEAK_PWR > 50.0) && (radarStatus.ANT_PEAK_PWR > 50.0))
	{
		m_RadarStatusValues["power_meter_ratio_db"] = radarStatus.POWER_METER_RATIO_DB;
	}
	else 
	{
		m_RadarStatusValues["power_meter_ratio_db"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.ANT_PWR_MTR_ZERO))
	{
		m_RadarStatusValues["ant_pwr_mtr_zero"] = radarStatus.ANT_PWR_MTR_ZERO;
	}
	else 
	{
		m_RadarStatusValues["ant_pwr_mtr_zero"] = EXCEPTIONDATA;
	}


	if(::wxFinite (radarStatus.XMTR_PWR_MTR_ZERO))
	{
		m_RadarStatusValues["xmtr_pwr_mtr_zero"] = radarStatus.XMTR_PWR_MTR_ZERO;
	}
	else
	{
		m_RadarStatusValues["xmtr_pwr_mtr_zero"] = EXCEPTIONDATA;
	}

// 	if(GetPlug()->m_radarType == wxT("SA"))
// 	{
		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite (radarStatus.ANT_PWR_MTR_ZERO_V))
			{
				m_RadarStatusValues["ant_pwr_mtr_zero_v"] = radarStatus.ANT_PWR_MTR_ZERO_V;
			}
			else
			{
				m_RadarStatusValues["ant_pwr_mtr_zero_v"] = EXCEPTIONDATA;
			}
		}
// 	}
// 	else
// 	{
// 		if(::wxFinite (radarStatus.ANT_PWR_MTR_ZERO_V))
// 		{
// 			m_RadarStatusValues["ant_pwr_mtr_zero_v"] = radarStatus.ANT_PWR_MTR_ZERO_V;
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["ant_pwr_mtr_zero_v"] = EXCEPTIONDATA;
// 		}
// 	}


		if(::wxFinite (radarStatus.RNSPLN))
		{
			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(radarStatus.RNSPLN > 0.0)
				{
					auto temp = 10.0 * std::log10(radarStatus.RNSPLN);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["rnspln"] = temp;
					}
					else
					{
						m_RadarStatusValues["rnspln"] = EXCEPTIONDATA;
					}

					
				}
// 				else
// 				{
// 					m_RadarStatusValues["rnspln"] = EXCEPTIONDATA;
// 				}
			}
			else 
			{
// 				if(radarStatus.RNSPLN > 0.0)
// 				{
					m_RadarStatusValues["rnspln"] = radarStatus.RNSPLN;
//				}
// 				else
// 				{
// 					m_RadarStatusValues["rnspln"] = EXCEPTIONDATA;
// 				}
			}
		}
		else
		{
			m_RadarStatusValues["rnspln"] = EXCEPTIONDATA;
		}

// 	if(GetPlug()->m_radarType == wxT("SA"))
// 	{
		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite (radarStatus.ANT_PEAK_PWR_V))
			{
				m_RadarStatusValues["ant_peak_pwr_v"] = radarStatus.ANT_PEAK_PWR_V;
			}
			else
			{
				m_RadarStatusValues["ant_peak_pwr_v"] = EXCEPTIONDATA;
			}
		}
// 	}
// 	else
// 	{
// 		if(::wxFinite (radarStatus.ANT_PEAK_PWR_V))
// 		{
// 			m_RadarStatusValues["ant_peak_pwr_v"] = radarStatus.ANT_PEAK_PWR_V;
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["ant_peak_pwr_v"] = EXCEPTIONDATA;
// 		}
// 	}

	

		if(::wxFinite (radarStatus.RNLPLN))
		{
			if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
			{
				if(radarStatus.RNLPLN > 0.0)
				{

					auto temp = 10.0 * std::log10(radarStatus.RNLPLN);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["rnlpln"] = temp;
					}
					else
					{
						m_RadarStatusValues["rnlpln"] = EXCEPTIONDATA;
					}

					
				}
// 				else
// 				{
// 					m_RadarStatusValues["rnlpln"] = EXCEPTIONDATA;
// 				}
			}
			else 
			{
// 				if(radarStatus.RNLPLN > 0.0)
// 				{
					m_RadarStatusValues["rnlpln"] = radarStatus.RNLPLN;
//				}
// 				else
// 				{
// 					m_RadarStatusValues["rnlpln"] = EXCEPTIONDATA;
// 				}
			}
		}
		else
		{
			m_RadarStatusValues["rnlpln"] = EXCEPTIONDATA;
		}
	
// 	if(GetPlug()->m_radarType == wxT("SA"))
// 	{
		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite (radarStatus.ANT_AVG_PWR_V))
			{
				m_RadarStatusValues["ant_avg_pwr_v"] = radarStatus.ANT_AVG_PWR_V;
			}
			else
			{
				m_RadarStatusValues["ant_avg_pwr_v"] = EXCEPTIONDATA;
			}
		}
// 	}
// 	else
// 	{
// 		if(::wxFinite (radarStatus.ANT_AVG_PWR_V))
// 		{
// 			m_RadarStatusValues["ant_avg_pwr_v"] = radarStatus.ANT_AVG_PWR_V;
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["ant_avg_pwr_v"] = EXCEPTIONDATA;
// 		}
// 	}


	if(::wxFinite (radarStatus.TE))
	{
		if((radarStatus.TE > 0) && (radarStatus.TE < 1e3))
		{
			m_RadarStatusValues["te"] = radarStatus.TE;
		}
		else
		{
			m_RadarStatusValues["te"] = EXCEPTIONDATA;
		}
		
	}
	else
	{
		m_RadarStatusValues["te"] = EXCEPTIONDATA;
	}

	
	if(GetPlug()->m_radarType == wxT("CC"))
	{
		m_RadarStatusValues["pulse_width"] = radarStatus.PULSE_WIDTH *1.0;
	}
	else
	{
		m_RadarStatusValues["pulse_width"] = radarStatus.PULSE_WIDTH * 0.001;
	}
	

	

	if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
	{
		if(::wxFinite(radarStatus.NOISE_HCHAN[0]))
		{
			if(radarStatus.NOISE_HCHAN[0] > 0)
			{
				auto temp = 10.0 * std::log10(radarStatus.NOISE_HCHAN[0]);
				if(std::fabs(temp) < 1e5)
				{
					m_RadarStatusValues["noise_hchan_1"] = temp;
				}
				else
				{
					m_RadarStatusValues["noise_hchan_1"] = EXCEPTIONDATA;
				}

				
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_1"] = EXCEPTIONDATA;
		}

		if(::wxFinite(radarStatus.NOISE_HCHAN[1]))
		{
			if(radarStatus.NOISE_HCHAN[1] > 0)
			{

				auto temp = 10.0 * std::log10(radarStatus.NOISE_HCHAN[1]);
				if(std::fabs(temp) < 1e5)
				{
					m_RadarStatusValues["noise_hchan_2"] = temp;
				}
				else
				{
					m_RadarStatusValues["noise_hchan_2"] = EXCEPTIONDATA;
				}

				
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_2"] = EXCEPTIONDATA;
		}

		if(::wxFinite(radarStatus.NOISE_HCHAN[2]))
		{
			if(radarStatus.NOISE_HCHAN[2] > 0)
			{
				auto temp = 10.0 * std::log10(radarStatus.NOISE_HCHAN[2]);
				if(std::fabs(temp) < 1e5)
				{
					m_RadarStatusValues["noise_hchan_3"] = temp;
				}
				else
				{
					m_RadarStatusValues["noise_hchan_3"] = EXCEPTIONDATA;
				}

				
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_3"] = EXCEPTIONDATA;
		}

		if(::wxFinite(radarStatus.NOISE_HCHAN[3]))
		{
			if(radarStatus.NOISE_HCHAN[3] > 0)
			{
				auto temp = 10.0 * std::log10(radarStatus.NOISE_HCHAN[3]);
				if(std::fabs(temp) < 1e5)
				{
					m_RadarStatusValues["noise_hchan_4"] = temp;
				}
				else
				{
					m_RadarStatusValues["noise_hchan_4"] = EXCEPTIONDATA;
				}

				
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_4"] = EXCEPTIONDATA;
		}

		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite(radarStatus.NOISE_VCHAN[0]))
			{
				if(radarStatus.NOISE_VCHAN[0] > 0)
				{

					auto temp = 10.0 * std::log10(radarStatus.NOISE_VCHAN[0]);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["noise_vchan_1"] = temp;
					}
					else
					{
						m_RadarStatusValues["noise_vchan_1"] = EXCEPTIONDATA;
					}

					
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_1"] = EXCEPTIONDATA;
			}

			if(::wxFinite(radarStatus.NOISE_VCHAN[1]))
			{
				if(radarStatus.NOISE_VCHAN[1] > 0)
				{
					auto temp = 10.0 * std::log10(radarStatus.NOISE_VCHAN[1]);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["noise_vchan_2"] = temp;
					}
					else
					{
						m_RadarStatusValues["noise_vchan_2"] = EXCEPTIONDATA;
					}

					
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_2"] = EXCEPTIONDATA;
			}

			if(::wxFinite(radarStatus.NOISE_VCHAN[2]))
			{
				if(radarStatus.NOISE_VCHAN[2] > 0)
				{

					auto temp = 10.0 * std::log10(radarStatus.NOISE_VCHAN[2]);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["noise_vchan_3"] = temp;
					}
					else
					{
						m_RadarStatusValues["noise_vchan_3"] = EXCEPTIONDATA;
					}
					
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_3"] = EXCEPTIONDATA;
			}

			if(::wxFinite(radarStatus.NOISE_VCHAN[3]))
			{
				if(radarStatus.NOISE_VCHAN[3] > 0)
				{
					auto temp = 10.0 * std::log10(radarStatus.NOISE_VCHAN[3]);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["noise_vchan_4"] = temp;
					}
					else
					{
						m_RadarStatusValues["noise_vchan_4"] = EXCEPTIONDATA;
					}

				
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_4"] = EXCEPTIONDATA;
			}
		}

		if(::wxFinite(radarStatus.RNOISEHCHAN))
		{
			if(radarStatus.RNOISEHCHAN > 0)
			{
				auto temp = 10.0 * std::log10(radarStatus.RNOISEHCHAN);
				if(std::fabs(temp) < 1e5)
				{
					m_RadarStatusValues["rnoisehchan"] = temp;
				}
				else
				{
					m_RadarStatusValues["rnoisehchan"] = EXCEPTIONDATA;
				}

			
			}
		}
		else
		{
			m_RadarStatusValues["rnoisehchan"] = EXCEPTIONDATA;
		}


		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite(radarStatus.RNOISEVCHAN))
			{
				if(radarStatus.RNOISEVCHAN > 0)
				{
					auto temp = 10.0 * std::log10(radarStatus.RNOISEVCHAN);
					if(std::fabs(temp) < 1e5)
					{
						m_RadarStatusValues["rnoisevchan"] = temp;
					}
					else
					{
						m_RadarStatusValues["rnoisevchan"] = EXCEPTIONDATA;
					}

					
				}
			}
			else
			{
				m_RadarStatusValues["rnoisevchan"] = EXCEPTIONDATA;
			}

		}
	}
	else 
	{
		if(::wxFinite(radarStatus.NOISE_HCHAN[0]))
		{
			if(std::fabs(radarStatus.NOISE_HCHAN[0]) > 0)
			{
				m_RadarStatusValues["noise_hchan_1"] = radarStatus.NOISE_HCHAN[0];
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_1"] = EXCEPTIONDATA;
		}

		if(::wxFinite(radarStatus.NOISE_HCHAN[1]))
		{
			if(std::fabs(radarStatus.NOISE_HCHAN[1]) > 0)
			{
				m_RadarStatusValues["noise_hchan_2"] = radarStatus.NOISE_HCHAN[1];
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_2"] = EXCEPTIONDATA;
		}

		if(::wxFinite(radarStatus.NOISE_HCHAN[2]))
		{
			if(std::fabs(radarStatus.NOISE_HCHAN[2]) > 0)
			{
				m_RadarStatusValues["noise_hchan_3"] = radarStatus.NOISE_HCHAN[2];
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_3"] = EXCEPTIONDATA;
		}

		if(::wxFinite(radarStatus.NOISE_HCHAN[3]))
		{
			if(std::fabs(radarStatus.NOISE_HCHAN[3]) > 0)
			{
				m_RadarStatusValues["noise_hchan_4"] = radarStatus.NOISE_HCHAN[3];
			}
		}
		else
		{
			m_RadarStatusValues["noise_hchan_4"] = EXCEPTIONDATA;
		}

		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite(radarStatus.NOISE_VCHAN[0]))
			{
				if(std::fabs(radarStatus.NOISE_VCHAN[0]) > 0)
				{
					m_RadarStatusValues["noise_vchan_1"] =radarStatus.NOISE_VCHAN[0];
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_1"] = EXCEPTIONDATA;
			}

			if(::wxFinite(radarStatus.NOISE_VCHAN[1]))
			{
				if(std::fabs(radarStatus.NOISE_VCHAN[1]) > 0)
				{
					m_RadarStatusValues["noise_vchan_2"] = radarStatus.NOISE_VCHAN[1];
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_2"] = EXCEPTIONDATA;
			}

			if(::wxFinite(radarStatus.NOISE_VCHAN[2]))
			{
				if(std::fabs(radarStatus.NOISE_VCHAN[2]) > 0)
				{
					m_RadarStatusValues["noise_vchan_3"] = radarStatus.NOISE_VCHAN[2];
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_3"] = EXCEPTIONDATA;
			}

			if(::wxFinite(radarStatus.NOISE_VCHAN[3]))
			{
				if(std::fabs(radarStatus.NOISE_VCHAN[3]) > 0)
				{
					m_RadarStatusValues["noise_vchan_4"] = radarStatus.NOISE_VCHAN[3];
				}
			}
			else
			{
				m_RadarStatusValues["noise_vchan_4"] = EXCEPTIONDATA;
			}
		}
		
		if(::wxFinite(radarStatus.RNOISEHCHAN))
		{
			if(std::fabs(radarStatus.RNOISEHCHAN) > 0)
			{
				m_RadarStatusValues["rnoisehchan"] =radarStatus.RNOISEHCHAN;
			}
		}
		else
		{
			m_RadarStatusValues["rnoisehchan"] = EXCEPTIONDATA;
		}


		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite(radarStatus.RNOISEVCHAN))
			{
				if(std::fabs(radarStatus.RNOISEVCHAN) > 0)
				{
					m_RadarStatusValues["rnoisevchan"] = radarStatus.RNOISEVCHAN;
				}
			}
			else
			{
				m_RadarStatusValues["rnoisevchan"] = EXCEPTIONDATA;
			}

		}
	}

		
	
	if((GetPlug()->m_radarType == wxT("SA")) || (GetPlug()->m_radarType == wxT("SB")) || (GetPlug()->m_radarType == wxT("CA")) || (GetPlug()->m_radarType == wxT("CB")))
	{
		if(radarStatus.POL == BIPOLE)
		{
			m_RadarStatusValues["pdp"] = radarStatus.PDP;

			m_RadarStatusValues["zdr"] = radarStatus.ZDR;

		}
	}
	else
	{
		m_RadarStatusValues["pdp"] = radarStatus.PDP;

		m_RadarStatusValues["zdr"] = radarStatus.ZDR;
	}

// 	if(GetPlug()->m_radarType == wxT("SA"))
// 	{
		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite (radarStatus.TE_V))
			{
				if((radarStatus.TE_V  > 0) && (radarStatus.TE_V < 1e3))
				{
					m_RadarStatusValues["te_v"] = radarStatus.TE_V;
				}
			}
			else
			{
				m_RadarStatusValues["te_v"] = EXCEPTIONDATA;
			}
		}
// 	}
// 	else
// 	{
// 		if(::wxFinite (radarStatus.TE_V))
// 		{
// 			if((radarStatus.TE_V  > 0) && (radarStatus.TE_V < 1e3))
// 			{
// 				m_RadarStatusValues["te_v"] = radarStatus.TE_V;
// 			}
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["te_v"] = EXCEPTIONDATA;
// 		}
// 	}




	if(::wxFinite (radarStatus.SYSCAL[0]))
	{
		m_RadarStatusValues["syscal_1"] = radarStatus.SYSCAL[0];
	}
	else
	{
		m_RadarStatusValues["syscal_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SYSCAL[1]))
	{
		m_RadarStatusValues["syscal_2"] = radarStatus.SYSCAL[1];
	}
	else
	{
		m_RadarStatusValues["syscal_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SYSCAL[2]))
	{
		m_RadarStatusValues["syscal_3"] = radarStatus.SYSCAL[2];
	}
	else
	{
		m_RadarStatusValues["syscal_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SYSCAL[3]))
	{
		m_RadarStatusValues["syscal_4"] = radarStatus.SYSCAL[3];
	}
	else
	{
		m_RadarStatusValues["syscal_4"] = EXCEPTIONDATA;
	}
	

	
	if(::wxFinite (radarStatus.EXPECTED_REFL_AMP[0]))
	{
		m_RadarStatusValues["expected_refl_amp_1"] = radarStatus.EXPECTED_REFL_AMP[0];
	}
	else
	{
		m_RadarStatusValues["expected_refl_amp_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_REFL_AMP[1]))
	{
		m_RadarStatusValues["expected_refl_amp_2"] = radarStatus.EXPECTED_REFL_AMP[1];
	}
	else
	{
		m_RadarStatusValues["expected_refl_amp_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_REFL_AMP[2]))
	{
		m_RadarStatusValues["expected_refl_amp_3"] = radarStatus.EXPECTED_REFL_AMP[2];
	}
	else
	{
		m_RadarStatusValues["expected_refl_amp_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_REFL_AMP[3]))
	{
		m_RadarStatusValues["expected_refl_amp_4"] = radarStatus.EXPECTED_REFL_AMP[3];
	}
	else
	{
		m_RadarStatusValues["expected_refl_amp_4"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_REFL_AMP[0]))
	{
		m_RadarStatusValues["measured_refl_amp_1"] = radarStatus.MEASURED_REFL_AMP[0];
	}
	else
	{
		m_RadarStatusValues["measured_refl_amp_1"] = EXCEPTIONDATA;

	}

	if(::wxFinite (radarStatus.MEASURED_REFL_AMP[1]))
	{
		m_RadarStatusValues["measured_refl_amp_2"] = radarStatus.MEASURED_REFL_AMP[1];
	}
	else
	{
		m_RadarStatusValues["measured_refl_amp_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_REFL_AMP[2]))
	{
		m_RadarStatusValues["measured_refl_amp_3"] = radarStatus.MEASURED_REFL_AMP[2];
	}
	else
	{
		m_RadarStatusValues["measured_refl_amp_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_REFL_AMP[3]))
	{
		m_RadarStatusValues["measured_refl_amp_4"] = radarStatus.MEASURED_REFL_AMP[3];
	}
	else
	{
		m_RadarStatusValues["measured_refl_amp_4"] = EXCEPTIONDATA;
	}


	if(::wxFinite (radarStatus.SYSSPLN))
	{
		m_RadarStatusValues["sysspln"] = radarStatus.SYSSPLN;
	}
	else
	{
		m_RadarStatusValues["sysspln"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SYSLPLN))
	{
		m_RadarStatusValues["syslpln"] = radarStatus.SYSLPLN;
	}
	else
	{
		m_RadarStatusValues["syslpln"] = EXCEPTIONDATA;
	}

	


	if(::wxFinite (radarStatus.EXPECTED_VELOCITY[0]))
	{
		m_RadarStatusValues["expected_velocity_1"] = radarStatus.EXPECTED_VELOCITY[0];
	}
	else
	{
		m_RadarStatusValues["expected_velocity_1"] = EXCEPTIONDATA;
	}
	 
	if(::wxFinite (radarStatus.EXPECTED_VELOCITY[1]))
	{
		m_RadarStatusValues["expected_velocity_2"] = radarStatus.EXPECTED_VELOCITY[1];
	}
	else
	{
		m_RadarStatusValues["expected_velocity_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_VELOCITY[2]))
	{
		m_RadarStatusValues["expected_velocity_3"] = radarStatus.EXPECTED_VELOCITY[2];
	}
	else
	{
		m_RadarStatusValues["expected_velocity_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_VELOCITY[3]))
	{
		m_RadarStatusValues["expected_velocity_4"] = radarStatus.EXPECTED_VELOCITY[3];
	}
	else
	{
		m_RadarStatusValues["expected_velocity_4"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_VELOCITY[0]))
	{
		m_RadarStatusValues["measured_velocity_1"] = radarStatus.MEASURED_VELOCITY[0];
	}
	else
	{
		m_RadarStatusValues["measured_velocity_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_VELOCITY[1]))
	{
		m_RadarStatusValues["measured_velocity_2"] = radarStatus.MEASURED_VELOCITY[1];
	}
	else
	{
		m_RadarStatusValues["measured_velocity_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_VELOCITY[2]))
	{
		m_RadarStatusValues["measured_velocity_3"] = radarStatus.MEASURED_VELOCITY[2];
	}
	else
	{
		m_RadarStatusValues["measured_velocity_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_VELOCITY[3]))
	{
		m_RadarStatusValues["measured_velocity_4"] = radarStatus.MEASURED_VELOCITY[3];
	}
	else
	{
		m_RadarStatusValues["measured_velocity_4"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_WIDTH[0]))
	{
		m_RadarStatusValues["expected_width_1"] = radarStatus.EXPECTED_WIDTH[0];
	}
	else
	{
		m_RadarStatusValues["expected_width_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_WIDTH[1]))
	{
		m_RadarStatusValues["expected_width_2"] = radarStatus.EXPECTED_WIDTH[1];
	}
	else
	{
		m_RadarStatusValues["expected_width_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_WIDTH[2]))
	{
		m_RadarStatusValues["expected_width_3"] = radarStatus.EXPECTED_WIDTH[2];
	}
	else
	{
		m_RadarStatusValues["expected_width_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_WIDTH[3]))
	{
		m_RadarStatusValues["expected_width_4"] = radarStatus.EXPECTED_WIDTH[3];
	}
	else
	{
		m_RadarStatusValues["expected_width_4"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_WIDTH[0]))
	{
		m_RadarStatusValues["measured_width_1"] = radarStatus.MEASURED_WIDTH[0];
	}
	else
	{
		m_RadarStatusValues["measured_width_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_WIDTH[1]))
	{
		m_RadarStatusValues["measured_width_2"] = radarStatus.MEASURED_WIDTH[1];
	}
	else
	{
		m_RadarStatusValues["measured_width_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_WIDTH[2]))
	{
		m_RadarStatusValues["measured_width_3"] = radarStatus.MEASURED_WIDTH[2];
	}
	else
	{
		m_RadarStatusValues["measured_width_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_WIDTH[3]))
	{
		m_RadarStatusValues["measured_width_4"] = radarStatus.MEASURED_WIDTH[3];
	}
	else
	{
		m_RadarStatusValues["measured_width_4"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SHELTER_TEMP))
	{
		m_RadarStatusValues["shelter_temp"] = radarStatus.SHELTER_TEMP;
	}
	else
	{
		m_RadarStatusValues["shelter_temp"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.XMTR_AIR_TEMP))
	{
		m_RadarStatusValues["xmtr_air_temp"] = radarStatus.XMTR_AIR_TEMP;
	}
	else
	{
		m_RadarStatusValues["xmtr_air_temp"] = EXCEPTIONDATA;
	}
	if(::wxFinite (radarStatus.RADOME_TEMP))
	{
		m_RadarStatusValues["radome_temp"] = radarStatus.RADOME_TEMP;
	}
	else
	{
		m_RadarStatusValues["radome_temp"] = EXCEPTIONDATA;
	}
	
	if(::wxFinite (radarStatus.EXPECTED_RF8_AMP[0]))
	{
		m_RadarStatusValues["expected_rf8_amp_1"] = radarStatus.EXPECTED_RF8_AMP[0];
	}
	else
	{
		m_RadarStatusValues["expected_rf8_amp_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_RF8_AMP[1]))
	{
		m_RadarStatusValues["expected_rf8_amp_2"] = radarStatus.EXPECTED_RF8_AMP[1];
	}
	else 
	{
		m_RadarStatusValues["expected_rf8_amp_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.EXPECTED_RF8_AMP[2]))
	{
		m_RadarStatusValues["expected_rf8_amp_3"] = radarStatus.EXPECTED_RF8_AMP[2];
	}
	else
	{
		m_RadarStatusValues["expected_rf8_amp_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SHELTER_HUMIDITY))
	{
		m_RadarStatusValues["shelter_humidity"] = radarStatus.SHELTER_HUMIDITY;
	}
	else
	{
		m_RadarStatusValues["shelter_humidity"] = EXCEPTIONDATA;
	}
	
	if(::wxFinite (radarStatus.XMTR_AIR_HUMIDITY))
	{
		m_RadarStatusValues["xmtr_air_humidity"] = radarStatus.XMTR_AIR_HUMIDITY;
	}
	else
	{
		m_RadarStatusValues["xmtr_air_humidity"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.RADOME_HUMIDITY))
	{
		m_RadarStatusValues["radome_humidity"] = radarStatus.RADOME_HUMIDITY;
	}
	else 
	{
		m_RadarStatusValues["radome_humidity"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_RF8_AMP[0]))
	{
		m_RadarStatusValues["measured_rf8_amp_1"] = radarStatus.MEASURED_RF8_AMP[0];
	}
	else
	{
		m_RadarStatusValues["measured_rf8_amp_1"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_RF8_AMP[1]))
	{
		m_RadarStatusValues["measured_rf8_amp_2"] = radarStatus.MEASURED_RF8_AMP[1];
	}
	else
	{
		m_RadarStatusValues["measured_rf8_amp_2"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.MEASURED_RF8_AMP[2]))
	{
		m_RadarStatusValues["measured_rf8_amp_3"] = radarStatus.MEASURED_RF8_AMP[2];
	}
	else
	{
		m_RadarStatusValues["measured_rf8_amp_3"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.PHASE_NOISE))
	{
		m_RadarStatusValues["phase_noise"] = radarStatus.PHASE_NOISE;
	}
	else
	{
		m_RadarStatusValues["phase_noise"] = EXCEPTIONDATA;
	}


// 	if(GetPlug()->m_radarType == wxT("SA"))
// 	{
		if(radarStatus.POL == BIPOLE)
		{
			if(::wxFinite (radarStatus.PHASE_NOISE_V))
			{
				m_RadarStatusValues["phase_noise_v"] = radarStatus.PHASE_NOISE_V;
			}
			else
			{
				m_RadarStatusValues["phase_noise_v"] = EXCEPTIONDATA;
			}

			if(::wxFinite (radarStatus.UNFILTERED_V_CHAN_PWR))
			{
				m_RadarStatusValues["unfiltered_v_chan_pwr"] = radarStatus.UNFILTERED_V_CHAN_PWR;
			}
			else
			{
				m_RadarStatusValues["unfiltered_v_chan_pwr"] = EXCEPTIONDATA;
			}

			if(::wxFinite (radarStatus.FILTERED_V_CHAN_PWR))
			{
				m_RadarStatusValues["filtered_v_chan_pwr"] = radarStatus.FILTERED_V_CHAN_PWR;
			}
			else
			{
				m_RadarStatusValues["filtered_v_chan_pwr"] = EXCEPTIONDATA;
			}
		}
// 	}
// 	else
// 	{
// 		if(::wxFinite (radarStatus.PHASE_NOISE_V))
// 		{
// 			m_RadarStatusValues["phase_noise_v"] = radarStatus.PHASE_NOISE_V;
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["phase_noise_v"] = EXCEPTIONDATA;
// 		}
// 
// 		if(::wxFinite (radarStatus.UNFILTERED_V_CHAN_PWR))
// 		{
// 			m_RadarStatusValues["unfiltered_v_chan_pwr"] = radarStatus.UNFILTERED_V_CHAN_PWR;
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["unfiltered_v_chan_pwr"] = EXCEPTIONDATA;
// 		}
// 
// 		if(::wxFinite (radarStatus.FILTERED_V_CHAN_PWR))
// 		{
// 			m_RadarStatusValues["filtered_v_chan_pwr"] = radarStatus.FILTERED_V_CHAN_PWR;
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["filtered_v_chan_pwr"] = EXCEPTIONDATA;
// 		}
// 	}
	
	
	if(::wxFinite (radarStatus.UNFILTERED_LIN_CHAN_PWR))
	{
		m_RadarStatusValues["unfiltered_lin_chan_pwr"] = radarStatus.UNFILTERED_LIN_CHAN_PWR;
	}
	else
	{
		m_RadarStatusValues["unfiltered_lin_chan_pwr"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.FILTERED_LIN_CHAN_PWR))
	{
		m_RadarStatusValues["filtered_lin_chan_pwr"] = radarStatus.FILTERED_LIN_CHAN_PWR;
	}
	else
	{
		m_RadarStatusValues["filtered_lin_chan_pwr"] = EXCEPTIONDATA;
	}

	if(::wxFinite (radarStatus.SYSCAL_DELTA))
	{
		m_RadarStatusValues["syscal_delta"] = radarStatus.SYSCAL_DELTA;
	}
	else
	{
		m_RadarStatusValues["syscal_delta"] = EXCEPTIONDATA;
	}

	if((radarStatus.TE > 0) && (radarStatus.TE < 1e3))
	{
		m_RadarStatusValues["noise_coefficient_h"] = 10 * std::log10(radarStatus.TE / 290.0 + 1);
	}
	else
	{
		m_RadarStatusValues["noise_coefficient_h"] = EXCEPTIONDATA;
	}

// 	if(GetPlug()->m_radarType == wxT("SA"))
// 	{
		if(radarStatus.POL == BIPOLE)
		{
			if((radarStatus.TE_V  > 0) && (radarStatus.TE_V < 1e3))
			{
				m_RadarStatusValues["noise_coefficient_v"] = 10 * std::log10(radarStatus.TE_V / 290.0 + 1);
			}
// 			else
// 			{
// 				m_RadarStatusValues["noise_coefficient_v"] = EXCEPTIONDATA;
// 			}
		}
// 	}
// 	else
// 	{
// 		if((radarStatus.TE_V  > 0) && (radarStatus.TE_V < 1e3))
// 		{
// 			m_RadarStatusValues["noise_coefficient_v"] = 10 * std::log10(radarStatus.TE_V / 290.0 + 1);
// 		}
// 		else
// 		{
// 			m_RadarStatusValues["noise_coefficient_v"] = EXCEPTIONDATA;
// 		}
//	}



}

void CRadarStatusAlarm::RadarStatusCheck()
{
	for(auto item = m_RadarStatusValues.cbegin(); item != m_RadarStatusValues.cend(); item++)
	{
		auto name = item->first;
		
		if(m_ThreadholdValues.find(name) != m_ThreadholdValues.cend())  //�����Ҫ�����Ĳ��������ݿ����м�¼�򱨾�
		{
		
			if(m_alarmLevel.find(name) == m_alarmLevel.cend())  //�����ȼ�����Ҫ�иò��������Ƽ�¼
			{
				continue;
			}

			if(m_alarmLevel[name] < 0)  //�������ȼ�С��0�򲻱���
			{
				continue;
			}

			auto value = m_RadarStatusValues[name];
			auto lowerLimit = m_ThreadholdValues[name].first;
			auto upperLimit = m_ThreadholdValues[name].second;

			if((std::fabs(lowerLimit) < ZEROBIAS) && (std::fabs(upperLimit) < ZEROBIAS))  //����ֵ��Ϊ0�򲻱���
			{
				continue;
			}
			

			if(m_RadarStatusExpFlag[name])  
			{
				if((value <= upperLimit) && (value >= lowerLimit))  //�����������󣬲����ػ�������������Т��
				{
					m_RadarStatusExpFlag[name] = false;
					m_RadarStatusEndureTime[name].second = ::wxDateTime::GetTimeNow();
					writeToDataBasewithEndTime(name);
				}

			}
			else
			{
				if((value > upperLimit) || (value < lowerLimit))  //������ֵ������
				{
					m_RadarStatusExpFlag[name] = true;
					m_RadarStatusEndureTime[name].first = ::wxDateTime::GetTimeNow();
					m_RadarStatusConcreteContent[name] = m_RadarStatusContent[name];
					if(m_RadarStatusValues[name] > EXCEPTIONDATA)  //����ò���ֵ����Ч���������򱨾�
					{
						if(name.find("measured") != std::string::npos)
						{
							std::string expectedName = name;
							expectedName.replace(0,std::strlen("measured"),"expected");
							m_RadarStatusConcreteContent[name] += wxString::Format(wxT("ֵΪ%.2f,������ֵ֮��Ĳ�ֵΪ%.2f"),m_RadarStatusValues[name],m_RadarStatusValues[name] - m_RadarStatusValues[expectedName]).ToStdString();
						}
						else
						{
							m_RadarStatusConcreteContent[name] += wxString::Format(wxT("ֵΪ%.2f,������ֵ��Χ[%.2f,%.2f]"),m_RadarStatusValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
						}
					}
					else
					{
						m_RadarStatusConcreteContent[name] += wxString(wxT("ֵ�쳣"));
					}
					
					writeToDataBase(name);

				}
				//else if(value < lowerLimit)
				//{
				//	m_RadarStatusExpFlag[name] = true;
				//	m_RadarStatusEndureTime[name].first = ::wxDateTime::GetTimeNow();
				//	m_RadarStatusConcreteContent[name] = m_RadarStatusContent[name];
				//	m_RadarStatusConcreteContent[name] += wxString::Format(wxT("ֵΪ%.2f,������ֵ��Χ[%.2f,%.2f]"),m_RadarStatusValues[name],m_ThreadholdValues[name].first,m_ThreadholdValues[name].second).ToStdString();
				//	writeToDataBase(name);
				//}
				else{}
			}

		}

	}
}



bool CRadarStatusAlarm::writeToDataBase(const std::string name)
{
	wxString sqlSentence = wxT("INSERT INTO alarminfo (time, endTime, type, alarmName,level, mode, content, origin) VALUES");
	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{

	
			sqlSentence.Append(wxString::Format(wxT("(%I64d, "), m_RadarStatusEndureTime[name].first));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_alarmType[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%s, "), wxT("NULL")));
			sqlSentence.Append(wxString::Format(wxT("%d, "), m_alarmLevel[name]));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxT("0")));
			sqlSentence.Append(wxString::Format(wxT("'%s', "), wxString(m_RadarStatusConcreteContent[name].c_str(),wxConvLocal)));
			sqlSentence.Append(wxString::Format(wxT("%d)"), 0));
	

			bool ret =  pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}

	}
	return true;
}

bool CRadarStatusAlarm::writeToDataBasewithEndTime(const std::string name)
{

	auto pDataBase = GetPlug()->getDatabase();

	if (pDataBase)
	{

			wxString sqlSentence;
			sqlSentence.Printf(wxT("UPDATE alarminfo SET endTime=%I64d WHERE endTime IS NULL AND time=%I64d AND content='%s'"), m_RadarStatusEndureTime[name].second,
																										m_RadarStatusEndureTime[name].first,
																										wxString(m_RadarStatusConcreteContent[name].c_str(),wxConvLocal));
		
			bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
			if(!ret)
			{
				SHOWFUNCNAME();
			}

	}
	
	return true;
}

int CRadarStatusAlarm::cbSQLGetAlarmType(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CRadarStatusAlarm *pT = static_cast<CRadarStatusAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmType(std::string(argv[0]));
	}

	return 0;
}

int CRadarStatusAlarm::cbSQLGetLowerValue(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CRadarStatusAlarm *pT = static_cast<CRadarStatusAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetLowerValue(std::string(argv[0]));
	}

	return 0;
}

int CRadarStatusAlarm::cbSQLGetUpperValue(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CRadarStatusAlarm *pT = static_cast<CRadarStatusAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetUpperValue(std::string(argv[0]));
	}

	return 0;
}

int CRadarStatusAlarm::cbSQLGetAlarmCNname(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CRadarStatusAlarm *pT = static_cast<CRadarStatusAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmCNname(std::string(argv[0]));
	}

	return 0;
}

int CRadarStatusAlarm::cbSQLGetAlarmLevel(void *pIn/*����Ĳ���*/, int argc, char **argv, char **azColName)
{
	CRadarStatusAlarm *pT = static_cast<CRadarStatusAlarm*>(pIn);
	if(pT)
	{
		pT->cbGetAlarmLevel(std::string(argv[0]));
	}

	return 0;
}

