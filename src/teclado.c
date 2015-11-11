/*
 * teclado.c
 *
 *  Created on: 5/11/2015
 *      Author: Luciano
 */


/*
===============================================================================
 Name        : teclado.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "headers.h"
#include "teclado.h"
#include "control_motor.h"
#include "prender_motor.h"
#include "uart.h"

#define cuenta 1000000




//Puertos teclado
const int FILAS_T[4] = {5,6,7,8};
const int COL_T[3] = {10,11,12};


void config_teclado(void) {
	//Pongo en salida los 4 puertos del teclado.
	config_puertos_teclado();
	//Las pongo en 0
	teclado_0();
	//Entrada de las columnas del teclado
	config_eint0();
	config_eint1();
	config_eint2();
}


// Configuramos EINT1.
void config_eint1(void)
{
	// Pin como input
	LPC_GPIO2->FIODIR &= ~(1 << 11);

	// Configure pinsel para P2[11] EINT1
	LPC_PINCON->PINSEL4 |= (1 << 22);
	LPC_PINCON->PINSEL4 &= ~(1 << 23);

	// Pongo el puerto interrupcion por flanco
	LPC_SC->EXTMODE |= (1 << 1);

	// Pongo la interrupcion activa por flanco bajo.
	LPC_SC->EXTPOLAR &= ~(1 << 1);

	// Activo la interrupcion
	NVIC_EnableIRQ(EINT1_IRQn);

	return;
}

// Configuramos EINT0.
void config_eint0(void)
{
	// Pin como input
	LPC_GPIO2->FIODIR &= ~(1 << 10);

	// Configure pinsel para P2[10] EINT0
	LPC_PINCON->PINSEL4 |= (1 << 20);
	LPC_PINCON->PINSEL4 &= ~(1 << 21);

	// Pongo el puerto interrupcion por flanco
	LPC_SC->EXTMODE |= (1 << 0);

	// Pongo la interrupcion activa por flanco bajo.
	LPC_SC->EXTPOLAR &= ~(1 << 0);

	// Activo la interrupcion
	NVIC_EnableIRQ(EINT0_IRQn);

	return;
}

// Configuramos EINT2
void config_eint2(void)
{
	// Pin como input
	LPC_GPIO2->FIODIR &= ~(1 << 12);

	// Configure pinsel para P2[12] EINT2
	LPC_PINCON->PINSEL4 |= (1 << 24);
	LPC_PINCON->PINSEL4 &= ~(1 << 25);

	// Pongo el puerto interrupcion por flanco
	LPC_SC->EXTMODE |= (1 << 2);

	// Pongo la interrupcion activa por flanco bajo.
	LPC_SC->EXTPOLAR &= ~(1 << 2);

	// Activo la interrupcion
	NVIC_EnableIRQ(EINT2_IRQn);

	return;
}

// Handler de la interrupcion por boton.
void EINT0_IRQHandler(void) {
	decodificar(0);
	LPC_GPIO0 ->FIOSET = (1<<22);

	while(!(LPC_GPIO2->FIOPIN & (1<<COL_T[0]))){}

	LPC_SC->EXTINT |= (1 << 0); //bajamos la bandera al salir
}

// Handler de la interrupcion por boton.
void EINT1_IRQHandler(void) {
	decodificar(1);
	LPC_GPIO0 ->FIOSET = (1<<22);

	while(!(LPC_GPIO2->FIOPIN & (1<<COL_T[1]))){}

	LPC_SC->EXTINT |= (1 << 1); //bajamos la bandera al salir
}

// Handler de la interrupcion por boton.
void EINT2_IRQHandler(void) {
	decodificar(2);
	LPC_GPIO0 ->FIOSET = (1<<22);

	while(!(LPC_GPIO2->FIOPIN & (1<<COL_T[2]))){}

	LPC_SC->EXTINT |= (1 << 2); //bajamos la bandera al salir
}


//Pongo como entrada los puertos del teclado.
void config_puertos_teclado(void)
{
	LPC_GPIO2->FIODIR |= (1 << FILAS_T[0]); // 5
	LPC_GPIO2->FIODIR |= (1 << FILAS_T[1]); // 6
	LPC_GPIO2->FIODIR |= (1 << FILAS_T[2]); // 7
	LPC_GPIO2->FIODIR |= (1 << FILAS_T[3]); // 8
}

//Hago que el teclado tire 0 por sus salidas.
void teclado_0(void)
{
	LPC_GPIO2->FIOCLR = (1 << FILAS_T[0]);
	LPC_GPIO2->FIOCLR = (1 << FILAS_T[1]);
	LPC_GPIO2->FIOCLR = (1 << FILAS_T[2]);
	LPC_GPIO2->FIOCLR = (1 << FILAS_T[3]);
}

//Pongo en 1 las salidas del teclado
void teclado_1(void)
{
	LPC_GPIO2->FIOSET = (1 << FILAS_T[0]);
	LPC_GPIO2->FIOSET = (1 << FILAS_T[1]);
	LPC_GPIO2->FIOSET = (1 << FILAS_T[2]);
	LPC_GPIO2->FIOSET = (1 << FILAS_T[3]);
}



//Funcion para ver que tecla se presiono.
void decodificar(int col)
{
	LPC_GPIO0 ->FIOCLR = (1<<22);

	//Pongo todas las salidas en 1.(filas)
	teclado_1();
	//Pongo en 0 la primera fila.
	int i;
	for(i=0; i<3; i++)
	{
		LPC_GPIO2->FIOCLR = (1 << FILAS_T[i]);
		delay1(10000);
		//Hay 0 en la columna que corresponde?
		if(!(LPC_GPIO2->FIOPIN & (1<<COL_T[col])))
		{
			teclado_0();
			set_velocidad_teclado((col+1)+(i*3));
			return;
		}
	}
	LPC_GPIO2->FIOCLR = (1 << FILAS_T[3]);
	if(!(LPC_GPIO2->FIOPIN & (1<<COL_T[col])))
	{
		teclado_0();
		if(col == 0)
		{
			set_velocidad_teclado(10); // tecla *
			return;
		}
		if(col == 1)
		{
			set_velocidad_teclado(0);
			return;
		}
		if(col == 2)
		{
			set_velocidad_teclado(11); // tecla #
			return;
		}
	}
	return;

}


void set_velocidad_teclado(int vel)
{
	switch(vel)
	{
			case 0:
				set_Pwm(700);
				motor_apagar();
				break;
			case 1:
				arrancar_motor();
				break;
			case 2:
				set_Pwm(1100);
				break;
			case 3:
				set_Pwm(1120);
				break;
			case 4:
				set_Pwm(1140);
				break;
			case 5:
				set_Pwm(1160);
				break;
			case 6:
				set_Pwm(1180);
				break;
			case 7:
				set_Pwm(1200);
				break;
			case 8:
				set_Pwm(1220);
				break;
			case 9:
				set_Pwm(1250);
				break;
			case 10:
				decrementar_pwm();
				break;
			case 11:
				incrementar_pwm();
				break;
	}
}

