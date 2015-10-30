/*
 * control_motor.c
 *
 *  Created on: 22/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "control_motor.h"

void config_PWM(void)
{
	  //LPC_SC->PCLKSEL0 &=~(3<<12);      // reset
	  //LPC_SC->PCLKSEL0 |= (1<<12);      // set PCLK to full CPU speed (96MHz)
	  LPC_SC->PCONP |= (1 << 6);        // PWM on
	  LPC_PINCON->PINSEL4 |= (1 << 0); // pinsel configurado para que P2.0 sea la salida de la señal del PWM
	  LPC_PWM1->TCR = (1<<1);           // counter reset
	  //LPC_PWM1->PR  = 0;     // clock /96000000 / prescaler (= PR +1) = 1 s
	  LPC_PWM1->MCR = (1<<1);           // reset on MR0
	  LPC_PWM1->MR0 = 500000;
	  //LPC_PWM1->MR0 = 65789;                // set PWM cycle 0,25Hz (according to manual)
	  LPC_PWM1->MR1 = 0;                // set duty to 50%
	  LPC_PWM1->LER = (1<<0)|(1<<1);    // latch MR0 & MR1
	  LPC_PWM1->PCR = (1<<9);           // PWM1 output enable
	  LPC_PWM1->TCR = (1<<1)|(1<<0)|(1<<3);// counter enable, PWM enable
	  LPC_PWM1->TCR = (1<<0)|(1<<3);    // counter enable, PWM enable
}

void arrancar_motor()
{
		set_Pwm(17500);
		delay();
		set_Pwm(22500);
		delay();
		set_Pwm(28500);

}

void set_Pwm(int num)
{
	LPC_PWM1->MR1 = num; // Valor del ciclo de trabajo del PWM.
	LPC_PWM1->LER = (1<<0)|(1<<1); // PARA ACTUALIZAR MR0
	//LPC_PWM1->LER |= (1<<1); // PARA ACTUALIZAR MR1
}

void delay(void)
{
	int i=0;
	for(i=0; i<25000000; i++)
	{}
}
