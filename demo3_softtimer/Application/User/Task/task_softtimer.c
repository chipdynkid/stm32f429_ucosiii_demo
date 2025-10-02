#include "task_softtimer.h"
#include "bsp_uart.h"

OS_TMR  Timer1; /* 软件定时器 */
OS_TMR  Timer2; /* 软件定时器 */

void Software_Timer_Callback(void *p_tmr, void *p_arg)
{
    static uint32_t timer1_num = 0;
    static uint32_t timer2_num = 0;
    
    if (p_tmr == &Timer1)
    {
        printf("timer1_num = %d\r\n",++timer1_num);
        if(timer1_num > 5000)
        {
            timer1_num = 0;
        }

    }
    else if (p_tmr == &Timer2)
    {
        printf("timer2_num = %d\r\n",++timer2_num);
        if(timer2_num > 5000)
        {
            timer2_num = 0;
        }
    }
    else
    {
        /* 什么都不做 */
    }
}