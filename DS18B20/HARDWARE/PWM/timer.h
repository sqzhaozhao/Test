#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

typedef enum 
{
	PWM_CH1,
	PWM_CH2,
	PWM_CH3,
	PWM_CH4
}PWM_CHAN;	


void tim2_count_init(u16 arr,u16 psc);
void tim3_pwm_init(u16 arr,u16 psc);
void set_pwm(PWM_CHAN ch, uint16_t Compare);

#endif
