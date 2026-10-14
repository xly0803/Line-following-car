#include "app_imurb_bt.h"

// 静态变量定义
static float ax, ay, az;           // 加速度计数据
static float gx, gy, gz;           // 陀螺仪数据
static float yaw, pitch, roll;     // 融合后的欧拉角
static float yaw_unwrapped = 0;    // 解包装后的连续航向角
static float last_yaw = 0;         // 用于解包装的上一个yaw值

// 四元数相关变量
static Quaternion quat = {1.0f, 0.0f, 0.0f, 0.0f};  // 当前姿态四元数
static uint8_t quat_initialized = 0;               // 四元数初始化标志
static uint8_t first_quat_update = 1;              // 首次四元数更新标志
static float beta = 0.1f;                          // 四元数滤波器增益
static float dt = 0.005f;                          // 采样时间，默认5ms

// 校准相关变量
static CalibState calib_state = CALIB_NOT_STARTED;  // 校准状态
static uint16_t calib_count = 0;                    // 有效采样计数
static float all_gx = 0.0f, all_gy = 0.0f, all_gz = 0.0f;  // 累加和
float gx_ave = 0.0f, gy_ave = 0.0f, gz_ave = 0.0f;  // 零漂均值

// 校准参数
#define CALIB_SAMPLE_COUNT 2000         // 目标有效采样次数
#define CALIB_MAX_ATTEMPTS 2500         // 最大尝试次数，防止卡死
#define CALIB_THRESHOLD 5.0f            // 角速度阈值：5°/s

// 校准辅助变量
static uint16_t calib_total_attempts = 0;      // 总尝试次数
static uint16_t calib_invalid_samples = 0;     // 无效样本数
static uint16_t calib_consecutive_invalid = 0; // 连续无效样本数
static uint8_t calib_first_delay = 0;          // 初始延迟计数
static uint8_t calib_external_ready = 0;       // 外部触发标志
static uint8_t use_flash_data = 0;             // 标志：是否使用Flash数据

// 常数定义
#define RAD_TO_DEG 57.2957795f
#define DEG_TO_RAD 0.0174532925f
#define GRAVITY 9.80665f

// ============================================================================
// 四元数工具函数
// ============================================================================

/**
 * @brief 四元数归一化
 */
static void quaternion_normalize(Quaternion *q) {
    float norm = sqrtf(q->q0*q->q0 + q->q1*q->q1 + q->q2*q->q2 + q->q3*q->q3);
    
    if (norm > 0.0f) {
        float inv_norm = 1.0f / norm;
        q->q0 *= inv_norm;
        q->q1 *= inv_norm;
        q->q2 *= inv_norm;
        q->q3 *= inv_norm;
    } else {
        // 归一化失败，重置为单位四元数
        q->q0 = 1.0f;
        q->q1 = 0.0f;
        q->q2 = 0.0f;
        q->q3 = 0.0f;
    }
}

/**
 * @brief 从加速度计数据初始化四元数
 */
static void imu_init_from_accel(float ax, float ay, float az) {
    // 归一化加速度计数据
    float norm = sqrtf(ax*ax + ay*ay + az*az);
    if (norm < 0.001f) {
        return;  // 加速度计数据无效
    }
    ax /= norm;
    ay /= norm;
    az /= norm;
    
    // 初始俯仰和横滚（假设Z轴向上，X轴向前）
    float pitch_rad = asinf(-ax);  // 俯仰角
    float roll_rad = atan2f(ay, az);  // 横滚角
    
    // 从横滚和俯仰计算四元数（偏航初始为0）
    float cy = cosf(pitch_rad * 0.5f);
    float sy = sinf(pitch_rad * 0.5f);
    float cr = cosf(roll_rad * 0.5f);
    float sr = sinf(roll_rad * 0.5f);
    
    quat.q0 = cr * cy;
    quat.q1 = sr * cy;
    quat.q2 = cr * sy;
    quat.q3 = sr * sy;
    
    quaternion_normalize(&quat);
    quat_initialized = 1;
}

/**
 * @brief 从四元数计算欧拉角
 */
