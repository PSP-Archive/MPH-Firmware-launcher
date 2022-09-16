
#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

/*

  Image functions header

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <pspkernel.h>
#include "graphic.h"


// *** MACROS ***

#define imageDraw(image,x,y)					if (image) imageDrawBlock(image,0,0,x,y,image->width,image->height)
#define imageDrawSized(image,x,y,width,height)	if (image) imageDrawBlockSized (image,0,0,image->width,image->height,x,y,width,height)


// *** STRUCTURES ***

typedef struct Image			// Image structure
{
 u32 width;						// Width of image
 u32 height;					// Height of image
 u32 widthA;					// Width aligned
 u32 heightA;					// Height aligned
 GRAPHIC_COLOR_TYPE *bmp;		// Bitmap array
 u8 useVRAM;					// 1 = Bitmap allocated in VRAM, 0 if allocated in RAM (16 aligned)
} Image;


// *** FUNCTIONS DECLARATIONS ***

u32 imageAlignSize (u32);
Image *imageCreate (u32, u32);
void imageDelete (Image *);
Image *imageGetDisplay (void);
u32 imageDrawBlock (Image *, u32, u32, u32, u32, u32, u32);
u32 imageDrawBlockSized (Image *, u32, u32, u32, u32, u32, u32, u32, u32);

u32 imageLoad (Image **, const char *, int);
u32 imageSave (Image *, const char *);


// *** FORMAT FUNCTIONS ***

#include "format/imagebmp.h"
#include "format/imagepng.h"

#endif
