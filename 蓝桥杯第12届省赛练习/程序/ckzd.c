#include<STC15F2K60S2.H>

void UartInit(void)		//9600bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xC7;		//�趨��ʱ��ֵ
	TH1 = 0xFE;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA=1;
	ES=1;
}

void ckzd()interrupt 4
{
	unsigned char i;
	i=SBUF;
	if(i==1)
	{
		RI=0;
		P2=0x80;
		P0=0x00;
		SBUF='c';
		while(TI==0);
		TI=0;	
	}
	if(i==2)
	{
		RI=0;
		P2=0x80;
		P0=0xff;
		SBUF='d';
		while(TI==0);
		TI=0;
	}	
}