// Program for Master Mode

#include<avr/io.h>
#include<inttypes.h>

#define TWI_READ 1
#define TWI_WRITE 0

void TWI_init_master(void){
	TWBR=0x48; // Bit rate, 100kHz, no prescaler
	//TWBR=0x12 //400kHz @16MHz
	// Set prescaler in TWPSn
	// SCL freq = F_CPU/(16+(2xTWBRxPrescaler))
}

void TWI_start(void){
	//Clear TWI interrupt flag, 
	//Put start condition, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	// Wait till start condition is sent
	while(!(TWCR & (1<<TWINT))); 
	// Check for ACK
	while((TWSR & 0xF8)!= 0x08); 
}

void TWI_repeated_start(void){
	//Clear TWI interrupt flag, 
	//Put start condition, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	//wait till restart condition is sent
	while(!(TWCR & (1<<TWINT)));
	//Check for ACK
	while((TWSR & 0xF8)!= 0x10); 
}

void TWI_write_address(uint8_t data){
	TWDR=data; //Address and direction
	//Clear TWI interrupt flag,Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEN); 
	// Wait till complete TWDR sent   
	while (!(TWCR & (1<<TWINT)));
	//wait for ACK
	while((TWSR & 0xF8)!= 0x18);  
}

void TWI_read_address(uint8_t data){
	TWDR=data; // Address and direction
	//Clear TWI interrupt flag,Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEN);
	//Wait till get data completed
	while (!(TWCR & (1<<TWINT)));
	//wait for ACK 
	while((TWSR & 0xF8)!= 0x40);
}

void TWI_write_data(uint8_t data){
	TWDR=data; //put data
	//Clear TWI interrupt flag,Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEN);
	//Wait till complete TWDR sent
	while (!(TWCR & (1<<TWINT)));
	//wait for ACK 
	while((TWSR & 0xF8) != 0x28);
}

uint8_t TWI_read_data(void){
	//Clear TWI interrupt flag,Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEN);
	//Wait till complete TWDR sent
	while (!(TWCR & (1<<TWINT)));
	//wait for ACK
	while((TWSR & 0xF8) != 0x58);
	return TWDR;
}

void TWI_stop(void){
	//Clear TWI interrupt flag, 
	//Put stop condition, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	//Wait till stop condition is transmitted
	while(TWCR & (1<<TWSTO));
}
