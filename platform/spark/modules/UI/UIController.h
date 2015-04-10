/* 
 * File:   UIController.h
 * Author: mat1
 *
 * Created on April 10, 2015, 10:42 AM
 */

#ifndef UICONTROLLER_H
#define	UICONTROLLER_H

class UIController
{
    bool startupComplete;
    
public:    
    void notifyStartupComplete() { startupComplete = true; }
    void beginStartup();
    bool inStartup() { return !startupComplete; }
    void updateScreen();
};

extern UIController uiController;

#endif	/* UICONTROLLER_H */

