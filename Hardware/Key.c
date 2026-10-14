#include "ti_msp_dl_config.h"
#include "Delay.h"

uint8_t Key_Num;


/* 按键状态：按下为 0（上拉输入） */
static uint8_t Key_GetState(void)
{
    /* DL_GPIO_readPins 返回掩码，某位为 0 表示该引脚为低电平（按下） */
    if (DL_GPIO_readPins(KEY_PORT, KEY_FLAG_PIN) == 0)
    {
        return 1;          /* 按键 1 按下 */
    }
    return 0;
}

uint8_t Key_GetNum(void)
{
    uint8_t Temp;
    if (Key_Num)
    {
        Temp = Key_Num;
        Key_Num = 0;
        return Temp;
    }
    return 0;
}

void Key_Tick(void)
{
    static uint8_t Count;
    static uint8_t CurrState, PrevState;

    Count++;
    if (Count >= 20)
    {
        Count = 0;

        PrevState = CurrState;
        CurrState = Key_GetState();

        /* 检测"释放"边沿（从按下变松开）时记录按键号 */
        if (CurrState == 0 && PrevState != 0)
        {
            Key_Num = PrevState;
        }
    }
}