static void quaternion_to_euler(Quaternion q, float *roll, float *pitch, float *yaw) {
    // 横滚 (绕X轴旋转)
    float sinr_cosp = 2.0f * (q.q0 * q.q1 + q.q2 * q.q3);
    float cosr_cosp = 1.0f - 2.0f * (q.q1 * q.q1 + q.q2 * q.q2);
    *roll = atan2f(sinr_cosp, cosr_cosp);
    
    // 俯仰 (绕Y轴旋转)
    float sinp = 2.0f * (q.q0 * q.q2 - q.q3 * q.q1);
    if (fabsf(sinp) >= 1.0f) {
        *pitch = (sinp > 0.0f) ? (float)M_PI/2.0f : -(float)M_PI/2.0f;
    } else {
        *pitch = asinf(sinp);
    }
    
    // 偏航 (绕Z轴旋转)
    float siny_cosp = 2.0f * (q.q0 * q.q3 + q.q1 * q.q2);
    float cosy_cosp = 1.0f - 2.0f * (q.q2 * q.q2 + q.q3 * q.q3);
    *yaw = atan2f(siny_cosp, cosy_cosp);
    
    // 弧度转换为度
    *roll *= RAD_TO_DEG;
    *pitch *= RAD_TO_DEG;
    *yaw *= RAD_TO_DEG;
}

/**
 * @brief 四元数姿态解算更新函数
 */
static void imu_quaternion_update(float gx_rad, float gy_rad, float gz_rad, 
                                  float ax_g, float ay_g, float az_g) {
    // 第一次运行初始化
    if (!quat_initialized) {
        imu_init_from_accel(ax_g, ay_g, az_g);
        first_quat_update = 0;
        return;
    }
    
    // 归一化加速度计数据
    float norm_a = sqrtf(ax_g*ax_g + ay_g*ay_g + az_g*az_g);
    if (norm_a < 0.001f) {
        return;  // 避免除以0
    }
    norm_a = 1.0f / norm_a;
    ax_g *= norm_a;
    ay_g *= norm_a;
    az_g *= norm_a;
    
    // 归一化当前四元数
    quaternion_normalize(&quat);
    
    // 计算当前姿态下的重力方向
    float vx = 2.0f * (quat.q1 * quat.q3 - quat.q0 * quat.q2);
    float vy = 2.0f * (quat.q0 * quat.q1 + quat.q2 * quat.q3);
    float vz = quat.q0 * quat.q0 - quat.q1 * quat.q1 - quat.q2 * quat.q2 + quat.q3 * quat.q3;
    
    // 计算加速度计测量值与重力估计值的误差
    float ex = (ay_g * vz - az_g * vy);
    float ey = (az_g * vx - ax_g * vz);
    float ez = (ax_g * vy - ay_g * vx);
    
    // 用误差修正陀螺仪读数
    float gx_corrected = gx_rad + beta * ex;
    float gy_corrected = gy_rad + beta * ey;
    float gz_corrected = gz_rad + beta * ez;
    
    // 四元数更新（一阶龙格-库塔法）
    float q0_dot = 0.5f * (-quat.q1 * gx_corrected - quat.q2 * gy_corrected - quat.q3 * gz_corrected);
    float q1_dot = 0.5f * (quat.q0 * gx_corrected + quat.q2 * gz_corrected - quat.q3 * gy_corrected);
    float q2_dot = 0.5f * (quat.q0 * gy_corrected - quat.q1 * gz_corrected + quat.q3 * gx_corrected);
    float q3_dot = 0.5f * (quat.q0 * gz_corrected + quat.q1 * gy_corrected - quat.q2 * gx_corrected);
    
    // 积分
    quat.q0 += q0_dot * dt;
    quat.q1 += q1_dot * dt;
    quat.q2 += q2_dot * dt;
    quat.q3 += q3_dot * dt;
    
    // 归一化四元数
    quaternion_normalize(&quat);
}

