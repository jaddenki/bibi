#include "pico/stdlib.h"
#include "ili9341.h"
#include "animation.h"
#include "ir_sensor.h"
// #include "distance_sensor.h"
#include <stdio.h>
#include "bibi_config.h"
// #include "test_functions.c"

int main() {
    stdio_init_all();
    
    ili9341_t display;
    ili9341_init(&display, spi1, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_PIN_MOSI, DISPLAY_PIN_SCK);
    
   for(;;) { // can optimize later i guess
        run_animation_demo(&display);
        sleep_ms(1000);
    }
    return 0;
}

