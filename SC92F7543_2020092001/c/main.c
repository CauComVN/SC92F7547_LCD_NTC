#include "H/Function_Init.H"
#include "H/UserHeader.h"
#include "H/Key.h"
#include "H/Signal.h"
#include "H/LCD.h"
#include "H/NTC.h"

MODETYPE eModeType;
HOLDTYPE eHoldType;
TEMPERATURE eTempType;
BATLEVEL eBatLevel;

unsigned short BatAdcBuf[BAT_ADC_BUF];

unsigned short usNtcTime;
unsigned short usBatTime;
unsigned short usSignalTime;
unsigned short usNtcDisTime;
/************************Power Off Region Start***************************/
#if 1
#if ENABLE_SOFT_POWER
void PowerOn(void)
{
	//P1.3 输出
	P1CON |= 0x08;
	P13 = 1;
}
void PowerOff(void)
{
	//P1.3 输出
	P1CON |= 0x08;
	P13 = 0;
}
#endif
#endif
/************************Power Off Region End***************************/

/************************Power Off Region Start***************************/
#if 1
#if ENABLE_SOFT_BACKLIGHT
void BackLightOn(void)
{
	//P1.1 输出H
	P1CON |= 0x02;
	P11 = 1;
}
void BackLightOff(void)
{
	//P1.3 输出L
	P1CON |= 0x02;
	P11 = 0;
}
#endif
#endif
/************************Power Off Region End***************************/

/************************Timer 1ms Region Start***************************/
unsigned char isTimer0_1ms = 0;
#if 1
void Timer0_Init(void)
{ 
	TMCON = 0X01;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	
	//T0设置
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
	TL0 = (65536 - TIMER_TICK_1MS)%256;    //溢出时间：时钟为Fsys，则16000*（1/Fsys）=1ms;
	TH0 = (65536 - TIMER_TICK_1MS)/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
}

void timer0() interrupt 1
{
    TL0 = (65536 - TIMER_TICK_1MS)%256;
	TH0 = (65536 - TIMER_TICK_1MS)/256;
	isTimer0_1ms = 1;
	
	usBatTime++;
	usSignalTime++;
	usNtcTime++;
	usNtcDisTime++;
}
#endif
/************************Timer 1ms Region End***************************/
/************************BAT Region End***************************/
#if 1
unsigned char AdcFlag = 0;
void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X20);  //清中断标志位
	AdcFlag = 1;
}

unsigned short xGetAdc(unsigned char Channel)
{
	unsigned short ADCValue;
	ADCCON = 0X80|Channel;		//开启ADC，ADC采样频率为2M ,选择Channel位ADC采样口
	if(Channel<8)
	{
		ADCCFG0 = 1<<Channel;   //设置Channel作为采样口
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //设置Channel作为采样口
	}
	IE |= 0X40;        //开启ADC中断
	ADCCFG2 = 0x04 | 0x02;//adc 时钟=Fhrc/12,tick = 36+14 = 50,16000000/12/50 = 
	ADCCON |= 0X40;   //开始ADC转换
	while(!AdcFlag);	         		 //等待 ADC转换完成;
	AdcFlag = 0;
	ADCValue = (ADCVH<<4)+(ADCVL>>4);
	return ADCValue;
}

