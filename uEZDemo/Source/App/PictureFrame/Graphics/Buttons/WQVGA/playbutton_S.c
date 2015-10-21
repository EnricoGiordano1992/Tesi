/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap converter for emWin V5.12.                           *
*        Compiled Jun 29 2011, 15:28:21                              *
*        (C) 1998 - 2010 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: playbutton_S                                          *
* Dimensions:  43 * 43                                               *
* NumColors:   3                                                     *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

static GUI_CONST_STORAGE GUI_COLOR Colorsplaybutton_S[] = {
     0xFFFFFF,0x00FF00,0x808080
};

static GUI_CONST_STORAGE GUI_LOGPALETTE Palplaybutton_S = {
  3,	/* number of entries */
  1, 	/* Has transparency */
  &Colorsplaybutton_S[0]
};

static GUI_CONST_STORAGE unsigned char acplaybutton_S[] = {
  /* RLE: 228 Pixels @ 000,000*/ 228, 0x00, 
  /* RLE: 005 Pixels @ 013,005*/ 5, 0x01, 
  /* RLE: 038 Pixels @ 018,005*/ 38, 0x00, 
  /* RLE: 005 Pixels @ 013,006*/ 5, 0x01, 
  /* RLE: 038 Pixels @ 018,006*/ 38, 0x00, 
  /* RLE: 006 Pixels @ 013,007*/ 6, 0x01, 
  /* RLE: 037 Pixels @ 019,007*/ 37, 0x00, 
  /* RLE: 008 Pixels @ 013,008*/ 8, 0x01, 
  /* RLE: 035 Pixels @ 021,008*/ 35, 0x00, 
  /* RLE: 008 Pixels @ 013,009*/ 8, 0x01, 
  /* RLE: 035 Pixels @ 021,009*/ 35, 0x00, 
  /* RLE: 009 Pixels @ 013,010*/ 9, 0x01, 
  /* RLE: 034 Pixels @ 022,010*/ 34, 0x00, 
  /* RLE: 010 Pixels @ 013,011*/ 10, 0x01, 
  /* RLE: 033 Pixels @ 023,011*/ 33, 0x00, 
  /* RLE: 010 Pixels @ 013,012*/ 10, 0x01, 
  /* RLE: 033 Pixels @ 023,012*/ 33, 0x00, 
  /* RLE: 013 Pixels @ 013,013*/ 13, 0x01, 
  /* RLE: 030 Pixels @ 026,013*/ 30, 0x00, 
  /* RLE: 013 Pixels @ 013,014*/ 13, 0x01, 
  /* RLE: 030 Pixels @ 026,014*/ 30, 0x00, 
  /* RLE: 014 Pixels @ 013,015*/ 14, 0x01, 
  /* RLE: 029 Pixels @ 027,015*/ 29, 0x00, 
  /* RLE: 016 Pixels @ 013,016*/ 16, 0x01, 
  /* RLE: 027 Pixels @ 029,016*/ 27, 0x00, 
  /* RLE: 016 Pixels @ 013,017*/ 16, 0x01, 
  /* RLE: 027 Pixels @ 029,017*/ 27, 0x00, 
  /* RLE: 017 Pixels @ 013,018*/ 17, 0x01, 
  /* RLE: 026 Pixels @ 030,018*/ 26, 0x00, 
  /* RLE: 018 Pixels @ 013,019*/ 18, 0x01, 
  /* RLE: 025 Pixels @ 031,019*/ 25, 0x00, 
  /* RLE: 018 Pixels @ 013,020*/ 18, 0x01, 
  /* RLE: 025 Pixels @ 031,020*/ 25, 0x00, 
  /* RLE: 019 Pixels @ 013,021*/ 19, 0x01, 
  /* RLE: 024 Pixels @ 032,021*/ 24, 0x00, 
  /* RLE: 019 Pixels @ 013,022*/ 19, 0x01, 
  /* ABS: 002 Pixels @ 032,022*/ 0, 2, 0x02, 0x02, 
  /* RLE: 022 Pixels @ 034,022*/ 22, 0x00, 
  /* RLE: 018 Pixels @ 013,023*/ 18, 0x01, 
  /* RLE: 003 Pixels @ 031,023*/ 3, 0x02, 
  /* RLE: 022 Pixels @ 034,023*/ 22, 0x00, 
  /* RLE: 017 Pixels @ 013,024*/ 17, 0x01, 
  /* ABS: 002 Pixels @ 030,024*/ 0, 2, 0x02, 0x02, 
  /* RLE: 024 Pixels @ 032,024*/ 24, 0x00, 
  /* RLE: 017 Pixels @ 013,025*/ 17, 0x01, 
  /* ABS: 002 Pixels @ 030,025*/ 0, 2, 0x02, 0x02, 
  /* RLE: 024 Pixels @ 032,025*/ 24, 0x00, 
  /* RLE: 016 Pixels @ 013,026*/ 16, 0x01, 
  /* ABS: 002 Pixels @ 029,026*/ 0, 2, 0x02, 0x02, 
  /* RLE: 025 Pixels @ 031,026*/ 25, 0x00, 
  /* RLE: 014 Pixels @ 013,027*/ 14, 0x01, 
  /* RLE: 003 Pixels @ 027,027*/ 3, 0x02, 
  /* RLE: 026 Pixels @ 030,027*/ 26, 0x00, 
  /* RLE: 014 Pixels @ 013,028*/ 14, 0x01, 
  /* RLE: 003 Pixels @ 027,028*/ 3, 0x02, 
  /* RLE: 026 Pixels @ 030,028*/ 26, 0x00, 
  /* RLE: 013 Pixels @ 013,029*/ 13, 0x01, 
  /* RLE: 003 Pixels @ 026,029*/ 3, 0x02, 
  /* RLE: 027 Pixels @ 029,029*/ 27, 0x00, 
  /* RLE: 011 Pixels @ 013,030*/ 11, 0x01, 
  /* RLE: 003 Pixels @ 024,030*/ 3, 0x02, 
  /* RLE: 029 Pixels @ 027,030*/ 29, 0x00, 
  /* RLE: 010 Pixels @ 013,031*/ 10, 0x01, 
  /* RLE: 003 Pixels @ 023,031*/ 3, 0x02, 
  /* RLE: 030 Pixels @ 026,031*/ 30, 0x00, 
  /* RLE: 009 Pixels @ 013,032*/ 9, 0x01, 
  /* ABS: 002 Pixels @ 022,032*/ 0, 2, 0x02, 0x02, 
  /* RLE: 032 Pixels @ 024,032*/ 32, 0x00, 
  /* RLE: 009 Pixels @ 013,033*/ 9, 0x01, 
  /* ABS: 002 Pixels @ 022,033*/ 0, 2, 0x02, 0x02, 
  /* RLE: 032 Pixels @ 024,033*/ 32, 0x00, 
  /* RLE: 008 Pixels @ 013,034*/ 8, 0x01, 
  /* ABS: 002 Pixels @ 021,034*/ 0, 2, 0x02, 0x02, 
  /* RLE: 033 Pixels @ 023,034*/ 33, 0x00, 
  /* RLE: 006 Pixels @ 013,035*/ 6, 0x01, 
  /* RLE: 003 Pixels @ 019,035*/ 3, 0x02, 
  /* RLE: 034 Pixels @ 022,035*/ 34, 0x00, 
  /* RLE: 006 Pixels @ 013,036*/ 6, 0x01, 
  /* RLE: 003 Pixels @ 019,036*/ 3, 0x02, 
  /* RLE: 034 Pixels @ 022,036*/ 34, 0x00, 
  /* RLE: 005 Pixels @ 013,037*/ 5, 0x01, 
  /* RLE: 003 Pixels @ 018,037*/ 3, 0x02, 
  /* RLE: 037 Pixels @ 021,037*/ 37, 0x00, 
  /* RLE: 004 Pixels @ 015,038*/ 4, 0x02, 
  /* RLE: 196 Pixels @ 019,038*/ 196, 0x00, 


  0};  /* 184 bytes for 1849 pixels */

GUI_CONST_STORAGE GUI_BITMAP bmplaybutton_S = {
  43, /* XSize */
  43, /* YSize */
  43, /* BytesPerLine */
  GUI_COMPRESS_RLE8, /* BitsPerPixel */
  acplaybutton_S,  /* Pointer to picture data (indices) */
  &Palplaybutton_S,  /* Pointer to palette */
  GUI_DRAW_RLE8
};

/* *** End of file *** */
