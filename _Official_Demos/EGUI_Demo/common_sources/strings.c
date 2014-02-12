/******************************************************************************
* Includes
******************************************************************************/
#include "d4d.h"
#include "strings.h"

/*******************************************
* String tables - Czech
*******************************************/
static const D4D_WCHAR* strings_czech[] = 
{
    D4D_DEFSTR("Otevrit"),                                  // D4D_STR_OPEN
    D4D_DEFSTR("Otevrit soubor"),                           // D4D_STR_OPENFILE
    D4D_DEFSTR("Zavrit"),                                   // D4D_STR_CLOSE
    D4D_DEFSTR("Zrusit"),                                   // D4D_STR_CANCEL
    D4D_DEFSTR("OK"),                                       // D4D_STR_OK
    D4D_DEFSTR("System"),                                   // D4D_STR_SYSTEM
    D4D_DEFSTR("Adresare"),                                 // D4D_STR_DIRECTORIES
    D4D_DEFSTR("Soubory"),                                  // D4D_STR_FILES
    D4D_DEFSTR("Disk"),                                     // D4D_STR_DISK
    D4D_DEFSTR("Jazyk"),                                    // D4D_STR_LANG
    D4D_DEFSTR("Anglictina"),                               // D4D_STR_LANGEN
    D4D_DEFSTR("Cestina"),                                  // D4D_STR_LENGCZ
    D4D_DEFSTR("Cinstina"),                                 // D4D_STR_LANGCHI
    D4D_DEFSTR("Rezerva1"),                                 // D4D_STR_LANGRES1
    D4D_DEFSTR("Rezerva2"),                                 // D4D_STR_LANGRES2
    D4D_DEFSTR("Rezerva3"),                                 // D4D_STR_LANGRES3
    D4D_DEFSTR("Rezerva4"),                                 // D4D_STR_LANGRES4
    D4D_DEFSTR("Nastaveni"),                                // D4D_STR_SETTINGS
    D4D_DEFSTR("Schema"),                                   // D4D_STR_SCHEME
    D4D_DEFSTR("Sede"),                                     // D4D_STR_SCHEME_GREY
    D4D_DEFSTR("Zelene"),                                   // D4D_STR_SCHEME_GREEN
    D4D_DEFSTR("Modre"),                                    // D4D_STR_SCHEME_BLUE
    D4D_DEFSTR("Cervene"),                                  // D4D_STR_SCHEME_RED
    D4D_DEFSTR("Pismo"),                                    // D4D_STR_FONTS
    D4D_DEFSTR("Courier"),                                  // D4D_STR_FONTS_COURIER
    D4D_DEFSTR("Arial"),                                    // D4D_STR_FONTS_ARIAL
    D4D_DEFSTR("Sporic obrazovky"),                         // D4D_STR_SCRSVR
    D4D_DEFSTR("Povolit"),                                  // D4D_STR_ENABLE
    D4D_DEFSTR("Obsah souboru. Pouzij tlacitko OTEVRIT!"),  // D4D_STR_DUMMY_LABEL
    D4D_DEFSTR("Nejde zobrazit!"),                          // D4D_STR_COULD_NOT_SHOW
    D4D_DEFSTR("Informace"),                                // D4D_STR_IMAGE_INFO
    D4D_DEFSTR("Typ:"),                                     // D4D_STR_TYPE
    D4D_DEFSTR("Sirka:"),                                   // D4D_STR_WIDTH
    D4D_DEFSTR("Vyska:"),                                   // D4D_STR_HEIGHT
    D4D_DEFSTR("Velikost:"),                                // D4D_STR_SIZE
    D4D_DEFSTR("Informace"),                                // D4D_STR_TEXT_INFO
    D4D_DEFSTR("ASCII"),                                    // D4D_STR_ASCII
    D4D_DEFSTR("Unicode"),                                  // D4D_STR_UNICODE
    D4D_DEFSTR("Informace"),                                // D4D_STR_INFO
    D4D_DEFSTR("Neznama")                                   // D4D_STR_UNKNOWN
};

