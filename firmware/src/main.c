#include "pico/stdlib.h"
#include "ili9341.h"
#include "animation.h"
#include <stdio.h>

// animations go here
extern const uint16_t idle[]; 

#define PIN_CS   13
#define PIN_RST  12
#define PIN_DC   17
#define PIN_MOSI 15
#define PIN_SCK  14

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
    
    ili9341_t display;
    ili9341_init(&display, spi1, PIN_CS, PIN_DC, PIN_RST, PIN_MOSI, PIN_SCK);
    
    while (true) {
        run_animation_demo(&display);
        sleep_ms(1000);
    }
    
    return 0;
}

