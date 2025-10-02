#ifndef __TASK_KEY_H
#define __TASK_KEY_H

#include "main.h"

/* Task_Pend ���� ����
 * ����: �������ȼ� ����ջ��С ������ƿ� ����ջ ������
 */
#define TASK_KEY_PRIO      (OS_CFG_PRIO_MAX - 10)           /* �������ȼ� */
#define TASK_KEY_STK_SIZE  1024                            /* ����ջ��С */

extern OS_TCB Task_Key_TCB;                              /* ������ƿ� */
extern CPU_STK Task_Key_STK[TASK_KEY_STK_SIZE];         /* ����ջ */

void Task_Key(void *p_arg);

#endif  /* __TASK_KEY_H */
