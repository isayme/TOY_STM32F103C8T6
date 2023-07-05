#ifndef __OLED_SSD1315_H
#define __OLED_SSD1315_H

#include <stdint.h>

// 屏幕宽高
#define OLED_SSD1315_WIDTH 128
#define OLED_SSD1315_HEIGHT 64

void OLED_SSD1315_Init(void);
void OLED_SSD1315_On(void);
void OLED_SSD1315_Off(void);

void OLED_SSD1315_Fill(uint8_t ch);
void OLED_SSD1315_Clear(void);

void OLED_SSD1315_WriteData(uint8_t data);
void OLED_SSD1315_SetPos(uint8_t x, uint8_t y);

void PLED_SSD1315_UGUI_PSet(int16_t x, int16_t y, uint32_t color);

#endif
