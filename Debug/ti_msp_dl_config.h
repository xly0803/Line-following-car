/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_MOTOR */
#define PWM_MOTOR_INST                                                     TIMG0
#define PWM_MOTOR_INST_IRQHandler                               TIMG0_IRQHandler
#define PWM_MOTOR_INST_INT_IRQN                                 (TIMG0_INT_IRQn)
#define PWM_MOTOR_INST_CLK_FREQ                                          2000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_MOTOR_C0_PORT                                             GPIOA
#define GPIO_PWM_MOTOR_C0_PIN                                     DL_GPIO_PIN_12
#define GPIO_PWM_MOTOR_C0_IOMUX                                  (IOMUX_PINCM34)
#define GPIO_PWM_MOTOR_C0_IOMUX_FUNC                 IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_PWM_MOTOR_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_MOTOR_C1_PORT                                             GPIOA
#define GPIO_PWM_MOTOR_C1_PIN                                     DL_GPIO_PIN_13
#define GPIO_PWM_MOTOR_C1_IOMUX                                  (IOMUX_PINCM35)
#define GPIO_PWM_MOTOR_C1_IOMUX_FUNC                 IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_MOTOR_C1_IDX                                DL_TIMER_CC_1_INDEX




/* Defines for QEI_0 */
#define QEI_0_INST                                                         TIMG8
#define QEI_0_INST_IRQHandler                                   TIMG8_IRQHandler
#define QEI_0_INST_INT_IRQN                                     (TIMG8_INT_IRQn)
/* Pin configuration defines for QEI_0 PHA Pin */
#define GPIO_QEI_0_PHA_PORT                                                GPIOB
#define GPIO_QEI_0_PHA_PIN                                         DL_GPIO_PIN_6
#define GPIO_QEI_0_PHA_IOMUX                                     (IOMUX_PINCM23)
#define GPIO_QEI_0_PHA_IOMUX_FUNC                    IOMUX_PINCM23_PF_TIMG8_CCP0
/* Pin configuration defines for QEI_0 PHB Pin */
#define GPIO_QEI_0_PHB_PORT                                                GPIOB
#define GPIO_QEI_0_PHB_PIN                                         DL_GPIO_PIN_7
#define GPIO_QEI_0_PHB_IOMUX                                     (IOMUX_PINCM24)
#define GPIO_QEI_0_PHB_IOMUX_FUNC                    IOMUX_PINCM24_PF_TIMG8_CCP1


/* Defines for CAPTURE_0 */
#define CAPTURE_0_INST                                                   (TIMG6)
#define CAPTURE_0_INST_IRQHandler                               TIMG6_IRQHandler
#define CAPTURE_0_INST_INT_IRQN                                 (TIMG6_INT_IRQn)
#define CAPTURE_0_INST_LOAD_VALUE                                           (0U)
/* GPIO defines for channel 0 */
#define GPIO_CAPTURE_0_C0_PORT                                             GPIOB
#define GPIO_CAPTURE_0_C0_PIN                                     DL_GPIO_PIN_26
#define GPIO_CAPTURE_0_C0_IOMUX                                  (IOMUX_PINCM57)
#define GPIO_CAPTURE_0_C0_IOMUX_FUNC                 IOMUX_PINCM57_PF_TIMG6_CCP0





/* Defines for TIMER_7 */
#define TIMER_7_INST                                                     (TIMG7)
#define TIMER_7_INST_IRQHandler                                 TIMG7_IRQHandler
#define TIMER_7_INST_INT_IRQN                                   (TIMG7_INT_IRQn)
#define TIMER_7_INST_LOAD_VALUE                                         (31999U)



