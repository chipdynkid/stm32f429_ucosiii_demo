#include "task_key.h"
#include "bsp_uart.h"
#include "bsp_led_key_exti.h"
#include "string.h"


extern OS_TCB Task1_TCB;                  /* 任务1任务控制块 */
extern OS_TCB Task2_TCB;                  /* 任务2任务控制块 */
OS_TCB                  Task_Key_TCB;                  /* 任务控制块 */
CPU_STK                 Task_Key_STK[TASK_KEY_STK_SIZE];

void func_demo2_task_pend_prio(uint8_t key);

void Task_Key(void *p_arg)
{
    OS_ERR err;
    uint8_t key = 0;
    while(1)
    {   
        key = KEY_Scan(0);
        if(key == KEY0_PRES)
        {
            func_demo2_task_pend_prio(key);
        }
        else if(key == KEY1_PRES)
        {
            func_demo2_task_pend_prio(key);
        }
        OSTimeDly(100,OS_OPT_TIME_DLY,&err);
    }
}

// pend & change prio
void func_demo2_task_pend_prio(uint8_t key)
{
#if DEMO2_TASK_ChangePrio
    OS_ERR err;
#endif
#if DEMO2_TASK_ChangePrio
    char task_key_temp[30];
#endif
    if (key == KEY0_PRES)
    {
#if DEMO2_TASK_PEND
        OSTaskSuspend(&Task1_TCB, &err);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"suspend task1\r\n", strlen("suspend task1\r\n"));
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET)
            ;
#endif
#if DEMO2_TASK_ChangePrio
        OSTaskChangePrio(   (OS_TCB    *)&Task1_TCB,
                            (OS_PRIO    )OS_CFG_PRIO_MAX - 4,
                            (OS_ERR    *)&err);
        OSTaskChangePrio(   (OS_TCB    *)&Task2_TCB,
                            (OS_PRIO    )OS_CFG_PRIO_MAX - 5,
                            (OS_ERR    *)&err);
        sprintf(task_key_temp, "task1 prio: %d, task2 prio: %d\r\n", Task1_TCB.Prio, Task2_TCB.Prio);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)task_key_temp, strlen(task_key_temp));
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET)
            ;
#endif
    }
    else if (key == KEY1_PRES)
    {
#if DEMO2_TASK_PEND
        OSTaskResume(&Task1_TCB, &err);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"resume task1\r\n", strlen("resume task1\r\n"));
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET)
            ;
#endif
#if DEMO2_TASK_ChangePrio
        OSTaskChangePrio(   (OS_TCB    *)&Task1_TCB,
                            (OS_PRIO    )OS_CFG_PRIO_MAX - 5,
                            (OS_ERR    *)&err);
        OSTaskChangePrio(   (OS_TCB    *)&Task2_TCB,
                            (OS_PRIO    )OS_CFG_PRIO_MAX - 4,
                            (OS_ERR    *)&err);
        sprintf(task_key_temp, "task1 prio: %d, task2 prio: %d\r\n", Task1_TCB.Prio, Task2_TCB.Prio);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)task_key_temp, strlen(task_key_temp));
        while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET)
            ;
#endif
    }
}