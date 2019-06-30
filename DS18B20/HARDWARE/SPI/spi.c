//---------------------------------------------------
//���ļ�Ϊģ��SPI�������
//CS1--C6  	CS2--C10	SCLK--C7
//MOSI--C8  MISO--C9
//---------------------------------------------------
#include "spi.h"
#include "delay.h"



void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE );//PORTBʱ��ʹ�� 
 
	//CS1--PC6,CS2--PC10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	CS1(1);//Ƭѡ����
	CS2(1);
	
	//SCLK  MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB

	CLK(0);
	MOSI(0);

	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PB13/14/15����������� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB	
}   

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 r_dat = 0;	
	u8 t_dat = TxData;
	u8 i;

	for(i=0; i<8; i++)
	{
		if(t_dat & 0x80)
			MOSI(1);
		else
			MOSI(0);
		t_dat = t_dat<<1;
		CLK(1);//CLK������д����
		delay_us(10);	
		
		CLK(0);//�½��ض�����
		r_dat = r_dat<<1;
		if(MISO)
			r_dat++;
		delay_us(10);	
	}
	return r_dat;
}

//д/���Ĵ���
//chip_sel:Ƭѡ�ź�
//addr--�Ĵ�����ַ
//datҪд������ݣ������������Ϊ����(��α�ֽڱ�ʾ��NULL_BYTE)
uint8_t write_read_data(uint8_t chip_sel, uint8_t addr, uint8_t dat)
{
	uint8_t rx_data;
	switch(chip_sel)
	{
		case 0://ѡ��MAX31856,�ȵ�ż����
		{
			CS2(1);//ȡ������Ƭѡ�ź�
			CS1(0);
			SPI1_ReadWriteByte(addr);
			rx_data = SPI1_ReadWriteByte(dat);
			CS1(1);
		}break;
		case 1://ѡ��MAX31865,PT100����
		{
			CS1(1);//ȡ������Ƭѡ�ź�
			CS2(0);
			SPI1_ReadWriteByte(addr);
			rx_data = SPI1_ReadWriteByte(dat);
			CS2(1);
		}break;
	}
	return rx_data;
}

