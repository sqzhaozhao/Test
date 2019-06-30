//---------------------------------------------------
//本文件为深圳富凯特科技的气体传感器485通讯协议文件
//通信参数: 起始位--1 数据位--8 停止位--1 校验--无 波特率--9600
//发送数据格式: 通讯地址 读取命令字 起始地址H 起始地址L 长度H 长度L CRC_L CRC_H
//可修改ID号，解锁，读取传感器数值及分辨率
//修改ID号时，需要先解锁，再修改。
//注：氧气读取的为百分比，且数值放大了100倍，如读取的12800，实际浓度为28%，与分辨率无关
//		其它传感器读取的是PPM值，需要通过传感器数值与分辨率来换算成PPM值，如读取的数值为1000
//	分辨率为1，则实际该气体浓度为1000/10=100ppm,如果分辨率为0，则数值为1000；分辨率为2则
//		数值为10；分辨率为3，4类似计算。
//---------------------------------------------------

#include "gas_sensor.h"
#include "rs485.h"	 
#include "sys.h"
#include "delay.h"

//修改通讯地址
void change_senser_addr(uint8_t addr)
{
	uint16_t crc16_dat;
	uint8_t rs485_unlock[8] = {0x00,0x06,0x00,0x11,0x00,0x18,0xD8,0x14};//解锁
	uint8_t rs485_addr[8] =   {0x00,0x06,0x00,0x13,0x00,0x01,0x39,0xDF};//修改ID号

	
	RS485_Send_Data(rs485_unlock,8);//解锁
	delay_ms(10);
	
	rs485_addr[5] = addr;
	crc16_dat = Crc16(rs485_addr, 6);
	rs485_addr[6] = crc16_dat & 0xff;
	rs485_addr[7] = (crc16_dat>>8) & 0xff;
	RS485_Send_Data(rs485_addr,8);//修改通讯地址(ID)
	delay_ms(10);
}

//读取传感器数值(PPM)
void read_sensor_ppm(uint8_t addr)
{
	uint16_t crc16_dat;
	uint8_t rs485_read_id[8] = {0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};
	
	rs485_read_id[0] = addr;
	crc16_dat = Crc16(rs485_read_id, 6);
	rs485_read_id[6] = crc16_dat & 0xff;
	rs485_read_id[7] = (crc16_dat>>8) & 0xff;
	RS485_Send_Data(rs485_read_id,8);
//	delay_ms(2);
}

//读取传感器分辨率
void read_sensor_resolution(uint8_t addr)
{
	uint16_t crc16_dat;
	uint8_t rs485_read_res[8] = {0x01,0x03,0x00,0x1E,0x00,0x01,0xE4,0x0C};
	
	rs485_read_res[0] = addr;
	crc16_dat = Crc16(rs485_read_res, 6);
	rs485_read_res[6] = crc16_dat & 0xff;
	rs485_read_res[7] = (crc16_dat>>8) & 0xff;
	RS485_Send_Data(rs485_read_res,8);
//	delay_ms(2);
}



