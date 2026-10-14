#include "ti_msp_dl_config.h"

// PWM 初始化（大部分已在 SysConfig 生成的代码里完成）
void PWM_Init(void)
{
    // 启动 PWM 定时器
    DL_TimerG_startCounter(PWM_MOTOR_INST);
}

// 设置通道 1 占空比
void PWM_SetCompare1(uint16_t Compare)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, Compare, DL_TIMER_CC_0_INDEX);
}

// 设置通道 2 占空比
void PWM_SetCompare2(uint16_t Compare)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, Compare, DL_TIMER_CC_1_INDEX);
}