//---------------------------------------------------
//该文件为PT100热电阻转化芯片MAX31865驱动代码
//接口：SPI,
//			CS1--C10  	SCLK--C7
//			MOSI--C8  MISO--C9
//注意：两线制将F+、F-分别与RTD+、RTD-相连
//---------------------------------------------------
#include "max31865.h"
#include "spi.h"
#include "sys.h"
#include "delay.h"
#include "math.h"

double pt100_a = 3.9083/1000;
double pt100_b = -5.775/10000000;
double pt100_a_pow = 0.0;

//MAX31856初始化配置
void max31865_config(void)
{
	SPI1_Init();//SPI管脚初始化
	
	//初始化各寄存器
	
	//配置为两线自动温度转化
	write_read_max31865(W_MAX31856_CONFIG_REG, MAX31856_CONFIG_REG_DATA);//

	
	delay_ms(1);
	
}

//写/读寄存器
//addr--寄存器地址
//dat要写入的数据，如果读该数据为任意(用伪字节表示，NULL_BYTE)
//注：按照下列dif = sqrt(pow(a) - 4(1 - R(t)/100)),T = (-a + sqrt(dif))/ 2b;只能计算0度以上的温度，
//0度以下的温度需要temp = adc_data * 1.075 /32 -256进行逼近运算
uint8_t write_read_max31865(uint8_t addr, uint8_t dat)
{
	return write_read_data(1, addr, dat);
}

//读线性化后的温度数据
//此处只保留整数部分
double read_max31865(void)
{
	double rtd_dat = 0.0;
	double temp = 0.0;
	uint8_t dat1,dat2,dat3 ,i;
	double dat_dif = 0.0;

	dat1 = write_read_max31865(0x01, 0xAA);//注：AA为无效字节，2^14-2^7
	dat2 = write_read_max31865(0x02, 0xAA);//2^6-2^0 X
	
	
	rtd_dat = (((dat1<<8)|dat2)>>1) * 430 /32768.0;//换算成RTD实际阻值
	
//	//超出0度范围通过标准公式计算,dif = sqrt(pow(a) - 4(1 - R(t)/100)),T = (-a + sqrt(dif))/ 2b;
	if(rtd_dat>100)
	{
		dat_dif = pow(pt100_a, 2) - 4 * pt100_b * (1 - rtd_dat/100);
		temp = (-pt100_a + sqrt(dat_dif))/ (2 * pt100_b);
	}
	else//否则在按照 temp = adc_data * 1.075 /32 -256进行逼近运算
	{
		temp = (((dat1<<8)|dat2)>>1) * 1.075 / 32 - 256;
	}
	return (temp);
}

