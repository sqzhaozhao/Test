#ifdef 	__GAS_SENSOR_H
#define __GAS_SENSOR_H

#include "sys.h"



void change_senser_addr(uint8_t addr);
void read_sensor_ppm(uint8_t addr);
void read_sensor_resolution(uint8_t addr);



#endif

