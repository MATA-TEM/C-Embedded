/*
 * Twimaster.h
 *
 * Created: 29/03/2025 05:42:28
 *  Author: NBODTKKU
 */ 


#ifndef TWI_MASTER_H
#define TWI_MASTER_H

#include <avr/io.h>

#define TWI_READ 1
#define TWI_WRITE 0

void TWI_init_master(void);
void TWI_start(void);
void TWI_repeated_start(void);
void TWI_write_address(uint8_t data);
void TWI_read_address(uint8_t data);
void TWI_write_data(uint8_t data);
uint8_t TWI_read_data(void);
void TWI_stop(void);

#endif