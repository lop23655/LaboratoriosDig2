/*
 * UART.h
 *
 * Created: 29/01/2026 11:26:46
 *  Author: fabis
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define RX_BUF_SIZE 32//Tamaño de los comandos que se puede poner por lìnea

void initUART(void);//Inicializa el UART y las interrupciones
void writeChar(char c);// Envía 1 carácter por UART 
void writeString(const char *s);// Envía un string por UART

// Devuelve el acumulado de '+' y '-' recibido desde la última lectura y lo reinicia a 0
int8_t uart_get_delta(void);

// Buffer de comandos por línea 
volatile char rx_buf[RX_BUF_SIZE];
volatile uint8_t cmd_ready;

#endif