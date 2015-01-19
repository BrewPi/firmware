/**************************************************************************
* 
* Copyright 2014 by Petr Gargulak. eGUI Community.
* Copyright 2009-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3 
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the eGUI project give you
* permission to link the eGUI sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your 
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based 
* on this library. 
* If you modify the eGUI sources, you may extend this exception 
* to your version of the eGUI sources, but you are not obligated 
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************//*!
*
* @file      d4d_screen_scrsvr.c
*
* @author    Petr Gargulak
* 
* @version   0.0.3.0
* 
* @date      Jan-14-2014
* 
* @brief     D4D driver - scrsvr of d4d typical screen source file
*
******************************************************************************/

/****************************************************************************
*       Includes header files
****************************************************************************/
#include "d4d.h"
#include "main.h"
#include "fonts.h"
#include "pictures.h"


/****************************************************************************
*       Local functions declarations
***************************************************************************/
static Byte SCRSVR_PicOnUsrMessage(D4D_MESSAGE* pMsg);

/****************************************************************************
*       Local variables declarations
***************************************************************************/
static sLWord dx, dy;
static D4D_POINT oldPos = {0, 0};

/****************************************************************************
*       D4D widgets (graphic objects) declarations
****************************************************************************/
D4D_DECLARE_PICTURE_INRAM(scrsvr_picLogo, 0, 0, 100, 53, &bmp_ScreenSaverLogo, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED), NULL, SCRSVR_PicOnUsrMessage);

/****************************************************************************
*       D4D screen declarations
****************************************************************************/
D4D_DECLARE_SCREEN_BEGIN(screen_scrsvr, ScreenScrSvr_, 0 , 0, (D4D_COOR)(D4D_SCREEN_SIZE_LONGER_SIDE), (D4D_COOR)(D4D_SCREEN_SIZE_SHORTER_SIDE), NULL, 0, NULL, 0, NULL)
  D4D_DECLARE_SCREEN_OBJECT(scrsvr_picLogo)    
D4D_DECLARE_SCREEN_END()    


/*-----------------------------------------------------------------------------
* FUNCTION:    ScreenScrSvr_MovePicture
* SCOPE:       Screen saver screen related function - local
* DESCRIPTION: Local function to move the screen saver picture
*
* PARAMETERS:  none
*              
* RETURNS:     none
*-----------------------------------------------------------------------------*/
static void ScreenScrSvr_MovePicture(void)
{  
  D4D_SIZE pictSize = D4D_GetBmpSize(((D4D_PICTURE*)scrsvr_picLogo.pParam)->pBmp);
  
  // Move the picture on new position
  oldPos = scrsvr_picLogo.position;
  
  if(((oldPos.x + dx + pictSize.cx + 1) >= screen_scrsvr.size.cx) || ((oldPos.x + dx) < 0))
    dx *= -1;
    
  scrsvr_picLogo.position.x = oldPos.x + dx;
  
  if(((oldPos.y + dy + pictSize.cy + 1) >= screen_scrsvr.size.cy) || ((oldPos.y + dy) < 0))
    dy *= -1;
    
  scrsvr_picLogo.position.y = oldPos.y + dy;
  
  D4D_InvalidateObject(&scrsvr_picLogo, D4D_TRUE);
}

/*-----------------------------------------------------------------------------
* FUNCTION:    SCRSVR_PicOnUsrMessage
* SCOPE:       Screen saver screen related function - local
* DESCRIPTION: User message handler function for picture object, to redraw the last picture position by background color
*
* PARAMETERS:  pMsg - pointer to D4D_MESSAGE structure
*              
* RETURNS:     return if this message discard or not
*-----------------------------------------------------------------------------*/
static Byte SCRSVR_PicOnUsrMessage(D4D_MESSAGE* pMsg)
{
  if(pMsg->nMsgId == D4D_MSG_DRAWDONE)
  {
    D4D_SIZE pictSize = D4D_GetBmpSize(((D4D_PICTURE*)scrsvr_picLogo.pParam)->pBmp);    
    D4D_SIZE rectSize;
    D4D_POINT rectPoint;
    
    // Draw the horizontal box
    rectSize.cy = D4D_Abs16(dy);
    rectSize.cx = pictSize.cx;
    
    rectPoint = oldPos;
    
    if(oldPos.y > scrsvr_picLogo.position.y)
      rectPoint.y += pictSize.cy;
    
    D4D_FillRect(&rectPoint, &rectSize, D4D_COLOR_BLACK);
    
    // Draw the vertical box
    rectSize.cx = D4D_Abs16(dx);
    rectSize.cy = pictSize.cy;
    
    rectPoint = oldPos;
    
    if(oldPos.x > scrsvr_picLogo.position.x)
      rectPoint.x += pictSize.cx;
    
    D4D_FillRect(&rectPoint, &rectSize, D4D_COLOR_BLACK);
  }
  
  return D4D_MSG_NOSKIP;
}


/****************************************************************************
*       D4D standard screen functions definitions
****************************************************************************/
// One time called screen function in screen initialization proces
static void ScreenScrSvr_OnInit()
{
  dx = 2;
  dy = 1;  
}

// Screen on Activate function called with each screen activation
static void ScreenScrSvr_OnActivate()
{
  D4D_MouseShow(D4D_FALSE);
  D4D_ClearScreen(D4D_COLOR_BLACK);
}

// Screen "Main" function called periodically in each D4D_poll runs
static void ScreenScrSvr_OnMain()
{
  if(time.bits.b200ms)
  {
    time.bits.b200ms = 0;
    
    ScreenScrSvr_MovePicture();
    
  }
}


// Screen on DeActivate function called with each screen deactivation
static void ScreenScrSvr_OnDeactivate()
{
  //D4D_ClearScreen(D4D_COLOR_SCR_DESKTOP);
  D4D_MouseShow(D4D_TRUE);
}

// Screen on message function called with each internal massage for this screen
static Byte ScreenScrSvr_OnObjectMsg(D4D_MESSAGE* pMsg)
{
  D4D_UNUSED(pMsg);
  
  return 0;
}



