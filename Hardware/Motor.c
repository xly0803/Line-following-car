// #include "ti_msp_dl_config.h"

// // 电机初始化（GPIO 和 PWM 已在 SysConfig 里配好）
// void Motor_Init(void)
// {
//     // 如果 SysConfig 里 PWM 没勾选 Start Timer，需要手动启动
//     DL_TimerG_startCounter(PWM_MOTOR_INST);
// }

// // 设置电机转速
// // n: 1=左轮, 2=右轮
// // PWM: -100 ~ +100，正负代表方向
// void Motor_SetPWM(uint8_t n, int16_t PWM)
// {
//     if (n == 1) {
//         // 左轮：AIN1、AIN2
//         if (PWM >= 0) {
//             DL_GPIO_setPins(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN);
//             DL_GPIO_clearPins(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN);
//             DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, PWM, DL_TIMER_CC_0_INDEX);
//         } else {
//             DL_GPIO_setPins(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN);
//             DL_GPIO_clearPins(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN);
//             DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, -PWM, DL_TIMER_CC_0_INDEX);
//         }
//     } else if (n == 2) {
//         // 右轮：BIN1、BIN2
//         if (PWM >= 0) {
//             DL_GPIO_setPins(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN);
//             DL_GPIO_clearPins(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN);
//             DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, PWM, DL_TIMER_CC_1_INDEX);
//         } else {
//             DL_GPIO_setPins(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN);
//             DL_GPIO_clearPins(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN);
//             DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST, -PWM, DL_TIMER_CC_1_INDEX);
//         }
//     }
// }

#include "ti_msp_dl_config.h"

// 把速度(0~100)换算成 CC 寄存器值。
// 本工程的 PWM 是“加载拉高、计数减到 CC 时拉低”，所以 CC 越大占空比越小。
// 为了符合直觉（0=停、100=满速），用 CC = 99 - 速度。
static uint16_t Motor_SpeedToCC(int16_t speed)
{
    int32_t s = (speed < 0) ? (-(int32_t)speed) : ((int32_t)speed); // 取绝对值
    if (s > 99) s = 99;   // 上限保护
    if (s < 0)  s = 0;
    return (uint16_t)(99 - s);
}

// 电机初始化（GPIO 和 PWM 已在 SysConfig 里配好）
void Motor_Init(void)
{
    DL_TimerG_startCounter(PWM_MOTOR_INST);
}

// 设置电机转速
// n: 1=左轮, 2=右轮
// PWM: -100 ~ +100，正负代表方向，0 = 停
void Motor_SetPWM(uint8_t n, int16_t PWM)
{
    if (n == 1) {
        // 左轮：AIN1、AIN2
        if (PWM >= 0) {
            DL_GPIO_setPins(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN);
            DL_GPIO_clearPins(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN);
        } else {
            DL_GPIO_setPins(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN);
            DL_GPIO_clearPins(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN);

        }
        DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST,
            Motor_SpeedToCC(PWM), DL_TIMER_CC_0_INDEX);
    } else if (n == 2) {
        // 右轮：BIN1、BIN2
        if (PWM >= 0) {
            DL_GPIO_setPins(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN);
            DL_GPIO_clearPins(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN);
        } else {
            DL_GPIO_setPins(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN);
            DL_GPIO_clearPins(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN);
        }
        DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST,
            Motor_SpeedToCC(PWM), DL_TIMER_CC_1_INDEX);
    }
}