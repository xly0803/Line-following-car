#include "ti_msp_dl_config.h"
#include "interrupt.h"
#include "clock.h"

uint8_t enable_group1_irq = 0;
volatile uint8_t imu_data_ready = 0;   // 新增
volatile uint32_t int1_cnt = 0;

void Interrupt_Init(void)
{

    NVIC_EnableIRQ(GPIO_IMU660RB_INT_IRQN); 
}


void SysTick_Handler(void)
{
    tick_ms++;
}

void GROUP1_IRQHandler(void)
{ 
    int1_cnt++;
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
#if defined GPIO_MULTIPLE_GPIOB_INT_IIDX
        case GPIO_MULTIPLE_GPIOB_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIOB))
            {
#if (defined GPIO_IMU660RB_PIN_IMU660RB_INT1_PORT) && (GPIO_IMU660RB_PIN_IMU660RB_INT1_PORT == GPIOB)
                case GPIO_IMU660RB_PIN_IMU660RB_INT1_IIDX:
                    imu_data_ready = 1;   // 只置标志，立刻返回（微秒级）
                   
                    break;
#endif
                default:
                    break;
            }
            break;
#endif

#if defined GPIO_IMU660RB_INT_IIDX
        case GPIO_IMU660RB_INT_IIDX:
            imu_data_ready = 1;
            break;
#endif

        default:
            break;
    }
}


