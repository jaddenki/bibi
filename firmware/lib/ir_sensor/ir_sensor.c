#include "ir_sensor.h"
#include "hardware/irq.h"
#include <stdio.h>

static ir_sensor_callback_t callbacks[32] = {NULL};
static bool irq_bank_enabled = false;

static void ir_sensor_irq_handler(void) {
    for (uint gpio = 0; gpio < 32; gpio++) {
        if (callbacks[gpio] == NULL) continue;
        
        uint32_t events = gpio_get_irq_event_mask(gpio);
        if (events) {
            gpio_acknowledge_irq(gpio, events);
            bool is_rising = (events & GPIO_IRQ_EDGE_RISE) != 0;
            callbacks[gpio](gpio, is_rising);
        }
    }
}

void ir_sensor_init(uint gpio_pin, ir_sensor_callback_t callback) {
    callbacks[gpio_pin] = callback;
    
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    
    if (!irq_bank_enabled) {
        irq_set_enabled(IO_IRQ_BANK0, true);
        irq_bank_enabled = true;
    }
    
    gpio_add_raw_irq_handler(gpio_pin, ir_sensor_irq_handler);
    
    gpio_set_irq_enabled(gpio_pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

bool ir_sensor_read(uint gpio_pin) {
    return gpio_get(gpio_pin);
}

