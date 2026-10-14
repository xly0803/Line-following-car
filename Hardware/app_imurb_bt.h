#ifndef APP_IMURB_BT_H
#define APP_IMURB_BT_H

#include "zf_common_headfile.h"
#include "qmath.h"
#include "task.h"
#include "app_led_bt.h"
#include "zf_driver_delay.h"
#include "app_menu_bt.h"
#include "app_flash_bt.h"

// 如果没有定义M_PI，则自己定义
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// 定义校准状态
typedef enum {
    CALIB_NOT_STARTED = 0,  // 未开始
    CALIB_IN_PROGRESS,      // 正在进行
    CALIB_COMPLETED         // 已完成
} CalibState;

// 四元数结构体
typedef struct {
    float q0;  // 实部
    float q1;  // i分量
    float q2;  // j分量
    float q3;  // k分量
} Quaternion;

// 全局变量声明
extern float gx_ave;  // 陀螺仪X轴零漂平均值
extern float gy_ave;  // 陀螺仪Y轴零漂平均值
extern float gz_ave;  // 陀螺仪Z轴零漂平均值

// 函数声明
// @ 简介：初始化IMU模块，设置初始状态
// @ 输入：无
// @ 输出：无
// @ 注意：此函数会尝试从Flash加载历史校准数据
void App_Imurb_Init(void);

// @ 简介：外部调用，触发开始校准流程
// @ 输入：无
// @ 输出：无
// @ 注意：调用此函数后，需保持车模静止，等待校准完成
void App_Imurb_Calib_Ready(void);

// @ 简介：执行IMU校准流程
// @ 输入：无
// @ 输出：无
// @ 注意：可选择使用Flash数据或重新校准
void App_Imurb_Calib_Task(void);

// @ 简介：用当前加速度计数据初始化互补滤波的积分器状态
// @ 输入：无
// @ 输出：无
// @ 注意：必须在车模静止状态下调用，且校准完成后调用一次
void App_Imurb_InitFilterState(void);

// @ 简介：手动触发互补滤波状态初始化
// @ 输入：无
// @ 输出：uint8_t - 1表示成功，0表示失败
// @ 注意：用于在校准后手动初始化，确保姿态解算从正确起点开始
uint8_t App_Imurb_ManualInitFilter(void);

// @ 简介：获取互补滤波状态是否已初始化
// @ 输入：无
// @ 输出：uint8_t - 1表示已初始化，0表示未初始化
uint8_t App_Imurb_IsFilterInitialized(void);

// @ 简介：获取IMU校准状态
// @ 输入：无
// @ 输出：CalibState - 校准状态枚举值
CalibState App_Imurb_GetCalibState(void);

// @ 简介：从Flash加载历史校准数据
// @ 输入：无
// @ 输出：uint8_t - 1表示加载成功，0表示加载失败
// @ 注意：开机时自动调用，失败表示Flash中没有有效校准数据
uint8_t App_Imurb_LoadCalibFromFlash(void);

// @ 简介：保存当前校准数据到Flash
// @ 输入：无
// @ 输出：uint8_t - 1表示保存成功，0表示保存失败
uint8_t App_Imurb_SaveCalibToFlash(void);

// @ 简介：读取并处理IMU原始数据，包括去零漂
// @ 输入：无
// @ 输出：无，更新全局变量ax/ay/az/gx/gy/gz
// @ 注意：此函数不进行姿态解算，仅提供处理后的传感器数据
void App_Imurb_Update(void);

void App_Imurb_Gyro_Update(void);
void App_Imurb_Acc_Update(void);

// @ 简介：执行姿态解算，融合陀螺仪和加速度计数据
// @ 输入：无
// @ 输出：无，更新全局变量yaw/pitch/roll
// @ 注意：1. 基于四元数的姿态解算算法
//         2. 需在校准完成后调用，且必须先初始化滤波状态
void App_Imurb_Proc(void);
void App_Imurb_First(void);
void App_Imurb_SetYaw(float yaw_deg);
// 数据获取函数
float App_Imurb_Get_Ax(void);
float App_Imurb_Get_Ay(void);
float App_Imurb_Get_Az(void);
float App_Imurb_Get_Gx(void);
float App_Imurb_Get_Gy(void);
float App_Imurb_Get_Gz(void);
float App_Imurb_Yaw(void);
float App_Imurb_Yaw_Rise(void);
float App_Imurb_Pitch(void);
float App_Imurb_Roll(void);
float App_Imurb_Gx_Ave(void);
float App_Imurb_Gy_Ave(void);
float App_Imurb_Gz_Ave(void);

#endif