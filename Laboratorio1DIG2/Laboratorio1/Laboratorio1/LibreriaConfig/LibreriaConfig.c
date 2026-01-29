/*
 * LibreriaConfig.c
 *
 * Created: 21/01/2026 17:43:03
 *  Author: fabis
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t cont_ms = 0; 

void setup_timer(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	
	//Activar el modo CTC(clear timer on compare) que me dicta que al ser iguales se reinician
	TCCR1B |= (1 << WGM12);
	// Se carga el valor para igualar a 1 segundo
	//Velocidad micro 16,000,000Hz/ prescaler 1024 - 1 = 15, 624
	OCR1A = 15624;
	// Habilita interrupción por comparación
	TIMSK1 |= (1 << OCIE1A);
}

void empezar_timer(void) {
	TCNT1 = 0;//El valor que cuenta los ticks del timer
	TIFR1 |= (1 << OCF1A);
	TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
}

void detener_timer(void) {
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Detiene el Timer
}

ISR(TIMER0_COMPA_vect){
	cont_ms++;
}

void setup_timer0(void)
{
	// CTC
	TCCR0A = (1 << WGM01);
	TCCR0B = 0;

	// 16MHz / 64 = 250kHz -> 1ms = 250 ticks -> OCR0A = 249
	OCR0A = 249;
	TCNT0 = 0;

	// Limpia flag compare
	TIFR0 |= (1 << OCF0A);

	// Habilita interrupción compare A
	TIMSK0 |= (1 << OCIE0A);

	// Arranca Timer0 prescaler 64
	TCCR0B |= (1 << CS01) | (1 << CS00);
}


void setup_interrup_boton(void) {
	// PB0 y PB1 entradas con pull-up
	DDRB  &= ~((1<<DDB0) | (1<<DDB1) | (1<<DDB2));
	PORTB |=  (1<<PORTB0) | (1<<PORTB1)| (1<<PORTB2);
	
	//Activa interrupciones en todas las entradas del puerto B
	PCICR |= (1 << PCIE0);
	//Habilita la interrupcion cuando ocurre un cambio en la entrada
	// Habilita PCINT0 (PB0), PCINT1 (PB1), PCINT2 (PB2)
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2);
	
	
}
	