// ============================================================================
// 初始化函数
// ============================================================================
void App_Imurb_Init(void){
    uint8_t init_yon = imu660rb_init();  // 可以在其中更改量程，目前为±4G，±2000dps
    
    if(init_yon) {
        // printf("IMU初始化失败\r\n");
    }
    
    // 重置所有变量
    yaw = pitch = roll = 0.0f;
    yaw_unwrapped = 0.0f;
    last_yaw = 0.0f;
    
    // 重置四元数
    quat.q0 = 1.0f;
    quat.q1 = 0.0f;
    quat.q2 = 0.0f;
    quat.q3 = 0.0f;
    quat_initialized = 0;
    first_quat_update = 1;
    use_flash_data = 0;  // 重置Flash使用标志
    
    // 默认尝试从Flash加载校准数据
    if(App_Imurb_LoadCalibFromFlash()){
        // Flash数据加载成功，直接完成校准
        calib_state = CALIB_COMPLETED;
        use_flash_data = 1;  // 标记使用Flash数据
        // printf("使用Flash中的历史校准数据\r\n");
    } else {
        // Flash没有有效数据，需要重新校准
        calib_state = CALIB_NOT_STARTED;
        // printf("Flash中没有校准数据，需要重新校准\r\n");
    }
    
    // 重置校准辅助变量
    calib_total_attempts = 0;
    calib_invalid_samples = 0;
    calib_consecutive_invalid = 0;
    calib_first_delay = 0;
    calib_external_ready = 0;
}

// ============================================================================
// 从Flash加载历史校准数据
// ============================================================================
#define GX_AVE_INDEX  (0)  // 第0个位置存储gx_ave
#define GY_AVE_INDEX  (1)  // 第1个位置存储gy_ave
#define GZ_AVE_INDEX  (2)  // 第2个位置存储gz_ave
uint8_t App_Imurb_LoadCalibFromFlash(void){
    uint8_t flash_valid = 0;
    
    // 1. 初始化Flash
    App_Flash_Init();
    
    // 2. 从Flash读取数据到缓冲区
    App_Flash_to_Buffer();
    
    // 3. 检查Flash中是否有有效数据
    float temp_gx = 0.0f, temp_gy = 0.0f, temp_gz = 0.0f;
    
    // 临时读取到局部变量
    temp_gx = flash_union_buffer_imutof[GX_AVE_INDEX].float_type;
    temp_gy = flash_union_buffer_imutof[GY_AVE_INDEX].float_type;
    temp_gz = flash_union_buffer_imutof[GZ_AVE_INDEX].float_type;
    
    // 检查数据是否有效
    if(!isnan(temp_gx) && !isnan(temp_gy) && !isnan(temp_gz) &&
       !isinf(temp_gx) && !isinf(temp_gy) && !isinf(temp_gz)){
        
        if(fabs(temp_gx) < 5.0f && fabs(temp_gy) < 5.0f && fabs(temp_gz) < 5.0f){
            // 数据有效，加载到全局变量
            gx_ave = temp_gx;
            gy_ave = temp_gy;
            gz_ave = temp_gz;
            flash_valid = 1;
        }
    }
    
    return flash_valid;
}

// ============================================================================
// 保存当前校准数据到Flash
// ============================================================================
uint8_t App_Imurb_SaveCalibToFlash(void){
    // 1. 将当前校准值存入缓冲区
    App_Value_to_Buffer(flash_gx_ave);
    App_Value_to_Buffer(flash_gy_ave);
    App_Value_to_Buffer(flash_gz_ave);
    
    // 2. 将缓冲区数据写入Flash
    //App_Buffer_to_Flash();
    return 1;
}

// ============================================================================
// 准备校准函数
// ============================================================================
void App_Imurb_Calib_Ready(void){
    calib_external_ready = 1;
    calib_first_delay = 0;
    calib_state = CALIB_NOT_STARTED;
    quat_initialized = 0;  // 重置四元数初始化标志
    use_flash_data = 0;    // 重置Flash使用标志
}

