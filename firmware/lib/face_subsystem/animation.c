#include "animation.h"
#include "pico/time.h"

uint32_t get_time_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

void animation_init(animation_t *anim, const uint16_t *sprite_sheet,
                   uint16_t frame_width, uint16_t frame_height,
                   uint16_t num_frames, uint32_t frame_delay_ms, bool loop) {
    anim->sprite_sheet = sprite_sheet;
    anim->frame_width = frame_width;
    anim->frame_height = frame_height;
    anim->num_frames = num_frames;
    anim->frame_delay_ms = frame_delay_ms;
    anim->current_frame = 0;
    anim->last_update_time = 0;
    anim->loop = loop;
    anim->playing = false;
}

void animation_start(animation_t *anim) {
    anim->playing = true;
    anim->last_update_time = get_time_ms();
}

void animation_update(animation_t *anim) {
    if (!anim->playing) return;

    uint32_t current_time = get_time_ms();
    uint32_t elapsed = current_time - anim->last_update_time;

    if (elapsed >= anim->frame_delay_ms) {
        anim->current_frame++;

        if (anim->current_frame >= anim->num_frames) {
            if (anim->loop) {
                anim->current_frame = 0;
            } else {
                anim->current_frame = anim->num_frames - 1;
                anim->playing = false;
            }
        }

        anim->last_update_time = current_time;
    }
}

void animation_draw_scaled(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint8_t scale) {
    animation_draw_frame_scaled(anim, display, x, y, anim->current_frame, scale);
}

void animation_draw_frame_scaled(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint16_t frame_index, uint8_t scale) {
    if (frame_index >= anim->num_frames) return;
    if (scale == 0) scale = 1; 
    uint32_t frame_size = anim->frame_width * anim->frame_height;
    const uint16_t *frame_data = anim->sprite_sheet + (frame_index * frame_size);

    for (uint16_t py = 0; py < anim->frame_height; py++) {
        for (uint16_t px = 0; px < anim->frame_width; px++) {
            uint16_t color = frame_data[py * anim->frame_width + px];
            
            uint16_t draw_x = x + (px * scale);
            uint16_t draw_y = y + (py * scale);
            
            ili9341_fill_rect(display, draw_x, draw_y, scale, scale, color);
        }
    }
}


void animation_draw_rotated(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint8_t scale) {
    animation_draw_frame_rotated(anim, display, x, y, anim->current_frame, scale);
}

void animation_draw_frame_rotated(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint16_t frame_index, uint8_t scale) {
    if (frame_index >= anim->num_frames) return;
    if (scale == 0) scale = 1;

    uint32_t frame_size = anim->frame_width * anim->frame_height;
    const uint16_t *frame_data = anim->sprite_sheet + (frame_index * frame_size);

    uint16_t orig_w = anim->frame_width;
    uint16_t orig_h = anim->frame_height;

    uint16_t rot_w = orig_h;
    uint16_t rot_h = orig_w;

    for (uint16_t ry = 0; ry < rot_h; ry++) {
        for (uint16_t rx = 0; rx < rot_w; rx++) {
            // src_row = orig_h - 1 - rx
            // src_col = ry
            uint16_t src_row = orig_h - 1 - rx;
            uint16_t src_col = ry;
            uint16_t color = frame_data[src_row * orig_w + src_col];

            uint16_t draw_x = x + (rx * scale);
            uint16_t draw_y = y + (ry * scale);
            ili9341_fill_rect(display, draw_x, draw_y, scale, scale, color);
        }
    }
}
