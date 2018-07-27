#include <avr/interrupt.h>
#include <avr/io.h>

#include "candaqtest.h"
#include "CANDAQ_can.h"
#include "CANDAQ_io.h"


volatile uint32_t candaq_clock = 0;
volatile uint8_t candaq_can_clock = 0;
volatile uint8_t candaq_can_trigger = 0;
volatile uint8_t candaq_input_clock = 0;
volatile uint8_t candaq_input_trigger = 0;


void candaq_timer_init(void);

int
main(void)
{
	/* Initialize peripherals */
	candaq_io_init();
	candaq_can_init();
	candaq_timer_init();	
	
	/* Enable global interrupts */
	sei();
	
	for (;;) {
		
		/* Perform periodic input tasks */
		if (candaq_input_trigger) {
			candaq_io_update();
			candaq_input_trigger = 0;
		}

		
		/* Perform periodic output tasks */
		if (candaq_can_trigger) {
		//	candaq_can_send_inputs();
			candaq_can_trigger = 0;
		}
	}
	
	return 0;
}


void
candaq_timer_init(void)
{
	/* Output compare interrupts every 1 ms */
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A   = 999;

	/* Set timer clock prescaler to 8 for a 1 MHz timer clock */
	TCCR1B |= (1 << CS11);
}

ISR(TIMER1_COMPA_vect)
{
	candaq_can_clock++;
	candaq_input_clock++;

	candaq_clock++;
	/* Trigger CAN messages */
	if (candaq_clock % CANDAQ_CAN_INTERVAL == 0) candaq_can_trigger = 1;

	/* Trigger input check */
	if (candaq_clock % CANDAQ_INPUT_INTERVAL == 0) candaq_input_trigger = 1;

}
