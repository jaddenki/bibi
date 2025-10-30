#include "animation.h"
#include "pico/time.h"

// Get current time in milliseconds
uint32_t get_time_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

// Initialize a sprite sheet animation
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

// Start playing the animation
void animation_start(animation_t *anim) {
    anim->playing = true;
    anim->last_update_time = get_time_ms();
}

// Stop the animation
void animation_stop(animation_t *anim) {
    anim->playing = false;
}

// Reset animation to first frame
void animation_reset(animation_t *anim) {
    anim->current_frame = 0;
    anim->last_update_time = get_time_ms();
}

// Update animation frame based on time
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

// Draw the current frame of the animation
void animation_draw(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y) {
    animation_draw_frame(anim, display, x, y, anim->current_frame);
}

// Draw a specific frame from the sprite sheet
void animation_draw_frame(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint16_t frame_index) {
    if (frame_index >= anim->num_frames) return;

    // Calculate the offset in the sprite sheet for this frame
    uint32_t frame_size = anim->frame_width * anim->frame_height;
    const uint16_t *frame_data = anim->sprite_sheet + (frame_index * frame_size);

    // Draw the frame
    ili9341_draw_bitmap(display, x, y, anim->frame_width, anim->frame_height, frame_data);
}

// Draw current frame scaled up by a factor (2x, 3x, 4x, etc)
void animation_draw_scaled(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint8_t scale) {
    animation_draw_frame_scaled(anim, display, x, y, anim->current_frame, scale);
}

// Draw a specific frame scaled up
void animation_draw_frame_scaled(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint16_t frame_index, uint8_t scale) {
    if (frame_index >= anim->num_frames) return;
    if (scale == 0) scale = 1;  // Prevent division by zero

    // Calculate the offset in the sprite sheet for this frame
    uint32_t frame_size = anim->frame_width * anim->frame_height;
    const uint16_t *frame_data = anim->sprite_sheet + (frame_index * frame_size);

    // Draw each pixel as a scale×scale block
    for (uint16_t py = 0; py < anim->frame_height; py++) {
        for (uint16_t px = 0; px < anim->frame_width; px++) {
            uint16_t color = frame_data[py * anim->frame_width + px];
            
            // Draw a scale×scale rectangle for this pixel
            uint16_t draw_x = x + (px * scale);
            uint16_t draw_y = y + (py * scale);
            
            ili9341_fill_rect(display, draw_x, draw_y, scale, scale, color);
        }
    }
}

// Initialize an individual frames animation
void animation_frames_init(animation_frames_t *anim, const uint16_t **frames,
                          uint16_t frame_width, uint16_t frame_height,
                          uint16_t num_frames, uint32_t frame_delay_ms, bool loop) {
    anim->frames = frames;
    anim->frame_width = frame_width;
    anim->frame_height = frame_height;
    anim->num_frames = num_frames;
    anim->frame_delay_ms = frame_delay_ms;
    anim->current_frame = 0;
    anim->last_update_time = 0;
    anim->loop = loop;
    anim->playing = false;
}

// Start playing frames animation
void animation_frames_start(animation_frames_t *anim) {
    anim->playing = true;
    anim->last_update_time = get_time_ms();
}

// Stop frames animation
void animation_frames_stop(animation_frames_t *anim) {
    anim->playing = false;
}

// Reset frames animation
void animation_frames_reset(animation_frames_t *anim) {
    anim->current_frame = 0;
    anim->last_update_time = get_time_ms();
}

// Update frames animation
void animation_frames_update(animation_frames_t *anim) {
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

// Draw current frame of frames animation
void animation_frames_draw(animation_frames_t *anim, ili9341_t *display, uint16_t x, uint16_t y) {
    if (anim->current_frame >= anim->num_frames) return;

    ili9341_draw_bitmap(display, x, y, anim->frame_width, anim->frame_height,
                       anim->frames[anim->current_frame]);
}

