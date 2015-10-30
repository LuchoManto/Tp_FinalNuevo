/*
 * conversor.c
 *
 *  Created on: 22/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "conversor.h"

int bandera_adc;

void config_adc()
{
	LPC_SC->PCONP |= (1 << 12); // habilitar el periferico PCONP para que funcione el ADC, por defecto esta deshabilitado

	LPC_SC->PCLKSEL0 &= ~(1 << 24);
	LPC_SC->PCLKSEL0 &= ~(1 << 25); // selecciono clock de adc en sysclock / 4

	LPC_ADC->ADCR |= (1 << 21); // coloca el ADC en modo operacional

	LPC_ADC->ADCR |= (1 << 8); // coloco el divisor de clock del adc en 1 para que se divida 25 / 2 y de 12,5 que es < 13

	LPC_PINCON->PINSEL0 |= (1 << 14);
	LPC_PINCON->PINSEL0 &= ~(1 << 15); //bits 14 y 15 de pinsel0 en "01" hacen que P0.23 sea AD0.0

	LPC_PINCON->PINMODE1 |= (1 << 15); //configuro el pin de entrada de modo que no tenga ni pull down ni pull up

	LPC_ADC->ADCR |= (1 << 16); //habilita el bit de interrupcion del ADC
	LPC_ADC->ADCR |= (1 << 26); //comenzar conversion cuando hay flanco en MATCH 1 de Timer0
	LPC_ADC->ADCR &= ~(1 << 27); //flanco de subida activa la conversion

	NVIC_IRQEnable (ADC_IRQn);
}


void ADC_IRQHandler()
{
	bandera_adc = 1;

//	LPC_ADC->ADSTAT &= ~ (1<<16); // se baja la bandera de interrupcion del adc
	if(LPC_ADC->ADSTAT & 1)
		LPC_ADC->ADDR0; // si la bandera que se subio era la del canal 0, se baja el bit de DONE correspondiente


}

uint8_t valor ()
{
	int valor_convertido;
	float valor_real;
	uint8_t valor_enviar = 0;

	if (bandera_adc == 1)
	{
		bandera_adc = 0;
		valor_convertido = (int)LPC_ADC->ADDR0;

		// valor real == (Vref+ - Vref-) * valor convertido /(2^resolucion - 1)
		// valor real == 2*valor convertido / 4095

		valor_real = (2*valor_convertido) / 4095;
		valor_enviar = valor_real;
	}
	return valor_enviar;
}


