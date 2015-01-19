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
* @file      d4d_extsrc.c
*
* @author   Michal hanak, Petr Gargulak
*
* @version   0.0.12.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver extscr functions c file
*
******************************************************************************/

#include "d4d.h"
#include "common_files/d4d_private.h"
#include "common_files/d4d_lldapi.h"

#ifdef D4D_STD_STRING_H_INCLUDE
  #include D4D_STD_STRING_H_INCLUDE
#endif

#if (D4D_EXTSRC_TEXT_ENABLE != D4D_FALSE) || (D4D_EXTSRC_FILE_ENABLE != D4D_FALSE)
  Byte d4d_extsrcBuffer[D4D_EXTSRC_BUFF_SIZE];
#endif

#if (D4D_EXTSRC_FILE_ENABLE != D4D_FALSE)

/******************************************************************************
* D4D External resources functions headers
*
*//*! @addtogroup doxd4d_extsrc_func
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Function that return back the file extension
* @param   pFileName - pointer file name string
* @return  the pointer to file extension (it point to same string)
* @note    In case that the file name doesn't content extension, the function returns NULL.
*******************************************************************************/
D4D_CHAR* D4D_GetFileExtension(D4D_CHAR* pFileName)
{
  int i;

  if(pFileName == NULL)
    return NULL;


  i = D4D_StrLen(pFileName);

  if(i == 0)
    return NULL;

  i--; // decrement index to point on last char

  while((pFileName[i] >= ' ') && (pFileName[i] != '.'))
  {
    i--;
  };

  if(pFileName[i] == '.')
    return &pFileName[i+1];
  else
    return NULL;
}

/**************************************************************************/ /*!
* @brief   The function find out if the path is absolute or relative
* @param   pFileName - pointer file name string
* @return  D4D_TRUE - The file name path is absolete, D4D_FALSE - relative absolute path
* @note    None
*******************************************************************************/
D4D_BOOL D4D_FileIsAbsolutePath(D4D_CHAR* pFileName)
{
  Byte i;

  if(!pFileName || !pFileName[0])
    return D4D_FALSE;

  for(i=1; i<=D4D_DRIVE_MAX_STRLEN; ++i) {
    if(pFileName[i] == ':')
      return D4D_TRUE;
  }

  return D4D_FALSE;
}
#endif

/**************************************************************************/ /*!
* @brief   The function returns length of string terminated by zero
* @param   s - input string
* @return  length of string
* @note    If the D4D_STD_STRING_H_INCLUDE is defined, the library strlen() function is called.
*          Only 1 Byte wide char is handled (not UNICODE).
*******************************************************************************/
int D4D_StrLen(register const D4D_CHAR *s)
{
#ifndef D4D_STD_STRING_H_INCLUDE
  register int n;

  if(!s)
    return 0;

  n = 0;
  while (*s++)
          n++;
  return(n);
#else
  // Standard system implementation
  return strlen(s);
#endif
}

/**************************************************************************/ /*!
* @brief   The function copy string one string to other one (terminated by zero)
* @param   sd - destination string
* @param   ss - source string
* @return  pointer to destination string
* @note    If the D4D_STD_STRING_H_INCLUDE is defined, the library strcpy() function is called.
*          Only 1 Byte wide char is handled (not UNICODE).
*******************************************************************************/
D4D_CHAR* D4D_StrCopy(register D4D_CHAR *sd, register const D4D_CHAR *ss)
{
#ifndef D4D_STD_STRING_H_INCLUDE
  register D4D_CHAR *s = sd;

  while (*sd++ = *ss++)
    ;

  return s;
#else
  // Standard system implementation
  return strcpy(sd, ss);
#endif
}

/**************************************************************************/ /*!
* @brief   The function copy string one string (UNICODE) to other one (STANDARD 8 bit)(terminated by zero)
* @param   sd - destination string
* @param   ss - source string
* @return  pointer to destination string
* @note    Is used to convert strings from standard ASCII to Unicode.
*******************************************************************************/
D4D_CHAR* D4D_StrCopyUnicode2Ascii(register D4D_CHAR *sd, register const D4D_WCHAR *ss)
{
  register D4D_CHAR *s = sd;

  while (*sd++ = (D4D_CHAR)*ss++)
    ;

  return s;
}

/**************************************************************************/ /*!
* @brief   The function copy string one string (STANDARD 8 bit) to other one (UNICODE)(terminated by zero)
* @param   sd - destination string
* @param   ss - source string
* @return  pointer to destination string
* @note    Is used to convert strings from Unicode to standard ASCII.
*******************************************************************************/
D4D_WCHAR* D4D_StrCopyAscii2Unicode(register D4D_WCHAR *sd, register const D4D_CHAR *ss)
{
  register D4D_WCHAR *s = sd;

  while (*sd++ = (D4D_WCHAR)*ss++)
    ;

  return s;
}

