
#ifndef PNG_INCLUDED
#define PNG_INCLUDED

/*

  Png format functions header (to use with Image structure)

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <png.h>

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#define png_bytep_NULL (png_bytep)NULL

// *** FUNCTIONS DECLARATION ***

u32 pngLoad (Image **, const char *, int);
u32 pngSave (Image *, const char *);

#endif
