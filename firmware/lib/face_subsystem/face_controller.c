#include "face_controller.h"

extern const uint16_t idle[];
extern const uint16_t eat[];
extern const uint16_t gah[];

void face_controller_init(face_controller_t *face, ili9341_t *display, 
                         uint16_t x_pos, uint16_t y_pos, uint8_t scale) {
    face->display = display;
    face->x_pos = x_pos;
    face->y_pos = y_pos;
    face->scale = scale;
    
    // idle: 16x16, 4 frames, 200ms per frame, loop
    animation_init(&face->idle_anim, idle, 16, 16, 4, 200, true);
    
    // eat: 16x16, 5 frames, 80ms per frame, loop
    animation_init(&face->eat_anim, eat, 16, 16, 5, 80, true);
    
    // gah: 16x16, 8 frames, 50ms per frame, don't loop (play once)
    animation_init(&face->gah_anim, gah, 16, 16, 8, 50, false);
    
    face->current_expression = FACE_IDLE;
    face->current_anim = &face->idle_anim;
    animation_start(face->current_anim);
}

void face_set_expression(face_controller_t *face, face_expression_t expression) {
    if (face->current_expression == expression && face->current_anim->playing) {
        return;
    }
    
    face->current_expression = expression;
    
    switch (expression) {
        case FACE_IDLE:
            face->current_anim = &face->idle_anim;
            break;
        case FACE_EAT:
            face->current_anim = &face->eat_anim;
            break;
        case FACE_GAH:
            face->current_anim = &face->gah_anim;
            break;
    }
    
    face->current_anim->current_frame = 0;
    animation_start(face->current_anim);
    
    // when gah finished, return to idle
    if (expression == FACE_GAH && !face->current_anim->playing) {
        face_set_expression(face, FACE_IDLE);
    }
}

void face_update(face_controller_t *face) {
    animation_update(face->current_anim);
    
    // when gah finished, return to idle
    if (face->current_expression == FACE_GAH && !face->current_anim->playing) {
        face_set_expression(face, FACE_IDLE);
    }
}

void face_draw(face_controller_t *face) {
    animation_draw_scaled(face->current_anim, face->display, 
                         face->x_pos, face->y_pos, face->scale);
}

