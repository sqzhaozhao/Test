//---------------------------------------------------
//���ļ�ΪPT100�ȵ���ת��оƬMAX31865��������
//�ӿڣ�SPI,
//			CS1--C10  	SCLK--C7
//			MOSI--C8  MISO--C9
//ע�⣺�����ƽ�F+��F-�ֱ���RTD+��RTD-����
//---------------------------------------------------
#include "max31865.h"
#include "spi.h"
#include "sys.h"
#include "delay.h"
#include "math.h"

double pt100_a = 3.9083/1000;
double pt100_b = -5.775/10000000;
double pt100_a_pow = 0.0;

//MAX31856��ʼ������
void max31865_config(void)
{
	SPI1_Init();//SPI�ܽų�ʼ��
	
	//��ʼ�����Ĵ���
	
	//����Ϊ�����Զ��¶�ת��
	write_read_max31865(W_MAX31856_CONFIG_REG, MAX31856_CONFIG_REG_DATA);//

	
	delay_ms(1);
	
}

//д/���Ĵ���
//addr--�Ĵ�����ַ
//datҪд������ݣ������������Ϊ����(��α�ֽڱ�ʾ��NULL_BYTE)
//ע����������dif = sqrt(pow(a) - 4(1 - R(t)/100)),T = (-a + sqrt(dif))/ 2b;ֻ�ܼ���0�����ϵ��¶ȣ�
//0�����µ��¶���Ҫtemp = adc_data * 1.075 /32 -256���бƽ�����
uint8_t write_read_max31865(uint8_t addr, uint8_t dat)
{
	return write_read_data(1, addr, dat);
}

//�����Ի�����¶�����
//�˴�ֻ������������
double read_max31865(void)
{
	double rtd_dat = 0.0;
	double temp = 0.0;
	uint8_t dat1,dat2,dat3 ,i;
	double dat_dif = 0.0;

	dat1 = write_read_max31865(0x01, 0xAA);//ע��AAΪ��Ч�ֽڣ�2^14-2^7
	dat2 = write_read_max31865(0x02, 0xAA);//2^6-2^0 X
	
	
	rtd_dat = (((dat1<<8)|dat2)>>1) * 430 /32768.0;//�����RTDʵ����ֵ
	
//	//����0�ȷ�Χͨ����׼��ʽ����,dif = sqrt(pow(a) - 4(1 - R(t)/100)),T = (-a + sqrt(dif))/ 2b;
	if(rtd_dat>100)
	{
		dat_dif = pow(pt100_a, 2) - 4 * pt100_b * (1 - rtd_dat/100);
		temp = (-pt100_a + sqrt(dat_dif))/ (2 * pt100_b);
	}
	else//�����ڰ��� temp = adc_data * 1.075 /32 -256���бƽ�����
	{
		temp = (((dat1<<8)|dat2)>>1) * 1.075 / 32 - 256;
	}
	return (temp);
}

