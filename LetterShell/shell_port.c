/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "shell.h"
#include "stm32f10x.h"
#include "uart.h"
//#include "cevent.h"
//#include "log.h"
#include "ugui.h"

#define SHELL_BUFFER_LEN 128

Shell shell;
char shellBuffer[SHELL_BUFFER_LEN];

static SemaphoreHandle_t shellMutex;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    UART_SendBuffer(data, len);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    short r = UART_RecvBuffer(data, len);
    return r;
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
    //shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, SHELL_BUFFER_LEN);
    if (xTaskCreate(shellTask, "shell", SHELL_BUFFER_LEN, &shell, tskIDLE_PRIORITY+1, NULL) != pdPASS)
    {
        printf("shell task creat failed");
    }
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

/**
 * 串口中断函数，名称不可自定义
 */
void USART1_IRQHandler(void) {
   if (USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET){
       uint8_t ch = USART_ReceiveData( USART1 );
       
       shellHandler(&shell, ch);
	} 
}

int tick(int i, char ch, char *str)
{
    printf("Hello, tick %d\r\n", xTaskGetTickCountFromISR());
    return 0;
}

//导出到命令列表里
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), tick, tick, tick);
