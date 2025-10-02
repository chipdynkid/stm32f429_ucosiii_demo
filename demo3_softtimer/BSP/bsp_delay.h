#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "main.h"

#define SYS_SUPPORT_OS 1

void BSP_Delay_Init(uint16_t sysclk);       /* ��ʼ���ӳٺ��� */
void Delay_ms(uint16_t nms);                /* ��ʱnms */
void Delay_us(uint32_t nus);                /* ��ʱnus */

#if (!SYS_SUPPORT_OS)                       /* û��ʹ��Systick�ж� */
    void HAL_Delay(uint32_t Delay);         /* HAL�����ʱ������SDIO����Ҫ�õ� */
#endif

#endif