// ============================================================================
// 陀螺仪零漂校准
// ============================================================================
uint32_t interrupt_state;
void App_Imurb_Calib_Task(void){
    PERIODIC(5)  // 5ms周期
    
    // 如果校准已完成，直接返回
    if(calib_state == CALIB_COMPLETED) return;
    
    // 初始延迟，确保车模放置稳定
    if(calib_first_delay < 200) {
        calib_first_delay++;
        return;
    }
    
    // 等待外部触发信号
    if(calib_first_delay == 200 && calib_external_ready == 0) {
        return;
    }
    
    // 开始校准流程
    if(calib_state == CALIB_NOT_STARTED){

        App_Buzzer_On(1);  // 指示灯表示校准开始
        
          // 保存当前中断状态并禁用所有中断
        interrupt_state = __get_PRIMASK();
        __disable_irq();
        
        calib_state = CALIB_IN_PROGRESS;
        calib_count = 0;
        all_gx = 0.0f;
        all_gy = 0.0f;
        all_gz = 0.0f;
        
        // 重置辅助变量
        calib_total_attempts = 0;
        calib_invalid_samples = 0;
        calib_consecutive_invalid = 0;
    }
    
    if(calib_state == CALIB_IN_PROGRESS){
        // 增加总尝试次数，防止死循环
        calib_total_attempts++;
        if(calib_total_attempts > CALIB_MAX_ATTEMPTS) {
            calib_state = CALIB_COMPLETED;
            return;
        }
        
        // 采集有效数据
        if(calib_count < CALIB_SAMPLE_COUNT){
            imu660rb_get_gyro();
            
            float gx_temp = -imu660rb_gyro_transition((int16_t)imu660rb_gyro_x);
            float gy_temp =  imu660rb_gyro_transition((int16_t)imu660rb_gyro_y);
            float gz_temp = -imu660rb_gyro_transition((int16_t)imu660rb_gyro_z);
                     
            // 检查数据有效性
            if(fabs(gx_temp) > CALIB_THRESHOLD || 
               fabs(gy_temp) > CALIB_THRESHOLD || 
               fabs(gz_temp) > CALIB_THRESHOLD){
                // 无效数据
                calib_invalid_samples++;
                calib_consecutive_invalid++;
                return;
            }
            
            // 有效数据
            calib_consecutive_invalid = 0;
            
            // 累加有效数据
            all_gx += gx_temp;
            all_gy += gy_temp;
            all_gz += gz_temp;
            calib_count++;
        }
        else{
            // 计算零漂平均值
            gx_ave = all_gx / CALIB_SAMPLE_COUNT;
            gy_ave = all_gy / CALIB_SAMPLE_COUNT;
            gz_ave = all_gz / CALIB_SAMPLE_COUNT;
            
            // 保存到Flash
            App_Imurb_SaveCalibToFlash();
            
            // 更新校准状态
            calib_state = CALIB_COMPLETED;
            calib_external_ready = 0;
            use_flash_data = 0;  // 本次使用的是新校准的数据
            
                // 恢复之前的中断状态
            __set_PRIMASK(interrupt_state);
            
            App_Buzzer_On(0);  // 指示灯表示校准完成
        }
    }
    return;
}

// ============================================================================
// 初始化互补滤波状态（现在改为四元数初始化）
// ============================================================================
void App_Imurb_InitFilterState(void){
    if(calib_state != CALIB_COMPLETED){
        return;
    }
    
    if(quat_initialized){
        return;
    }
    
    // 获取当前加速度计数据
    imu660rb_get_acc();
    float init_ax = -imu660rb_acc_transition(imu660rb_acc_x) / GRAVITY;
    float init_ay =  imu660rb_acc_transition(imu660rb_acc_y) / GRAVITY;
    float init_az = -imu660rb_acc_transition(imu660rb_acc_z) / GRAVITY;
    
    // 初始化四元数
    imu_init_from_accel(init_ax, init_ay, init_az);
    
    // 立即获取一次欧拉角
    float roll_rad, pitch_rad, yaw_rad;
    quaternion_to_euler(quat, &roll_rad, &pitch_rad, &yaw_rad);
    
    // 转换为度
    roll = roll_rad;
    pitch = pitch_rad;
    yaw = yaw_rad;
    yaw_unwrapped = yaw;
    last_yaw = yaw;
    
    quat_initialized = 1;
}

