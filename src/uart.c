/*
 * uart.c
 *
 *  Created on: 30/10/2015
 *      Author: Gastón
 */

#include "headers.h"
#include "uart.h"
#include "control_motor.h"
#include "prender_motor.h"
#include "conversor.h"
#include "control_velocidad.h"

void enviar_numero(char valor);
void enviar_ok(char funcion);

uint8_t uartData = 0;
int valorsensor;
short int valorrpm;

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
	int subtotal;
	int veces;
	int j;

	uartData=LPC_UART3->RBR;

	switch(uartData){
	case 'i':
		enviar_ok('i');
		incrementar_pwm();
		break;
	case 'd':
		enviar_ok('d');
		decrementar_pwm();
		break;
	case '0':
		enviar_ok('0');
		set_Pwm(700);
		motor_apagar();

		break;
	case '1':
		enviar_ok('1');
		arrancar_motor();
		break;
	case '2':
		enviar_ok('2');
		set_Pwm(1100);
		break;
	case '3':
		enviar_ok('3');
		set_Pwm(1120);
		break;
	case '4':
		enviar_ok('4');
		set_Pwm(1140);
		break;
	case '5':
		enviar_ok('5');
		set_Pwm(1160);
		break;
	case '6':
		enviar_ok('6');
		set_Pwm(1180);
		break;
	case '7':
		enviar_ok('7');
		set_Pwm(1200);
		break;
	case '8':
		enviar_ok('8');
		set_Pwm(1220);
		break;
	case '9':
		enviar_ok('9');
		set_Pwm(1250);
		break;
	case 'r':
		valorrpm = get_RPM();
		enviar_int(valorrpm);
		break;
	case 'a':
		subtotal = 0;
		veces = 50;
		for(j=0; j<veces; j++)
		{
			subtotal = subtotal + valor_sensor();
		}
		valorsensor = subtotal / veces;
		enviar_int(valorsensor);
		break;
	}
}


//Envia un int de hasta  4 cifras
void enviar_int(int valor)
{
	int n=4;
	int i;
	char numberArray[4];

	for ( i=0; i<n; ++i)
	{
		numberArray[i] = valor % 10;
		valor = valor/10;
	}

	for ( i=4; i>0; i--)
	{
		enviar_numero(numberArray[i-1]);

	}

	enviarUART(10);
}

//Transforma un numero char en ascii
void enviar_numero(char valor)
{
	switch(valor)
	{
		case 0:
			enviarUART('0');
			break;
		case 1:
			enviarUART('1');
			break;
		case 2:
			enviarUART('2');
			break;
		case 3:
			enviarUART('3');
			break;
		case 4:
			enviarUART('4');
			break;
		case 5:
			enviarUART('5');
			break;
		case 6:
			enviarUART('6');
			break;
		case 7:
			enviarUART('7');
			break;
		case 8:
			enviarUART('8');
			break;
		case 9:
			enviarUART('9');
			break;
	}
}

void enviar_ok(char funcion)
{
	enviarUART(funcion);
	enviarUART('-');
	enviarUART('O');
	enviarUART('K');
	enviarUART(10);
}
