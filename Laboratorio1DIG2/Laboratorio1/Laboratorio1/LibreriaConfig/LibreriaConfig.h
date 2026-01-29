/*
 * LibreriaConfig.h
 *
 * Created: 21/01/2026 17:43:28
 *  Author: fabis
 */ 


#ifndef LIBRERIACONFIG_H_
#define LIBRERIACONFIG_H_

#include <avr/io.h>

void setup_timer(void);//Funcion para preparar timer1
void empezar_timer(void);
void detener_timer(void);
void setup_interrup_boton(void);//Funcion para preparar botones

void setup_timer0(void);
extern volatile uint32_t cont_ms; //Contador de milisegundos


#endif /* LIBRERIACONFIG_H_ */