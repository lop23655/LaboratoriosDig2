/*
 * CFile1.c
 *
 * Created: 15/01/2026 19:20:21
 *  Author: Fabián Lopez 23655
 */ 

#include "DisplayNum.h"

void contador09PD(uint8_t num)
{
	//Con las conexiones del display en PD1 en c, PD0 en  GND, PD2 en d, PD3 en e, PD4 en g, PD5 en f, PD6 en a, PD7 en b

		switch (num)
		{
			case 5:
			//Display 5
			PORTD |=  (1<<PORTD6);
			PORTD |=  (1<<PORTD5);
			PORTD |=  (1<<PORTD4);
			PORTD |=  (1<<PORTD1);
			PORTD |=  (1<<PORTD2);
			PORTD &= ~(1<<PORTD0);
			PORTD &= ~(1<<PORTD3);
			PORTD &= ~(1<<PORTD7);
			break;

			case 4:
			//Display 4 f g c b encendido
			PORTD &=  ~(1<<PORTD6);
			PORTD |=  (1<<PORTD5);
			PORTD |=  (1<<PORTD4);
			PORTD |=  (1<<PORTD1);
			PORTD &=  ~(1<<PORTD2);
			PORTD &= ~(1<<PORTD0);
			PORTD &= ~(1<<PORTD3);
			PORTD |=  (1<<PORTD7);
			break;

			case 3:
			//Display 3 f e apagado
			PORTD |=  (1<<PORTD6);
			PORTD &=  ~(1<<PORTD5);
			PORTD |=  (1<<PORTD4);
			PORTD |=  (1<<PORTD1);
			PORTD |=  (1<<PORTD2);
			PORTD |=  (1<<PORTD0);
			PORTD &= ~(1<<PORTD3);
			PORTD |=  (1<<PORTD7);
			break;

			case 2:
			//Display 2 f c apagado
			PORTD |=  (1<<PORTD6);
			PORTD &=  ~(1<<PORTD5);
			PORTD |=  (1<<PORTD4);
			PORTD &=  ~(1<<PORTD1);
			PORTD |=  (1<<PORTD2);
			PORTD |=  (1<<PORTD0);
			PORTD |=  (1<<PORTD3);
			PORTD |=  (1<<PORTD7);
			break;

			case 1:
			//Display 1 b c encendido
			PORTD &= ~(1<<PORTD6);
			PORTD &= ~(1<<PORTD5);
			PORTD &= ~(1<<PORTD4);
			PORTD |=  (1<<PORTD1);
			PORTD &= ~(1<<PORTD2);
			PORTD &= ~(1<<PORTD0);
			PORTD &= ~(1<<PORTD3);
			PORTD |=  (1<<PORTD7);
			break;

			case 0:
			//Display 0 g apagado
			PORTD |=  (1<<PORTD6);
			PORTD |=  (1<<PORTD5);
			PORTD &=  ~(1<<PORTD4);
			PORTD |=  (1<<PORTD1);
			PORTD |=  (1<<PORTD2);
			PORTD |=  (1<<PORTD0);
			PORTD |=  (1<<PORTD3);
			PORTD |=  (1<<PORTD7);
			break;

			default:
			break;
		}

	
}