#ifndef __LCD_7789V2_H
#define __LCD_7789V2_H		

#include <stdint.h>

void LCD_ST7789V2_Init(void);//LCD初始化
void LCD_ST7789V2_DrawPoint(int16_t x,int16_t y,uint16_t color);//在指定位置画一个点

#endif
