#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "main.h"

#define SYS_SUPPORT_OS 1

void BSP_Delay_Init(uint16_t sysclk);       /* 初始化延迟函数 */
void Delay_ms(uint16_t nms);                /* 延时nms */
void Delay_us(uint32_t nus);                /* 延时nus */

#if (!SYS_SUPPORT_OS)                       /* 没有使用Systick中断 */
    void HAL_Delay(uint32_t Delay);         /* HAL库的延时函数，SDIO等需要用到 */
#endif

#endif
