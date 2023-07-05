#ifndef __IIC_H
#define __IIC_H

#include <stdint.h>

void IIC_Configuration(void);

void IIC_WriteByte(uint8_t slaveAddr, uint8_t addr, uint8_t data);
void IIC_WriteBuffer(uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t bLen);

uint8_t IIC_ReadByte(uint8_t slaveAddr, uint8_t addr, uint8_t data);
void IIC_ReadBuffer(uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t bLen);


#endif