/*******************************************
* String tables - Chinese_BIG5
********************************************/
static const D4D_WCHAR string_chinese_big5_D4D_STR_OPEN[] = {0x6253, 0x5F00, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_OPENFILE[] = {0x6253, 0x5F00, 0x6587, 0x4EF6, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_CLOSE[] = {0x5173, 0x95ED, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_CANCEL[] = {0x53D6, 0x6D88, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_OK[] = {0x786E, 0x5B9A, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SYSTEM[] = {0x7CFB, 0x7EDF, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_DIRECTORIES[] = {0x76EE, 0x5F55, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_FILES[] = {0x6587, 0x4EF6, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_DISK[] = {0x78C1, 0x76D8, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_LANG[] = {0x0030, 0x8BED, 0x8A00, 0x0000};// change
static const D4D_WCHAR string_chinese_big5_D4D_STR_LANGEN[] = {0x82F1, 0x8BED, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_LENGCZ[] = {0x6377, 0x514B, 0x8BED, 0x0000};
const D4D_WCHAR string_chinese_big5_D4D_STR_LANGCHI[] = {0x6C49, 0x8BED, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_LANGRES1[] = {0x9884, 0x7559, 0x0031, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_LANGRES2[] = {0x9884, 0x7559, 0x0032, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_LANGRES3[] = {0x9884, 0x7559, 0x0033, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_LANGRES4[] = {0x9884, 0x7559, 0x0034, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SETTINGS[] = {0x8BBE, 0x7F6E, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SCHEME[] = {0x65B9, 0x6848, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SCHEME_GREY[] = {0x7070, 0x8272, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SCHEME_GREEN[] = {0x7EFF, 0x8272, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SCHEME_BLUE[] = {0x84DD, 0x8272, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SCHEME_RED[] = {0x7EA2, 0x8272, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_FONTS[] = {0x5B57, 0x4F53, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SCRSVR[] = {0x5C4F, 0x4FDD, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_ENABLE[] = {0x4F7F, 0x80FD, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_DUMMY_LABEL[] = {0x6587, 0x4EF6, 0x5185, 0x5BB9, 0x3002, 0x0020, 0x4F7F, 0x7528, 0x201C, 0x6253, 0x5F00, 0x201D, 0x6309, 0x94AE, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_COULD_NOT_SHOW[] = {0x4E0D, 0x80FD, 0x663E, 0x793A, 0xFF01, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_IMAGE_INFO[] = {0x56FE, 0x50CF, 0x4FE1, 0x606F, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_TYPE[] = {0x7C7B, 0x578B, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_WIDTH[] = {0x5BBD, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_HEIGHT[] = {0x9AD8, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_SIZE[] = {0x5C3A, 0x5BF8, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_TEXT_INFO[] = {0x6587, 0x672C, 0x4FE1, 0x606F, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_INFO[] = {0x4FE1, 0x606F, 0x0000};
static const D4D_WCHAR string_chinese_big5_D4D_STR_UNKNOWN[] = {0x672A, 0x77E5, 0x0000};
static const D4D_WCHAR* strings_chinese_big5[] = 
{
    string_chinese_big5_D4D_STR_OPEN,     // D4D_STR_OPEN
    string_chinese_big5_D4D_STR_OPENFILE,     // D4D_STR_OPENFILE
    string_chinese_big5_D4D_STR_CLOSE,     // D4D_STR_CLOSE
    string_chinese_big5_D4D_STR_CANCEL,     // D4D_STR_CANCEL
    string_chinese_big5_D4D_STR_OK,     // D4D_STR_OK
    string_chinese_big5_D4D_STR_SYSTEM,     // D4D_STR_SYSTEM
    string_chinese_big5_D4D_STR_DIRECTORIES,     // D4D_STR_DIRECTORIES
    string_chinese_big5_D4D_STR_FILES,     // D4D_STR_FILES
    string_chinese_big5_D4D_STR_DISK,     // D4D_STR_DISK
    string_chinese_big5_D4D_STR_LANG,     // D4D_STR_LANG
    string_chinese_big5_D4D_STR_LANGEN,     // D4D_STR_LANGEN
    string_chinese_big5_D4D_STR_LENGCZ,     // D4D_STR_LENGCZ
    string_chinese_big5_D4D_STR_LANGCHI,     // D4D_STR_LANGCHI
    string_chinese_big5_D4D_STR_LANGRES1,     // D4D_STR_LANGRES1
    string_chinese_big5_D4D_STR_LANGRES2,     // D4D_STR_LANGRES2
    string_chinese_big5_D4D_STR_LANGRES3,     // D4D_STR_LANGRES3
    string_chinese_big5_D4D_STR_LANGRES4,     // D4D_STR_LANGRES4
    string_chinese_big5_D4D_STR_SETTINGS,     // D4D_STR_SETTINGS
    string_chinese_big5_D4D_STR_SCHEME,     // D4D_STR_SCHEME
    string_chinese_big5_D4D_STR_SCHEME_GREY,     // D4D_STR_SCHEME_GREY
    string_chinese_big5_D4D_STR_SCHEME_GREEN,     // D4D_STR_SCHEME_GREEN
    string_chinese_big5_D4D_STR_SCHEME_BLUE,     // D4D_STR_SCHEME_BLUE
    string_chinese_big5_D4D_STR_SCHEME_RED,     // D4D_STR_SCHEME_RED
    string_chinese_big5_D4D_STR_FONTS,     // D4D_STR_FONTS
    D4D_DEFSTR("Courier"),                                  // D4D_STR_FONTS_COURIER
    D4D_DEFSTR("Arial"),                                    // D4D_STR_FONTS_ARIAL
    string_chinese_big5_D4D_STR_SCRSVR,     // D4D_STR_SCRSVR
    string_chinese_big5_D4D_STR_ENABLE,     // D4D_STR_ENABLE
    string_chinese_big5_D4D_STR_DUMMY_LABEL,     // D4D_STR_DUMMY_LABEL
    string_chinese_big5_D4D_STR_COULD_NOT_SHOW,     // D4D_STR_COULD_NOT_SHOW
    string_chinese_big5_D4D_STR_IMAGE_INFO,     // D4D_STR_IMAGE_INFO
    string_chinese_big5_D4D_STR_TYPE,     // D4D_STR_TYPE
    string_chinese_big5_D4D_STR_WIDTH,     // D4D_STR_WIDTH
    string_chinese_big5_D4D_STR_HEIGHT,     // D4D_STR_HEIGHT
    string_chinese_big5_D4D_STR_SIZE,     // D4D_STR_SIZE
    string_chinese_big5_D4D_STR_TEXT_INFO,     // D4D_STR_TEXT_INFO
    D4D_DEFSTR("ASCII"),                                    // D4D_STR_ASCII
    D4D_DEFSTR("UniCode"),                                  // D4D_STR_UNICODE
    string_chinese_big5_D4D_STR_INFO,     // D4D_STR_INFO
    string_chinese_big5_D4D_STR_UNKNOWN     // D4D_STR_UNKNOWN
};

/*******************************************
* String tables - English
********************************************/
static const D4D_WCHAR* strings_english[] = 
{
    D4D_DEFSTR("Open"),                                     // D4D_STR_OPEN
    D4D_DEFSTR("Open File"),                                // D4D_STR_OPENFILE
    D4D_DEFSTR("Close"),                                    // D4D_STR_CLOSE
    D4D_DEFSTR("Cancel"),                                   // D4D_STR_CANCEL
    D4D_DEFSTR("OK"),                                       // D4D_STR_OK
    D4D_DEFSTR("System"),                                   // D4D_STR_SYSTEM
    D4D_DEFSTR("Directories"),                              // D4D_STR_DIRECTORIES
    D4D_DEFSTR("Files"),                                    // D4D_STR_FILES
    D4D_DEFSTR("Disk"),                                     // D4D_STR_DISK
    D4D_DEFSTR("Language"),                                 // D4D_STR_LANG
    D4D_DEFSTR("English"),                                  // D4D_STR_LANGEN
    D4D_DEFSTR("Czech"),                                    // D4D_STR_LENGCZ
    D4D_DEFSTR("Chinesse"),                                 // D4D_STR_LANGCHI
    D4D_DEFSTR("Reserve1"),                                 // D4D_STR_LANGRES1
    D4D_DEFSTR("Reserve2"),                                 // D4D_STR_LANGRES2
    D4D_DEFSTR("Reserve3"),                                 // D4D_STR_LANGRES3
    D4D_DEFSTR("Reserve4"),                                 // D4D_STR_LANGRES4
    D4D_DEFSTR("Settings"),                                 // D4D_STR_SETTINGS
    D4D_DEFSTR("Scheme"),                                   // D4D_STR_SCHEME
    D4D_DEFSTR("Grey"),                                     // D4D_STR_SCHEME_GREY
    D4D_DEFSTR("Green"),                                    // D4D_STR_SCHEME_GREEN
    D4D_DEFSTR("Blue"),                                     // D4D_STR_SCHEME_BLUE
    D4D_DEFSTR("Red"),                                      // D4D_STR_SCHEME_RED
    D4D_DEFSTR("Fonts"),                                    // D4D_STR_FONTS
    D4D_DEFSTR("Courier"),                                  // D4D_STR_FONTS_COURIER
    D4D_DEFSTR("Arial"),                                    // D4D_STR_FONTS_ARIAL
    D4D_DEFSTR("Screen saver"),                             // D4D_STR_SCRSVR
    D4D_DEFSTR("Enable"),                                   // D4D_STR_ENABLE
    D4D_DEFSTR("File Content. Use OPEN button!"),           // D4D_STR_DUMMY_LABEL
    D4D_DEFSTR("Could not show!"),                          // D4D_STR_COULD_NOT_SHOW
    D4D_DEFSTR("Image info"),                               // D4D_STR_IMAGE_INFO
    D4D_DEFSTR("Type:"),                                    // D4D_STR_TYPE
    D4D_DEFSTR("Width:"),                                   // D4D_STR_WIDTH
    D4D_DEFSTR("Height:"),                                  // D4D_STR_HEIGHT
    D4D_DEFSTR("Size:"),                                    // D4D_STR_SIZE
    D4D_DEFSTR("Text Info"),                                // D4D_STR_TEXT_INFO
    D4D_DEFSTR("ASCII"),                                    // D4D_STR_ASCII
    D4D_DEFSTR("Unicode"),                                  // D4D_STR_UNICODE
    D4D_DEFSTR("Info"),                                     // D4D_STR_INFO
    D4D_DEFSTR("Unknown")                                   // D4D_STR_UNKNOWN
};

/*******************************************
* Table of string tables
*******************************************/
D4D_DECLARE_STRING_TABLE_BEGIN
    D4D_DECLARE_STRING(STRING_LANG_CZECH, strings_czech)
    D4D_DECLARE_STRING(STRING_LANG_CHINESE_BIG5, strings_chinese_big5)
    D4D_DECLARE_STRING(STRING_LANG_ENGLISH, strings_english)
D4D_DECLARE_STRING_TABLE_END
