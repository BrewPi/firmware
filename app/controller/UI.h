/*
 * UI.h
 *
 * Created: 09/01/2015 03:38:24
 *  Author: mat
 */

#pragma once
#include <stdint.h>

struct UI {
    /**
     * Initialize the UI components.
     */
    static uint8_t init();

    /**
     * Display the startup page and retrieve how long the startup page should be displayed 
     * for in milliseconds before moving on to the main page.
     */
    static uint32_t showStartupPage();

    /**
     * Show the main controller page. 
     */
    static void showControllerPage();

    /**
     * Called as often as possible by the application. Allows the UI to peform
     * quick background tasks, such as reading the touch screen.
     */    
    static void ticks();
    
    /**
     * Called from the main loop to update the current page and perform event
     * processing.
     */
    static void update();
    
    /**
     * Determines if this UI is in startup mode. The controller services piLink
     * requests and continues to update the UI, but the main control loop is not executed.
     * @return 
     */
    static bool inStartup();


};

extern UI ui;