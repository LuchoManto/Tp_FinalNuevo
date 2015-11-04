/*
 * conversor.c
 *
 *  Created on: 22/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "conversor.h"


int bandera_adc;
int valor_convertido;
float valor_real;
int valor_enviar;


void config_adc(void)
{
	LPC_PINCON->PINSEL1 |= (1<<14); //Configuro el pin P0.23 para que funcione como entrada analogica para el AD0.0
	LPC_PINCON->PINSEL1 &=~ (1<<15);

	LPC_SC->PCONP |= (1 << 12); // habilitar el periferico PCONP para que funcione el ADC, por defecto esta deshabilitado

	LPC_SC->PCLKSEL0 &= ~(1 << 24);
	LPC_SC->PCLKSEL0 &= ~(1 << 25); // selecciono clock de adc en sysclock / 4

	LPC_ADC->ADCR |= (1 << 21); // coloca el ADC en modo power-down.

	LPC_ADC->ADCR |=(1<<16); //ADC en modo Burst.

	LPC_ADC->ADCR |= (1 << 8); // coloco el divisor de clock del adc en 1 para que se divida 25 / 2 y de 12,5 que es < 13

	LPC_PINCON->PINSEL0 |= (1 << 14);
	LPC_PINCON->PINSEL0 &= ~(1 << 15); //bits 14 y 15 de pinsel0 en "01" hacen que P0.23 sea AD0.0

	LPC_PINCON->PINMODE1 |= (1 << 15); //configuro el pin de entrada de modo que no tenga ni pull down ni pull up
}

int valor_sensor(void)
{
	int valor;

	int valor_real;

	valor =  (LPC_ADC->ADDR0 >> 4) & 0xFFF ; //meto en valor_convertido los bit entre 4 y 15 del ADDR0.

	// valor real == (Vref+ - Vref-) * valor convertido /(2^resolucion - 1)
	// valor real == 2*valor convertido / 4095

	valor_real = (3300*valor) / 4095;

	return valor_real;
}




