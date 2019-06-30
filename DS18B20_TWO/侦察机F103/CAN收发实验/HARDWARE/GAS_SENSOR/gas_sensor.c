//---------------------------------------------------
//���ļ�Ϊ���ڸ����ؿƼ������崫����485ͨѶЭ���ļ�
//ͨ�Ų���: ��ʼλ--1 ����λ--8 ֹͣλ--1 У��--�� ������--9600
//�������ݸ�ʽ: ͨѶ��ַ ��ȡ������ ��ʼ��ַH ��ʼ��ַL ����H ����L CRC_L CRC_H
//���޸�ID�ţ���������ȡ��������ֵ���ֱ���
//�޸�ID��ʱ����Ҫ�Ƚ��������޸ġ�
//ע��������ȡ��Ϊ�ٷֱȣ�����ֵ�Ŵ���100�������ȡ��12800��ʵ��Ũ��Ϊ28%����ֱ����޹�
//		������������ȡ����PPMֵ����Ҫͨ����������ֵ��ֱ����������PPMֵ�����ȡ����ֵΪ1000
//	���ֱ���Ϊ1����ʵ�ʸ�����Ũ��Ϊ1000/10=100ppm,����ֱ���Ϊ0������ֵΪ1000���ֱ���Ϊ2��
//		��ֵΪ10���ֱ���Ϊ3��4���Ƽ��㡣
//---------------------------------------------------

#include "gas_sensor.h"
#include "rs485.h"	 
#include "sys.h"
#include "delay.h"

//�޸�ͨѶ��ַ
void change_senser_addr(uint8_t addr)
{
	uint16_t crc16_dat;
	uint8_t rs485_unlock[8] = {0x00,0x06,0x00,0x11,0x00,0x18,0xD8,0x14};//����
	uint8_t rs485_addr[8] =   {0x00,0x06,0x00,0x13,0x00,0x01,0x39,0xDF};//�޸�ID��

	
	RS485_Send_Data(rs485_unlock,8);//����
	delay_ms(10);
	
	rs485_addr[5] = addr;
	crc16_dat = Crc16(rs485_addr, 6);
	rs485_addr[6] = crc16_dat & 0xff;
	rs485_addr[7] = (crc16_dat>>8) & 0xff;
	RS485_Send_Data(rs485_addr,8);//�޸�ͨѶ��ַ(ID)
	delay_ms(10);
}

//��ȡ��������ֵ(PPM)
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

//��ȡ�������ֱ���
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



