#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"

int main() {
	Delay_Configuration();
	Led_Configuration();
	
	while (1) {
		Led_On();
		Delay_S(1);
		Led_Off();
		Delay_S(1);
	}
}
