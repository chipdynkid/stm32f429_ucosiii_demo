#ifndef TASK_SOFTTIMER_H
#define TASK_SOFTTIMER_H

#include "main.h"

void Software_Timer_Callback(void *p_tmr, void *p_arg);

extern OS_TMR  Timer1; /* �����ʱ�� */
extern OS_TMR  Timer2; /* �����ʱ�� */

#endif // TASK_SOFTTIMER_H