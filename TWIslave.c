//Functions for TWI Slave mode
#include <avr/io.h>
#include <util/delay.h>


void TWI_init_slave(void){
	TWAR=0x20; //slave address
}

void TWI_write_slave(unsigned char data){
	TWDR= data; //data to be sent
	//Enable TWI, Clear TWI interrupt flag
	TWCR= (1<<TWEN)|(1<<TWINT);   
	//Wait for ACK
	while((TWSR & 0xF8) != 0xC0);
}

//Function to match the slave address and slave direction bit(write)
void TWI_match_write_slave(void) {
	while((TWSR & 0xF8)!= 0xA8){ // Loop till get ACK
		// Get ACK, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		// Wait for TWINT flag
		while (!(TWCR & (1<<TWINT)));
	}
}

unsigned char TWI_read_slave(void){
	// Clear TWI interrupt flag,Get ACK, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
	while((TWSR & 0xF8)!=0x80); // Wait for ACK
	return TWDR; // return data
}

//Function to match the slave address and slave direction bit(read)
void TWI_match_read_slave(void) {
	while((TWSR & 0xF8)!= 0x60){// Loop till correct ACK
		// Get ACK, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}