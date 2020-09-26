#ifndef _NTC_H_
#define _NTC_H_


#define TEMP_L_LIMIT	-10
#define TEMP_H_LIMIT	50

#define TEMP_TOTAL	(TEMP_H_LIMIT - TEMP_L_LIMIT + 1)

#define NTC_UP	0

 
#define ADC_MAX 4095.0    //12?ADC

#define NTC_R  100  //�������裬��λK

#if NTC_UP //NTC �ڵ�Դ��
	#define NTC_ADC(ntc)   (unsigned short)(((NTC_R*ADC_MAX)/(ntc+NTC_R)))	//��ֵ��ADC��ϵ NTC�ڵ�Դ�ˣ�ADCֵ����
#else	//NTC �ӵ�
	#define NTC_ADC(ntc)   (unsigned short)(((ntc*ADC_MAX)/(ntc+NTC_R)))	//��ֵ��ADC��ϵ NTC�ӵأ�ADCֵ����
#endif


extern short xGetTempCbyNtc(unsigned short adc);
	
#endif
