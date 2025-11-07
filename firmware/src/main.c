#include "pico/stdlib.h"
#include "ili9341.h"
#include "face_controller.h"
#include "ir_sensor.h"
// #include "distance_sensor.h"
#include <stdio.h>
#include "bibi_config.h"

void run_animation_demo(ili9341_t *display);
void test_face_with_ir_sensor(ili9341_t *display, uint gpio_pin);

int main() {
    stdio_init_all();
    
    ili9341_t display;
    ili9341_init(&display, spi1, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_PIN_MOSI, DISPLAY_PIN_SCK);
    
    test_face_with_ir_sensor(&display, 35);
    
    return 0;
}

