#include "front_sensor.h"
#include "../../include/bibi_config.h"
#include "state.h"
#include "pwm.h"
#include "face_controller.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include <stdio.h>

void init_adc() {
    adc_init();
    adc_gpio_init(IR_PIN_FRONT);
    adc_select_input(7);
}

uint16_t read_adc() {
    return adc_read();
}

void interpret_front_sensor(uint16_t value) {
    printf("%d\n", value);
    fflush(stdout);
    if(value > 0x0400){
        // obstacle detected in front
        if (g_face != NULL) {
            face_set_expression(g_face, FACE_GAH);
        }
        
        // begin turning sequence
        notTurning = false;
        stop();
        forward_l();
        timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000; // 1 second
    }
}