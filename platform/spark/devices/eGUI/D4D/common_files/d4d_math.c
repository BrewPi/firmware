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
* @file      d4d_math.c
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.28.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver math functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"

const sByte d4d_sinTbl[64] =
{
    0x00,
    0x03,
    0x06,
    0x09,
    0x0D,
    0x10,
    0x13,
    0x16,
    0x19,
    0x1C,
    0x1F,
    0x22,
    0x25,
    0x28,
    0x2B,
    0x2E,
    0x31,
    0x34,
    0x37,
    0x3A,
    0x3D,
    0x40,
    0x42,
    0x45,
    0x48,
    0x4A,
    0x4D,
    0x4F,
    0x52,
    0x54,
    0x56,
    0x59,
    0x5B,
    0x5D,
    0x5F,
    0x61,
    0x63,
    0x65,
    0x67,
    0x69,
    0x6B,
    0x6C,
    0x6E,
    0x70,
    0x71,
    0x72,
    0x74,
    0x75,
    0x76,
    0x77,
    0x78,
    0x79,
    0x7A,
    0x7B,
    0x7C,
    0x7C,
    0x7D,
    0x7D,
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7F,
    0x7F
};

/***************************************************************************//*!
*
* @brief  Fast 16/8=8 divide operation with rounding and saturation
*
* @param num The 16-bit numerator to be divided
* @param div The 8-bit denominator
*
* This function divides a given 16-bit value by a 8-bit divisor and returns
* 8-bit quotient. Half of the divisor is added to divident before the division
* so the result is rounded.
*
* @return This function returns the 8-bit quotient. Returned value is 255
*         when overflow occurs.
*
******************************************************************************/
/*
#pragma NO_ENTRY
#pragma NO_EXIT
#pragma NO_RETURN
#pragma NO_FRAME
*/
static Byte D4D_Divide8(Word num, Byte div)
{
    D4D_UNUSED(num);
    D4D_UNUSED(div);

#if (D4D_MCU_TYPE == D4D_HC08) || (D4D_MCU_TYPE == D4D_HCS08)
    asm
    {
        PSHA
        PSHX
        PSHH

        LSRA            /* div/2 */
        ADD   2,SP      /* add div/2 to num on the stack */
        STA   2,SP
        CLRA
        ADC   1,SP
        STA   1,SP

        PULH
        PULA            /* HA = num + div/2 */
        PULX            /* X = div */

        DIV             /* A=HA/X, H=HA%X */
        BCC ok          /* return A when okay */
        LDA #0xff       /* return 255 on overflow */
    ok: RTS
    }

    /* ignored dead code (just for C syntax) */
    return 0;
#elif (D4D_MCU_TYPE == D4D_HC12) || (D4D_MCU_TYPE == D4D_HCS12) || (D4D_MCU_TYPE == D4D_HCS12X)
    return (Byte)(num/div);
#elif (D4D_MCU_TYPE == D4D_MCF51) || (D4D_MCU_TYPE == D4D_MCF52) || (D4D_MCU_TYPE == D4D_MPC51) || (D4D_MCU_TYPE == D4D_MK)
    return (Byte)(num/div);
#else
  #error "Not supported MCU for D4D math divide!"
#endif

    // ignored dead code (just for C syntax)

}

static Word D4D_Divide16(LWord num, Word div)
{
  return (Word)(num/div);

}

/******************************************************************************
* Begin of D4D_MATH public functions
*//*! @addtogroup doxd4d_math_func
* @{
*******************************************************************************/

/***************************************************************************//*!
*
* @brief  Simple proportion unsigned calculation -  8 bit
*
* @param u1 as unsigned integer <0..0xFF> ~ <0..255>
* @param u2 as unsigned integer <0..0xFF> ~ <0..255>
* @param d (divider) as unsigned integer <0..0xFF> ~ <0..255>
*
* This function calculates the simple proportion value of arguments (u1*u2) / d
*
* @return unsigned simple proportion value <0..255> ~ <0x0..0xFF>
*
*****************************************************************************/
Byte D4D_MulDivUU8(Byte u1, Byte u2, Byte d)
{
    Word n = (Word)(((Word)u1 * (Word)u2) + (d / 2));
    return D4D_Divide8(n, d);
}

