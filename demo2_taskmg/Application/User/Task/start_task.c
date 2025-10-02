#include "start_task.h"
#include "bsp_delay.h"
#include "bsp_uart.h"
#include "bsp_led_key_exti.h"
#include "string.h"
#include "stdio.h"

//TASK header file
#include "task_key.h"

extern OS_TCB StartTask_TCB;                  /* ����1������ƿ� */
char cpusage[30];

/* TASK1 ���� ����
 * ����: �������ȼ� ����ջ��С ������ƿ� ����ջ ������
 */
#define TASK1_PRIO      (OS_CFG_PRIO_MAX - 5)           /* �������ȼ� */
#define TASK1_STK_SIZE  1024                            /* ����ջ��С */
OS_TCB                  Task1_TCB;                  /* ������ƿ� */
CPU_STK                 Task1_STK[TASK1_STK_SIZE];
void Task1(void *p_arg);                                /* ������ */

/* TASK1 ���� ����
 * ����: �������ȼ� ����ջ��С ������ƿ� ����ջ ������
 */
#define TASK2_PRIO      (OS_CFG_PRIO_MAX - 4)           /* �������ȼ� */
#define TASK2_STK_SIZE  1024                            /* ����ջ��С */
OS_TCB                  Task2_TCB;                  /* ������ƿ� */
CPU_STK                 Task2_STK[TASK2_STK_SIZE];
void Task2(void *p_arg);   

void func_demo2_task_cpu_usage(void);                           /* ������ */

/* USER CODE BEGIN 4 */
void Start_Task(void *p_arg)
{
    OS_ERR err;
    CPU_INT32U cnts;


    /* ��ʼ��CPU�� */
    CPU_Init();
    
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OSCfg_TickRate_Hz);
    OS_CPU_SysTickInit(cnts);
    
    /* ����ʱ��Ƭ���ȣ�ʱ��Ƭ��ΪĬ��ֵ */
    OSSchedRoundRobinCfg(OS_TRUE, 0, &err);

#if DEMO2_TASK_CPU_USAGE
    /* OS_CFG_DBG_EN & CPU_CFG_TS_EN & CPU_CFG_TS_32_EN ����Ҫʹ�� */
    OSStatTaskCPUUsageInit(&err);
    if (err != OS_ERR_NONE) {
        printf("CPU Usage Init Error\r\n");
    }
#endif

    App_OS_SetAllHooks();

    OSTaskCreate(   (OS_TCB        *)&Task1_TCB,
                    (CPU_CHAR      *)"task1",
                    (OS_TASK_PTR    )Task1,
                    (void          *)0,
                    (OS_PRIO        )TASK1_PRIO,
                    (CPU_STK       *)Task1_STK,
                    (CPU_STK_SIZE   )TASK1_STK_SIZE / 10,
                    (CPU_STK_SIZE   )TASK1_STK_SIZE,
                    (OS_MSG_QTY     )0,
                    (OS_TICK        )0,
                    (void          *)0,
                    (OS_OPT         )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                    (OS_ERR        *)&err);

    OSTaskCreate(   (OS_TCB        *)&Task2_TCB,
                    (CPU_CHAR      *)"task2",
                    (OS_TASK_PTR    )Task2,
                    (void          *)0,
                    (OS_PRIO        )TASK2_PRIO,
                    (CPU_STK       *)Task2_STK,
                    (CPU_STK_SIZE   )TASK2_STK_SIZE / 10,
                    (CPU_STK_SIZE   )TASK2_STK_SIZE,
                    (OS_MSG_QTY     )0,
                    (OS_TICK        )0,
                    (void          *)0,
                    (OS_OPT         )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                    (OS_ERR        *)&err);
    
    OSTaskCreate(   (OS_TCB        *)&Task_Key_TCB,
                    (CPU_CHAR      *)"task_key",
                    (OS_TASK_PTR    )Task_Key,
                    (void          *)0,
                    (OS_PRIO        )TASK_KEY_PRIO,
                    (CPU_STK       *)Task_Key_STK,
                    (CPU_STK_SIZE   )TASK_KEY_STK_SIZE / 10,
                    (CPU_STK_SIZE   )TASK_KEY_STK_SIZE,
                    (OS_MSG_QTY     )0,
                    (OS_TICK        )0,
                    (void          *)0,
                    (OS_OPT         )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                    (OS_ERR        *)&err);

    OSTaskDel((OS_TCB *)0, &err);
}

void Task1(void *p_arg)
{
	OS_ERR err;

	while(1)
	{ 
        printf("task1 is running\r\n");
#if DEMO2_TASK_CPU_USAGE
        func_demo2_task_cpu_usage();
#endif
        // Delay_ms(500);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err); 
	}
}

void Task2(void *p_arg)
{
	OS_ERR err;
    
	while(1)
	{ 
        printf("task2 is running\r\n");

        // Delay_ms(500);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err); 
	}
}

void func_demo2_task_cpu_usage(void)
{
    sprintf(cpusage, "CPU Usage: %d.%d%%\tIdle task Usage: %d.%d%%\r\n", \
    OSStatTaskCPUUsage/100, OSStatTaskCPUUsage%100, \
    OSIdleTaskTCB.CPUUsageMax/100, OSIdleTaskTCB.CPUUsageMax%100);
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cpusage, strlen(cpusage));
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET)
        ;
    sprintf(cpusage, "Task1 Usage: %d.%d%%\tTask2 Usage: %d.%d%%\r\n", \
    Task1_TCB.CPUUsageMax/100, Task1_TCB.CPUUsageMax%100, \
    Task2_TCB.CPUUsageMax/100, Task2_TCB.CPUUsageMax%100);
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cpusage, strlen(cpusage));
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET)
        ;
}