#include "front_sensor.h"
#include "../../include/bibi_config.h"
#include "state.h"
#include "pwm.h"
#include "hardware/adc.h"
#include <stdio.h>

void init_adc() {
    adc_init();
    adc_gpio_init(IR_PIN_FRONT);
    adc_select_input(7);
}

uint16_t read_adc() {
    adc_read();
}

void interpret_front_sensor(uint16_t value) {
    if(value > 0x4000){
        // begin turning sequence
        notTurning = false;
        stop();
        reverse_l();
        reverse_r();
        state = 0;
        timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000; // 1 second
    }
}