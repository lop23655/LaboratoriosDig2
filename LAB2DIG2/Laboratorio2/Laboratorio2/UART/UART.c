/*
 * UART.c
 *
 * Created: 29/01/2026 11:26:34
 *  Author: fabis
 */ 
#include "UART.h"

volatile char rx_buf[RX_BUF_SIZE];// Aqui se guarda el comando recibido por la consola
volatile uint8_t rx_idx = 0; //Indice actual donde se encuentra el buffer
volatile uint8_t cmd_ready = 0;//La bandera se vuelve uno cuando se termina de escribir un comando

volatile int8_t s3_delta = 0; //Variable donde se guarda el contador de los + y -

void initUART(void) {
	// PD1 TX, PD0 RX
	DDRD |=  (1<<DDD1);//Colocando TX como salida
	DDRD &= ~(1<<DDD0);//Colocando RX como entrada

	UCSR0A = 0;// Limpia configuraciones
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);   // Habilita interrupcion cuando recibe un byte, habilita la recepciòn y transmision (RX y TX)
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);               // 8 bits, sin paridad, 1 stop (8N1)

	UBRR0  = 103; // Baud rate 9600 a 16MHz
}

void writeChar(char c) {
	while (!(UCSR0A & (1<<UDRE0)));//Espera a que el buffer de transmision este vaciò
	UDR0 = c;//Carga el byte para poder enviarlo
}

void writeString(const char *s) { 
	while (*s) writeChar(*s++); //Recore hasta que ya no se reconozca texto, envìa todos los caracteres como una oraciòn
}

int8_t uart_get_delta(void)
{
	
	uint8_t s = SREG;//Guarda el estado global de las interrupciones
	cli(); //Desahabilitamos interrupciones
	int8_t d = s3_delta; //Guarda el valor actual del contador
	s3_delta = 0;// Lo reinicia
	SREG = s; //Se restaura el estado previo a las interrupciones
	return d;// Se devuelve el valor del contador
}

ISR(USART_RX_vect) {
	char c = UDR0; //Se lee el caracter recibido

	if (c == '\r') return; // Se coloca para ignorar la continuación de la línea

	
	// '+' y '-' afectan S3 )contador)
	if (c == '+') { if (s3_delta < 127) s3_delta++; return; }//Si recibe + incrementa, evitando overflow
	if (c == '-') { if (s3_delta > -128) s3_delta--; return; } //Si recibe - decrementa, evitando underflow

	// Guarda comandos por línea en rx_buf hasta '\n'
	// Útil si luego se quiere comandos tipo "RESET\n"
	if (c == '\n' || rx_idx >= RX_BUF_SIZE - 1) {
		rx_buf[rx_idx] = '\0';
		rx_idx = 0;
		cmd_ready = 1;
		} else {
		rx_buf[rx_idx++] = c;
	}
}