/*
 * PWM.h
 *
 * Created: 30/04/2025 16:00:07
 *  Author: fabis
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <stdint.h>     // <-- necesario para uint8_t, uint16_t
#include <avr/interrupt.h>

// --- ADC ---
void adc_init(void);
uint16_t adc_read(uint8_t ch);
void format_voltage(char out[6], uint16_t adc);

void initServoTimer1();
void updateServoPulse(uint16_t pulso);

void initServoTimer12();
void updateServoPulse2(uint16_t pulso2);

void initServoTimer2();
void updateServoPulse3(uint16_t pulso);
void updateServoPulse4(uint16_t pulso);
#endif /* PWM0_H_ */