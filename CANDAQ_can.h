#ifndef __CANDAQ_CAN_H__
#define __CANDAQ_CAN_H__

/* CAN input status message ID */
#define CANDAQ_CAN_ID 0x500

/* CAN transmit and receive pages */
#define CANDAQ_CAN_PAGE_RX 0
#define CANDAQ_CAN_PAGE_TX 1

/*
  CAN message interval (in milliseconds)

  The input status messages will be sent after every interval using the counter
  from Timer/Counter1.

  Default is 100 ms for a 10 Hz update rate
*/
#define CANDAQ_CAN_INTERVAL 100

void candaq_can_init(void);

void candaq_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len);

void candaq_can_send_inputs(void);

#endif