#include "H/Signal.h"
#include "H/Function_Init.H"

unsigned short L_TEMP,L_TEMP1,D0,D0_old=0;	

#define Max_average_count	16
char old_number;			
unsigned short old_Data[16];
/***********************************************
*功能：让读数站稳，不乱飘。
*较正点由 ((value-minvalue)*100/maxvalue)决定。
***********************************************/
void Function_average_number(void)
{
//static char old_zero;
long sum_Data=0;
long temp_Data;
long Max_Data,Min_Data;
int Temp_group;
long n;
char i,Number_average;
	
	n = D0 - old_Data[0];

	Temp_group = 20;
		
	if((n<Temp_group)&&(n>(-Temp_group)))
	{
		Number_average = Max_average_count;
				
		if(old_number>=Number_average)
		{
			for(i=0;i<Number_average-1;i++)
			{
				old_Data[i] = old_Data[i+1];
			}
		}
		else
		{						
			old_number++;
		}
		
		old_Data[old_number-1] = D0;
		
		Max_Data = old_Data[0];
		Min_Data = old_Data[0];
		
		for(i=0;i<old_number;i++)
		{
			sum_Data = sum_Data+old_Data[i];
			if(Max_Data<old_Data[i])
				Max_Data=old_Data[i];
			if(Min_Data>old_Data[i])
				Min_Data=old_Data[i];	
		}

		if(old_number<5)
		{
			temp_Data = sum_Data/old_number;
		}
		else
		{
			temp_Data = (sum_Data-Max_Data-Min_Data)/(old_number-2);
		}			
	}
	else
	{
		old_Data[0] = D0;
		old_number = 1;			
	}
		
}	


short code mucai_taba[][2] =
{
	{0,0},
	{60,30},		//40G	,猜想
	{80,40},		//30G	,猜想
	{115,45},		//20G
	{135,53},		//15G
	{154,56},		//10G
	{186,63},		//6G
	{222,69},		//2.4G,
	{246,85},		//1.2G
	{288,88},		//600M
	{324,94},		//300M
	{357,99},		//100M
	{399,106},		//45M
	{431,113},		//22M
	{435,115},		//20M
	{466,126},		//10M
	{477,130},		//8M
	{499,136},		//5M
	{523,144},		//3M
	{543,153},		//2M
	{554,156},		//1.6M
	{578,163},		//1M
	{583,167},		//900K
	{598,173},		//680K
	{616,183},		//490K
	{639,229},		//330K
	{665,304},		//220K
	{692,385},		//150K
	{723,485},		//100K
	{732,505},		//90K
	{741,549},		//80K
	{752,584},		//70K
	{765,638},		//60K
	{782,721},		//50K
	{802,849},		//40K
	{827,923},		//30K
	{861,936},		//20K
	{911,955},		//10K
	{1000,995},
	{1010,998},
	{1030,999}
	
};

short code mucai_tabb[][2] =
{
	{0,0},
	{60,35},		//40G	,猜想
	{80,45},		//30G	,猜想
	{115,56},		//20G
	{135,64},		//15G
	{154,69},
	{186,76},
	{222,83},
	{246,99},
	{288,102},		//600M
	{324,108},		//300M
	{357,113},		//100M
	{399,121},		//45M
	{431,128},		//22M
	{435,130},		//20M
	{466,144},		//10M
	{477,149},		//8M
	{499,156},		//5M
	{523,164},		//3M
	{543,173},		//2M
	{554,177},		//1.6M
	{578,184},		//1M
	{583,189},		//900K
	{598,196},		//680K
	{616,217},		//490K
	{639,257},		//330K
	{665,328},		//220K
	{692,385},		//150K
	{723,485},		//100K
	{732,505},		//90K
	{741,549},		//80K
	{752,584},		//70K
	{765,638},		//60K
	{782,721},		//50K
	{802,849},		//40K
	{827,923},		//30K
	{861,936},		//20K
	{911,955},		//10K
	{1000,995},
	{1010,998},
	{1030,999}
	
};

