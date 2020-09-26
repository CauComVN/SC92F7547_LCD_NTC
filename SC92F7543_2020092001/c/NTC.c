#include "H/NTC.h"
#include "H/alg.h"


unsigned short NtcAdc[TEMP_TOTAL]=
{
	NTC_ADC(551.100),
	NTC_ADC(522.117),
	NTC_ADC(494.824),
	NTC_ADC(469.113),
	NTC_ADC(444.886),
	NTC_ADC(422.050),
	NTC_ADC(400.518),
	NTC_ADC(380.209),
	NTC_ADC(361.048),
	NTC_ADC(342.963),
	NTC_ADC(326.560),
	NTC_ADC(309.764),
	NTC_ADC(294.529),
	NTC_ADC(280.131),
	NTC_ADC(266.520),
	NTC_ADC(253.647),
	NTC_ADC(241.470),
	NTC_ADC(229.946),
	NTC_ADC(219.036),
	NTC_ADC(208.706),
	NTC_ADC(198.920),
	NTC_ADC(189.647),
	NTC_ADC(180.857),
	NTC_ADC(172.523),
	NTC_ADC(164.618),
	NTC_ADC(157.118),
	NTC_ADC(150.000),
	NTC_ADC(143.243),
	NTC_ADC(136.827),
	NTC_ADC(130.731),
	NTC_ADC(124.940),
	NTC_ADC(119.435),
	NTC_ADC(114.202),
	NTC_ADC(109.225),
	NTC_ADC(104.491),
	NTC_ADC(100.000),
	NTC_ADC(95.699),
	NTC_ADC(91.617),
	NTC_ADC(87.731),
	NTC_ADC(84.028),
	NTC_ADC(80.501),
	NTC_ADC(77.140),
	NTC_ADC(73.936),
	NTC_ADC(70.881),
	NTC_ADC(67.968),
	NTC_ADC(96.188),
	NTC_ADC(92.537),
	NTC_ADC(60.006),
	NTC_ADC(57.590),
	NTC_ADC(55.283),
	NTC_ADC(53.080),
	NTC_ADC(50.976),
	NTC_ADC(48.965),
	NTC_ADC(47.044),
	NTC_ADC(45.207),
	NTC_ADC(43.451),
	NTC_ADC(41.771),
	NTC_ADC(40.165),
	NTC_ADC(38.628),
	NTC_ADC(37.157),
	NTC_ADC(35.750),
};

short xGetTempCbyNtc(unsigned short adc)
{
	unsigned char i;
    float tp;
    //adc+=70;
	#if NTC_UP
    for (i=0; i<TEMP_TOTAL; i++)
    {
        if (adc<NtcAdc[i])
        {
            break;
        }
    }
    if (i==0)
    {
        tp=TEMP_L_LIMIT;
    }
    else if (i>=TEMP_TOTAL)
    {
        tp=TEMP_H_LIMIT;
    }
    else
    {
//        tp=(adc-NtcAdc[i-1])*10.0f/(NtcAdc[i]-NtcAdc[i-1])+(i-40-1)*10.0f;
    }
	#else
    for (i=0; i<TEMP_TOTAL; i++)
    {
        if (adc>NtcAdc[i])
        {
            break;
        }
    }
    if (i==0)
    {
        tp=TEMP_L_LIMIT;
    }
    else if (i>=TEMP_TOTAL)
    {
        tp=TEMP_H_LIMIT;
    }
    else
    {
//        tp=(NtcAdc[i-1]-adc)*10.0f/(NtcAdc[i-1]-NtcAdc[i])+(i-40-1)*10.0f;
		tp = (i + TEMP_L_LIMIT)*10.0f;
    }
	#endif
	Alg_Start(tp);
	tp = Alg_Process(tp);
    return (short)tp;
}


