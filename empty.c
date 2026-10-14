/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "OLED_Data.h"
#include "OLED.h"
#include "Delay.h"
#include "MPU6050.h"
#include <math.h>
#include "PWM.h"
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"
#include "FollowLine.h"
#include "Key.h"
#include "app_imurb_bt.h"

uint8_t Read_8Road_Sensor(void);
int16_t AX,AY,AZ,GX,GY,GZ;
extern uint8_t sensorData;
float Angle;
float Error;

int16_t LeftPWM,RightPWM;
int16_t AvePWM, DifPWM;

uint8_t KeyNum,KeyFlag;

float LeftSpeed, RightSpeed;
float AveSpeed,DifSpeed;

PID_t SpeedPID = {
    .Kp = 1.0,
    .Ki = 0.02,
    .Kd = 0,
    .OutMax = 50,
    .OutMin = -50,
};

// 差速环：让 DifSpeed 趋近 0，左右轮速度一致
PID_t DifPID = {
    .Kp = 1.2,
    .Ki = 0.05,
    .Kd = 0,
    .OutMax = 30,
    .OutMin = -30,
};



void TIMER_7_INST_IRQHandler(void);

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_7_INST_INT_IRQN);
    
    // 启动定时器计数
    DL_TimerG_startCounter(TIMER_7_INST);
    Motor_Init();
    OLED_Init();
    Encoder_Init();
    MPU6050_Init();
    LineFollow_Init();
    OLED_Update(); 
    while (1) {
        
        // OLED_Printf(0, 0, OLED_6X8, "AX:%d", AX);
        // OLED_Printf(0, 8, OLED_6X8, "GX:%d", GX);
        // OLED_Printf(0, 24, OLED_6X8, "A:%05.1f", AngleAcc);
		// OLED_Printf(0, 32, OLED_6X8, "A:%05.1f", AngleGyro);
		// OLED_Printf(0, 40, OLED_6X8, "A:%05.1f", Angle);
        KeyNum = Key_GetNum();
        if(KeyNum == 1)
        {
            if(KeyFlag == 0)
            {
                KeyFlag = 1;
            }
            else 
            {
                KeyFlag = 0;
            }
        }
        OLED_ShowBinNum(0, 16, sensorData,8, OLED_6X8);
		OLED_Printf(0, 0, OLED_6X8, "outLef:%d", LeftPWM);
		OLED_Printf(0, 8, OLED_6X8, "outRig:%d", RightPWM);
        OLED_Printf(0, 16, OLED_6X8, "actLef:%04.1f", LeftSpeed);
		OLED_Printf(0, 24, OLED_6X8, "actRig:%04.1f", RightSpeed);
        OLED_Printf(0, 32, OLED_6X8, "outAve:%d", AvePWM);
		OLED_Printf(0, 40, OLED_6X8, "outDif:%d", TurnPWM);
        OLED_Printf(0, 56, OLED_6X8, "flag:%d",KeyFlag);
        OLED_Printf(0, 40, OLED_6X8, "tgt:%03.1f", LINE_BASE_SPEED);
        OLED_Printf(0, 48, OLED_6X8, "difSpd:%04.1f", DifSpeed);
        OLED_Printf(64, 56, OLED_6X8, "difPWM:%d", DifPWM);
                OLED_Printf(64, 0, OLED_6X8, "rigpwm:%d", RightPWM);
        OLED_Printf(64, 8, OLED_6X8, "%03.1f", Error);
        OLED_Update(); 
        

    }
}

uint8_t Read_8Road_Sensor(void)
{
    return (uint8_t)((DL_GPIO_readPins(GPIOB, 0x0001FE00) >> 9) & 0xFF);
}

void TIMER_7_INST_IRQHandler(void)
{
    static uint16_t Count0,Count1;
    switch (DL_TimerG_getPendingInterrupt(TIMER_7_INST)) {
        case DL_TIMER_IIDX_ZERO:
            Key_Tick();

            Count0++;
            if(Count0 >= 10)
            {
                sensorData = Read_8Road_Sensor();

                MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
                Angle += GZ / 32768.0 * 2000.0 * 0.1;//代码每100ms执行一次 故*0.1

                Count0 = 0;
            }

            Count1++;
            if(Count1 >= 5)

            {
                if(KeyFlag == 1)
                {
                    LeftSpeed  = -Encoder_Get(1) / 44.0 / 0.05;
                    RightSpeed = -Encoder_Get(2) / 44.0 / 0.05;
                    AveSpeed   = (LeftSpeed + RightSpeed) / 2;
                    DifSpeed = LeftSpeed - RightSpeed;
                    // ---- 1. 循迹外环：算出 TurnPWM ----
                    Line_Follow(sensorData);
                    Error = Line_GetError(sensorData);
                    
                    // ---- 2. 速度内环：平均速度跟踪基础速度 ----
                    SpeedPID.Target = LINE_BASE_SPEED;
                    SpeedPID.Actual = AveSpeed;
                    PID_Update(&SpeedPID);
                    AvePWM = (int16_t)SpeedPID.Out;

                    //差速环pid
                    DifPID.Target = 0;
                    DifPID.Actual = DifSpeed;
                    PID_Update(&DifPID);
                    DifPWM = (int16_t)DifPID.Out;

                    // LeftPWM  = AvePWM + DifPWM;
                    // RightPWM = AvePWM - DifPWM;

                    // Motor_SetPWM(1, LeftPWM);
                    // Motor_SetPWM(2, RightPWM);


                    if (LostCnt > LINE_LOST_TIMEOUT)
                    {
                        /* 长时间丢线：直接停车，绕过所有叠加和限幅 */
                        Motor_SetPWM(1, 0);
                        Motor_SetPWM(2, 0);
                        LeftPWM  = 0;
                        RightPWM = 0;
                    }
                    else
                    {
                        /* 正常：叠加 + 限幅 */
                        LeftPWM  = AvePWM + TurnPWM;
                        RightPWM = AvePWM - TurnPWM;

                    float absErr = (Error > 0) ? Error : -Error;
                    if (absErr <= 0.5f)
                    {
                        LeftPWM  += DifPWM / 2;
                        RightPWM -= DifPWM / 2;
                    }                       
                    if (LeftPWM  > 90) LeftPWM  = 90;
                    if (LeftPWM  < 10) LeftPWM  = 10;
                    if (RightPWM > 90) RightPWM = 90;
                    if (RightPWM < 10) RightPWM = 10;

                    Motor_SetPWM(1, LeftPWM);
                    Motor_SetPWM(2, RightPWM);
                    }
                    

                }
                else if(KeyFlag == 0)
                {
                    LeftPWM = 0;
                    RightPWM = 0;
                    Motor_SetPWM(1, LeftPWM);
                    Motor_SetPWM(2, RightPWM);
                }
                Count1 = 0;
            }
            break;
        default:
            break;
    }
}