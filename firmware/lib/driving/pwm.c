#include "pwm.h"
#include "../../include/bibi_config.h"
#include "hardware/pwm.h"
#include "state.h"
#include "../face_subsystem/face_controller.h"
#include <stdio.h>
#include <stdlib.h>

void init_fan(){
    // init pwm
    gpio_init(FAN_PIN);
    gpio_set_dir(FAN_PIN, 1);
    uint slice_num = pwm_gpio_to_slice_num(FAN_PIN);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, 150);
    pwm_init(slice_num, &cfg, false);
    gpio_set_function(FAN_PIN, GPIO_FUNC_PWM);
    pwm_set_wrap(slice_num, 10000);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
    pwm_set_enabled(slice_num, true);

    // init gpio (button irq)
    gpio_add_raw_irq_handler(RAND_BUTTON, fan_button_irq_handler);
    gpio_set_irq_enabled(RAND_BUTTON, GPIO_IRQ_EDGE_RISE, 1);
    gpio_init(RAND_BUTTON);

    // init timer for delay in randomizing
    timer1_hw->inte = 1u << 0;
    irq_set_exclusive_handler(TIMER1_IRQ_0, timer1_irq_handler);
    irq_set_enabled(TIMER1_IRQ_0, 1);
}

void fan_button_irq_handler(){
    if(gpio_get_irq_event_mask(RAND_BUTTON) == GPIO_IRQ_EDGE_RISE){
        gpio_acknowledge_irq(RAND_BUTTON, GPIO_IRQ_EDGE_FALL);
        if(notRandomizing){
            notRandomizing = false;
            pwm_set_chan_level(pwm_gpio_to_slice_num(FAN_PIN), PWM_CHAN_B, rand() % 10000);
            timer1_hw->alarm[0] = timer1_hw->timerawl + 400000;
            
            // GAMBLIBIIIIII
            if (g_face != NULL) {
                face_set_expression(g_face, FACE_GAMBLING);
            }
        }
    }
}

void timer1_irq_handler(){
    timer1_hw->intr = 1u << 0;
    notRandomizing = true;
}

void init_motor_control(){
    gpio_init(MOTOR_L_PIN_A);
    gpio_init(MOTOR_R_PIN_A);
    gpio_set_dir(MOTOR_L_PIN_A, 1);
    gpio_set_dir(MOTOR_R_PIN_A, 1);
}

void forward_l(){
    gpio_put(MOTOR_L_PIN_A, 1);
}

void forward_r(){
    gpio_put(MOTOR_R_PIN_A, 1);
}

void stop(){
    gpio_put(MOTOR_L_PIN_A, 0);
    gpio_put(MOTOR_R_PIN_A, 0);
}