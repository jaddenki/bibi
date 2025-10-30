#ifndef ANIMATION_H
#define ANIMATION_H

#include "ili9341.h"
#include <stdint.h>
#include <stdbool.h>

// Animation structure for sprite sheets
typedef struct {
    const uint16_t *sprite_sheet;  // Pointer to sprite sheet data (RGB565)
    uint16_t frame_width;           // Width of each frame
    uint16_t frame_height;          // Height of each frame
    uint16_t num_frames;            // Total number of frames
    uint16_t current_frame;         // Current frame index
    uint32_t frame_delay_ms;        // Delay between frames in milliseconds
    uint32_t last_update_time;      // Last time the frame was updated
    bool loop;                      // Whether to loop the animation
    bool playing;                   // Whether animation is currently playing
} animation_t;

// Animation for individual frame arrays
typedef struct {
    const uint16_t **frames;        // Array of pointers to individual frames
    uint16_t frame_width;
    uint16_t frame_height;
    uint16_t num_frames;
    uint16_t current_frame;
    uint32_t frame_delay_ms;
    uint32_t last_update_time;
    bool loop;
    bool playing;
} animation_frames_t;

// Function prototypes for sprite sheet animations
void animation_init(animation_t *anim, const uint16_t *sprite_sheet, 
                   uint16_t frame_width, uint16_t frame_height, 
                   uint16_t num_frames, uint32_t frame_delay_ms, bool loop);

void animation_start(animation_t *anim);
void animation_stop(animation_t *anim);
void animation_reset(animation_t *anim);
void animation_update(animation_t *anim);
void animation_draw(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y);
void animation_draw_frame(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint16_t frame_index);

// Scaled drawing - draws animation at NxN scale
void animation_draw_scaled(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint8_t scale);
void animation_draw_frame_scaled(animation_t *anim, ili9341_t *display, uint16_t x, uint16_t y, uint16_t frame_index, uint8_t scale);

// Function prototypes for individual frame animations
void animation_frames_init(animation_frames_t *anim, const uint16_t **frames,
                          uint16_t frame_width, uint16_t frame_height,
                          uint16_t num_frames, uint32_t frame_delay_ms, bool loop);

void animation_frames_start(animation_frames_t *anim);
void animation_frames_stop(animation_frames_t *anim);
void animation_frames_reset(animation_frames_t *anim);
void animation_frames_update(animation_frames_t *anim);
void animation_frames_draw(animation_frames_t *anim, ili9341_t *display, uint16_t x, uint16_t y);

// Utility function to get current time in milliseconds
uint32_t get_time_ms(void);

#endif // ANIMATION_H

