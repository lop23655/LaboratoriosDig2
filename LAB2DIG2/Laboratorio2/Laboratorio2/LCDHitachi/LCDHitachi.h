/*
 * LCDHitahci.h
 *
 * Created: 22/01/2026 19:28:25
 *  Author: fabis
 */ 


#ifndef LCDHITACHI_H_
#define LCDHITACHI_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void initLCD8bits(void);

void LCD_Port(uint8_t a);
void LCD_CMD(uint8_t cmd);
void LCD_Write_Char(char c);
void LCD_Write_String(const char *s);

void LCD_Shift_Left(void);
void LCD_Shift_Right(void);

void LCD_Set_Cursor(uint8_t col, uint8_t row);

#endif
