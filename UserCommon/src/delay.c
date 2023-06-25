#include "stm32f10x.h"
#include "delay.h"

static u8 fac_us=0;//us延时倍乘数	

void Delay_Configuration(void) {
	RCC_ClocksTypeDef RCC_Clocks;
	
	// SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	// fac_us=SystemCoreClock/8000000;	
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	// 64M-->1us:64
	RCC_GetClocksFreq(&RCC_Clocks);
	fac_us = RCC_Clocks.SYSCLK_Frequency/1000000; //fac_us=64;	
}

void Delay_Us(uint32_t t) {
	uint32_t temp;	    	 
	SysTick->LOAD = t*fac_us; 					//时间加载	  		 
	SysTick->VAL = 0x00;        					//清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do {
		temp = SysTick->CTRL;
	} while ((temp&0x01) && !(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	
}

void Delay_Ms(uint32_t t) {
	uint32_t i = 0;
	for (; i < t; i++) {
		Delay_Us(1000);
	}
}

void Delay_S(uint32_t t) {
	uint32_t i = 0;
	for (; i < t; i++) {
		Delay_Ms(1000);
	}
}
