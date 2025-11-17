#include "auto_driving.h"
#include "pwm.h"
#include "bibi_config.h"
#include "state.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include <stdio.h>

int state = 0;
bool left = false;
bool right = false;

// initialize io_bank gpio handler, timer irq handler
void init_auto_driving(){
    irq_set_enabled(IO_IRQ_BANK0, 1);
    gpio_add_raw_irq_handler(IR_PIN_RIGHT, io_bank_handler);
    gpio_add_raw_irq_handler(IR_PIN_LEFT, io_bank_handler);
    gpio_set_irq_enabled(IR_PIN_RIGHT, GPIO_IRQ_EDGE_FALL, 1);
    gpio_set_irq_enabled(IR_PIN_LEFT, GPIO_IRQ_EDGE_FALL, 1);
    
    gpio_init(IR_PIN_LEFT);
    gpio_init(IR_PIN_RIGHT);

    timer0_hw->inte = 1u << 0;
    irq_set_exclusive_handler(TIMER0_IRQ_0, timer0_irq_handler);
    irq_set_enabled(TIMER0_IRQ_0, 1);
}

void io_bank_handler(){
    if(gpio_get_irq_event_mask(IR_PIN_RIGHT) == GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(IR_PIN_RIGHT, GPIO_IRQ_EDGE_FALL);
        right = true;
        notTurning = false;
    }
    else if(gpio_get_irq_event_mask(IR_PIN_LEFT) == GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(IR_PIN_LEFT, GPIO_IRQ_EDGE_FALL);
        left = true;
        notTurning = false;
    }

    // reverse both motors
    reverse_l();
    reverse_r();
    state = 0;
    timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000; // 1 second
}

void timer0_irq_handler(){
    if(!state){
        if(left){
            // re-reverse left motor
            forward_l();
            timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000;
        }
        else if(right){
            // re-reverse right motor
            forward_r();
            timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000;
        }
        state = 1;
    }
    else{
        if(left){
            // re-reverse right motor
            forward_r();
            left = false;
            notTurning = true;
        }
        else if(right){
            // re-reverse left motor
            forward_l();
            right = false;
            notTurning = true;
        }
    }
}
