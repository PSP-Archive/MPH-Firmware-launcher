
/*

  Bitmap font functions source

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "font.h"


// *** GLOBALS VARIABLES ***

FontType *fontType = NULL;


// *** FUNCTIONS ***

u32 fontInit (FontType *font)

/*

  Initialize the font functions

  Parameters :	font		->	Pointer to the font to copy

  Return :		0			->	OK
				1			->	Already initialized
				2			->	Bad parameters
				3			->	Unable to allocate memory

*/

{
 // Verify if already initialized
 if (fontType) return 1;

 // Verify parameters
 if (!(font)) return 2;

 // Allocate fontType
 fontType = (FontType *) malloc(sizeof(FontType));
 if (!(fontType)) return 3;

 memcpy(fontType,font,sizeof(FontType));

 return 0;
}

u32 fontTerm (void)

/*

  Terminate the font functions

  Parameters :	None

  Return :		0			->	OK
				1			->	Not initialized

*/

{
 // Verify if initialized
 if (!(fontType)) return 1;

 // Free memory
 free(fontType);
 fontType = NULL;

 return 0;
}

u32 fontPrint (const char *string, FontOption *option, u32 x, u32 y/*, GRAPHIC_COLOR_TYPE c*/)

/*

  Print a string with current font

  Parameters :	string		->	String to print
				option		->	Pointer to option font structure
				x			->	X location of the string
				y			->	Y location of the string

  Return :		Number of caracters print or 0 if error

*/

{
 char *ptr;
 u32 xf, yf;


 // Verify if initialized
 if (!(fontType)) return 0;

 // Verify parameters
 if ((!(string)) || (!(option))) return 0;

 // Initialize pointer to first caracter
 ptr = (char *) string;

 // Caracter loop
 while ((*ptr) && (x < (GRAPHIC_SCR_WIDTH - option->width)))
 {
  // Test if caracter is available in font
  if (((*ptr) >= fontType->begin) && ((*ptr) <= fontType->end))
  {
   // Find coordinates of caracter in font
   xf = ((((*ptr) - fontType->begin) % fontType->carrow) * fontType->incx) + fontType->border;
   yf = ((((*ptr) - fontType->begin) / fontType->carrow) * fontType->incy) + fontType->border;

   // Draw caracter
   if ((option->width == fontType->width) && (option->height == fontType->height))
	imageDrawBlock(fontType->font,xf,yf,x,y,option->width,option->height);
   else
	imageDrawBlockSized(fontType->font,xf,yf,fontType->width,fontType->height,x,y,option->width,option->height);

   // Update x coordinate
   x += (option->width + option->space);
  }

  // Go to the next caracter
  ptr++;
 }

 return (ptr - string);
}
