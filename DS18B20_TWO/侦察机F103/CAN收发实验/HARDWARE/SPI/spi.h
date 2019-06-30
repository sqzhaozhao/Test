#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

//MAX31856片选信号，当为0时，不允许其它片选
#define CS1(n)			(n?GPIO_SetBits(GPIOA,GPIO_Pin_3):GPIO_ResetBits(GPIOA,GPIO_Pin_3))

//MAX31865片选信号，当为0时，不允许其它片选
#define CS2(n)			(n?GPIO_SetBits(GPIOA,GPIO_Pin_4):GPIO_ResetBits(GPIOA,GPIO_Pin_4))


#define CLK(n)		(n?GPIO_SetBits(GPIOA,GPIO_Pin_0):GPIO_ResetBits(GPIOA,GPIO_Pin_0))
#define MOSI(n)		(n?GPIO_SetBits(GPIOA,GPIO_Pin_1):GPIO_ResetBits(GPIOA,GPIO_Pin_1))


#define MISO  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键0	    													  
void SPI1_Init(void);			 //初始化SPI口
u8 SPI1_ReadWriteByte(u8 TxData);
uint8_t write_read_data(uint8_t chip_sel, uint8_t addr, uint8_t dat);

#endif

