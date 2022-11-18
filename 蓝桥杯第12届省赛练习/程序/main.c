#include<STC15F2K60S2.h>
#include"ds1302.h"
#include"adda.h"
#define uchar unsigned char
#include"ds18b20.c"
#include"ds1302.c"
#include"adda.c"
#include"key.c"
#include"NE555.c"
uchar seven_seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar flash,cp,t,t_dot,sr,dac,ns;
void timer0_init()
{
	TMOD=0x01;
	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;
	EA=1;			  //中断总开关
	TR0=1;			  //定时器t0开关
	ET0=1;			  //定时器t0中断允许位开关
	
}
void timer0_isr()interrupt 1
{
	TH0=(65536-5000)/256;
	TL0=(65536-5000)%256;
	cp++;
	if(cp>=100)
	{
		flash=~flash;		
		t=ds18b20_read();
		t_dot=dot*0.625*100;
		cp=0;
	}
}
void display()
{
	uchar x,k,m,n;
	P0 = 0xff;
	P36 = 0;
	if(key_flag==0)
	{
		switch(x)
		{
			case 0:P2=0xc0;P0=0x01;P2=0xe0;P0=seven_seg[hour/10];break;
			case 1:P2=0xc0;P0=0x02;P2=0xe0;P0=seven_seg[hour%10];break;
			case 2:P2=0xc0;P0=0x04;P2=0xe0;P0=0xbf | flash;break;
			case 3:P2=0xc0;P0=0x08;P2=0xe0;P0=seven_seg[min/10];break;
			case 4:P2=0xc0;P0=0x10;P2=0xe0;P0=seven_seg[min%10];break;
			case 5:P2=0xc0;P0=0x20;P2=0xe0;P0=0xbf | flash;break;
			case 6:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[sec/10];break;
			case 7:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[sec%10];break;
		}
		x++;
		if(x>7){x=0;}
	}
	if(key_flag==1)
	{
		switch(k)
		{
			case 0:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[t_dot/10%10];break;
			case 1:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[t_dot%10];break;
			case 2:P2=0xc0;P0=0x20;P2=0xe0;P0=seven_seg[t%10]& 0x7f ;break;
			case 3:P2=0xc0;P0=0x10;P2=0xe0;P0=seven_seg[t/10];break;
			case 4:P2=0xc0;P0=0x01;P2=0xe0;P0=0xc6;break;	
		}
		k++;
		if(k>4){k=0;}
	}
	if(key_flag==2)						//滑动变阻器输出电压，需要用直接添加。
	{
		switch(m)
		{
			case 0:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[sr%10];break;
			case 1:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[sr/10%10];break;
			case 2:P2=0xc0;P0=0x20;P2=0xe0;P0=seven_seg[sr/100] & 0x7f;break;
			case 3:P2=0xc0;P0=0x01;P2=0xe0;P0=0xc1;break;	
		}
		m++;
		if(m>3){m=0;}	
	}
	if(key_flag==3)				 	 
	{
		switch(n)
		{
			case 0:P2=0xc0;P0=0x80;P2=0xe0;P0=seven_seg[ns%10];break;
			case 1:P2=0xc0;P0=0x40;P2=0xe0;P0=seven_seg[ns/10];break;
		}
		n++;
		if(n>2){n=0;}	
	}
}
void stc_init()
{
	P2=0xa0;
	P0=0x00;
	P2=0x00;
	P0=0xff;
}
void eeprom_read1()
{
	if(t>=30)
	{
		eeprom_writer(1,t);
		ns=eeprom_read(1);
	}
}								 
void main()
{  	
	stc_init();
	timer0_init();
	ds1302_init();
	write_cz1302(); 
	while(1)
	{
		sr=adc_read(1);   //光敏电阻与滑动变阻器两者不能同时添加
	//	sr=adc_read(3); 	//滑动变阻器
		get_1302();
		display();
		eeprom_read1();
		key_scan();
		dac_writer(255);			
	}
}