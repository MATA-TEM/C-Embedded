/*
 * servo32.c
 *
 * Created: 09/03/2025 22:23:54
 * Author : NBODTKKU
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <stdlib.h>
#include "timer2_millis.h"

#define SL_ADDR 0x01
#define SERVO_PIN PD5       // OC1A ที่ PD5
#define REQUEST_INTERVAL 200
#define DEADBAND 2
#define ANGLE_TO_PULSE(angle) ((angle * 11) + 500)

volatile uint8_t current_angle = 90;  // มุมเริ่มต้น 90 องศา
volatile uint8_t new_angle = 90;
volatile uint8_t sending = 0;
volatile uint64_t last_request_time = 0;

uint64_t millis(void) {
	uint64_t m;
	cli();
	m = __millis;
	sei();
	return m;
}

ISR(TWI_vect) {
	switch (TWSR & 0xFC) {
		case TW_START:
		sending = 1;
		TWDR = (SL_ADDR << 1) | 0x01;
		TWCR &= ~(1 << TWSTA);
		break;
		case TW_MR_SLA_ACK:
		TWCR &= ~((1 << TWSTA) | (1 << TWSTO));
		break;
		case TW_MR_DATA_ACK:
		sending = 0;
		new_angle = TWDR * 180 / 255; // แปลง 0-255 เป็น 0-180 องศา
		TWCR |= (1 << TWSTO);
		TWCR &= ~(1 << TWSTA);
		break;
	}
	TWCR |= (1 << TWINT);
}

void PWM_init() {
	// Fast PWM, ICR1 as TOP, Non-inverting mode
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8
	
	ICR1 = 40000 - 1; // คาบเวลา 20ms (50Hz)
	DDRD |= (1 << PD5);
	OCR1A = ANGLE_TO_PULSE(90) * 2; // ค่าเริ่มต้น 90 องศา
}

void update_servo() {
	static uint8_t Tem_angle = 90;	
	if (abs((int8_t)new_angle - (int8_t)Tem_angle) > DEADBAND) {
		Tem_angle = new_angle;
		current_angle = new_angle;
		uint16_t pulse = ANGLE_TO_PULSE(current_angle);
		OCR1A = pulse * 2; // แปลงเป็นค่า Timer1 (40000/20000 = 2)
	}
}

int main(void) {
	init_timer2_millis();
	PWM_init();
	
	PORTC |= (1 << PC0) | (1 << PC1);
	TWBR = 0x12;  // 400kHz @ 16MHz
	TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWIE);

	sei();

	while (1) {
		uint64_t current_time = millis();

		if (current_time - last_request_time >= REQUEST_INTERVAL && !sending) {
			last_request_time = current_time;
			TWCR |= (1 << TWSTA);
		}

		update_servo();
	}
}

























