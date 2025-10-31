#include "pico/stdlib.h"
#include "ili9341.h"
#include "animation.h"
#include "ir_sensor.h"
// #include "distance_sensor.h"
#include <stdio.h>
#include "bibi_config.h"

// animations go here
extern const uint16_t idle[]; 

void on_ir_sensor_event(uint gpio, bool is_rising) {
    if (is_rising) {
        printf("IR Sensor (GPIO %d): change\n", gpio);
    }
}

void test_ir_sensor(uint gpio_pin) {
    ir_sensor_init(gpio_pin, on_ir_sensor_event);
    
    while (true) {
        tight_loop_contents();
    }
}

// void test_distance_sensor(uint gpio_pin) {
//     distance_sensor_init(gpio_pin);    
//     while (true) {
//         uint16_t raw = distance_sensor_read_raw();
//         uint16_t voltage = distance_sensor_read_voltage();
        
//         printf("Raw: %4d | Voltage: %4d mV\n", raw, voltage);
//         sleep_ms(200);
//     }
// }

void run_animation_demo(ili9341_t *display) {
    animation_t demo_anim;
    animation_init(&demo_anim, idle, 16, 16, 4, 200, true);
    
    animation_start(&demo_anim);
    ili9341_fill_screen(display, 0x0000);
    
    uint32_t frame_count = 0;
    uint8_t scale = 8;
    
    while (true) {
        frame_count++;
        animation_update(&demo_anim);
    
        uint16_t scaled_size = 16 * scale;
        uint16_t center_x = (240 - scaled_size) / 2;
        uint16_t center_y = (320 - scaled_size) / 2;
        
        animation_draw_scaled(&demo_anim, display, center_x, center_y, scale);
        
        sleep_ms(100);
        
        if (frame_count > 2000) break;
    }
    
    printf("one cycle\n");
}

int main() {
    stdio_init_all();
    
    // ili9341_t display;
    // ili9341_init(&display, spi1, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_PIN_MOSI, DISPLAY_PIN_SCK);
    
    // while (true) { // can optimize later i guess
    //     run_animation_demo(&display);
    //     sleep_ms(1000);
    // }
    
    // sleep_ms(2000);
    
    test_ir_sensor(35); // plug yo sensor into gp35 for this test
    // test_distance_sensor(26);
    return 0;
}

