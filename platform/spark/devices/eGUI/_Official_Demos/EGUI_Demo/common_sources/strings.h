#ifndef __STRINGS_H
#define __STRINGS_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/
#define D4D_STRINGTABLE_PREFIX '@'

// String table names
#define STRING_LANG_CZECH                    0
#define STRING_LANG_CHINESE_BIG5             1
#define STRING_LANG_ENGLISH                  2

// String names
#define D4D_STR_OPEN                          D4D_DEFSTR("@00")
#define D4D_STR_OPENFILE                      D4D_DEFSTR("@01")
#define D4D_STR_CLOSE                         D4D_DEFSTR("@02")
#define D4D_STR_CANCEL                        D4D_DEFSTR("@03")
#define D4D_STR_OK                            D4D_DEFSTR("@04")
#define D4D_STR_SYSTEM                        D4D_DEFSTR("@05")
#define D4D_STR_DIRECTORIES                   D4D_DEFSTR("@06")
#define D4D_STR_FILES                         D4D_DEFSTR("@07")
#define D4D_STR_DISK                          D4D_DEFSTR("@08")
#define D4D_STR_LANG                          D4D_DEFSTR("@09")
#define D4D_STR_LANGEN                        D4D_DEFSTR("@10")
#define D4D_STR_LANGCZ                        D4D_DEFSTR("@11")
#define D4D_STR_LANGCHI                       D4D_DEFSTR("@12")
#define D4D_STR_LANGRES1                      D4D_DEFSTR("@13")
#define D4D_STR_LANGRES2                      D4D_DEFSTR("@14")
#define D4D_STR_LANGRES3                      D4D_DEFSTR("@15")
#define D4D_STR_LANGRES4                      D4D_DEFSTR("@16")
#define D4D_STR_SETTINGS                      D4D_DEFSTR("@17")
#define D4D_STR_SCHEME                        D4D_DEFSTR("@18")
#define D4D_STR_SCHEME_GREY                   D4D_DEFSTR("@19")
#define D4D_STR_SCHEME_GREEN                  D4D_DEFSTR("@20")
#define D4D_STR_SCHEME_BLUE                   D4D_DEFSTR("@21")
#define D4D_STR_SCHEME_RED                    D4D_DEFSTR("@22")
#define D4D_STR_FONTS                         D4D_DEFSTR("@23")
#define D4D_STR_FONTS_COURIER                 D4D_DEFSTR("@24")
#define D4D_STR_FONTS_ARIAL                   D4D_DEFSTR("@25")
#define D4D_STR_SCRSVR                        D4D_DEFSTR("@26")
#define D4D_STR_ENABLE                        D4D_DEFSTR("@27")
#define D4D_STR_DUMMY_LABEL                   D4D_DEFSTR("@28")
#define D4D_STR_COULD_NOT_SHOW                D4D_DEFSTR("@29")
#define D4D_STR_IMAGE_INFO                    D4D_DEFSTR("@30")
#define D4D_STR_TYPE                          D4D_DEFSTR("@31")
#define D4D_STR_WIDTH                         D4D_DEFSTR("@32")
#define D4D_STR_HEIGHT                        D4D_DEFSTR("@33")
#define D4D_STR_SIZE                          D4D_DEFSTR("@34")
#define D4D_STR_TEXT_INFO                     D4D_DEFSTR("@35")
#define D4D_STR_ASCII                         D4D_DEFSTR("@36")
#define D4D_STR_UNICODE                       D4D_DEFSTR("@37")
#define D4D_STR_INFO                          D4D_DEFSTR("@38")
#define D4D_STR_UNKNOWN                       D4D_DEFSTR("@39")

/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
extern const D4D_WCHAR string_chinese_big5_D4D_STR_LANGCHI[];
/******************************************************************************
* Global functions
******************************************************************************/


#endif