short code mucai_tabc[][2] =
{
	{0,0},
	{60,45},		//40G	,猜想
	{80,50},		//30G	,猜想
	{115,62},		//20G
	{135,72},		//15G
	{154,77},
	{186,84},
	{222,92},
	{246,112},
	{288,115},		//600M
	{324,123},		//300M
	{357,130},		//100M
	{399,141},		//45M
	{431,150},		//22M
	{435,155},		//20M
	{466,174},		//10M
	{477,178},		//8M
	{499,191},		//5M
	{523,204},		//3M
	{543,217},		//2M
	{554,226},		//1.6M
	{578,236},		//1M
	{583,241},		//900K
	{598,248},		//680K
	{616,273},		//490K
	{639,309},		//330K
	{665,349},		//220K
	{692,385},		//150K
	{723,485},		//100K
	{732,505},		//90K
	{741,549},		//80K
	{752,584},		//70K
	{765,638},		//60K
	{782,721},		//50K
	{802,849},		//40K
	{827,923},		//30K
	{861,936},		//20K
	{911,955},		//10K
	{1000,995},
	{1010,998},
	{1030,999}
	
};

short code mucai_tabd[][2] =
{
	{0,0},
	{60,50},		//40G	,猜想
	{80,58},		//30G	,猜想
	{115,68},		//20G
	{135,79},		//15G
	{154,84},
	{186,93},
	{222,101},
	{246,117},
	{288,118},		//600M
	{324,128},		//300M
	{357,134},		//100M
	{399,143},		//45M
	{431,151},		//22M
	{435,153},		//20M
	{466,166},		//10M
	{477,172},		//8M
	{499,180},		//5M
	{523,191},		//3M
	{543,205},		//2M
	{554,211},		//1.6M
	{578,222},		//1M
	{583,227},		//900K
	{598,233},		//680K
	{616,255},		//490K
	{639,286},		//330K
	{665,333},		//220K
	{692,385},		//150K
	{723,485},		//100K
	{732,505},		//90K
	{741,549},		//80K
	{752,584},		//70K
	{765,638},		//60K
	{782,721},		//50K
	{802,849},		//40K
	{827,923},		//30K
	{861,936},		//20K
	{911,955},		//10K
	{1000,995},
	{1010,998},
	{1030,999}
	
};

short code mucai_tabe[][2] =
{
	{0,0},
	{10,10},
	{20,20},
	{30,30},
	{40,40},
	{50,50},
	{60,60},
	{70,70},
	{80,80},
	{90,90},
	{100,100},
	{110,110},
	{120,120},
	{130,130},
	{140,140},
	{150,150},
	{160,160},
	{170,170},
	{180,180},
	{190,190},
	{200,200},
	{210,210},
	{220,220},
	{230,230},
	{240,240},
	{250,250},
	{260,260},
	{270,270},
	{280,280},
	{290,290},
	{300,300},
	{400,400},
	{500,500},
	{600,600},
	{700,700},
	{800,800},
	{900,900},
	{1000,1000},
	{1010,1010},
	{1030,1030}
	
};


