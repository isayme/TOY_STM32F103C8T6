#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

void Delay_Configuration(void);

void Delay_Us(uint32_t t);

void Delay_Ms(uint32_t t);

void Delay_S(uint32_t t);

#endif
