#ifndef __PWM_H
#define __PWM_H

#include <stdint.h>

/**
 * 周期 = 70000000/(arr + 1)/(psc + 1)
  * 50Hz = 70000000/(59999 + 1)/(23 + 1)
 * @param arr 周期
 * @param psc 分频系数
 */
void Pwm_Init(uint16_t arr, uint16_t psc);

// void Pwn_

#endif //STM32STD_PWM_H
