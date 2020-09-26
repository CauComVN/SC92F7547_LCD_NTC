#ifndef _NTC_H_
#define _NTC_H_


#define TEMP_L_LIMIT	-10
#define TEMP_H_LIMIT	50

#define TEMP_TOTAL	(TEMP_H_LIMIT - TEMP_L_LIMIT + 1)

#define NTC_UP	0

 
#define ADC_MAX 4095.0    //12?ADC

#define NTC_R  100  //串联电阻，单位K

#if NTC_UP //NTC 在电源端
	#define NTC_ADC(ntc)   (unsigned short)(((NTC_R*ADC_MAX)/(ntc+NTC_R)))	//阻值与ADC关系 NTC在电源端，ADC值升序
#else	//NTC 接地
	#define NTC_ADC(ntc)   (unsigned short)(((ntc*ADC_MAX)/(ntc+NTC_R)))	//阻值与ADC关系 NTC接地，ADC值降序
#endif


extern short xGetTempCbyNtc(unsigned short adc);
	
#endif
