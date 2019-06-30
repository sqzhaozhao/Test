//---------------------------------------------------
//该文件为模拟SPI程序代码
//CS1--C6  	CS2--C10	SCLK--C7
//MOSI--C8  MISO--C9
//---------------------------------------------------
#include "spi.h"
#include "delay.h"



void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE );//PORTB时钟使能 
 
	//CS1--PC6,CS2--PC10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	CS1(1);//片选拉高
	CS2(1);
	
	//SCLK  MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB

	CLK(0);
	MOSI(0);

	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //PB13/14/15复用推挽输出 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB	
}   

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
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
		CLK(1);//CLK上升沿写数据
		delay_us(10);	
		
		CLK(0);//下降沿读数据
		r_dat = r_dat<<1;
		if(MISO)
			r_dat++;
		delay_us(10);	
	}
	return r_dat;
}

//写/读寄存器
//chip_sel:片选信号
//addr--寄存器地址
//dat要写入的数据，如果读该数据为任意(用伪字节表示，NULL_BYTE)
uint8_t write_read_data(uint8_t chip_sel, uint8_t addr, uint8_t dat)
{
	uint8_t rx_data;
	switch(chip_sel)
	{
		case 0://选择MAX31856,热电偶测温
		{
			CS2(1);//取消其它片选信号
			CS1(0);
			SPI1_ReadWriteByte(addr);
			rx_data = SPI1_ReadWriteByte(dat);
			CS1(1);
		}break;
		case 1://选择MAX31865,PT100测温
		{
			CS1(1);//取消其它片选信号
			CS2(0);
			SPI1_ReadWriteByte(addr);
			rx_data = SPI1_ReadWriteByte(dat);
			CS2(1);
		}break;
	}
	return rx_data;
}

