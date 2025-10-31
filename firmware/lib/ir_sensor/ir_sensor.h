#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "pico/stdlib.h"

// true if object detected (floor), false if cleared
typedef void (*ir_sensor_callback_t)(uint gpio, bool is_rising);

void ir_sensor_init(uint gpio_pin, ir_sensor_callback_t callback);

bool ir_sensor_read(uint gpio_pin);

#endif // IR_SENSOR_H

