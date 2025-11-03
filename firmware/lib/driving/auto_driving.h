#ifndef AUTO_DRIVING_H
#define AUTO_DRIVING_H

#include "pico/stdlib.h"

void init_auto_driving(); // initialize io_bank gpio handler, timer irq handler

void io_bank_handler();

void timer0_irq_handler();

#endif // IR_SENSOR_H

