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
*****************************************************************************
*
* $FileName: fonts.c$
* $Version : 1.0.3.0$
* $Date    : Jan-15-2014$
*
* Comments:
*
* Author: eGUI Converter utility
*
* This file contains the generated fonts data by eGUI converter utility
*
*END************************************************************************/

#include "d4d.h"
#include "fonts.h"

// Font Table declaration 

D4D_DECLARE_FONT_TABLE_BEGIN                                          
  D4D_DECLARE_FONT(FONT_SYSTEM, d4dfnt_CourierNew12_desc, 1, 1, 0, 0)
  D4D_DECLARE_FONT_FILE(FONT_SYSTEM_SMALL, "CourierNew8.d4l", 1, 1, 0, 0)
  D4D_DECLARE_FONT(FONT_SYSTEM_BIG, d4dfnt_CourierNew12_desc, 2, 2, 0, 0)
D4D_DECLARE_FONT_TABLE_END


D4D_DECLARE_USR_FONT_TABLE_BEGIN(fontTable_Arial)
  D4D_DECLARE_FONT(FONT_SYSTEM, d4dfnt_Arial12_desc, 1, 1, 0, 0)
  D4D_DECLARE_FONT_FILE(FONT_SYSTEM_SMALL, "Arial8.d4l", 1, 1, 0, 0)
  D4D_DECLARE_FONT(FONT_SYSTEM_BIG, d4dfnt_Arial12_desc, 2, 2, 0, 0)
D4D_DECLARE_FONT_TABLE_END

