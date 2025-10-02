#include "bsp_led_key_exti.h"
#include "bsp_delay.h"

void BSP_LED_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIOB->BSRR = (uint16_t)0x0001;
    GPIOB->BSRR = (uint16_t)0x0002;

    gpio_init_struct.Pin = LED0_GPIO_PIN;             /* LED0���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;      /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;              /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;    /* ���� */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct); /* ��ʼ��LED0���� */

    gpio_init_struct.Pin = LED1_GPIO_PIN;             /* LED1���� */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct); /* ��ʼ��LED1���� */
}

void BSP_KEY_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOH_CLK_ENABLE();
    // __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* �������� */
    gpio_init_struct.Pin = KEY0_GPIO_PIN | KEY1_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);

    // gpio_init_struct.Pin = KEY2_GPIO_PIN;
    // HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = WKUP_GPIO_PIN;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);
}

void BSP_EXTI_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio_init_struct.Pin = KEY2_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);               /* ��ռ0�������ȼ�2 */
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);                       /* ʹ���ж���3 */

}

uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up = 1; /* �������ɿ���־ */
	uint8_t keyval = 0;
	if (mode) key_up = 1; /* ֧������ */
	if (key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1))
	{
        /* �����ɿ���־Ϊ 1, ��������һ������������ */
	    Delay_ms(10); /* ȥ���� */
	    key_up = 0;
	    if (KEY0 == 0) keyval = KEY0_PRES;
	    if (KEY1 == 0) keyval = KEY1_PRES;
	    if (WK_UP == 1) keyval = WKUP_PRES;
	}
	else if (KEY0 == 1 && KEY1 == 1 && WK_UP == 0)
	{
        /* û���κΰ�������, ��ǰ����ɿ� */
	    key_up = 1;
	}
	return keyval; /* ���ؼ�ֵ */
}
