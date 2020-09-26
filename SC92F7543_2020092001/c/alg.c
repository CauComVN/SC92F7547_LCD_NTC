#include "H/alg.h"
#include "math.h"

float A;
float B;//B=0
float Q;
float H;
float R;
float x_last;//上次最优值
float p_mid;
float p_last;//上次最优协方差
float kg;
float x_mid;//当前预测值
float x_now;//当前最优值
float z_measure;//当前测量值


/*一阶互补滤波*/
static float filter_last = 0;
float First_Order_Filter(float x)
{
	static float	a = 0.78;
	if (filter_last == 0)
		filter_last = x;
	filter_last = (1-a)*filter_last + a * x;
	return filter_last;
}


void Alg_Start(float x)
{
	static unsigned char alg_start = 0;
	if (alg_start == 0)
	{
		A = 1;
		B = 0;
		Q = 0.5;
		H = 1;
		R = 0.25;
		
		x_last = x;
		p_last = 0.5;
		alg_start = 1;
		
		filter_last = x;
	}
}

float Alg_Process(float x)
{
	x = First_Order_Filter(x);
	
	x_mid = A*x_last /*+ B *Uk*/ + Q;//B=0
	p_mid = p_last+Q;/*A*p_last*A' + Q:A*A'=1*/
	kg = p_mid/(p_mid +R);
	z_measure = x;
	x_now = x_mid + kg * (z_measure - H * x_mid);
	p_last = (1-kg)*p_mid;
	x_last = x_now;
	return x_now;
}
