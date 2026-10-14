#include "ti_msp_dl_config.h"

volatile int32_t encoder2_count = 0;
volatile uint32_t irq_count = 0;

void Encoder_Init(void)
{
    // 启动两个定时器
    DL_TimerG_startCounter(QEI_0_INST);       // 编码器1 QEI（PB6/PB7）
    DL_TimerG_startCounter(CAPTURE_0_INST);   // 编码器2 Capture（PB26/PB27）

    // 使能捕获中断（通道0，A相）
    DL_TimerG_enableInterrupt(CAPTURE_0_INST, DL_TIMERG_INTERRUPT_CC0_UP_EVENT);
    NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);
}

// 编码器2 捕获中断（A相 PB26 上升沿触发）
void CAPTURE_0_INST_IRQHandler(void)
{
    irq_count++;
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_0_INST)) {
        case DL_TIMER_IIDX_CC0_UP:
        {
            // 读 B 相（PB27）电平判方向
            uint8_t phb = (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_27) != 0) ? 1 : 0;
            if (phb == 0) {
                encoder2_count++;
            } else {
                encoder2_count--;
            }
            break;
        }
        default:
            break;
    }
}

int32_t Encoder_Peek(uint8_t n)
{
    if (n == 2) {
        return (int16_t)DL_Timer_getTimerCount(QEI_0_INST);
    } else if (n == 1) {
        return encoder2_count;   // volatile 变量，读一下即可
    }
    return 0;
}

int32_t Encoder_Get(uint8_t n)
{
    int32_t temp;

    if (n == 2) {
        // 编码器2：TIMG8 QEI 计数器
        temp = (int16_t)DL_Timer_getTimerCount(QEI_0_INST);
        DL_Timer_setTimerCount(QEI_0_INST, 0);
        return temp;
    } else if (n == 1) {
        // 编码器1：软件计数
        __disable_irq();
        temp = encoder2_count;
        encoder2_count = 0;
        __enable_irq();
        return temp*4;
    }
    return 0;
}