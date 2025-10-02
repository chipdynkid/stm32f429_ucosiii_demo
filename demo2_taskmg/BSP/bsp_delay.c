#include "bsp_delay.h"

static uint32_t g_fac_us = 0;       /* us��ʱ������ */

/* ���SYS_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS) */
#if SYS_SUPPORT_OS

#include "os.h"

/**
 * @brief     systick�жϷ�����,ʹ��OSʱ�õ�
 * @param     ticks : ��ʱ�Ľ�����  
 * @retval    ��
 */  
void SysTick_Handler(void)
{
    if (OSRunning == OS_STATE_OS_RUNNING)   /* OS��ʼ����,��ִ�������ĵ��ȴ��� */
    {
        OS_CPU_SysTickHandler();            /* ����uC/OS-III��SysTick�жϷ������ */
    }
    HAL_IncTick();
}
#endif

/**
 * @brief     ��ʼ���ӳٺ���
 * @param     sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(rcc_c_ck), 168MHz
 * @retval    ��
 */  
void BSP_Delay_Init(uint16_t sysclk)
{

    uint32_t reload;
    
    SysTick->CTRL = 0;                                      /* ��Systick״̬���Ա���һ�����裬������￪���жϻ�ر����ж� */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);    /* SYSTICKʹ���ⲿʱ��Դ��Ƶ��ΪHCLK */
    g_fac_us = sysclk;                                      /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ��,��Ϊ1us�Ļ���ʱ�� */
    
#if SYS_SUPPORT_OS                                          /* �����Ҫ֧��OS. */
    reload = SysTick->LOAD = sysclk;                        /* �����������ֵ������ʱ���� */
    reload *= 1000000 / OSCfg_TickRate_Hz;                  /* ����OSCfg_TickRate_Hz�趨���ʱ��
                                                             * reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����
                                                             */
#else
    reload = SysTick->LOAD = sysclk;                        /* �����������ֵ������ʱ���� */
    reload *= 1000000 / (1000U / uwTickFreq);
#endif
    SysTick->CTRL |= 1 << 1;                                /* ����SYSTICK�ж� */
    SysTick->LOAD = reload;                                 /* ÿ1/delay_ostickspersec���ж�һ�� */
    SysTick->CTRL |= 1 << 0;                                /* ����SYSTICK */

}

#if SYS_SUPPORT_OS                                      /* �����Ҫ֧��OS, �����´��� */

/**
 * @brief     ��ʱnus
 * @param     nus: Ҫ��ʱ��us��
 * @note      nusȡֵ��Χ : 0 ~ 190887435us(���ֵ�� 2^32 / fac_us @fac_us = 21)
 * @retval    ��
 */ 
void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload;
    OS_ERR err;
    
    reload = SysTick->LOAD;     /* LOAD��ֵ */
    ticks = nus * g_fac_us;     /* ��Ҫ�Ľ����� */
    OSSchedLock(&err);          /* ��ֹOS���ȣ���ֹ���us��ʱ */
    told = SysTick->VAL;        /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����. */
            }
            else
            {
                tcnt += reload - tnow + told;
            }

            told = tnow;

            if (tcnt >= ticks) break;   /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
        }
    }

    OSSchedUnlock(&err);                /* �ָ�OS���� */
} 

/**
 * @brief     ��ʱnms
 * @param     nms: Ҫ��ʱ��ms�� (0< nms <= 65535) 
 * @retval    ��
 */
void Delay_ms(uint16_t nms)
{
    uint32_t i;
    
    for (i=0; i<nms; i++)
    {
        Delay_us(1000);
    }                /* ��ͨ��ʽ��ʱ */
}

#else  /* ��ʹ��OSʱ, �����´��� */

/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        nusȡֵ��Χ : 0~190887435(���ֵ�� 2^32 / fac_us @fac_us = 21)
 * @retval      ��
 */
void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD��ֵ */
    ticks = nus * g_fac_us;                 /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                    /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= 65535)
 * @retval      ��
 */
void Delay_ms(uint16_t nms)
{
    uint32_t repeat = nms / 30;     /*  ������30,�ǿ��ǵ������г�ƵӦ�� */
    uint32_t remain = nms % 30;

    while (repeat)
    {
        Delay_us(30 * 1000);        /* ����Delay_us ʵ�� 1000ms ��ʱ */
        repeat--;
    }

    if (remain)
    {
        Delay_us(remain * 1000);    /* ����Delay_us, ��β����ʱ(remain ms)������ */
    }
}

/**
 * @brief       HAL���ڲ������õ�����ʱ
 * @note        HAL�����ʱĬ����Systick���������û�п�Systick���жϻᵼ�µ��������ʱ���޷��˳�
 * @param       Delay : Ҫ��ʱ�ĺ�����
 * @retval      None
 */
void HAL_Delay(uint32_t Delay)
{
    Delay_ms(Delay);
}
#endif