int Measure_Process(MODETYPE eMode,unsigned short adcValue)
{
char i,j;		
unsigned long temp_value,temp_value1;
	
		L_TEMP = adcValue;
		
		D0 = L_TEMP;
		
		//平均算法。
		Function_average_number();
		
		//稳定
#if 1
		if(((D0-D0_old)<20)&&((D0-D0_old)>-20))
		{
			D0 = (D0_old + D0_old + D0_old + D0 + D0) / 5;
		}		
#endif		
		
		L_TEMP = D0;
		
		//A类型
		if(eMode == eMODEA)
		{
			i=0;
			j=1;
			while(j)
			{
				if(L_TEMP<=mucai_taba[i][0])
				{
					j=0;
					break;
				}
				i++;
				if(i>=40)
				{
					j=0;
					break;
				}				
			}
			
			if((i>0)&(i<50))
			{
				temp_value = mucai_taba[i-1][1];
				temp_value1 = (L_TEMP-mucai_taba[i-1][0]);
				temp_value1 = temp_value1*(mucai_taba[i][1]-mucai_taba[i-1][1]);
				temp_value1 = temp_value1 / (mucai_taba[i][0]-mucai_taba[i-1][0]);
				temp_value = temp_value + temp_value1; 
			}
			else
			{
				temp_value = 0;
			}			
			
			L_TEMP1 = (long)(temp_value);

			if(L_TEMP1>=1000)
			{
				L_TEMP1 = 1000;
			}
								
//			LCD_BUFFER[2] = L_TEMP1 /100;
//			L_TEMP1 = L_TEMP1 % 100;
//			LCD_BUFFER[1] = L_TEMP1/10;
//			L_TEMP1 = L_TEMP1 % 10;
//			LCD_BUFFER[0] = L_TEMP1; 
			return L_TEMP1;
				
		}
		else if(eMode == eMODEB)
		{
			i=0;
			j=1;
			while(j)
			{
				if(L_TEMP<=mucai_tabb[i][0])
				{
					j=0;
					break;
				}
				i++;
				if(i>=40)
				{
					j=0;
					break;
				}				
			}
			
			if((i>0)&(i<50))
			{
				temp_value = mucai_tabb[i-1][1];
				temp_value1 = (L_TEMP-mucai_tabb[i-1][0]);
				temp_value1 = temp_value1*(mucai_tabb[i][1]-mucai_tabb[i-1][1]);
				temp_value1 = temp_value1 / (mucai_tabb[i][0]-mucai_tabb[i-1][0]);
				temp_value = temp_value + temp_value1; 
			}
			else
			{
				temp_value = 0;
			}			
			
			L_TEMP1 = (long)(temp_value);

			if(L_TEMP1>=1000)
			{
				L_TEMP1 = 1000;
			}
								
//			LCD_BUFFER[2] = L_TEMP1 /100;
//			L_TEMP1 = L_TEMP1 % 100;
//			LCD_BUFFER[1] = L_TEMP1/10;
//			L_TEMP1 = L_TEMP1 % 10;
//			LCD_BUFFER[0] = L_TEMP1; 
				
			return L_TEMP1;
		}
		else if(eMode == eMODEC)
		{
			i=0;
			j=1;
			while(j)
			{
				if(L_TEMP<=mucai_tabc[i][0])
				{
					j=0;
					break;
				}
				i++;
				if(i>=40)
				{
					j=0;
					break;
				}				
			}
			
			if((i>0)&(i<50))
			{
				temp_value = mucai_tabc[i-1][1];
				temp_value1 = (L_TEMP-mucai_tabc[i-1][0]);
				temp_value1 = temp_value1*(mucai_tabc[i][1]-mucai_tabc[i-1][1]);
				temp_value1 = temp_value1 / (mucai_tabc[i][0]-mucai_tabc[i-1][0]);
				temp_value = temp_value + temp_value1; 
			}
			else
			{
				temp_value = 0;
			}			
			
			L_TEMP1 = (long)(temp_value);

			if(L_TEMP1>=1000)
			{
				L_TEMP1 = 1000;
			}
								
//			LCD_BUFFER[2] = L_TEMP1 /100;
//			L_TEMP1 = L_TEMP1 % 100;
//			LCD_BUFFER[1] = L_TEMP1/10;
//			L_TEMP1 = L_TEMP1 % 10;
//			LCD_BUFFER[0] = L_TEMP1; 
			return L_TEMP1;
				
		}
		else if(eMode == eMODED)
		{
			i=0;
			j=1;
			while(j)
			{
				if(L_TEMP<=mucai_tabd[i][0])
				{
					j=0;
					break;
				}
				i++;
				if(i>=40)
				{
					j=0;
					break;
				}				
			}
			
			if((i>0)&(i<50))
			{
				temp_value = mucai_tabd[i-1][1];
				temp_value1 = (L_TEMP-mucai_tabd[i-1][0]);
				temp_value1 = temp_value1*(mucai_tabd[i][1]-mucai_tabd[i-1][1]);
				temp_value1 = temp_value1 / (mucai_tabd[i][0]-mucai_tabd[i-1][0]);
				temp_value = temp_value + temp_value1; 
			}
			else
			{
				temp_value = 0;
			}			
			
			L_TEMP1 = (long)(temp_value);

			if(L_TEMP1>=1000)
			{
				L_TEMP1 = 1000;
			}
								
//			LCD_BUFFER[2] = L_TEMP1 /100;
//			L_TEMP1 = L_TEMP1 % 100;
//			LCD_BUFFER[1] = L_TEMP1/10;
//			L_TEMP1 = L_TEMP1 % 10;
//			LCD_BUFFER[0] = L_TEMP1; 
			return L_TEMP1;
		}
//		else
//		{
//			LCD_BUFFER[2] = L_TEMP /100;
//			L_TEMP = L_TEMP % 100;
//			LCD_BUFFER[1] = L_TEMP/10;
//			L_TEMP = L_TEMP % 10;
//			LCD_BUFFER[0] = L_TEMP; 
//		}	
		return L_TEMP;
//		Display_Lcd_Number(2,LCD_BUFFER[2]);
//		Display_Lcd_Number(1,LCD_BUFFER[1]);
//		Display_Lcd_Number(0,LCD_BUFFER[0]);
//		Display_Lcd_Dot(1);
}