// ============================================================================
// 更新IMU原始数据
// ============================================================================
void App_Imurb_Update(void){
    // 如果校准未完成，仍可读取数据但不进行姿态解算
    if(calib_state != CALIB_COMPLETED){
        // 可以读取但标记为未校准
        imu660rb_get_acc();
        ax = -imu660rb_acc_transition(imu660rb_acc_x) / GRAVITY;
        ay =  imu660rb_acc_transition(imu660rb_acc_y) / GRAVITY;
        az = -imu660rb_acc_transition(imu660rb_acc_z) / GRAVITY;
        
        imu660rb_get_gyro();
        gx = -imu660rb_gyro_transition(imu660rb_gyro_x) * DEG_TO_RAD;
        gy =  imu660rb_gyro_transition(imu660rb_gyro_y) * DEG_TO_RAD;
        gz = -imu660rb_gyro_transition(imu660rb_gyro_z) * DEG_TO_RAD;
        return;
    }
  
    // 获取加速度计数据（转换为g单位）
    imu660rb_get_acc();
    ax = -imu660rb_acc_transition(imu660rb_acc_x) / GRAVITY;
    ay =  imu660rb_acc_transition(imu660rb_acc_y) / GRAVITY;
    az = -imu660rb_acc_transition(imu660rb_acc_z) / GRAVITY;
    
    // 获取陀螺仪数据并去零漂（转换为rad/s单位）
    imu660rb_get_gyro();
    
    // 转换为实际物理值并减去零漂平均值
    gx = (-imu660rb_gyro_transition(imu660rb_gyro_x) - gx_ave) * DEG_TO_RAD;
    gy = ( imu660rb_gyro_transition(imu660rb_gyro_y) - gy_ave) * DEG_TO_RAD;
    gz = (-imu660rb_gyro_transition(imu660rb_gyro_z) - gz_ave) * DEG_TO_RAD;
}

void App_Imurb_Gyro_Update(void){
    // 如果校准未完成，仍可读取数据但不进行姿态解算
    if(calib_state != CALIB_COMPLETED){
        imu660rb_get_gyro();
        gx = -imu660rb_gyro_transition(imu660rb_gyro_x) * DEG_TO_RAD;
        gy =  imu660rb_gyro_transition(imu660rb_gyro_y) * DEG_TO_RAD;
        gz = -imu660rb_gyro_transition(imu660rb_gyro_z) * DEG_TO_RAD;
        return;
    }
    
    // 获取陀螺仪数据并去零漂
    imu660rb_get_gyro();
    
    // 转换为实际物理值并减去零漂平均值
    gx = (-imu660rb_gyro_transition(imu660rb_gyro_x) - gx_ave) * DEG_TO_RAD;
    gy = ( imu660rb_gyro_transition(imu660rb_gyro_y) - gy_ave) * DEG_TO_RAD;
    gz = (-imu660rb_gyro_transition(imu660rb_gyro_z) - gz_ave) * DEG_TO_RAD;
}

void App_Imurb_Acc_Update(void){
    // 获取加速度计数据
    imu660rb_get_acc();
    ax = -imu660rb_acc_transition(imu660rb_acc_x) / GRAVITY;
    ay =  imu660rb_acc_transition(imu660rb_acc_y) / GRAVITY;
    az = -imu660rb_acc_transition(imu660rb_acc_z) / GRAVITY;
}

// ============================================================================
// 姿态解算处理（四元数版本）
// ============================================================================

