/*
 * teclado.h
 *
 *  Created on: 5/11/2015
 *      Author: Luciano
 */

#ifndef TECLADO_H_
#define TECLADO_H_



#endif /* TECLADO_H_ */

void config_eint0(void);
void config_eint1(void);
void config_eint2(void);
void led2Init (void);
void imprimir_disp(void);
void imprimir(int numero);
void config_puertos_teclado(void);
void teclado_0(void);
void decodificar(int col);
void teclado_1(void);
void config_displays(void);


void EINT0_IRQHandler(void);
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);

void config_teclado(void);
void set_velocidad_teclado();
