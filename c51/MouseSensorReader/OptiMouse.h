#ifndef OPTI_MOUSE_H
#define OPTI_MOUSE_H

/**
 * Interface to optical mouse sensor
 */

#include "types.h"

#define OPTI_MOUSE_PROC_RAW_IMAGE 1

void opti_mouse__start( void );

INT8 opti_mouse__get_dx( void );
INT8 opti_mouse__get_dy( void );

#if OPTI_MOUSE_PROC_RAW_IMAGE == 1
UINT8* opti_mouse__get_raw_image( void );
#endif

#endif /* OPTI_MOUSE_H */
