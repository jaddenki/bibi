// #include "test_functions.h"
#include "pico/stdlib.h"
#include "ili9341.h"
#include "face_controller.h"
#include "ir_sensor.h"
// #include "distance_sensor.h"
#include <stdio.h>
#include "bibi_config.h"

static face_controller_t *g_face = NULL;
static bool ir_object_detected = false;

void on_ir_edge_detected(uint gpio, bool is_rising) {
    if (g_face == NULL) return;
    
    if (is_rising) {
        ir_object_detected = !ir_object_detected;
        if (ir_object_detected) {
            face_set_expression(g_face, FACE_GAH);
        }
    }
}

// void test_ir_sensor(uint gpio_pin) {
//     ir_sensor_init(gpio_pin, on_ir_sensor_event);
    
//     while (true) {
//         tight_loop_contents();
//     }
// }

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
    face_controller_t face;
    
    uint8_t scale = 8;
    uint16_t scaled_size = 16 * scale;
    uint16_t center_x = (240 - scaled_size) / 2;
    uint16_t center_y = (320 - scaled_size) / 2;
    
    face_controller_init(&face, display, center_x, center_y, scale, false);
    ili9341_fill_screen(display, 0x0000);
    
    // cycle through different expressions
    uint32_t loop_count = 0;
    
    while (true) {
        face_update(&face);
        face_draw(&face);
        
        sleep_ms(50);
        loop_count++;
        
        // switch expressions every few seconds
        if (loop_count == 60) {
            face_set_expression(&face, FACE_EAT);
        } else if (loop_count == 120) {
            face_set_expression(&face, FACE_GAH);
        } else if (loop_count == 180) {
            face_set_expression(&face, FACE_IDLE);
        } else if (loop_count > 240) {
            break;
        }
    }
}

void test_face_with_ir_sensor(void) {
    
    ili9341_t display;
    ili9341_init(&display, spi0, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_PIN_MOSI, DISPLAY_PIN_SCK);
    face_controller_t face;
    
    uint8_t scale = 8;
    uint16_t scaled_size = 16 * scale;
    uint16_t center_x = (240 - scaled_size) / 2;
    uint16_t center_y = (320 - scaled_size) / 2;
    
    face_controller_init(&face, &display, center_x, center_y, scale, true);
    ili9341_fill_screen(&display, 0x0000);
    
    g_face = &face;
    
    ir_sensor_init(IR_PIN_RIGHT, on_ir_edge_detected);
    ir_sensor_init(IR_PIN_LEFT, on_ir_edge_detected);
    
    while (true) {
        face_update(&face);
        face_draw(&face);
        
        sleep_ms(50);
    }
}

