#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


void BSP_UART_Init(void);
void BSP_UART_DMA_Init(void);

#define USART_REC_LEN 1
#define RX_BUFFER_SIZE 256

typedef struct
{
    uint8_t Buffer[RX_BUFFER_SIZE];
    uint8_t Data;
    uint16_t DataCnt;
} UART_RxTypeDef;

extern UART_RxTypeDef Uart1Rx; // 为UART2声明外部结构体变量
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern uint16_t Uart1_RxSign;
extern uint8_t Uart1_RxBuffer[USART_REC_LEN];


#ifdef __cplusplus
}
#endif

#endif
