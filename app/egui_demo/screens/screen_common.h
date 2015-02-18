#ifndef SCREEN_COMMON_H
#define	SCREEN_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "d4d.h"
#include "fonts.h"
#include "pictures.h"

// C wrapper for C++ function millis
unsigned long millisC(void);   
unsigned long microsC(void);   
    
#ifdef __cplusplus
}
#endif

#include "d4d_screen_log.h"

#endif	/* SCREEN_COMMON_H */

