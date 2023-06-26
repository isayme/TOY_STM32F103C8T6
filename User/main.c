#include "stm32f10x.h"
#include <stdio.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "delay.h"

static void Task_Breath_Led(void *arg __attribute__((unused))) {
    for (;;) {
        Led_On();
        vTaskDelay(1000);
        Led_Off();
        vTaskDelay(1000);
    }
}

int main() {
    // Delay_Configuration();
	Led_Configuration();
	
	xTaskCreate(Task_Breath_Led, "BreathLed", 32, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
	
	while (1); // not run here
}
