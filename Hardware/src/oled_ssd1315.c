#include <stdint.h>

#include "oled_ssd1315.h"
#include "iic.h"

#define WIDTH 128
#define HEIGHT 64
#define ROWS 8

// 显示屏 IIC 地址
#define OLED_SSD1315_IIC_ADDR 0x78

static uint8_t screen[WIDTH][HEIGHT >> 3] = {0};
    
void OLED_SSD1315_WriteCmd(uint8_t cmd) {
    IIC_WriteByte(OLED_SSD1315_IIC_ADDR, 0x00, cmd);
}

void OLED_SSD1315_WriteData(uint8_t data) {
    IIC_WriteByte(OLED_SSD1315_IIC_ADDR, 0x40, data);
}

//坐标设置
void OLED_SSD1315_SetPos(uint8_t x, uint8_t y) {
	OLED_SSD1315_WriteCmd(0xb0 + y);
	OLED_SSD1315_WriteCmd(x & 0x0f);
    OLED_SSD1315_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	
}

void OLED_SSD1315_On() {
    OLED_SSD1315_WriteCmd ( 0X8D);  //SET DCDC命令
    OLED_SSD1315_WriteCmd ( 0X14);  //DCDC ON
    OLED_SSD1315_WriteCmd ( 0XAF);  //DISPLAY ON
}

void OLED_SSD1315_Off() {
    OLED_SSD1315_WriteCmd ( 0X8D);  //SET DCDC命令
    OLED_SSD1315_WriteCmd ( 0X10);  //DCDC OFF
    OLED_SSD1315_WriteCmd ( 0XAE);  //DISPLAY OFF	
}


void OLED_SSD1315_Fill(uint8_t ch) {
    uint8_t x, y;
    for (y = 0; y < ROWS; y++) {
        OLED_SSD1315_SetPos(0, y);

        // 写完数据后显示屏光标会自动右移1位
        for (x = 0; x < WIDTH; x++) {
            OLED_SSD1315_WriteData(ch);
        }
    }
}

void OLED_SSD1315_Clear() {
    OLED_SSD1315_Fill(0x00);
}

void sleep(uint32_t i) {
    while(i--);
}

void OLED_SSD1315_Init() {
    sleep(1000000);
    OLED_SSD1315_WriteCmd(0xAE);//--display off
	OLED_SSD1315_WriteCmd(0x00);//---set low column address
	OLED_SSD1315_WriteCmd(0x10);//---set high column address
	OLED_SSD1315_WriteCmd(0x40);//--set start line address  
	OLED_SSD1315_WriteCmd(0xB0);//--set page address
	OLED_SSD1315_WriteCmd(0x81); // contract control
	OLED_SSD1315_WriteCmd(0xFF);//--128   
	OLED_SSD1315_WriteCmd(0xA1);//set segment remap 
	OLED_SSD1315_WriteCmd(0xA6);//--normal / reverse
	OLED_SSD1315_WriteCmd(0xA8);//--set multiplex ratio(1 to 64)
	OLED_SSD1315_WriteCmd(0x3F);//--1/32 duty
	OLED_SSD1315_WriteCmd(0xC8);//Com scan direction
	OLED_SSD1315_WriteCmd(0xD3);//-set display offset
	OLED_SSD1315_WriteCmd(0x00);//
	
	OLED_SSD1315_WriteCmd(0xD5);//set osc division
	OLED_SSD1315_WriteCmd(0x80);//
	
	OLED_SSD1315_WriteCmd(0xD8);//set area color mode off
	OLED_SSD1315_WriteCmd(0x05);//
	
	OLED_SSD1315_WriteCmd(0xD9);//Set Pre-Charge Period
	OLED_SSD1315_WriteCmd(0xF1);//
	
	OLED_SSD1315_WriteCmd(0xDA);//set com pin configuartion
	OLED_SSD1315_WriteCmd(0x12);//
	
	OLED_SSD1315_WriteCmd(0xDB);//set Vcomh
	OLED_SSD1315_WriteCmd(0x30);//
	
	OLED_SSD1315_WriteCmd(0x8D);//set charge pump enable
	OLED_SSD1315_WriteCmd(0x14);//
	
	OLED_SSD1315_WriteCmd(0xAF);//--turn on oled panel
    
    OLED_SSD1315_Clear();
}

void PLED_SSD1315_UGUI_PSet(int16_t x, int16_t y, uint32_t color) {
    uint8_t value;
    
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }
    
	value = screen[x][y>>3];

	OLED_SSD1315_WriteCmd(0xb0+(y>>3));
	OLED_SSD1315_WriteCmd(x&0x0f);
    OLED_SSD1315_WriteCmd(((x&0xf0)>>4)|0x10);
	
	if (color > 0) {
		value |= 0x01 << (y % 8);
	} else {
		value &= ~(0x01 << (y % 8));
	}
    screen[x][y/8] = value;

	OLED_SSD1315_WriteData(value);
}
