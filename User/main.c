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
#include "lcd_st7789v2.h"
#include "shell_port.h"

static UG_GUI oledGUI;

static UG_GUI lcdGUI;

static void Task_Breath_Led(void *arg __attribute__((unused))) {
    uint16_t delay = 1000;
    
    for (;;) {
        Led_On();
        vTaskDelay(delay);
        Led_Off();
        vTaskDelay(delay);
    }
}

/**
 * 初始化任务
 */
//static void Task_Init(void *arg __attribute__((unused))) {  
//    
//    UART_SendString("os init task start\r\n");
//    
//    Led_Configuration();
//    
//    IIC_Configuration();
//    
//    Delay_Configuration();

////    LCD_ST7789V2_Init();
////    UG_Init(&lcdGUI, LCD_ST7789V2_DrawPoint, 240, 320);
////    UG_FillScreen(C_BLACK);
////    UG_FontSelect(&FONT_6X8);
////    UG_PutString(23, 24, "Hello World!");
////    
//    OLED_SSD1315_Init();
//    UG_Init(&oledGUI, PLED_SSD1315_UGUI_PSet, 128, 64);
//    UG_FontSelect(&FONT_6X8);
//    
//    UG_PutString(23, 24, "Hello World!");
//    
//    
//   
//    
//    xTaskCreate(Task_Breath_Led, "BreathLed", 10, NULL, tskIDLE_PRIORITY + 2, NULL);
//    UART_SendString("os started task BreathLed\r\n");
//  
//    // 任务完成，删除自身
//    vTaskDelete(NULL);

//    UART_SendString("os init task exit\r\n");
//}

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
    RCC_Configuration();
    
    UART_Init(UART_BOUND_115200);

    UART_SendString("os init task start\r\n");
    
    Led_Configuration();
    
    IIC_Configuration();
    
    Delay_Configuration();
        
    LCD_ST7789V2_Init();
    UG_Init(&lcdGUI, LCD_ST7789V2_DrawPoint, LCD_W, LCD_H);
    UG_FillScreen(C_BLACK);
    UG_FontSelect(&FONT_6X8);
    UG_ConsoleSetArea(0, 0, LCD_W, LCD_H);
    UG_ConsoleSetForecolor(C_WHITE);
    UG_ConsoleSetBackcolor(C_BLACK);
    
//    OLED_SSD1315_Init();
//    UG_Init(&oledGUI, PLED_SSD1315_UGUI_PSet, 128, 64);
//    UG_FontSelect(&FONT_6X8);
//    
//    UG_PutString(23, 24, "Hello World!");

  
    
    userShellInit();

    xTaskCreate(Task_Breath_Led, "BreathLed", 10, NULL, tskIDLE_PRIORITY + 2, NULL);
	//xTaskCreate(Task_Init, "Init", 20, NULL, tskIDLE_PRIORITY + 1, NULL);
   
	vTaskStartScheduler();
	
	while (1); // never run here
}
