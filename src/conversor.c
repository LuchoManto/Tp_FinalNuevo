/*
 * conversor.c
 *
 *  Created on: 22/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "conversor.h"


int bandera_adc;

void config_adc(void)
{
	LPC_PINCON->PINSEL1 |= (1<<14); //Configuro el pin P0.23 para que funcione como entrada analogica para el AD0.0
	LPC_PINCON->PINSEL1 &=~ (1<<15);

	LPC_SC->PCONP |= (1 << 12); // habilitar el periferico PCONP para que funcione el ADC, por defecto esta deshabilitado

	LPC_SC->PCLKSEL0 &= ~(1 << 24);
	LPC_SC->PCLKSEL0 &= ~(1 << 25); // selecciono clock de adc en sysclock / 4

	LPC_ADC->ADCR |= (1 << 21); // coloca el ADC en modo operacional

	LPC_ADC->ADCR |= (1 << 8); // coloco el divisor de clock del adc en 1 para que se divida 25 / 2 y de 12,5 que es < 13

	LPC_PINCON->PINSEL0 |= (1 << 14);
	LPC_PINCON->PINSEL0 &= ~(1 << 15); //bits 14 y 15 de pinsel0 en "01" hacen que P0.23 sea AD0.0

	//LPC_PINCON->PINMODE1 |= (1 << 15); //configuro el pin de entrada de modo que no tenga ni pull down ni pull up

	LPC_ADC->ADCR |= (1 << 16); //habilita el bit de interrupcion del ADC
	//LPC_ADC->ADCR |= (1 << 26); //comenzar conversion cuando hay flanco en MATCH 1 de Timer0
	LPC_ADC->ADCR &= ~(1 << 27); //flanco de subida activa la conversion

	//NVIC_IRQEnable (ADC_IRQn);
}


/*void ADC_IRQHandler()
{
	bandera_adc = 1;

//	LPC_ADC->ADSTAT &= ~ (1<<16); // se baja la bandera de interrupcion del adc
	if(LPC_ADC->ADSTAT & 1)
		LPC_ADC->ADDR0; // si la bandera que se subio era la del canal 0, se baja el bit de DONE correspondiente

}
*/

//comience conversion, mida, envie el dato y desactivar el ADC.

int valor (void)
{

	int valor_convertido;
	float valor_real;
	int valor_enviar = 0;

	LPC_ADC->ADCR |= (1 << 21); // coloca el ADC en modo operacional

	LPC_ADC->ADCR |= (1<<24);
	LPC_ADC->ADCR &=~ (1<<25); //STAR CONVERSION NOW.
	LPC_ADC->ADCR &=~ (1<<26);

	while (!(LPC_ADC->ADDR0 & (1<<31)))
	{

	}
	valor_convertido = 0xFFF & ((int)LPC_ADC->ADDR0 >> 4); //meto en valor_convertido los bit entre 4 y 15 del ADDR0.

	// valor real == (Vref+ - Vref-) * valor convertido /(2^resolucion - 1)
	// valor real == 2*valor convertido / 4095

	valor_real = (2*valor_convertido) / 4095;
	valor_enviar = valor_real;

	LPC_ADC->ADCR &=~ (1 << 21); // coloca el ADC en modo operacional

	LPC_ADC->ADCR |= (1<<24);
	LPC_ADC->ADCR &=~ (1<<25);  //STOP ADC.
	LPC_ADC->ADCR &=~ (1<<26);

	return valor_enviar;
}