/* Defines for SPI_IMU660RB */
#define SPI_IMU660RB_INST                                                  SPI0
#define SPI_IMU660RB_INST_IRQHandler                            SPI0_IRQHandler
#define SPI_IMU660RB_INST_INT_IRQN                                SPI0_INT_IRQn
#define GPIO_SPI_IMU660RB_PICO_PORT                                       GPIOB
#define GPIO_SPI_IMU660RB_PICO_PIN                               DL_GPIO_PIN_17
#define GPIO_SPI_IMU660RB_IOMUX_PICO                            (IOMUX_PINCM43)
#define GPIO_SPI_IMU660RB_IOMUX_PICO_FUNC            IOMUX_PINCM43_PF_SPI0_PICO
#define GPIO_SPI_IMU660RB_POCI_PORT                                       GPIOB
#define GPIO_SPI_IMU660RB_POCI_PIN                               DL_GPIO_PIN_19
#define GPIO_SPI_IMU660RB_IOMUX_POCI                            (IOMUX_PINCM45)
#define GPIO_SPI_IMU660RB_IOMUX_POCI_FUNC            IOMUX_PINCM45_PF_SPI0_POCI
/* GPIO configuration for SPI_IMU660RB */
#define GPIO_SPI_IMU660RB_SCLK_PORT                                       GPIOB
#define GPIO_SPI_IMU660RB_SCLK_PIN                               DL_GPIO_PIN_18
#define GPIO_SPI_IMU660RB_IOMUX_SCLK                            (IOMUX_PINCM44)
#define GPIO_SPI_IMU660RB_IOMUX_SCLK_FUNC            IOMUX_PINCM44_PF_SPI0_SCLK



/* Port definition for Pin Group GPIO_GRP_1 */
#define GPIO_GRP_1_PORT                                                  (GPIOB)

/* Defines for PIN_0: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_GRP_1_PIN_0_PIN                                    (DL_GPIO_PIN_27)
#define GPIO_GRP_1_PIN_0_IOMUX                                   (IOMUX_PINCM58)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOA)

/* Defines for FLAG: GPIOA.30 with pinCMx 5 on package pin 37 */
#define KEY_FLAG_PIN                                            (DL_GPIO_PIN_30)
#define KEY_FLAG_IOMUX                                            (IOMUX_PINCM5)
/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOB)

/* Defines for PIN_9: GPIOB.9 with pinCMx 26 on package pin 61 */
#define GPIO_GRP_0_PIN_9_PIN                                     (DL_GPIO_PIN_9)
#define GPIO_GRP_0_PIN_9_IOMUX                                   (IOMUX_PINCM26)
/* Defines for PIN_10: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_GRP_0_PIN_10_PIN                                   (DL_GPIO_PIN_10)
#define GPIO_GRP_0_PIN_10_IOMUX                                  (IOMUX_PINCM27)
/* Defines for PIN_11: GPIOB.11 with pinCMx 28 on package pin 63 */
#define GPIO_GRP_0_PIN_11_PIN                                   (DL_GPIO_PIN_11)
#define GPIO_GRP_0_PIN_11_IOMUX                                  (IOMUX_PINCM28)
/* Defines for PIN_12: GPIOB.12 with pinCMx 29 on package pin 64 */
#define GPIO_GRP_0_PIN_12_PIN                                   (DL_GPIO_PIN_12)
#define GPIO_GRP_0_PIN_12_IOMUX                                  (IOMUX_PINCM29)
/* Defines for PIN_13: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GPIO_GRP_0_PIN_13_PIN                                   (DL_GPIO_PIN_13)
#define GPIO_GRP_0_PIN_13_IOMUX                                  (IOMUX_PINCM30)
/* Defines for PIN_14: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_GRP_0_PIN_14_PIN                                   (DL_GPIO_PIN_14)
#define GPIO_GRP_0_PIN_14_IOMUX                                  (IOMUX_PINCM31)
/* Defines for PIN_15: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GPIO_GRP_0_PIN_15_PIN                                   (DL_GPIO_PIN_15)
#define GPIO_GRP_0_PIN_15_IOMUX                                  (IOMUX_PINCM32)
/* Defines for PIN_16: GPIOB.16 with pinCMx 33 on package pin 4 */
#define GPIO_GRP_0_PIN_16_PIN                                   (DL_GPIO_PIN_16)
#define GPIO_GRP_0_PIN_16_IOMUX                                  (IOMUX_PINCM33)
/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOB)

