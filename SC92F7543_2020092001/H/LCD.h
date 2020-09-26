#ifndef _LCD_H_
#define _LCD_H_


#include "H/Function_Init.H"
#include "H/UserHeader.h"

#define LCD_BUF_LEN	25

extern void LCD_Init(void);
extern void LED_Init(void);
extern void LCD_Temperature(TEMPERATURE eTemp);
extern void LCD_Mode(MODETYPE eMode);
extern void Lcd_Hold(HOLDTYPE eHold);
extern void LCD_Bat(unsigned char level);
extern void LCD_Signal(unsigned short signal);
extern void LCD_NTC(short usNtc);
extern void LCD_Reflash(void);
#endif
