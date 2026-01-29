#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "LCDHitachi/LCDHitachi.h" //Funciones para habilitar los pines de la pantalla Hitachi, para 8 bytes
#include "PWM/PWM.h"   // Funciones para modificar ADC
#include "UART/UART.h"   // <-- importante

void u16_to_str4(uint16_t val, char out[5]);
void s3_to_str(int16_t v, char out[6]);


int main(void)
{
	initLCD8bits();//Setear las funciones del Hitachi
	adc_init();//Setear las funciones para poder lear los puertos en ADC
	initUART();//Setear las funciones para la comunicacion serial
	sei();

	LCD_Set_Cursor(1,1);
	LCD_Write_String("S1: ");
	LCD_Set_Cursor(10,1); LCD_Write_String("S2:");
	LCD_Set_Cursor(1,2); LCD_Write_String("S3:");
	
	int16_t s3 = 0;

	while (1)
	{
		// Aplicar + / - recibidos
		int8_t d = uart_get_delta();
		if (d != 0) s3 += d;

		// Leer potenciómetros
		uint16_t s1_adc = adc_read(0); // A0
		uint16_t s2_adc = adc_read(1); // A1

		// Formatear
		char s1_txt[6];
		format_voltage(s1_txt, s1_adc);  // "0.00V"

		char s2_txt[5];
		u16_to_str4(s2_adc, s2_txt);     // "   0".."1023"

		char s3_txt[6];
		s3_to_str(s3, s3_txt);           // "+0000"

		// Mostrar en LCD (layout que cabe)
		// Línea1: "S1:0.00V S2:"
		LCD_Set_Cursor(4,1);  LCD_Write_String(s1_txt);   // col4
		LCD_Set_Cursor(13,1); LCD_Write_String("   ");    // limpia restos por si acaso
		// Línea1 col10 ya dice S2:
		LCD_Set_Cursor(13,1); LCD_Write_String(s2_txt);   // col13 (4 chars)

		// Línea2: "S3:+0000"
		LCD_Set_Cursor(4,2);  LCD_Write_String(s3_txt);

		// Enviar por UART
		writeString("S1=");
		writeString(s1_txt);
		writeString(" S2=");
		writeString(s2_txt);
		writeString(" S3=");
		writeString(s3_txt);
		writeString("\r\n");

		_delay_ms(200);
	}
}

void u16_to_str4(uint16_t val, char out[5])
{
	// "   0" .. "1023" (4 chars + '\0')
	out[0] = ' '; out[1] = ' '; out[2] = ' '; out[3] = ' '; out[4] = '\0';
	int i = 3;
	do { out[i--] = (val % 10) + '0'; val /= 10; } while (val && i >= 0);
}

void s3_to_str(int16_t v, char out[6])
{
	if (v > 9999)  v = 9999;
	if (v < -9999) v = -9999;

	out[0] = (v < 0) ? '-' : '+';
	if (v < 0) v = -v;

	out[1] = '0' + (v / 1000) % 10;
	out[2] = '0' + (v / 100)  % 10;
	out[3] = '0' + (v / 10)   % 10;
	out[4] = '0' + (v % 10);
	out[5] = '\0';
}

