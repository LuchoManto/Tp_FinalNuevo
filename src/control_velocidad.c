/*
 * control_velocidad.c
 *
 *  Created on: 29/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "control_motor.h"


void config_TMR0(void) // contador de eventos
{
	LPC_PINCON->PINSEL3 |= (1 << 21);
	LPC_PINCON->PINSEL3 |= (1 << 20); // habilito P1.26 como CAP0.0

	LPC_TIM0->CCR |= 1; // timer0 en modo Counter a flanco de subida
	//los bits 2 y 3 son 00, por lo que el capture se toma en el pin CAP0.0, o sea el pin 1.26


	LPC_TIM0 -> TCR |= (1 << 1);		//Reseteo timer
	LPC_TIM0 -> TCR &= ~(1 << 1);		//Reseteo timer
	LPC_TIM0 -> TCR |= (1 << 0);		//arranco timer
}

void config_TMR1(void) // base de tiempo
{
	LPC_SC->PCLKSEL0 &=~ (1 << 2);
	LPC_SC->PCLKSEL0 &=~ (1 << 3); // F = CLK/4

	LPC_TIM0 -> MCR |= (1<<3);	//que interrumpa el match1
	LPC_TIM0 -> MCR |= (1<<4);	//que resetee el timer cuando llegue

	LPC_TIM0 -> MR1 = 250000; // base de tiempo de 100ms

	NVIC_EnableIRQ(TIMER0_IRQn); //habilito interrupcion

	LPC_TIM0 -> TCR |= (1 << 1);		//Reseteo timer
	LPC_TIM0 -> TCR &= ~(1 << 1);		//Reseteo timer
	LPC_TIM0 -> TCR |= (1 << 0);		//arranco timer
}

//Handler de la base de tiempo
void TIMER1_IRQHandler(void)
{
	LPC_TIM1 -> IR |= (1<<0); // Limpio bandera de interrupcion MR1

	// aca hay que obtener el valor del TC de timer0, que basicamente tiene la cantidad de eventos en 100ms
	// una vez que hice lo que tengo que hacer con el valor del TC de timer0, lo reseteo

}
