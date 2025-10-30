# bibi's face <3

## files

- `animation.c` / `animation.h` - helps with frame sequences
- `anim.c` - animation data (frame arrays 0_0)

## usage

```c
#include "animation.h"

extern const uint16_t idle[];

animation_t demo_anim;
animation_init(&demo_anim, idle, 16, 16, 4, 200, true);
animation_start(&demo_anim);

// main loop:
animation_update(&demo_anim);
animation_draw_scaled(&demo_anim, &display, x, y, scale);
```

## To do:

- what animations should i include?
  - startup
  - low battery
  - cleaning ?
  - misc idle ?

