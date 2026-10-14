#include "ti_msp_dl_config.h"

/**
  * 函    数：I2C写SCL电平
  * 参    数：BitValue 要写入SCL的电平值，0或1
  * 说    明：SCL = PA11
  */
void MyI2C_W_SCL(uint8_t BitValue)
{
    if (BitValue)
        DL_GPIO_setPins(MPU_PORT, MPU_MSCL_PIN);
    else
        DL_GPIO_clearPins(MPU_PORT, MPU_MSCL_PIN);
    delay_cycles(200);   // 延时约1us @80MHz，满足I2C时序
}

/**
  * 函    数：I2C写SDA电平
  * 参    数：BitValue 要写入SDA的电平值，0或1
  * 说    明：SDA = PA10
  */
void MyI2C_W_SDA(uint8_t BitValue)
{
    if (BitValue)
        DL_GPIO_setPins(MPU_PORT, MPU_MSDA_PIN);
    else
        DL_GPIO_clearPins(MPU_PORT, MPU_MSDA_PIN);
    delay_cycles(200);
}

/**
  * 函    数：I2C读SDA电平
  * 参    数：无
  * 返 回 值：SDA当前电平，0或1
  * 说    明：SDA = PA10
  */
uint8_t MyI2C_R_SDA(void)
{
    return (DL_GPIO_readPins(MPU_PORT, MPU_MSDA_PIN) != 0) ? 1 : 0;
}

/**
  * 函    数：I2C初始化
  * 参    数：无
  * 说    明：GPIO 初始化已在 SysConfig 中完成
  *           这里只需要释放总线，让SCL和SDA都保持高电平
  */
void MyI2C_Init(void)
{
    /*
     * 关键修复：SDA 在 SysConfig 里被配成“开漏输出”，但
     * DL_GPIO_initDigitalOutputFeatures() 不会使能输入通路(INENA)，
     * 导致 DL_GPIO_readPins() 读不到从机把 SDA 拉低的电平，
     * 于是 ACK 和所有读回来的数据都是错的。
     * 这里手动打开 INENA，让 GPIO 既能开漏输出低电平、又能读回真实总线电平。
	 * 操了 debug半天找不到问题在哪 结果跟我说ccs不能同时配置输入输出 真是cs
     */
    IOMUX->SECCFG.PINCM[MPU_MSDA_IOMUX] |= IOMUX_PINCM_INENA_ENABLE;

    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/**
  * 函    数：I2C起始信号
  */
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

/**
  * 函    数：I2C停止信号
  */
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的字节
  */
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(!!(Byte & (0x80 >> i)));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

/**
  * 函    数：I2C接收一个字节
  * 返 回 值：接收到的字节
  */
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);   // 释放SDA，让从机控制
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA())
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

/**
  * 函    数：I2C发送应答位
  * 参    数：AckBit 0表示应答，1表示非应答
  */
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

/**
  * 函    数：I2C接收应答位
  * 返 回 值：0表示从机应答，1表示从机非应答
  */
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);   // 释放SDA
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}