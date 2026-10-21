#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

extern uint8_t enable_group1_irq;
extern volatile uint8_t imu_data_ready;
extern volatile uint32_t int1_cnt;
void Interrupt_Init(void);

#endif