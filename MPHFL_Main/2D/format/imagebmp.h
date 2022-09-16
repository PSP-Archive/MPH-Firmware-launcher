
#ifndef BMP_INCLUDED
#define BMP_INCLUDED

/*

  Bmp format functions header (to use with Image structure)

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <malloc.h>
#include <string.h>


// *** DEFINES ***

#define BMP_MAGIC			"BM"


// *** STRUCTURES ***

typedef struct BmpFileHeader		// Bitmap file header
{
 //char magic[2];						// Signature
 u32 filesize;						// Size of the file
 u16 reserved[2];					// Reserved, must be 0
 u32 offdata;						// Offset of the data image
} BmpFileHeader;

typedef struct BmpInfoHeader		// Bitmap info header
{
 u32 size;							// Size of the structure
 u32 width;							// Width of the image
 u32 height;						// Height of the image
 u16 planes;						// Number of planes
 u16 depth;							// Bit depth of color (Number of bits per pixel)
 u32 compression;					// Compression flag
 u32 imagesize;						// Size of the image
 u32 Xpermeters;					// Horizontal pixels per meter
 u32 Ypermeters;					// Vertical pixels per meter
 u32 colorused;						// Number of color used
 u32 colorimp;						// Number of important color used
} BmpInfoHeader;


// *** FUNCTIONS DECLARATION ***

u32 bmpLoad (Image **, const char *, int);
u32 bmpSave (Image *, const char *);

#endif
