#include "H/LCD.h"
unsigned char isReflash = 0;
/************************LCD Region End***************************/
#if 1
/*
LCD 说明:
1、S1：S1显示时，表示负数
2、OBC,1,2,S2,3，4：表示温度摄氏度或华氏度，S2表示小数点，4表示单位C/F
3、5，6，S3，7：表示signal
4、8表示HOLD：H是HOLD
5、9表示MODE：A，B，C，D
*/
unsigned char xdata LCDRAM[LCD_BUF_LEN]  _at_  0x700;
unsigned char xdata LCDTemp[LCD_BUF_LEN];

void LCD_Init(void)
{
	char i;
	DDRCON = 0xB0;    //-xyy zzzz  x:LCD/LED模式 y:占空比  z:LCD电压调节
	P0VO = 0xFC;
	P1VO = 0x04;
	P2VO = 0x3F;
	P3VO = 0xFF;
	OTCON = 0x04;    //---- xxyz  x:LCD分压电阻 y:复位管脚选择 z:LCD偏置电压
	for(i=0;i<LCD_BUF_LEN;i++)
	{
		LCDTemp[i] = 0x00;
	}
}
void LED_Init(void)
{
	unsigned int i;
	DDRCON = 0xF0;    //-xyy zzzz  x:LCD/LED模式 y:占空比1/8:00 1/6:01 1/5:10 1/4:11  z:LCD电压调节
	
	P0VO = 0x1C;
	P1VO = 0x04;
	P2VO = 0x3F;
	P3VO = 0xFF;
	
	OTCON = 0x04;    //---- xxyz  x:LCD分压电阻 y:复位管脚选择 z:LCD偏置电压

	for(i=0;i<LCD_BUF_LEN;i++)
	{
		LCDTemp[i] = 0x00;
	}	
}

#endif
/************************LCD Region End***************************/


#if 1	//Tmp unit
code unsigned char Temp_C[1]=
{
	190
};
code unsigned char Temp_F[1]=
{
	0
};
code unsigned char Temp_CF_8[1]=
{
	190
};
void LCD_Temperature(TEMPERATURE eTemp)
{	
	char i;
	short bits;
	unsigned char * pData = 0;
	for(i=0;i<sizeof(Temp_CF_8);i++)
	{
		bits = Temp_CF_8[i];
		LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
	}
	i = 0;
	if (eTemp == eTEMP_C)
	{		
		pData = Temp_C;
		i = sizeof(Temp_C);
	}
//	else
//	{
//		pData = Temp_F;
//		i = sizeof(Temp_C);
//	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
	isReflash = 1;
}
#endif
#if 1	//MODE
code unsigned char Mode_A[1]=
{
	196,
};

