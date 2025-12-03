#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct face_controller_t face_controller_t;

extern volatile bool notTurning;
extern volatile int state;
extern volatile bool notRandomizing;

extern face_controller_t *g_face;

#endif