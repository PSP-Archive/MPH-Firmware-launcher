
#ifndef PNG_INCLUDED
#define PNG_INCLUDED

/*

  Png format functions header (to use with Image structure)

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <png.h>


// *** FUNCTIONS DECLARATION ***

u32 pngLoad (Image **, const char *, int);
u32 pngSave (Image *, const char *);

#endif
