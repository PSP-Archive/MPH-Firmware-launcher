
/*

  Image functions source

  By MPH (mphtheone@hotmail.com)

*/


// INCLUDES

#include "image.h"


// GLOBAL VARIABLES

extern void *graphicTextureOpt;											// For optimize texture render (in graphic.c)
Image imageDisplay = { 480, 272, 512, 272, GRAPHIC_VRAM_ADDRESS , 1 };	// Image of current display


// FUNCTIONS

u32 imageAlignSize (u32 n)

/*

  Align a number to the next power of 2

  Parameters :	n			->	Number to align

  Return :		Number aligned

*/

{
 int x;


 // Test if already aligned
 if (((~n) & (n - 1)) == (n - 1)) return n;

 // Align
 for (x=0;n!=0;x++) n >>= 1;
 n = 1 << x;

 return n;
}

Image *imageCreate (u32 width, u32 height)

/*

  Create an image structure

  Parameters :	width		->	Width of image
				height		->	Height of image

  Return :		Pointer to the image structure created

*/

{
 Image *image;


 // Allocate new image
 image = (Image *) malloc(sizeof(Image));
 if (!(image)) return NULL;

 // Init new image
 image->width = width;
 image->height = height;
 image->widthA = imageAlignSize(width);
 image->heightA = imageAlignSize(height);

 // Allocate bitmap in VRAM if possible
#if (GRAPHIC_FORCE_RAM == 0)

 image->bmp = vramAlloc(image->widthA * image->heightA * sizeof(GRAPHIC_COLOR_TYPE));
 if (image->bmp)
 {
  image->useVRAM = 1;
  return image;
 }

#endif

 // Allocate bitmap in RAM if possible (16 aligned)
 image->bmp = memalign(16,image->widthA * image->heightA * sizeof(GRAPHIC_COLOR_TYPE));
 if (image->bmp)
 {
  image->useVRAM = 0;
  return image;
 }

 // No allocation succesfull, erase image
 free(image);

 return NULL;
}

void imageDelete (Image *image)

/*

  Delete an image structure

  Parameters :	image		->	Pointer to the image to delete

  Return :		None

*/

{
 // If bad parameters
 if (!(image)) return;

 // Free bitmap
 if (image->bmp)
 {
  if (image->useVRAM)
   vramFree(image->bmp);
  else
   free(image->bmp);
 }

 // Free image
 free(image);
}

Image *imageGetDisplay (void)

/*

  Get the display image structure (for screenshot)

  Parameters :	None

  Return :		Pointer to the display image structure

*/

{
 return &imageDisplay;
}

u32 imageDrawBlock (Image *image, u32 srcX, u32 srcY, u32 destX, u32 destY, u32 width, u32 height)

/*

  Draw a block of image in draw buffer

  Parameters :	Image		->	Pointer to image contains the block
				srcX		->	X coordinates of block begin
				srcY		->	Y coordinates of block begin
				destX		->	X coordinates where the block will be drawn
				destY		->	Y coordinates where the block will be drawn
				width		->	Width of block
				height		->	Height of block

  Return :		0			->	OK
				1			->	Bad parameters

*/

{
 // If bad parameters
 if (!(image)) return 1;

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 // Check if swizzle
 if (graphicTextureOpt)
 {
  if (((Image *) graphicTextureOpt)->useVRAM != image->useVRAM) sceGuTexMode(GRAPHIC_BUFFER_FORMAT,0,0,((image->useVRAM) ? GU_FALSE : GU_TRUE));
 }
 else
  sceGuTexMode(GRAPHIC_BUFFER_FORMAT,0,0,((image->useVRAM) ? GU_FALSE : GU_TRUE));

#endif

 // Check if image is the same of last image drawed
 if (image != (Image *) graphicTextureOpt)
 {
  // Set current image as texture
  sceGuTexImage(0,image->widthA,image->heightA,image->widthA,image->bmp);

  // Last image is current image
  graphicTextureOpt = (void *) image;
 }

 // Draw image as texture
 graphicBlitByTextureFast(srcX,srcY,destX,destY,width,height);

 return 0;
}

u32 imageDrawBlockSized (Image *image, u32 srcX, u32 srcY, u32 srcW, u32 srcH, u32 destX, u32 destY, u32 destW, u32 destH)

/*

  Draw a block of image sized in draw buffer

  Parameters :	Image		->	Pointer to image contains the block
				srcX		->	X coordinates of block begin
				srcY		->	Y coordinates of block begin
				srcW		->	Width of block to draw
				srcH		->	Height of block to draw
				destX		->	X coordinates where the block will be drawn
				destY		->	Y coordinates where the block will be drawn
				destW		->	Width of block drawn
				destH		->	Height of block drawn

  Return :		0			->	OK
				1			->	Bad parameters

*/

{
 // If bad parameters
 if (!(image)) return 1;

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 // Check if swizzle
 if (graphicTextureOpt)
 {
  if (((Image *) graphicTextureOpt)->useVRAM != image->useVRAM) sceGuTexMode(GRAPHIC_BUFFER_FORMAT,0,0,((image->useVRAM) ? GU_FALSE : GU_TRUE));
 }
 else
  sceGuTexMode(GRAPHIC_BUFFER_FORMAT,0,0,((image->useVRAM) ? GU_FALSE : GU_TRUE));

#endif

 // Check if image is the same of last image drawed
 if (image != (Image *) graphicTextureOpt)
 {
  // Set current image as texture
  sceGuTexImage(0,image->widthA,image->heightA,image->widthA,image->bmp);

  // Last image is current image
  graphicTextureOpt = (void *) image;
 }

 // Draw image as texture
 graphicBlitByTextureSized(srcX,srcY,srcW,srcH,destX,destY,destW,destH);

 return 0;
}

u32 imageLoad (Image **image, const char *filename, int alpha)

/*

  Load an image file with functions which depends of name

  Parameters :	image		->	Address of image pointer which contains the image
				filename	->	Name of the file to load
				alpha		->	If < 0, use alpha of file else use alpha defined (for all pixels)

  Return :		Value of return function or 0xFF if file is not managed

*/

{
 // If bmp
 if (strstr(strlwr((char *) filename),".bmp")) return bmpLoad(image,filename,alpha);

 // If png
 if (strstr(strlwr((char *) filename),".png")) return pngLoad(image,filename,alpha);

 return 0xFF;
}

u32 imageSave (Image *image, const char *filename)

/*

  Save an image file with functions which depends of name

  Parameters :	image		->	Pointer of image to save
				filename	->	Name of the file to save

  Return :		Value of return function or 0xFF if file is not managed

*/

{
 // If bmp
 if (strstr(strlwr((char *) filename),".bmp")) return bmpSave(image,filename);

 // If png
 if (strstr(strlwr((char *) filename),".png")) return pngSave(image,filename);

 return 0xFF;
}


// *** FORMAT FUNCTIONS ***

#include "format/imagebmp.c"
#include "format/imagepng.c"
