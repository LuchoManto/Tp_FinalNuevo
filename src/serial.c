/*
 * serial.c
 *
 *  Created on: 30/10/2015
 *      Author: Luciano
 */

#include "headers.h"
#include "serial.h"

uint8_t caracter = 0;
uint8_t k = 0;
uint32_t IIR_temp;

void enviar (uint8_t c)
{
	while((LPC_UART3->LSR&(1<<5))==0){}
	LPC_UART3->THR=c;
}

void SysTick_Handler(void)
{
	static int i = 100;
	i--;
	if(i==0)
	{
		i=100;
		caracter++;
		enviar(caracter);
	}
}

/*interrupción que se genera por el módulo UART3
 * la única fuente de interrupción habilitada en el módulo UART3 es la de Receive data available*/

void UART3_IRQHandler(void)
{
	k=LPC_UART3->RBR;
	enviar(k);
}

void config_UART3(void)
{

 	//-----------------------------------Configuracion del módulo UART-------------------------------------//


LPC_SC->PCONP|=1<<25;	//Encendemos el periférico

/*en este punto deberíamos asignar la frecuencia de clock del modulo UART3
pero debido a que por defecto está en PCLK_UART3=Cclock/4 lo dejaremos así de lo contrario deberíamos
ingresar al PCLKSEL1 en el que tenemos los dos bits necesarios para seleccionar
la frecuencia de Pclk para el módulo UART3 */

//-----a continuación configuraremos el formato de la trama de datos de la transmisión UART3LCR-----//

LPC_UART3->LCR|=11;
/*usaremos 1 bit de stop, pero como por defecto se tiene 1 bit de stop no es necesario configurar
parámetro*/

/*Configuramos el módulo uart para que no tenga paridad pero como por defecto está desabilitado no tocamos
este bit*/

/*También por defecto dejamos desabilitado el Break Control, por defecto está desabilitado*/


/*ahora colocamos en un 1 en al bit 7 del LCR, el bit Divisor Latch Access Bit para acceder a DLL y DLM*/
LPC_UART3->LCR|=(1<<7);

											//-----Trama de Datos-----//

/*A continuación configuramos el Baud rate, es decir, la tasa de transmisión en baudios por segundo*/
/*es decir DLL y DLM*/


/*Configuraremos DLL y DLM para obtener un Baud Rate de 9600 baudios
 * para esto tenemos */

//
//							   Pclk
//Baudrate   =    ----------------------------------
//						16 X (256*DLM+DLL)
//
//no haremos caso al divisor de frecuencia fraccional dado que el error que cometemos no es significativo
//
//
LPC_UART3->DLL=0b10100001; //DLL = 161 -> 9585 baudios -> error de 14 -> 0.1458%
LPC_UART3->DLM=0;

LPC_UART3->LCR &= 0b01111111; /*Colocamos el Bit DLAB nuevamente a cero para que
esto nos impide el acceso a DLL y DLM pero nos permite acceder a RBR, THR e IER*/

/*en IER seleccionamos la fuente de interrupción del módulo UART3 que queremos que dispare la interrupción */
LPC_UART3->IER=1; // Corresponde a la interrupción por Receive Data Available

/*Dado que solo Habilitamos una interrupción del módulo UART3 no necesitamos acceder al registro
IIR para preguntar quién interrumpio*/

//Configuramos NVIC

NVIC_EnableIRQ(UART3_IRQn);

//Configuramos los pines de RX y TX

LPC_PINCON->PINSEL0|=0b1010; //configuramos los pines P0.0 como TX y P0.1 como RX

//-----------------------------------Configuracion del módulo UART-------------------------------------//

}
