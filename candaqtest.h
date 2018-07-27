
#ifndef __CANDAQ_H__
#define __CANDAQ_H__

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "CANDAQ_can.h"
#include "CANDAQ_io.h"

/* Check inputs every 10 ms */
#define CANDAQ_INPUT_INTERVAL 10


#endif
