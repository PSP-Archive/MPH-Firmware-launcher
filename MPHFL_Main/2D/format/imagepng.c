
/*

  Png format functions source (to use with Image structure)

  By MPH (mphtheone@hotmail.com)

*/

// *** INCLUDES ***

#include "imagepng.h"


// *** FUNCTIONS ***

static void pngUserWarning (png_structp pngStruct, png_const_charp msg)

/*

  Function called by PNG functions if error occurs

  Parameters :	pngStruct	->	Pointer to png structure
				msg			->	Warning message

  Return :		None

*/

{
 // ignore PNG warnings
}

u32 pngLoad (Image **image, const char *filename, int alpha)

/*

  Load a PNG file into a Image

  Parameters :	image		->	Address of image pointer which contains PNG image
				filename	->	Name of PNG file
				alpha		->	If < 0, use alpha of file else use alpha defined (for all pixels)

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to open file
				3			->	Unable to create PNG structure
				4			->	Unable to create PNG info structure
				5			->	Unable to create Image structure
				6			->	Unable to allocate line memory
				7			->	Unable to allocate image memory

*/

{
 FILE *fd;
 png_uint_32 width, height;
 GRAPHIC_COLOR_TYPE *bmp, *temp;
 png_structp pngRead;
 png_infop pngInfo;
 unsigned int sig_read = 0;
 int bit_depth, color_type, interlace_type;
 u32* line;
 u32 r, g, b, a;
 int x, y;


 // Verify parameters
 if ((!(image)) || (!(filename))) return 1;

 // Open file
 fd = fopen(filename,"rb");
 if (!(fd)) return 2;

 // Create PNG read structure
 pngRead = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
 if (!(pngRead))
 {
  fclose(fd);

  return 3;
 }

 // Set PNG function error
 png_set_error_fn(pngRead,(png_voidp) NULL,(png_error_ptr) NULL,pngUserWarning);

 // Create PNG info structure
 pngInfo = png_create_info_struct(pngRead);
 if (!(pngInfo))
 {
  png_destroy_read_struct(&pngRead,png_infopp_NULL,png_infopp_NULL);
  fclose(fd);

  return 4;
 }

 // Read file
 png_init_io(pngRead,fd);
 png_set_sig_bytes(pngRead,sig_read);
 png_read_info(pngRead,pngInfo);
 png_get_IHDR(pngRead,pngInfo,&width,&height,&bit_depth,&color_type,&interlace_type,int_p_NULL,int_p_NULL);
 png_set_strip_16(pngRead);
 png_set_packing(pngRead);

 // Create the image
 *image = imageCreate(width,height);
 if (!(*image))
 {
  png_destroy_read_struct(&pngRead,&pngInfo,png_infopp_NULL);
  fclose(fd);

  return 5;
 }

 // Find good color format
 if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(pngRead);
 if ((color_type == PNG_COLOR_TYPE_GRAY) && (bit_depth < 8)) png_set_gray_1_2_4_to_8(pngRead);
 if (png_get_valid(pngRead,pngInfo,PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pngRead);

 png_set_filler(pngRead,0xFF,PNG_FILLER_AFTER);

 // Allocate one line
 line = (u32 *) malloc(width * 4);
 if (!(line))
 {
  imageDelete(*image);
  png_destroy_read_struct(&pngRead,&pngInfo,png_infopp_NULL);
  fclose(fd);

  return 6;
 }

 // Initialize bmp pointer
 temp = NULL;							// For warning message

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 if (!((*image)->useVRAM))

#endif

#if (GRAPHIC_FORCE_SWIZZLE != 1)

 {
  temp = (GRAPHIC_COLOR_TYPE *) memalign(16,(*image)->widthA * (*image)->height * sizeof(GRAPHIC_COLOR_TYPE));
  bmp = temp;
 }

#else

 bmp = (*image)->bmp;

#endif

#if (GRAPHIC_FORCE_SWIZZLE == 0)

 else
  bmp = (*image)->bmp;

#endif

 if (!(bmp))
 {
  free(line);
  imageDelete(*image);
  png_destroy_read_struct(&pngRead,&pngInfo,png_infopp_NULL);
  fclose(fd);

  return 7;
 }

 // Height loop
 for (y=0;y<(int) height;y++) 
 {
  // Read a row
  png_read_row(pngRead,(u8 *) line,png_bytep_NULL);

  // Width loop
  for (x=0;x<(int) width;x++)
  {
   // Read pixel
   r = line[x] & 0xFF;
   g = (line[x] >> 8) & 0xFF;
   b = (line[x] >> 16) & 0xFF;
   a = (line[x] >> 24) & 0xFF;

   // Write pixel
   bmp[x] = GRAPHIC_MAKE_COLOR(r,g,b,((alpha < 0) ? a : alpha));
  }

  // Go to the next line
  bmp += (*image)->widthA;
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
 png_read_end(pngRead,pngInfo);
 png_destroy_read_struct(&pngRead,&pngInfo,png_infopp_NULL);
 fclose(fd);

 return 0;
}

u32 pngSave (Image *image, const char *filename)

/*

  Save an image to a file in PNG format

  Parameters :	image		->	Pointer to the image to save
				filename	->	Name of file to create

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to create file
				3			->	Unable to create PNG structure
				4			->	Unable to create PNG info structure
				5			->	Unable to allocate line memory

*/

{
 FILE *fd;
 GRAPHIC_COLOR_TYPE *bmp;
 png_structp pngWrite;
 png_infop pngInfo;
 u8* line;
 int x, y, x2;



 // Verify parameters
 if ((!(image)) || (!(filename))) return 1;

 // Open file
 fd = fopen(filename,"wb");
 if (!(fd)) return 2;

 // Create PNG write structure
 pngWrite = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
 if (!(pngWrite))
 {
  fclose(fd);

  return 3;
 }

 // Create PNG info structure
 pngInfo = png_create_info_struct(pngWrite);
 if (!(pngInfo))
 {
  png_destroy_write_struct(&pngWrite,png_infopp_NULL);
  fclose(fd);

  return 4;
 }

 // Write file
 png_init_io(pngWrite,fd);
 png_set_IHDR(pngWrite,pngInfo,image->width,image->height,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
 png_write_info(pngWrite,pngInfo);

 // Allocate one line
 line = (u8 *) malloc(image->width * 3);
 if (!(line))
 {
  png_destroy_write_struct(&pngWrite,&pngInfo);
  fclose(fd);

  return 5;
 }

 // Initialize bmp pointer
 bmp = image->bmp;

 // Height loop
 for (y=0;y<image->height;y++) 
 {
  // Width loop
  for (x=0,x2=0;x<image->width;x++)
  {
   // Write pixel in line
   line[x2++] = GRAPHIC_GET_COLOR_R(bmp[x]);
   line[x2++] = GRAPHIC_GET_COLOR_G(bmp[x]);
   line[x2++] = GRAPHIC_GET_COLOR_B(bmp[x]);
   //line[x2++] = GRAPHIC_GET_COLOR_A(bmp[x]);
  }

  // Write line
  png_write_row(pngWrite,line);

  // Go to the next line
  bmp += image->widthA;
 }

 // Free memory
 free(line);
 png_write_end(pngWrite,pngInfo);
 png_destroy_write_struct(&pngWrite,&pngInfo);
 fclose(fd);

 return 0;
}
