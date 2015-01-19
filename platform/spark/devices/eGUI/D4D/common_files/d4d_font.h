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
* @file      d4d_font.h
*
* @author     Petr Gargulak
*
* @version   0.0.35.0
*
* @date      Oct-2-2013
*
* @brief     D4D Driver fonts header file
*
*******************************************************************************/

#ifndef __D4D_FONT_H
#define __D4D_FONT_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* D4D FONTS setting  constants
*
*//*! @addtogroup doxd4d_font_const
* @{
*******************************************************************************/

/*! @brief This macro is used to enable external fonts resource support.
           If not defined, the external font resource support is disabled as a default.*/
#ifndef D4D_FNT_EXTSRC_SUPPORT
  #define D4D_FNT_EXTSRC_SUPPORT D4D_FALSE
#endif

/*! @} End of doxd4d_font_const                                               */

/******************************************************************************
* D4D FONTS types
*
*//*! @addtogroup doxd4d_font_type
* @{
*******************************************************************************/
/*!
  @defgroup doxd4d_font_type_underline D4D FONT Defines masks of underline properties
  This section specifies masks of underline properties.
  @ingroup doxd4d_font_type
*/

/**
 * @addtogroup doxd4d_font_type_underline
 * @{
 */
/*! @brief Font Underline property masks*/
#define D4D_FNT_PRTY_UNDERLINE_MASK              (0x03) ///< Underline Property mask
#define D4D_FNT_PRTY_UNDERLINE_NONE_MASK         (0x00) ///< None Underline Text Property mask
#define D4D_FNT_PRTY_UNDERLINE_LINE_MASK         (0x01) ///< One Line Underline Text Property mask
#define D4D_FNT_PRTY_UNDERLINE_DOT_MASK          (0x02) ///< Dot Line Underline Text Property mask
#define D4D_FNT_PRTY_UNDERLINE_RES_MASK          (0x03) ///< Reserved -  Underline Text Property mask
/* @} */

/*!
  @defgroup doxd4d_font_type_strikethrough D4D FONT Defines masks of strike through properties
  This section specifies masks of strike through properties.
  @ingroup doxd4d_font_type
*/

/**
 * @addtogroup doxd4d_font_type_strikethrough
 * @{
 */
/*! @brief Font Strike through property masks*/
#define D4D_FNT_PRTY_STRIKETHROUGH_MASK          (0x0C) ///< Strike Through Property mask
#define D4D_FNT_PRTY_STRIKETHROUGH_NONE_MASK     (0x00) ///< None Strike Through Text Property mask
#define D4D_FNT_PRTY_STRIKETHROUGH_SINGLE_MASK   (0x04) ///< One Line Strike Through Text Property mask
#define D4D_FNT_PRTY_STRIKETHROUGH_DOUBLE_MASK   (0x08) ///< Two Lines Strike Through Text Property mask
#define D4D_FNT_PRTY_STRIKETHROUGH_TRIPLE_MASK   (0x0C) ///< Three Lines Strike Through Text Property mask

#define D4D_FNT_PRTY_STRIKETHROUGH_SHIFT         (0x02) ///< Strike through property bits shift
/* @} */

#define D4D_FNT_PRTY_TRANSPARENT_MASK            (0x10)
#define D4D_FNT_PRTY_TRANSPARENT_NO_MASK         (0x00)
#define D4D_FNT_PRTY_TRANSPARENT_YES_MASK        (0x10)

typedef Byte D4D_FONT_PROPERTIES;

/*! @} End of doxd4d_font_type                                               */

/******************************************************************************
* Internal types
******************************************************************************/
#define D4D_FONT_PACK_BITORDER 0x01
#define D4D_FONT_PACK_BITORDER_BIGEND 0
#define D4D_FONT_PACK_BITORDER_LITTLEEND 0x01

#define D4D_FONT_PACK_SCANBASED 0x02
#define D4D_FONT_PACK_SCANBASED_ROW 0
#define D4D_FONT_PACK_SCANBASED_COLUMN 0x02

#define D4D_FONT_PACK_SCANPREFERRED 0x04
#define D4D_FONT_PACK_SCANPREFERRED_ROW 0
#define D4D_FONT_PACK_SCANPREFERRED_COLUMN 0x04

#define D4D_FONT_PACK_COMPRESSED 0x08
#define D4D_FONT_PACK_COMPRESSED_OFF 0
#define D4D_FONT_PACK_COMPRESSED_ON 0x08

#define D4D_FONT_PACK_DATA_LEN 0x030
#define D4D_FONT_PACK_DATA_LEN_8B 0x00
#define D4D_FONT_PACK_DATA_LEN_16B 0x10
#define D4D_FONT_PACK_DATA_LEN_32B 0x20

#define D4D_FONT_FLAGS_IX_STYLE 0x01
#define D4D_FONT_FLAGS_IX_STYLE_LINEAR 0x00
#define D4D_FONT_FLAGS_IX_STYLE_NONLINEAR 0x01

#define D4D_FONT_FLAGS_IX_TYPE 0x02
#define D4D_FONT_FLAGS_IX_TYPE_LOOKUP 0x00
#define D4D_FONT_FLAGS_IX_TYPE_MAP 0x02

#define D4D_FONT_FLAGS_FNT_WIDTH 0x04
#define D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE 0x00
#define D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL 0x04



typedef Byte D4D_FONT_PACK;
typedef Byte D4D_FONT_FLAGS;

#ifdef D4D_UNICODE
  typedef D4D_TCHAR D4D_FONT_IX;
  typedef Word D4D_FONT_REV;
#else
  typedef Byte D4D_FONT_IX;
  typedef Byte D4D_FONT_REV;
#endif

typedef Word D4D_FONT_OFFSET;
typedef Byte D4D_FONT_DATA;
typedef Byte D4D_FONT;


typedef Byte D4D_FONT_SIZE;
typedef Word D4D_FONT_DSIZE;

#pragma pack (push)
#pragma pack (1)

  typedef struct
  {
    D4D_FONT_SIZE width;
    D4D_FONT_SIZE height;
  }D4D_FONT_SIZES;

  typedef struct D4D_FONT_DESCRIPTOR_S
  {
    D4D_FONT_REV revision;       //1 Font descriptor version number
    D4D_FONT_FLAGS flags;        //1 linear / non linear , proporcional or not,
    D4D_FONT_IX startChar;       //2/1 start char of used table
    D4D_FONT_IX charCnt;         //2/1 end char of used table
    D4D_FONT_IX charDefault;     //2/1 index of char that will be printed instead of
    D4D_FONT_SIZE charSize;      //1 Font size (size of font loaded in PC)
    D4D_FONT_DSIZE charBmpSize;   //2 Size of font bitmpap for non proportional fonts
    D4D_FONT_SIZE charBaseLine;  //1 offset from Y0 coordinate to baseline
    D4D_FONT_SIZES charFullSize; //2 size of biggest char x/y
    D4D_FONT_PACK pack;          //1 packing condition of individual bitmaps 15
    const D4D_FONT_IX *pIxTable;       // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    const D4D_FONT_OFFSET *pOffTable;  // Offset table - used when proporcial font is set in flags
    const D4D_FONT_SIZE *pSizeTable;   // Size table - used when proporcial font is set in flags
    const D4D_FONT_DATA *pFontData; // bitmap/font data array
    const struct D4D_FONT_DESCRIPTOR_S* pNext;  // pointer on next page for this font (used in unicode)
  }D4D_FONT_DESCRIPTOR;

#pragma pack (pop)

typedef struct
{
  D4D_FONT  ix_font;
  D4D_FONT_DESCRIPTOR* pFontDescriptor;
  D4D_FONT_SIZES scale;           // the scale of the font in both axes values must be > 0
  D4D_FONT_SIZES charSpacing;     // increment for finest cell in x/y to provide line/char spacing
  D4D_CHAR* fileName;
}D4D_FONT_TYPE;

typedef struct{
  D4D_COOR x;
  D4D_COOR y;
  D4D_TCHAR* pText;
  D4D_FONT_TYPE* pFontType;
  D4D_TAB* pTab;
  D4D_COLOR colorText;
  D4D_COLOR colorBack;
  D4D_FONT_PROPERTIES properties;
  D4D_INDEX textLength;
  D4D_INDEX textOffset;
  D4D_COOR maxWidth;
}D4D_PRINT_DESC;

/******************************************************************************
* Macros
******************************************************************************/

#define D4D_DECLARE_USR_FONT_TABLE_BEGIN(name)  const D4D_FONT_TYPE name[] = {

#define D4D_DECLARE_FONT_TABLE_BEGIN  D4D_DECLARE_USR_FONT_TABLE_BEGIN(d4d_FontTable)



#define D4D_DECLARE_FONT(fontId, font_descriptor, xScale, yScale, charSpace, lineSpace) \
{fontId, (D4D_FONT_DESCRIPTOR*)&(font_descriptor), { xScale, yScale }, { charSpace, lineSpace }, NULL}, // font info will be replaced by font descriptor

#define D4D_DECLARE_FONT_FILE(fontId, fileName, xScale, yScale, charSpace, lineSpace) \
{fontId, NULL, { xScale, yScale }, { charSpace, lineSpace }, fileName}, // font info will be replaced by font descriptor


#define D4D_DECLARE_FONT_TABLE_END {0, NULL, {0, 0}, {0, 0}, NULL } };

/******************************************************************************
* Global variables
******************************************************************************/

extern const D4D_FONT_TYPE d4d_FontTable[];

/******************************************************************************
* Global functions
******************************************************************************/

void D4D_SetFontTable(D4D_FONT_TYPE* pFontTable);
D4D_FONT_TYPE* D4D_GetFontTable(void);

D4D_FONT_TYPE* D4D_GetFont(D4D_FONT ix);
D4D_FONT_SIZES D4D_GetFontSize(D4D_FONT ix);
D4D_FONT_SIZE D4D_GetFontHeight(D4D_FONT ix);
D4D_FONT_SIZE D4D_GetFontWidth(D4D_FONT ix);
D4D_FONT_SIZE D4D_GetCharWidth(D4D_FONT ix, D4D_TCHAR ch);
D4D_TCHAR D4D_GetChar(D4D_FONT_TYPE* pFontType, D4D_FONT_IX ix);
D4D_COOR D4D_GetNextTab(D4D_TAB* pTab, D4D_COOR pos);



#if D4D_EXTSRC_FILE_ENABLE != D4D_FALSE
  D4D_BOOL D4D_ExtFntInit(void);
  void D4D_ExtFntSetWorkPath(D4D_CHAR* pPath);
  D4D_CHAR* D4D_ExtFntGetWorkPath(void);
  D4D_FILEPTR D4D_OpenFntFile(D4D_CHAR* pFileName);
#endif

#endif  /* __D4D_FONT_H */


