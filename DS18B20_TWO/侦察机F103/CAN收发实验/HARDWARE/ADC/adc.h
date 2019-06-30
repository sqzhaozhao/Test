#ifndef _ADC_H
#define _ADC_H

//#define GPIOA_IDR ( GPIOA_BASE + 0X08)            
//#define GPIOA_ODR ( GPIOA_BASE + 0X0C)
//#define GPIOB_IDR ( GPIOB_BASE + 0X08) 
//#define GPIOB_ODR ( GPIOB_BASE + 0X0C)
//#define GPIOC_IDR ( GPIOC_BASE + 0X08)
//#define GPIOC_ODR ( GPIOC_BASE + 0X0C)
//#define GPIOD_IDR ( GPIOD_BASE + 0X08)
//#define GPIOD_ODR ( GPIOD_BASE + 0X0C) 
//#define BitBound(Addr,BitNum) (*((volatile unsigned long*)((Addr&0xF0000000)+0X2000000+((Addr&0xFFFFF)<<5)+(BitNum<<2))))
//#define PAOut(n)       BitBound(GPIOA_ODR,n)
//#define PAIn(n)        BitBound(GPIOA_IDR,n)
//#define PBOut(n)       BitBound(GPIOB_ODR,n)
//#define PBIn(n)        BitBound(GPIOB_IDR,n)
//#define PCOut(n)       BitBound(GPIOC_ODR,n)
//#define PCIn(n)        BitBound(GPIOC_IDR,n)
//#define PDOut(n)       BitBound(GPIOD_ODR,n)
//#define PDIn(n)        BitBound(GPIOD_IDR,n)	
//#define N              100
//#define M              4

#define DTAT_NUM              100
#define CHAN_NUM             	8

void Get_ADC_Value(void);
void DMA_Configuration(void);
void ADC_Configuration(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void ADC_Config_Init(void);

extern float holder_dat;//ÔÆÌ¨Ðý×ª½Ç¶È


#endif

