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
* @file      d4d_extsrc.h
*
* @author     Petr Gargulak
*
* @version   0.0.18.0
*
* @date      Oct-2-2013
*
* @brief     D4D Driver extscr helper functions header file
*
*******************************************************************************/

#ifndef __D4D_EXTSRC_H
#define __D4D_EXTSRC_H

#if !defined(__MQX__) && !defined(D4D_OS_MQX)
 // #include <stdlib.h>
#endif

/******************************************************************************
* D4D External Resources  constants
*
*//*! @addtogroup doxd4d_extsrc_const
* @{
*******************************************************************************/
/*! @brief D4D_IMG_WORK_PATH_DEFAULT constant default declaration   ("a:\\")     */
/*! @note D4D_IMG_WORK_PATH_DEFAULT sets the default path for eGUI images in case that
    relative path is used for image declaration. The image path could be changed by
    void D4D_ExtImgSetWorkPath(D4D_CHAR* pPath). */
#ifndef D4D_IMG_WORK_PATH_DEFAULT
  #define D4D_IMG_WORK_PATH_DEFAULT "a:\\"
#endif

/*! @brief D4D_FNT_WORK_PATH_DEFAULT constant default declaration  ("a:\\")      */
/*! @note D4D_FNT_WORK_PATH_DEFAULT sets the default path for eGUI fonts in case that
    relative path is used for fonts declaration. The font path could be changed by
    void D4D_ExtFntSetWorkPath(D4D_CHAR* pPath). */
#ifndef D4D_FNT_WORK_PATH_DEFAULT
  #define D4D_FNT_WORK_PATH_DEFAULT "a:\\"
#endif

/*! @brief D4D_DRIVE_MAX_STRLEN constant default declaration  (8)                */
/*! @note D4D_DRIVE_MAX_STRLEN sets the default maximum length of the drive identifier
    recognized in the absolute paths. E.g. length of "a:" is 1 while "usb:" is 3. */
#ifndef D4D_DRIVE_MAX_STRLEN
  #define D4D_DRIVE_MAX_STRLEN 8
#endif

/*! @} End of doxd4d_extsrc_const                                              */

/******************************************************************************
* D4D External Resources types
*
*//*! @addtogroup doxd4d_extsrc_type
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   File open eGUI function type
* @param   filename - pointer to string with file name
* @param   mode - mode of open file
* @return  pointer to file structure
* @note    The type should reflect standard File Open function type
*******************************************************************************/
typedef D4D_FILEPTR (*D4DFILE_FOPEN)(D4D_CHAR* filename, D4D_CHAR* mode);

/**************************************************************************/ /*!
* @brief   File close eGUI function type
* @param   p_file - pointer to file structure
* @return  result of operation 0 - OK
* @note    The type should reflect standard File Close function type
*******************************************************************************/
typedef int (*D4DFILE_FCLOSE)(D4D_FILEPTR p_file);

/**************************************************************************/ /*!
* @brief   File read eGUI function type
* @param   p_file - pointer to file structure
* @param   buffer - pointer to application buffer
* @param   size - size of data to read
* @return  result of operation >0 read bytes, <0 error
* @note    The type should reflect standard File Read function type
*******************************************************************************/
typedef int (*D4DFILE_READ)(D4D_FILEPTR p_file, void* buffer, int size);

/**************************************************************************/ /*!
* @brief   File write eGUI function type
* @param   p_file - pointer to file structure
* @param   buffer - pointer to application buffer
* @param   size - size of data to write
* @return  result of operation >0 written bytes, <0 error
* @note    The type should reflect standard File Write function type
*******************************************************************************/
typedef int (*D4DFILE_WRITE)(D4D_FILEPTR p_file, const void* buffer, int size);

/**************************************************************************/ /*!
* @brief   File seek eGUI function type
* @param   p_file - pointer to file structure
* @param   offset - new offset from select origin
* @param   origin - origin of offset
* @return  result of operation 0 - OK
* @note    The type should reflect standard File Seek function type
*******************************************************************************/
typedef int (*D4DFILE_FSEEK)(D4D_FILEPTR p_file, long offset, int origin);

