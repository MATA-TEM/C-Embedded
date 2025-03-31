// time2, 1ms ISR routine and increase __millis

#include <avr/io.h>
#include <avr/interrupt.h>
uint64_t volatile __millis=0;


#if (defined __AVR_ATmega32__) || (defined __AVR_ATmega32A__)

ISR (TIMER2_COMP_vect){
	__millis++;
}
void init_timer2_millis(){
	TCCR2|=(1<<CS22); //(CLK/64).
	TCCR2|=(1<<WGM21); //autoreload.
	TIMSK=(1<<OCIE2);  //enable INT.
	TIFR=(1<<OCF2);   //clear flag
	TCNT2=0x00;   //initialize counter0.
	//interupt every (1/16000000)*64*1000*(249+1) = 1ms
	OCR2=249; // output compare register A
	sei(); 
}

#else
// ATmega328p
ISR (TIMER2_COMPA_vect){
	__millis++;
}
void init_timer2_millis(){
	TCCR2B=(1<<CS22); //(CLK/64).
	TCCR2A|=(1<<WGM21); //autoreload.
	TIMSK2=(1<<OCIE2A);  //enable INT.
	TIFR2=(1<<OCF2A);   //clear flag
	TCNT2=0x00;   //initialize counter0.
	//interupt every (1/16000000)*64*1000*(249+1) = 1ms
	OCR2A=249; // output compare register A
	sei(); 
}

#endif