const Byte d4dfnt_CourierNew12_data_table[] = { 

/* character 0x0020 (' '): (width = 10, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0021 ('!'): (width = 10, offset = 23) */
0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0x06, 0x01, 0x80, 0x60, 0x18, 0x06, 0x01, 0x80, 0x00, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0022 ('"'): (width = 10, offset = 46) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x8E, 0xE1, 0x10, 0x44, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0023 ('#'): (width = 10, offset = 69) */
0x00, 0x00, 0x00, 0x00, 0x6C, 0x1B, 0x06, 0xC1, 0xB1, 0xFE, 0x36, 0x1F, 0xE3, 0x60, 0xD8, 0x36,
0x0D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0024 ('$'): (width = 10, offset = 92) */
0x00, 0x00, 0x00, 0x80, 0xFC, 0x63, 0x18, 0xC7, 0x00, 0xF0, 0x1E, 0x01, 0xC6, 0x31, 0x8C, 0x7E,
0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0025 ('%'): (width = 10, offset = 115) */
0x00, 0x00, 0x00, 0x00, 0xC0, 0x48, 0x12, 0x03, 0x18, 0x3C, 0x3C, 0x18, 0xC0, 0x48, 0x12, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0026 ('&'): (width = 10, offset = 138) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x0C, 0x03, 0x00, 0xC0, 0x18, 0x0E, 0xC6, 0xE1, 0x98, 0x3B,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0027 ('''): (width = 10, offset = 161) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x03, 0x80, 0x40, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0028 ('('): (width = 10, offset = 184) */
0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0x03, 0x01, 0xC0, 0x60, 0x18, 0x06, 0x01, 0x80, 0x70, 0x0C,
0x01, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0029 (')'): (width = 10, offset = 207) */
0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x06, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x18,
0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002A ('*'): (width = 10, offset = 230) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x1F, 0xE7, 0xF8, 0x78, 0x3F, 0x0C, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002B ('+'): (width = 10, offset = 253) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x20, 0x7F, 0x02, 0x00, 0x80, 0x20, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002C (','): (width = 10, offset = 276) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08,
0x06, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 

/* character 0x002D ('-'): (width = 10, offset = 299) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002E ('.'): (width = 10, offset = 322) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002F ('/'): (width = 10, offset = 345) */
0x00, 0x00, 0x00, 0x18, 0x06, 0x03, 0x00, 0xC0, 0x60, 0x18, 0x0C, 0x06, 0x01, 0x80, 0xC0, 0x30,
0x18, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0030 ('0'): (width = 10, offset = 368) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x36, 0x18, 0xC6, 0x31, 0x8C, 0x63, 0x18, 0xC6, 0x30, 0xD8, 0x1C,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0031 ('1'): (width = 10, offset = 391) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x7C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x7F,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0032 ('2'): (width = 10, offset = 414) */
0x00, 0x00, 0x00, 0x00, 0x78, 0x33, 0x18, 0xC6, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x80, 0x7F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0033 ('3'): (width = 10, offset = 437) */
0x00, 0x00, 0x00, 0x01, 0xF8, 0xC3, 0x00, 0xC0, 0x60, 0xF8, 0x07, 0x00, 0xC0, 0x33, 0x0C, 0x7E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0034 ('4'): (width = 10, offset = 460) */
0x00, 0x00, 0x00, 0x00, 0x38, 0x0E, 0x07, 0x81, 0x60, 0xD8, 0x26, 0x19, 0x87, 0xF0, 0x18, 0x1F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0035 ('5'): (width = 10, offset = 483) */
0x00, 0x00, 0x00, 0x00, 0xFC, 0x30, 0x0C, 0x03, 0x00, 0xF8, 0x23, 0x00, 0xC0, 0x31, 0x0C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0036 ('6'): (width = 10, offset = 506) */
0x00, 0x00, 0x00, 0x00, 0x3C, 0x38, 0x0C, 0x06, 0x01, 0xB8, 0x73, 0x18, 0xC6, 0x30, 0xCC, 0x1E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0037 ('7'): (width = 10, offset = 529) */
0x00, 0x00, 0x00, 0x03, 0xF8, 0x86, 0x01, 0x80, 0xC0, 0x30, 0x0C, 0x03, 0x01, 0x80, 0x60, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0038 ('8'): (width = 10, offset = 552) */
0x00, 0x00, 0x00, 0x00, 0xF8, 0x63, 0x18, 0xC6, 0x30, 0xF8, 0x63, 0x18, 0xC6, 0x31, 0x8C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0039 ('9'): (width = 10, offset = 575) */
0x00, 0x00, 0x00, 0x00, 0xF0, 0x66, 0x18, 0xC6, 0x31, 0x9C, 0x3B, 0x00, 0xC0, 0x60, 0x38, 0x78,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003A (':'): (width = 10, offset = 598) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003B (';'): (width = 10, offset = 621) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08,
0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003C ('<'): (width = 10, offset = 644) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0x80, 0x80, 0xC0, 0xC0, 0x0C, 0x00, 0x80, 0x18, 0x01,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003D ('='): (width = 10, offset = 667) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003E ('>'): (width = 10, offset = 690) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0C, 0x00, 0x80, 0x18, 0x01, 0x81, 0x80, 0x80, 0xC0, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003F ('?'): (width = 10, offset = 713) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x18, 0xC6, 0x30, 0x0C, 0x0E, 0x06, 0x01, 0x80, 0x00, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0040 ('@'): (width = 10, offset = 736) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x22, 0x10, 0x84, 0x21, 0x38, 0x52, 0x14, 0x84, 0xE1, 0x00, 0x22,
0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0041 ('A'): (width = 10, offset = 759) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x07, 0x81, 0x20, 0xCC, 0x33, 0x0F, 0xC6, 0x19, 0x86, 0xF3,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0042 ('B'): (width = 10, offset = 782) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x18, 0xC6, 0x31, 0x8C, 0x7E, 0x18, 0xC6, 0x31, 0x8C, 0xFE,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0043 ('C'): (width = 10, offset = 805) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x98, 0x6C, 0x0B, 0x00, 0xC0, 0x30, 0x0C, 0x09, 0x84, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0044 ('D'): (width = 10, offset = 828) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x18, 0xC6, 0x19, 0x86, 0x61, 0x98, 0x66, 0x19, 0x8C, 0xFE,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0045 ('E'): (width = 10, offset = 851) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x46, 0x11, 0x90, 0x7C, 0x19, 0x06, 0x11, 0x84, 0xFF,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0046 ('F'): (width = 10, offset = 874) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x98, 0x26, 0x09, 0x90, 0x7C, 0x19, 0x06, 0x01, 0x80, 0xF8,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0047 ('G'): (width = 10, offset = 897) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x18, 0xCC, 0x13, 0x00, 0xC0, 0x33, 0xEC, 0x31, 0x8C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0048 ('H'): (width = 10, offset = 920) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x98, 0xC6, 0x31, 0x8C, 0x7F, 0x18, 0xC6, 0x31, 0x8C, 0xF7,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0049 ('I'): (width = 10, offset = 943) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x83, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x7F,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004A ('J'): (width = 10, offset = 966) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x81, 0x80, 0x60, 0x18, 0x06, 0x31, 0x8C, 0x63, 0x18, 0x7C,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004B ('K'): (width = 10, offset = 989) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x98, 0xC6, 0x61, 0xB0, 0x78, 0x1F, 0x06, 0x61, 0x8C, 0xF3,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004C ('L'): (width = 10, offset = 1012) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x23, 0x08, 0xC2, 0xFF,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004D ('M'): (width = 10, offset = 1035) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xC1, 0xF0, 0x6E, 0x3B, 0xDE, 0xD5, 0xB7, 0x6C, 0x9B, 0x06, 0xF7,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004E ('N'): (width = 10, offset = 1058) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x98, 0xC7, 0x31, 0xEC, 0x6B, 0x1B, 0xC6, 0x71, 0x8C, 0xF3,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004F ('O'): (width = 10, offset = 1081) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x18, 0xCC, 0x1B, 0x06, 0xC1, 0xB0, 0x6C, 0x19, 0x8C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0050 ('P'): (width = 10, offset = 1104) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x18, 0xC6, 0x31, 0x8C, 0x63, 0x1F, 0x86, 0x01, 0x80, 0xFC,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0051 ('Q'): (width = 10, offset = 1127) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x18, 0xCC, 0x1B, 0x06, 0xC1, 0xB0, 0x6C, 0x19, 0x8C, 0x3E,
0x06, 0x63, 0xF0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0052 ('R'): (width = 10, offset = 1150) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x18, 0xC6, 0x31, 0x8C, 0x7C, 0x19, 0x86, 0x31, 0x8C, 0xF9,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0053 ('S'): (width = 10, offset = 1173) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x18, 0xC6, 0x31, 0xC0, 0x3E, 0x01, 0xC6, 0x31, 0x8C, 0x7E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0054 ('T'): (width = 10, offset = 1196) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x26, 0x49, 0x92, 0x64, 0x18, 0x06, 0x01, 0x80, 0x60, 0x7E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0055 ('U'): (width = 10, offset = 1219) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x98, 0xC6, 0x31, 0x8C, 0x63, 0x18, 0xC6, 0x31, 0x8C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0056 ('V'): (width = 10, offset = 1242) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x98, 0xC6, 0x30, 0xD8, 0x36, 0x0D, 0x81, 0x40, 0x70, 0x1C,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0057 ('W'): (width = 10, offset = 1265) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0xF0, 0x6C, 0x9B, 0x76, 0xDD, 0x95, 0x47, 0x71, 0xDC, 0x63,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0058 ('X'): (width = 10, offset = 1288) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x98, 0xC3, 0x60, 0x70, 0x1C, 0x07, 0x03, 0x61, 0x8C, 0xF7,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0059 ('Y'): (width = 10, offset = 1311) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xD8, 0x63, 0x30, 0x78, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x3F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005A ('Z'): (width = 10, offset = 1334) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x10, 0xC4, 0x60, 0x30, 0x08, 0x06, 0x03, 0x11, 0x84, 0x7F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005B ('['): (width = 10, offset = 1357) */
0x00, 0x00, 0x00, 0x00, 0x3C, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C,
0x03, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005C ('\'): (width = 10, offset = 1380) */
0x00, 0x00, 0x06, 0x01, 0x80, 0x30, 0x0C, 0x01, 0x80, 0x60, 0x0C, 0x01, 0x80, 0x60, 0x0C, 0x03,
0x00, 0x60, 0x18, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005D (']'): (width = 10, offset = 1403) */
0x00, 0x00, 0x00, 0x00, 0xF0, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C,
0x03, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005E ('^'): (width = 10, offset = 1426) */
0x00, 0x00, 0x00, 0x80, 0x50, 0x14, 0x08, 0x84, 0x11, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005F ('_'): (width = 10, offset = 1449) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 

/* character 0x0060 ('`'): (width = 10, offset = 1472) */
0x00, 0x00, 0x01, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0061 ('a'): (width = 10, offset = 1495) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x0C, 0x03, 0x0F, 0xC6, 0x31, 0x9C, 0x3B,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0062 ('b'): (width = 10, offset = 1518) */
0x00, 0x00, 0x00, 0x03, 0x80, 0x60, 0x18, 0x06, 0xE1, 0xCC, 0x61, 0x98, 0x66, 0x19, 0xCC, 0xEE,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0063 ('c'): (width = 10, offset = 1541) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xD1, 0x8C, 0xC1, 0x30, 0x0C, 0x11, 0x8C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0064 ('d'): (width = 10, offset = 1564) */
0x00, 0x00, 0x00, 0x00, 0x1C, 0x03, 0x00, 0xC3, 0xB1, 0x9C, 0xC3, 0x30, 0xCC, 0x31, 0x9C, 0x3B,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0065 ('e'): (width = 10, offset = 1587) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE1, 0x8C, 0xC1, 0xBF, 0xEC, 0x01, 0x86, 0x3F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0066 ('f'): (width = 10, offset = 1610) */
0x00, 0x00, 0x00, 0x00, 0x3F, 0x18, 0x06, 0x07, 0xF0, 0x60, 0x18, 0x06, 0x01, 0x80, 0x60, 0x7F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0067 ('g'): (width = 10, offset = 1633) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xB9, 0x9C, 0xC3, 0x30, 0xCC, 0x31, 0x9C, 0x3B,
0x00, 0xC0, 0x30, 0xF8, 0x00, 0x00, 0x00, 

/* character 0x0068 ('h'): (width = 10, offset = 1656) */
0x00, 0x00, 0x00, 0x03, 0x80, 0x60, 0x18, 0x06, 0xE1, 0xCC, 0x63, 0x18, 0xC6, 0x31, 0x8C, 0xF7,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0069 ('i'): (width = 10, offset = 1679) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x03, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x7F,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006A ('j'): (width = 10, offset = 1702) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x07, 0xE0, 0x18, 0x06, 0x01, 0x80, 0x60, 0x18, 0x06,
0x01, 0x80, 0x61, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x006B ('k'): (width = 10, offset = 1725) */
0x00, 0x00, 0x00, 0x03, 0x80, 0x60, 0x18, 0x06, 0xF1, 0xB0, 0x78, 0x1E, 0x06, 0xC1, 0x98, 0xEF,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006C ('l'): (width = 10, offset = 1748) */
0x00, 0x00, 0x00, 0x00, 0xF0, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x7F,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006D ('m'): (width = 10, offset = 1771) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF1, 0xB6, 0x6D, 0x9B, 0x66, 0xD9, 0xB6, 0xED,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006E ('n'): (width = 10, offset = 1794) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xE1, 0xCC, 0x63, 0x18, 0xC6, 0x31, 0x8C, 0xF7,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006F ('o'): (width = 10, offset = 1817) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE1, 0x8C, 0xC1, 0xB0, 0x6C, 0x19, 0x8C, 0x3E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0070 ('p'): (width = 10, offset = 1840) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xE1, 0xCC, 0x61, 0x98, 0x66, 0x19, 0xCC, 0x6E,
0x18, 0x06, 0x03, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0071 ('q'): (width = 10, offset = 1863) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xB9, 0x9C, 0xC3, 0x30, 0xCC, 0x31, 0x9C, 0x3B,
0x00, 0xC0, 0x30, 0x3E, 0x00, 0x00, 0x00, 

/* character 0x0072 ('r'): (width = 10, offset = 1886) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x70, 0xE6, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0xFE,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0073 ('s'): (width = 10, offset = 1909) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF1, 0x8C, 0x78, 0x0F, 0x80, 0x71, 0x8C, 0x7E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0074 ('t'): (width = 10, offset = 1932) */
0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x0C, 0x0F, 0xE0, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC4, 0x1E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0075 ('u'): (width = 10, offset = 1955) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x71, 0x8C, 0x63, 0x18, 0xC6, 0x31, 0x9C, 0x3B,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0076 ('v'): (width = 10, offset = 1978) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x79, 0x8C, 0x63, 0x0D, 0x83, 0x60, 0x70, 0x1C,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0077 ('w'): (width = 10, offset = 2001) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x3F, 0x06, 0xC9, 0xB7, 0x67, 0x71, 0xDC, 0x63,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0078 ('x'): (width = 10, offset = 2024) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x78, 0xD8, 0x1C, 0x07, 0x01, 0xC0, 0xD8, 0xF7,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0079 ('y'): (width = 10, offset = 2047) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x3D, 0x86, 0x33, 0x0C, 0xC1, 0x60, 0x78, 0x0C,
0x03, 0x01, 0x81, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x007A ('z'): (width = 10, offset = 2070) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF1, 0x0C, 0x06, 0x07, 0x03, 0x01, 0x84, 0x7F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007B ('{'): (width = 10, offset = 2093) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x06, 0x01, 0x80, 0x60, 0x18, 0x0C, 0x01, 0x80, 0x60, 0x18,
0x06, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007C ('|'): (width = 10, offset = 2116) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C,
0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007D ('}'): (width = 10, offset = 2139) */
0x00, 0x00, 0x00, 0x00, 0x60, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x01, 0x80, 0xC0, 0x30, 0x0C,
0x03, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007E ('~'): (width = 10, offset = 2162) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x49, 0x01, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007F (''): (width = 5, offset = 2185) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_OFFSET d4dfnt_CourierNew12_offset_table[] = { 

/*       offset       char      hexcode     */
/*       ======      =====      =======     */
            0,    /*   ' '        0020       */
           23,    /*   '!'        0021       */
           46,    /*   '"'        0022       */
           69,    /*   '#'        0023       */
           92,    /*   '$'        0024       */
          115,    /*   '%'        0025       */
          138,    /*   '&'        0026       */
          161,    /*   '''        0027       */
          184,    /*   '('        0028       */
          207,    /*   ')'        0029       */
          230,    /*   '*'        002A       */
          253,    /*   '+'        002B       */
          276,    /*   ','        002C       */
          299,    /*   '-'        002D       */
          322,    /*   '.'        002E       */
          345,    /*   '/'        002F       */
          368,    /*   '0'        0030       */
          391,    /*   '1'        0031       */
          414,    /*   '2'        0032       */
          437,    /*   '3'        0033       */
          460,    /*   '4'        0034       */
          483,    /*   '5'        0035       */
          506,    /*   '6'        0036       */
          529,    /*   '7'        0037       */
          552,    /*   '8'        0038       */
          575,    /*   '9'        0039       */
          598,    /*   ':'        003A       */
          621,    /*   ';'        003B       */
          644,    /*   '<'        003C       */
          667,    /*   '='        003D       */
          690,    /*   '>'        003E       */
          713,    /*   '?'        003F       */
          736,    /*   '@'        0040       */
          759,    /*   'A'        0041       */
          782,    /*   'B'        0042       */
          805,    /*   'C'        0043       */
          828,    /*   'D'        0044       */
          851,    /*   'E'        0045       */
          874,    /*   'F'        0046       */
          897,    /*   'G'        0047       */
          920,    /*   'H'        0048       */
          943,    /*   'I'        0049       */
          966,    /*   'J'        004A       */
          989,    /*   'K'        004B       */
         1012,    /*   'L'        004C       */
         1035,    /*   'M'        004D       */
         1058,    /*   'N'        004E       */
         1081,    /*   'O'        004F       */
         1104,    /*   'P'        0050       */
         1127,    /*   'Q'        0051       */
         1150,    /*   'R'        0052       */
         1173,    /*   'S'        0053       */
         1196,    /*   'T'        0054       */
         1219,    /*   'U'        0055       */
         1242,    /*   'V'        0056       */
         1265,    /*   'W'        0057       */
         1288,    /*   'X'        0058       */
         1311,    /*   'Y'        0059       */
         1334,    /*   'Z'        005A       */
         1357,    /*   '['        005B       */
         1380,    /*   '\'        005C       */
         1403,    /*   ']'        005D       */
         1426,    /*   '^'        005E       */
         1449,    /*   '_'        005F       */
         1472,    /*   '`'        0060       */
         1495,    /*   'a'        0061       */
         1518,    /*   'b'        0062       */
         1541,    /*   'c'        0063       */
         1564,    /*   'd'        0064       */
         1587,    /*   'e'        0065       */
         1610,    /*   'f'        0066       */
         1633,    /*   'g'        0067       */
         1656,    /*   'h'        0068       */
         1679,    /*   'i'        0069       */
         1702,    /*   'j'        006A       */
         1725,    /*   'k'        006B       */
         1748,    /*   'l'        006C       */
         1771,    /*   'm'        006D       */
         1794,    /*   'n'        006E       */
         1817,    /*   'o'        006F       */
         1840,    /*   'p'        0070       */
         1863,    /*   'q'        0071       */
         1886,    /*   'r'        0072       */
         1909,    /*   's'        0073       */
         1932,    /*   't'        0074       */
         1955,    /*   'u'        0075       */
         1978,    /*   'v'        0076       */
         2001,    /*   'w'        0077       */
         2024,    /*   'x'        0078       */
         2047,    /*   'y'        0079       */
         2070,    /*   'z'        007A       */
         2093,    /*   '{'        007B       */
         2116,    /*   '|'        007C       */
         2139,    /*   '}'        007D       */
         2162,    /*   '~'        007E       */
         2185    /*   ''        007F       */
};

const D4D_FONT_SIZE d4dfnt_CourierNew12_width_table[] = { 

/*       width        char      hexcode     */
/*       ======      =====      =======     */
          10,    /*   ' '        0020       */
          10,    /*   '!'        0021       */
          10,    /*   '"'        0022       */
          10,    /*   '#'        0023       */
          10,    /*   '$'        0024       */
          10,    /*   '%'        0025       */
          10,    /*   '&'        0026       */
          10,    /*   '''        0027       */
          10,    /*   '('        0028       */
          10,    /*   ')'        0029       */
          10,    /*   '*'        002A       */
          10,    /*   '+'        002B       */
          10,    /*   ','        002C       */
          10,    /*   '-'        002D       */
          10,    /*   '.'        002E       */
          10,    /*   '/'        002F       */
          10,    /*   '0'        0030       */
          10,    /*   '1'        0031       */
          10,    /*   '2'        0032       */
          10,    /*   '3'        0033       */
          10,    /*   '4'        0034       */
          10,    /*   '5'        0035       */
          10,    /*   '6'        0036       */
          10,    /*   '7'        0037       */
          10,    /*   '8'        0038       */
          10,    /*   '9'        0039       */
          10,    /*   ':'        003A       */
          10,    /*   ';'        003B       */
          10,    /*   '<'        003C       */
          10,    /*   '='        003D       */
          10,    /*   '>'        003E       */
          10,    /*   '?'        003F       */
          10,    /*   '@'        0040       */
          10,    /*   'A'        0041       */
          10,    /*   'B'        0042       */
          10,    /*   'C'        0043       */
          10,    /*   'D'        0044       */
          10,    /*   'E'        0045       */
          10,    /*   'F'        0046       */
          10,    /*   'G'        0047       */
          10,    /*   'H'        0048       */
          10,    /*   'I'        0049       */
          10,    /*   'J'        004A       */
          10,    /*   'K'        004B       */
          10,    /*   'L'        004C       */
          10,    /*   'M'        004D       */
          10,    /*   'N'        004E       */
          10,    /*   'O'        004F       */
          10,    /*   'P'        0050       */
          10,    /*   'Q'        0051       */
          10,    /*   'R'        0052       */
          10,    /*   'S'        0053       */
          10,    /*   'T'        0054       */
          10,    /*   'U'        0055       */
          10,    /*   'V'        0056       */
          10,    /*   'W'        0057       */
          10,    /*   'X'        0058       */
          10,    /*   'Y'        0059       */
          10,    /*   'Z'        005A       */
          10,    /*   '['        005B       */
          10,    /*   '\'        005C       */
          10,    /*   ']'        005D       */
          10,    /*   '^'        005E       */
          10,    /*   '_'        005F       */
          10,    /*   '`'        0060       */
          10,    /*   'a'        0061       */
          10,    /*   'b'        0062       */
          10,    /*   'c'        0063       */
          10,    /*   'd'        0064       */
          10,    /*   'e'        0065       */
          10,    /*   'f'        0066       */
          10,    /*   'g'        0067       */
          10,    /*   'h'        0068       */
          10,    /*   'i'        0069       */
          10,    /*   'j'        006A       */
          10,    /*   'k'        006B       */
          10,    /*   'l'        006C       */
          10,    /*   'm'        006D       */
          10,    /*   'n'        006E       */
          10,    /*   'o'        006F       */
          10,    /*   'p'        0070       */
          10,    /*   'q'        0071       */
          10,    /*   'r'        0072       */
          10,    /*   's'        0073       */
          10,    /*   't'        0074       */
          10,    /*   'u'        0075       */
          10,    /*   'v'        0076       */
          10,    /*   'w'        0077       */
          10,    /*   'x'        0078       */
          10,    /*   'y'        0079       */
          10,    /*   'z'        007A       */
          10,    /*   '{'        007B       */
          10,    /*   '|'        007C       */
          10,    /*   '}'        007D       */
          10,    /*   '~'        007E       */
           5     /*   ''        007F       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_32_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL,   // linear / nonlinear , proporcional or not
    ' ',     // start char of used table
    96,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    23,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {10, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    d4dfnt_CourierNew12_offset_table,     // Offset table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_width_table,        // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_32_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_32_data_table[] = { 

/* character 0x201C (' '): (width = 10, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x08, 0x83, 0x30, 0x44, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x201D (' '): (width = 10, offset = 23) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x08, 0x86, 0x61, 0x10, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_48_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_32_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x201C,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    23,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {10, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_32_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_48_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_48_data_table[] = { 

/* character 0x3002 (' '): (width = 11, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C,
0x06, 0xC0, 0xD8, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_78_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_48_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x3002,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    25,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {11, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_48_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_78_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_78_data_table[] = { 

/* character 0x4E0D (' '): (width = 16, offset = 0) */
0x00, 0x30, 0x00, 0x60, 0x00, 0xC0, 0x01, 0x80, 0x03, 0xCC, 0x06, 0xC6, 0x0C, 0xC3, 0x38, 0xC1,
0xE0, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x4E2D (' '): (width = 16, offset = 36) */
0x00, 0xC0, 0x00, 0xC0, 0x7F, 0xFF, 0x60, 0xC1, 0x60, 0xC1, 0x60, 0xC1, 0x60, 0xC1, 0x60, 0xC1,
0x7F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x4E86 (' '): (width = 16, offset = 72) */
0x00, 0x06, 0x00, 0x0C, 0x00, 0x18, 0x00, 0x30, 0x00, 0xE0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x07, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x4EF6 (' '): (width = 16, offset = 108) */
0x19, 0x98, 0x19, 0x98, 0x33, 0xFF, 0x33, 0x18, 0x73, 0x18, 0xF6, 0x18, 0x30, 0x18, 0x37, 0xFF,
0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_78_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x4E0D,     /*   4E0D        19981            0       */
         0x4E2D,     /*   4E2D        20013            1       */
         0x4E86,     /*   4E86        20102            2       */
         0x4EF6     /*   4EF6        20214            3       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_79_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_78_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x4E0D,     // start char of used table
    4,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_78_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_78_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_79_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_79_data_table[] = { 

/* character 0x4F53 (' '): (width = 16, offset = 0) */
0x18, 0x30, 0x18, 0x30, 0x3F, 0xFF, 0x30, 0x30, 0x70, 0x78, 0xF0, 0x78, 0x30, 0xFC, 0x30, 0xFC,
0x31, 0xB6, 0x33, 0x33, 0x37, 0xFF, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x4F7F (' '): (width = 16, offset = 36) */
0x1F, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x37, 0xFF, 0x76, 0x31, 0x76, 0x31, 0xF6, 0x31, 0x37, 0xFF,
0x30, 0x30, 0x31, 0xB0, 0x30, 0xF0, 0x30, 0x60, 0x30, 0xF8, 0x33, 0x8E, 0x3E, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x4FDD (' '): (width = 16, offset = 72) */
0x1B, 0x03, 0x1B, 0x03, 0x33, 0x03, 0x33, 0x03, 0x73, 0xFF, 0xF0, 0x30, 0x30, 0x30, 0x3F, 0xFF,
0x30, 0x78, 0x30, 0xFC, 0x31, 0xB6, 0x33, 0x33, 0x3E, 0x31, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x4FE1 (' '): (width = 16, offset = 108) */
0x0C, 0x00, 0x0C, 0x00, 0x1B, 0xFF, 0x18, 0x00, 0x38, 0x00, 0x78, 0xFF, 0xD8, 0x00, 0x18, 0x00,
0x18, 0xFF, 0x18, 0x00, 0x18, 0x00, 0x18, 0xFF, 0x18, 0xC3, 0x18, 0xC3, 0x18, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_79_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x4F53,     /*   4F53        20307            0       */
         0x4F7F,     /*   4F7F        20351            1       */
         0x4FDD,     /*   4FDD        20445            2       */
         0x4FE1     /*   4FE1        20449            3       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_80_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_79_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x4F53,     // start char of used table
    4,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_79_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_79_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_80_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_80_data_table[] = { 

/* character 0x50CF (' '): (width = 16, offset = 0) */
0x19, 0x8C, 0x1B, 0x18, 0x37, 0xFF, 0x3F, 0x31, 0x73, 0x31, 0xF3, 0xFF, 0x30, 0x60, 0x31, 0xF3,
0x37, 0x3E, 0x30, 0x6C, 0x31, 0xDE, 0x37, 0x3F, 0x30, 0xED, 0x37, 0x8C, 0x30, 0x78, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_81_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_80_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x50CF,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_80_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_81_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_81_data_table[] = { 

/* character 0x514B (' '): (width = 16, offset = 0) */
0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE,
0x03, 0x30, 0x03, 0x30, 0x03, 0x30, 0x06, 0x30, 0x0C, 0x30, 0x38, 0x30, 0xE0, 0x1F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5173 (' '): (width = 16, offset = 36) */
0x07, 0x1C, 0x03, 0x18, 0x1F, 0xFF, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x3F, 0xFF, 0x00, 0xE0,
0x00, 0xE0, 0x00, 0xF0, 0x01, 0xF8, 0x03, 0x9E, 0x1F, 0x0F, 0x3C, 0x07, 0x30, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5185 (' '): (width = 16, offset = 72) */
0x00, 0xC0, 0x7F, 0xFF, 0x60, 0xC1, 0x60, 0xC1, 0x60, 0xC1, 0x61, 0xE1, 0x61, 0xB1, 0x63, 0x19,
0x66, 0x0D, 0x6C, 0x07, 0x78, 0x07, 0x60, 0x01, 0x60, 0x01, 0x60, 0x01, 0x60, 0x0F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_81_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x514B,     /*   514B        20811            0       */
         0x5173,     /*   5173        20851            1       */
         0x5185     /*   5185        20869            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_83_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_81_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x514B,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_81_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_81_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_83_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_83_data_table[] = { 

/* character 0x53D6 (' '): (width = 16, offset = 0) */
0x33, 0x00, 0x33, 0xFF, 0x33, 0x61, 0x3F, 0x61, 0x33, 0x61, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x33,
0x33, 0x1E, 0x33, 0x1E, 0x37, 0x0C, 0x3F, 0x1E, 0xF3, 0x33, 0x03, 0x61, 0x03, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_84_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_83_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x53D6,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_83_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_84_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_84_data_table[] = { 

/* character 0x54C7 (' '): (width = 16, offset = 0) */
0x00, 0x18, 0xFD, 0xFF, 0xCC, 0x18, 0xCC, 0x18, 0xCC, 0x18, 0xCF, 0xFF, 0xCC, 0x00, 0xCC, 0x18,
0xCC, 0x18, 0xFC, 0x18, 0x01, 0xFF, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x03, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_86_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_84_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x54C7,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_84_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_86_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_86_data_table[] = { 

/* character 0x56FE (' '): (width = 16, offset = 0) */
0x3F, 0xFF, 0x31, 0xC1, 0x31, 0x99, 0x33, 0xFD, 0x37, 0xB1, 0x3C, 0xF1, 0x30, 0x61, 0x30, 0xF9,
0x33, 0x9F, 0x3E, 0xE3, 0x30, 0x31, 0x31, 0xC1, 0x30, 0x71, 0x30, 0x01, 0x3F, 0xFF, 0x30, 0x01,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_87_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_86_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x56FE,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_86_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_87_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_87_data_table[] = { 

/* character 0x578B (' '): (width = 16, offset = 0) */
0x1B, 0x19, 0x1B, 0x19, 0x1B, 0x19, 0xFF, 0xF9, 0x1B, 0x19, 0x1B, 0x19, 0x33, 0x01, 0xE3, 0x07,
0x00, 0xC0, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_89_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_87_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x578B,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_87_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_89_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_89_data_table[] = { 

/* character 0x59CB (' '): (width = 16, offset = 0) */
0x30, 0x18, 0x30, 0x30, 0xFE, 0x33, 0x36, 0x61, 0x36, 0x6F, 0x67, 0xF8, 0x66, 0x00, 0x66, 0x00,
0xCC, 0x7F, 0xFC, 0x61, 0xDC, 0x61, 0x1E, 0x61, 0x33, 0x61, 0x60, 0x61, 0xC0, 0x7F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_91_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_89_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x59CB,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_89_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_91_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_91_data_table[] = { 

/* character 0x5B57 (' '): (width = 16, offset = 0) */
0x7F, 0xFF, 0x60, 0x01, 0x60, 0x01, 0x6F, 0xFD, 0x00, 0x18, 0x00, 0x30, 0x00, 0xE0, 0x00, 0xC0,
0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x07, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5B9A (' '): (width = 16, offset = 36) */
0x7F, 0xFF, 0x60, 0x01, 0x60, 0x01, 0x00, 0x00, 0x3F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x18, 0xC0,
0x18, 0xC0, 0x18, 0xFE, 0x18, 0xC0, 0x3C, 0xC0, 0x36, 0xC0, 0x63, 0xC0, 0xC0, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5BB9 (' '): (width = 16, offset = 72) */
0x7F, 0xFF, 0x60, 0x01, 0x66, 0x19, 0x0C, 0xCC, 0x19, 0xE6, 0x73, 0x33, 0x06, 0x18, 0x0C, 0x0C,
0x38, 0x07, 0xEF, 0xFD, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5BBD (' '): (width = 16, offset = 108) */
0x30, 0xC0, 0x3F, 0xFF, 0x63, 0x99, 0x7F, 0xFF, 0x03, 0x30, 0x18, 0x06, 0x0F, 0xFF, 0x0C, 0xC6,
0x0C, 0xE6, 0x0C, 0xC6, 0x0C, 0xF6, 0x0D, 0xE6, 0x19, 0xE1, 0x03, 0x61, 0x0E, 0x71, 0x78, 0x1F,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5BF8 (' '): (width = 16, offset = 144) */
0x00, 0x0C, 0x00, 0x0C, 0xFF, 0xFF, 0x00, 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x06, 0x0C, 0x03, 0x0C,
0x03, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x7C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_91_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x5B57,     /*   5B57        23383            0       */
         0x5B9A,     /*   5B9A        23450            1       */
         0x5BB9,     /*   5BB9        23481            2       */
         0x5BBD,     /*   5BBD        23485            3       */
         0x5BF8     /*   5BF8        23544            4       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_92_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_91_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5B57,     // start char of used table
    5,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_91_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_91_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_92_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_92_data_table[] = { 

/* character 0x5C3A (' '): (width = 16, offset = 0) */
0x18, 0x03, 0x18, 0x03, 0x18, 0x03, 0x18, 0x03, 0x1F, 0xFF, 0x18, 0x60, 0x18, 0x60, 0x18, 0x60,
0x18, 0x30, 0x30, 0x30, 0x30, 0x18, 0x30, 0x0C, 0x60, 0x06, 0x60, 0x03, 0xC0, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5C4F (' '): (width = 16, offset = 36) */
0x30, 0x01, 0x30, 0x01, 0x3F, 0xFF, 0x30, 0x00, 0x33, 0x06, 0x31, 0x8C, 0x37, 0xFF, 0x31, 0x8C,
0x31, 0x8C, 0x31, 0x8C, 0x3F, 0xFF, 0x61, 0x8C, 0x63, 0x0C, 0x66, 0x0C, 0xDC, 0x0C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_92_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x5C3A,     /*   5C3A        23610            0       */
         0x5C4F     /*   5C4F        23631            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_94_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_92_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5C3A,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_92_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_92_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_94_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_94_data_table[] = { 

/* character 0x5E93 (' '): (width = 16, offset = 0) */
0x00, 0x61, 0x3F, 0xFF, 0x30, 0xC0, 0x30, 0xE3, 0x3F, 0xFF, 0x31, 0x80, 0x33, 0x38, 0x33, 0x30,
0x36, 0x37, 0x33, 0xFC, 0x30, 0x31, 0x3F, 0xFF, 0x30, 0x30, 0x60, 0x30, 0x60, 0x30, 0x00, 0x30,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_95_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_94_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5E93,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_94_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_95_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_95_data_table[] = { 

/* character 0x5F00 (' '): (width = 16, offset = 0) */
0x3F, 0xFF, 0x3F, 0xFF, 0x07, 0x1C, 0x07, 0x1C, 0x07, 0x1C, 0x07, 0x1C, 0x7F, 0xFF, 0x7F, 0xFF,
0x07, 0x1C, 0x07, 0x1C, 0x07, 0x1C, 0x0F, 0x1C, 0x1E, 0x1C, 0x3C, 0x1C, 0x30, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x5F55 (' '): (width = 16, offset = 36) */
0x0F, 0xFE, 0x00, 0x0C, 0x00, 0x0C, 0x1F, 0xFC, 0x00, 0x19, 0x7F, 0xFF, 0x00, 0x60, 0x18, 0x63,
0x0E, 0x66, 0x07, 0x7C, 0x00, 0x60, 0x07, 0xFC, 0x3C, 0x67, 0x30, 0x60, 0x07, 0xE0, 0x01, 0x80,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_95_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x5F00,     /*   5F00        24320            0       */
         0x5F55     /*   5F55        24405            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_96_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_95_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5F00,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_95_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_95_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_96_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_96_data_table[] = { 

/* character 0x606F (' '): (width = 16, offset = 0) */
0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC, 0x0C, 0x0C,
0x0C, 0x0C, 0x0F, 0xFC, 0x01, 0x83, 0x6C, 0xC1, 0x6C, 0xC6, 0x6C, 0x06, 0xC7, 0xFE, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_98_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_96_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x606F,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_96_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_98_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_98_data_table[] = { 

/* character 0x6253 (' '): (width = 16, offset = 0) */
0x19, 0xFF, 0xFF, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x1F, 0x06, 0x3C, 0x06,
0xF8, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x78, 0x3E, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_99_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_98_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6253,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_98_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_99_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_99_data_table[] = { 

/* character 0x6301 (' '): (width = 16, offset = 0) */
0x19, 0xFF, 0x18, 0x18, 0xFE, 0x18, 0x18, 0x18, 0x1B, 0xFF, 0x18, 0x06, 0x1E, 0x06, 0x3F, 0xFF,
0xF8, 0x06, 0x19, 0x86, 0x18, 0xC6, 0x18, 0x66, 0x18, 0x06, 0x18, 0x06, 0x78, 0x1E, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x6309 (' '): (width = 16, offset = 36) */
0x33, 0xFF, 0xFF, 0x00, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0xFF, 0x3C, 0x66, 0xF0, 0x66,
0x30, 0xC6, 0x30, 0xC6, 0x31, 0xFC, 0x30, 0x1C, 0x30, 0x36, 0x30, 0xE3, 0xF3, 0x81, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x6377 (' '): (width = 16, offset = 72) */
0x37, 0xFF, 0xFC, 0x30, 0x33, 0xFF, 0x30, 0x33, 0x30, 0x33, 0x37, 0xFF, 0x3C, 0x33, 0xF0, 0x33,
0x33, 0xFF, 0x30, 0x30, 0x33, 0x30, 0x33, 0x3F, 0x37, 0xB0, 0x36, 0xF0, 0xFC, 0x3F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_99_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x6301,     /*   6301        25345            0       */
         0x6309,     /*   6309        25353            1       */
         0x6377     /*   6377        25463            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_101_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_99_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6301,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_99_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_99_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_101_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_101_data_table[] = { 

/* character 0x652F (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x3F, 0xFC, 0x06, 0x0C, 0x06, 0x18,
0x03, 0x30, 0x01, 0xE0, 0x00, 0xC0, 0x01, 0xE0, 0x07, 0x38, 0x1C, 0x0E, 0xF0, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x6587 (' '): (width = 16, offset = 36) */
0x00, 0xC0, 0xFF, 0xFF, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x18, 0x06, 0x18, 0x03, 0x30, 0x03, 0x30,
0x01, 0xE0, 0x00, 0xC0, 0x01, 0xE0, 0x03, 0x30, 0x0E, 0x1C, 0x38, 0x07, 0xE0, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x65B9 (' '): (width = 16, offset = 72) */
0x00, 0xC0, 0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xFF, 0x01, 0x83, 0x03, 0x03,
0x03, 0x03, 0x03, 0x03, 0x06, 0x03, 0x06, 0x03, 0x0C, 0x03, 0x18, 0x06, 0x70, 0x3C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_101_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x652F,     /*   652F        25903            0       */
         0x6587,     /*   6587        25991            1       */
         0x65B9     /*   65B9        26041            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_102_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_101_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x652F,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_101_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_101_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_102_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_102_data_table[] = { 

/* character 0x663E (' '): (width = 16, offset = 0) */
0x1F, 0xFF, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFF, 0x19, 0xB8,
0x03, 0x30, 0x33, 0x33, 0x1B, 0x33, 0x0F, 0x36, 0x0F, 0x3C, 0x03, 0x31, 0x7F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_103_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_102_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x663E,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_102_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_103_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_103_data_table[] = { 

/* character 0x672A (' '): (width = 16, offset = 0) */
0x3F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x01, 0xE0, 0x03, 0xF0,
0x06, 0xD8, 0x0C, 0xCC, 0x18, 0xC6, 0x30, 0xC3, 0xE0, 0xC1, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x672C (' '): (width = 16, offset = 36) */
0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x01, 0xE0, 0x03, 0xF0, 0x03, 0xF0, 0x06, 0xD8, 0x0C, 0xCC,
0x18, 0xC6, 0x30, 0xC3, 0xEF, 0xFD, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_103_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x672A,     /*   672A        26410            0       */
         0x672C     /*   672C        26412            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_104_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_103_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x672A,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_103_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_103_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_104_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_104_data_table[] = { 

/* character 0x6848 (' '): (width = 16, offset = 0) */
0xFF, 0xFF, 0xC3, 0x00, 0xFF, 0xFF, 0x06, 0x0C, 0x0F, 0x98, 0x00, 0xF0, 0x03, 0xDC, 0x3E, 0x07,
0x00, 0xC0, 0xFF, 0xFF, 0x01, 0xE0, 0x07, 0xF8, 0x1C, 0xCE, 0xF0, 0xC3, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_108_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_104_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6848,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_104_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_108_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_108_data_table[] = { 

/* character 0x6C49 (' '): (width = 16, offset = 0) */
0x38, 0x01, 0x0F, 0xFF, 0x0D, 0x83, 0x07, 0x83, 0x67, 0x83, 0x36, 0xC6, 0x1C, 0xC6, 0x0C, 0x6C,
0x0C, 0x6C, 0x18, 0x38, 0x18, 0x18, 0x78, 0x38, 0x18, 0x6C, 0x18, 0xC7, 0x1B, 0x83, 0x1E, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_109_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_108_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6C49,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_108_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_109_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_109_data_table[] = { 

/* character 0x6D88 (' '): (width = 16, offset = 0) */
0x18, 0xD9, 0x00, 0xDB, 0xC0, 0x18, 0x61, 0xFF, 0x31, 0x81, 0x01, 0x81, 0x01, 0xFF, 0x01, 0x81,
0x19, 0x81, 0x19, 0xFF, 0x31, 0x81, 0x31, 0x81, 0x61, 0x81, 0x61, 0x81, 0xC1, 0x8F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_112_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_109_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6D88,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_109_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_112_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_112_data_table[] = { 

/* character 0x7070 (' '): (width = 16, offset = 0) */
0x30, 0x00, 0x30, 0x60, 0x30, 0x60, 0x33, 0x63, 0x33, 0x63, 0x33, 0x66, 0x36, 0x6C, 0x3C, 0xF0,
0x30, 0xF0, 0x31, 0x98, 0x61, 0x98, 0x63, 0x0C, 0x66, 0x06, 0xCC, 0x03, 0xF8, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_117_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_112_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7070,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_112_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_117_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_117_data_table[] = { 

/* character 0x7528 (' '): (width = 16, offset = 0) */
0x30, 0xC1, 0x30, 0xC1, 0x30, 0xC1, 0x3F, 0xFF, 0x30, 0xC1, 0x30, 0xC1, 0x30, 0xC1, 0x30, 0xC1,
0x3F, 0xFF, 0x30, 0xC1, 0x30, 0xC1, 0x30, 0xC1, 0x60, 0xC1, 0x60, 0xC1, 0xC0, 0xCF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x7559 (' '): (width = 16, offset = 36) */
0x30, 0x19, 0x30, 0x19, 0x33, 0x19, 0x33, 0x31, 0x3F, 0xB1, 0xF9, 0xEF, 0x00, 0xC0, 0x00, 0x00,
0x3F, 0xFF, 0x30, 0xC3, 0x30, 0xC3, 0x3F, 0xFF, 0x30, 0xC3, 0x30, 0xC3, 0x3F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_117_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x7528,     /*   7528        29992            0       */
         0x7559     /*   7559        30041            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_118_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_117_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7528,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_117_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_117_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_118_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_118_data_table[] = { 

/* character 0x76D8 (' '): (width = 16, offset = 0) */
0x00, 0xE0, 0x0F, 0xFF, 0x0D, 0x86, 0x0C, 0xE6, 0x0C, 0x67, 0x7F, 0xFF, 0x0D, 0x86, 0x0C, 0xE6,
0x18, 0x66, 0x18, 0x1E, 0x70, 0x0F, 0x1F, 0xFF, 0x19, 0xB3, 0x19, 0xB3, 0x7F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x76EE (' '): (width = 16, offset = 36) */
0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06,
0x18, 0x06, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_118_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x76D8,     /*   76D8        30424            0       */
         0x76EE     /*   76EE        30446            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_119_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_118_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x76D8,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_118_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_118_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_119_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_119_data_table[] = { 

/* character 0x77E5 (' '): (width = 16, offset = 0) */
0x7F, 0xBF, 0x6C, 0x31, 0xCC, 0x31, 0x0C, 0x31, 0x0C, 0x31, 0xFF, 0xF1, 0x0C, 0x31, 0x0C, 0x31,
0x0C, 0x31, 0x1E, 0x31, 0x1B, 0x31, 0x31, 0xB1, 0x31, 0xBF, 0x60, 0x00, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_120_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_119_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x77E5,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_119_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_120_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_120_data_table[] = { 

/* character 0x786E (' '): (width = 16, offset = 0) */
0x7F, 0x7F, 0x1C, 0x66, 0x1C, 0xEE, 0x18, 0xFF, 0x19, 0xEC, 0x3F, 0xEC, 0x3B, 0x7F, 0x7B, 0x6C,
0x7B, 0x6C, 0x1B, 0x7F, 0x1B, 0x60, 0x1F, 0x60, 0x18, 0xC0, 0x19, 0xC3, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x78C1 (' '): (width = 16, offset = 36) */
0x30, 0x6C, 0x37, 0xFF, 0x30, 0xC3, 0x60, 0xC3, 0x61, 0x86, 0x7D, 0x86, 0xEF, 0x7D, 0x6F, 0x6D,
0x6D, 0xC7, 0x6C, 0xC3, 0x6D, 0x86, 0x6D, 0xE7, 0x7F, 0x6D, 0x03, 0xFF, 0x07, 0xBE, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_120_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x786E,     /*   786E        30830            0       */
         0x78C1     /*   78C1        30913            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_121_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_120_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x786E,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_120_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_120_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_121_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_121_data_table[] = { 

/* character 0x793A (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x18, 0xC6,
0x18, 0xC6, 0x30, 0xC3, 0x30, 0xC3, 0x60, 0xC1, 0xC0, 0xC0, 0x00, 0xC0, 0x07, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_124_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_121_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x793A,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_121_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_124_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_124_data_table[] = { 

/* character 0x7C7B (' '): (width = 16, offset = 0) */
0x0C, 0xC6, 0x06, 0xCC, 0x06, 0xD9, 0x7F, 0xFF, 0x06, 0xC0, 0x07, 0xDC, 0x1C, 0xC7, 0x70, 0xC3,
0x00, 0xE0, 0x00, 0xC1, 0x7F, 0xFF, 0x00, 0xE0, 0x01, 0xB8, 0x03, 0x0E, 0x0E, 0x03, 0x78, 0x01,
0x00, 0x00, 0x00, 0x00, 

/* character 0x7CFB (' '): (width = 16, offset = 36) */
0x03, 0x00, 0x1E, 0x18, 0x0C, 0x30, 0x06, 0x66, 0x03, 0xC3, 0x01, 0x81, 0x03, 0x3F, 0xFF, 0xE0,
0x00, 0xC0, 0x0C, 0xCC, 0x0C, 0xCC, 0x18, 0xC6, 0x30, 0xC3, 0x60, 0xC1, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_124_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x7C7B,     /*   7C7B        31867            0       */
         0x7CFB     /*   7CFB        31995            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_126_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_124_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7C7B,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_124_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_124_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_126_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_126_data_table[] = { 

/* character 0x7EA2 (' '): (width = 16, offset = 0) */
0x0E, 0x00, 0x18, 0x01, 0x19, 0xFF, 0x33, 0x98, 0x36, 0x18, 0x7E, 0x18, 0x6C, 0x18, 0x18, 0x18,
0x30, 0x18, 0x7F, 0x18, 0x60, 0x18, 0x03, 0x18, 0x0E, 0x18, 0x78, 0x19, 0x63, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x7EDF (' '): (width = 16, offset = 36) */
0x0C, 0x18, 0x18, 0x19, 0x31, 0xFF, 0x37, 0x30, 0x66, 0x63, 0x7C, 0x61, 0x19, 0xFF, 0x19, 0xBD,
0x33, 0x6C, 0x7E, 0x6C, 0x60, 0x6C, 0x03, 0x6C, 0x1E, 0xCC, 0x70, 0xCC, 0x01, 0x8F, 0x07, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x7EFF (' '): (width = 16, offset = 72) */
0x1C, 0xFF, 0x18, 0x06, 0x30, 0x06, 0x37, 0xFE, 0x6C, 0x06, 0x7D, 0xFF, 0x18, 0x18, 0x31, 0x99,
0x36, 0xDF, 0x7C, 0x7E, 0x00, 0x7E, 0x07, 0xDB, 0x1F, 0x99, 0x73, 0x19, 0x00, 0xF8, 0x00, 0x30,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_126_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x7EA2,     /*   7EA2        32418            0       */
         0x7EDF,     /*   7EDF        32479            1       */
         0x7EFF     /*   7EFF        32511            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_127_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_126_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7EA2,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_126_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_126_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_127_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_127_data_table[] = { 

/* character 0x7F6E (' '): (width = 16, offset = 0) */
0x33, 0x33, 0x33, 0x33, 0x3F, 0xFF, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xC0, 0x0F, 0xFE, 0x6C, 0x06,
0x6F, 0xFE, 0x6C, 0x06, 0x6F, 0xFE, 0x6C, 0x06, 0x6F, 0xFE, 0x60, 0x00, 0x7F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_128_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_127_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7F6E,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_127_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_128_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_128_data_table[] = { 

/* character 0x80FD (' '): (width = 16, offset = 0) */
0x31, 0xB3, 0x37, 0xFE, 0xFC, 0xF0, 0x00, 0x30, 0x00, 0x30, 0x7F, 0x1F, 0x63, 0x00, 0x63, 0x30,
0x7F, 0x30, 0x63, 0x33, 0x63, 0x3E, 0x7F, 0x30, 0x63, 0x30, 0x63, 0x30, 0x6F, 0x1F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_130_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_128_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x80FD,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_128_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_130_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_130_data_table[] = { 

/* character 0x8272 (' '): (width = 16, offset = 0) */
0x1F, 0xF0, 0x30, 0x30, 0x60, 0x60, 0xC0, 0xC0, 0x3F, 0xFF, 0x30, 0xC3, 0x30, 0xC3, 0x30, 0xC3,
0x3F, 0xFF, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x1F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x82F1 (' '): (width = 16, offset = 36) */
0xFF, 0xFF, 0x06, 0x18, 0x06, 0xD8, 0x00, 0xC0, 0x1F, 0xFE, 0x18, 0xC6, 0x18, 0xC6, 0x18, 0xC6,
0xFF, 0xFF, 0x00, 0xC0, 0x01, 0xE0, 0x03, 0x30, 0x06, 0x18, 0x1C, 0x0E, 0xF0, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_130_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x8272,     /*   8272        33394            0       */
         0x82F1     /*   82F1        33521            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_132_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_130_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x8272,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_130_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_130_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_132_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_132_data_table[] = { 

/* character 0x84DD (' '): (width = 16, offset = 0) */
0x03, 0x9D, 0x7F, 0xFF, 0x03, 0x18, 0x06, 0x30, 0x37, 0x78, 0x3E, 0x63, 0x36, 0x7E, 0x36, 0xD8,
0x37, 0x8C, 0x06, 0x0C, 0x30, 0x06, 0x1F, 0xFF, 0x19, 0xB3, 0x19, 0xB3, 0x7F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_138_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_132_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x84DD,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_132_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_138_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_138_data_table[] = { 

/* character 0x8A00 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00,
0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_139_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_138_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x8A00,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_138_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_139_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_139_data_table[] = { 

/* character 0x8BBE (' '): (width = 16, offset = 0) */
0x18, 0x7E, 0x0C, 0x66, 0x0C, 0x66, 0x00, 0xC6, 0x0C, 0xC6, 0x7C, 0xC7, 0x0D, 0x80, 0x0D, 0xFF,
0x0C, 0xC6, 0x0C, 0xC6, 0x0F, 0x6C, 0x0E, 0x3C, 0x0E, 0x18, 0x1C, 0x3E, 0x0C, 0xE3, 0x07, 0x80,
0x00, 0x00, 0x00, 0x00, 

/* character 0x8BED (' '): (width = 16, offset = 36) */
0x19, 0xFF, 0x0C, 0x18, 0x0C, 0x30, 0x00, 0xFF, 0x0C, 0x33, 0x7C, 0x63, 0x0C, 0x63, 0x0F, 0xFF,
0x0C, 0x00, 0x0C, 0xC1, 0x0C, 0xFF, 0x0F, 0xC3, 0x0E, 0xC3, 0x1C, 0xC3, 0x18, 0xFF, 0x00, 0xC3,
0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_139_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x8BBE,     /*   8BBE        35774            0       */
         0x8BED     /*   8BED        35821            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_148_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_139_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x8BBE,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_139_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_139_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_148_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_148_data_table[] = { 

/* character 0x94AE (' '): (width = 16, offset = 0) */
0x1C, 0xC3, 0x1B, 0x7F, 0x1F, 0x9B, 0x30, 0x1B, 0x63, 0x1B, 0x7F, 0xB3, 0x6C, 0x33, 0x0F, 0xFF,
0x7F, 0xB3, 0x0C, 0x33, 0x0C, 0x33, 0x0C, 0x33, 0x0F, 0x33, 0x0E, 0x33, 0x0D, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_149_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_148_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x94AE,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_148_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_149_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_149_data_table[] = { 

/* character 0x95ED (' '): (width = 16, offset = 0) */
0x0D, 0x81, 0x36, 0xFF, 0x3E, 0x01, 0x30, 0x71, 0x30, 0x61, 0x3F, 0xFF, 0x30, 0x61, 0x30, 0xE1,
0x31, 0xE1, 0x33, 0x61, 0x36, 0x61, 0x3C, 0x61, 0x30, 0x61, 0x31, 0xE1, 0x30, 0x0F, 0x30, 0x03,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_152_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_149_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x95ED,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_149_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_152_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_152_data_table[] = { 

/* character 0x9884 (' '): (width = 16, offset = 0) */
0x7F, 0xFF, 0x03, 0x0C, 0x36, 0xD9, 0x1C, 0x7F, 0x0C, 0x61, 0x7F, 0xEF, 0x0F, 0x6D, 0x0F, 0x6D,
0x0E, 0x6D, 0x0C, 0x6D, 0x0C, 0x6D, 0x0C, 0x6D, 0x0C, 0x1E, 0x0C, 0x33, 0x7C, 0x60, 0x1B, 0xC0,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_154_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_152_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x9884,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_152_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_154_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_154_data_table[] = { 

/* character 0x9AD8 (' '): (width = 16, offset = 0) */
0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC, 0x00, 0x00, 0x7F, 0xFF,
0x60, 0x01, 0x67, 0xF9, 0x66, 0x19, 0x66, 0x19, 0x67, 0xF9, 0x60, 0x01, 0x60, 0x07, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_255_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_154_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x9AD8,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_154_data_table,      // bitmap/font data array
    &d4dfnt_CourierNew12_Page_255_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_CourierNew12_Page_255_data_table[] = { 

/* character 0xFF01 (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0xFF0C (' '): (width = 11, offset = 36) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
0x07, 0x00, 0x60, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_OFFSET d4dfnt_CourierNew12_Page_255_offset_table[] = { 

/*       offset       char      hexcode     */
/*       ======      =====      =======     */
            0,    /*   ' '        FF01       */
           36    /*   ' '        FF0C       */
};

const D4D_FONT_IX d4dfnt_CourierNew12_Page_255_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0xFF01,     /*   FF01        65281            0       */
         0xFF0C     /*   FF0C        65292            1       */
};

const D4D_FONT_SIZE d4dfnt_CourierNew12_Page_255_width_table[] = { 

/*       width        char      hexcode     */
/*       ======      =====      =======     */
          16,     /*   ' '        FF01       */
          11     /*   ' '        FF0C       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew12_Page_255_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL,   // linear / nonlinear , proporcional or not
    0xFF01,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    36,     // height of font bitmpap for non proporcional fonts
    13,      // offset from Y0 coordination to baseline
    {16, 18},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_CourierNew12_Page_255_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    d4dfnt_CourierNew12_Page_255_offset_table,     // Offset table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_255_width_table,        // Size table - used when proporcial font is set in flags
    d4dfnt_CourierNew12_Page_255_data_table,      // bitmap/font data array
    NULL       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial8_data_table[] = { 

/* character 0x0020 (' '): (width = 3, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0021 ('!'): (width = 3, offset = 6) */
0x00, 0x6D, 0xB6, 0x1B, 0x00, 0x00, 

/* character 0x0022 ('"'): (width = 5, offset = 12) */
0x00, 0x01, 0xBD, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0023 ('#'): (width = 6, offset = 21) */
0x00, 0x00, 0x09, 0x27, 0xF4, 0x92, 0xFE, 0x49, 0x00, 0x00, 0x00, 

/* character 0x0024 ('$'): (width = 6, offset = 32) */
0x00, 0x02, 0x1C, 0xAA, 0x8F, 0x1E, 0x2A, 0xA7, 0x08, 0x00, 0x00, 

/* character 0x0025 ('%'): (width = 9, offset = 43) */
0x00, 0x00, 0x00, 0x1C, 0x8A, 0x45, 0x43, 0xA0, 0x2E, 0x15, 0x12, 0x89, 0xC0, 0x00, 0x00, 0x00,


/* character 0x0026 ('&'): (width = 8, offset = 59) */
0x00, 0x00, 0x00, 0x38, 0x6C, 0x6C, 0x30, 0x7A, 0xCE, 0xCC, 0x7E, 0x00, 0x00, 0x00, 

/* character 0x0027 ('''): (width = 3, offset = 73) */
0x00, 0x6D, 0x80, 0x00, 0x00, 0x00, 

/* character 0x0028 ('('): (width = 4, offset = 79) */
0x00, 0x02, 0x6C, 0xCC, 0xCC, 0x46, 0x20, 

/* character 0x0029 (')'): (width = 4, offset = 86) */
0x00, 0x08, 0x44, 0x66, 0x66, 0x4C, 0x80, 

/* character 0x002A ('*'): (width = 4, offset = 93) */
0x00, 0x04, 0xE4, 0xA0, 0x00, 0x00, 0x00, 

/* character 0x002B ('+'): (width = 6, offset = 100) */
0x00, 0x00, 0x00, 0x00, 0x82, 0x3E, 0x20, 0x80, 0x00, 0x00, 0x00, 

/* character 0x002C (','): (width = 3, offset = 111) */
0x00, 0x00, 0x00, 0x1B, 0x28, 0x00, 

/* character 0x002D ('-'): (width = 4, offset = 117) */
0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 

/* character 0x002E ('.'): (width = 3, offset = 124) */
0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 

/* character 0x002F ('/'): (width = 3, offset = 130) */
0x00, 0x12, 0x92, 0x52, 0x00, 0x00, 

/* character 0x0030 ('0'): (width = 6, offset = 136) */
0x00, 0x00, 0x1C, 0xDB, 0x6D, 0xB6, 0xDB, 0x67, 0x00, 0x00, 0x00, 

/* character 0x0031 ('1'): (width = 6, offset = 147) */
0x00, 0x00, 0x0C, 0x73, 0xCB, 0x0C, 0x30, 0xC3, 0x00, 0x00, 0x00, 

/* character 0x0032 ('2'): (width = 6, offset = 158) */
0x00, 0x00, 0x1C, 0xD8, 0x61, 0x8C, 0x63, 0x0F, 0x80, 0x00, 0x00, 

/* character 0x0033 ('3'): (width = 6, offset = 169) */
0x00, 0x00, 0x1C, 0xD8, 0x63, 0x06, 0x1B, 0x67, 0x00, 0x00, 0x00, 

/* character 0x0034 ('4'): (width = 6, offset = 180) */
0x00, 0x00, 0x06, 0x39, 0x65, 0xA6, 0xFC, 0x61, 0x80, 0x00, 0x00, 

/* character 0x0035 ('5'): (width = 6, offset = 191) */
0x00, 0x00, 0x1E, 0x63, 0x0F, 0x26, 0x1B, 0x67, 0x00, 0x00, 0x00, 

/* character 0x0036 ('6'): (width = 6, offset = 202) */
0x00, 0x00, 0x1C, 0xDB, 0x0F, 0x36, 0xDB, 0x67, 0x00, 0x00, 0x00, 

/* character 0x0037 ('7'): (width = 6, offset = 213) */
0x00, 0x00, 0x3E, 0x18, 0xC3, 0x18, 0x61, 0x86, 0x00, 0x00, 0x00, 

/* character 0x0038 ('8'): (width = 6, offset = 224) */
0x00, 0x00, 0x1C, 0xDB, 0x67, 0x36, 0xDB, 0x67, 0x00, 0x00, 0x00, 

/* character 0x0039 ('9'): (width = 6, offset = 235) */
0x00, 0x00, 0x1C, 0xDB, 0x6D, 0x9E, 0x1B, 0x67, 0x00, 0x00, 0x00, 

/* character 0x003A (':'): (width = 3, offset = 246) */
0x00, 0x01, 0xB0, 0x1B, 0x00, 0x00, 

/* character 0x003B (';'): (width = 3, offset = 252) */
0x00, 0x01, 0xB0, 0x1B, 0x28, 0x00, 

/* character 0x003C ('<'): (width = 6, offset = 258) */
0x00, 0x00, 0x00, 0x08, 0xCC, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 

/* character 0x003D ('='): (width = 6, offset = 269) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0xF8, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003E ('>'): (width = 6, offset = 280) */
0x00, 0x00, 0x00, 0x81, 0x81, 0x86, 0x62, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003F ('?'): (width = 7, offset = 291) */
0x00, 0x00, 0x01, 0xC6, 0xC1, 0x86, 0x18, 0x00, 0x60, 0xC0, 0x00, 0x00, 0x00, 

/* character 0x0040 ('@'): (width = 11, offset = 304) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x86, 0x08, 0xBE, 0xA4, 0xD5, 0x12, 0xA2, 0x54, 0xD1, 0x7C, 0x30,
0x61, 0xF0, 0x00, 0x00, 

/* character 0x0041 ('A'): (width = 8, offset = 324) */
0x00, 0x00, 0x00, 0x38, 0x38, 0x6C, 0x6C, 0x6C, 0xFE, 0xC6, 0xC6, 0x00, 0x00, 0x00, 

/* character 0x0042 ('B'): (width = 7, offset = 338) */
0x00, 0x00, 0x07, 0xCC, 0xD9, 0xBE, 0x66, 0xCD, 0x9B, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0043 ('C'): (width = 8, offset = 351) */
0x00, 0x00, 0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, 0x00, 0x00, 

/* character 0x0044 ('D'): (width = 7, offset = 365) */
0x00, 0x00, 0x07, 0xCC, 0xD9, 0xB3, 0x66, 0xCD, 0x9B, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0045 ('E'): (width = 6, offset = 378) */
0x00, 0x00, 0x3E, 0xC3, 0x0F, 0xB0, 0xC3, 0x0F, 0x80, 0x00, 0x00, 

/* character 0x0046 ('F'): (width = 6, offset = 389) */
0x00, 0x00, 0x3E, 0xC3, 0x0F, 0xB0, 0xC3, 0x0C, 0x00, 0x00, 0x00, 

/* character 0x0047 ('G'): (width = 8, offset = 400) */
0x00, 0x00, 0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0xC6, 0x66, 0x3C, 0x00, 0x00, 0x00, 

/* character 0x0048 ('H'): (width = 7, offset = 414) */
0x00, 0x00, 0x06, 0x6C, 0xD9, 0xBF, 0x66, 0xCD, 0x9B, 0x30, 0x00, 0x00, 0x00, 

/* character 0x0049 ('I'): (width = 3, offset = 427) */
0x00, 0x6D, 0xB6, 0xDB, 0x00, 0x00, 

/* character 0x004A ('J'): (width = 6, offset = 433) */
0x00, 0x00, 0x06, 0x18, 0x61, 0x86, 0x1B, 0x67, 0x00, 0x00, 0x00, 

/* character 0x004B ('K'): (width = 7, offset = 444) */
0x00, 0x00, 0x06, 0x6D, 0x9B, 0x3C, 0x7C, 0xD9, 0x9B, 0x30, 0x00, 0x00, 0x00, 

/* character 0x004C ('L'): (width = 7, offset = 457) */
0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC1, 0x83, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x004D ('M'): (width = 10, offset = 470) */
0x00, 0x00, 0x00, 0x03, 0x8E, 0xE3, 0xBD, 0xEF, 0x7B, 0x56, 0xDD, 0xB7, 0x6C, 0x98, 0x00, 0x00,
0x00, 0x00, 

/* character 0x004E ('N'): (width = 7, offset = 488) */
0x00, 0x00, 0x06, 0x6C, 0xDD, 0xBB, 0x6E, 0xDD, 0x9B, 0x30, 0x00, 0x00, 0x00, 

/* character 0x004F ('O'): (width = 8, offset = 501) */
0x00, 0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00, 0x00, 

/* character 0x0050 ('P'): (width = 7, offset = 515) */
0x00, 0x00, 0x07, 0xCC, 0xD9, 0xB3, 0x7C, 0xC1, 0x83, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0051 ('Q'): (width = 8, offset = 528) */
0x00, 0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xD6, 0x6C, 0x3C, 0x02, 0x00, 0x00, 

/* character 0x0052 ('R'): (width = 7, offset = 542) */
0x00, 0x00, 0x07, 0xCC, 0xD9, 0xB3, 0x7C, 0xD9, 0x9B, 0x18, 0x00, 0x00, 0x00, 

/* character 0x0053 ('S'): (width = 7, offset = 555) */
0x00, 0x00, 0x03, 0xCC, 0xD8, 0x3C, 0x1E, 0x0D, 0x99, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0054 ('T'): (width = 7, offset = 568) */
0x00, 0x00, 0x07, 0xE3, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00, 0x00, 0x00, 

/* character 0x0055 ('U'): (width = 7, offset = 581) */
0x00, 0x00, 0x06, 0x6C, 0xD9, 0xB3, 0x66, 0xCD, 0x99, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0056 ('V'): (width = 8, offset = 594) */
0x00, 0x00, 0x00, 0xC6, 0xC6, 0x6C, 0x6C, 0x6C, 0x6C, 0x38, 0x38, 0x00, 0x00, 0x00, 

/* character 0x0057 ('W'): (width = 10, offset = 608) */
0x00, 0x00, 0x00, 0x02, 0x23, 0x9C, 0xF7, 0x6D, 0x5B, 0x56, 0xF7, 0x98, 0xC6, 0x30, 0x00, 0x00,
0x00, 0x00, 

/* character 0x0058 ('X'): (width = 7, offset = 626) */
0x00, 0x00, 0x06, 0x6C, 0xCF, 0x0C, 0x18, 0x79, 0x9B, 0x30, 0x00, 0x00, 0x00, 

/* character 0x0059 ('Y'): (width = 7, offset = 639) */
0x00, 0x00, 0x06, 0x6C, 0xC9, 0x1E, 0x18, 0x30, 0x60, 0xC0, 0x00, 0x00, 0x00, 

/* character 0x005A ('Z'): (width = 7, offset = 652) */
0x00, 0x00, 0x07, 0xE0, 0xC3, 0x0C, 0x18, 0x61, 0x83, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x005B ('['): (width = 4, offset = 665) */
0x00, 0x0E, 0xCC, 0xCC, 0xCC, 0xCC, 0xE0, 

/* character 0x005C ('\'): (width = 3, offset = 672) */
0x00, 0x48, 0x92, 0x44, 0x80, 0x00, 

/* character 0x005D (']'): (width = 4, offset = 678) */
0x00, 0x0E, 0x66, 0x66, 0x66, 0x66, 0xE0, 

/* character 0x005E ('^'): (width = 6, offset = 685) */
0x00, 0x00, 0x08, 0x71, 0x4D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005F ('_'): (width = 6, offset = 696) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 

/* character 0x0060 ('`'): (width = 4, offset = 707) */
0x00, 0x0C, 0x60, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0061 ('a'): (width = 6, offset = 714) */
0x00, 0x00, 0x00, 0x01, 0xC9, 0x9E, 0xDB, 0x67, 0x80, 0x00, 0x00, 

/* character 0x0062 ('b'): (width = 7, offset = 725) */
0x00, 0x00, 0x06, 0x0C, 0x1F, 0x33, 0x66, 0xCD, 0x9B, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0063 ('c'): (width = 6, offset = 738) */
0x00, 0x00, 0x00, 0x01, 0xCD, 0xB0, 0xC3, 0x67, 0x00, 0x00, 0x00, 

/* character 0x0064 ('d'): (width = 7, offset = 749) */
0x00, 0x00, 0x00, 0x60, 0xCF, 0xB3, 0x66, 0xCD, 0x99, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x0065 ('e'): (width = 7, offset = 762) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x33, 0x7E, 0xC1, 0x99, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0066 ('f'): (width = 4, offset = 775) */
0x00, 0x07, 0xCE, 0xCC, 0xCC, 0xC0, 0x00, 

/* character 0x0067 ('g'): (width = 7, offset = 782) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0xB3, 0x66, 0xCD, 0x99, 0xF4, 0x67, 0x80, 0x00, 

/* character 0x0068 ('h'): (width = 7, offset = 795) */
0x00, 0x00, 0x06, 0x0C, 0x1F, 0x33, 0x66, 0xCD, 0x9B, 0x30, 0x00, 0x00, 0x00, 

/* character 0x0069 ('i'): (width = 3, offset = 808) */
0x00, 0x61, 0xB6, 0xDB, 0x00, 0x00, 

/* character 0x006A ('j'): (width = 3, offset = 814) */
0x00, 0x61, 0xB6, 0xDB, 0x68, 0x00, 

/* character 0x006B ('k'): (width = 7, offset = 820) */
0x00, 0x00, 0x06, 0x0C, 0x19, 0xB6, 0x78, 0xF9, 0xB3, 0x30, 0x00, 0x00, 0x00, 

/* character 0x006C ('l'): (width = 3, offset = 833) */
0x00, 0x6D, 0xB6, 0xDB, 0x00, 0x00, 

/* character 0x006D ('m'): (width = 11, offset = 839) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x33, 0x36, 0x66, 0xCC, 0xD9, 0x9B, 0x33, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x006E ('n'): (width = 7, offset = 859) */
0x00, 0x00, 0x00, 0x00, 0x1F, 0x33, 0x66, 0xCD, 0x9B, 0x30, 0x00, 0x00, 0x00, 

/* character 0x006F ('o'): (width = 7, offset = 872) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x33, 0x66, 0xCD, 0x99, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0070 ('p'): (width = 7, offset = 885) */
0x00, 0x00, 0x00, 0x00, 0x1F, 0x33, 0x66, 0xCD, 0x9B, 0xE6, 0x0C, 0x00, 0x00, 

/* character 0x0071 ('q'): (width = 7, offset = 898) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0xB3, 0x66, 0xCD, 0x99, 0xF0, 0x60, 0xC0, 0x00, 

/* character 0x0072 ('r'): (width = 5, offset = 911) */
0x00, 0x00, 0x00, 0x7B, 0x18, 0xC6, 0x30, 0x00, 0x00, 

/* character 0x0073 ('s'): (width = 7, offset = 920) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x33, 0x78, 0x3D, 0x99, 0xE0, 0x00, 0x00, 0x00, 

/* character 0x0074 ('t'): (width = 4, offset = 933) */
0x00, 0x04, 0xCE, 0xCC, 0xCC, 0x60, 0x00, 

/* character 0x0075 ('u'): (width = 7, offset = 940) */
0x00, 0x00, 0x00, 0x00, 0x19, 0xB3, 0x66, 0xCD, 0x99, 0xF0, 0x00, 0x00, 0x00, 

/* character 0x0076 ('v'): (width = 6, offset = 953) */
0x00, 0x00, 0x00, 0x03, 0x6D, 0xB6, 0xD9, 0xC7, 0x00, 0x00, 0x00, 

/* character 0x0077 ('w'): (width = 10, offset = 964) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x6D, 0xDB, 0x76, 0x77, 0x1D, 0xC6, 0x30, 0x00, 0x00,
0x00, 0x00, 

/* character 0x0078 ('x'): (width = 6, offset = 982) */
0x00, 0x00, 0x00, 0x03, 0x6D, 0x9C, 0x73, 0x6D, 0x80, 0x00, 0x00, 

/* character 0x0079 ('y'): (width = 6, offset = 993) */
0x00, 0x00, 0x00, 0x02, 0x3D, 0xB6, 0xD9, 0xC7, 0x18, 0xC0, 0x00, 

/* character 0x007A ('z'): (width = 6, offset = 1004) */
0x00, 0x00, 0x00, 0x03, 0xE1, 0x8C, 0x63, 0x0F, 0x80, 0x00, 0x00, 

/* character 0x007B ('{'): (width = 4, offset = 1015) */
0x00, 0x06, 0xCC, 0xCC, 0x8C, 0xCC, 0x60, 

/* character 0x007C ('|'): (width = 3, offset = 1022) */
0x00, 0x24, 0x92, 0x49, 0x24, 0x00, 

/* character 0x007D ('}'): (width = 4, offset = 1028) */
0x00, 0x0C, 0x66, 0x66, 0x36, 0x66, 0xC0, 

/* character 0x007E ('~'): (width = 6, offset = 1035) */
0x00, 0x00, 0x00, 0x00, 0x06, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007F (''): (width = 4, offset = 1046) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_OFFSET d4dfnt_Arial8_offset_table[] = { 

/*       offset       char      hexcode     */
/*       ======      =====      =======     */
            0,    /*   ' '        0020       */
            6,    /*   '!'        0021       */
           12,    /*   '"'        0022       */
           21,    /*   '#'        0023       */
           32,    /*   '$'        0024       */
           43,    /*   '%'        0025       */
           59,    /*   '&'        0026       */
           73,    /*   '''        0027       */
           79,    /*   '('        0028       */
           86,    /*   ')'        0029       */
           93,    /*   '*'        002A       */
          100,    /*   '+'        002B       */
          111,    /*   ','        002C       */
          117,    /*   '-'        002D       */
          124,    /*   '.'        002E       */
          130,    /*   '/'        002F       */
          136,    /*   '0'        0030       */
          147,    /*   '1'        0031       */
          158,    /*   '2'        0032       */
          169,    /*   '3'        0033       */
          180,    /*   '4'        0034       */
          191,    /*   '5'        0035       */
          202,    /*   '6'        0036       */
          213,    /*   '7'        0037       */
          224,    /*   '8'        0038       */
          235,    /*   '9'        0039       */
          246,    /*   ':'        003A       */
          252,    /*   ';'        003B       */
          258,    /*   '<'        003C       */
          269,    /*   '='        003D       */
          280,    /*   '>'        003E       */
          291,    /*   '?'        003F       */
          304,    /*   '@'        0040       */
          324,    /*   'A'        0041       */
          338,    /*   'B'        0042       */
          351,    /*   'C'        0043       */
          365,    /*   'D'        0044       */
          378,    /*   'E'        0045       */
          389,    /*   'F'        0046       */
          400,    /*   'G'        0047       */
          414,    /*   'H'        0048       */
          427,    /*   'I'        0049       */
          433,    /*   'J'        004A       */
          444,    /*   'K'        004B       */
          457,    /*   'L'        004C       */
          470,    /*   'M'        004D       */
          488,    /*   'N'        004E       */
          501,    /*   'O'        004F       */
          515,    /*   'P'        0050       */
          528,    /*   'Q'        0051       */
          542,    /*   'R'        0052       */
          555,    /*   'S'        0053       */
          568,    /*   'T'        0054       */
          581,    /*   'U'        0055       */
          594,    /*   'V'        0056       */
          608,    /*   'W'        0057       */
          626,    /*   'X'        0058       */
          639,    /*   'Y'        0059       */
          652,    /*   'Z'        005A       */
          665,    /*   '['        005B       */
          672,    /*   '\'        005C       */
          678,    /*   ']'        005D       */
          685,    /*   '^'        005E       */
          696,    /*   '_'        005F       */
          707,    /*   '`'        0060       */
          714,    /*   'a'        0061       */
          725,    /*   'b'        0062       */
          738,    /*   'c'        0063       */
          749,    /*   'd'        0064       */
          762,    /*   'e'        0065       */
          775,    /*   'f'        0066       */
          782,    /*   'g'        0067       */
          795,    /*   'h'        0068       */
          808,    /*   'i'        0069       */
          814,    /*   'j'        006A       */
          820,    /*   'k'        006B       */
          833,    /*   'l'        006C       */
          839,    /*   'm'        006D       */
          859,    /*   'n'        006E       */
          872,    /*   'o'        006F       */
          885,    /*   'p'        0070       */
          898,    /*   'q'        0071       */
          911,    /*   'r'        0072       */
          920,    /*   's'        0073       */
          933,    /*   't'        0074       */
          940,    /*   'u'        0075       */
          953,    /*   'v'        0076       */
          964,    /*   'w'        0077       */
          982,    /*   'x'        0078       */
          993,    /*   'y'        0079       */
         1004,    /*   'z'        007A       */
         1015,    /*   '{'        007B       */
         1022,    /*   '|'        007C       */
         1028,    /*   '}'        007D       */
         1035,    /*   '~'        007E       */
         1046    /*   ''        007F       */
};

const D4D_FONT_SIZE d4dfnt_Arial8_width_table[] = { 

/*       width        char      hexcode     */
/*       ======      =====      =======     */
           3,    /*   ' '        0020       */
           3,    /*   '!'        0021       */
           5,    /*   '"'        0022       */
           6,    /*   '#'        0023       */
           6,    /*   '$'        0024       */
           9,    /*   '%'        0025       */
           8,    /*   '&'        0026       */
           3,    /*   '''        0027       */
           4,    /*   '('        0028       */
           4,    /*   ')'        0029       */
           4,    /*   '*'        002A       */
           6,    /*   '+'        002B       */
           3,    /*   ','        002C       */
           4,    /*   '-'        002D       */
           3,    /*   '.'        002E       */
           3,    /*   '/'        002F       */
           6,    /*   '0'        0030       */
           6,    /*   '1'        0031       */
           6,    /*   '2'        0032       */
           6,    /*   '3'        0033       */
           6,    /*   '4'        0034       */
           6,    /*   '5'        0035       */
           6,    /*   '6'        0036       */
           6,    /*   '7'        0037       */
           6,    /*   '8'        0038       */
           6,    /*   '9'        0039       */
           3,    /*   ':'        003A       */
           3,    /*   ';'        003B       */
           6,    /*   '<'        003C       */
           6,    /*   '='        003D       */
           6,    /*   '>'        003E       */
           7,    /*   '?'        003F       */
          11,    /*   '@'        0040       */
           8,    /*   'A'        0041       */
           7,    /*   'B'        0042       */
           8,    /*   'C'        0043       */
           7,    /*   'D'        0044       */
           6,    /*   'E'        0045       */
           6,    /*   'F'        0046       */
           8,    /*   'G'        0047       */
           7,    /*   'H'        0048       */
           3,    /*   'I'        0049       */
           6,    /*   'J'        004A       */
           7,    /*   'K'        004B       */
           7,    /*   'L'        004C       */
          10,    /*   'M'        004D       */
           7,    /*   'N'        004E       */
           8,    /*   'O'        004F       */
           7,    /*   'P'        0050       */
           8,    /*   'Q'        0051       */
           7,    /*   'R'        0052       */
           7,    /*   'S'        0053       */
           7,    /*   'T'        0054       */
           7,    /*   'U'        0055       */
           8,    /*   'V'        0056       */
          10,    /*   'W'        0057       */
           7,    /*   'X'        0058       */
           7,    /*   'Y'        0059       */
           7,    /*   'Z'        005A       */
           4,    /*   '['        005B       */
           3,    /*   '\'        005C       */
           4,    /*   ']'        005D       */
           6,    /*   '^'        005E       */
           6,    /*   '_'        005F       */
           4,    /*   '`'        0060       */
           6,    /*   'a'        0061       */
           7,    /*   'b'        0062       */
           6,    /*   'c'        0063       */
           7,    /*   'd'        0064       */
           7,    /*   'e'        0065       */
           4,    /*   'f'        0066       */
           7,    /*   'g'        0067       */
           7,    /*   'h'        0068       */
           3,    /*   'i'        0069       */
           3,    /*   'j'        006A       */
           7,    /*   'k'        006B       */
           3,    /*   'l'        006C       */
          11,    /*   'm'        006D       */
           7,    /*   'n'        006E       */
           7,    /*   'o'        006F       */
           7,    /*   'p'        0070       */
           7,    /*   'q'        0071       */
           5,    /*   'r'        0072       */
           7,    /*   's'        0073       */
           4,    /*   't'        0074       */
           7,    /*   'u'        0075       */
           6,    /*   'v'        0076       */
          10,    /*   'w'        0077       */
           6,    /*   'x'        0078       */
           6,    /*   'y'        0079       */
           6,    /*   'z'        007A       */
           4,    /*   '{'        007B       */
           3,    /*   '|'        007C       */
           4,    /*   '}'        007D       */
           6,    /*   '~'        007E       */
           4     /*   ''        007F       */
};

const Byte d4dfnt_Arial12_data_table[] = { 

/* character 0x0020 (' '): (width = 4, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0021 ('!'): (width = 4, offset = 10) */
0x00, 0x06, 0x66, 0x66, 0x66, 0x66, 0x06, 0x60, 0x00, 0x00, 

/* character 0x0022 ('"'): (width = 8, offset = 20) */
0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 

/* character 0x0023 ('#'): (width = 9, offset = 39) */
0x00, 0x00, 0x00, 0x03, 0x61, 0xB0, 0xD9, 0xFC, 0xFE, 0x36, 0x1B, 0x1F, 0xCF, 0xE6, 0xC3, 0x61,
0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0024 ('$'): (width = 9, offset = 61) */
0x00, 0x00, 0x02, 0x03, 0x83, 0xE3, 0x59, 0xA0, 0xF0, 0x3C, 0x0F, 0x03, 0xCD, 0x66, 0xB1, 0xF0,
0x70, 0x10, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0025 ('%'): (width = 16, offset = 83) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x30, 0xCC, 0x60, 0xCC, 0x60, 0xCC, 0xC0, 0xCD, 0x80,
0x79, 0x80, 0x03, 0x3C, 0x03, 0x66, 0x06, 0x66, 0x06, 0x66, 0x0C, 0x66, 0x18, 0x3C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0026 ('&'): (width = 12, offset = 121) */
0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x3F, 0x83, 0x18, 0x31, 0x81, 0xF0, 0x1E, 0x03, 0x64, 0x67,
0x66, 0x3C, 0x61, 0xE3, 0xFF, 0x1E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0027 ('''): (width = 4, offset = 150) */
0x00, 0x06, 0x66, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0028 ('('): (width = 5, offset = 160) */
0x00, 0x00, 0x33, 0x18, 0xCC, 0x63, 0x18, 0xC6, 0x30, 0xC6, 0x30, 0xC0, 

/* character 0x0029 (')'): (width = 5, offset = 172) */
0x00, 0x01, 0x86, 0x31, 0x86, 0x31, 0x8C, 0x63, 0x19, 0x8C, 0x66, 0x00, 

/* character 0x002A ('*'): (width = 6, offset = 184) */
0x00, 0x00, 0x08, 0xF8, 0x8D, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002B ('+'): (width = 9, offset = 199) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60, 0x30, 0xFF, 0x7F, 0x86, 0x03, 0x01, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002C (','): (width = 4, offset = 221) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x62, 0x24, 0x00, 

/* character 0x002D ('-'): (width = 5, offset = 231) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xF0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x002E ('.'): (width = 4, offset = 243) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 

/* character 0x002F ('/'): (width = 4, offset = 253) */
0x00, 0x03, 0x33, 0x66, 0x66, 0x66, 0xCC, 0xC0, 0x00, 0x00, 

/* character 0x0030 ('0'): (width = 9, offset = 263) */
0x00, 0x00, 0x00, 0x07, 0x87, 0xE7, 0x3B, 0x0D, 0x86, 0xC3, 0x61, 0xB0, 0xD8, 0x6E, 0x73, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0031 ('1'): (width = 9, offset = 285) */
0x00, 0x00, 0x00, 0x01, 0x81, 0xC1, 0xE1, 0xB0, 0x98, 0x0C, 0x06, 0x03, 0x01, 0x80, 0xC0, 0x60,
0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0032 ('2'): (width = 9, offset = 307) */
0x00, 0x00, 0x00, 0x07, 0x87, 0xE7, 0x1B, 0x0C, 0x06, 0x06, 0x07, 0x07, 0x07, 0x06, 0x07, 0xFB,
0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0033 ('3'): (width = 9, offset = 329) */
0x00, 0x00, 0x00, 0x07, 0xC7, 0xF6, 0x18, 0x0C, 0x3C, 0x1E, 0x03, 0x80, 0xD8, 0x6E, 0x73, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0034 ('4'): (width = 9, offset = 351) */
0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x70, 0x78, 0x6C, 0x36, 0x33, 0x31, 0x9F, 0xEF, 0xF0, 0x30,
0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0035 ('5'): (width = 9, offset = 373) */
0x00, 0x00, 0x00, 0x0F, 0xC7, 0xE3, 0x03, 0x81, 0xF8, 0xFE, 0x63, 0x80, 0xD8, 0x6E, 0x73, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0036 ('6'): (width = 9, offset = 395) */
0x00, 0x00, 0x00, 0x07, 0xC7, 0xF3, 0x1B, 0x01, 0xB8, 0xFE, 0x73, 0xB0, 0xD8, 0x66, 0x33, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0037 ('7'): (width = 9, offset = 417) */
0x00, 0x00, 0x00, 0x1F, 0xEF, 0xF0, 0x30, 0x30, 0x18, 0x18, 0x0C, 0x06, 0x07, 0x03, 0x01, 0x80,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0038 ('8'): (width = 9, offset = 439) */
0x00, 0x00, 0x00, 0x07, 0x87, 0xE6, 0x1B, 0x0D, 0x86, 0x7E, 0x3F, 0x30, 0xD8, 0x6C, 0x33, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0039 ('9'): (width = 9, offset = 461) */
0x00, 0x00, 0x00, 0x07, 0x87, 0xE6, 0x33, 0x0D, 0x86, 0xE7, 0x3F, 0x8E, 0xC0, 0x6C, 0x67, 0xF1,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003A (':'): (width = 6, offset = 483) */
0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003B (';'): (width = 6, offset = 498) */
0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x04, 0x10, 0x80, 0x00, 

/* character 0x003C ('<'): (width = 9, offset = 513) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3C, 0x78, 0x70, 0x1E, 0x03, 0xC0, 0x70, 0x08,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003D ('='): (width = 9, offset = 535) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFD, 0xFE, 0x00, 0x7F, 0xBF, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003E ('>'): (width = 9, offset = 557) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x03, 0x80, 0xF0, 0x1E, 0x03, 0x87, 0x8F, 0x0E, 0x04, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x003F ('?'): (width = 10, offset = 579) */
0x00, 0x00, 0x00, 0x00, 0x78, 0x3F, 0x1C, 0x66, 0x18, 0x0E, 0x07, 0x03, 0x80, 0xC0, 0x30, 0x00,
0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0040 ('@'): (width = 16, offset = 603) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x0F, 0xFC, 0x1C, 0x0E, 0x39, 0xDE, 0x37, 0xFB,
0x76, 0x3B, 0x6C, 0x33, 0x6C, 0x33, 0x6C, 0x33, 0x6C, 0x76, 0x6F, 0xFC, 0x37, 0xB8, 0x38, 0x03,
0x1C, 0x0E, 0x0F, 0xFC, 0x03, 0xF0, 

/* character 0x0041 ('A'): (width = 11, offset = 641) */
0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0xE0, 0x36, 0x06, 0xC0, 0xD8, 0x31, 0x86, 0x30, 0xFE, 0x3F,
0xE6, 0x0C, 0xC1, 0xB0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0042 ('B'): (width = 12, offset = 668) */
0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x7F, 0xC6, 0x0C, 0x60, 0xC6, 0x0C, 0x7F, 0x87, 0xFC, 0x60,
0xE6, 0x06, 0x60, 0x67, 0xFC, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0043 ('C'): (width = 12, offset = 697) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x3F, 0xC3, 0x0E, 0x60, 0x46, 0x00, 0x60, 0x06, 0x00, 0x60,
0x06, 0x04, 0x30, 0xE3, 0xFC, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0044 ('D'): (width = 12, offset = 726) */
0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x7F, 0xC6, 0x0C, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60,
0x66, 0x06, 0x60, 0xC7, 0xFC, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0045 ('E'): (width = 11, offset = 755) */
0x00, 0x00, 0x00, 0x00, 0x3F, 0xE7, 0xFC, 0xC0, 0x18, 0x03, 0x00, 0x7F, 0xCF, 0xF9, 0x80, 0x30,
0x06, 0x00, 0xFF, 0x9F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0046 ('F'): (width = 10, offset = 782) */
0x00, 0x00, 0x00, 0x01, 0xFE, 0x7F, 0x98, 0x06, 0x01, 0x80, 0x7F, 0x1F, 0xC6, 0x01, 0x80, 0x60,
0x18, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0047 ('G'): (width = 12, offset = 806) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x3F, 0xC3, 0x0E, 0x60, 0x46, 0x00, 0x60, 0x06, 0x3E, 0x63,
0xE6, 0x06, 0x30, 0xE3, 0xFC, 0x0F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0048 ('H'): (width = 12, offset = 835) */
0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x7F, 0xE7, 0xFE, 0x60,
0x66, 0x06, 0x60, 0x66, 0x06, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0049 ('I'): (width = 4, offset = 864) */
0x00, 0x06, 0x66, 0x66, 0x66, 0x66, 0x66, 0x60, 0x00, 0x00, 

/* character 0x004A ('J'): (width = 9, offset = 874) */
0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x80, 0xD8, 0x6E, 0x73, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004B ('K'): (width = 12, offset = 896) */
0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x60, 0xC6, 0x18, 0x63, 0x06, 0x60, 0x6F, 0x07, 0xB0, 0x71,
0x86, 0x18, 0x60, 0xC6, 0x0E, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004C ('L'): (width = 10, offset = 925) */
0x00, 0x00, 0x00, 0x01, 0x80, 0x60, 0x18, 0x06, 0x01, 0x80, 0x60, 0x18, 0x06, 0x01, 0x80, 0x60,
0x1F, 0xE7, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004D ('M'): (width = 13, offset = 949) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE7, 0x07, 0x3C, 0x79, 0xE3, 0xCD, 0x16, 0x6D, 0xB3, 0x6D,
0x9B, 0x6C, 0xCE, 0x66, 0x73, 0x33, 0x99, 0x88, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004E ('N'): (width = 12, offset = 980) */
0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x70, 0x67, 0x86, 0x78, 0x66, 0xC6, 0x66, 0x66, 0x66, 0x63,
0x66, 0x1E, 0x61, 0xE6, 0x0E, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x004F ('O'): (width = 12, offset = 1009) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xC3, 0x0C, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60,
0x66, 0x06, 0x30, 0xC3, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0050 ('P'): (width = 11, offset = 1038) */
0x00, 0x00, 0x00, 0x00, 0x3F, 0x87, 0xF8, 0xC3, 0x98, 0x33, 0x0E, 0x7F, 0x8F, 0xE1, 0x80, 0x30,
0x06, 0x00, 0xC0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0051 ('Q'): (width = 12, offset = 1065) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xC3, 0x0C, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60,
0x66, 0x66, 0x33, 0xC3, 0xFC, 0x0E, 0xC0, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0052 ('R'): (width = 12, offset = 1094) */
0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x7F, 0xC6, 0x0E, 0x60, 0x66, 0x0E, 0x7F, 0xC7, 0xF0, 0x63,
0x86, 0x1C, 0x60, 0xC6, 0x0E, 0x60, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0053 ('S'): (width = 11, offset = 1123) */
0x00, 0x00, 0x00, 0x00, 0x0F, 0x83, 0xF8, 0xC3, 0x98, 0x33, 0xC0, 0x3F, 0x01, 0xF0, 0x07, 0x30,
0x67, 0x1C, 0x7F, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0054 ('T'): (width = 10, offset = 1150) */
0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xC3, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C,
0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0055 ('U'): (width = 12, offset = 1174) */
0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60,
0x66, 0x06, 0x70, 0xE3, 0xFC, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0056 ('V'): (width = 11, offset = 1203) */
0x00, 0x00, 0x00, 0x00, 0x60, 0x3C, 0x06, 0xC1, 0x98, 0x31, 0x8C, 0x31, 0x86, 0x30, 0x6C, 0x0D,
0x80, 0xE0, 0x1C, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0057 ('W'): (width = 15, offset = 1230) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1C, 0x3E, 0x38, 0x6C, 0x71, 0x99, 0xB3, 0x33, 0x66, 0x36,
0xD8, 0x6D, 0xB0, 0xDB, 0x60, 0xE3, 0xC1, 0xC7, 0x03, 0x8E, 0x07, 0x1C, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 

/* character 0x0058 ('X'): (width = 11, offset = 1266) */
0x00, 0x00, 0x00, 0x00, 0x30, 0x67, 0x1C, 0x63, 0x06, 0xC0, 0xF8, 0x0E, 0x01, 0xC0, 0x7C, 0x0D,
0x83, 0x18, 0xE3, 0x98, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0059 ('Y'): (width = 10, offset = 1293) */
0x00, 0x00, 0x00, 0x03, 0x03, 0xE1, 0xD8, 0x63, 0x30, 0xCC, 0x1E, 0x03, 0x00, 0xC0, 0x30, 0x0C,
0x03, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005A ('Z'): (width = 9, offset = 1317) */
0x00, 0x00, 0x00, 0x0F, 0xF7, 0xF8, 0x18, 0x18, 0x1C, 0x0C, 0x0C, 0x0E, 0x06, 0x06, 0x07, 0xFF,
0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005B ('['): (width = 5, offset = 1339) */
0x00, 0x00, 0xF7, 0xB1, 0x8C, 0x63, 0x18, 0xC6, 0x31, 0x8C, 0x7B, 0xC0, 

/* character 0x005C ('\'): (width = 4, offset = 1351) */
0x00, 0x0C, 0xCC, 0x66, 0x66, 0x66, 0x33, 0x30, 0x00, 0x00, 

/* character 0x005D (']'): (width = 5, offset = 1361) */
0x00, 0x01, 0xEF, 0x18, 0xC6, 0x31, 0x8C, 0x63, 0x18, 0xC6, 0xF7, 0x80, 

/* character 0x005E ('^'): (width = 9, offset = 1373) */
0x00, 0x00, 0x00, 0x03, 0x03, 0xC1, 0xE1, 0x98, 0xCC, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x005F ('_'): (width = 9, offset = 1395) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 

/* character 0x0060 ('`'): (width = 5, offset = 1417) */
0x00, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0061 ('a'): (width = 9, offset = 1429) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFE, 0x63, 0x07, 0x8F, 0xCE, 0x66, 0x33, 0xF8,
0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0062 ('b'): (width = 10, offset = 1451) */
0x00, 0x00, 0x00, 0x01, 0x80, 0x60, 0x18, 0x06, 0xE1, 0xFC, 0x73, 0x98, 0x66, 0x19, 0x86, 0x73,
0x9F, 0xC6, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0063 ('c'): (width = 9, offset = 1475) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x7E, 0x73, 0x30, 0x18, 0x0C, 0x07, 0x31, 0xF8,
0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0064 ('d'): (width = 10, offset = 1497) */
0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x80, 0x61, 0xD8, 0xFE, 0x73, 0x98, 0x66, 0x19, 0x86, 0x73,
0x8F, 0xE1, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0065 ('e'): (width = 9, offset = 1521) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7C, 0x63, 0x3F, 0x9F, 0xCC, 0x07, 0x31, 0xF0,
0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0066 ('f'): (width = 5, offset = 1543) */
0x00, 0x00, 0x77, 0xB3, 0xFF, 0x63, 0x18, 0xC6, 0x31, 0x80, 0x00, 0x00, 

/* character 0x0067 ('g'): (width = 10, offset = 1555) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xD8, 0xFE, 0x73, 0x98, 0x66, 0x19, 0x86, 0x73,
0x8F, 0xE1, 0xD9, 0x86, 0x7F, 0x8F, 0xC0, 0x00, 

/* character 0x0068 ('h'): (width = 10, offset = 1579) */
0x00, 0x00, 0x00, 0x01, 0x80, 0x60, 0x18, 0x06, 0xF1, 0xFE, 0x71, 0x98, 0x66, 0x19, 0x86, 0x61,
0x98, 0x66, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0069 ('i'): (width = 4, offset = 1603) */
0x00, 0x06, 0x60, 0x66, 0x66, 0x66, 0x66, 0x60, 0x00, 0x00, 

/* character 0x006A ('j'): (width = 4, offset = 1613) */
0x00, 0x06, 0x60, 0x66, 0x66, 0x66, 0x66, 0x66, 0xEC, 0x00, 

/* character 0x006B ('k'): (width = 9, offset = 1623) */
0x00, 0x00, 0x00, 0x0C, 0x06, 0x03, 0x01, 0x8C, 0xCC, 0x6C, 0x3E, 0x1F, 0x0E, 0xC6, 0x63, 0x19,
0x8C, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006C ('l'): (width = 4, offset = 1645) */
0x00, 0x06, 0x66, 0x66, 0x66, 0x66, 0x66, 0x60, 0x00, 0x00, 

/* character 0x006D ('m'): (width = 14, offset = 1655) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xE7, 0x1F, 0xFE, 0x73, 0x99,
0x8C, 0x66, 0x31, 0x98, 0xC6, 0x63, 0x19, 0x8C, 0x66, 0x31, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 

/* character 0x006E ('n'): (width = 10, offset = 1689) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xF1, 0xFE, 0x71, 0x98, 0x66, 0x19, 0x86, 0x61,
0x98, 0x66, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x006F ('o'): (width = 10, offset = 1713) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE0, 0xFC, 0x73, 0x98, 0x66, 0x19, 0x86, 0x73,
0x8F, 0xC1, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0070 ('p'): (width = 10, offset = 1737) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xE1, 0xFC, 0x73, 0x98, 0x66, 0x19, 0x86, 0x73,
0x9F, 0xC6, 0xE1, 0x80, 0x60, 0x18, 0x00, 0x00, 

/* character 0x0071 ('q'): (width = 10, offset = 1761) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xD8, 0xFE, 0x73, 0x98, 0x66, 0x19, 0x86, 0x73,
0x8F, 0xE1, 0xD8, 0x06, 0x01, 0x80, 0x60, 0x00, 

/* character 0x0072 ('r'): (width = 6, offset = 1785) */
0x00, 0x00, 0x00, 0x00, 0x06, 0xDF, 0x71, 0x86, 0x18, 0x61, 0x86, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0073 ('s'): (width = 9, offset = 1800) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFE, 0x63, 0x3C, 0x0F, 0x81, 0xE6, 0x33, 0xF8,
0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0074 ('t'): (width = 5, offset = 1822) */
0x00, 0x00, 0x46, 0x33, 0xFF, 0x63, 0x18, 0xC6, 0x3C, 0xE0, 0x00, 0x00, 

/* character 0x0075 ('u'): (width = 10, offset = 1834) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x19, 0x86, 0x61, 0x98, 0x66, 0x19, 0x86, 0x63,
0x9F, 0xE3, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0076 ('v'): (width = 9, offset = 1858) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8C, 0xC6, 0x63, 0x1B, 0x0D, 0x86, 0xC1, 0xC0, 0xE0,
0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0077 ('w'): (width = 13, offset = 1880) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1C, 0x78, 0xE3, 0x67, 0x33, 0x6D,
0x9B, 0x6C, 0xDB, 0x63, 0x8E, 0x1C, 0x70, 0xE3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0078 ('x'): (width = 9, offset = 1911) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x8C, 0xEE, 0x36, 0x0E, 0x07, 0x03, 0x83, 0x63, 0xB9,
0x8C, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x0079 ('y'): (width = 9, offset = 1933) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x83, 0x63, 0x31, 0x8D, 0x86, 0xC3, 0xE0, 0xE0,
0x70, 0x30, 0x78, 0x38, 0x00, 0x00, 

/* character 0x007A ('z'): (width = 9, offset = 1955) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0xFE, 0x06, 0x07, 0x07, 0x07, 0x03, 0x03, 0xF9,
0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007B ('{'): (width = 6, offset = 1977) */
0x00, 0x00, 0x07, 0x3C, 0xC3, 0x0C, 0x33, 0x8E, 0x0C, 0x30, 0xC3, 0x0C, 0x3C, 0x70, 0x00, 

/* character 0x007C ('|'): (width = 4, offset = 1992) */
0x00, 0x06, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 

/* character 0x007D ('}'): (width = 6, offset = 2002) */
0x00, 0x00, 0x38, 0xF0, 0xC3, 0x0C, 0x30, 0x71, 0xCC, 0x30, 0xC3, 0x0C, 0xF3, 0x80, 0x00, 

/* character 0x007E ('~'): (width = 9, offset = 2017) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xBF, 0xD1, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x007F (''): (width = 5, offset = 2039) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_OFFSET d4dfnt_Arial12_offset_table[] = { 

/*       offset       char      hexcode     */
/*       ======      =====      =======     */
            0,    /*   ' '        0020       */
           10,    /*   '!'        0021       */
           20,    /*   '"'        0022       */
           39,    /*   '#'        0023       */
           61,    /*   '$'        0024       */
           83,    /*   '%'        0025       */
          121,    /*   '&'        0026       */
          150,    /*   '''        0027       */
          160,    /*   '('        0028       */
          172,    /*   ')'        0029       */
          184,    /*   '*'        002A       */
          199,    /*   '+'        002B       */
          221,    /*   ','        002C       */
          231,    /*   '-'        002D       */
          243,    /*   '.'        002E       */
          253,    /*   '/'        002F       */
          263,    /*   '0'        0030       */
          285,    /*   '1'        0031       */
          307,    /*   '2'        0032       */
          329,    /*   '3'        0033       */
          351,    /*   '4'        0034       */
          373,    /*   '5'        0035       */
          395,    /*   '6'        0036       */
          417,    /*   '7'        0037       */
          439,    /*   '8'        0038       */
          461,    /*   '9'        0039       */
          483,    /*   ':'        003A       */
          498,    /*   ';'        003B       */
          513,    /*   '<'        003C       */
          535,    /*   '='        003D       */
          557,    /*   '>'        003E       */
          579,    /*   '?'        003F       */
          603,    /*   '@'        0040       */
          641,    /*   'A'        0041       */
          668,    /*   'B'        0042       */
          697,    /*   'C'        0043       */
          726,    /*   'D'        0044       */
          755,    /*   'E'        0045       */
          782,    /*   'F'        0046       */
          806,    /*   'G'        0047       */
          835,    /*   'H'        0048       */
          864,    /*   'I'        0049       */
          874,    /*   'J'        004A       */
          896,    /*   'K'        004B       */
          925,    /*   'L'        004C       */
          949,    /*   'M'        004D       */
          980,    /*   'N'        004E       */
         1009,    /*   'O'        004F       */
         1038,    /*   'P'        0050       */
         1065,    /*   'Q'        0051       */
         1094,    /*   'R'        0052       */
         1123,    /*   'S'        0053       */
         1150,    /*   'T'        0054       */
         1174,    /*   'U'        0055       */
         1203,    /*   'V'        0056       */
         1230,    /*   'W'        0057       */
         1266,    /*   'X'        0058       */
         1293,    /*   'Y'        0059       */
         1317,    /*   'Z'        005A       */
         1339,    /*   '['        005B       */
         1351,    /*   '\'        005C       */
         1361,    /*   ']'        005D       */
         1373,    /*   '^'        005E       */
         1395,    /*   '_'        005F       */
         1417,    /*   '`'        0060       */
         1429,    /*   'a'        0061       */
         1451,    /*   'b'        0062       */
         1475,    /*   'c'        0063       */
         1497,    /*   'd'        0064       */
         1521,    /*   'e'        0065       */
         1543,    /*   'f'        0066       */
         1555,    /*   'g'        0067       */
         1579,    /*   'h'        0068       */
         1603,    /*   'i'        0069       */
         1613,    /*   'j'        006A       */
         1623,    /*   'k'        006B       */
         1645,    /*   'l'        006C       */
         1655,    /*   'm'        006D       */
         1689,    /*   'n'        006E       */
         1713,    /*   'o'        006F       */
         1737,    /*   'p'        0070       */
         1761,    /*   'q'        0071       */
         1785,    /*   'r'        0072       */
         1800,    /*   's'        0073       */
         1822,    /*   't'        0074       */
         1834,    /*   'u'        0075       */
         1858,    /*   'v'        0076       */
         1880,    /*   'w'        0077       */
         1911,    /*   'x'        0078       */
         1933,    /*   'y'        0079       */
         1955,    /*   'z'        007A       */
         1977,    /*   '{'        007B       */
         1992,    /*   '|'        007C       */
         2002,    /*   '}'        007D       */
         2017,    /*   '~'        007E       */
         2039    /*   ''        007F       */
};

const D4D_FONT_SIZE d4dfnt_Arial12_width_table[] = { 

/*       width        char      hexcode     */
/*       ======      =====      =======     */
           4,    /*   ' '        0020       */
           4,    /*   '!'        0021       */
           8,    /*   '"'        0022       */
           9,    /*   '#'        0023       */
           9,    /*   '$'        0024       */
          16,    /*   '%'        0025       */
          12,    /*   '&'        0026       */
           4,    /*   '''        0027       */
           5,    /*   '('        0028       */
           5,    /*   ')'        0029       */
           6,    /*   '*'        002A       */
           9,    /*   '+'        002B       */
           4,    /*   ','        002C       */
           5,    /*   '-'        002D       */
           4,    /*   '.'        002E       */
           4,    /*   '/'        002F       */
           9,    /*   '0'        0030       */
           9,    /*   '1'        0031       */
           9,    /*   '2'        0032       */
           9,    /*   '3'        0033       */
           9,    /*   '4'        0034       */
           9,    /*   '5'        0035       */
           9,    /*   '6'        0036       */
           9,    /*   '7'        0037       */
           9,    /*   '8'        0038       */
           9,    /*   '9'        0039       */
           6,    /*   ':'        003A       */
           6,    /*   ';'        003B       */
           9,    /*   '<'        003C       */
           9,    /*   '='        003D       */
           9,    /*   '>'        003E       */
          10,    /*   '?'        003F       */
          16,    /*   '@'        0040       */
          11,    /*   'A'        0041       */
          12,    /*   'B'        0042       */
          12,    /*   'C'        0043       */
          12,    /*   'D'        0044       */
          11,    /*   'E'        0045       */
          10,    /*   'F'        0046       */
          12,    /*   'G'        0047       */
          12,    /*   'H'        0048       */
           4,    /*   'I'        0049       */
           9,    /*   'J'        004A       */
          12,    /*   'K'        004B       */
          10,    /*   'L'        004C       */
          13,    /*   'M'        004D       */
          12,    /*   'N'        004E       */
          12,    /*   'O'        004F       */
          11,    /*   'P'        0050       */
          12,    /*   'Q'        0051       */
          12,    /*   'R'        0052       */
          11,    /*   'S'        0053       */
          10,    /*   'T'        0054       */
          12,    /*   'U'        0055       */
          11,    /*   'V'        0056       */
          15,    /*   'W'        0057       */
          11,    /*   'X'        0058       */
          10,    /*   'Y'        0059       */
           9,    /*   'Z'        005A       */
           5,    /*   '['        005B       */
           4,    /*   '\'        005C       */
           5,    /*   ']'        005D       */
           9,    /*   '^'        005E       */
           9,    /*   '_'        005F       */
           5,    /*   '`'        0060       */
           9,    /*   'a'        0061       */
          10,    /*   'b'        0062       */
           9,    /*   'c'        0063       */
          10,    /*   'd'        0064       */
           9,    /*   'e'        0065       */
           5,    /*   'f'        0066       */
          10,    /*   'g'        0067       */
          10,    /*   'h'        0068       */
           4,    /*   'i'        0069       */
           4,    /*   'j'        006A       */
           9,    /*   'k'        006B       */
           4,    /*   'l'        006C       */
          14,    /*   'm'        006D       */
          10,    /*   'n'        006E       */
          10,    /*   'o'        006F       */
          10,    /*   'p'        0070       */
          10,    /*   'q'        0071       */
           6,    /*   'r'        0072       */
           9,    /*   's'        0073       */
           5,    /*   't'        0074       */
          10,    /*   'u'        0075       */
           9,    /*   'v'        0076       */
          13,    /*   'w'        0077       */
           9,    /*   'x'        0078       */
           9,    /*   'y'        0079       */
           9,    /*   'z'        007A       */
           6,    /*   '{'        007B       */
           4,    /*   '|'        007C       */
           6,    /*   '}'        007D       */
           9,    /*   '~'        007E       */
           5     /*   ''        007F       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_32_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL,   // linear / nonlinear , proporcional or not
    ' ',     // start char of used table
    96,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    23,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    d4dfnt_Arial12_offset_table,     // Offset table - used when proporcial font is set in flags
    d4dfnt_Arial12_width_table,        // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_32_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_32_data_table[] = { 

/* character 0x201C (' '): (width = 8, offset = 0) */
0x00, 0x00, 0x00, 0x12, 0x24, 0x24, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 

/* character 0x201D (' '): (width = 8, offset = 19) */
0x00, 0x00, 0x00, 0x6C, 0x6C, 0x24, 0x24, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_48_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_32_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x201C,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    19,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {8, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_32_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_48_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_48_data_table[] = { 

/* character 0x3002 (' '): (width = 11, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x70, 0x1B, 0x03, 0x60, 0x38, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_78_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_48_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x3002,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    27,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {11, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_48_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_78_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_78_data_table[] = { 

/* character 0x4E0D (' '): (width = 16, offset = 0) */
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x30, 0x00, 0x60, 0x00, 0xC0, 0x01, 0x80, 0x03, 0xCC, 0x06, 0xC6,
0x0C, 0xC3, 0x38, 0xC1, 0xE0, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x4E2D (' '): (width = 16, offset = 38) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x7F, 0xFF, 0x60, 0xC1, 0x60, 0xC1, 0x60, 0xC1,
0x60, 0xC1, 0x60, 0xC1, 0x7F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x4E86 (' '): (width = 16, offset = 76) */
0x00, 0x00, 0x7F, 0xFF, 0x00, 0x06, 0x00, 0x0C, 0x00, 0x18, 0x00, 0x30, 0x00, 0xE0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x4EF6 (' '): (width = 16, offset = 114) */
0x0C, 0x18, 0x0D, 0x98, 0x19, 0x98, 0x19, 0x98, 0x33, 0xFF, 0x33, 0x18, 0x73, 0x18, 0xF6, 0x18,
0x30, 0x18, 0x37, 0xFF, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x18,
0x30, 0x18, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_78_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x4E0D,     /*   4E0D        19981            0       */
         0x4E2D,     /*   4E2D        20013            1       */
         0x4E86,     /*   4E86        20102            2       */
         0x4EF6     /*   4EF6        20214            3       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_79_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_78_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x4E0D,     // start char of used table
    4,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_78_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_78_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_79_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_79_data_table[] = { 

/* character 0x4F53 (' '): (width = 16, offset = 0) */
0x0C, 0x30, 0x0C, 0x30, 0x18, 0x30, 0x18, 0x30, 0x3F, 0xFF, 0x30, 0x30, 0x70, 0x78, 0xF0, 0x78,
0x30, 0xFC, 0x30, 0xFC, 0x31, 0xB6, 0x33, 0x33, 0x37, 0xFF, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30,
0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 

/* character 0x4F7F (' '): (width = 16, offset = 38) */
0x18, 0x30, 0x18, 0x30, 0x1F, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x37, 0xFF, 0x76, 0x31, 0x76, 0x31,
0xF6, 0x31, 0x37, 0xFF, 0x30, 0x30, 0x31, 0xB0, 0x30, 0xF0, 0x30, 0x60, 0x30, 0xF8, 0x33, 0x8E,
0x3E, 0x03, 0x00, 0x00, 0x00, 0x00, 

/* character 0x4FDD (' '): (width = 16, offset = 76) */
0x0C, 0x00, 0x0F, 0xFF, 0x1B, 0x03, 0x1B, 0x03, 0x33, 0x03, 0x33, 0x03, 0x73, 0xFF, 0xF0, 0x30,
0x30, 0x30, 0x3F, 0xFF, 0x30, 0x78, 0x30, 0xFC, 0x31, 0xB6, 0x33, 0x33, 0x3E, 0x31, 0x30, 0x30,
0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 

/* character 0x4FE1 (' '): (width = 16, offset = 114) */
0x06, 0x00, 0x06, 0xFF, 0x0C, 0x00, 0x0C, 0x00, 0x1B, 0xFF, 0x18, 0x00, 0x38, 0x00, 0x78, 0xFF,
0xD8, 0x00, 0x18, 0x00, 0x18, 0xFF, 0x18, 0x00, 0x18, 0x00, 0x18, 0xFF, 0x18, 0xC3, 0x18, 0xC3,
0x18, 0xFF, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_79_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x4F53,     /*   4F53        20307            0       */
         0x4F7F,     /*   4F7F        20351            1       */
         0x4FDD,     /*   4FDD        20445            2       */
         0x4FE1     /*   4FE1        20449            3       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_80_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_79_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x4F53,     // start char of used table
    4,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_79_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_79_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_80_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_80_data_table[] = { 

/* character 0x50CF (' '): (width = 16, offset = 0) */
0x0C, 0xC0, 0x0C, 0xFC, 0x19, 0x8C, 0x1B, 0x18, 0x37, 0xFF, 0x3F, 0x31, 0x73, 0x31, 0xF3, 0xFF,
0x30, 0x60, 0x31, 0xF3, 0x37, 0x3E, 0x30, 0x6C, 0x31, 0xDE, 0x37, 0x3F, 0x30, 0xED, 0x37, 0x8C,
0x30, 0x78, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_81_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_80_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x50CF,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_80_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_81_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_81_data_table[] = { 

/* character 0x514B (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06,
0x18, 0x06, 0x1F, 0xFE, 0x03, 0x30, 0x03, 0x30, 0x03, 0x30, 0x06, 0x30, 0x0C, 0x30, 0x38, 0x30,
0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5173 (' '): (width = 16, offset = 38) */
0x00, 0x00, 0x0E, 0x0C, 0x07, 0x1C, 0x03, 0x18, 0x1F, 0xFF, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60,
0x3F, 0xFF, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xF0, 0x01, 0xF8, 0x03, 0x9E, 0x1F, 0x0F, 0x3C, 0x07,
0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5185 (' '): (width = 16, offset = 76) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x7F, 0xFF, 0x60, 0xC1, 0x60, 0xC1, 0x60, 0xC1, 0x61, 0xE1,
0x61, 0xB1, 0x63, 0x19, 0x66, 0x0D, 0x6C, 0x07, 0x78, 0x07, 0x60, 0x01, 0x60, 0x01, 0x60, 0x01,
0x60, 0x0F, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_81_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x514B,     /*   514B        20811            0       */
         0x5173,     /*   5173        20851            1       */
         0x5185     /*   5185        20869            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_83_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_81_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x514B,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_81_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_81_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_83_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_83_data_table[] = { 

/* character 0x53D6 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0xFF, 0xC0, 0x33, 0x00, 0x33, 0xFF, 0x33, 0x61, 0x3F, 0x61, 0x33, 0x61, 0x33, 0x33,
0x33, 0x33, 0x3F, 0x33, 0x33, 0x1E, 0x33, 0x1E, 0x37, 0x0C, 0x3F, 0x1E, 0xF3, 0x33, 0x03, 0x61,
0x03, 0xC0, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_84_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_83_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x53D6,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_83_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_84_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_84_data_table[] = { 

/* character 0x54C7 (' '): (width = 16, offset = 0) */
0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0xFD, 0xFF, 0xCC, 0x18, 0xCC, 0x18, 0xCC, 0x18, 0xCF, 0xFF,
0xCC, 0x00, 0xCC, 0x18, 0xCC, 0x18, 0xFC, 0x18, 0x01, 0xFF, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18,
0x03, 0xFF, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_86_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_84_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x54C7,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_84_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_86_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_86_data_table[] = { 

/* character 0x56FE (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x31, 0xC1, 0x31, 0x99, 0x33, 0xFD, 0x37, 0xB1, 0x3C, 0xF1,
0x30, 0x61, 0x30, 0xF9, 0x33, 0x9F, 0x3E, 0xE3, 0x30, 0x31, 0x31, 0xC1, 0x30, 0x71, 0x30, 0x01,
0x3F, 0xFF, 0x30, 0x01, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_87_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_86_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x56FE,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_86_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_87_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_87_data_table[] = { 

/* character 0x578B (' '): (width = 16, offset = 0) */
0x00, 0x01, 0x7F, 0xD9, 0x1B, 0x19, 0x1B, 0x19, 0x1B, 0x19, 0xFF, 0xF9, 0x1B, 0x19, 0x1B, 0x19,
0x33, 0x01, 0xE3, 0x07, 0x00, 0xC0, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_89_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_87_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x578B,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_87_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_89_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_89_data_table[] = { 

/* character 0x59CB (' '): (width = 16, offset = 0) */
0x30, 0x18, 0x30, 0x18, 0x30, 0x18, 0x30, 0x30, 0xFE, 0x33, 0x36, 0x61, 0x36, 0x6F, 0x67, 0xF8,
0x66, 0x00, 0x66, 0x00, 0xCC, 0x7F, 0xFC, 0x61, 0xDC, 0x61, 0x1E, 0x61, 0x33, 0x61, 0x60, 0x61,
0xC0, 0x7F, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_91_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_89_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x59CB,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_89_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_91_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_91_data_table[] = { 

/* character 0x5B57 (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0x7F, 0xFF, 0x60, 0x01, 0x60, 0x01, 0x6F, 0xFD, 0x00, 0x18, 0x00, 0x30,
0x00, 0xE0, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5B9A (' '): (width = 16, offset = 38) */
0x00, 0xC0, 0x00, 0xC0, 0x7F, 0xFF, 0x60, 0x01, 0x60, 0x01, 0x00, 0x00, 0x3F, 0xFF, 0x00, 0xC0,
0x00, 0xC0, 0x18, 0xC0, 0x18, 0xC0, 0x18, 0xFE, 0x18, 0xC0, 0x3C, 0xC0, 0x36, 0xC0, 0x63, 0xC0,
0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5BB9 (' '): (width = 16, offset = 76) */
0x00, 0xC0, 0x00, 0xC0, 0x7F, 0xFF, 0x60, 0x01, 0x66, 0x19, 0x0C, 0xCC, 0x19, 0xE6, 0x73, 0x33,
0x06, 0x18, 0x0C, 0x0C, 0x38, 0x07, 0xEF, 0xFD, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5BBD (' '): (width = 16, offset = 114) */
0x00, 0x00, 0x01, 0x80, 0x30, 0xC0, 0x3F, 0xFF, 0x63, 0x99, 0x7F, 0xFF, 0x03, 0x30, 0x18, 0x06,
0x0F, 0xFF, 0x0C, 0xC6, 0x0C, 0xE6, 0x0C, 0xC6, 0x0C, 0xF6, 0x0D, 0xE6, 0x19, 0xE1, 0x03, 0x61,
0x0E, 0x71, 0x78, 0x1F, 0x00, 0x00, 

/* character 0x5BF8 (' '): (width = 16, offset = 152) */
0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0xFF, 0xFF, 0x00, 0x0C, 0x00, 0x0C, 0x0C, 0x0C,
0x06, 0x0C, 0x03, 0x0C, 0x03, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C,
0x00, 0x7C, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_91_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x5B57,     /*   5B57        23383            0       */
         0x5B9A,     /*   5B9A        23450            1       */
         0x5BB9,     /*   5BB9        23481            2       */
         0x5BBD,     /*   5BBD        23485            3       */
         0x5BF8     /*   5BF8        23544            4       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_92_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_91_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5B57,     // start char of used table
    5,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_91_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_91_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_92_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_92_data_table[] = { 

/* character 0x5C3A (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x1F, 0xFF, 0x18, 0x03, 0x18, 0x03, 0x18, 0x03, 0x18, 0x03, 0x1F, 0xFF, 0x18, 0x60,
0x18, 0x60, 0x18, 0x60, 0x18, 0x30, 0x30, 0x30, 0x30, 0x18, 0x30, 0x0C, 0x60, 0x06, 0x60, 0x03,
0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5C4F (' '): (width = 16, offset = 38) */
0x00, 0x00, 0x3F, 0xFF, 0x30, 0x01, 0x30, 0x01, 0x3F, 0xFF, 0x30, 0x00, 0x33, 0x06, 0x31, 0x8C,
0x37, 0xFF, 0x31, 0x8C, 0x31, 0x8C, 0x31, 0x8C, 0x3F, 0xFF, 0x61, 0x8C, 0x63, 0x0C, 0x66, 0x0C,
0xDC, 0x0C, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_92_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x5C3A,     /*   5C3A        23610            0       */
         0x5C4F     /*   5C4F        23631            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_94_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_92_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5C3A,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_92_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_92_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_94_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_94_data_table[] = { 

/* character 0x5E93 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0xC0, 0x00, 0x61, 0x3F, 0xFF, 0x30, 0xC0, 0x30, 0xE3, 0x3F, 0xFF, 0x31, 0x80,
0x33, 0x38, 0x33, 0x30, 0x36, 0x37, 0x33, 0xFC, 0x30, 0x31, 0x3F, 0xFF, 0x30, 0x30, 0x60, 0x30,
0x60, 0x30, 0x00, 0x30, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_95_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_94_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5E93,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_94_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_95_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_95_data_table[] = { 

/* character 0x5F00 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x3F, 0xFF, 0x07, 0x1C, 0x07, 0x1C, 0x07, 0x1C, 0x07, 0x1C,
0x7F, 0xFF, 0x7F, 0xFF, 0x07, 0x1C, 0x07, 0x1C, 0x07, 0x1C, 0x0F, 0x1C, 0x1E, 0x1C, 0x3C, 0x1C,
0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x5F55 (' '): (width = 16, offset = 38) */
0x00, 0x00, 0x00, 0x0C, 0x0F, 0xFE, 0x00, 0x0C, 0x00, 0x0C, 0x1F, 0xFC, 0x00, 0x19, 0x7F, 0xFF,
0x00, 0x60, 0x18, 0x63, 0x0E, 0x66, 0x07, 0x7C, 0x00, 0x60, 0x07, 0xFC, 0x3C, 0x67, 0x30, 0x60,
0x07, 0xE0, 0x01, 0x80, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_95_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x5F00,     /*   5F00        24320            0       */
         0x5F55     /*   5F55        24405            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_96_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_95_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x5F00,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_95_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_95_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_96_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_96_data_table[] = { 

/* character 0x606F (' '): (width = 16, offset = 0) */
0x01, 0x80, 0x03, 0x00, 0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C,
0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC, 0x01, 0x83, 0x6C, 0xC1, 0x6C, 0xC6, 0x6C, 0x06,
0xC7, 0xFE, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_98_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_96_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x606F,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_96_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_98_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_98_data_table[] = { 

/* character 0x6253 (' '): (width = 16, offset = 0) */
0x18, 0x00, 0x18, 0x00, 0x19, 0xFF, 0xFF, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06,
0x1F, 0x06, 0x3C, 0x06, 0xF8, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06,
0x78, 0x3E, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_99_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_98_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6253,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_98_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_99_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_99_data_table[] = { 

/* character 0x6301 (' '): (width = 16, offset = 0) */
0x18, 0x18, 0x18, 0x18, 0x19, 0xFF, 0x18, 0x18, 0xFE, 0x18, 0x18, 0x18, 0x1B, 0xFF, 0x18, 0x06,
0x1E, 0x06, 0x3F, 0xFF, 0xF8, 0x06, 0x19, 0x86, 0x18, 0xC6, 0x18, 0x66, 0x18, 0x06, 0x18, 0x06,
0x78, 0x1E, 0x00, 0x00, 0x00, 0x00, 

/* character 0x6309 (' '): (width = 16, offset = 38) */
0x30, 0x18, 0x30, 0x18, 0x33, 0xFF, 0xFF, 0x00, 0x33, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0xFF,
0x3C, 0x66, 0xF0, 0x66, 0x30, 0xC6, 0x30, 0xC6, 0x31, 0xFC, 0x30, 0x1C, 0x30, 0x36, 0x30, 0xE3,
0xF3, 0x81, 0x00, 0x00, 0x00, 0x00, 

/* character 0x6377 (' '): (width = 16, offset = 76) */
0x30, 0x30, 0x30, 0x30, 0x37, 0xFF, 0xFC, 0x30, 0x33, 0xFF, 0x30, 0x33, 0x30, 0x33, 0x37, 0xFF,
0x3C, 0x33, 0xF0, 0x33, 0x33, 0xFF, 0x30, 0x30, 0x33, 0x30, 0x33, 0x3F, 0x37, 0xB0, 0x36, 0xF0,
0xFC, 0x3F, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_99_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x6301,     /*   6301        25345            0       */
         0x6309,     /*   6309        25353            1       */
         0x6377     /*   6377        25463            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_101_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_99_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6301,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_99_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_99_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_101_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_101_data_table[] = { 

/* character 0x652F (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x3F, 0xFC,
0x06, 0x0C, 0x06, 0x18, 0x03, 0x30, 0x01, 0xE0, 0x00, 0xC0, 0x01, 0xE0, 0x07, 0x38, 0x1C, 0x0E,
0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 

/* character 0x6587 (' '): (width = 16, offset = 38) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x18, 0x06, 0x18,
0x03, 0x30, 0x03, 0x30, 0x01, 0xE0, 0x00, 0xC0, 0x01, 0xE0, 0x03, 0x30, 0x0E, 0x1C, 0x38, 0x07,
0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 

/* character 0x65B9 (' '): (width = 16, offset = 76) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xFF,
0x01, 0x83, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x06, 0x03, 0x06, 0x03, 0x0C, 0x03, 0x18, 0x06,
0x70, 0x3C, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_101_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x652F,     /*   652F        25903            0       */
         0x6587,     /*   6587        25991            1       */
         0x65B9     /*   65B9        26041            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_102_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_101_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x652F,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_101_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_101_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_102_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_102_data_table[] = { 

/* character 0x663E (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06,
0x1F, 0xFF, 0x19, 0xB8, 0x03, 0x30, 0x33, 0x33, 0x1B, 0x33, 0x0F, 0x36, 0x0F, 0x3C, 0x03, 0x31,
0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_103_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_102_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x663E,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_102_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_103_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_103_data_table[] = { 

/* character 0x672A (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF,
0x01, 0xE0, 0x03, 0xF0, 0x06, 0xD8, 0x0C, 0xCC, 0x18, 0xC6, 0x30, 0xC3, 0xE0, 0xC1, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 

/* character 0x672C (' '): (width = 16, offset = 38) */
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x01, 0xE0, 0x03, 0xF0, 0x03, 0xF0,
0x06, 0xD8, 0x0C, 0xCC, 0x18, 0xC6, 0x30, 0xC3, 0xEF, 0xFD, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_103_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x672A,     /*   672A        26410            0       */
         0x672C     /*   672C        26412            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_104_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_103_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x672A,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_103_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_103_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_104_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_104_data_table[] = { 

/* character 0x6848 (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0xC3, 0x00, 0xFF, 0xFF, 0x06, 0x0C, 0x0F, 0x98, 0x00, 0xF0,
0x03, 0xDC, 0x3E, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0x01, 0xE0, 0x07, 0xF8, 0x1C, 0xCE, 0xF0, 0xC3,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_108_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_104_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6848,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_104_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_108_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_108_data_table[] = { 

/* character 0x6C49 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x38, 0x01, 0x0F, 0xFF, 0x0D, 0x83, 0x07, 0x83, 0x67, 0x83, 0x36, 0xC6,
0x1C, 0xC6, 0x0C, 0x6C, 0x0C, 0x6C, 0x18, 0x38, 0x18, 0x18, 0x78, 0x38, 0x18, 0x6C, 0x18, 0xC7,
0x1B, 0x83, 0x1E, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_109_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_108_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6C49,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_108_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_109_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_109_data_table[] = { 

/* character 0x6D88 (' '): (width = 16, offset = 0) */
0x60, 0x18, 0x31, 0x99, 0x18, 0xD9, 0x00, 0xDB, 0xC0, 0x18, 0x61, 0xFF, 0x31, 0x81, 0x01, 0x81,
0x01, 0xFF, 0x01, 0x81, 0x19, 0x81, 0x19, 0xFF, 0x31, 0x81, 0x31, 0x81, 0x61, 0x81, 0x61, 0x81,
0xC1, 0x8F, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_112_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_109_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x6D88,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_109_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_112_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_112_data_table[] = { 

/* character 0x7070 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x3F, 0xFF, 0x30, 0x00, 0x30, 0x60, 0x30, 0x60, 0x33, 0x63, 0x33, 0x63, 0x33, 0x66,
0x36, 0x6C, 0x3C, 0xF0, 0x30, 0xF0, 0x31, 0x98, 0x61, 0x98, 0x63, 0x0C, 0x66, 0x06, 0xCC, 0x03,
0xF8, 0x01, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_117_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_112_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7070,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_112_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_117_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_117_data_table[] = { 

/* character 0x7528 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x3F, 0xFF, 0x30, 0xC1, 0x30, 0xC1, 0x30, 0xC1, 0x3F, 0xFF, 0x30, 0xC1, 0x30, 0xC1,
0x30, 0xC1, 0x30, 0xC1, 0x3F, 0xFF, 0x30, 0xC1, 0x30, 0xC1, 0x30, 0xC1, 0x60, 0xC1, 0x60, 0xC1,
0xC0, 0xCF, 0x00, 0x00, 0x00, 0x00, 

/* character 0x7559 (' '): (width = 16, offset = 38) */
0x03, 0x80, 0x3E, 0x7F, 0x30, 0x19, 0x30, 0x19, 0x33, 0x19, 0x33, 0x31, 0x3F, 0xB1, 0xF9, 0xEF,
0x00, 0xC0, 0x00, 0x00, 0x3F, 0xFF, 0x30, 0xC3, 0x30, 0xC3, 0x3F, 0xFF, 0x30, 0xC3, 0x30, 0xC3,
0x3F, 0xFF, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_117_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x7528,     /*   7528        29992            0       */
         0x7559     /*   7559        30041            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_118_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_117_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7528,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_117_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_117_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_118_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_118_data_table[] = { 

/* character 0x76D8 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x01, 0x80, 0x00, 0xE0, 0x0F, 0xFF, 0x0D, 0x86, 0x0C, 0xE6, 0x0C, 0x67, 0x7F, 0xFF,
0x0D, 0x86, 0x0C, 0xE6, 0x18, 0x66, 0x18, 0x1E, 0x70, 0x0F, 0x1F, 0xFF, 0x19, 0xB3, 0x19, 0xB3,
0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 

/* character 0x76EE (' '): (width = 16, offset = 38) */
0x00, 0x00, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x18, 0x06,
0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06, 0x18, 0x06,
0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_118_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x76D8,     /*   76D8        30424            0       */
         0x76EE     /*   76EE        30446            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_119_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_118_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x76D8,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_118_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_118_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_119_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_119_data_table[] = { 

/* character 0x77E5 (' '): (width = 16, offset = 0) */
0x30, 0x00, 0x30, 0x00, 0x7F, 0xBF, 0x6C, 0x31, 0xCC, 0x31, 0x0C, 0x31, 0x0C, 0x31, 0xFF, 0xF1,
0x0C, 0x31, 0x0C, 0x31, 0x0C, 0x31, 0x1E, 0x31, 0x1B, 0x31, 0x31, 0xB1, 0x31, 0xBF, 0x60, 0x00,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_120_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_119_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x77E5,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_119_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_120_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_120_data_table[] = { 

/* character 0x786E (' '): (width = 16, offset = 0) */
0x00, 0x38, 0x00, 0x30, 0x7F, 0x7F, 0x1C, 0x66, 0x1C, 0xEE, 0x18, 0xFF, 0x19, 0xEC, 0x3F, 0xEC,
0x3B, 0x7F, 0x7B, 0x6C, 0x7B, 0x6C, 0x1B, 0x7F, 0x1B, 0x60, 0x1F, 0x60, 0x18, 0xC0, 0x19, 0xC3,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0x78C1 (' '): (width = 16, offset = 38) */
0x00, 0xC6, 0xFE, 0x66, 0x30, 0x6C, 0x37, 0xFF, 0x30, 0xC3, 0x60, 0xC3, 0x61, 0x86, 0x7D, 0x86,
0xEF, 0x7D, 0x6F, 0x6D, 0x6D, 0xC7, 0x6C, 0xC3, 0x6D, 0x86, 0x6D, 0xE7, 0x7F, 0x6D, 0x03, 0xFF,
0x07, 0xBE, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_120_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x786E,     /*   786E        30830            0       */
         0x78C1     /*   78C1        30913            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_121_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_120_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x786E,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_120_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_120_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_121_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_121_data_table[] = { 

/* character 0x793A (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x3F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xC0,
0x00, 0xC0, 0x18, 0xC6, 0x18, 0xC6, 0x30, 0xC3, 0x30, 0xC3, 0x60, 0xC1, 0xC0, 0xC0, 0x00, 0xC0,
0x07, 0xC0, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_124_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_121_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x793A,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_121_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_124_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_124_data_table[] = { 

/* character 0x7C7B (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0xE0, 0x0C, 0xC6, 0x06, 0xCC, 0x06, 0xD9, 0x7F, 0xFF, 0x06, 0xC0, 0x07, 0xDC,
0x1C, 0xC7, 0x70, 0xC3, 0x00, 0xE0, 0x00, 0xC1, 0x7F, 0xFF, 0x00, 0xE0, 0x01, 0xB8, 0x03, 0x0E,
0x0E, 0x03, 0x78, 0x01, 0x00, 0x00, 

/* character 0x7CFB (' '): (width = 16, offset = 38) */
0x00, 0x3F, 0x7F, 0xE0, 0x03, 0x00, 0x1E, 0x18, 0x0C, 0x30, 0x06, 0x66, 0x03, 0xC3, 0x01, 0x81,
0x03, 0x3F, 0xFF, 0xE0, 0x00, 0xC0, 0x0C, 0xCC, 0x0C, 0xCC, 0x18, 0xC6, 0x30, 0xC3, 0x60, 0xC1,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_124_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x7C7B,     /*   7C7B        31867            0       */
         0x7CFB     /*   7CFB        31995            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_126_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_124_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7C7B,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_124_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_124_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_126_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_126_data_table[] = { 

/* character 0x7EA2 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x18, 0x01, 0x19, 0xFF, 0x33, 0x98, 0x36, 0x18, 0x7E, 0x18,
0x6C, 0x18, 0x18, 0x18, 0x30, 0x18, 0x7F, 0x18, 0x60, 0x18, 0x03, 0x18, 0x0E, 0x18, 0x78, 0x19,
0x63, 0xFF, 0x00, 0x00, 0x00, 0x00, 

/* character 0x7EDF (' '): (width = 16, offset = 38) */
0x00, 0x00, 0x00, 0x30, 0x0C, 0x18, 0x18, 0x19, 0x31, 0xFF, 0x37, 0x30, 0x66, 0x63, 0x7C, 0x61,
0x19, 0xFF, 0x19, 0xBD, 0x33, 0x6C, 0x7E, 0x6C, 0x60, 0x6C, 0x03, 0x6C, 0x1E, 0xCC, 0x70, 0xCC,
0x01, 0x8F, 0x07, 0x00, 0x00, 0x00, 

/* character 0x7EFF (' '): (width = 16, offset = 76) */
0x00, 0x00, 0x00, 0x00, 0x1C, 0xFF, 0x18, 0x06, 0x30, 0x06, 0x37, 0xFE, 0x6C, 0x06, 0x7D, 0xFF,
0x18, 0x18, 0x31, 0x99, 0x36, 0xDF, 0x7C, 0x7E, 0x00, 0x7E, 0x07, 0xDB, 0x1F, 0x99, 0x73, 0x19,
0x00, 0xF8, 0x00, 0x30, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_126_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x7EA2,     /*   7EA2        32418            0       */
         0x7EDF,     /*   7EDF        32479            1       */
         0x7EFF     /*   7EFF        32511            2       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_127_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_126_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7EA2,     // start char of used table
    3,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_126_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_126_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_127_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_127_data_table[] = { 

/* character 0x7F6E (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x3F, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x3F, 0xFF, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xC0,
0x0F, 0xFE, 0x6C, 0x06, 0x6F, 0xFE, 0x6C, 0x06, 0x6F, 0xFE, 0x6C, 0x06, 0x6F, 0xFE, 0x60, 0x00,
0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_128_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_127_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x7F6E,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_127_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_128_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_128_data_table[] = { 

/* character 0x80FD (' '): (width = 16, offset = 0) */
0x18, 0x30, 0x1B, 0x30, 0x31, 0xB3, 0x37, 0xFE, 0xFC, 0xF0, 0x00, 0x30, 0x00, 0x30, 0x7F, 0x1F,
0x63, 0x00, 0x63, 0x30, 0x7F, 0x30, 0x63, 0x33, 0x63, 0x3E, 0x7F, 0x30, 0x63, 0x30, 0x63, 0x30,
0x6F, 0x1F, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_130_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_128_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x80FD,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_128_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_130_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_130_data_table[] = { 

/* character 0x8272 (' '): (width = 16, offset = 0) */
0x0C, 0x00, 0x0C, 0x00, 0x1F, 0xF0, 0x30, 0x30, 0x60, 0x60, 0xC0, 0xC0, 0x3F, 0xFF, 0x30, 0xC3,
0x30, 0xC3, 0x30, 0xC3, 0x3F, 0xFF, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00,
0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 

/* character 0x82F1 (' '): (width = 16, offset = 38) */
0x06, 0x18, 0x06, 0x18, 0xFF, 0xFF, 0x06, 0x18, 0x06, 0xD8, 0x00, 0xC0, 0x1F, 0xFE, 0x18, 0xC6,
0x18, 0xC6, 0x18, 0xC6, 0xFF, 0xFF, 0x00, 0xC0, 0x01, 0xE0, 0x03, 0x30, 0x06, 0x18, 0x1C, 0x0E,
0xF0, 0x03, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_130_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x8272,     /*   8272        33394            0       */
         0x82F1     /*   82F1        33521            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_132_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_130_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x8272,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_130_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_130_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_132_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_132_data_table[] = { 

/* character 0x84DD (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x03, 0x18, 0x03, 0x9D, 0x7F, 0xFF, 0x03, 0x18, 0x06, 0x30, 0x37, 0x78, 0x3E, 0x63,
0x36, 0x7E, 0x36, 0xD8, 0x37, 0x8C, 0x06, 0x0C, 0x30, 0x06, 0x1F, 0xFF, 0x19, 0xB3, 0x19, 0xB3,
0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_138_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_132_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x84DD,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_132_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_138_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_138_data_table[] = { 

/* character 0x8A00 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE, 0x18, 0x06, 0x18, 0x06,
0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_139_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_138_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x8A00,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_138_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_139_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_139_data_table[] = { 

/* character 0x8BBE (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0xC0, 0x18, 0x7E, 0x0C, 0x66, 0x0C, 0x66, 0x00, 0xC6, 0x0C, 0xC6, 0x7C, 0xC7,
0x0D, 0x80, 0x0D, 0xFF, 0x0C, 0xC6, 0x0C, 0xC6, 0x0F, 0x6C, 0x0E, 0x3C, 0x0E, 0x18, 0x1C, 0x3E,
0x0C, 0xE3, 0x07, 0x80, 0x00, 0x00, 

/* character 0x8BED (' '): (width = 16, offset = 38) */
0x00, 0x00, 0x00, 0x00, 0x19, 0xFF, 0x0C, 0x18, 0x0C, 0x30, 0x00, 0xFF, 0x0C, 0x33, 0x7C, 0x63,
0x0C, 0x63, 0x0F, 0xFF, 0x0C, 0x00, 0x0C, 0xC1, 0x0C, 0xFF, 0x0F, 0xC3, 0x0E, 0xC3, 0x1C, 0xC3,
0x18, 0xFF, 0x00, 0xC3, 0x00, 0x00

};

const D4D_FONT_IX d4dfnt_Arial12_Page_139_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0x8BBE,     /*   8BBE        35774            0       */
         0x8BED     /*   8BED        35821            1       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_148_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_139_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x8BBE,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_139_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_139_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_148_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_148_data_table[] = { 

/* character 0x94AE (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x30, 0x00, 0x1C, 0xC3, 0x1B, 0x7F, 0x1F, 0x9B, 0x30, 0x1B, 0x63, 0x1B, 0x7F, 0xB3,
0x6C, 0x33, 0x0F, 0xFF, 0x7F, 0xB3, 0x0C, 0x33, 0x0C, 0x33, 0x0C, 0x33, 0x0F, 0x33, 0x0E, 0x33,
0x0D, 0xFF, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_149_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_148_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x94AE,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_148_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_149_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_149_data_table[] = { 

/* character 0x95ED (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x18, 0x00, 0x0D, 0x81, 0x36, 0xFF, 0x3E, 0x01, 0x30, 0x71, 0x30, 0x61, 0x3F, 0xFF,
0x30, 0x61, 0x30, 0xE1, 0x31, 0xE1, 0x33, 0x61, 0x36, 0x61, 0x3C, 0x61, 0x30, 0x61, 0x31, 0xE1,
0x30, 0x0F, 0x30, 0x03, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_152_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_149_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x95ED,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_149_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_152_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_152_data_table[] = { 

/* character 0x9884 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0x01, 0x7F, 0xFF, 0x03, 0x0C, 0x36, 0xD9, 0x1C, 0x7F, 0x0C, 0x61, 0x7F, 0xEF,
0x0F, 0x6D, 0x0F, 0x6D, 0x0E, 0x6D, 0x0C, 0x6D, 0x0C, 0x6D, 0x0C, 0x6D, 0x0C, 0x1E, 0x0C, 0x33,
0x7C, 0x60, 0x1B, 0xC0, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_154_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_152_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x9884,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_152_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_154_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_154_data_table[] = { 

/* character 0x9AD8 (' '): (width = 16, offset = 0) */
0x00, 0xC0, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x0F, 0xFC,
0x00, 0x00, 0x7F, 0xFF, 0x60, 0x01, 0x67, 0xF9, 0x66, 0x19, 0x66, 0x19, 0x67, 0xF9, 0x60, 0x01,
0x60, 0x07, 0x00, 0x00, 0x00, 0x00

};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_255_desc;

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_154_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_LINEAR | D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE,   // linear / nonlinear , proporcional or not
    0x9AD8,     // start char of used table
    1,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    NULL,      // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    NULL,      // Offset table - used when proporcial font is set in flags
    NULL,      // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_154_data_table,      // bitmap/font data array
    &d4dfnt_Arial12_Page_255_desc       // pointer for next UNICODE page
};

const Byte d4dfnt_Arial12_Page_255_data_table[] = { 

/* character 0xFF01 (' '): (width = 16, offset = 0) */
0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

/* character 0xFF0C (' '): (width = 11, offset = 38) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xE0, 0x1C, 0x01, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00

};

const D4D_FONT_OFFSET d4dfnt_Arial12_Page_255_offset_table[] = { 

/*       offset       char      hexcode     */
/*       ======      =====      =======     */
            0,    /*   ' '        FF01       */
           38    /*   ' '        FF0C       */
};

const D4D_FONT_IX d4dfnt_Arial12_Page_255_index_table[] = { 

/*        char      hexacode      decimal       index     */
/*       ======     ========      =======      =======    */
         0xFF01,     /*   FF01        65281            0       */
         0xFF0C     /*   FF0C        65292            1       */
};

const D4D_FONT_SIZE d4dfnt_Arial12_Page_255_width_table[] = { 

/*       width        char      hexcode     */
/*       ======      =====      =======     */
          16,     /*   ' '        FF01       */
          11     /*   ' '        FF0C       */
};

extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial12_Page_255_desc = {
    0,       // Font descriptor version number
    D4D_FONT_FLAGS_IX_STYLE_NONLINEAR | D4D_FONT_FLAGS_IX_TYPE_MAP | D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL,   // linear / nonlinear , proporcional or not
    0xFF01,     // start char of used table
    2,   // count of all chars of used table
    0x0020,     // index of char that will be printed instead of
    12,       // font size (size of font loaded in PC)
    38,     // height of font bitmpap for non proporcional fonts
    15,      // offset from Y0 coordination to baseline
    {16, 19},     // size of biggest char x/y
    D4D_FONT_PACK_BITORDER_BIGEND | D4D_FONT_PACK_SCANBASED_ROW | D4D_FONT_PACK_SCANPREFERRED_ROW | D4D_FONT_PACK_COMPRESSED_ON,     // packing condition of individual bitmaps
    d4dfnt_Arial12_Page_255_index_table,          // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    d4dfnt_Arial12_Page_255_offset_table,     // Offset table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_255_width_table,        // Size table - used when proporcial font is set in flags
    d4dfnt_Arial12_Page_255_data_table,      // bitmap/font data array
    NULL       // pointer for next UNICODE page
};

