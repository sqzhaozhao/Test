#ifndef __MAX31865_H
#define	__MAX31865_H

#include "sys.h"

//д����ʱ���Ĵ�����ַ
#define W_MAX31856_CONFIG_REG           0x80
#define W_MAX31856_HFAULTMSB_REG        0x83
#define W_MAX31856_HFAULTLSB_REG        0x84
#define W_MAX31856_LFAULTMSB_REG        0x85
#define W_MAX31856_LFAULTLSB_REG        0x86


//������ʱ�����ݼĴ�����ַ
#define R_MAX31856_CONFIG_REG           0x00
#define R_MAX31856_RTDMSB_REG           0x01
#define R_MAX31856_RTDLSB_REG           0x02
#define R_MAX31856_HFAULTMSB_REG        0x03
#define R_MAX31856_HFAULTLSB_REG        0x04
#define R_MAX31856_LFAULTMSB_REG        0x05
#define R_MAX31856_LFAULTLSB_REG        0x06
#define R_MAX31856_FAULTSTAT_REG        0x07


//д����

//�Զ�ת������/������--E2,	������--F2
#define MAX31856_CONFIG_REG_DATA        0xE2

void max31865_config(void);
uint8_t write_read_max31865(uint8_t addr, uint8_t dat);
double read_max31865(void);




#endif