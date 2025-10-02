#ifndef __BSP_LED_KEY_EXTI_H
#define __BSP_LED_KEY_EXTI_H

#include "main.h"

void BSP_LED_Init(void);
void BSP_KEY_Init(void);
void BSP_EXTI_Init(void);

uint8_t KEY_Scan(uint8_t mode);

#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) /* 读取 KEY0 引脚 */
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) /* 读取 KEY1 引脚 */
// #define KEY2 HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) /* 读取 KEY2 引脚 */
#define WK_UP HAL_GPIO_ReadPin(WKUP_GPIO_PORT, WKUP_GPIO_PIN) /* 读取 WKUP 引脚 */
#define KEY0_PRES 1 /* KEY0 按下 */
#define KEY1_PRES 2 /* KEY1 按下 */
// #define KEY2_PRES 3 /* KEY2 按下 */
#define WKUP_PRES 4 /* KEY_UP 按下(即 WK_UP) */

#endif
