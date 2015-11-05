/*
 * prender_motor.c
 *
 *  Created on: 4/11/2015
 *      Author: Luciano
 */

#define MOTOR 5
#include "headers.h"

void motorOn_init()
{
	LPC_GPIO0->FIODIR |= (1 << MOTOR); // pin P0.5 habilitado
	motor_apagar(); // apagado desde el inicio
}
void motor_prender()
{
	LPC_GPIO0->FIOSET |= (1 << MOTOR);
}
void motor_apagar()
{
	LPC_GPIO0->FIOCLR |= (1 << MOTOR);
}
