#include "ADDA.h"

void Delay2ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 22;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}


unsigned char date,date1,date3;
//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//应答位控制
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

unsigned char adc_read(unsigned char addr)
{
	IIC_Start();			 //打开iic总线
	IIC_SendByte(0x90);		 //向iic总线中写入数据
	IIC_WaitAck();			 //等待响应
	IIC_SendByte(addr);		 //写入需要读取的地址AIN0，AIN1，AIN2，AIN3.
	IIC_WaitAck();			 //等待响应
	IIC_Stop();				 //关闭总线
	IIC_Start();			 //打开总线
	IIC_SendByte(0x91);		 //从iic总线中读取数据
	IIC_WaitAck();		     //等待响应
	date=IIC_RecByte();		 //将读取到的数据赋值给变量
	IIC_Ack(0);				 //不应答
	IIC_Stop();				 //关闭iic总线
	return date;			 //返回date的值，并在主函数中调用，得到对应ain口的电压。
}

void dac_writer(unsigned char addr)
{
	IIC_Start();			   //打开iic总线开关
	IIC_SendByte(0x90);		   //向iic总线中写入数据
	IIC_WaitAck();			   //等待响应
	IIC_SendByte(0x40);		   //向0x40这个地址写 
	IIC_WaitAck();		   	   //等待响应
	IIC_SendByte(addr);		   //写入特定电压值
	IIC_WaitAck();			   //等待响应
	IIC_Stop();				   //关闭iic总线
}

unsigned char eeprom_read(unsigned char addr)
{
	IIC_Start();			  //打开iic总线开关
	IIC_SendByte(0xa0);		  //对eeprom的写操作
	IIC_WaitAck();			  //等待响应
	IIC_SendByte(addr);		  //写入读取时的地址
	IIC_WaitAck();			  //等待响应
	IIC_Stop();
	IIC_Start();
	IIC_SendByte(0xa1);	      //开始读取
	IIC_WaitAck();			  //等待响应
	date1=IIC_RecByte();	  //将读取到的值赋给变量
	IIC_Ack(0);				  //不应答
	IIC_Stop();				  //关闭iic总线
	return date1;			  //返回变量值
	Delay2ms();				  //延时2ms
}

void eeprom_writer(unsigned char addr,date3)
{
	IIC_Start();			  //打开iic总线
	IIC_SendByte(0xa0);		  //找到0xa0地址
	IIC_WaitAck();			  //等待响应
	IIC_SendByte(addr);		  //写入地址
	IIC_WaitAck();
	IIC_SendByte(date3);		   //写入向该地址存储的数据
	IIC_WaitAck();						 //等待响应
	IIC_Stop();								  //iic总线关闭
	Delay2ms();											   //延时2ms
}

