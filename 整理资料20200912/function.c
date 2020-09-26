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
   
   P0MDIN &= ~0x0c;   //P0.2 0.3 ����Ϊģ������ 
   P1MDIN =0XF9;      //p1.1,p1.2 as analog port
   P2MDIN =0XFF;      //p2 ȫ��Ϊ���ģʽ

   P0MDOUT =0xFF;     
   P1MDOUT =0xBF;     //P1.6 as open-drain for I2C_SDA
   P2MDOUT =0x0F;     //P2.1,P2.2 p2.3as pull-push for LED,P2.0 as pull-push for I2C_VCC

   P0SKIP  =0X8C;     //Button interrupt,XTAL
   P1SKIP  =0X06;     //Skip p1.2,1.1 analog pin
   P2SKIP  =0X00;
   XBR0    =0x00;
   XBR1    =0x40;     //Enable Crossbar
//------------------���Ͳ��õĹܽ�------------------
}
//-----------------------------------------------------------------------------
//��ʱ��3��ʼ����
//ѡ���ʱ����1Mʱ��,Nus Ϊ��ʱ��3���ʱ��
//��ΪADC���ж�ʱ��Դ,��ʱ�����ж�ʹ��û�д�
//------------------------------------------------------------
void Timer3_Init(unsigned int Nus)
{
	TMR3CN  = 0x00;	//�������־λTF3��ʹ���ڲ�ʱ��/12 =1MHz��16λ�Զ���װ
	CKCON  |= 0x40;  // Select SYSCLK for timer 3 source

	TMR3RL  = 65535 - Nus; // init reload value for NuS

	TMR3	= 0xffff;//ֱ����װ
	TMR3CN &=~0x04;	//TR3     = 0;//�رն�ʱ��
}
//-----------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x05;                      // ADC0 disabled, normal tracking,
                                       // conversion triggered on TMR3 overflow

   REF0CN = 0x03;                      //  Enable on-chip VREF

   ADC0CF=((SYSCLK/CLK_SAR) - 1)<<3;	      //SARʱ��=3MHz

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
	  TMR3CN &=~0x04;						//TR3     = 0;�رն�ʱ��
      AD0INT = 0;                               // clear ADC0 conv. complete flag��ֹAD0=1
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
	  iTemp = (i*5 - 40) * OPA;//������OPA��
	  return iTemp;
	}
   }
	iTemp = (i*5 - 40) * OPA;//������OPA��
    iTemp = (((TABLE[cBottom] - iValue)*5*OPA) / (TABLE[cBottom] - TABLE[cTop])) + iTemp;//����OPA��
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
//Pn_x ��������� 0x00 ��P1.0 0x01 ��P1.1 һֱ��0x0fΪP2.7
//Nn_x ��������� GND��ΪPort, Nn_x = 0x1F; ���忴DataSheet AMX0N���ñ�
//T3��ʱ����,ADCʹ�ܴ�
//��ʱ2ms ���ڼ�T3���ϸ�ADC�����ж�,����ADC�����ж�
//-------------------------------------------------
void OpenChannel_ADC(unsigned char Pn_x)//,unsigned char Nn_x) 
{
    AMX0P = Pn_x;             		  // ADC0 initial positive input = Pn+1.x
    AMX0N = 0x1F;                     // ADC0 negative input = GND
    TMR3CN |= 0x04;					  // �򿪶�ʱ��
	AD0EN = 1;                        // Enable ADC0
    delayms(2);
}
signed int function(void)
{
	pdata signed int temp;
	InternalClock();//ת�����ڲ�ʱ��
	OpenChannel_ADC(0x01);//10uS*100=1mS < 2mS
	temp = ConvertTemperature(&mV,0x0A);
	//ChangeClock();//��ʱע�� �����ʱ��Ϊ�ڲ�ʱ�� ���ݺ���洢������ʱ��
	return(temp);
}
unsigned int battery(void)
{
    pdata unsigned long buff;
    pdata unsigned int battery_mV;
    InternalClock();//ת�����ڲ�ʱ��
	OpenChannel_ADC(0x02);//10uS*100=1mS < 2mS
	buff = mV;
	buff = buff * 2440 /1023;
	battery_mV = (unsigned int) buff;
	ChangeClock();//��ʱע�� �����ʱ��û�в���USBʱ�����ⲿʱ��32768
	return(battery_mV);
}
/*---------------------------------------------
void delayms(unsigned int ms)//����˺�������Ҫ4������ �˳�2������ (�����char ��3������)
{
   if(CLKSEL&0x01)//3����������
   //��ʱʱ��Ϊ32768 ÿ����������Ϊ366.21uS
   {
    while(--ms);//��ʱ8*ms+2���������� 
   }//(8*N+5)*366.21=(2.9*N+1.8)ms
   else//��ʱʱ��Ϊ12M ÿ����������ʱ��Ϊ1uS
   {
     ms = ms * 100;//32���������� msӦС��655
	 while((ms--)!=1 ); //10*ms+2����������
   }//1000*NuS+2+32+3+6=N.041mS
}
*/