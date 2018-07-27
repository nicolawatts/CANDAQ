#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "candaqtest.h"

#include "CANDAQ_io.h"

uint8_t candaq_io_values = 0;

void
candaq_io_init(void)
{
	/* Enable internal pull-ups for input pins */
	PORTB |= 0x38; /* 0011_1000 (6, 4, 3) */
	PORTC |= 0xE1; /* 1110_0001 (5, 2, 1, 8) */
	PORTD |= 0x01; /* 0000_0001 (7) */
	//PORTE |= 
}

void
candaq_io_update(void)
{
	if (PORTE & (0x08)){
		 candaq_io_values |= 0x01;
	//	uint8_t msg[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //later, change to 16bit x 4 
	//	msg[1] = candaq_io_values;
	//	candaq_can_send(1, 0x400, msg, 8);
		}
	else candaq_io_values &= ~0x01;
	
}

uint8_t
candaq_io_value(uint8_t input)
{
	return candaq_io_values & (1 << (input - 1));
}