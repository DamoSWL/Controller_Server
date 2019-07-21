#include "UPSHandlerRS232.h"


UPSHandlerRS232::UPSHandlerRS232(void):m_udpHandler(NULL),m_newDataFlag(FALSE)
{
}


UPSHandlerRS232::~UPSHandlerRS232(void)
{
}

DWORD WINAPI UPSHandlerRS232::exec(LPVOID para)
{
	UPSHandlerRS232* self = static_cast<UPSHandlerRS232*>(para);
	if(self)
	{
		if(self->startReceive())   //打开网络端口
		{
			while (!GetPlug()->GetExit())  //在循坏中每隔一段时间查询一次
			{

				INT cnt = 5;  //发送次数，有些站台网络不稳定，要多发几次，该变量设置发送次数

				if(GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND)   //根据不同台站发送不同命令
				{
					do
					{
						self->queryUPSData();		//发送UPS查询指令			
						::wxSleep(2);
						if(GetPlug()->GetExit())
						{
							break;
						}

					}while((!self->m_newDataFlag) && (cnt--));
				}
				else if((GetPlug()->m_Radarcd.Find(wxT("Z9774")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9776")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9778")) != wxNOT_FOUND))
				{
					self->queryUPSData();
				}
				else if((GetPlug()->m_Radarcd.Find(wxT("Z9531")) != wxNOT_FOUND))//济南站
				{
					self->queryUPSData();
				}
				else if((GetPlug()->m_Radarcd.Find(wxT("Z9230")) != wxNOT_FOUND))//重庆站
				{
					self->queryUPSData();
				}
				else{}


				::wxSleep(2);  

				if(self->m_newDataFlag)  //若接收到数据，则将数据写入数据库
				{
					self->writeToDataBase();
					self->m_newDataFlag = FALSE;
				}
				
				
				for(UINT index = 0;index < GetPlug()->getfileConfig().UPSInterval*60;index++)
				{
					if(GetPlug()->GetExit())
					{
						break;
					}
					::wxSleep(1);
				}
			}

			self->stopReceive();
		}
	}

	return EXIT_SUCCESS;
}

