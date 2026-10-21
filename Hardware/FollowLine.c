#include "FollowLine.h"

/* ==================== 传感器权重表 ==================== */
// bit7 在最左，bit0 在最右；方向反了就反转此数组
const int8_t SensorWeight[8] = {
    -7, -5, -3, -1, +1, +3, +5, +7
};
uint8_t sensorData;
/* ==================== 转向PID（外环） ==================== */
// 注意：循迹外环只用 Kp + Kd，Ki = 0
PID_t TurnPID = {
    .Kp = 6.0f,
    .Ki = 0.0f,
    .Kd = 3.0f,
    .OutMax = 40.0f,
    .OutMin = -40.0f,
    .OutOffset = 0.0f,
};

/* ==================== 全局变量 ==================== */
int16_t TurnPWM = 0;
uint16_t LostCnt = 0;
int8_t  LastDir = 0;
float Lastyaw = 0;
uint8_t AngleFlag = 1;
uint8_t LostFlag = 0;
/* ==================== 初始化 ==================== */
void LineFollow_Init(void)
{
    TurnPWM = 0;
    LostCnt = 0;
    LastDir = 0;

    PID_Init(&TurnPID);
}

/* ==================== 读取并解析循迹偏差 ==================== */
float Line_GetError(uint8_t data)
{
    int sum = 0;
    int weightSum = 0;

    for (int i = 0; i < 8; i++)
    {
        if (data & (1 << i))   // 1 = 检测到黑线
        {
            sum += SensorWeight[i];
            weightSum++;
        }
        
    }

    if (weightSum == 0 || weightSum == 8)
    {
        LostFlag = 1;
        if(AngleFlag == 1)
        {
            Lastyaw = yaw;
            AngleFlag = 0;
        }
        
        return LINE_LOST_ERROR;
    }
    else 
    {
        LostFlag = 0;
        AngleFlag = 1;
    }
    return (float)sum / weightSum;
}

/* ==================== 循迹转向PID计算 ==================== */
void Line_Follow(uint8_t data)
{
    float error = Line_GetError(data);

    // ---- 丢线处理 ----
    if (LostFlag)
    {
        LostCnt++;
        Alpha = 1;
        if (LostCnt > LINE_LOST_TIMEOUT)
        {
            TurnPWM = 0;          // 长时间丢线：转向归零，由 main 决定停车
        }
        else 
        {
            TurnPWM = 0;
        }
    }
    else 
    {
        Alpha = 0.2;
        LostCnt = 0;

        // ---- 借用 PID_Update 的标准框架 ----
        // 循迹本质就是让偏差 error 收敛到 0
        TurnPID.Target = 0.0f;
        TurnPID.Actual = error;
        PID_Update(&TurnPID);         // 内部已算好 P + I + D、死区补偿、限幅
        TurnPWM = (int16_t)TurnPID.Out;

        // 记录方向，用于丢线记忆
        if (error > 0.5f)       LastDir =  1;
        else if (error < -0.5f) LastDir = -1;

    }
}