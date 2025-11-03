#include "auto_driving.h"
#include <stdio.h>

int state = 0;
bool left = false;
bool right = false;

// initialize io_bank gpio handler, timer irq handler
void init_auto_driving(){
    irq_set_enabled(IO_IRQ_BANK0, 1);
    gpio_add_raw_irq_handler(8, io_bank_handler);
    gpio_add_raw_irq_handler(39, io_bank_handler);
    gpio_set_irq_enabled(8, GPIO_IRQ_EDGE_FALL, 1);
    gpio_set_irq_enabled(39, GPIO_IRQ_EDGE_FALL, 1);
    
    gpio_init(8);
    gpio_init(39);

    timer0_hw->inte = 1u << 0;
    irq_set_exclusive_handler(TIMER0_IRQ_0, timer0_irq_handler);
    irq_set_enabled(TIMER0_IRQ_0, 1);
}

void io_bank_handler(){
    if(gpio_get_irq_event_mask(8) == GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(8, GPIO_IRQ_EDGE_FALL);
        left = true;
    }
    else if(gpio_get_irq_event_mask(39) == GPIO_IRQ_EDGE_FALL){
        gpio_acknowledge_irq(39, GPIO_IRQ_EDGE_FALL);
        right = true;
    }

    // reverse both motors
    state = 0;
    timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000; // 1 second
}

void timer0_irq_handler(){
    if(!state){
        if(left){
            // re-reverse left motor
            timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000;
        }
        else if(right){
            // re-reverse right motor
            timer0_hw->alarm[0] = timer0_hw->timerawl + 1000000;
        }
        state = 1;
    }
    else{
        if(left){
            // re-reverse right motor
            left = false;
        }
        else if(right){
            // re-reverse left motor
            right = false;
        }
    }
}
