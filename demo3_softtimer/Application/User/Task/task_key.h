#ifndef __TASK_KEY_H
#define __TASK_KEY_H

#include "main.h"

/* Task_Pend 任务 配置
 * 包括: 任务优先级 任务栈大小 任务控制块 任务栈 任务函数
 */
#define TASK_KEY_PRIO      (OS_CFG_PRIO_MAX - 10)           /* 任务优先级 */
#define TASK_KEY_STK_SIZE  1024                            /* 任务栈大小 */

extern OS_TCB Task_Key_TCB;                              /* 任务控制块 */
extern CPU_STK Task_Key_STK[TASK_KEY_STK_SIZE];         /* 任务栈 */

void Task_Key(void *p_arg);

#endif  /* __TASK_KEY_H */