/*! @brief D4D_SEEK_SET eGUI Origin constant for begin of file (1)            */
#define D4D_SEEK_SET    1
/*! @brief D4D_SEEK_CUR eGUI Origin constant for current position of file (2) */
#define D4D_SEEK_CUR    2
/*! @brief D4D_SEEK_END eGUI Origin constant for end of file (3)              */
#define D4D_SEEK_END    3

/**************************************************************************/ /*!
* @brief   File tell eGUI function type
* @param   p_file - pointer to file structure
* @return  >0 current position of file, <0 error
* @note    The type should reflect standard File Tell function type
*******************************************************************************/
typedef long (*D4DFILE_FTELL)(D4D_FILEPTR p_file);

/**************************************************************************/ /*!
* @brief   String translate function for customizing strings
* @param   originTxt - pointer to original string that should be translate to customize one
* @param   newTxt - pointer to new string that should be printed
* @param   offset - offset in new string that should be returned in newTxt buffer
* @param   size - size of customized chunk that should be returned in newTxt buffer
* @return  length of printed string to newTxt buffer
* @note    This is way how to print in eGUI any string customized. Is called if the D4D_EXTSRC_TEXT_ENABLE
*          is enabled, and the string first character is D4D_STRINGTABLE_PREFIX and the string doesn't
*          content of string tables.
*******************************************************************************/
typedef int (*D4DLOCALE_TRANSLATE_STR)(const D4D_TCHAR* originTxt, D4D_TCHAR* newTxt, int offset, int size);

/**************************************************************************/ /*!
* @brief   String translate string get lenght function for customized strings
* @param   originTxt - pointer to original string that should be translate to customize one
* @return  length of the translated string
* @note    This is way how to print in eGUI any string customized. Is called if the D4D_EXTSRC_TEXT_ENABLE
*          is enabled, and the string first character is D4D_STRINGTABLE_PREFIX and the string doesn't
*          content of string tables. This function should return length of traslated string to eGUI.
*******************************************************************************/
typedef int (*D4DLOCALE_GET_TRANSLATE_STR_LEN)(const D4D_TCHAR* originTxt);

/*! @brief   eGUI structure to store API for external files support*/
typedef struct D4DFILE_FUNCTIONS_S
{
  D4DFILE_FOPEN D4DFILE_fopen;          ///< Pointer to File Open Function
  D4DFILE_FCLOSE D4DFILE_fclose;        ///< Pointer to File Close Function
  D4DFILE_READ D4DFILE_read;            ///< Pointer to File Read Function
  D4DFILE_WRITE D4DFILE_write;          ///< Pointer to File Write Function
  D4DFILE_FSEEK D4DFILE_fseek;          ///< Pointer to File Seek Function
  D4DFILE_FTELL D4DFILE_ftell;          ///< Pointer to File Tell Function
}D4DFILE_FUNCTIONS;

/*! @brief   eGUI structure to store API for external strings (translated) support*/
typedef struct D4DLOCALE_FUNCTIONS_S
{
  D4DLOCALE_TRANSLATE_STR D4DLOCALE_TranslateStr;                       ///< Pointer to Translate String Function
  D4DLOCALE_GET_TRANSLATE_STR_LEN D4DLOCALE_GetTranslateStrLength;      ///< Pointer to Get Translate String Length Function
}D4DLOCALE_FUNCTIONS;

/*! @} End of doxd4d_extsrc_type                                               */

/******************************************************************************
* Internal types
******************************************************************************/
/******************************************************************************
* D4D External resources macros
*
*//*! @addtogroup doxd4d_extsrc_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that define the external custom strings functionality
* @param   D4DLOCALE_TranslateStr - pointer to user translate function
* @param   D4DLOCALE_GetTranslateStrLength - pointer to to user get translated text function
* @note    This macro MUST be declared if the D4D_EXTSRC_TEXT_ENABLE is enabled to support translated
*          string.
*******************************************************************************/
#define D4D_DECLARE_LOCALE_FUNCTIONS(D4DLOCALE_TranslateStr, D4DLOCALE_GetTranslateStrLength) \
  const D4DLOCALE_FUNCTIONS d4d_extsrcLocale = \
  { \
    D4DLOCALE_TranslateStr, /* Translate locale function pointer */ \
    D4DLOCALE_GetTranslateStrLength  /* Translate locale get str Length function pointer */ \
  };

