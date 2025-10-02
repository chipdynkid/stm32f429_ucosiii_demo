#include "bsp_delay.h"

static uint32_t g_fac_us = 0;       /* us延时倍乘数 */

/* 如果SYS_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS) */
#if SYS_SUPPORT_OS

#include "os.h"

/**
 * @brief     systick中断服务函数,使用OS时用到
 * @param     ticks : 延时的节拍数  
 * @retval    无
 */  
void SysTick_Handler(void)
{
    if (OSRunning == OS_STATE_OS_RUNNING)   /* OS开始跑了,才执行正常的调度处理 */
    {
        OS_CPU_SysTickHandler();            /* 调用uC/OS-III的SysTick中断服务程序 */
    }
    HAL_IncTick();
}
#endif

/**
 * @brief     初始化延迟函数
 * @param     sysclk: 系统时钟频率, 即CPU频率(rcc_c_ck), 168MHz
 * @retval    无
 */  
void BSP_Delay_Init(uint16_t sysclk)
{

    uint32_t reload;
    
    SysTick->CTRL = 0;                                      /* 清Systick状态，以便下一步重设，如果这里开了中断会关闭其中断 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);    /* SYSTICK使用外部时钟源，频率为HCLK */
    g_fac_us = sysclk;                                      /* 不论是否使用OS,g_fac_us都需要使用,作为1us的基础时基 */
    
#if SYS_SUPPORT_OS                                          /* 如果需要支持OS. */
    reload = SysTick->LOAD = sysclk;                        /* 设置最大重载值用于延时计算 */
    reload *= 1000000 / OSCfg_TickRate_Hz;                  /* 根据OSCfg_TickRate_Hz设定溢出时间
                                                             * reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右
                                                             */
#else
    reload = SysTick->LOAD = sysclk;                        /* 设置最大重载值用于延时计算 */
    reload *= 1000000 / (1000U / uwTickFreq);
#endif
    SysTick->CTRL |= 1 << 1;                                /* 开启SYSTICK中断 */
    SysTick->LOAD = reload;                                 /* 每1/delay_ostickspersec秒中断一次 */
    SysTick->CTRL |= 1 << 0;                                /* 开启SYSTICK */

}

#if SYS_SUPPORT_OS                                      /* 如果需要支持OS, 用以下代码 */

/**
 * @brief     延时nus
 * @param     nus: 要延时的us数
 * @note      nus取值范围 : 0 ~ 190887435us(最大值即 2^32 / fac_us @fac_us = 21)
 * @retval    无
 */ 
void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload;
    OS_ERR err;
    
    reload = SysTick->LOAD;     /* LOAD的值 */
    ticks = nus * g_fac_us;     /* 需要的节拍数 */
    OSSchedLock(&err);          /* 阻止OS调度，防止打断us延时 */
    told = SysTick->VAL;        /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    /* 这里注意一下SYSTICK是一个递减的计数器就可以了. */
            }
            else
            {
                tcnt += reload - tnow + told;
            }

            told = tnow;

            if (tcnt >= ticks) break;   /* 时间超过/等于要延迟的时间,则退出. */
        }
    }

    OSSchedUnlock(&err);                /* 恢复OS调度 */
} 

/**
 * @brief     延时nms
 * @param     nms: 要延时的ms数 (0< nms <= 65535) 
 * @retval    无
 */
void Delay_ms(uint16_t nms)
{
    uint32_t i;
    
    for (i=0; i<nms; i++)
    {
        Delay_us(1000);
    }                /* 普通方式延时 */
}

#else  /* 不使用OS时, 用以下代码 */

/**
 * @brief       延时nus
 * @param       nus: 要延时的us数.
 * @note        nus取值范围 : 0~190887435(最大值即 2^32 / fac_us @fac_us = 21)
 * @retval      无
 */
void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * g_fac_us;                 /* 需要的节拍数 */
    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
}

/**
 * @brief       延时nms
 * @param       nms: 要延时的ms数 (0< nms <= 65535)
 * @retval      无
 */
void Delay_ms(uint16_t nms)
{
    uint32_t repeat = nms / 30;     /*  这里用30,是考虑到可能有超频应用 */
    uint32_t remain = nms % 30;

    while (repeat)
    {
        Delay_us(30 * 1000);        /* 利用Delay_us 实现 1000ms 延时 */
        repeat--;
    }

    if (remain)
    {
        Delay_us(remain * 1000);    /* 利用Delay_us, 把尾数延时(remain ms)给做了 */
    }
}

/**
 * @brief       HAL库内部函数用到的延时
 * @note        HAL库的延时默认用Systick，如果我们没有开Systick的中断会导致调用这个延时后无法退出
 * @param       Delay : 要延时的毫秒数
 * @retval      None
 */
void HAL_Delay(uint32_t Delay)
{
    Delay_ms(Delay);
}
#endif









