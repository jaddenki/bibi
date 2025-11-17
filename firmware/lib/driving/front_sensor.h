#ifndef FRONT_SENSOR_H
#define FRONT_SENSOR_H

#include "pico/stdlib.h"

void init_adc();

uint16_t read_adc();

void interpret_front_sensor(uint16_t value);

#endif

