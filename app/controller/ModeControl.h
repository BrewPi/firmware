/* 
 * File:   ModeControl.h
 * Author: mat
 *
 * Created on 27 February 2015, 23:21
 */

#ifndef MODECONTROL_H
#define	MODECONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define	MODE_FRIDGE_CONSTANT 'f'
#define MODE_BEER_CONSTANT 'b'
#define MODE_BEER_PROFILE 'p'
#define MODE_OFF 'o'
#define MODE_TEST 't'

    
    
typedef char control_mode_t;
control_mode_t ModeControl_GetMode();

/**
 * Sets the mode and returns the previous mode.
 * @param mode
 * @return 
 */
control_mode_t ModeControl_SetMode(control_mode_t mode);


#ifdef	__cplusplus
}
#endif

#endif	/* MODECONTROL_H */