/**************************************************************************/ /*!
* @brief   The function concatenate strings (terminated by zero)
* @param   sd - destination string
* @param   ss - source string
* @return  pointer to destination string
* @note    If the D4D_STD_STRING_H_INCLUDE is defined, the library strcat() function is called.
*          Only 1 Byte wide char is handled (not UNICODE).
*******************************************************************************/
D4D_CHAR* D4D_StrCat(register D4D_CHAR *sd, register const D4D_CHAR *ss)
{
#ifndef D4D_STD_STRING_H_INCLUDE
  register D4D_CHAR *s = sd;

  while (*sd)
    sd++;

  while ((*sd++ = *ss++))
    ;

  return s;
#else
  // Standard system implementation
  return strcat(sd, ss);
#endif
}

/**************************************************************************/ /*!
* @brief   The function concatenate strings (terminated by zero) (UNICODE version)
* @param   sd - destination string
* @param   ss - source string
* @return  pointer to destination string
* @note    Handling just only UNICODE strings.
*******************************************************************************/
D4D_WCHAR* D4D_StrCatUnicode(register D4D_WCHAR *sd, register const D4D_WCHAR *ss)
{
  register D4D_WCHAR *s = sd;

  while (*sd)
    sd++;

  while (*sd++ = *ss++)
    ;

  return s;
}

/**************************************************************************/ /*!
* @brief   The function copare two strings (terminated by zero)
* @param   s1 - first string
* @param   s2 - second string
* @return  0 - strings are identical, <>0 - strings are different
* @note    If the D4D_STD_STRING_H_INCLUDE is defined, the library strcmp() function is called.
*          Only 1 Byte wide char is handled (not UNICODE).
*******************************************************************************/
int D4D_CompareStrings(const D4D_CHAR* s1, const D4D_CHAR* s2)
{
#ifndef D4D_STD_STRING_H_INCLUDE
  D4D_CHAR val2 = *s2++;
    while(val2)
    {
        if (*s1++ != val2)
        {
            if (s1[-1] < val2)
                return -1;

            return 1;
        }
        val2 = *s2++;
    }

    if (*s1)        // if we get to here, then s2 terminated
      return 1;   // s1 is still going

    return 0;       // they both terminated
#else
  // Standard system implementation
  return strcmp(s1, s2);
#endif
}

/**************************************************************************/ /*!
* @brief   The function copare two strings (terminated by zero) (UNICODE version)
* @param   s1 - first string
* @param   s2 - second string
* @return  0 - strings are identical, <>0 - strings are different
* @note    The strings MUST have type D4D_WCHAR*.
*******************************************************************************/
int D4D_CompareStringsUnicode(const D4D_WCHAR* s1, const D4D_WCHAR* s2)
{
  D4D_WCHAR val2 = *s2++;

  while(val2)
  {
      if (*s1++ != val2)
      {
          if (s1[-1] < val2)
              return -1;

          return 1;
      }
      val2 = *s2++;
  }

  if (*s1)        // if we get to here, then s2 terminated
    return 1;   // s1 is still going

  return 0;       // they both terminated
}

/**************************************************************************/ /*!
* @brief   The function convert all small alpha characters to upper in string(terminated by zero)
* @param   s - input string
* @return  none
* @note    Only 1 Byte wide char is handled (not UNICODE).
*******************************************************************************/
void D4D_ToUpper(D4D_CHAR* s)
{
  while(s && *s)
  {
    if((*s >= 'a') && (*s <= 'z'))
      *s -= 'a' - 'A';

    s++;
  }
}

/**************************************************************************/ /*!
* @brief   The function swap bytes in Long
* @param   n - input long number
* @return  swapped input long number
* @note    Is Used to handling endianism.
*******************************************************************************/
LWord D4D_SwapLong(LWord n)
{
  Byte temp[4];

  temp[3] = *((Byte *)&n);
  temp[2] = ((Byte *)&n)[1];
  temp[1] = ((Byte *)&n)[2];
  temp[0] = ((Byte *)&n)[3];
  return (*(LWord *)temp);
}

/**************************************************************************/ /*!
* @brief   The function swap bytes in Word
* @param   n - input word number
* @return  swapped input word number
* @note    Is Used to handling endianism.
*******************************************************************************/
Word D4D_SwapWord(Word n)
{
  Byte temp[2];

  temp[1] = *((Byte *)&n);
  temp[0] = ((Byte *)&n)[1];
  return (*(Word *)temp);
}

/*! @} End of doxd4d_extsrc_func                                             */