code unsigned char Mode_B[1]=
{
	197,
};
code unsigned char Mode_C[1]=
{
	198,
};
code unsigned char Mode_D[1]=
{
	199,
};
code unsigned char Mode_8[4]=
{
	196,197,198,199
};
void LCD_Mode(MODETYPE eMode)
{	
	char i;
	short bits;
	unsigned char * pData = 0;
	
	for(i=0;i<sizeof(Mode_8);i++)
	{
		bits = Mode_8[i];
		LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
	}
	if (eMode == eMODEA)
	{
		pData = Mode_A;
		i = sizeof(Mode_A);
	}
	else if (eMode == eMODEB)
	{
		pData = Mode_B;
		i = sizeof(Mode_B);
	}
	else if (eMode == eMODEC)
	{
		pData = Mode_C;
		i = sizeof(Mode_C);
	}
	else if (eMode == eMODED)
	{
		pData = Mode_D;
		i = sizeof(Mode_D);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
	isReflash = 1;
}
#endif
#if 1	//HOLD
code unsigned char Hold[1]=
{
	188,
};
void Lcd_Hold(HOLDTYPE eHold)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	
	if (eHold == eHOLD_DISABLE)
	{
		for(i=0;i<sizeof(Hold);i++)
		{
			bits = Hold[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
	}
	else
	{
		pData = Hold;
		i = sizeof(Hold);
		for(i = i -1;i>=0;i--)
		{
			bits = pData[i];
			LCDTemp[bits>>3] |= 1<<(bits&0x07);
		}
	}
	isReflash = 1;
}
#endif


#if 1	//BAT
code unsigned char Bat_1_Bits[1]=
{
	191,
};
code unsigned char Bat_2_Bits[2]=
{
	191,189
};
void LCD_Bat(unsigned char level)
{	
	char i;
	short bits;
	unsigned char * pData = 0;
	
	for(i=0;i<sizeof(Bat_2_Bits);i++)
	{
		bits = Bat_2_Bits[i];
		LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
	}
	i = 0;
	if (level == 1)
	{
		pData = Bat_1_Bits;
		i = sizeof(Bat_1_Bits);
	}
	else if (level == 2)
	{
		pData = Bat_2_Bits;
		i = sizeof(Bat_2_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
	isReflash = 1;
}
#endif
#if 1	//Signal
code unsigned char Signal_10_0_Bits[6]=
{
	109,111,116,117,118,119
};
code unsigned char Signal_10_1_Bits[2]=
{
	117,118
};
code unsigned char Signal_10_2_Bits[5]=
{
	110,111,116,117,119
};
code unsigned char Signal_10_3_Bits[5]=
{
	110,116,117,118,119
};
code unsigned char Signal_10_4_Bits[4]=
{
	109,110,117,118
};
code unsigned char Signal_10_5_Bits[5]=
{
	109,110,116,118,119
};
code unsigned char Signal_10_6_Bits[6]=
{
	109,110,111,116,118,119
};
code unsigned char Signal_10_7_Bits[3]=
{
	116,117,118
};
code unsigned char Signal_10_8_Bits[7]=
{
	109,110,111,116,117,118,119
};
code unsigned char Signal_10_9_Bits[6]=
{
	109,110,116,117,118,119
};


code unsigned char Signal_1_0_Bits[6]=
{
	125,127,132,133,134,135
};
code unsigned char Signal_1_1_Bits[2]=
{
	133,134
};
code unsigned char Signal_1_2_Bits[5]=
{
	126,127,132,133,135
};
code unsigned char Signal_1_3_Bits[5]=
{
	126,132,133,134,135
};
code unsigned char Signal_1_4_Bits[4]=
{
	125,126,133,134
};
code unsigned char Signal_1_5_Bits[5]=
{
	125,126,132,134,135
};
code unsigned char Signal_1_6_Bits[6]=
{
	125,126,127,132,134,135
};
code unsigned char Signal_1_7_Bits[3]=
{
	132,133,134
};
code unsigned char Signal_1_8_Bits[7]=
{
	125,126,127,132,133,134,135
};
code unsigned char Signal_1_9_Bits[6]=
{
	125,126,132,133,134,135
};


code unsigned char Signal_0_0_Bits[7]=
{
	140,141,143,180,181,182,183
};
code unsigned char Signal_0_1_Bits[3]=
{
	140,181,182
};
code unsigned char Signal_0_2_Bits[6]=
{
	140,142,143,180,181,183
};
code unsigned char Signal_0_3_Bits[6]=
{
	140,142,180,181,182,183
};
code unsigned char Signal_0_4_Bits[5]=
{
	140,141,142,181,182
};
code unsigned char Signal_0_5_Bits[6]=
{
	140,141,142,180,182,183
};
code unsigned char Signal_0_6_Bits[7]=
{
	140,141,142,143,180,182,183
};
code unsigned char Signal_0_7_Bits[4]=
{
	140,180,181,182
};
code unsigned char Signal_0_8_Bits[8]=
{
	140,141,142,143,180,181,182,183
};
code unsigned char Signal_0_9_Bits[7]=
{
	140,141,142,180,181,182,183
};
void LCD_Signal_10(unsigned short usSignal)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	if (usSignal >= 999)
		bits = 999;
	else
		bits = usSignal;
	bits =  (bits/100);
	i = 0;
	if (bits == 0)
	{
//		pData = Signal_10_0_Bits;
//		i = sizeof(Signal_10_0_Bits);
	}
	else if (bits == 1)
	{
		pData = Signal_10_1_Bits;
		i = sizeof(Signal_10_1_Bits);
	}
	else if (bits == 2)
	{
		pData = Signal_10_2_Bits;
		i = sizeof(Signal_10_2_Bits);
	}
	else if (bits == 3)
	{
		pData = Signal_10_3_Bits;
		i = sizeof(Signal_10_3_Bits);
	}
	else if (bits == 4)
	{
		pData = Signal_10_4_Bits;
		i = sizeof(Signal_10_4_Bits);
	}
	else if (bits == 5)
	{
		pData = Signal_10_5_Bits;
		i = sizeof(Signal_10_5_Bits);
	}
	else if (bits == 6)
	{
		pData = Signal_10_6_Bits;
		i = sizeof(Signal_10_6_Bits);
	}
	else if (bits == 7)
	{
		pData = Signal_10_7_Bits;
		i = sizeof(Signal_10_7_Bits);
	}
	else if (bits == 8)
	{
		pData = Signal_10_8_Bits;
		i = sizeof(Signal_10_8_Bits);
	}
	else if (bits == 9)
	{
		pData = Signal_10_9_Bits;
		i = sizeof(Signal_10_9_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
}
void LCD_Signal_1(unsigned short usSignal)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	
	usSignal %= 100;
	bits = (usSignal / 10);
	if (bits == 0)
	{
		pData = Signal_1_0_Bits;
		i = sizeof(Signal_1_0_Bits);
	}
	else if (bits == 1)
	{
		pData = Signal_1_1_Bits;
		i = sizeof(Signal_1_1_Bits);
	}
	else if (bits == 2)
	{
		pData = Signal_1_2_Bits;
		i = sizeof(Signal_1_2_Bits);
	}
	else if (bits == 3)
	{
		pData = Signal_1_3_Bits;
		i = sizeof(Signal_1_3_Bits);
	}
	else if (bits == 4)
	{
		pData = Signal_1_4_Bits;
		i = sizeof(Signal_1_4_Bits);
	}
	else if (bits == 5)
	{
		pData = Signal_1_5_Bits;
		i = sizeof(Signal_1_5_Bits);
	}
	else if (bits == 6)
	{
		pData = Signal_1_6_Bits;
		i = sizeof(Signal_1_6_Bits);
	}
	else if (bits == 7)
	{
		pData = Signal_1_7_Bits;
		i = sizeof(Signal_1_7_Bits);
	}
	else if (bits == 8)
	{
		pData = Signal_1_8_Bits;
		i = sizeof(Signal_1_8_Bits);
	}
	else if (bits == 9)
	{
		pData = Signal_1_9_Bits;
		i = sizeof(Signal_1_9_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
}
void LCD_Signal_0(unsigned short usSignal)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	
	bits = usSignal;
	if (bits == 0)
	{
		pData = Signal_0_0_Bits;
		i = sizeof(Signal_0_0_Bits);
	}
	else if (bits == 1)
	{
		pData = Signal_0_1_Bits;
		i = sizeof(Signal_0_1_Bits);
	}
	else if (bits == 2)
	{
		pData = Signal_0_2_Bits;
		i = sizeof(Signal_0_2_Bits);
	}
	else if (bits == 3)
	{
		pData = Signal_0_3_Bits;
		i = sizeof(Signal_0_3_Bits);
	}
	else if (bits == 4)
	{
		pData = Signal_0_4_Bits;
		i = sizeof(Signal_0_4_Bits);
	}
	else if (bits == 5)
	{
		pData = Signal_0_5_Bits;
		i = sizeof(Signal_0_5_Bits);
	}
	else if (bits == 6)
	{
		pData = Signal_0_6_Bits;
		i = sizeof(Signal_0_6_Bits);
	}
	else if (bits == 7)
	{
		pData = Signal_0_7_Bits;
		i = sizeof(Signal_0_7_Bits);
	}
	else if (bits == 8)
	{
		pData = Signal_0_8_Bits;
		i = sizeof(Signal_0_8_Bits);
	}
	else if (bits == 9)
	{
		pData = Signal_0_9_Bits;
		i = sizeof(Signal_0_9_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
}

void LCD_Signal(unsigned short signal)
{
	char i;
	short bits;
	unsigned char * pData = 0;
		
	{
		//清除十分位，个位，十位
		for(i=0;i<sizeof(Signal_10_8_Bits);i++)
		{
			bits = Signal_10_8_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
		for(i=0;i<sizeof(Signal_1_8_Bits);i++)
		{
			bits = Signal_1_8_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
		for(i=0;i<sizeof(Signal_0_8_Bits);i++)
		{
			bits = Signal_0_8_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
	}
	//十位
	LCD_Signal_10(signal);
	if (signal >= 100)
		signal -= 100;
	//个位
	LCD_Signal_1(signal);
	//十分位
	signal = signal % 10;
	LCD_Signal_0(signal);
	
	isReflash = 1;
}
#endif
#if 1	//NTC
#if 1	//Temp Num
code unsigned char Temp_100_Bits[1]=
{
	4,
};
code unsigned char Temp_10_0_Bits[6]=
{
	5,7,12,13,14,15
};
code unsigned char Temp_10_1_Bits[2]=
{
	13,14
};
code unsigned char Temp_10_2_Bits[5]=
{
	6,7,12,13,15
};
code unsigned char Temp_10_3_Bits[5]=
{
	6,12,13,14,15
};
code unsigned char Temp_10_4_Bits[4]=
{
	5,6,13,14
};
code unsigned char Temp_10_5_Bits[5]=
{
	5,6,12,14,15
};
code unsigned char Temp_10_6_Bits[6]=
{
	5,6,7,12,14,15
};
code unsigned char Temp_10_7_Bits[3]=
{
	12,13,14
};
code unsigned char Temp_10_8_Bits[7]=
{
	5,6,7,12,13,14,15
};
code unsigned char Temp_10_9_Bits[6]=
{
	5,6,12,13,14,15
};


code unsigned char Temp_1_0_Bits[6]=
{
	21,23,28,29,30,31
};
code unsigned char Temp_1_1_Bits[2]=
{
	29,30
};
code unsigned char Temp_1_2_Bits[5]=
{
	22,23,28,29,31
};
code unsigned char Temp_1_3_Bits[5]=
{
	22,28,29,30,31
};
code unsigned char Temp_1_4_Bits[4]=
{
	21,22,29,30
};
code unsigned char Temp_1_5_Bits[5]=
{
	21,22,28,30,31
};
code unsigned char Temp_1_6_Bits[6]=
{
	21,22,23,28,30,31
};
code unsigned char Temp_1_7_Bits[3]=
{
	28,29,30
};
code unsigned char Temp_1_8_Bits[7]=
{
	21,22,23,28,29,30,31
};
code unsigned char Temp_1_9_Bits[6]=
{
	21,22,28,29,30,31
};


code unsigned char Temp_0_0_Bits[7]=
{
	52,53,55,100,101,102,103
};
code unsigned char Temp_0_1_Bits[3]=
{
	52,101,102
};
code unsigned char Temp_0_2_Bits[6]=
{
	52,54,55,100,101,103
};
code unsigned char Temp_0_3_Bits[6]=
{
	52,54,100,101,102,103
};
code unsigned char Temp_0_4_Bits[5]=
{
	52,53,54,101,102
};
code unsigned char Temp_0_5_Bits[6]=
{
	52,53,54,100,102,103
};
code unsigned char Temp_0_6_Bits[7]=
{
	52,53,54,55,100,102,103
};
code unsigned char Temp_0_7_Bits[4]=
{
	52,100,101,102
};
code unsigned char Temp_0_8_Bits[8]=
{
	52,53,54,55,100,101,102,103
};
code unsigned char Temp_0_9_Bits[7]=
{
	52,53,54,100,101,102,103
};
#endif

void LCD_NTC_100(unsigned short usNtc)
{
	char i;
	short bits;
	
	if (usNtc >= 1000)	//显示百位
	{
		for(i=0;i<sizeof(Temp_100_Bits);i++)
		{
			bits = Temp_100_Bits[i];
			LCDTemp[bits>>3] |= 1<<(bits&0x07);
		}
	}
	else	//清除百位
	{
		for(i=0;i<sizeof(Temp_100_Bits);i++)
		{
			bits = Temp_100_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
	}

}
void LCD_NTC_10(unsigned short usNtc)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	if (usNtc >= 1000)
		bits = usNtc - 1000;
	else
		bits = usNtc;
	bits =  (bits/100);
	i = 0;
	if ((bits == 0) && (usNtc >= 1000))
	{
		pData = Temp_10_0_Bits;
		i = sizeof(Temp_10_0_Bits);
	}
	else if (bits == 1)
	{
		pData = Temp_10_1_Bits;
		i = sizeof(Temp_10_1_Bits);
	}
	else if (bits == 2)
	{
		pData = Temp_10_2_Bits;
		i = sizeof(Temp_10_2_Bits);
	}
	else if (bits == 3)
	{
		pData = Temp_10_3_Bits;
		i = sizeof(Temp_10_3_Bits);
	}
	else if (bits == 4)
	{
		pData = Temp_10_4_Bits;
		i = sizeof(Temp_10_4_Bits);
	}
	else if (bits == 5)
	{
		pData = Temp_10_5_Bits;
		i = sizeof(Temp_10_5_Bits);
	}
	else if (bits == 6)
	{
		pData = Temp_10_6_Bits;
		i = sizeof(Temp_10_6_Bits);
	}
	else if (bits == 7)
	{
		pData = Temp_10_7_Bits;
		i = sizeof(Temp_10_7_Bits);
	}
	else if (bits == 8)
	{
		pData = Temp_10_8_Bits;
		i = sizeof(Temp_10_8_Bits);
	}
	else if (bits == 9)
	{
		pData = Temp_10_9_Bits;
		i = sizeof(Temp_10_9_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
}
void LCD_NTC_1(unsigned short usNtc)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	
	usNtc %= 100;
	bits = (usNtc / 10);
	if (bits == 0)
	{
		pData = Temp_1_0_Bits;
		i = sizeof(Temp_1_0_Bits);
	}
	else if (bits == 1)
	{
		pData = Temp_1_1_Bits;
		i = sizeof(Temp_1_1_Bits);
	}
	else if (bits == 2)
	{
		pData = Temp_1_2_Bits;
		i = sizeof(Temp_1_2_Bits);
	}
	else if (bits == 3)
	{
		pData = Temp_1_3_Bits;
		i = sizeof(Temp_1_3_Bits);
	}
	else if (bits == 4)
	{
		pData = Temp_1_4_Bits;
		i = sizeof(Temp_1_4_Bits);
	}
	else if (bits == 5)
	{
		pData = Temp_1_5_Bits;
		i = sizeof(Temp_1_5_Bits);
	}
	else if (bits == 6)
	{
		pData = Temp_1_6_Bits;
		i = sizeof(Temp_1_6_Bits);
	}
	else if (bits == 7)
	{
		pData = Temp_1_7_Bits;
		i = sizeof(Temp_1_7_Bits);
	}
	else if (bits == 8)
	{
		pData = Temp_1_8_Bits;
		i = sizeof(Temp_1_8_Bits);
	}
	else if (bits == 9)
	{
		pData = Temp_1_9_Bits;
		i = sizeof(Temp_1_9_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
}
void LCD_NTC_0(unsigned short usNtc)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	
	bits = usNtc;
	if (bits == 0)
	{
		pData = Temp_0_0_Bits;
		i = sizeof(Temp_0_0_Bits);
	}
	else if (bits == 1)
	{
		pData = Temp_0_1_Bits;
		i = sizeof(Temp_0_1_Bits);
	}
	else if (bits == 2)
	{
		pData = Temp_0_2_Bits;
		i = sizeof(Temp_0_2_Bits);
	}
	else if (bits == 3)
	{
		pData = Temp_0_3_Bits;
		i = sizeof(Temp_0_3_Bits);
	}
	else if (bits == 4)
	{
		pData = Temp_0_4_Bits;
		i = sizeof(Temp_0_4_Bits);
	}
	else if (bits == 5)
	{
		pData = Temp_0_5_Bits;
		i = sizeof(Temp_0_5_Bits);
	}
	else if (bits == 6)
	{
		pData = Temp_0_6_Bits;
		i = sizeof(Temp_0_6_Bits);
	}
	else if (bits == 7)
	{
		pData = Temp_0_7_Bits;
		i = sizeof(Temp_0_7_Bits);
	}
	else if (bits == 8)
	{
		pData = Temp_0_8_Bits;
		i = sizeof(Temp_0_8_Bits);
	}
	else if (bits == 9)
	{
		pData = Temp_0_9_Bits;
		i = sizeof(Temp_0_9_Bits);
	}
	for(i = i -1;i>=0;i--)
	{
		bits = pData[i];
		LCDTemp[bits>>3] |= 1<<(bits&0x07);
	}
}
void LCD_NTC(short usNtc)
{
	char i;
	short bits;
	unsigned char * pData = 0;
	
	if (usNtc < 0)
	{
		usNtc = -usNtc;
		//设置负号
		LCDTemp[20>>3] |= 1<<(20&0x07);
	}
	else
	{
		//清除负号
		LCDTemp[20>>3] &= ~(1<<(20&0x07));
	}
	LCD_NTC_100(usNtc);
	
	{
		//清除十分位，个位，十位
		for(i=0;i<sizeof(Temp_10_8_Bits);i++)
		{
			bits = Temp_10_8_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
		for(i=0;i<sizeof(Temp_1_8_Bits);i++)
		{
			bits = Temp_1_8_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
		for(i=0;i<sizeof(Temp_0_8_Bits);i++)
		{
			bits = Temp_0_8_Bits[i];
			LCDTemp[bits>>3] &= ~(1<<(bits&0x07));
		}
	}
	//十位
	LCD_NTC_10(usNtc);
	if (usNtc >= 1000)
		usNtc -= 1000;
	//个位
	LCD_NTC_1(usNtc);
	//十分位
	usNtc = usNtc % 10;
	LCD_NTC_0(usNtc);
	
	isReflash = 1;
}

#endif
void LCD_Reflash(void)
{
	unsigned char i;
	if (isReflash)
	{
		isReflash = 0;
		for(i=0;i<LCD_BUF_LEN;i++)
		{
			LCDRAM[i] = LCDTemp[i];
		}
	}
}
//LED 1,0,1
//LED 2,2,3
//LED 3,7,12
//LED 4,13,14
//LED 5,15,16
//LED 6,17,22
//LED 7,23,24
/*
S3:表示HOLD
LED7 左边2竖表示电量
LED7 中间横线表示C/F，亮了表示C
LED7 其他4个分别表示MODE A，B，C，D
*/

