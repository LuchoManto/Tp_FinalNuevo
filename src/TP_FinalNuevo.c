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


int main(void) {

    LPC_GPIO0->FIODIR |= (1<<22);
	LPC_GPIO0->FIOCLR |= (1<<22);

	configUART();

	LPC_GPIO0->FIOSET |= (1<<22);

	while(1);

    return 0 ;
}


