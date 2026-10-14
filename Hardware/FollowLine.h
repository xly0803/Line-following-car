#ifndef __FOLLOWLINE_H__
#define __FOLLOWLINE_H__

#include <stdint.h>
#include "PID.h"

/* ==================== 可调参数 ==================== */
#define LINE_BASE_SPEED     13.0f
#define LINE_LOST_TIMEOUT   50
#define LINE_LOST_ERROR     999.0f

/* ==================== 全局变量 ==================== */
extern const int8_t SensorWeight[8];
extern PID_t   TurnPID;
extern int16_t TurnPWM;
extern uint8_t LostCnt;
extern int8_t  LastDir;
extern uint8_t MotorFlag;
/* ==================== 接口函数 ==================== */
float Line_GetError(uint8_t data);
void  Line_Follow(uint8_t data);
void  LineFollow_Init(void);

#endif