/**************************************************************************/ /*!
* @brief   Macro that define the API to external resources
* @param   D4DFILE_fopen - pointer to open file user/system function
* @param   D4DFILE_fclose - pointer to close file user/system function
* @param   D4DFILE_read - pointer to read file user/system function
* @param   D4DFILE_write -  - pointer to write file user/system function
* @param   D4DFILE_fseek -  - pointer to seek file user/system function
* @param   D4DFILE_ftell -  - pointer to tell file user/system function
* @note    This macro MUST be declared if the external resources are used in project.
*******************************************************************************/
#define D4D_DECLARE_FILE_FUNCTIONS(D4DFILE_fopen, D4DFILE_fclose, D4DFILE_read, D4DFILE_write, D4DFILE_fseek, D4DFILE_ftell) \
  const D4DFILE_FUNCTIONS d4d_extsrcFile = \
  { \
    D4DFILE_fopen, /* D4D File - fopen */ \
    D4DFILE_fclose, /* D4D File - fclose */ \
    D4DFILE_read, /* D4D File - fread */ \
    D4DFILE_write, /* D4D File - fwrite */ \
    D4DFILE_fseek, /* D4D File - fseek */ \
    D4DFILE_ftell /* D4D File - ftell */ \
  };



#if D4D_ENDIAN == D4D_ENDIAN_LITTLE
  /*! @brief   Macro for handling endianism - read 8bit -  from Little Endian*/
  #define D4D_READ8L(x) (x)
  /*! @brief   Macro for handling endianism - read 16bit - from Little Endian*/
  #define D4D_READ16L(x) (x)
  /*! @brief   Macro for handling endianism - read 32bit - from Little Endian*/
  #define D4D_READ32L(x) (x)

  /*! @brief   Macro for handling endianism - read 8bit - from Big Endian*/
  #define D4D_READ8B(x) (x)
  /*! @brief   Macro for handling endianism - read 16bit - from Big Endian*/
  #define D4D_READ16B(x) D4D_SwapWord((x))
  /*! @brief   Macro for handling endianism - read 32bit - from Big Endian*/
  #define D4D_READ32B(x) D4D_SwapLong((x))

  /*! @brief   Macro for handling endianism - write 8bit -  from Little Endian*/
  #define D4D_WRITE8L(x) (x)
  /*! @brief   Macro for handling endianism - write 16bit -  from Little Endian*/
  #define D4D_WRITE16L(x) (x)
  /*! @brief   Macro for handling endianism - write 32bit -  from Little Endian*/
  #define D4D_WRITE32L(x) (x)

  /*! @brief   Macro for handling endianism - write 8bit -  from Big Endian*/
  #define D4D_WRITE8B(x) (x)
  /*! @brief   Macro for handling endianism - write 16bit -  from Big Endian*/
  #define D4D_WRITE16B(x) D4D_SwapWord((x))
  /*! @brief   Macro for handling endianism - write 32bit -  from Big Endian*/
  #define D4D_WRITE32B(x) D4D_SwapLong((x))


#else
  #define D4D_READ8L(x) (x)
  #define D4D_READ16L(x) D4D_SwapWord((x))
  #define D4D_READ32L(x) D4D_SwapLong((x))

  #define D4D_READ8B(x) (x)
  #define D4D_READ16B(x) (x)
  #define D4D_READ32B(x) (x)

  #define D4D_WRITE8L(x) (x)
  #define D4D_WRITE16L(x) D4D_SwapWord((x))
  #define D4D_WRITE32L(x) D4D_SwapLong((x))

  #define D4D_WRITE8B(x) (x)
  #define D4D_WRITE16B(x) (x)
  #define D4D_WRITE32B(x) (x)

#endif



/*! @} End of doxd4d_extsrc_macro                                             */

