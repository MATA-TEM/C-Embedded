//#define NON_READ_BLOCK
enum{BLOCK,NON_BLOCK};

#if (defined __AVR_ATmega32__) || (defined __AVR_ATmega32A__)
void init_uart(uint32_t baud){
	unsigned int ubrr;
	//Set baud rate, baud=Fosc/(16*(UBRR+1))
	//Set baud rate, baud=Fosc/(8*(UBRR+1)) (2X mode)
	ubrr =(unsigned int)(F_CPU/8/baud)-1;
	UBRRH =(unsigned char)(ubrr>>8);
	UBRRL =(unsigned char)ubrr;//Double the USART Transmission Speed  to reduce speed error
	UCSRA =(1<<U2X);
	// Enable transmitter and receiver.
	UCSRB =(1<<TXEN)|(1<<RXEN);
	//Set frame to 8data, 2stop bit, use UBRRH(URSEL)
	UCSRC =(1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
}

void putUART(uint8_t c){
	// Wait for empty transmit buffer.
	while(!(UCSRA  & (1<<UDRE)) );
	UDR =c;
}

int16_t getUART(uint8_t readBlock){
	if(NON_BLOCK ==readBlock){
	   if(!(UCSRA &(1<<RXC)))
		return -1;
	} else if(BLOCK ==readBlock){
		while (!(UCSRA &(1<<RXC)));
	}
	//no checking error bits.
	return UDR;
}

#else	// ATmega328p
void init_uart(uint32_t baud){
	unsigned int ubrr;
	//Set baud rate, baud=Fosc/(16*(UBRR+1))
	//Set baud rate, baud=Fosc/(8*(UBRR+1)) (2X mode)
	ubrr =(unsigned int)(F_CPU/8/baud)-1;
	UBRR0H =(unsigned char)(ubrr>>8);
	UBRR0L =(unsigned char)ubrr;//Double the USART Transmission Speed  to reduce speed error
	UCSR0A =(1<<U2X0);
	// Enable transmitter and receiver.
	UCSR0B =(1<<TXEN0)|(1<<RXEN0);
	//Set frame to 8data, 2stop bit
	UCSR0C =(1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
}
void putUART(uint8_t c){
	// Wait for empty transmit buffer.
	while(!(UCSR0A  & (1<<UDRE0)) );
	UDR0 =c;
}
int16_t getUART(uint8_t readBlock){
	if(NON_BLOCK ==readBlock){
		if(!(UCSR0A &(1<<RXC0)))
		return -1;
	} else if(BLOCK ==readBlock){
		while (!(UCSR0A &(1<<RXC0)));
	}
	//no checking error bits.
	return UDR0;
}

#endif 