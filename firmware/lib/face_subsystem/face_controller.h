#ifndef FACE_CONTROLLER_H
#define FACE_CONTROLLER_H

#include "animation.h"
#include "ili9341.h"

typedef enum {
    FACE_IDLE,
    FACE_EAT,
    FACE_GAH
} face_expression_t;

typedef struct {
    animation_t idle_anim;
    animation_t eat_anim;
    animation_t gah_anim;
    
    animation_t *current_anim;
    face_expression_t current_expression;
    
    ili9341_t *display;
    uint16_t x_pos;
    uint16_t y_pos;
    uint8_t scale;
} face_controller_t;

void face_controller_init(face_controller_t *face, ili9341_t *display, 
                         uint16_t x_pos, uint16_t y_pos, uint8_t scale);

void face_set_expression(face_controller_t *face, face_expression_t expression);

void face_update(face_controller_t *face);

void face_draw(face_controller_t *face);

#endif // FACE_CONTROLLER_H

