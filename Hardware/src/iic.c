#include <stdio.h>

#include "stm32f10x.h"
#include "iic.h"
#include "uart.h"

// 主机地址，用不到则随便写，无影响
#define HOST_ADDR 0x88
// 总线速度，400K，根据自己所用芯片手册查看支持的速度。	
#define BUS_SPEED 400000

void IIC_GPIO_Configuration() {
    GPIO_InitTypeDef  GPIO_InitStructure;               //定义结构体配置GPIO
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);	//使能I2C1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟
    /*STM32F103芯片的硬件I2C1: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		//设置输出模式为开漏输出，需接上拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化GPIO
}

void IIC_Configuration() {
    I2C_InitTypeDef  I2C_InitStructure;                 //定义结构体配置I2C
    
    IIC_GPIO_Configuration();
    
    I2C_DeInit(I2C1);	//将外设I2C寄存器重设为缺省值
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;			//工作模式
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//时钟占空比，Tlow/Thigh = 2
	I2C_InitStructure.I2C_OwnAddress1 = HOST_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	//使能应答位
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//设置地址长度7位
	I2C_InitStructure.I2C_ClockSpeed = BUS_SPEED;
	I2C_Init(I2C1, &I2C_InitStructure);         //初始化I2C
    
    I2C_Cmd(I2C1, ENABLE);  //启用I2C
    
    UART_SendString("iic init, host addr");
    printf("0x%02x, bus speed: %d\r\n", HOST_ADDR, BUS_SPEED);
}


void IIC_GenerateStart() {
    while( I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));//等待总线空闲
    I2C_GenerateSTART(I2C1, ENABLE);//发送开始信号
    while( I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);//检测EV5事件
}

void IIC_GenerateStop() {
    I2C_GenerateSTOP(I2C1, ENABLE);//发送停止信号
}

void IIC_WriteByte(uint8_t slaveAddr, uint8_t addr, uint8_t data) {
    uint8_t buf[1] = {0};
    buf[0] = data;
    
    IIC_WriteBuffer(slaveAddr, addr, buf, 1);
}

void IIC_WriteBuffer(uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t bLen){
    IIC_GenerateStart();
    
	//发送设备写地址
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	//检测EV6事件
	while( I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//发送要操作设备内部的地址
	I2C_SendData(I2C1, addr);
	//检测EV8_2事件
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
    while (bLen--) {
        I2C_SendData(I2C1, *buf);//发送数据
        buf++;
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
	
	IIC_GenerateStop();
}

uint8_t IIC_ReadByte(uint8_t slaveAddr, uint8_t addr, uint8_t data){
    // IIC_GenerateStart();
    return 0;
}

void IIC_ReadBuffer(uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t bLen){
}
