#include <avr/interrupt.h>
#include <avr/io.h>

#include "candaqtest.h"
#include "CANDAQ_can.h"

uint16_t candaq_can_get_id(void);
void candaq_can_set_id(uint16_t id);
void candaq_can_set_mask(uint16_t mask);

void 
candaq_can_init(void)
{
	/*Configure CAN_TX as output PD5*/
	/*Port D data direction register*/
	DDRD |= (1 << DDD5);  // DDD5 bit 5 sets CAN output
	
	/*CAN General Control Register*/
	//reset CAN controller
	CANGCON = (1 << SWRES);
	
	/*
	  Configure CAN bit timing
	  Baud rate of 1 Mbps with a 16 MHz clock. See Table 19-2 on page 188 of the
	  data sheet.
	*/
	CANBT1 = 0x00;
	CANBT2 = 0x0C;
	CANBT3 = 0x36;
	
	/* Enable interrupts */
	CANGIE |= (1 << ENIT) | (1 << ENRX);
	CANIE2 |= (1 << IEMOB0);
	
	/* Configure message receive on page 0 */
	CANPAGE  = 0;
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	candaq_can_set_id(0);
	candaq_can_set_mask(0);

	/* All input messages are 1 byte */
	CANCDMOB |= 8;

	/* Enable receive */
	CANCDMOB |= (1 << CONMOB1);

	/* Enable CAN controller */
	CANGCON |= (1 << ENASTB);	
}

candaq_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len)
{
	cli(); //needed?
	uint8_t i;
	
	CANPAGE = page << MOBNB0;

	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	candaq_can_set_id(id);

	for (i = 0; i < len; i++) {
		CANMSG = msg[i];
	}
	
	CANCDMOB |= len;
	CANCDMOB |= (1 << CONMOB0);

	while((CANSTMOB & (1 << TXOK)) != (1 << TXOK));
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;
	sei(); //needed?
}

void
candaq_can_set_id(uint16_t id) //what does this function do?
{
	CANIDT4 = 0;
	CANIDT3 = 0;
	CANIDT2 = (uint8_t)  id << 5;
	CANIDT1 = (uint16_t) id >> 3;
}

uint16_t
candaq_can_get_id(void)
{
	uint16_t id = 0;
	
	id |= (CANIDT2 >> 5);
	id |= (CANIDT1 << 3);

	return id;
}

void
candaq_can_send_inputs(void) //this needs to be edited to send values from SPI not from pin
{
	uint8_t msg[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //later, change to 16bit x 4
	msg[1] = 1;
	msg[3] = 1;
	msg[5] = 1;
	candaq_can_send(1, 0x400, msg, 8);
		
	//msg[1] = (PINC & (1 << PINC6)) > 0;
	//msg[3] = (PINC & (1 << PINC0)) > 0;
	//candaq_can_send(1, 0x400, msg, 8);

}

void
candaq_can_set_mask(uint16_t mask)
{
	CANIDM4 = 0;
	CANIDM3 = 0;
	CANIDM2 = (uint8_t)  mask << 5;
	CANIDM1 = (uint16_t) mask >> 3;
}



ISR(CANIT_vect)
{
	uint16_t id;
	uint8_t  len;
	uint8_t  idx;
	uint8_t  byte;

	CANPAGE = 0;

	id  = candaq_can_get_id();
	len = (CANCDMOB & 0x0F);
	
	/* Re-enable transmission */
	CANSTMOB = 0;
	CANCDMOB |= (1 << CONMOB1);
}
