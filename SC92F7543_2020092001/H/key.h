#ifndef _KEY_H_
#define _KEY_H_
#define KEY_DEBOUNCE	40	//40ms
#define KEY_COMBDEBOUNCE	200
#define KEY_LONGDEBOUNCE	1000	//500ms

extern void KeyInit(void);
extern unsigned short KeyScanProcess(void);
extern unsigned char xGetKeyFlag(void);
extern unsigned char xGetCurrentKey(void);

#endif
