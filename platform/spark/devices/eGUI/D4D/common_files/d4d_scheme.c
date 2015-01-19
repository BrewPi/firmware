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
* @file      d4d_scheme.c
*
* @author     Petr Gargulak
*
* @version   0.0.15.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver color scheme c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

/**************************************************************
*
* Global variables
*
***************************************************************
*/

D4D_DECLARE_CLR_SCHEME(d4d_clr_scheme_default, \
        D4D_COLOR_SCR_DESKTOP, D4D_COLOR_SCR_OUTLINE, D4D_COLOR_SCR_TITLEBAR, D4D_COLOR_SCR_TILTLETEXT, D4D_COLOR_SCR_EXIT_BTN_FORE, D4D_COLOR_SCR_EXIT_BTN_BCKG,\
        D4D_COLOR_BCKG_NORM, D4D_COLOR_BCKG_DISABLED, D4D_COLOR_BCKG_FOCUS, D4D_COLOR_BCKG_CAPTURE,\
        D4D_COLOR_FORE_NORM, D4D_COLOR_FORE_DISABLED, D4D_COLOR_FORE_FOCUS, D4D_COLOR_FORE_CAPTURE,\
        D4D_COLOR_GAUG_HUB, D4D_COLOR_GAUG_POINTER,\
        D4D_COLOR_SLDR_BAR_BCKG, D4D_COLOR_SLDR_BAR_FORE, D4D_COLOR_SLDR_BAR_START, D4D_COLOR_SLDR_BAR_END,\
        D4D_COLOR_CHECKBOX_ICON_BCKG,\
        D4D_COLOR_GRAPH_GRID,\
        D4D_COLOR_PRGRS_BAR_BAR_BCKG, D4D_COLOR_PRGRS_BAR_BAR_FORE, D4D_COLOR_PRGRS_BAR_BAR_END \
        )

/*************************************************************
*
* Local variables
*
**************************************************************/

// pointer to default color scheme pointer
static D4D_CLR_SCHEME *pSchemeDefault = (D4D_CLR_SCHEME*)&d4d_clr_scheme_default;


/******************************************************************************
* Begin of D4D_SCHEME public functions
*//*! @addtogroup doxd4d_scheme_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function return the pointer to current use color scheme of screen
* @param   pScreen - pointer to screen
* @return  pointer to screen current use of color scheme
* @note    This function find out the current use object scheme of object.
*          It handles with standard default scheme and screen color scheme.
*******************************************************************************/
D4D_CLR_SCHEME* D4D_ScreenGetScheme(D4D_SCREEN * pScreen)
{
  if(pScreen == NULL)
    return pSchemeDefault;

  if(pScreen->clrScheme != NULL)
    return pScreen->clrScheme;

  return pSchemeDefault;
}

/**************************************************************************/ /*!
* @brief   Function return the pointer to current use object scheme of object
* @param   pObj - pointer to object
* @return  pointer to object current use of color scheme
* @note    This function find out the current use object scheme of object.
*          It handles with standard default scheme, screen color scheme and also
*          directly with user object color scheme.
*******************************************************************************/
D4D_CLR_SCHEME* D4D_ObjectGetScheme(D4D_OBJECT * pObj)
{
  if(pObj == NULL)
    return pSchemeDefault;

  if(pObj->clrScheme != NULL)
    return pObj->clrScheme;

  if(pObj->pRelations)
  {
    D4D_OBJECT* pParent = D4D_GetParentObject((D4D_OBJECT*)pObj);

    if(pParent)
      return D4D_ObjectGetScheme(pParent);
  }

  return D4D_ScreenGetScheme(pObj->pData->pScreen);
}

/**************************************************************************/ /*!
* @brief   Function return the pointer to default color scheme
* @return  pointer to screen current use of color scheme
* @note    This function just return current used default color scheme pointer
*******************************************************************************/
D4D_CLR_SCHEME* D4D_GetDefaultScheme(void)
{
  return pSchemeDefault;
}

/**************************************************************************/ /*!
* @brief   Function sets the new default color scheme
* @param   pScheme - pointer to object
* @return  none
* @note    This function sets the new default color scheme. If by parameter is handled NULL,
*               then it's used default color scheme defined in d4d_usrcfg.h file.
*******************************************************************************/
void D4D_SetDefaultScheme(D4D_CLR_SCHEME* pScheme)
{
  if(pScheme == NULL)
    pSchemeDefault = (D4D_CLR_SCHEME*)&d4d_clr_scheme_default;
  else
    pSchemeDefault = pScheme;
}

/**************************************************************************/ /*!
* @brief   Function return object current fore color
* @param   pObj - pointer to object
* @param   draw - draw runtime flags handled to object by D4D_MSG_ONDRAW
* @return  Current used fore color of object.
* @note    Function by object flags and also draw parameter return one of object color from
*          color scheme - normal/disable/focus/capture colors.
*******************************************************************************/
D4D_COLOR D4D_ObjectGetForeColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);

  if(!D4D_IsEnabled(pObj))
    return pScheme->foreDis;

  if(draw & D4D_OBJECT_DRAWFLAGS_CAPTURING)
    return pScheme->foreCapture;

  if(draw & D4D_OBJECT_DRAWFLAGS_FOCUSED)
    return pScheme->foreFocus;

  return pScheme->fore;
}

