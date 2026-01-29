/*
 * PWM.c
 *
 * Created: 30/04/2025 15:59:00
 *  Author: fabis
 */ 

//#include "PWM.h"
//#define TIMEREF 5000UL   // 5000 mV (si AVcc=5V). Si es 3.3V usa 3300UL.
/*void initServoTimer1()
{
	// PB1 (OC1A) como salida
	DDRB |= (1 << DDB1);

	// Modo 14: Fast PWM con ICR1 como TOP
	TCCR1A = (1 << WGM11); // WGM11 = 1
	TCCR1B = (1 << WGM13) | (1 << WGM12); // WGM13 y WGM12 = 1

	// No invertido en OC1A: COM1A1 = 1, COM1A0 = 0
	TCCR1A |= (1 << COM1A1) ;

	// Establecer TOP en ICR1 para 20 ms (50 Hz)
	// f = 16 MHz / 8 = 2 MHz, 20 ms = 40000 ticks
	ICR1 = 39999;

	// Prescaler = 8
	TCCR1B |= (1 << CS11);
}

void updateServoPulse(uint16_t pulso)
{
	// pulse_width entre 1000 (1ms) y 2000 (2ms)
	OCR1A = pulso;
}

void initServoTimer12()
{
	// PB2 (OC1B) como salida
	DDRB |= (1 << DDB2);
	
	TCCR1A |= (1 << COM1B1);
}

void updateServoPulse2(uint16_t pulso2)
{
	// pulse_width entre 1000 (1ms) y 2000 (2ms)
	OCR1B = pulso2;
}

void initServoTimer2()
{
	// PB3 (OC2A) y PD3 (OC2B) como salidas
	DDRB |= (1<<DDB3);
	DDRD |= (1<<DDD3);

	// Fast PWM 8-bit, no invertido en OC2A/OC2B
	TCCR2A = (1<<WGM20)|(1<<WGM21)
	| (1<<COM2A1)|(1<<COM2B1);
	// prescaler = 1024 ? 61 Hz frame, tick = 64 µs
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
}

// Escala y carga OCR2A (servo3)
void updateServoPulse3(uint16_t pulso)
{
	OCR2A = pulso;
}

// Escala y carga OCR2B (servo4)
void updateServoPulse4(uint16_t pulso)
{
	OCR2B = pulso;
}
*/

#include "PWM.h"

#define TIMEREF 5000UL   // mV (AVcc=5V). Cambia a 3300UL si es 3.3V

void adc_init(void)
{
	ADMUX  = (1 << REFS0); // AVcc como referencia
	ADCSRA = (1 << ADEN)  |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
}

uint16_t adc_read(uint8_t ch)
{
	ADMUX = (ADMUX & 0xF0) | (ch & 0x0F); // selecciona canal

	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)) { }
	return ADC;
}

void format_voltage(char out[6], uint16_t adc)
{
	uint32_t mv = ((uint32_t)adc * TIMEREF + 511) / 1023;

	uint8_t v_int = (uint8_t)(mv / 1000);
	uint16_t dec2 = (uint16_t)((mv % 1000) / 10);

	out[0] = '0' + v_int;
	out[1] = '.';
	out[2] = '0' + (dec2 / 10);
	out[3] = '0' + (dec2 % 10);
	out[4] = 'V';
	out[5] = '\0';
}