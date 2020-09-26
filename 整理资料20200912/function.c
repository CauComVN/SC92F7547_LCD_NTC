#include "c8051f320.h"  
#include  <intrins.h>
#include  <math.h>

sfr16 ADC0     = 0xbd;
sfr16 TMR3     = 0x94;                 // Timer3 counter
sfr16 TMR3RL   = 0x92;                 // Timer3 reload

#define MEASUREMENT_COUNTER	       100
#define ARRAY_NUMBER	       	   25
#define SYSCLK 12000000     //system clock
#define	CLK_SAR 3000000	    //ADC clock
pdata  unsigned int  mV;
void delayms(unsigned int ms);
unsigned int code TABLE[ARRAY_NUMBER+1]=
{
788,				  //-40,
742,691,636,579,521,  //-35,-30,-25,-20,-15
463,409,358,311,269,  //-10, -5,  0,  5, 10
231,199,170,146,125,  // 15, 20, 25, 30, 35
107, 92, 79, 68, 59,  // 40, 45, 50, 55, 60
 51, 44, 38, 33, 29   // 65, 70, 75, 80, 85
};

void Port_Init(void)
{
   P0 = 0x00;    
   P1 = 0x00;     
   P2 = 0x00;
   
   P0MDIN &= ~0x0c;   //P0.2 0.3 设置为模拟输入 
   P1MDIN =0XF9;      //p1.1,p1.2 as analog port
   P2MDIN =0XFF;      //p2 全作为输出模式

   P0MDOUT =0xFF;     
   P1MDOUT =0xBF;     //P1.6 as open-drain for I2C_SDA
   P2MDOUT =0x0F;     //P2.1,P2.2 p2.3as pull-push for LED,P2.0 as pull-push for I2C_VCC

   P0SKIP  =0X8C;     //Button interrupt,XTAL
   P1SKIP  =0X06;     //Skip p1.2,1.1 analog pin
   P2SKIP  =0X00;
   XBR0    =0x00;
   XBR1    =0x40;     //Enable Crossbar
//------------------拉低不用的管脚------------------
}
//-----------------------------------------------------------------------------
//定时器3初始化，
//选择的时钟是1M时钟,Nus 为定时器3溢出时间
//作为ADC的中断时钟源,定时器的中断使能没有打开
//------------------------------------------------------------
void Timer3_Init(unsigned int Nus)
{
	TMR3CN  = 0x00;	//清溢出标志位TF3，使用内部时钟/12 =1MHz，16位自动重装
	CKCON  |= 0x40;  // Select SYSCLK for timer 3 source

	TMR3RL  = 65535 - Nus; // init reload value for NuS

	TMR3	= 0xffff;//直接重装
	TMR3CN &=~0x04;	//TR3     = 0;//关闭定时器
}
//-----------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x05;                      // ADC0 disabled, normal tracking,
                                       // conversion triggered on TMR3 overflow

   REF0CN = 0x03;                      //  Enable on-chip VREF

   ADC0CF=((SYSCLK/CLK_SAR) - 1)<<3;	      //SAR时钟=3MHz

   ADC0CF |= 0x00;                     // Right-justify results
   
   Timer3_Init(10);//t=In(2^10/0.25)*(5+1)*10^3*10+10*1/3+1.5=0.499+3.3+1.5=5.3uS < 10uS
 
   EIE1 |= 0x08;                       // Enable ADC0 EOC interrupt
   EIP1 |= 0x08;
}
//-----------------------------------------------------------------------------
void ADC0_ISR (void) interrupt 10
{
  static pdata unsigned long accumulator = 0;     // accumulator for averaging
  static pdata unsigned int  measurements = MEASUREMENT_COUNTER;  // measurement counter
  unsigned long result=0;
   AD0INT = 0;                               // clear ADC0 conv. complete flag
   accumulator += ADC0;
   measurements--;
   if(measurements == 0)
   {  
   	  AD0EN = 0;                            // Disenable ADC0
	  TMR3CN &=~0x04;						//TR3     = 0;关闭定时器
      AD0INT = 0;                               // clear ADC0 conv. complete flag防止AD0=1
      measurements = MEASUREMENT_COUNTER;
      result = accumulator / MEASUREMENT_COUNTER;
	  mV =(unsigned int) result;
      accumulator=0;
   }
}
//------------------------------------------------------------------
signed int ConvertTemperature(unsigned int *iData,unsigned char OPA)
{
 pdata signed int iTemp;
 pdata unsigned int iValue;
 pdata unsigned char i;
 pdata unsigned char cBottom,cTop;
 iValue =(signed int) *iData;
  if(iValue > TABLE[0])
  {
    return 0xffff;
   }
  if(iValue < TABLE[ARRAY_NUMBER])
  {
    return 0xffff;
   }
  cBottom = 0;
  cTop    = ARRAY_NUMBER;
  for(i= ARRAY_NUMBER/2;(cTop-cBottom)!=1;)
  {
    if(iValue < TABLE[i])
	{
	  cBottom = i;
	  i = (cTop + cBottom) / 2;
	}
	else if(iValue > TABLE[i])
	{
	  cTop = i; 
      i = (cTop + cBottom) / 2; 
	}
	else
	{
	  iTemp = (i*5 - 40) * OPA;//扩大了OPA倍
	  return iTemp;
	}
   }
	iTemp = (i*5 - 40) * OPA;//扩大了OPA被
    iTemp = (((TABLE[cBottom] - iValue)*5*OPA) / (TABLE[cBottom] - TABLE[cTop])) + iTemp;//扩大OPA倍
	return iTemp;
}
void InternalClock(void)
{
   OSCICN |=0x80;             // enable internal oscillator
   CLKSEL &=0xFC;             // sysclk switch to internal oscillator 
}
void ExternClock(void)
{
   CLKSEL  |=0x01;          //sysclk switch to external oscillator
   OSCICN  &=0x7F;          //disable internal oscillator
}

