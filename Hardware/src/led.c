#include "stm32f10x.h"
#include "led.h"

#define LED_
void Led_Configuration() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Led_On() {
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void Led_Off() {
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
