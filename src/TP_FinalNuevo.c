/*
===============================================================================
 Name        : TP_FinalNuevo.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "headers.h"
#include "uart.h"
#include "control_motor.h"
#include "conversor.h"
#include "control_velocidad.h"



int main(void) {

    LPC_GPIO0->FIODIR |= (1<<22);
	LPC_GPIO0->FIOCLR |= (1<<22);

	motorOn_init();
	motor_prender();
	//config_CAP2();
	//config_TMR1();
	config_PWM();
	config_adc();
	configUART();

	LPC_GPIO0->FIOSET |= (1<<22);

	while(1);

    return 0 ;
}