/**************************************************************************/ /*!
* @brief   Function return object current background color
* @param   pObj - pointer to object
* @param   draw - draw runtime flags handled to object by D4D_MSG_ONDRAW
* @return  Current used background color of object.
* @note    Function by object flags and also draw parameter return one of object color from
*          color scheme - normal/disable/focus/capture colors.
*******************************************************************************/
D4D_COLOR D4D_ObjectGetBckgColor(D4D_OBJECT * pObj, D4D_OBJECT_DRAWFLAGS draw)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);

  if(!D4D_IsEnabled(pObj))
    return pScheme->bckgDis;

  if(draw & D4D_OBJECT_DRAWFLAGS_CAPTURING)
    return pScheme->bckgCapture;

  if(draw & D4D_OBJECT_DRAWFLAGS_FOCUSED)
    return pScheme->bckgFocus;

  return pScheme->bckg;
}

/**************************************************************************/ /*!
* @brief   Function return object current fill fore color
* @param   pObj - pointer to object
* @return  Current used fill fore color of object.
* @note    Function by object flags return one of object color from
*          color scheme - normal/disable colors.
*******************************************************************************/
D4D_COLOR D4D_ObjectGetForeFillColor(D4D_OBJECT * pObj)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);

  if(!D4D_IsEnabled(pObj))
    return pScheme->foreDis;

  return pScheme->fore;
}

/**************************************************************************/ /*!
* @brief   Function return object current fill background color
* @param   pObj - pointer to object
* @return  Current used fill background color of object.
* @note    Function by object flags return one of object color from
*          color scheme - normal/disable colors.
*******************************************************************************/
D4D_COLOR D4D_ObjectGetBckgFillColor(D4D_OBJECT * pObj)
{
  D4D_CLR_SCHEME *pScheme = D4D_ObjectGetScheme(pObj);

  if(!D4D_IsEnabled(pObj))
    return pScheme->bckgDis;

  return pScheme->bckg;
}

/**************************************************************************/ /*!
* @brief   Compute cross color between two basic color in 256 steps
* @param   startColor - starting color for computing cross color
* @param   endColor - ending color for computing cross color
* @param   value - crossing value <0-255>
* @return  Crrossing color between begin and end color handled in parameters by handled step(value).
* @note    Function compute by simple proportion the cross color between two colors by handled step.
*******************************************************************************/
D4D_COLOR D4D_GetCrossColor(D4D_COLOR startColor, D4D_COLOR endColor, Byte value)
{
  Byte r, g, b;
  Byte c_s, c_e;

  //Red color computing
  c_s = D4D_COLOR_GET_R(startColor);
  c_e = D4D_COLOR_GET_R(endColor);

  if(c_s < c_e)
    r = (Byte)(c_s + D4D_MulDivUU8(value, (Byte)(c_e - c_s), 255));
  else
    r = (Byte)(c_s - D4D_MulDivUU8(value, (Byte)(c_s - c_e), 255));

  //Green color computing
  c_s = D4D_COLOR_GET_G(startColor);
  c_e = D4D_COLOR_GET_G(endColor);

  if(c_s < c_e)
    g = (Byte)(c_s + D4D_MulDivUU8(value, (Byte)(c_e - c_s), 255));
  else
    g = (Byte)(c_s - D4D_MulDivUU8(value, (Byte)(c_s - c_e), 255));

  //Blue color computing
  c_s = D4D_COLOR_GET_B(startColor);
  c_e = D4D_COLOR_GET_B(endColor);

  if(c_s < c_e)
    b = (Byte)(c_s + D4D_MulDivUU8(value, (Byte)(c_e - c_s), 255));
  else
    b = (Byte)(c_s - D4D_MulDivUU8(value, (Byte)(c_s - c_e), 255));

  return D4D_COLOR_RGB(r, g, b);

}

/**************************************************************************/ /*!
* @brief   Compute change of color intesity
* @param   color - input color to change intesity
* @param   intensity - change of intesity - signed <-100 .. 100>
* @return  New color computed from input color and intensity change.
* @note    Function compute by simple proportion the color with changed intesity in +-100% scale.
*******************************************************************************/
D4D_COLOR D4D_ChangeColorIntesity(D4D_COLOR color, sByte intensity)
{
  sWord r, g, b;
  Byte absIntesity;

  if(intensity > 100)
    intensity = 100;

  if(intensity < -100)
    intensity = -100;

  absIntesity = D4D_Abs(intensity);


  r = D4D_MulDivUU8(absIntesity, D4D_COLOR_GET_R(color), 100);
  g = D4D_MulDivUU8(absIntesity, D4D_COLOR_GET_G(color), 100);
  b = D4D_MulDivUU8(absIntesity, D4D_COLOR_GET_B(color), 100);

  if(intensity > 0)
  {
    r += D4D_COLOR_GET_R(color);
    if(r > 255)
      r = 255;

    g += D4D_COLOR_GET_G(color);
    if(g > 255)
      g = 255;

    b += D4D_COLOR_GET_B(color);
    if(b > 255)
      b = 255;
  }else
  {
    r = (Byte)(D4D_COLOR_GET_R(color) - r);
    g = (Byte)(D4D_COLOR_GET_G(color) - g);
    b = (Byte)(D4D_COLOR_GET_B(color) - b);
  }

  return D4D_COLOR_RGB(r, g, b);
}

/**************************************************************************/ /*!
* @brief   Compute the grayscale color
* @param   color - input color to change to grey scale
* @return  New grey scale color computed from input color
* @note    Function compute the grey scale color from input color.
*******************************************************************************/
D4D_COLOR D4D_GetGreyScale(D4D_COLOR color)
{
  Byte red, green, blue;

  // Get the separate color components
  red = D4D_COLOR_GET_R(color);
  green = D4D_COLOR_GET_G(color);
  blue = D4D_COLOR_GET_B(color);

  // Get the arithmetic mean of all color components and store it back as individual components
  red = green = blue = (Byte)((Word)(red + green + blue) / 3);

  // Create the color in GreyScale
  return D4D_COLOR_RGB(red, green, blue);
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_scheme_func                                              */
/******************************************************************************/
