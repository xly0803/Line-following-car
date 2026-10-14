#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

void Motor_Init(void);
void Motor_SetPWM(uint8_t n, int16_t PWM);

#endif
