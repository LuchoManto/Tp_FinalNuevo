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

	//LPC_ADC->ADCR |= (1 << 16); //habilita el bit de interrupcion del ADC
	//LPC_ADC->ADCR |= (1 << 26); //comenzar conversion cuando hay flanco en MATCH 1 de Timer0
	//LPC_ADC->ADCR &= ~(1 << 27); //flanco de subida activa la conversion


}
float valor (void)
{
	int valor;
	uint8_t valor_convertido;
	uint8_t valor_convertido1;

	float valor_real;
	float valor_enviar = 0;

	valor =  (LPC_ADC->ADDR0 >> 4) & 0xFFF ; //meto en valor_convertido los bit entre 4 y 15 del ADDR0.

	valor_convertido =  valor & 0xFF;
	valor_convertido1 =  (valor >> 8) & 0xF;
	// valor real == (Vref+ - Vref-) * valor convertido /(2^resolucion - 1)
	// valor real == 2*valor convertido / 4095

	valor_real = (3.3*valor) / 4095;
	valor_enviar = valor_real;

	return valor_enviar;
}