/***************************************************************************//*!
*
* @brief  Simple proportion signed calculation -  8 bit
*
* @param s as signed integer <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F>
* @param u as unsigned integer <0..0xFF> ~ <0..255>
* @param d (divider) as unsigned integer <0..0xFF> ~ <0..255>
*
* This function calculates the simple proportion value of arguments (s*u) / d
*
* @return unsigned simple proportion value <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F>
*
*****************************************************************************/
sByte D4D_MulDivSU8(sByte s, Byte u, Byte d)
{
    Word n;
    Byte r;

    if(s >= 0)
    {
        n = (Word)(((Byte)s * (Word)u) + (d / 2));
        r = D4D_Divide8(n, d);
        return (sByte)((r > (Byte)127)? 127 : r);
    }
    else
    {
        n = (Word)((((Byte)-s) * (Word)u) + (d / 2));
        r = D4D_Divide8(n, d);
        return (sByte)(r > 128 ? -128 : (sByte)(-1 * ((sByte)r)));
    }
}

/***************************************************************************//*!
*
* @brief  Simple proportion unsigned calculation -  16 bit
*
* @param u1 as unsigned integer <0..0xFFFF> ~ <0..65535>
* @param u2 as unsigned integer <0..0xFFFF> ~ <0..65535>
* @param d (divider) as unsigned integer <0..0xFFFF> ~ <0..65535>
*
* This function calculates the simple proportion value of arguments (u1*u2) / d
*
* @return unsigned simple proportion value <0..0xFFFF> ~ <0..65535>
*
*****************************************************************************/
Word D4D_MulDivUU16(Word u1, Word u2, Word d)
{
    LWord n = (LWord)((LWord)((LWord)u1 * (LWord)u2) + (d / 2));
    return D4D_Divide16(n, d);
}

/***************************************************************************//*!
*
* @brief  Simple proportion signed calculation -  8 bit
*
* @param s as signed integer <-32768..-1,0..32767> ~ <0xFFFF..0x8000,0x00..0x7FFF>
* @param u as unsigned integer <0..0xFFFF> ~ <0..65535>
* @param d (divider) as unsigned integer <0..0xFFFF> ~ <0..65535>
*
* This function calculates the simple proportion value of arguments (s*u) / d
*
* @return unsigned simple proportion value <-32768..-1,0..32767> ~ <0xFFFF..0x8000,0x00..0x7FFF>
*
*****************************************************************************/
sWord D4D_MulDivSU16(sWord s, Word u, Word d)
{
    LWord n;
    Word r;

    if(s >= 0)
    {
        n = (LWord)(((Word)s * (LWord)u) + (d / 2));
        r = D4D_Divide16(n, d);
        return (sWord)((r > (Word)0x7FFF)? 0x7FFF : r);
    }
    else
    {
        n = (LWord)((((Word)-s) * (LWord)u)) + (d / 2);
        r = D4D_Divide16(n, d);
        return (sWord)(r > 0x8000 ? -32768 : (sWord)(-1 * ((sWord)r)));
    }
}

/***************************************************************************//*!
*
* @brief  Calculation of absolute value -  8 bit
*
* @param num as signed integer <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F>
*
* This function calculates the absolute value (unsigned) of argument num(signed)
*
* @return unsigned absolute value <0..0x80> ~ <0..128>
*
*****************************************************************************/
Byte D4D_Abs(sByte num)
{
  if(num < 0)
    num = (sByte)(num * -1);

  return (Byte)num;
}

/***************************************************************************//*!
*
* @brief  Calculation of absolute value -  16 bit
*
* @param num as signed integer <-32768..-1,0..32767> ~ <0xFFFF..0x8000,0x00..0x7FFF>
*
* This function calculates the absolute value (unsigned) of argument num(signed)
*
* @return unsigned absolute value <0..0x8000> ~ <0..32768>
*
*****************************************************************************/
Word D4D_Abs16(sWord num)
{
  if(num < 0)
    num = (sWord)(num * -1);

  return (Word)num;
}

/***************************************************************************//*!
*
* @brief  Calculation of absolute value -  32 bit
*
* @param num as signed integer <-2,147,483,648..-1,0..2,147,483,647> ~ <0xFFFFFFFF..0x80000000,0x00000000..0x7FFFFFFF>
*
* This function calculates the absolute value (unsigned) of argument num(signed)
*
* @return unsigned absolute value <0..0x80000000> ~ <0..2,147,483,648>
*
*****************************************************************************/
LWord D4D_Abs32(sLWord num)
{
  if(num < 0)
    num = (sLWord)(num * -1);

  return (LWord)num;
}

