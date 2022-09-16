
/*

  Bmp format functions source (to use with Image structure)

  By MPH (mphtheone@hotmail.com)

*/

// *** INCLUDES ***

#include "imagebmp.h"


// *** FUNCTIONS ***

u32 bmpLoad (Image **image, const char *filename, int alpha)

/*

  Load a BMP file into a Image

  Parameters :	image		->	Address of image pointer which contains BMP image
				filename	->	Name of BMP file
				alpha		->	If < 0, use alpha of file else use alpha defined (for all pixels)

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to open file

*/

{
 int fd;
 char magic[2];					// Not in structure because alignment make error
 u32 size;
 BmpFileHeader fileheader;
 BmpInfoHeader infoheader;
 GRAPHIC_COLOR_TYPE *bmp, *temp;
 u8 *line;
 u32 r, g, b, a;
 int x, y, x2, sizeline;


 // Verify parameters
 if ((!(image)) || (!(filename))) return 1;

 // Open file
 fd = sceIoOpen(filename,PSP_O_RDONLY,0);
 if (fd < 0) return 2;

 // Find size of file
 size = sceIoLseek(fd,0,PSP_SEEK_END);
 sceIoLseek(fd,0,PSP_SEEK_SET);

 // Read magic
 sceIoRead(fd,magic,2);
 if (memcmp(magic,BMP_MAGIC,strlen(BMP_MAGIC)))
 {
  sceIoClose(fd);
  return 3;
 }

 // Read file structure
 if (sceIoRead(fd,&fileheader,sizeof(fileheader)) != sizeof(fileheader))
 {
  sceIoClose(fd);
  return 4;
 }

 // Verify file header
 if (fileheader.filesize != size)
 {
  sceIoClose(fd);
  return 5; 
 }

 // Read info structure
 if (sceIoRead(fd,&infoheader,sizeof(BmpInfoHeader)) != sizeof(BmpInfoHeader))
 {
  sceIoClose(fd);
  return 6;
 }

 // Create image
 *image = imageCreate(infoheader.width,infoheader.height);
 if (!(*image))
 {
  sceIoClose(fd);
  return 7; 
 }

 // Find bytes size of a pixel
 infoheader.depth >>= 3;

 // Allocate one line (with padding)
 line = (u8 *) malloc(infoheader.width * infoheader.depth);
 if (!(line))
 {
  imageDelete(*image);
  sceIoClose(fd);

  return 8;
 }

 // Initialize bmp pointer
 temp = NULL;							// For warning message

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 if (!((*image)->useVRAM))

#endif

#if (GRAPHIC_FORCE_SWIZZLE != 1)

 {
  temp = (GRAPHIC_COLOR_TYPE *) memalign(16,(*image)->widthA * (*image)->height * sizeof(GRAPHIC_COLOR_TYPE));
  bmp = temp + (((*image)->height - 1) * (*image)->widthA);
 }

#else

 bmp = (*image)->bmp + (((*image)->height - 1) * (*image)->widthA);;

#endif

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 else
  bmp = (*image)->bmp + (((*image)->height - 1) * (*image)->widthA);;

#endif

 if (!(bmp))
 {
  free(line);
  imageDelete(*image);
  sceIoClose(fd);

  return 9;
 }

 // Go to image data
 sceIoLseek(fd,fileheader.offdata,PSP_SEEK_SET);

 // Initialize alpha if don't exist
 a = 0xFF;

 // Calculate the size of a line (multiple of 4)
 sizeline = (infoheader.width * infoheader.depth) + ((infoheader.width * infoheader.depth) % 4);

 // Height loop
 for (y=0;y<infoheader.height;y++)
 {
  // Read a row
  sceIoRead(fd,line,sizeline);

  // Width loop
  for (x=0,x2=0;x<infoheader.width;x++)
  {
   // Read pixel
   b = line[x2++];
   g = line[x2++];
   r = line[x2++];
   //if (infoheader.depth == 4) a = line[x2++];

   // Write pixel
   bmp[x] = GRAPHIC_MAKE_COLOR(r,g,b,((alpha < 0) ? a : alpha));
  }

  // Go to the previous line
  bmp -= (*image)->widthA;
 }

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 if (!((*image)->useVRAM))

#endif

#if (GRAPHIC_FORCE_SWIZZLE != 1)

 {
  graphicSwizzleFast((const u8 *) temp,(u8 *) (*image)->bmp,(*image)->widthA * sizeof(GRAPHIC_COLOR_TYPE),(*image)->height);
  free(temp);
 }

#endif

  // Update data
 sceKernelDcacheWritebackAll();

 // Free memory
 free(line);
 sceIoClose(fd);

 return 0;
}

u32 bmpSave (Image *image, const char *filename)

/*

  Save an image to a file in BMP format

  Parameters :	image		->	Pointer to the image to save
				filename	->	Name of file to create

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to create file

*/

{
 return 0;
}
