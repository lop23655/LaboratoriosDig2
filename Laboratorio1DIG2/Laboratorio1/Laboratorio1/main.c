/*
 * Laboratorio1.c
 *
 * Created: 15/01/2026 19:13:45
 * Author : Fabián Lopez 23655
 */ 

#define F_CPU 16000000UL
#define antirebote 60 //Generar constante para el antirebote
#include <avr/io.h>
#include <avr/interrupt.h>
#include "DisplayNum/DisplayNum.h"
#include "LibreriaConfig/LibreriaConfig.h"

typedef enum {Espera, Contando, Jugando, Finalizado} estado_juego;
	

void setup(void);

estado_juego estado = Espera;

uint8_t contadorJ1 = 0;//Contador de botonazos jugador 1
uint8_t contadorJ2 = 0;//Contador de botonazos jugador 2
uint8_t caso_DIP7 = 0;//Display 
//uint8_t inicio = 0;// Valor para inicio del juego

volatile uint8_t estado_pinb = 0xFF; // para flancos (solo 1->0)




int main(void)
{
    setup();
	
    while (1) 
    {

    }
}
/*******************************FUNCIONES**************************************/

void setup() {
	cli();
	//Colocar pueto C como salidas
	DDRC = (1<<DDC1)|(1<<DDC2)|(1<<DDC3)|(1<<DDC4)|(1<<DDC5)|(1<<DDC0);
	//Colocar PB4 y PB5 como salidas
	DDRB |= (1<<DDB4)|(1<<DDB5);
	//Colocar pueto D como salidas
	DDRD = 0xFF;
	
	setup_timer0();
	setup_timer();
	setup_interrup_boton();
	
	estado_pinb = PINB;
	sei();
}

void LEDS_J2(uint8_t c)
{
	//J2: PC5, PC5, PC3, PC2
	PORTC &= ~((1<<PORTC5)|(1<<PORTC4)|(1<<PORTC3)|(1<<PORTC2));
	
	switch (c) {
		case 1: PORTC |= (1<<PORTC5); break;
		case 2: PORTC |= (1<<PORTC4); break;
		case 3: PORTC |= (1<<PORTC3); break;
		case 4: PORTC |= (1<<PORTC2); break;
		default: break;
	}
}

void LEDS_J1(uint8_t c)
{
	// J1: PC1, PC0, PB5, PB4
	PORTC &= ~((1<<PORTC1)|(1<<PORTC0));
	PORTB &= ~((1<<PORTB5)|(1<<PORTB4));

	switch (c) {
		case 1: PORTC |= (1<<PORTC1); break;
		case 2: PORTC |= (1<<PORTC0); break;
		case 3: PORTB |= (1<<PORTB5); break;
		case 4: PORTB |= (1<<PORTB4); break;
		default: break;
	}
}

void LEDS_ganador(uint8_t ganador)
{
	estado = Finalizado; //Se cambia el estado para que ya no se puedan realizar otras acciones
	
	detener_timer();
	
	// Bloquea  los botones 
	PCICR &= ~(1 << PCIE0);
	PCIFR |= (1 << PCIF0);

	// Mostrar número del ganador en el display (1 o 2)
	contador09PD(ganador);
	
	//Encender LEDS del ganador y apagar los del perdedor
	if (ganador == 1) {
		PORTC |= (1<<PORTC1)|(1<<PORTC0);
		PORTB |= (1<<PORTB5)|(1<<PORTB4);
		PORTC &= ~((1<<PORTC5)|(1<<PORTC4)|(1<<PORTC3)|(1<<PORTC2));
		} else {
		PORTC |= (1<<PORTC5)|(1<<PORTC4)|(1<<PORTC3)|(1<<PORTC2);
		PORTC &= ~((1<<PORTC1)|(1<<PORTC0));
		PORTB &= ~((1<<PORTB5)|(1<<PORTB4));
	}
	
}

/*******************************INTERUPCIONES**************************************/
ISR(TIMER1_COMPA_vect){
	//Al utilizar empezar timer te manda aca
	//Si el display esta contando no se puede modicar
	if (estado != Contando)
	{
		detener_timer();
		return;	
	}
	
	//Si ya se mostro el 0
	if (caso_DIP7 == 0)
	{
		//Cuando el display muestre 0 se puede iniciar
		estado = Jugando; 
		detener_timer();
		
		//Habilita los botones
		estado_pinb = PINB;
		PCIFR |= (1 << PCIF0);
		PCICR |= (1 << PCIE0);
		return;
		
	}
	caso_DIP7--;
	contador09PD(caso_DIP7);
}

//Boton para inicio y los puntos para los jugadores
ISR(PCINT0_vect) {
	uint32_t ultimo_pb0 = 0;
	uint32_t ultimo_pb1 = 0;
	uint32_t ultimo_pb2 = 0;
	
	uint8_t actual = PINB;
	uint8_t cambio = actual ^ estado_pinb;//Se compara con un XOR si hubo un cambio entre los PB(si fue presionado y soltado un boton)
	uint8_t pulldown = cambio & estado_pinb & ~actual;//Evalua que se haya soltado el boton
	estado_pinb = actual;
	
	uint32_t tiempoAnti = cont_ms;
	
	if (estado == Finalizado) return;
	
	//Estado espera bloque que los demas botones actuen hasta el inicio
	if (estado == Espera){
		if (pulldown & (1<<PINB2)){
			if ((tiempoAnti - ultimo_pb2) < antirebote) return;
			ultimo_pb2 = tiempoAnti;
			
			estado = Contando;
			// Bloquea botones durante el conteo
			PCICR &= ~(1 << PCIE0);
			PCIFR |= (1 << PCIF0);

			caso_DIP7 = 5;//Comienza el conteo desde 5
			contador09PD(caso_DIP7);
			empezar_timer();
		}
		return;
		
	}
	
	//Durante el conteo ignorar todo
	if (estado == Contando) return;
	
	if (estado == Jugando)//Si ya termino el conteo del display
	{
		//Se revisa si el pinb0 ya paso el antirebote
		if (pulldown & (1<<PINB0)) {
			if ((tiempoAnti - ultimo_pb0) >= antirebote) {
				ultimo_pb0 = tiempoAnti;
				//Si el contador no ha llegado a 4 seguir sumando en el decada
				if (contadorJ2 < 5) contadorJ2++;
				LEDS_J2(contadorJ2);
				//Se llego o se paso el jugador es considerado ganador
				if (contadorJ2 >= 5) LEDS_ganador(2);
			}
		}
		//Se revisa si el pinb1 ya paso el antirebote
		if (pulldown& (1<<PINB1)) {
			if ((tiempoAnti - ultimo_pb1) >= antirebote) {
				ultimo_pb1 = tiempoAnti;

				if (contadorJ1 < 5) contadorJ1++;
				LEDS_J1(contadorJ1);

				if (contadorJ1 >= 5) LEDS_ganador(1);
			}
		}
	}
}

