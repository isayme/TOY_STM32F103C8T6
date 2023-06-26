#include "stm32f10x.h"
#include "led.h"

void Led_Configuration() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	Led_Off();
}

void Led_On() {
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void Led_Off() {
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

uint8_t Led_IsOn() {
	return Led_IsOff() ? 0 : 1;
}

uint8_t Led_IsOff() {
	return GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) > 0 ? 1 : 0;
}
