#include "pico/stdlib.h"
#include "pico/time.h"
#include "ili9341.h"
#include "face_controller.h"
#include "ir_sensor.h"
#include "front_sensor.h"
#include "pwm.h"
#include "auto_driving.h"
#include "state.h"
#include <stdio.h>
#include "bibi_config.h"

volatile bool notTurning = true;
volatile int state = 0;
volatile bool notRandomizing = true;

face_controller_t *g_face = NULL;

// animation timing
static uint32_t last_frame_time = 0;
static uint32_t frame_count = 0;
#define FRAME_DELAY_MS 50 

int main() {
    stdio_init_all();
    
    ili9341_t display;
    ili9341_init(&display, spi1, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_PIN_MOSI, DISPLAY_PIN_SCK);
    
    face_controller_t face;
    uint8_t scale = 8;
    uint16_t scaled_size = 16 * scale;
    uint16_t center_x = (240 - scaled_size) / 2;
    uint16_t center_y = (320 - scaled_size) / 2;
    
    face_controller_init(&face, &display, center_x, center_y, scale, true);
    ili9341_fill_screen(&display, 0x0000); 
    
    g_face = &face;

    // initialize movement
    init_motor_control();
    init_adc();
    init_auto_driving();

    // initialize fan
    init_fan();

    for(;;) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // draw animation at fixed intervals
        if (current_time - last_frame_time >= FRAME_DELAY_MS) {
            face_update(&face);
            face_draw(&face);
            last_frame_time = current_time;
            frame_count++;
        }

        if(notTurning) {
            interpret_front_sensor(read_adc());
        }
    }
    
    return 0;
}

