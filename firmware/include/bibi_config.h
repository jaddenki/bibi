#ifndef BIBI_CONFIG_H
#define BIBI_CONFIG_H

// ============================================================================
// Bibi Robot - Global Configuration
// ============================================================================

// Hardware Configuration
#define BIBI_MICROCONTROLLER_RP2350

// bibi face old (just for when i test cus im lazy thx)
// #define DISPLAY_PIN_CS   13
// #define DISPLAY_PIN_RST  12
// #define DISPLAY_PIN_DC   17
// #define DISPLAY_PIN_MOSI 15
// #define DISPLAY_PIN_SCK  14

// bibi face
#define DISPLAY_PIN_CS   1
#define DISPLAY_PIN_RST  4 
#define DISPLAY_PIN_DC   5
#define DISPLAY_PIN_MOSI 3
#define DISPLAY_PIN_SCK  2

// bibi see (sensor config)
#define IR_PIN_FRONT 47 // Vs is 5V; ADC Channel 7
#define IR_PIN_RIGHT 8  // Vcc is 3.3V
#define IR_PIN_LEFT  7  // Vcc is 3.3V

// bibi move
// motor left
#define MOTOR_L_PIN_A 20
#define MOTOR_L_PIN_B 27

// motor right
#define MOTOR_R_PIN_A 19
#define MOTOR_R_PIN_B 28

// bibi clean
#define FAN_PIN 35      // PWM 9B

#define SYSTEM_BAUD_RATE 115200

#endif // BIBI_CONFIG_H

