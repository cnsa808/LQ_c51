#include<STC15F2K60S2.H>
#include<intrins.h>
#define uchar unsigned char
uchar key_flag=0,i=0x01;
void delay(uchar x)		//@11.0592MHz
{
	while(x--);
}

void key_scan()
{
	P42=0;
	if(P32==0)			//按键S4
	{
		delay(50);
		while(P32==0);
		if(P32==1)
		{
			key_flag++;				  //three mode
			if(key_flag>3)key_flag=0;
		}					
	}
						  //**************************剩余按键完成指定操作需后续添加******************************//
}