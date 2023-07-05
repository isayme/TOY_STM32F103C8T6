#include "stm32f10x.h"
#include <stdio.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"
#include "iic.h"
#include "oled_ssd1315.h"
#include "ugui.h"
#include "uart.h"

static void Task_Breath_Led(void *arg __attribute__((unused))) {
    uint16_t delay = 1000;
    
    for (;;) {
        Led_On();
        vTaskDelay(delay);
        Led_Off();
        vTaskDelay(delay);
    }
}

void RCC_Configuration() {
    RCC_DeInit(); // 复位 RCC 寄存器
    
    RCC_HSEConfig(RCC_HSE_ON); // 打开外部高速晶振
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); // 等待高速晶振就绪
    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // 使能 flash 预读
    FLASH_SetLatency(FLASH_Latency_2); // flash 操作延时
    
    // AHB, AP2 and AP1 clock are necessary for the peripherals to function
    // HCLK for AHB = SYSCLK (max is SYSCLK, up to 72MHz)
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    
    // PCLK2 for APB2 = HCLK (max is SYSCLK, up to 72MHz)
    RCC_PCLK2Config(RCC_HCLK_Div1);
    
    // PCLK1 for APB1 = HCLK (HCLK <= 36MHz)
    RCC_PCLK1Config(RCC_HCLK_Div2);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //设置ADC时钟频率
    
    // 配置锁相环时钟 PLLCLK = HSE * RCC_PLLMul_x
    // PLLCLK = 8MHz * 9 = 72MHz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    // Enable PLL
    RCC_PLLCmd(ENABLE);
    // Wait till PLL is ready
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);      //将PLL设置为系统时钟源  /* Select PLL as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);           //等待系统时钟源的启动  /* Wait till PLL is used as system clock source */
}

int main() {
    UG_GUI pGUI;

    RCC_Configuration();
    IIC_Configuration();
    
    // Delay_Configuration();
	Led_Configuration();

    OLED_SSD1315_Init();
    
    UART_Init(UART_BOUND_115200);
    
    UG_Init(&pGUI, PLED_SSD1315_UGUI_PSet, 128, 64);
    UG_FontSelect(&FONT_6X8);
    
    UG_PutString(23, 24, "Hello World!");
    
    printf("Hello with UART, bound: %d\n", UART_BOUND_115200);

	xTaskCreate(Task_Breath_Led, "BreathLed", 32, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();
	
	while (1); // never run here
}
