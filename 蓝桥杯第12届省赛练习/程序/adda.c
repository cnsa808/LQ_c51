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
//������������
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//Ӧ��λ����
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

//�ȴ�Ӧ��
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

//ͨ��I2C���߷�������
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

//��I2C�����Ͻ�������
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
	IIC_Start();			 //��iic����
	IIC_SendByte(0x90);		 //��iic������д������
	IIC_WaitAck();			 //�ȴ���Ӧ
	IIC_SendByte(addr);		 //д����Ҫ��ȡ�ĵ�ַAIN0��AIN1��AIN2��AIN3.
	IIC_WaitAck();			 //�ȴ���Ӧ
	IIC_Stop();				 //�ر�����
	IIC_Start();			 //������
	IIC_SendByte(0x91);		 //��iic�����ж�ȡ����
	IIC_WaitAck();		     //�ȴ���Ӧ
	date=IIC_RecByte();		 //����ȡ�������ݸ�ֵ������
	IIC_Ack(0);				 //��Ӧ��
	IIC_Stop();				 //�ر�iic����
	return date;			 //����date��ֵ�������������е��ã��õ���Ӧain�ڵĵ�ѹ��
}

void dac_writer(unsigned char addr)
{
	IIC_Start();			   //��iic���߿���
	IIC_SendByte(0x90);		   //��iic������д������
	IIC_WaitAck();			   //�ȴ���Ӧ
	IIC_SendByte(0x40);		   //��0x40�����ַд 
	IIC_WaitAck();		   	   //�ȴ���Ӧ
	IIC_SendByte(addr);		   //д���ض���ѹֵ
	IIC_WaitAck();			   //�ȴ���Ӧ
	IIC_Stop();				   //�ر�iic����
}

unsigned char eeprom_read(unsigned char addr)
{
	IIC_Start();			  //��iic���߿���
	IIC_SendByte(0xa0);		  //��eeprom��д����
	IIC_WaitAck();			  //�ȴ���Ӧ
	IIC_SendByte(addr);		  //д���ȡʱ�ĵ�ַ
	IIC_WaitAck();			  //�ȴ���Ӧ
	IIC_Stop();
	IIC_Start();
	IIC_SendByte(0xa1);	      //��ʼ��ȡ
	IIC_WaitAck();			  //�ȴ���Ӧ
	date1=IIC_RecByte();	  //����ȡ����ֵ��������
	IIC_Ack(0);				  //��Ӧ��
	IIC_Stop();				  //�ر�iic����
	return date1;			  //���ر���ֵ
	Delay2ms();				  //��ʱ2ms
}

void eeprom_writer(unsigned char addr,date3)
{
	IIC_Start();			  //��iic����
	IIC_SendByte(0xa0);		  //�ҵ�0xa0��ַ
	IIC_WaitAck();			  //�ȴ���Ӧ
	IIC_SendByte(addr);		  //д���ַ
	IIC_WaitAck();
	IIC_SendByte(date3);		   //д����õ�ַ�洢������
	IIC_WaitAck();						 //�ȴ���Ӧ
	IIC_Stop();								  //iic���߹ر�
	Delay2ms();											   //��ʱ2ms
}

