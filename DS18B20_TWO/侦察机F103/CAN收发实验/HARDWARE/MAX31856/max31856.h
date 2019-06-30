#ifndef __MAX31856_H
#define	__MAX31856_H

#include "sys.h"

//伪字节
#define	NULL_BYTE 0xAA

//写操作时寄存器地址
#define	W_CRO_REG_ADDR 				0x80
#define	W_CR1_REG_ADDR        0x81
#define	W_MASK_REG_ADDR       0x82
#define	W_CJHF_REG_ADDR       0x83
#define	W_CJLF_REG_ADDR       0x84
#define	W_LTHFTH_REG_ADDR     0x85
#define	W_LTHFTL_REG_ADDR     0x86
#define	W_LTLFTH_REG_ADDR     0x87
#define	W_LTLFT_REG_ADDR      0x88
#define	W_CJTO_REG_ADDR       0x89
#define	W_CJTH_REG_ADDR       0x8A
#define	W_CJTL_REG_ADDR       0x8B


//读操作时寄存器地址
#define	R_CRO_REG_ADDR 				0x00
#define	R_CR1_REG_ADDR        0x01
#define	R_MASK_REG_ADDR       0x02
#define	R_CJHF_REG_ADDR       0x03
#define	R_CJLF_REG_ADDR       0x04
#define	R_LTHFTH_REG_ADDR     0x05
#define	R_LTHFTL_REG_ADDR     0x06
#define	R_LTLFTH_REG_ADDR     0x07
#define	R_LTLFT_REG_ADDR      0x08
#define	R_CJTO_REG_ADDR       0x09
#define	R_CJTH_REG_ADDR       0x0A
#define	R_CJTL_REG_ADDR       0x0B

//线性化的TC温度地址
#define	R_LTCBH_REG_ADDR			0x0C
#define	R_LTCBM_REG_ADDR      0x0D
#define	R_LTCBL_REG_ADDR      0x0E

//故障寄存器地址
#define	R_SR_REG_ADDR         0x0F


//热电偶类型
#define	TC_TYPE_B         0x00
#define	TC_TYPE_E         0x01
#define	TC_TYPE_J         0x02
#define	TC_TYPE_K         0x03
#define	TC_TYPE_N         0x04
#define	TC_TYPE_R         0x05
#define	TC_TYPE_S         0x06
#define	TC_TYPE_T         0x07


void max31856_config(uint8_t sensor_type);
float read_max31856(void);
uint8_t write_read_max31856(uint8_t addr, uint8_t dat);






#endif



