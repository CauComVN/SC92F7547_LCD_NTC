#include "H/Function_Init.H"
#include "H/UserHeader.h"
#include "H/Key.h"


unsigned short usKeyPin = 0;
unsigned short usKeyDebounce = 0;
unsigned char  ucKeyFlag = 0;
unsigned char  ucComKeyCnt = 0;
unsigned char ucKeyCurrent;

void KeyInit(void)
{
	//P05,P06,P07
	P0CON &= 0x1F;
	P0PH |= 0xE0;
}

unsigned char xGetKeyPin(void)
{
	unsigned char keypresscnt = 0;
	ucKeyCurrent = 0;
	if (P07 == 0)
	{
		ucKeyCurrent |= 1<<2;
		keypresscnt++;
	}
	else if (P06 == 0)
	{
		ucKeyCurrent |= 1<<1;
		keypresscnt++;
	}
	else if (P05 == 0)
	{
		ucKeyCurrent |= 1<<0;
		keypresscnt++;
	}
	
	return keypresscnt;
}

//unsigned char xGetKeyFlag(void)
//{
//	return ucKeyFlag;
//}

//unsigned char xGetCurrentKey(void)
//{
//	return ucKeyCurrent;
//}

unsigned short KeyScanProcess(void)
{
	unsigned short ret = 0;
	unsigned char keypresscnt;
	P0VO = 0x1C;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	KeyInit();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	
	ret = 0;
	keypresscnt = xGetKeyPin();
	if (ucKeyCurrent)//key press
	{
		usKeyDebounce++;
		if ((keypresscnt == 1)&& (ucKeyFlag != 0x80))
		{
			if (usKeyPin == ucKeyCurrent)
			{
				if (usKeyDebounce >= KEY_LONGDEBOUNCE)
				{
					usKeyDebounce = KEY_LONGDEBOUNCE;
					ucKeyFlag = 0x40;
				}
			}
			else
			{
				
				if (ucKeyFlag == 0x80)
				{
					ret = usKeyPin + 0x8000;
				}
				else if (usKeyDebounce >= KEY_LONGDEBOUNCE)
				{
					ret = usKeyPin + 0x4000;
				}
				else
				{
					ret = usKeyPin;
				}
				usKeyDebounce = 0;
				ucKeyFlag = 0;
				usKeyPin = 0;
				ucComKeyCnt = 0;
				usKeyPin = ucKeyCurrent;
				usKeyDebounce = 0;
			}
		}
		else // comb
		{
			if (usKeyPin == ucKeyCurrent)
			{
				if (usKeyDebounce >= KEY_COMBDEBOUNCE)
				{
					usKeyDebounce = KEY_COMBDEBOUNCE;
					ucKeyFlag = 0x80;
				}
			}
			else
			{
				if (ucComKeyCnt < keypresscnt)
				{
					ucComKeyCnt = keypresscnt;
					usKeyPin = ucKeyCurrent;
					usKeyDebounce = 0;
				}
			}
		}
	}
	else
	{
		if (ucKeyFlag == 0x80)
		{
			ret = usKeyPin + 0x8000;
		}
		else if (usKeyDebounce >= KEY_LONGDEBOUNCE)
		{
			ret = usKeyPin + 0x4000;
		}
		else
		{
			ret = usKeyPin;
		}
		usKeyDebounce = 0;
		ucKeyFlag = 0;
		usKeyPin = 0;
		ucComKeyCnt = 0;
	}
	P0VO = 0xFC;
	return ret;
}


