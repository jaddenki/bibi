#include "pwm.h"
#include "bibi_config.h"
#include "hardware/pwm.h"
#include <stdio.h>

void init_pwm(){
    
}

void init_motor_control(){
    gpio_init(MOTOR_L_PIN_A);
    gpio_init(MOTOR_L_PIN_B);
    gpio_init(MOTOR_R_PIN_A);
    gpio_init(MOTOR_R_PIN_B);
    gpio_set_dir(MOTOR_L_PIN_A, 1);
    gpio_set_dir(MOTOR_L_PIN_B, 1);
    gpio_set_dir(MOTOR_R_PIN_A, 1);
    gpio_set_dir(MOTOR_R_PIN_B, 1);
}

void forward_l(){
    gpio_put(MOTOR_L_PIN_A, 1);
    gpio_put(MOTOR_L_PIN_B, 0);
}

void forward_r(){
    gpio_put(MOTOR_R_PIN_A, 0);
    gpio_put(MOTOR_R_PIN_B, 1);
}

void reverse_l(){
    gpio_put(MOTOR_L_PIN_A, 0);
    gpio_put(MOTOR_L_PIN_B, 1);
}

void reverse_r(){
    gpio_put(MOTOR_R_PIN_A, 1);
    gpio_put(MOTOR_R_PIN_B, 0);
}

void stop(){
    gpio_put(MOTOR_L_PIN_A, 0);
    gpio_put(MOTOR_L_PIN_B, 0);
    gpio_put(MOTOR_R_PIN_A, 0);
    gpio_put(MOTOR_R_PIN_B, 0);
}