void ChangeClock(void)
{
  if(!(REG0CN & 0x40))		 // if not connect the USB,open external oscillator
  {          
	ExternClock();
   }
  else
  {
    InternalClock();
   }
}

//-------------------------------------------------
//Pn_x 正向输入口 0x00 是P1.0 0x01 是P1.1 一直到0x0f为P2.7
//Nn_x 负向输入口 GND作为Port, Nn_x = 0x1F; 具体看DataSheet AMX0N设置表
//T3定时器打开,ADC使能打开
//延时2ms 这期间T3不断给ADC触发中断,满足ADC过样中断
//-------------------------------------------------
void OpenChannel_ADC(unsigned char Pn_x)//,unsigned char Nn_x) 
{
    AMX0P = Pn_x;             		  // ADC0 initial positive input = Pn+1.x
    AMX0N = 0x1F;                     // ADC0 negative input = GND
    TMR3CN |= 0x04;					  // 打开定时器
	AD0EN = 1;                        // Enable ADC0
    delayms(2);
}
signed int function(void)
{
	pdata signed int temp;
	InternalClock();//转换到内部时钟
	OpenChannel_ADC(0x01);//10uS*100=1mS < 2mS
	temp = ConvertTemperature(&mV,0x0A);
	//ChangeClock();//此时注意 这里的时钟为内部时钟 根据后面存储来决定时钟
	return(temp);
}
unsigned int battery(void)
{
    pdata unsigned long buff;
    pdata unsigned int battery_mV;
    InternalClock();//转换到内部时钟
	OpenChannel_ADC(0x02);//10uS*100=1mS < 2mS
	buff = mV;
	buff = buff * 2440 /1023;
	battery_mV = (unsigned int) buff;
	ChangeClock();//此时注意 这里的时钟没有插上USB时候是外部时钟32768
	return(battery_mV);
}
/*---------------------------------------------
void delayms(unsigned int ms)//进入此函数就需要4个周期 退出2个周期 (如果是char 就3个周期)
{
   if(CLKSEL&0x01)//3个机器周期
   //此时时钟为32768 每个机器周期为366.21uS
   {
    while(--ms);//此时8*ms+2个机器周期 
   }//(8*N+5)*366.21=(2.9*N+1.8)ms
   else//此时时钟为12M 每个机器周期时间为1uS
   {
     ms = ms * 100;//32个机器周期 ms应小于655
	 while((ms--)!=1 ); //10*ms+2个机器周期
   }//1000*NuS+2+32+3+6=N.041mS
}
*/