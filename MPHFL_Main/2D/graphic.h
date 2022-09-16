
#ifndef GRAPHIC_INCLUDED
#define GRAPHIC_INCLUDED

/*

  Graphic functions header based on samples of PSPSDK

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <malloc.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include "vram.h"


// *** DEFINES ***

#define GRAPHIC_COLOR_8888									// Pixel color format
#define GRAPHIC_FORCE_SWIZZLE				0				// Force using of swizzle (0 = no force, depend to texture, 1 = force to not use swizzle, 2 = force to use swizzle)
#define GRAPHIC_FORCE_RAM					0				// Force using of ram (0 = no force, depend of vram alloc, 1 = force to use ram)
#define GRAPHIC_BLIT_SLICE					64				// Size of copy block for blit
//#define GRAPHIC_USE_TRANSPARENT				0xFFFF00FF		// Transparent color if defined


#if defined (GRAPHIC_COLOR_4444)

 #define GRAPHIC_COLOR_TYPE					u16							// Type of color
 #define GRAPHIC_BUFFER_FORMAT				GU_PSM_4444					// Draw buffer color format
 #define GRAPHIC_VERTEX_FORMAT				GU_COLOR_4444				// Vertex color format

 #define GRAPHIC_GET_COLOR_R(c)				((c << 4) & 0xF0)
 #define GRAPHIC_GET_COLOR_G(c)				(c & 0xF0)
 #define GRAPHIC_GET_COLOR_B(c)				((c >> 4) & 0xF0)
 #define GRAPHIC_GET_COLOR_A(c)				((c >> 8) & 0xF0)

 #define GRAPHIC_MAKE_COLOR(r,g,b,a)		(((r & 0xF0) >> 4) | (g & 0xF0) | ((b & 0xF0) << 4) | ((a & 0xF0) << 8))
 #define GRAPHIC_MAKE_ALPHA(c,a)			((c & 0xFFF) | ((a & 0xF0) << 8))

#elif defined (GRAPHIC_COLOR_5551)

 #define GRAPHIC_COLOR_TYPE					u16							// Type of color
 #define GRAPHIC_BUFFER_FORMAT				GU_PSM_5551					// Draw buffer color format
 #define GRAPHIC_VERTEX_FORMAT				GU_COLOR_5551				// Vertex color format

 #define GRAPHIC_GET_COLOR_R(c)				((c << 3) & 0xF8)
 #define GRAPHIC_GET_COLOR_G(c)				((c >> 2) & 0xF8)
 #define GRAPHIC_GET_COLOR_B(c)				((c >> 7) & 0xF8)
 #define GRAPHIC_GET_COLOR_A(c)				((c >> 8) & 0x80)

 #define GRAPHIC_MAKE_COLOR(r,g,b,a)		(((r & 0xF8) >> 3) | ((g & 0xF8) << 2) | ((b & 0xF8) << 7) | ((a & 0x80) << 8))
 #define GRAPHIC_MAKE_ALPHA(c,a)			((c & 0x7FFF) | ((a & 0x80) << 8))

#elif defined (GRAPHIC_COLOR_8888)

 #define GRAPHIC_COLOR_TYPE					u32							// Type of color
 #define GRAPHIC_BUFFER_FORMAT				GU_PSM_8888					// Draw buffer color format
 #define GRAPHIC_VERTEX_FORMAT				GU_COLOR_8888				// Vertex color format

 #define GRAPHIC_GET_COLOR_R(c)				(c & 0xFF)
 #define GRAPHIC_GET_COLOR_G(c)				((c >> 8) & 0xFF)
 #define GRAPHIC_GET_COLOR_B(c)				((c >> 16) & 0xFF)
 #define GRAPHIC_GET_COLOR_A(c)				((c >> 24) & 0xFF)

 #define GRAPHIC_MAKE_COLOR(r,g,b,a)		((r & 0xFF) | ((g & 0xFF) << 8) | ((b & 0xFF) << 16) | ((a & 0xFF) << 24))
 #define GRAPHIC_MAKE_ALPHA(c,a)			((c & 0xFFFFFF) | ((a & 0xFF) << 24))

#endif


#define GRAPHIC_VRAM_ADDRESS	(void *) (0x40000000 | 0x04000000)		// VRAM address OR 0x40000000 for non using cache

#define GRAPHIC_VSP_WIDTH		4096									// Virtual space width
#define GRAPHIC_VSP_HEIGHT		4096									// Virtual space height

//#define GRAPHIC_DPT_NEAR		0xC350									// Depth near
//#define GRAPHIC_DPT_FAR			0x2710									// Depth far

#define GRAPHIC_BUF_WIDTH		512										// Buffer width (screen width aligned on power of 2)
#define GRAPHIC_SCR_WIDTH		480										// Screen width
#define GRAPHIC_SCR_HEIGHT		272										// Screen height

#define GRAPHIC_FRAME_SIZE		(GRAPHIC_BUF_WIDTH * GRAPHIC_SCR_HEIGHT * sizeof(GRAPHIC_COLOR_TYPE))


// *** STRUCTURES ***

typedef struct GraphicVT			// Vertex texture structure
{
 u16 u, v;							// Texture coordinates
 GRAPHIC_COLOR_TYPE color;			// Color
 u16 x, y, z;						// Vertex coordinates
} GraphicVT;

typedef struct GraphicVC			// Vertex color structure
{
 GRAPHIC_COLOR_TYPE color;			// Color
 u16 x, y, z;						// Vertex coordinates
} GraphicVC;

typedef struct GraphicImg			// Image structure
{
 u32 width, height;					// Width and height of image
 u32 widthA, heightA;				// Width and height of image (aligned)
 GRAPHIC_COLOR_TYPE *bitmap;		// Pointer to bitmap data
} GraphicImg;


// *** FUNCTIONS DECLARATION ***

void graphicInit (void);
void graphicTerm (void);
void graphicStartRender (void);
void graphicStopRender (void);
void graphicClearBuffer (u32);
void graphicSwizzleFast (const u8 *, u8 *, u32, u32);
void graphicBlitByGE (void *, void *, u32, u32, u32, u32, u32, u32, u32, u32);
void graphicBlitByTextureSized (u16, u16, u16, u16, u16, u16, u16, u16);
void graphicBlitByTextureFast (u16, u16, u16, u16, u16, u16);
void graphicDrawPixel (u16, u16, GRAPHIC_COLOR_TYPE);
void graphicDrawLine (u16, u16, u16, u16, GRAPHIC_COLOR_TYPE);
void graphicDrawRectangle (u16, u16, u16, u16, GRAPHIC_COLOR_TYPE, u8);
void graphicDrawTriangle (u16, u16, u16, u16, u16, u16, GRAPHIC_COLOR_TYPE, u8);

#endif
