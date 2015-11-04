/*
 * prender_motor.c
 *
 *  Created on: 4/11/2015
 *      Author: Luciano
 */
#define MOTOR 5

void motorOn_init()
{
	LPC_GPIO0->FIODIR |= (1 << MOTOR); // pin P0.5 habilitado

	LPC_PINCON->PINMODE0 |= (1 << 10);
	LPC_PINCON->PINMODE0 |= (1 << 11); // pin P0.5 en modo pull-down
}
void motor_prender()
{
	LPC_GPIO0->FIOSET |= (1 << MOTOR);
}
void motor_apagar()
{
	LPC_GPIO0->FIOCLR |= (1 << MOTOR);
}
