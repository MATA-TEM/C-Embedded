/*
 * servo8.c
 *
 * Created: 09/03/2025 15:41:09
 * Author : NBODTKKU
 */ 



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "uart_printf.h"
#include "timer2_millis.h"

#define SL_ADDR 0x01
uint8_t volatile data = 1;


uint64_t millis(void) {
	uint64_t m;
	cli();          // Disable interrupts
	m = __millis;   // Read millis value
	sei();          // Enable interrupts
	return m;
}

ISR(TWI_vect) {
	switch(TWSR & 0xFC) {
		case TW_ST_SLA_ACK:
		TWDR = data;
		TWCR &= ~((1 << TWSTO) | (1 << TWEA));
		break;
		case TW_ST_LAST_DATA:
		TWCR |= (1 << TWEA);
		break;
	}
	TWCR |= (1 << TWINT);
}

int main(void) {
	DDRD = 0xFF;
	
	// กำหนดค่า Timer2 สำหรับ millis()
	init_timer2_millis();
	
	// กำหนดค่า USART สำหรับ printf
	init_usart_printf(9600);

	// กำหนดค่า ADC
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// เปิด Pull-up สำหรับ SDA และ SCL
	PORTC |= (1 << 4) | (1 << 5);

	// กำหนด Slave Address
	TWAR = (SL_ADDR << 1) | 0x00;

	// เปิดใช้งาน TWI
	TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWIE);

	sei();  // เปิด Interrupt Global

	uint64_t last_print_time = 0;
	const uint64_t print_interval = 200;  // 200ms

	while (1) {
		uint64_t current_time = millis();
		
		if (current_time - last_print_time >= print_interval) {
			last_print_time = current_time;
			
			ADCSRA |= (1 << ADSC);
			while (ADCSRA & (1 << ADSC));
			data = ADCH;
			if (!data) data = 1;
			PORTD = data;

			printf("ADC Value: %u\n", data);
		}
	}
}


























