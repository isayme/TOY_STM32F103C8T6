#include <stdio.h>
#include "FreeRTOS.h"
#include "stm32f10x.h"
#include "delay.h"

static u8 fac_us = 0; //us延时倍乘数	

void Delay_Configuration(void) {
    RCC_ClocksTypeDef RCC_Clocks;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	// 72M-->1us: 72
	RCC_GetClocksFreq(&RCC_Clocks);
	fac_us = RCC_Clocks.SYSCLK_Frequency/1000000; //fac_us= 72;
    
    fac_us = SystemCoreClock / 1000000;
    
    printf("delay configuration done\r\n");
}

void Delay_Us_Without_FreeRTOS(uint32_t nus) {
    u32 temp;
    SysTick->LOAD=fac_us*nus;         //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;         //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;                //清零计数器
    do {
        temp=SysTick->CTRL;           //读取当前倒计数值
    } while((temp&0x01)&&(!(temp&(1<<16))));     //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}
// https://www.cnblogs.com/KingZhan/p/16534656.html
void Delay_Us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,reload,tcnt=0;
    
    if((0x0001&(SysTick->CTRL)) ==0)    //定时器未工作
    {
        Delay_Us_Without_FreeRTOS(nus);
        return;
    }
    
    reload = SysTick->LOAD;                     //获取重装载寄存器值
    ticks = nus * (SystemCoreClock / 1000000);  //计数时间值   括号里的代表1us秒嘀嗒定时器的value会向下降多少值
    told=SysTick->VAL;                          //获取当前数值寄存器值（开始时数值）

    while(1)
    {
          tnow=SysTick->VAL;          //获取当前数值寄存器值
          if(tnow!=told)              //当前值不等于开始值说明已在计数
          {         

                 if(tnow<told)             //当前值小于开始数值，说明未计到0
                      tcnt+=told-tnow;     //计数值=开始值-当前值

                 else                  //当前值大于开始数值，说明已计到0并重新计数
                        tcnt+=reload-tnow+told;   //计数值=重装载值-当前值+开始值  （已
                                                  //从开始值计到0） 
                 told=tnow;                //更新开始值
                 if(tcnt>=ticks)break;     //时间超过/等于要延迟的时间,则退出.
          } 
    }     
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