unsigned char BatAdcCnt;
void BatProcess(void)
{
	static unsigned char isInitAdc = 0;
	unsigned short ADCValue;
	unsigned char i,j;
	BATLEVEL level;
	if (isInitAdc == 0)
	{
		for(BatAdcCnt = 0;BatAdcCnt < BAT_ADC_BUF;BatAdcCnt++)
			BatAdcBuf[BatAdcCnt] = xGetAdc(14);
		isInitAdc = 1;
	}
	if (BatAdcCnt == BAT_ADC_BUF)
	{
		BatAdcCnt = 0;
		for (j = 0; j < BAT_ADC_BUF; j++)
		{
			for (i = 0; i < BAT_ADC_BUF - 1 - j; i++)
			{
				if (BatAdcBuf[i] < BatAdcBuf[i + 1])
				{
					ADCValue = BatAdcBuf[i]; 
					BatAdcBuf[i] = BatAdcBuf[i + 1]; 
					BatAdcBuf[i + 1] = ADCValue;
				}
			}
		}
		ADCValue = 0;
		for (i = 2; i < 6; i++)
		{
			ADCValue+=BatAdcBuf[i]>>2;
		}
		if (ADCValue > BAT_LEVEL2_VALUE)
		{
			level = eBAT2;
		}
		else if (ADCValue > BAT_LEVEL1_VALUE)
		{
			level = eBAT1;
		}
		else
		{
			level = eBAT0;
		}
		if (eBatLevel != level)
		{
			eBatLevel = level;
			LCD_Bat((unsigned char)eBatLevel);
		}
	}
	else
	{
		BatAdcBuf[BatAdcCnt] = xGetAdc(14);
	}
}
#endif
/************************BAT Region End***************************/
/************************Signal Region End***************************/
#if 1
void SignalProcess(void)
{
	unsigned short ADCValue;
	if (eHoldType == eHOLD_ENABLE)
		return;
	ADCValue = xGetAdc(15);
	LCD_Signal(Measure_Process(eModeType,ADCValue));
}
#endif
/************************Signal Region End***************************/
/************************NTC Region End***************************/
#if 1
void NtcProcess(void)
{
	unsigned short ADCValue;
	short ntc;
	ADCValue = xGetAdc(13);
	
	ntc = xGetTempCbyNtc(ADCValue);
	if (usNtcDisTime >= NTC_DISPLAY_TIME)
	{
		usNtcDisTime -= NTC_DISPLAY_TIME;
		if(eTempType == eTEMP_F)
		{
			ntc=(ntc+320)*9/5;
		}
		LCD_NTC(ntc);
	}
}
#endif
/************************NTC Region End***************************/



void main(void)
{
	unsigned short Key;
	IO_Init();
	WDTCON |= 0x10;		    //清看门狗
	OPERCON = 0X01;       //CHKSUM运算
	_nop_();
	LCD_Init();
//	LED_Init();
	
	#if ENABLE_SOFT_POWER
	PowerOn();
	#endif
	#if ENABLE_SOFT_BACKLIGHT
	BackLightOn();
	#endif
	//1ms Timer
	Timer0_Init();
	KeyInit();
	
	EA = 1;
	
	eModeType = eMODEA;
	eHoldType = eHOLD_DISABLE;
	eTempType = eTEMP_C;
	LCD_Temperature(eTempType);
		
	LCD_Temperature(eTempType);
	LCD_Mode(eModeType);
	
	Lcd_Hold(eHoldType);
	BatProcess();
	while(1)
	{
		if (isTimer0_1ms)
		{
			isTimer0_1ms = 0;
			//Key Scan
			Key = KeyScanProcess();
			if (Key == 0x01)//P05,HOLD
			{
				if (eHoldType == eHOLD_DISABLE)
				{
					eHoldType = eHOLD_ENABLE;
				}
				else
				{
					eHoldType = eHOLD_DISABLE;
				}
				Lcd_Hold(eHoldType);
			}
			else if (Key == 0x02)//P06, back light
			{
				#if ENABLE_SOFT_BACKLIGHT
				if (P11 == 0)
					BackLightOn();
				else
					BackLightOff();
				#endif
			}
			else if (Key == 0x04)//P07,power
			{
			}
			else if (Key == 0x4001)//P05,MODE
			{
				if (eModeType == eMODEA)
				{
					eModeType = eMODEB;
				}
				else if (eModeType == eMODEB)
				{
					eModeType = eMODEC;
				}
				else if (eModeType == eMODEC)
				{
					eModeType = eMODED;
				}
				else if (eModeType == eMODED)
				{
					eModeType = eMODEA;
				}
				LCD_Mode(eModeType);
			}
			else if (Key == 0x4002)//P06 C/F
			{
				if (eTempType == eTEMP_C)
				{
					eTempType = eTEMP_F;
				}
				else
				{
					eTempType = eTEMP_C;
				}
				LCD_Temperature(eTempType);
			}
			else if (Key == 0x4004)//P07,power
			{
				#if ENABLE_SOFT_POWER
				PowerOff();
				EA = 0;
				while(1);
				#endif
			}
			
			//Bat process
			if (usBatTime >= BAT_CHECK_TIME)
			{
				usBatTime -= BAT_CHECK_TIME;
				BatProcess();
			}
			
			//Signal
			if (usSignalTime >= SIGNAL_CHECK_TIME)
			{
				usSignalTime -= SIGNAL_CHECK_TIME;
				SignalProcess();
			}
			
			//NTC
			if (usNtcTime >= NTC_CHECK_TIME)
			{
				usNtcTime -= NTC_CHECK_TIME;
				
				if (eHoldType == eHOLD_ENABLE)
					NtcProcess();
			}
		}
		LCD_Reflash();
	}
}