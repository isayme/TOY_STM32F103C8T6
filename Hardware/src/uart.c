#include <stdio.h>

#include "stm32f10x.h"
#include "uart.h"

void UART_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 开时钟（USART1 TX-A9 RX-A10）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化GPIO
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 中断
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; // 子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 初始化USART
    USART_InitStruct.USART_BaudRate = bound;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);

    // 开启/关闭串口中断
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    
    // 使能串口
    USART_Cmd(USART1, ENABLE);
    
    printf("uart1 enabled, bound: %d\r\n", bound);
}

/**
 * 串口中断函数，名称不可自定义
 */
void USART1_IRQHandler(void) {
    
}

void UART_SendData(uint8_t data) {
    USART_SendData(USART1, data);
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));	
}

void UART_SendString(char* string)
{
	char *str = string;
	while(*str) {
		UART_SendData(*str);
		str++;
	}		
}

// printf 重定向到串口1，需勾选 Use Micro Lib
int fputc(int ch, FILE *f)
{
	// UART_SendData(ch);
    USART_SendData(USART1, (unsigned char) ch);
    while (!(USART1->SR & USART_FLAG_TXE));
    
	return ch;
}
