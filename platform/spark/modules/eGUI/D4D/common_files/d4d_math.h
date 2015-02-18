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
* @file      d4d_math.h
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.17.0
*
* @date      Oct-2-2013
*
* @brief     D4D Driver math helper functions header file
*
*******************************************************************************/

#ifndef __D4D_MATH_H
#define __D4D_MATH_H

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/

// convert deg angle of -180..180 to internal representation (-128..127)
#define D4D_ANGLE_DEG(deg) (((deg)==180)?0xff:((deg)*0x100/180))

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/

sByte D4D_Sin(Byte x);
sByte D4D_Cos(Byte x);

Byte D4D_MulDivUU8(Byte u1, Byte u2, Byte d);
sByte D4D_MulDivSU8(sByte s, Byte u, Byte d);

Word D4D_MulDivUU16(Word u1, Word u2, Word d);
sWord D4D_MulDivSU16(sWord s, Word u, Word d);

Byte D4D_Abs(sByte num);
Word D4D_Abs16(sWord num);
LWord D4D_Abs32(sLWord num);

Word D4D_LimitU16(Word val, Word min, Word max);
sWord D4D_LimitS16(sWord val, sWord min, sWord max);

Byte D4D_LimitU8(Byte val, Byte min, Byte max);
sByte D4D_LimitS8(sByte val, sByte min, sByte max);

#ifdef D4D_COOR_TYPE
  #define D4D_MulDivUU  D4D_MulDivUU16
  #define D4D_MulDivSU  D4D_MulDivSU16
  #define D4D_LimitU  D4D_LimitU16
  #define D4D_LimitS  D4D_LimitS16
#else

  #define D4D_MulDivUU  D4D_MulDivUU8
  #define D4D_MulDivSU  D4D_MulDivSU8
  #define D4D_LimitU  D4D_LimitU8
  #define D4D_LimitS  D4D_LimitS8
#endif


#endif /* __D4D_MATH_H */
