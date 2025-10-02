/**
 ******************************************************************************
 * @file    Demonstrations/Inc/main.h
 * @author  MCD Application Team
 * @brief   Header for main.c module
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "os_task_cfg.h"
#include "os_interface.h"
#include <stdio.h>
#include <stdlib.h>

/* Deifine ------------------------------------------------------------------*/
#define LED0_GPIO_PORT GPIOB
#define LED0_GPIO_PIN GPIO_PIN_0
#define LED1_GPIO_PORT GPIOB
#define LED1_GPIO_PIN GPIO_PIN_1

#define KEY0_GPIO_PORT GPIOH
#define KEY0_GPIO_PIN GPIO_PIN_3
#define KEY1_GPIO_PORT GPIOH
#define KEY1_GPIO_PIN GPIO_PIN_2
#define KEY2_GPIO_PORT GPIOC
#define KEY2_GPIO_PIN GPIO_PIN_13
#define WKUP_GPIO_PORT GPIOA
#define WKUP_GPIO_PIN GPIO_PIN_0

void Error_Handler(void);

#endif /* __MAIN_H */
