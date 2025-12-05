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
volatile bool notRandomizing = true;

// 

void test_face_with_ir_sensor();
// animation timing

int main() {
    stdio_init_all();
    
    // ili9341_t display;
    // ili9341_init(&display, spi1, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_PIN_MOSI, DISPLAY_PIN_SCK);
    
    // test_face_with_ir_sensor();

    // initialize movement
    init_motor_control();
    init_adc();
    init_auto_driving();
    init_face();

    // initialize fan
    init_fan();

    for(;;) {
        if(notTurning) {
            printf("i'm not turning lol\n");
            interpret_front_sensor(read_adc());
            face_update(g_face);
            face_draw(g_face);
        }
        else {
            printf("i'm turning lol");
            face_update(g_face);
            face_draw(g_face);
        }

        // printf("%d\n", read_adc());
        // sleep_ms(100);
    }
    
    return 0;
}

