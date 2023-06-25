#include "stm32f10x.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"

static void Task_Led_On(void *arg __attribute__((unused))) {

    for (;;) {
        if (Led_IsOff()) {
					vTaskDelay(500);
					Led_On();
					vTaskDelay(500);
				} else {
					vTaskDelay(50);
				}
    }
}

static void Task_Led_Off(void *arg __attribute__((unused))) {
    for (;;) {
        if (Led_IsOn()) {
					vTaskDelay(500);
					Led_Off();
					vTaskDelay(500);
				} else {
					vTaskDelay(50);
				}
    }
}

int main() {
	// Delay_Configuration();
	Led_Configuration();
	
	xTaskCreate(Task_Led_Off, "ledOff", 32, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(Task_Led_On, "ledOn", 32, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
	
	while (1); // not run here
}
