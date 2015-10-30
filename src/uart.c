/*
 * uart.c
 *
 *  Created on: 30/10/2015
 *      Author: Gastón
 */

#include "headers.h"
#include "uart.h"
#include "control_motor.h"
#include "conversor.h"

uint8_t uartData = 0;


void configUART(void)
{
	LPC_SC->PCONP|=1<<25;	//Encendemos el periférico

	LPC_UART3->LCR|=11;
	/*usaremos 1 bit de stop, pero como por defecto se tiene 1 bit de stop no es necesario configurar
	parámetro*/

	/*ahora colocamos en un 1 en al bit 7 del LCR, el bit Divisor Latch Access Bit para acceder a DLL y DLM*/
	LPC_UART3->LCR|=(1<<7);

	//							   Pclk
	//Baudrate   =    ----------------------------------
	//						16 X (256*DLM+DLL)

	LPC_UART3->DLL=150; //DLL = 162 -> 9600 baudios
	LPC_UART3->DLM=0;
	LPC_UART3->FDR|= (1<<0);
	LPC_UART3->FDR|= (1<<7);
	LPC_UART3->FDR|= (1<<6);

	LPC_UART3->LCR &= 0b01111111; /*Colocamos el Bit DLAB nuevamente a cero para que
	esto nos impide el acceso a DLL y DLM pero nos permite acceder a RBR, THR e IER*/

	LPC_UART3->IER=1; // Corresponde a la interrupción por Receive Data Available

	//Configuramos NVIC
	NVIC_EnableIRQ(UART3_IRQn);

	LPC_PINCON->PINSEL0|=0b1010; //configuramos los pines P0.0 como TX y P0.1 como RX
}


void enviarUART(uint8_t c)
{
	while((LPC_UART3->LSR&(1<<5))==0){}
	LPC_UART3->THR=c;
}

void UART3_IRQHandler(void)
{
	uartData=LPC_UART3->RBR;
	enviarUART(uartData);
	int valor1;
	switch(uartData){
	case 43: //+
		incrementar_pwm();
		break;
	case 45: //-
		decrementar_pwm();
		break;
	case 48: //0
		set_Pwm(700);
		break;
	case 49: //1
		arrancar_motor();
		break;
	case 50: //2
		set_Pwm(1100);
		break;
	case 51: //3
		set_Pwm(1120);
		break;
	case 52: //4
		set_Pwm(1140);
		break;
	case 53: //5
		set_Pwm(1160);
		break;
	case 54:  //6
		set_Pwm(1180);
		break;
	case 55:  //7
		set_Pwm(1200);
		break;
	case 56: //8
		set_Pwm(1220);
		break;
	case 57: //9
		set_Pwm(1250);
		break;
	case 114:
		get_RPM();
		break;
	}
}
