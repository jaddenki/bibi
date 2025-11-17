#include "pico/stdlib.h"
#include "ili9341.h"
#include "face_controller.h"
#include "ir_sensor.h"
// #include "distance_sensor.h"
#include <stdio.h>
#include "bibi_config.h"

void run_animation_demo(ili9341_t *display);
void test_face_with_ir_sensor();

int main() {
    stdio_init_all();
    test_face_with_ir_sensor();
    
    return 0;
}

