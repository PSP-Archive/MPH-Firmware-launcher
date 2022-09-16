
#ifndef FONT_INCLUDED
#define FONT_INCLUDED

/*

  Bitmap font functions header

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "image.h"


// *** STRUCTURES ***

typedef struct FontType				// Font type
{
 Image *font;						// Image of font
 u32 border;						// Border for first caracter in a row (in pixels)
 u32 incx;							// Increment X for find next caracter (in pixels)
 u32 incy;							// Increment Y for find next caracter row (in pixels)
 u32 carrow;						// Number of caracter in a row
 u32 width;							// Width of a caracter
 u32 height;						// Height of a caracter
 u32 begin;							// ASCII code of begin caracter
 u32 end;							// ASCII code of end caracter
} FontType;

typedef struct FontOption
{
 u32 width;							// Width of a caracter
 u32 height;						// Height of a caracter
 u32 space;							// Space between two caracters (in pixels)
} FontOption;


// *** FUNCTIONS DECLARATIONS ***

u32 fontInit (FontType *);															// Initialize the font functions
u32 fontTerm (void);																// Terminate the font functions
u32 fontPrint (const char *, FontOption *, u32, u32/*, GRAPHIC_COLOR_TYPE*/);		// Print a string with current font

#endif