void App_Imurb_Proc(void){
    // motor_control为0时重置四元数状态
    if(motor_control == 0){
        quat.q0 = 1.0f;
        quat.q1 = 0.0f;
        quat.q2 = 0.0f;
        quat.q3 = 0.0f;
        quat_initialized = 0;
        first_quat_update = 1;
        yaw = pitch = roll = 0.0f;
        yaw_unwrapped = 0.0f;
        last_yaw = 0.0f;
        return;
    }
    
    //PERIODIC(5)  // 5ms周期
    
    // 如果校准未完成，不进行姿态解算
    if(calib_state != CALIB_COMPLETED){
        return;
    }
    
    // 如果四元数状态未初始化，先进行初始化
    if(!quat_initialized){
        App_Imurb_InitFilterState();
        return;  // 本次不进行解算，等待下一次调用
    }
    
    // 更新传感器数据
    //App_Imurb_Update(); //这里单独测试打开，control控制时关闭
    
    // 四元数姿态解算
    imu_quaternion_update(gx, gy, gz, ax, ay, az);
    
    // 从四元数获取欧拉角
    float roll_rad, pitch_rad, yaw_rad;
    quaternion_to_euler(quat, &roll_rad, &pitch_rad, &yaw_rad);
    
    // 更新输出角度
    roll = roll_rad;
    pitch = pitch_rad;
    yaw = yaw_rad;
    
    // 对yaw进行解包装，得到连续角度
    if (first_quat_update) {
        last_yaw = yaw;
        yaw_unwrapped = yaw;
        first_quat_update = 0;
    } else {
        float delta = yaw - last_yaw;
        
        if (delta > 180.0f) delta -= 360.0f;
        else if (delta < -180.0f) delta += 360.0f;
        
        yaw_unwrapped += delta;
        last_yaw = yaw;
    }
    
    return;
}

// 重置 IMU 的 yaw 为指定角度（度）
void App_Imurb_SetYaw(float yaw_deg)
{
    // 重置四元数为仅绕 Z 轴旋转 yaw_deg 的姿态
    float half_yaw = yaw_deg * 0.5f * DEG_TO_RAD;
    quat.q0 = cosf(half_yaw);
    quat.q1 = 0.0f;
    quat.q2 = 0.0f;
    quat.q3 = sinf(half_yaw);
    
    // 重置解包装状态
    last_yaw = yaw_deg;
    yaw_unwrapped = yaw_deg;
    yaw = yaw_deg;
    
    // 标记四元数已初始化
    quat_initialized = 1;
    first_quat_update = 0;
}

void App_Imurb_First(void){
    if(motor_control == 0){
        quat.q0 = 1.0f;
        quat.q1 = 0.0f;
        quat.q2 = 0.0f;
        quat.q3 = 0.0f;
        quat_initialized = 0;
        first_quat_update = 1;
        yaw = pitch = roll = 0.0f;
        yaw_unwrapped = 0.0f;
        last_yaw = 0.0f;
        return;
    }
}

// ============================================================================
// 手动初始化滤波状态
// ============================================================================
uint8_t App_Imurb_ManualInitFilter(void){
    if(calib_state != CALIB_COMPLETED){
        return 0;
    }
    
    App_Imurb_InitFilterState();
    return 1;
}

// ============================================================================
// 获取滤波初始化状态
// ============================================================================
uint8_t App_Imurb_IsFilterInitialized(void){
    return quat_initialized;
}

// ============================================================================
// 获取校准状态
// ============================================================================
CalibState App_Imurb_GetCalibState(void){
    return calib_state;
}

// ============================================================================
// 数据获取函数
// ============================================================================
float App_Imurb_Get_Ax(void){
    return ax * GRAVITY;  // 返回m/s2单位
}

float App_Imurb_Get_Ay(void){
    return ay * GRAVITY;  // 返回m/s2单位
}

float App_Imurb_Get_Az(void){
    return az * GRAVITY;  // 返回m/s2单位
}

float App_Imurb_Get_Gx(void){
    return gx * RAD_TO_DEG;  // 返回°/s单位
}

float App_Imurb_Get_Gy(void){
    return gy * RAD_TO_DEG;  // 返回°/s单位
}

float App_Imurb_Get_Gz(void){
    return gz * RAD_TO_DEG;  // 返回°/s单位
}

float App_Imurb_Yaw(void){
    return yaw;
}

float App_Imurb_Yaw_Rise(void){
    return yaw_unwrapped;  // 返回连续角度
}

float App_Imurb_Pitch(void){
    return pitch;
}

float App_Imurb_Roll(void){
    return roll;
}

float App_Imurb_Gx_Ave(void){
    return gx_ave;
}

float App_Imurb_Gy_Ave(void){
    return gy_ave;
}

float App_Imurb_Gz_Ave(void){
    return gz_ave;
}