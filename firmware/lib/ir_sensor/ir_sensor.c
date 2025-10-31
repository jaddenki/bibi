#include "ir_sensor.h"
#include <stdio.h>

static ir_sensor_callback_t user_callback = NULL;

static void ir_sensor_irq_handler(uint gpio, uint32_t events) {
    if (user_callback == NULL) return;
    
    bool is_rising = (events & GPIO_IRQ_EDGE_RISE) != 0;
    user_callback(gpio, is_rising);
}

void ir_sensor_init(uint gpio_pin, ir_sensor_callback_t callback) {
    user_callback = callback;
    
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    
    gpio_set_irq_enabled_with_callback(gpio_pin, 
                                       GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, 
                                       true, 
                                       &ir_sensor_irq_handler);
}

bool ir_sensor_read(uint gpio_pin) {
    return gpio_get(gpio_pin);
}