/***************************************************************************//*!
*
* @brief  Calculation of limitation value (unsigned) -  16 bit
*
* @param val as unsigned integer <0..0xFFFF> ~ <0..65535> - value that will be limited
*
* @param min as unsigned integer <0..0xFFFF> ~ <0..65535> - minimal limitation value
*
* @param max as unsigned integer <0..0xFFFF> ~ <0..65535> - maximal limitation value
*
* This function calculates the limitation of value by input value borders
*
* @return unsigned limited value <min..max>
*
*****************************************************************************/
Word D4D_LimitU16(Word val, Word min, Word max)
{
  if(val < min)
    return min;

  if(val > max)
    return max;


  return val;
}

/***************************************************************************//*!
*
* @brief  Calculation of limitation value (signed) -  16 bit
*
* @param val as signed integer <-32768..-1,0..32767> ~ <0xFFFF..0x8000,0x00..0x7FFF> - value that will be limited
*
* @param min as signed integer <-32768..-1,0..32767> ~ <0xFFFF..0x8000,0x00..0x7FFF> - minimal limitation value
*
* @param max as signed integer <-32768..-1,0..32767> ~ <0xFFFF..0x8000,0x00..0x7FFF> - maximal limitation value
*
* This function calculates the limitation of value by input value borders
*
* @return signed limited value <min..max>
*
*****************************************************************************/
sWord D4D_LimitS16(sWord val, sWord min, sWord max)
{
  if(val < min)
    return min;

  if(val > max)
    return max;


  return val;
}

/***************************************************************************//*!
*
* @brief  Calculation of limitation value (unsigned) -  8 bit
*
* @param val as unsigned integer <0..0xFF> ~ <0..255> - value that will be limited
*
* @param min as unsigned integer <0..0xFF> ~ <0..255> - minimal limitation value
*
* @param max as unsigned integer <0..0xFF> ~ <0..255> - maximal limitation value
*
* This function calculates the limitation of value by input value borders
*
* @return unsigned limited value <min..max>
*
*****************************************************************************/
Byte D4D_LimitU8(Byte val, Byte min, Byte max)
{
  if(val < min)
    return min;

  if(val > max)
    return max;


  return val;
}

/***************************************************************************//*!
*
* @brief  Calculation of limitation value (signed) -  8 bit
*
* @param val as signed integer <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> - value that will be limited
*
* @param min as signed integer <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> - minimal limitation value
*
* @param max as signed integer <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> - maximal limitation value
*
* This function calculates the limitation of value by input value borders
*
* @return signed limited value <min..max>
*
*****************************************************************************/
sByte D4D_LimitS8(sByte val, sByte min, sByte max)
{
  if(val < min)
    return min;

  if(val > max)
    return max;

  return val;
}

/***************************************************************************//*!
*
* @brief  Sine calculation using lookup table
*
* @param x as unsigned integer <0..0xFF> ~ <0..2PI)
*
* This function calculates the sine value of argument phase
* 64 x 8 bit look-up table.
*
* @return signed sine value <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> ~ <-1,+1>
*
*****************************************************************************/

sByte D4D_Sin(Byte x)
{
    if(x < 0x80)
    {
        if(x <= 0x3F)
            return d4d_sinTbl[x];
        else
            return d4d_sinTbl[0x3F-(x-0x40)];
    }
    else
    {
        if (x <= 0xBF)
            return (sByte)(-d4d_sinTbl[x-0x80]);
        else
            return (sByte)(-d4d_sinTbl[0x3F-(x-0xC0)]);
    }
}

/***************************************************************************//*!
*
* @brief  Cosine calculation using lookup table
*
* @param x as unsigned integer <0..0xFF> ~ <0..2PI)
*
* This function calculates the cosine value of argument phase
* 64 x 8 bit look-up table.
*
* @return signed cosine value <-128..-1,0..127> ~ <0xFF..0x80,0x00..0x7F> ~ <-1,+1>
*
*****************************************************************************/
sByte D4D_Cos(Byte x)
{
    if(x < 0x80)
    {
        if(x <= 0x3F)
            return d4d_sinTbl[0x3F-x];
        else
            return (sByte)(-d4d_sinTbl[x-0x40]);
    }
    else
    {
        if (x <= 0xBF)
            return (sByte)(-d4d_sinTbl[0x3F-(x-0x80)]);
        else
            return d4d_sinTbl[x-0xC0];
    }
}

/******************************************************************************
*       End of public functions                                               */
/*! @} End of doxd4d_math_func                                               */
/******************************************************************************/

/******************************************************************************
*       Private functions                                                     *
******************************************************************************/
