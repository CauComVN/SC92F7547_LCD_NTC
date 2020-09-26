#include <SH79F166A.h>

extern unsigned short D0;

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