int UPSHandlerRS232::processData(void *pHandle, const char *pData, UINT Len)  
{
	UPSHandlerRS232* self = static_cast<UPSHandlerRS232*>(pHandle);
	if(self)
	{
		if(GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND)  //根据不同台站不同UPS型号处理相应数据
		{
			if((pData[0] == 0x01) && (pData[1] == 0x04))
			{
				self->m_newDataFlag = TRUE;

				self->m_UPSDataRS232.inputLine1Vol = ((UCHAR)pData[3] * 256 + (UCHAR)pData[4]) * 0.1;
				self->m_UPSDataRS232.inputLine2Vol = ((UCHAR)pData[5] * 256 + (UCHAR)pData[6]) * 0.1;
				self->m_UPSDataRS232.inputLine3Vol = ((UCHAR)pData[7] * 256 + (UCHAR)pData[8]) * 0.1;

				self->m_UPSDataRS232.inputLine1Fre = ((UCHAR)pData[9] * 256 + (UCHAR)pData[10]) * 0.1;
				self->m_UPSDataRS232.inputLine2Fre = ((UCHAR)pData[9] * 256 + (UCHAR)pData[10]) * 0.1;
				self->m_UPSDataRS232.inputLine3Fre = ((UCHAR)pData[9] * 256 + (UCHAR)pData[10]) * 0.1;

				self->m_UPSDataRS232.inputLine1Cur = ((UCHAR)pData[11] * 256 + (UCHAR)pData[12]) * 0.1;
				self->m_UPSDataRS232.inputLine2Cur = ((UCHAR)pData[13] * 256 + (UCHAR)pData[14]) * 0.1;
				self->m_UPSDataRS232.inputLine3Cur = ((UCHAR)pData[15] * 256 + (UCHAR)pData[16]) * 0.1;

				self->m_UPSDataRS232.inputLine1Power = self->m_UPSDataRS232.inputLine1Vol * self->m_UPSDataRS232.inputLine1Cur / 1000;
				self->m_UPSDataRS232.inputLine2Power = self->m_UPSDataRS232.inputLine2Vol * self->m_UPSDataRS232.inputLine2Cur / 1000;
				self->m_UPSDataRS232.inputLine3Power = self->m_UPSDataRS232.inputLine3Vol * self->m_UPSDataRS232.inputLine3Cur / 1000;


				self->m_UPSDataRS232.outputLine1Vol = ((UCHAR)pData[23] * 256 + (UCHAR)pData[24]) * 0.1;
				self->m_UPSDataRS232.outputLine2Vol = ((UCHAR)pData[25] * 256 + (UCHAR)pData[26]) * 0.1;
				self->m_UPSDataRS232.outputLine3Vol = ((UCHAR)pData[27] * 256 + (UCHAR)pData[28]) * 0.1;

				self->m_UPSDataRS232.outputFre = ((UCHAR)pData[29] * 256 + (UCHAR)pData[30]) * 0.1;

				self->m_UPSDataRS232.outputLine1Cur = ((UCHAR)pData[31] * 256 + (UCHAR)pData[32]) * 0.1;
				self->m_UPSDataRS232.outputLine2Cur = ((UCHAR)pData[33] * 256 + (UCHAR)pData[34]) * 0.1;
				self->m_UPSDataRS232.outputLine3Cur = ((UCHAR)pData[35] * 256 + (UCHAR)pData[36]) * 0.1;

				self->m_UPSDataRS232.outputLine1Power = ((UCHAR)pData[37] * 256 + (UCHAR)pData[38]) * 0.1;
				self->m_UPSDataRS232.outputLine2Power = ((UCHAR)pData[39] * 256 + (UCHAR)pData[40]) * 0.1;
				self->m_UPSDataRS232.outputLine3Power = ((UCHAR)pData[41] * 256 + (UCHAR)pData[42]) * 0.1;
				self->m_UPSDataRS232.outputTotalPower = self->m_UPSDataRS232.outputLine1Power + self->m_UPSDataRS232.outputLine2Power + self->m_UPSDataRS232.outputLine3Power;

				self->m_UPSDataRS232.outputLine1Load = ((UCHAR)pData[43] * 256 + (UCHAR)pData[44]);
				self->m_UPSDataRS232.outputLine2Load = ((UCHAR)pData[45] * 256 + (UCHAR)pData[46]);
				self->m_UPSDataRS232.outputLine3Load = ((UCHAR)pData[47] * 256 + (UCHAR)pData[48]);

				self->m_UPSDataRS232.bypassLine1Vol = ((UCHAR)pData[55] * 256 + (UCHAR)pData[56]) * 0.1;
				self->m_UPSDataRS232.bypassLine2Vol = ((UCHAR)pData[57] * 256 + (UCHAR)pData[58]) * 0.1;
				self->m_UPSDataRS232.bypassLine3Vol = ((UCHAR)pData[59] * 256 + (UCHAR)pData[60]) * 0.1;

				self->m_UPSDataRS232.bypassFre = ((UCHAR)pData[61] * 256 + (UCHAR)pData[62]) * 0.1;

				self->m_UPSDataRS232.batteryVol = ((UCHAR)pData[63] * 256 + (UCHAR)pData[64]) * 0.1;
				self->m_UPSDataRS232.batteryCurrent = ((UCHAR)pData[67] * 256 + (UCHAR)pData[68]) * 0.1;
				self->m_UPSDataRS232.batteryCap = (UCHAR)pData[75] * 256 + (UCHAR)pData[76];
				self->m_UPSDataRS232.estimatedMinutesOnBat = (UCHAR)pData[77] * 256 + (UCHAR)pData[78];
				self->m_UPSDataRS232.upsFaultId =  ((UCHAR)pData[81] * 256 + (UCHAR)pData[82]) * 0.1;
			}
		}
		else if((GetPlug()->m_Radarcd.Find(wxT("Z9774")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9776")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9778")) != wxNOT_FOUND)) //广西台站的科士达EP60型号
		{
			self->m_newDataFlag = TRUE;
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,pData);

			std::string content(pData,Len);
			if((content.find("IN") != std::string::npos) && (content.find("BATT") != std::string::npos))
			{
				auto Pos = content.find("=");
				auto previousPos = Pos + 1;
				Pos = content.find(",");

				auto tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine1Vol  = std::stof(tmpStr) * 2.350;
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine2Vol = std::stof(tmpStr) * 2.350;
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine3Vol = std::stof(tmpStr) * 2.350;
				previousPos = Pos + 1;


				Pos = content.find("Hz",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine1Fre = std::stof(tmpStr);
				self->m_UPSDataRS232.inputLine2Fre = std::stof(tmpStr);
				self->m_UPSDataRS232.inputLine3Fre = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find("BY",previousPos);
				previousPos = Pos + 1;
				Pos = content.find(",",previousPos);
				previousPos = Pos + 1;
				Pos = content.find("Hz",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.bypassFre = std::stof(tmpStr);

				Pos = content.find("OUT",previousPos);
				self->m_UPSDataRS232.bypass = 0;  //市电供电
				if(Pos == std::string::npos)
				{
					self->m_UPSDataRS232.bypass = 1;   //旁路供电
					Pos = content.find("BY",previousPos);
				}

				previousPos = Pos + 1;
				Pos = content.find(",",previousPos);
				previousPos = Pos + 1;
				Pos = content.find(",",previousPos);

				if(Pos == std::string::npos)
				{
					return EXIT_FAILURE;
				}

				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.outputFre = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find("%",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.outputLine1Load = std::stof(tmpStr);
				self->m_UPSDataRS232.outputLine2Load = std::stof(tmpStr);
				self->m_UPSDataRS232.outputLine3Load = std::stof(tmpStr);

			}
			else if((content.find("IN") != std::string::npos) && (content.find("Ts") != std::string::npos))
			{
				auto Pos = content.find("=");
				auto previousPos = Pos + 1;
				Pos = content.find(",");

				auto tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine1Cur = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine2Cur = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(";",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.inputLine3Cur = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find("OUT",previousPos);
				self->m_UPSDataRS232.bypass = 0;  //市电供电
				if(Pos == std::string::npos)
				{
					self->m_UPSDataRS232.bypass = 1;   //旁路供电
					Pos = content.find("BY",previousPos);
				}

				previousPos = Pos + 1;
				Pos = content.find("=",previousPos);
				previousPos = Pos + 1;
				Pos = content.find(",",previousPos);

				if(Pos == std::string::npos)
				{
					return EXIT_FAILURE; 
				}

				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.outputLine1Cur = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.outputLine2Cur = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find("Arms",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.outputLine3Cur = std::stof(tmpStr);

				self->m_UPSDataRS232.inputLine1Power = self->m_UPSDataRS232.inputLine1Vol * self->m_UPSDataRS232.inputLine1Cur / 1000;
				self->m_UPSDataRS232.inputLine2Power = self->m_UPSDataRS232.inputLine2Vol * self->m_UPSDataRS232.inputLine2Cur / 1000;
				self->m_UPSDataRS232.inputLine3Power = self->m_UPSDataRS232.inputLine3Vol * self->m_UPSDataRS232.inputLine3Cur / 1000;
			}
			else if(content.find("%Apk") != std::string::npos)
			{
				auto Pos = content.find("=");
				auto previousPos = Pos + 1;
				Pos = content.find(",");

				auto tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.bypassLine1Vol = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.bypassLine2Vol = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(";",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.bypassLine3Vol = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find("OUT");
				previousPos = Pos + 1;
				Pos = content.find("=",previousPos);
				previousPos = Pos + 1;
				Pos = content.find(",",previousPos);

				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.outputLine1Vol = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				self->m_UPSDataRS232.outputLine2Vol = std::stof(tmpStr);
				previousPos = Pos + 1;

				Pos = content.find(",",previousPos);
				self->m_UPSDataRS232.outputLine3Vol= std::stof(tmpStr);

				self->m_UPSDataRS232.outputLine1Power = self->m_UPSDataRS232.outputLine1Vol * self->m_UPSDataRS232.outputLine1Cur / 1000;
				self->m_UPSDataRS232.outputLine2Power = self->m_UPSDataRS232.outputLine2Vol * self->m_UPSDataRS232.outputLine2Cur / 1000;
				self->m_UPSDataRS232.outputLine3Power = self->m_UPSDataRS232.outputLine3Vol * self->m_UPSDataRS232.outputLine3Cur / 1000;
				self->m_UPSDataRS232.outputTotalPower = self->m_UPSDataRS232.outputLine1Power + self->m_UPSDataRS232.outputLine2Power + self->m_UPSDataRS232.outputLine3Power;
			}
			
			else if(content.find("Vbc") != std::string::npos)
			{
				auto Pos = content.find("Vbc");
				auto previousPos = Pos + 1;
				Pos = content.find("=",previousPos);
				previousPos = Pos + 1;
				Pos = content.find(";",previousPos);
				auto tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.batteryVol = std::stof(tmpStr);

				previousPos = Pos + 1;
				Pos = content.find("min",previousPos);
				tmpStr = content.substr(previousPos,Pos - previousPos);
				self->m_UPSDataRS232.estimatedMinutesOnBat = std::stof(tmpStr);

			}
			else{}
		}
		else if(GetPlug()->m_Radarcd.Find(wxT("Z9531")) != wxNOT_FOUND)  //Z9531济南站，Modbus通信协议设计说明书_Ver 1.34_C.pdf
		{
			const unsigned char *puD = (unsigned char *)pData;
			const int RegLen = 57;
			if((puD[0] == 0x01) && (puD[1] == 0x03) && puD[2]==(RegLen*2) && Len==(3+RegLen*2+2))
			{
				CRCData CRCCheckData;
				CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)puD,3+RegLen*2);
				if(CRCCheckData.crcByte[1]==pData[Len-2] && CRCCheckData.crcByte[0]==puD[Len-1])
				{	
					self->m_newDataFlag = TRUE;

					const int headLen = 3;
					int S = headLen;
					self->m_UPSDataRS232.bypassLine1Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine2Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine3Vol = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.bypassLine1Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine2Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine3Cur = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.bypassFre = (puD[S++] * 256 + puD[S++]) * 0.01;
					
					S=headLen+13*2;
					self->m_UPSDataRS232.inputLine1Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine2Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine3Vol = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.inputLine1Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine2Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine3Cur = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.inputLine1Fre = (puD[S++] * 256 + puD[S++]) * 0.01;
					self->m_UPSDataRS232.inputLine2Fre = (puD[S++] * 256 + puD[S++]) * 0.01;
					self->m_UPSDataRS232.inputLine3Fre = (puD[S++] * 256 + puD[S++]) * 0.01;

					S=headLen+25*2;
					self->m_UPSDataRS232.outputLine1Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					
					self->m_UPSDataRS232.outputLine1Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Cur = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.outputFre = (puD[S++] * 256 + puD[S++]) * 0.01;
					
					S=headLen+40*2;
					self->m_UPSDataRS232.outputLine1Power = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Power = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Power = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputTotalPower = self->m_UPSDataRS232.outputLine1Power + self->m_UPSDataRS232.outputLine2Power + self->m_UPSDataRS232.outputLine3Power;

					S=headLen+46*2;
					self->m_UPSDataRS232.outputLine1Load = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Load = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Load = (puD[S++] * 256 + puD[S++]) * 0.1;

					S=headLen+50*2;
					
					self->m_UPSDataRS232.batteryVol = (puD[S++] * 256 + puD[S++]) * 0.1;
					S=headLen+52*2;
					
					unsigned short temp = (puD[S++]<<8) | puD[S++];
					short stemp;memcpy(&stemp,&temp,sizeof(short));
					self->m_UPSDataRS232.batteryCurrent = stemp * 0.1;

					S=headLen+54*2;
					self->m_UPSDataRS232.batteryTemperature = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.estimatedMinutesOnBat = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.batteryCap = (puD[S++] * 256 + puD[S++]) * 0.1;
					
					/*self->m_UPSDataRS232.inputLine1Power = self->m_UPSDataRS232.inputLine1Vol * self->m_UPSDataRS232.inputLine1Cur / 1000;
					self->m_UPSDataRS232.inputLine2Power = self->m_UPSDataRS232.inputLine2Vol * self->m_UPSDataRS232.inputLine2Cur / 1000;
					self->m_UPSDataRS232.inputLine3Power = self->m_UPSDataRS232.inputLine3Vol * self->m_UPSDataRS232.inputLine3Cur / 1000;	
					self->m_UPSDataRS232.upsFaultId =  ((UCHAR)puD[81] * 256 + (UCHAR)puD[82]) * 0.1;*/

					
				}				
			}
		}
		else if((GetPlug()->m_Radarcd.Find(wxT("Z9230")) != wxNOT_FOUND))//Z9230重庆站
		{
			const unsigned char *puD = (unsigned char *)pData;
			const int RegLen = 31;
			if((puD[0] == 0x01) && (puD[1] == 0x03) && puD[2]==(RegLen*2) && Len==(3+RegLen*2+2))
			{
				CRCData CRCCheckData;
				CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)puD,3+RegLen*2);
				if(CRCCheckData.crcByte[1]==pData[Len-2] && CRCCheckData.crcByte[0]==puD[Len-1])
				{		
					const int headLen = 3;		
					int S = headLen,Base=170;
					self->m_UPSDataRS232.inputLine1Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine2Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine3Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine1Fre = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.outputLine1Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputFre = (puD[S++] * 256 + puD[S++]) * 0.01;

					self->m_UPSDataRS232.outputLine1Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Cur = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.outputLine1Load = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Load = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Load = (puD[S++] * 256 + puD[S++]) * 0.1;

					S=headLen+(188-Base)*2;
					self->m_UPSDataRS232.bypassLine1Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine2Vol = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine3Vol = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.bypassLine1Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine2Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.bypassLine3Cur = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.bypassFre = (puD[S++] * 256 + puD[S++]) * 0.01;
					
					S=headLen+(200-Base)*2;
					self->m_UPSDataRS232.batteryTemperature = (puD[S++] * 256 + puD[S++]) * 0.1;
						
					/*self->m_UPSDataRS232.inputLine1Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine2Cur = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.inputLine3Cur = (puD[S++] * 256 + puD[S++]) * 0.1;					
					self->m_UPSDataRS232.inputLine2Fre = (puD[S++] * 256 + puD[S++]) * 0.01;
					self->m_UPSDataRS232.inputLine3Fre = (puD[S++] * 256 + puD[S++]) * 0.01;			

					self->m_UPSDataRS232.outputLine1Power = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine2Power = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputLine3Power = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.outputTotalPower = self->m_UPSDataRS232.outputLine1Power + self->m_UPSDataRS232.outputLine2Power + self->m_UPSDataRS232.outputLine3Power;
					self->m_UPSDataRS232.batteryVol = (puD[S++] * 256 + puD[S++]) * 0.1;
					unsigned short temp = (puD[S++]<<8) | puD[S++];
					short stemp;memcpy(&stemp,&temp,sizeof(short));
					self->m_UPSDataRS232.batteryCurrent = stemp * 0.1;

					self->m_UPSDataRS232.secondOnBattery = (puD[S++] * 256 + puD[S++]) * 0.1;
					self->m_UPSDataRS232.batteryCap = (puD[S++] * 256 + puD[S++]) * 0.1;

					self->m_UPSDataRS232.inputLine1Power = self->m_UPSDataRS232.inputLine1Vol * self->m_UPSDataRS232.inputLine1Cur / 1000;
					self->m_UPSDataRS232.inputLine2Power = self->m_UPSDataRS232.inputLine2Vol * self->m_UPSDataRS232.inputLine2Cur / 1000;
					self->m_UPSDataRS232.inputLine3Power = self->m_UPSDataRS232.inputLine3Vol * self->m_UPSDataRS232.inputLine3Cur / 1000;	
					self->m_UPSDataRS232.upsFaultId =  ((UCHAR)puD[81] * 256 + (UCHAR)puD[82]) * 0.1;*/

					self->m_newDataFlag = TRUE;
				}				
			}

			
			//wxString strData(pData,wxConvLocal);
			//if(strData.Mid(0,5)==wxT("^D113"))//第一部分结果返回
			//{
			//	int start=8,wid=5;
			//	self->m_UPSDataRS232.inputLine1Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.inputLine2Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.inputLine3Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	wid = 4;
			//	self->m_UPSDataRS232.inputLine1Fre = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.inputLine2Fre = self->m_UPSDataRS232.inputLine1Fre;
			//	self->m_UPSDataRS232.inputLine3Fre = self->m_UPSDataRS232.inputLine1Fre;

			//	wid = 5;
			//	self->m_UPSDataRS232.outputLine1Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine2Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine3Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	wid = 4;
			//	self->m_UPSDataRS232.outputFre =wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//
			//	wid = 5;
			//	self->m_UPSDataRS232.outputLine1Cur = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine2Cur = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine3Cur = wxAtof(strData.Mid(start,wid));start=start+wid+1;

			//	wid = 3;
			//	self->m_UPSDataRS232.outputLine1Load = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine2Load = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine3Load = wxAtof(strData.Mid(start,wid));start=start+wid+1;

			//	start=start+wid+1;//跳过Total Load Percent
			//	wid = 5;
			//	self->m_UPSDataRS232.batteryVol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//
			//	start=start+wid+1;//跳过Batter voltae P
			//	wid = 4;
			//	self->m_UPSDataRS232.batteryTemperature = wxAtof(strData.Mid(start,wid));start=start+wid+1;

			//	start = start+4;
			//	wid = 1;
			//	self->m_UPSDataRS232.bypass = wxAtof(strData.Mid(start,wid));
			//					
			//	//发送第二部分查询命令
			//	char cmd[50]={0};
			//	sprintf(cmd,"^P005Q3LD");
			//	cmd[strlen(cmd)]=0xd;
			//	self->m_udpHandler->SendData(cmd,strlen(cmd),GetPlug()->m_UPSIPAdress.c_str(),23);
			//}
			//else if(strData.Mid(0,5)==wxT("^D010"))//第二部分结果返回
			//{				
			//	int start=6+6*15,wid=5;
			//	self->m_UPSDataRS232.outputLine1Power = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine2Power = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputLine3Power = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.outputTotalPower = self->m_UPSDataRS232.outputLine1Power + self->m_UPSDataRS232.outputLine2Power + self->m_UPSDataRS232.outputLine3Power;
			//	
			//	//发送第三部分查询命令
			//	char cmd[50]={0};
			//	sprintf(cmd,"^P004Q3Y");
			//	cmd[strlen(cmd)]=0xd;
			//	self->m_udpHandler->SendData(cmd,strlen(cmd),GetPlug()->m_UPSIPAdress.c_str(),23);
			//}
			//else if(strData.Mid(0,5)==wxT("^D043"))//第三部分结果返回
			//{
			//	int start=5,wid=5;
			//	self->m_UPSDataRS232.bypassLine1Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.bypassLine2Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.bypassLine3Vol = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.bypassLine1Cur = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.bypassLine2Cur = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.bypassLine3Cur = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	
			//	wid=4;
			//	self->m_UPSDataRS232.bypassFre = wxAtof(strData.Mid(start,wid));

			//	//发送第四部分查询命令
			//	char cmd[50]={0};
			//	sprintf(cmd,"^P004QBV");
			//	cmd[strlen(cmd)]=0xd;
			//	self->m_udpHandler->SendData(cmd,strlen(cmd),GetPlug()->m_UPSIPAdress.c_str(),23);
			//}
			//else if(strData.Mid(0,5)==wxT("^D049"))//第四部分结果返回
			//{
			//	self->m_UPSDataRS232.inputLine1Cur = 0;
			//	self->m_UPSDataRS232.inputLine2Cur = 0;
			//	self->m_UPSDataRS232.inputLine3Cur = 0;

			//	self->m_UPSDataRS232.inputLine1Power = 0;
			//	self->m_UPSDataRS232.inputLine2Power = 0;
			//	self->m_UPSDataRS232.inputLine3Power = 0;	
			//	
			//	int start=5+6*2,wid=5;
			//	self->m_UPSDataRS232.batteryCurrent = wxAtof(strData.Mid(start,wid));start=start+wid+1;

			//	start = start +6*3;
			//	self->m_UPSDataRS232.batteryCap = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.secondOnBattery = wxAtof(strData.Mid(start,wid));start=start+wid+1;
			//	self->m_UPSDataRS232.upsFaultId =  ;

			//		self->m_newDataFlag = TRUE;
			//}
		
			
		}
	}
	

		/*if(Len >= 103)
		{
		self->m_newDataFlag = TRUE;
		self->m_UPSDataRS232.batCondition = 0;
		self->m_UPSDataRS232.batStatus = 0;
		self->m_UPSDataRS232.batChargeStatus = (UCHAR)pData[47];
		self->m_UPSDataRS232.secondOnBattery = 0;
		self->m_UPSDataRS232.batteryVol = ((UCHAR)pData[44] * 256 + (UCHAR)pData[43]);

		if( (UCHAR)pData[47] == 0)
		{
		self->m_UPSDataRS232.batteryCurrent = (((UCHAR)pData[45] * 256 + (UCHAR)pData[46])) * 10.0;
		}
		else
		{
		self->m_UPSDataRS232.batteryCurrent = ((UCHAR)pData[45] * 256 + (UCHAR)pData[46]);
		}
		self->m_UPSDataRS232.batteryTemperature = 0;
		self->m_UPSDataRS232.inputLine1Fre = ((UCHAR)pData[42] * 256 + (UCHAR)pData[41]) / 10;
		self->m_UPSDataRS232.inputLine1Vol = (UCHAR)pData[35] * 230.0 / 100.0;
		self->m_UPSDataRS232.inputLine1Cur = (UCHAR)pData[38] * 80.0 / 100.0;
		self->m_UPSDataRS232.inputLine1Power = self->m_UPSDataRS232.inputLine1Vol * self->m_UPSDataRS232.inputLine1Cur ;
		self->m_UPSDataRS232.inputLine2Fre = ((UCHAR)pData[42] * 256 + (UCHAR)pData[41]) / 10;
		self->m_UPSDataRS232.inputLine2Vol = (UCHAR)pData[36] * 230.0 / 100.0;
		self->m_UPSDataRS232.inputLine2Cur = (UCHAR)pData[39] * 80.0 / 100.0;
		self->m_UPSDataRS232.inputLine2Power = self->m_UPSDataRS232.inputLine2Vol * self->m_UPSDataRS232.inputLine2Cur;
		self->m_UPSDataRS232.inputLine3Fre = ((UCHAR)pData[42] * 256 + (UCHAR)pData[41]) / 10;
		self->m_UPSDataRS232.inputLine3Vol = (UCHAR)pData[37] * 230.0 / 100.0;
		self->m_UPSDataRS232.inputLine3Cur = (UCHAR)pData[40] * 80.0 / 100.0;
		self->m_UPSDataRS232.inputLine3Power = self->m_UPSDataRS232.inputLine3Vol * self->m_UPSDataRS232.inputLine3Cur;
		self->m_UPSDataRS232.outputSource = 0;
		self->m_UPSDataRS232.outputFre = ((UCHAR)pData[69] * 256 + (UCHAR)pData[68]) / 10;
		self->m_UPSDataRS232.outputLines = 0;
		self->m_UPSDataRS232.outputLine1Vol = (UCHAR)pData[59];
		self->m_UPSDataRS232.outputLine1Cur = (UCHAR)pData[62] * 58.0 / 100.0;
		self->m_UPSDataRS232.outputLine1Power = self->m_UPSDataRS232.outputLine1Vol * self->m_UPSDataRS232.outputLine1Cur;
		self->m_UPSDataRS232.outputLine1Load = 0;
		self->m_UPSDataRS232.outputLine2Vol = (UCHAR)pData[60];
		self->m_UPSDataRS232.outputLine2Cur = (UCHAR)pData[63] * 58.0 / 100.0;
		self->m_UPSDataRS232.outputLine2Power = self->m_UPSDataRS232.outputLine2Vol * self->m_UPSDataRS232.outputLine2Cur;
		self->m_UPSDataRS232.outputLine2Load = 0;
		self->m_UPSDataRS232.outputLine3Vol = (UCHAR)pData[61];
		self->m_UPSDataRS232.outputLine3Cur = (UCHAR)pData[64] * 58.0 / 100.0;
		self->m_UPSDataRS232.outputLine3Power = self->m_UPSDataRS232.outputLine3Vol * self->m_UPSDataRS232.outputLine3Cur;
		self->m_UPSDataRS232.outputLine3Load = 0;
		self->m_UPSDataRS232.bypassFre = ((UCHAR)pData[58] * 256 + (UCHAR)pData[57]) / 10;
		self->m_UPSDataRS232.bypassLines = 0;
		self->m_UPSDataRS232.bypassLine1Vol = (UCHAR)pData[52] * 256 + (UCHAR)pData[51];
		self->m_UPSDataRS232.bypassLine1Cur = 0;
		self->m_UPSDataRS232.bypassLine1Power = 0;
		self->m_UPSDataRS232.bypassLine2Vol = (UCHAR)pData[54] * 256 + (UCHAR)pData[53];
		self->m_UPSDataRS232.bypassLine2Cur = 0;
		self->m_UPSDataRS232.bypassLine2Power = 0;
		self->m_UPSDataRS232.bypassLine3Vol = (UCHAR)pData[56] * 256 + (UCHAR)pData[55];
		self->m_UPSDataRS232.bypassLine3Cur = 0;
		self->m_UPSDataRS232.bypassLine3Power = 0;
		self->m_UPSDataRS232.outputTotalPower = self->m_UPSDataRS232.outputLine1Power + self->m_UPSDataRS232.outputLine2Power + self->m_UPSDataRS232.outputLine3Power;
		self->m_UPSDataRS232.batteryCap = ((UCHAR)pData[75] * 256 + (UCHAR)pData[74]) * 100.0 / 150.0;
		self->m_UPSDataRS232.upsFaultId = (UCHAR)pData[48];
		self->m_UPSDataRS232.upsShutdown = 0;
		self->m_UPSDataRS232.chargeFault = 0;
		self->m_UPSDataRS232.systemShutdown = 0;
		self->m_UPSDataRS232.autoRestart = 0;
		self->m_UPSDataRS232.shutdownDelay = 0;
		self->m_UPSDataRS232.shutdownAtonce = 0;
		self->m_UPSDataRS232.uspsCommunication = 0;
		self->m_UPSDataRS232.batteryFault =0 ;
		self->m_UPSDataRS232.batteryVolLow = 0;
		self->m_UPSDataRS232.bypass = 0;
		self->m_UPSDataRS232.otherFault = 0;
		self->m_UPSDataRS232.testInProgress = 0;
		}*/
		


	return 0;
}


