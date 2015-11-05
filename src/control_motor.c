/*
 * control_motor.c
 *
 *  Created on: 22/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "control_motor.h"

int pwm = 0;

void config_PWM(void)
{
	  LPC_SC->PCONP |= (1 << 6);        // PWM on
	  LPC_PINCON->PINSEL4 |= (1 << 0); // pinsel configurado para que P2.0 sea la salida de la señal del PWM
	  LPC_PWM1->TCR = (1<<1);           // counter reset
	  LPC_PWM1->MCR = (1<<1);           // reset on MR0
	  LPC_PWM1->MR0 = 500000;			//50Hz
	  LPC_PWM1->MR1 = 0;                // set duty to 50%
	  LPC_PWM1->LER = (1<<0)|(1<<1);    // latch MR0 & MR1
	  LPC_PWM1->PCR = (1<<9);           // PWM1 output enable
	  LPC_PWM1->TCR = (1<<1)|(1<<0)|(1<<3);// counter enable, PWM enable
	  LPC_PWM1->TCR = (1<<0)|(1<<3);    // counter enable, PWM enable
}

void arrancar_motor(void)
{
		set_Pwm(700); //700us
		delay();
		set_Pwm(900);	//900us
		delay();
		set_Pwm(1100);	//1100us
}

void set_Pwm(int num)
{
	pwm = num;
	num = num*25;
	LPC_PWM1->MR1 = num; // Valor del ciclo de trabajo del PWM.
	LPC_PWM1->LER = (1<<0)|(1<<1); // PARA ACTUALIZAR MR0
}

void incrementar_pwm(void)
{
	if(pwm < 1250)
	{
		pwm = pwm + 10;
		set_Pwm(pwm);
	}
}

void decrementar_pwm(void)
{
	if(pwm > 1100)
	{
		pwm = pwm - 10;
		set_Pwm(pwm);
	}
}

int get_pwm(void)
{
	return pwm;
}

void delay(void)
{
	int i=0;
	for(i=0; i<25000000; i++)
	{}
}

void delay1(int delays)
{
	int i=0;
	for(i=0; i<delays; i++)
	{}
}
