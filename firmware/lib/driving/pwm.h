#ifndef PWM_H
#define PWM_H

#include "pico/stdlib.h"

void init_pwm();

void init_motor_control();

void forward_l();

void forward_r();

void reverse_l();

void reverse_r();

void stop();

#endif