void UPSHandlerRS232::processEvent(void *pHandle, int Opt, char *pData /*= nullptr*/, UINT Len /*= 0*/)
{

}

BOOL UPSHandlerRS232::startReceive() //打开网络端口
{
	m_udpHandler = GetPlug()->m_UDPFactory.GetInstance();

	if(m_udpHandler)
	{
		m_udpHandler->RegHandle(this, UPSHandlerRS232::processData,UPSHandlerRS232::processEvent);  //注册回调函数

		if (m_udpHandler->StartReceive(0, "0.0.0.0", 7500, nullptr, 0, 8192, 8192, 1000000))
		{
			GetPlug()->AddLog(LOG_TYPE_MESSAGE,"端口7500绑定成功");
		}
		else
		{
			GetPlug()->AddLog(LOG_TYPE_ERROR,"端口7500绑定失败");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL UPSHandlerRS232::stopReceive()
{
	if(m_udpHandler)
	{
		m_udpHandler->StopReceive();

		GetPlug()->m_UDPFactory.ReleaseInstance(m_udpHandler);
		m_udpHandler = NULL;

		return TRUE;
	}

	return FALSE;
}

BOOL UPSHandlerRS232::queryUPSData()
{
	if(m_udpHandler->IsWorking())
	{
		ZeroMemory((void*)&m_UPSDataRS232,sizeof(UPSDataRS232));

		if(GetPlug()->m_Radarcd.Find(wxT("Z9393")) != wxNOT_FOUND)  //根据不同站点发送不同UPS指令
		{
			char cmd[8] = {'\0'};
			cmd[0] = 0x01;
			cmd[1] = 0x04;
			cmd[2] = 0x75;
			cmd[3] = 0x30;
			cmd[4] = 0x00;
			cmd[5] = 0x29;

			CRCData CRCCheckData;
			CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)cmd,6);

			cmd[6] = CRCCheckData.crcByte[1];
			cmd[7] = CRCCheckData.crcByte[0];

			m_udpHandler->SendData(cmd,8,GetPlug()->m_UPSIPAdress.c_str(),23);

		}
		else if((GetPlug()->m_Radarcd.Find(wxT("Z9774")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9776")) != wxNOT_FOUND) || (GetPlug()->m_Radarcd.Find(wxT("Z9778")) != wxNOT_FOUND))
		{
			m_udpHandler->SendData("8",1,GetPlug()->m_UPSIPAdress.c_str(),23);
			::wxSleep(2);
			m_udpHandler->SendData("2",1,GetPlug()->m_UPSIPAdress.c_str(),23);
			::wxSleep(2);
			m_udpHandler->SendData("2",1,GetPlug()->m_UPSIPAdress.c_str(),23);
			::wxSleep(2);
			m_udpHandler->SendData("2",1,GetPlug()->m_UPSIPAdress.c_str(),23);
			::wxSleep(2);
			m_udpHandler->SendData("3",1,GetPlug()->m_UPSIPAdress.c_str(),23);
			::wxSleep(2);
			m_udpHandler->SendData("2",1,GetPlug()->m_UPSIPAdress.c_str(),23);
			::wxSleep(2);
		}
		else if(GetPlug()->m_Radarcd.Find(wxT("Z9531")) != wxNOT_FOUND)  //Z9531济南站 Modbus通信协议设计说明书_Ver 1.34_C.pdf
		{
			unsigned char cmd[8] = {'\0'};
			cmd[0] = 0x01;//设备地址(需要修改)
			cmd[1] = 0x03;//功能码，0x03遥测量获取
			cmd[2] = 0x00;
			cmd[3] = 0x00;//寄存器起始地址
			cmd[4] = 0x00;
			cmd[5] = 0x39;//寄存器个数 57
			CRCData CRCCheckData;
			CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)cmd,6);
			cmd[6] = CRCCheckData.crcByte[1];//0x85
			cmd[7] = CRCCheckData.crcByte[0];//0xD8
			////01 03 00 00 00 39 85 D8
			m_udpHandler->SendData((char*)cmd,8,GetPlug()->m_UPSIPAdress.c_str(),23);
		}
		else if((GetPlug()->m_Radarcd.Find(wxT("Z9230")) != wxNOT_FOUND))//Z9230重庆站 RT(R款)Modbus 通讯协议.pdf
		{
			return false;
			unsigned char cmd[8] = {'\0'};
			cmd[0] = 0x02;//设备地址(需要修改)
			cmd[1] = 0x03;//功能码，0x03
			cmd[2] = 0x00;
			cmd[3] = 0xAA;//寄存器起始地址
			cmd[4] = 0;
			cmd[5] = 31;//寄存器个数

			CRCData CRCCheckData;
			CRCCheckData.crcData = GetPlug()->chkcrc((unsigned char*)cmd,6);

			cmd[6] = CRCCheckData.crcByte[1];
			cmd[7] = CRCCheckData.crcByte[0];
			m_udpHandler->SendData((char*)cmd,8,GetPlug()->m_UPSIPAdress.c_str(),23);
		}
		else{}

		//return m_udpHandler->SendData("\xC0",1,GetPlug()->m_UPSIPAdress.c_str(),23);
	}
	return false;
}



bool UPSHandlerRS232::writeToDataBase()
{
	wxString sqlSentence = wxT("INSERT INTO upsinfo (updateTime,\
														estimatedMinutesOnBat, \
														batteryVol, \
														batteryCurrent, \
														inputLine1Fre, \
														inputLine1Vol, \
														inputLine1Cur, \
														inputLine1Power, \
														inputLine2Fre, \
														inputLine2Vol, \
														inputLine2Cur, \
														inputLine2Power, \
														inputLine3Fre, \
														inputLine3Vol, \
														inputLine3Cur, \
														inputLine3Power, \
														outputFre, \
														outputLine1Vol, \
														outputLine1Cur, \
														outputLine1Power, \
														outputLine1Load, \
														outputLine2Vol, \
														outputLine2Cur, \
														outputLine2Power, \
														outputLine2Load, \
														outputLine3Vol, \
														outputLine3Cur, \
														outputLine3Power, \
														outputLine3Load, \
														bypassFre, \
														bypassLine1Vol, \
														bypassLine2Vol, \
														bypassLine3Vol, \
														outputTotalPower, \
														batteryCap, \
														upsFaultId) VALUES");

	auto pDataBase = GetPlug()->getDatabase();
	if(pDataBase)
	{
		
		sqlSentence.Append(wxString::Format(wxT("(%I64d, "),wxDateTime::GetTimeNow()));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.estimatedMinutesOnBat));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.batteryVol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.batteryCurrent));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine1Fre));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine1Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine1Cur));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine1Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine2Fre));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine2Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine2Cur));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine2Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine3Fre));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine3Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine3Cur));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.inputLine3Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputFre));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine1Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine1Cur));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine1Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine1Load));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine2Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine2Cur));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine2Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine2Load));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine3Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine3Cur));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine3Power));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputLine3Load));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.bypassFre));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.bypassLine1Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.bypassLine2Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.bypassLine3Vol));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.outputTotalPower));
		sqlSentence.Append(wxString::Format(wxT("%f, "), m_UPSDataRS232.batteryCap));
		sqlSentence.Append(wxString::Format(wxT("%f)"), m_UPSDataRS232.upsFaultId));


		bool ret = pDataBase->RunSQLNoQuery(sqlSentence);
		if(!ret)
		{
			SHOWFUNCNAME();
		}
	}

	return true;

}