/* Defines for OSCL: GPIOB.2 with pinCMx 15 on package pin 50 */
#define OLED_OSCL_PIN                                            (DL_GPIO_PIN_2)
#define OLED_OSCL_IOMUX                                          (IOMUX_PINCM15)
/* Defines for OSDA: GPIOB.3 with pinCMx 16 on package pin 51 */
#define OLED_OSDA_PIN                                            (DL_GPIO_PIN_3)
#define OLED_OSDA_IOMUX                                          (IOMUX_PINCM16)
/* Port definition for Pin Group MPU */
#define MPU_PORT                                                         (GPIOA)

/* Defines for MSCL: GPIOA.11 with pinCMx 22 on package pin 57 */
#define MPU_MSCL_PIN                                            (DL_GPIO_PIN_11)
#define MPU_MSCL_IOMUX                                           (IOMUX_PINCM22)
/* Defines for MSDA: GPIOA.10 with pinCMx 21 on package pin 56 */
#define MPU_MSDA_PIN                                            (DL_GPIO_PIN_10)
#define MPU_MSDA_IOMUX                                           (IOMUX_PINCM21)
/* Defines for AIN1: GPIOB.0 with pinCMx 12 on package pin 47 */
#define MOTOR_AIN1_PORT                                                  (GPIOB)
#define MOTOR_AIN1_PIN                                           (DL_GPIO_PIN_0)
#define MOTOR_AIN1_IOMUX                                         (IOMUX_PINCM12)
/* Defines for AIN2: GPIOB.1 with pinCMx 13 on package pin 48 */
#define MOTOR_AIN2_PORT                                                  (GPIOB)
#define MOTOR_AIN2_PIN                                           (DL_GPIO_PIN_1)
#define MOTOR_AIN2_IOMUX                                         (IOMUX_PINCM13)
/* Defines for BIN1: GPIOA.24 with pinCMx 54 on package pin 25 */
#define MOTOR_BIN1_PORT                                                  (GPIOA)
#define MOTOR_BIN1_PIN                                          (DL_GPIO_PIN_24)
#define MOTOR_BIN1_IOMUX                                         (IOMUX_PINCM54)
/* Defines for BIN2: GPIOA.25 with pinCMx 55 on package pin 26 */
#define MOTOR_BIN2_PORT                                                  (GPIOA)
#define MOTOR_BIN2_PIN                                          (DL_GPIO_PIN_25)
#define MOTOR_BIN2_IOMUX                                         (IOMUX_PINCM55)
/* Port definition for Pin Group GPIO_IMU660RB */
#define GPIO_IMU660RB_PORT                                               (GPIOB)

/* Defines for PIN_IMU660RB_CS: GPIOB.20 with pinCMx 48 on package pin 19 */
#define GPIO_IMU660RB_PIN_IMU660RB_CS_PIN                       (DL_GPIO_PIN_20)
#define GPIO_IMU660RB_PIN_IMU660RB_CS_IOMUX                      (IOMUX_PINCM48)
/* Defines for PIN_IMU660RB_INT1: GPIOB.21 with pinCMx 49 on package pin 20 */
// pins affected by this interrupt request:["PIN_IMU660RB_INT1"]
#define GPIO_IMU660RB_INT_IRQN                                  (GPIOB_INT_IRQn)
#define GPIO_IMU660RB_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_IMU660RB_PIN_IMU660RB_INT1_IIDX                (DL_GPIO_IIDX_DIO21)
#define GPIO_IMU660RB_PIN_IMU660RB_INT1_PIN                     (DL_GPIO_PIN_21)
#define GPIO_IMU660RB_PIN_IMU660RB_INT1_IOMUX                    (IOMUX_PINCM49)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_MOTOR_init(void);
void SYSCFG_DL_QEI_0_init(void);
void SYSCFG_DL_CAPTURE_0_init(void);
void SYSCFG_DL_TIMER_7_init(void);
void SYSCFG_DL_SPI_IMU660RB_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
