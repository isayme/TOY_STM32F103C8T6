#ifndef __LED_H
#define __LED_H

#include <stdint.h>

void Led_Configuration(void);

void Led_On(void);

void Led_Off(void);

uint8_t Led_IsOn(void);

uint8_t Led_IsOff(void);

#endif
