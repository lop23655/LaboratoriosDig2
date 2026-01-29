/*
 * LCDHitachio.c
 *
 * Created: 22/01/2026 19:28:38
 *  Author: fabis
 */ 
#include "LCDHitachi.h"

/* RS = PD2, E = PD3 */
#define LCD_RS_PORT PORTD
#define LCD_RS_DDR  DDRD
#define LCD_RS_PIN  PD2

#define LCD_E_PORT  PORTD
#define LCD_E_DDR   DDRD
#define LCD_E_PIN   PD3

static inline void lcd_pulse_enable(void)
{
    LCD_E_PORT |=  (1 << LCD_E_PIN);
    _delay_us(1);	
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(50);
}

/*
  Mapeo según tu imagen:
  D0..D3 -> PD4..PD7
  D4..D7 -> PB0..PB3
*/
void LCD_Port(uint8_t a)
{
    // PD4..PD7 = bits 0..3
    PORTD = (PORTD & 0x0F) | ((a & 0x0F) << 4);

    // PB0..PB3 = bits 4..7
    PORTB = (PORTB & 0xF0) | ((a >> 4) & 0x0F);
}

void LCD_CMD(uint8_t cmd)
{
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN); // RS=0
    LCD_Port(cmd);
    lcd_pulse_enable();

    if (cmd == 0x01 || cmd == 0x02) _delay_ms(2);
    else _delay_us(50);
}

void LCD_Write_Char(char c)
{
    LCD_RS_PORT |= (1 << LCD_RS_PIN); // RS=1
    LCD_Port((uint8_t)c);
    lcd_pulse_enable();
    _delay_us(50);
}

void LCD_Write_String(const char *s)
{
    while (*s) LCD_Write_Char(*s++);
}

void LCD_Shift_Left(void)  { LCD_CMD(0x18); }
void LCD_Shift_Right(void) { LCD_CMD(0x1C); }

void LCD_Set_Cursor(uint8_t col, uint8_t row)
{
    uint8_t addr = (row == 1) ? 0x00 : 0x40;
    if (col < 1) col = 1;
    addr += (col - 1);
    LCD_CMD(0x80 | addr);
}

void initLCD8bits(void)
{
    // Control como salida
    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    LCD_E_DDR  |= (1 << LCD_E_PIN);

    // Datos como salida:
    // PD4..PD7 (nibble bajo) y PB0..PB3 (nibble alto)
    DDRD |= (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);
    DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3);

    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    LCD_E_PORT  &= ~(1 << LCD_E_PIN);

    _delay_ms(50);   // más seguro que 20ms

    // Secuencia del datasheet (8-bit init): 0x30 x3
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN); // RS=0

    LCD_Port(0x30);
    lcd_pulse_enable();
    _delay_ms(10);

    LCD_Port(0x30);
    lcd_pulse_enable();
    _delay_us(200);

    LCD_Port(0x30);
    lcd_pulse_enable();
    _delay_us(200);

    // Configuración final
    LCD_CMD(0x38); // 8-bit, 2 líneas, 5x8
    LCD_CMD(0x08); // display off
    LCD_CMD(0x01); // clear
    LCD_CMD(0x06); // entry mode
    LCD_CMD(0x0C); // display on
}