/******************************************************************************
* D4D External resources functions headers
*
*//*! @addtogroup doxd4d_extsrc_func
* @{
*******************************************************************************/
/*! @brief   Macro used as short cut to used File Open function defined by D4D_DECLARE_FILE_FUNCTIONS.*/
#define D4D_FileOpen    d4d_extsrcFile.D4DFILE_fopen
/*! @brief   Macro used as short cut to used File Close function defined by D4D_DECLARE_FILE_FUNCTIONS.*/
#define D4D_FileClose   d4d_extsrcFile.D4DFILE_fclose
/*! @brief   Macro used as short cut to used File Read function defined by D4D_DECLARE_FILE_FUNCTIONS.*/
#define D4D_FileRead    d4d_extsrcFile.D4DFILE_read
/*! @brief   Macro used as short cut to used File Write function defined by D4D_DECLARE_FILE_FUNCTIONS.*/
#define D4D_FileWrite   d4d_extsrcFile.D4DFILE_write
/*! @brief   Macro used as short cut to used File Seek function defined by D4D_DECLARE_FILE_FUNCTIONS.*/
#define D4D_FileSeek    d4d_extsrcFile.D4DFILE_fseek
/*! @brief   Macro used as short cut to used File Tell function defined by D4D_DECLARE_FILE_FUNCTIONS.*/
#define D4D_FileTell    d4d_extsrcFile.D4DFILE_ftell

/*! @brief   Mem Alloc eGUI definition. If not defined by user config any special user function, it used standard system function.*/
#ifndef D4D_MemAlloc
  #if defined(__MQX__) || defined(D4D_OS_MQX)
    #define D4D_MemAlloc(size)  _mem_alloc(size)
  #else
    #define D4D_MemAlloc(size)  malloc(size)
  #endif
#endif

/*! @brief   Mem Free eGUI definition. If not defined by user config any special user function, it used standard system function.*/
#ifndef D4D_MemFree
  #if defined(__MQX__) || defined(D4D_OS_MQX)
    #define D4D_MemFree(pMem)   _mem_free(pMem)
  #else
    #define D4D_MemFree(pMem)   free(pMem)
  #endif
#endif

/*! @brief   Mem Copy eGUI definition. If not defined by user config any special user function, it used standard system function.*/
#ifndef D4D_MemCopy
  #if defined(__MQX__) || defined(D4D_OS_MQX)
    #define D4D_MemCopy(pDst, pSrc, size)   _mem_copy(pSrc, pDst, size)
  #else
    #define D4D_MemCopy(pDst, pSrc, size)   memcpy(pDst, pSrc, size)
  #endif
#endif

/*! @} End of doxd4d_extsrc_func                                             */

/******************************************************************************
* Global variables
******************************************************************************/

extern const D4DLOCALE_FUNCTIONS d4d_extsrcLocale;
extern const D4DFILE_FUNCTIONS d4d_extsrcFile;

/******************************************************************************
* Global functions
******************************************************************************/
D4D_CHAR* D4D_GetFileExtension(D4D_CHAR* pFileName);
D4D_BOOL D4D_FileIsAbsolutePath(D4D_CHAR* pFileName);
int D4D_StrLen(register const D4D_CHAR *s);
D4D_CHAR* D4D_StrCopy(register D4D_CHAR *sd, register const D4D_CHAR *ss);
D4D_WCHAR* D4D_StrCopyAscii2Unicode(register D4D_WCHAR *sd, register const D4D_CHAR *ss);
D4D_CHAR* D4D_StrCopyUnicode2Ascii(register D4D_CHAR *sd, register const D4D_WCHAR *ss);
D4D_CHAR* D4D_StrCat(register D4D_CHAR *sd, register const D4D_CHAR *ss);
D4D_WCHAR* D4D_StrCatUnicode(register D4D_WCHAR *sd, register const D4D_WCHAR *ss);
int D4D_CompareStrings(const D4D_CHAR* s1, const D4D_CHAR* s2);
int D4D_CompareStringsUnicode(const D4D_WCHAR* s1, const D4D_WCHAR* s2);
void D4D_ToUpper(D4D_CHAR* s);
LWord D4D_SwapLong(LWord n);
Word D4D_SwapWord(Word n);

#ifdef D4D_UNICODE
  #define D4D_CompareStringsUniversal(s1, s2) D4D_CompareStringsUnicode(s1, s2)
#else
  #define D4D_CompareStringsUniversal(s1, s2) D4D_CompareStrings(s1, s2)
#endif

#endif /* __D4D_EXTSRC_H */
