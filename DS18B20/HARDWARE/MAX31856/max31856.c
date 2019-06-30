//---------------------------------------------------
//���ļ�Ϊ�ȵ�żת��оƬMAX31856��������
//�ӿڣ�SPI,
//			CS1--C6  	SCLK--C7
//			MOSI--C8  MISO--C9
//ע�⣺�ȵ�ż��������ȶˣ��в��ɷ��ӣ�����������ݲ���
//---------------------------------------------------
#include "max31856.h"
#include "spi.h"
#include "sys.h"
#include "delay.h"



//MAX31856��ʼ������
void max31856_config(uint8_t sensor_type)
{
	SPI1_Init();//SPI�ܽų�ʼ��
	

	//��ʼ�����Ĵ���
	write_read_max31856(W_CRO_REG_ADDR 		, 0x80);//0xA6
	write_read_max31856(W_CR1_REG_ADDR    , TC_TYPE_K);//03--K���ȵ�ż��06-S���ȵ�ż
	write_read_max31856(W_MASK_REG_ADDR   , 0xFF);
	write_read_max31856(W_CJHF_REG_ADDR   , 0x7F);
	write_read_max31856(W_CJLF_REG_ADDR   , 0xC0);
	write_read_max31856(W_LTHFTH_REG_ADDR , 0x7F);
	write_read_max31856(W_LTHFTL_REG_ADDR , 0xFF);
	write_read_max31856(W_LTLFTH_REG_ADDR , 0x80);
	write_read_max31856(W_LTLFT_REG_ADDR  , 0x00);
	write_read_max31856(W_CJTO_REG_ADDR   , 0x00);
	write_read_max31856(W_CJTH_REG_ADDR   , 0x00);
	write_read_max31856(W_CJTL_REG_ADDR   , 0x00);
	
	delay_ms(1);
	
}

//д/���Ĵ���
//addr--�Ĵ�����ַ
//datҪд������ݣ������������Ϊ����(��α�ֽڱ�ʾ��NULL_BYTE)
uint8_t write_read_max31856(uint8_t addr, uint8_t dat)
{
	return write_read_data(0, addr, dat);
}

//�����Ի�����¶�����
//�˴�ֻ������������
float read_max31856(void)
{
	float temp = 0;
	uint8_t dat1,dat2,dat3 ,i;

	dat1 = write_read_max31856(R_LTCBH_REG_ADDR, NULL_BYTE);//sign 2^10~2^4
	delay_ms(1);

	dat2 = write_read_max31856(R_LTCBM_REG_ADDR, NULL_BYTE);//2^3~2^(-4)
	delay_ms(1);
	
	dat3 = write_read_max31856(R_LTCBL_REG_ADDR, NULL_BYTE);//2^(-5)~2^(-7) bit4 bit3 bit2 bit1 bit0
	delay_ms(1);
	
	temp = (dat1<<4|((dat2>>4) & 0x0f)) + (((dat2&0x0f)<<3) | (dat3>>5))/128.0;//��������������
	
	return (temp);
}

