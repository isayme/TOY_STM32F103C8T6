#ifndef __LCD_7789V2_H
#define __LCD_7789V2_H		

#include <stdint.h>

/**
 * 显示器驱动芯片：ST7789V2
 */
#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0 || USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320
#else
#define LCD_W 320
#define LCD_H 240
#endif

void LCD_ST7789V2_Init(void);//LCD初始化
void LCD_ST7789V2_DrawPoint(int16_t x,int16_t y,uint16_t color);//在指定位置画一个点

#endif
