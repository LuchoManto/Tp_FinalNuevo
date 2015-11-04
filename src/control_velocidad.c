/*
 * control_velocidad.c
 *
 *  Created on: 29/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "control_motor.h"
#include "uart.h"

short int rpm;

void config_control_vel()
{
	config_CAP2();
	config_TMR1();
}

short int get_RPM(void)
{
	return rpm;
}


void config_CAP2(void) // contador de eventos (usa Timer2)
{
	LPC_SC->PCONP |= (1 << 22); //habilito Timer2

	LPC_PINCON->PINSEL0 |= (1 << 8);
	LPC_PINCON->PINSEL0 |= (1 << 9); // habilito P0.4 como CAP2.0

	LPC_PINCON->PINMODE0 &= ~(1 << 8);
	LPC_PINCON->PINMODE0 |= (1 << 9); // P0.4 sin pull up ni pull down


	LPC_TIM2->CCR &= ~(1 << 0);
	LPC_TIM2->CCR &= ~(1 << 1);
	LPC_TIM2->CCR &= ~(1 << 2);

	LPC_TIM2->CTCR |= (1 << 0);
	LPC_TIM2->CTCR &= ~(1 << 1); // timer2 en modo Counter a flanco de subida

	LPC_TIM2->CTCR &= ~(1 << 2);
	LPC_TIM2->CTCR &= ~(1 << 3);
	//los bits 2 y 3 son 00, por lo que el capture se toma en el pin CAP0.0, o sea el pin P0.4

//	LPC_TIM2->MCR |= (1 << 3); // MATCH0 interrumpe
	//LPC_TIM2->MCR |= (1 << 4); //MATCH0 resetea
	//LPC_TIM2->MR1 = 5; // DESPUES DE 5 EVENTOS, SE RESETEA.

	//NVIC_EnableIRQ(TIMER2_IRQn); //habilito interrupcion

	LPC_TIM2 -> TC = 0; // reseteo TC


	LPC_TIM2 -> TCR |= (1 << 1);		//Reseteo timer
	LPC_TIM2 -> TCR &= ~(1 << 1);		//Reseteo timer
	LPC_TIM2 -> TCR |= (1 << 0);		//arranco timer
}

void config_TMR1(void) // base de tiempo
{
	LPC_SC->PCONP |= (1 << 2); //habilito Timer1

	LPC_SC->PCLKSEL0 &=~ (1 << 2);
	LPC_SC->PCLKSEL0 &=~ (1 << 3); // F = CLK/4

	LPC_TIM1 -> MCR |= (1<<3);	//que interrumpa el match1
	LPC_TIM1 -> MCR |= (1<<4);	//que resetee el timer cuando llegue

	LPC_TIM1 -> MR1 = 2500000; // base de tiempo de 1000ms

	NVIC_EnableIRQ(TIMER0_IRQn); //habilito interrupcion

	LPC_TIM1 -> TCR |= (1 << 1);		//Reseteo timer
	LPC_TIM1 -> TCR &= ~(1 << 1);		//Reseteo timer
	LPC_TIM1-> TCR |= (1 << 0);		//arranco timer
}

//Handler de la base de tiempo
void TIMER1_IRQHandler(void)
{
	LPC_TIM1 -> IR |= (1<<0); // Limpio bandera de interrupcion MR1

	rpm = (short int)LPC_TIM2 -> TC * 60;

	LPC_TIM2 -> TC = 0; // reseteo timer2

	// aca hay que obtener el valor del TC de timer0, que basicamente tiene la cantidad de eventos en 100ms
	// una vez que hice lo que tengo que hacer con el valor del TC de timer0, lo reseteo

}
