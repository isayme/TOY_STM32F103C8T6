#ifndef __UART_H
#define __UART_H

#include <stdint.h>

#define UART_BOUND_115200 115200
#define UART_BOUND_9600 9600

/**
 * bound: 波特率，如 115200, 9600
 */
void UART_Init(uint32_t bound);

void UART_SendData(uint8_t data);
void UART_SendString(char* string);

#endif
