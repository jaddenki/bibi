#ifndef FACE_CONTROLLER_H
#define FACE_CONTROLLER_H

#include "animation.h"
#include "ili9341.h"
#include "../../include/bibi_config.h"

typedef enum {
    FACE_IDLE,
    FACE_EAT,
    FACE_GAH
} face_expression_t;

typedef struct face_controller_t {
    animation_t idle_anim;
    animation_t eat_anim;
    animation_t gah_anim;
    
    animation_t *current_anim;
    face_expression_t current_expression;
    
    ili9341_t *display;
    uint16_t x_pos;
    uint16_t y_pos;
    uint8_t scale;
    bool rotate90; // if true, draw frames rotated 90deg clockwise
} face_controller_t;

void face_controller_init(face_controller_t *face, ili9341_t *display, 
                         uint16_t x_pos, uint16_t y_pos, uint8_t scale, bool rotate90);

void face_set_expression(face_controller_t *face, face_expression_t expression);

void face_update(face_controller_t *face);

void face_draw(face_controller_t *face);

void init_face(void);

extern face_controller_t *g_face;

#endif // FACE_CONTROLLER_H

