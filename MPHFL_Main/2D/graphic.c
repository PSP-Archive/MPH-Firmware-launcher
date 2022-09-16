
/*

  Graphic functions source based on samples of PSPSDK

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "graphic.h"


// *** GLOBALS VARIABLES ***

static u32 __attribute__((aligned(16))) graphicList[262144];			// Graphic list context
void *graphicBuffer;													// Current draw buffer
void *graphicDrawBuffer = NULL;											// Draw buffer (for VRAM alloc/free)
void *graphicDispBuffer = NULL;											// Display buffer (for VRAM alloc/free)
//void *graphicDepthBuffer = NULL;											// Depth buffer (for VRAM alloc/free)
void *graphicTextureOpt = NULL;											// For optimize texture render


// *** FUNCTIONS ***

void graphicInit (void)

/*

  Initialize the GU system

  Parameters :	None

  Return :		None

*/

{
 // Allocate buffer in VRAM to avoid overwrite of buffer
 graphicDrawBuffer = vramAlloc(GRAPHIC_FRAME_SIZE);
 graphicDispBuffer = vramAlloc(GRAPHIC_FRAME_SIZE);
 //graphicDepthBuffer = vramAlloc(GRAPHIC_BUF_WIDTH);

 // Init GU system
 sceGuInit();

 // Start filling a new display context
 sceGuStart(GU_DIRECT,graphicList);

 // Set draw buffer parameters (32 bits color, start at address 0 in VRAM, frame buffer width = 512)
 sceGuDrawBuffer(GRAPHIC_BUFFER_FORMAT,(void *) 0,GRAPHIC_BUF_WIDTH);

 // Set display buffer parameters (size of screen, start at address 0x88000 (after draw buffer) in VRAM, frame buffer width = 512)
 // 0x88000 = 512 * 272 * 4
 sceGuDispBuffer(GRAPHIC_SCR_WIDTH,GRAPHIC_SCR_HEIGHT,(void *) GRAPHIC_FRAME_SIZE,GRAPHIC_BUF_WIDTH);

 // Set depth buffer (3D only ?), 0x110000 = 0x88000 * 2 (two buffers before)
 //sceGuDepthBuffer((void *) (GRAPHIC_FRAME_SIZE * 2),GRAPHIC_BUF_WIDTH);

 // Set virtual coordinate offset (PSP has 4096,4096 virtual space), center in virtual space
 sceGuOffset((GRAPHIC_VSP_WIDTH - GRAPHIC_SCR_WIDTH) >> 1,(GRAPHIC_VSP_HEIGHT - GRAPHIC_SCR_HEIGHT) >> 1);

 // Set the current viewport (first point is center of viewport)
 sceGuViewport(GRAPHIC_VSP_WIDTH >> 1,GRAPHIC_VSP_HEIGHT >> 1,GRAPHIC_SCR_WIDTH,GRAPHIC_SCR_HEIGHT);

 // Set distance of depth (near first, far then)
 //sceGuDepthRange(GRAPHIC_DPT_NEAR,GRAPHIC_DPT_FAR);

 // Set what to scissor within the current viewport
 sceGuScissor(0,0,GRAPHIC_SCR_WIDTH,GRAPHIC_SCR_HEIGHT);

 // Enable GE state (scissor)
 sceGuEnable(GU_SCISSOR_TEST);

 // Set the depth test function (GU_GEQUAL = pixel that match the test pass)
 //sceGuDepthFunc(GU_GEQUAL);

 // Enable GE state (depth)
 //sceGuEnable(GU_DEPTH_TEST);

 // Set the current face order (GU_CW = clockwise primitives are not culled)
 //sceGuFrontFace(GU_CW);

 // Enable GE state (cull face)
 //sceGuEnable(GU_CULL_FACE);

 // Set how the primitives are shaded (GU_SMOOTH = gouraud shaded)
 sceGuShadeModel(GU_FLAT /* GU_SMOOTH*/);

 // Enable GE state (texture 2D)
 sceGuEnable(GU_TEXTURE_2D);

 // Enable GE state (clip planes)
 sceGuEnable(GU_CLIP_PLANES);

#ifdef GRAPHIC_USE_TRANSPARENT

 // Set color test function
 sceGuColorFunc(GU_NOTEQUAL,GRAPHIC_USE_TRANSPARENT,0x00FFFFFF);

 // Enable GE state (color test)
 sceGuEnable(GU_COLOR_TEST);

#endif

 // Set blending parameters
 sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);

 // Enable GE state (blending)
 sceGuEnable(GU_BLEND);

 // Clear the draw buffer
 graphicClearBuffer(0x0);

 // Finish current display list and go back to the parent context
 sceGuFinish();

 // Wait until display list has finished executing
 sceGuSync(0,0);

 // Wait vblank start
 sceDisplayWaitVblankStart();

 // Turn display (GU_TRUE = on)
 sceGuDisplay(GU_TRUE);

 // Set draw buffer to current draw buffer
 graphicBuffer = GRAPHIC_VRAM_ADDRESS;
}

void graphicTerm (void)

/*

  Terminate the GU system

  Parameters :	None

  Return :		None

*/

{
 // Terminate GU system
 sceGuTerm();

 // Free buffer in VRAM
 vramFree(graphicDrawBuffer);
 vramFree(graphicDispBuffer);
 //vramFree(graphicDepthBuffer);
}

void graphicStartRender (void)

/*

  Start a new render

  Parameters :	None

  Return :		None

*/

{
 // Start render
 sceGuStart(GU_DIRECT,graphicList);

 // Set texture mode parameters
#if (GRAPHIC_FORCE_SWIZZLE == 1)

 sceGuTexMode(GRAPHIC_BUFFER_FORMAT,0,0,GU_FALSE);

#elif (GRAPHIC_FORCE_SWIZZLE == 2)

 sceGuTexMode(GRAPHIC_BUFFER_FORMAT,0,0,GU_TRUE);

#endif

 // Set texture filter
 sceGuTexFilter(GU_LINEAR,GU_LINEAR);

 // Set texture function
 sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);

 // Init texture optimizer
 graphicTextureOpt = NULL;
}

void graphicStopRender (void)

/*

  Stop the current render

  Parameters :	None

  Return :		None

*/

{
 // Stop render
 sceGuFinish();

 // Wait until display list has finished executing
 sceGuSync(0,0);

 // Wait vblank start
 sceDisplayWaitVblankStart();

 // Swap the buffers and save draw buffer
 graphicBuffer = sceGuSwapBuffers();
}

void graphicClearBuffer (u32 color)

/*

  Clear the draw buffer

  Parameters :	color		->	Clear color (always 8888 format)

  Return :		None

*/

{
 // Set the current clear color
 sceGuClearColor(color);

 // Set the clear depth
// sceGuClearDepth(0);

 // Clear current draw buffer (clear color buffer)
 sceGuClear(GU_COLOR_BUFFER_BIT/* | GU_DEPTH_BUFFER_BIT*/);
}

void graphicSwizzleFast (const u8 *in, u8 *out, u32 width, u32 height)

/*

  Swizzle fast texture for optimizing render

  Parameters :	in			->	Pointer to texture to swizzle
				out			->	Pointer to memory which contains swizzle
				width		->	Width of the image aligned (in bytes)
				height		->	Height of the image

  Return :		None

*/

{
 const u8 *ysrc, *xsrc;				// Current pointer
 const u32 *src;					// Current src pointer
 u32 *dst;							// Current dest pointer
 u32 nBlockX, nBlockY;				// Number of block to swizzle in buffer
 u32 blockX, blockY;				// Current block swizzled
 u32 srcPitch, srcRow;				// For increment block and line
 u32 x;								// For loop
 

 // Find the block number
 nBlockX = (width >> 0x4);			// 2 * for two half part
 nBlockY = (height >> 0x3);

 // Find the increment variable (32 bits)
 srcPitch = (width - 16) >> 0x2;

 // Find the increment block Y
 srcRow = width << 0x3;

 // Init variables
 ysrc = in;
 dst = (u32 *) out;

 // Height loop
 for (blockY=0;blockY<nBlockY;blockY++)
 {
  xsrc = ysrc;

  // Width loop
  for (blockX=0;blockX<nBlockX;blockX++)
  {
   src = (u32 *) xsrc;

   // Write a block (32 bits copy)
   for (x=0;x<8;x++)
   {
    *(dst++) = *(src++);
    *(dst++) = *(src++);
    *(dst++) = *(src++);
    *(dst++) = *(src++);

	// Go to the next line
    src += srcPitch;
   }

   // Go to the second half part
   xsrc += 16;
  }

  // Go to the down block
  ysrc += srcRow;
 }
}

void graphicBlitByGE (void *src, void *dest, u32 srcX, u32 srcY, u32 destX, u32 destY, u32 sizeX, u32 sizeY, u32 srcA, u32 destA)

/*

  Blit a memory in draw buffer by GE

  Parameters :	src			->	Pointer to memory source
				dest		->	Pointer to memory destination
				srcX		->	X coordinates of memory source to copy
				srcY		->	Y coordinates of memory source to copy
				destX		->	X coordinates of memory destination
				destY		->	Y coordinates of memory destination
				sizeX		->	Width of memory to copy
				sizeY		->	Height of memory to copy
				srcA		->	Width of memory source aligned
				destA		->	Width of memory destination aligned

  Return :		None

*/

{
 sceGuCopyImage(GRAPHIC_BUFFER_FORMAT,srcX,srcY,sizeX,sizeY,srcA,src,destX,destY,destA,dest);
}

void graphicBlitByTextureSized (u16 srcX, u16 srcY, u16 srcW, u16 srcH, u16 destX, u16 destY, u16 destW, u16 destH)

/*

  Blit a memory in draw buffer by texture (sized)

  Parameters :	srcX		->	X coordinates of memory source to copy
				srcY		->	Y coordinates of memory source to copy
				srcW		->	Width of memory to copy
				srcH		->	Height of memory to copy
				destX		->	X coordinates of memory destination
				destY		->	Y coordinates of memory destination
				destW		->	Width of memory copied
				destH		->	Height of memory copied

  Return :		None

*/

{
 GraphicVT *vertex;
 
 
 // Allocate vertex
 vertex = (GraphicVT *) sceGuGetMemory(sizeof(GraphicVT) << 1);

 // Init vertices
 vertex[0].u = srcX;
 vertex[0].v = srcY;
 vertex[0].color = 0;
 vertex[0].x = destX;
 vertex[0].y = destY;
 vertex[0].z = 0;

 vertex[1].u = srcX + srcW;
 vertex[1].v = srcY + srcH;
 vertex[1].color = 0;
 vertex[1].x = destX + destW;
 vertex[1].y = destY + destH;
 vertex[1].z = 0;

 // GE copy the texture
 sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertex);
}

void graphicBlitByTextureFast (u16 srcX, u16 srcY, u16 destX, u16 destY, u16 sizeX, u16 sizeY)

/*

  Blit a memory in draw buffer by texture (fast)

  Parameters :	srcX		->	X coordinates of memory source to copy
				srcY		->	Y coordinates of memory source to copy
				destX		->	X coordinates of memory destination
				destY		->	Y coordinates of memory destination
				sizeX		->	Width of memory to copy
				sizeY		->	Height of memory to copy

  Return :		None

*/

{
 GraphicVT *vertex;
 int start, end, width;


 // Loop for write texture in sized block
 for (start=srcX,end=srcX+sizeX;start<end;start+=GRAPHIC_BLIT_SLICE,destX+=GRAPHIC_BLIT_SLICE)
 {
  // Allocate vertex
  vertex = (GraphicVT *) sceGuGetMemory(sizeof(GraphicVT) << 1);

  // Find remaining width
  width = ((start + GRAPHIC_BLIT_SLICE) < end) ? GRAPHIC_BLIT_SLICE : (end - start);

  // Init vertices
  vertex[0].u = start;
  vertex[0].v = srcY;
  vertex[0].color = 0;
  vertex[0].x = destX;
  vertex[0].y = destY;
  vertex[0].z = 0;

  vertex[1].u = start + width;
  vertex[1].v = srcY + sizeY;
  vertex[1].color = 0;
  vertex[1].x = destX + width;
  vertex[1].y = destY + sizeY;
  vertex[1].z = 0;

  // GE copy the texture
  sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertex);
 }
}

void graphicDrawPixel (u16 x, u16 y, GRAPHIC_COLOR_TYPE color)

/*

  Draw a pixel in draw buffer

  Parameters :	x			->	X coordinates of pixel
				y			->	Y coordinates of pixel
				color		->	Color of pixel

  Return :		None

*/

{
 GraphicVC *vertex;


 // Allocate vertex
 vertex = (GraphicVC *) sceGuGetMemory(sizeof(GraphicVC) << 1);

 // Init vertice
 vertex[0].color = color;
 vertex[0].x = x;
 vertex[0].y = y;
 vertex[0].z = 0;

 // GE draw the pixel
 sceGuDisable(GU_TEXTURE_2D);
 sceGuDrawArray(GU_POINTS,GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,1,0,vertex);
 sceGuEnable(GU_TEXTURE_2D);
}

void graphicDrawLine (u16 x1, u16 y1, u16 x2, u16 y2, GRAPHIC_COLOR_TYPE color)

/*

  Draw a line in draw buffer

  Parameters :	x1			->	X coordinates of line begin
				y1			->	Y coordinates of line begin
				x2			->	X coordinates of line end
				y2			->	Y coordinates of line end
				color		->	Color of line

  Return :		None

*/

{
 GraphicVC *vertex;


 // Allocate vertex
 vertex = (GraphicVC *) sceGuGetMemory(sizeof(GraphicVC) << 1);

 // Init vertices
 vertex[0].color = color;
 vertex[0].x = x1;
 vertex[0].y = y1;
 vertex[0].z = 0;

 vertex[1].color = color;
 vertex[1].x = x2;
 vertex[1].y = y2;
 vertex[1].z = 0;

 // GE draw the pixel
 sceGuDisable(GU_TEXTURE_2D);
 sceGuDrawArray(GU_LINES,GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertex);
 sceGuEnable(GU_TEXTURE_2D);
}

void graphicDrawRectangle (u16 x, u16 y, u16 sizeX, u16 sizeY, GRAPHIC_COLOR_TYPE color, u8 fill)

/*

  Draw a rectangle

  Parameters :	x			->	X coordinates of rectangle
				y			->	Y coordinates of rectangle
				sizeX		->	Width of rectangle
				sizeY		->	Height of rectangle
				color		->	Color of rectangle
				fill		->	If != 0, fill the rectangle

  Return :		None

*/

{
 GraphicVC *vertex;


 if (fill)
 {
  // Allocate vertex
  vertex = (GraphicVC *) sceGuGetMemory(sizeof(GraphicVC) << 1);

  // Init vertices
  vertex[0].color = color;
  vertex[0].x = x;
  vertex[0].y = y;
  vertex[0].z = 0;

  vertex[1].color = color;
  vertex[1].x = x + sizeX;
  vertex[1].y = y + sizeY;
  vertex[1].z = 0;

  // GE draw the pixel
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_SPRITES,GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertex);
  sceGuEnable(GU_TEXTURE_2D);
 }
 else
 {
   // Allocate vertex
  vertex = (GraphicVC *) sceGuGetMemory(sizeof(GraphicVC) * 5);

  // Init vertices
  vertex[0].color = color;
  vertex[0].x = x;
  vertex[0].y = y;
  vertex[0].z = 0;

  vertex[1].color = color;
  vertex[1].x = x + sizeX;
  vertex[1].y = y;
  vertex[1].z = 0;

  vertex[2].color = color;
  vertex[2].x = x + sizeX;
  vertex[2].y = y + sizeY + 1;
  vertex[2].z = 0;

  vertex[3].color = color;
  vertex[3].x = x;
  vertex[3].y = y + sizeY;
  vertex[3].z = 0;

  vertex[4].color = color;
  vertex[4].x = x;
  vertex[4].y = y;
  vertex[4].z = 0;

  // GE draw the pixel
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_LINE_STRIP,GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,5,0,vertex);
  sceGuEnable(GU_TEXTURE_2D);
 }
}

void graphicDrawTriangle (u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, GRAPHIC_COLOR_TYPE color, u8 fill)

/*

  Draw a triangle

  Parameters :	x1			->	X coordinates of first point of triangle
				y1			->	Y coordinates of first point of triangle
				x2			->	X coordinates of second point of triangle
				y2			->	Y coordinates of second point of triangle
				x3			->	X coordinates of third point of triangle
				y3			->	Y coordinates of third point of triangle
				color		->	Color of triangle
				fill		->	If != 0, fill the triangle

  Return :		None

*/

{
 GraphicVC *vertex;


 if (fill)
 {
  // Allocate vertex
  vertex = (GraphicVC *) sceGuGetMemory(sizeof(GraphicVC) * 3);

  // Init vertices
  vertex[0].color = color;
  vertex[0].x = x1;
  vertex[0].y = y1;
  vertex[0].z = 0;

  vertex[1].color = color;
  vertex[1].x = x2;
  vertex[1].y = y2;
  vertex[1].z = 0;

  vertex[2].color = color;
  vertex[2].x = x3;
  vertex[2].y = y3;
  vertex[2].z = 0;

  // GE draw the pixel
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_TRIANGLE_STRIP,GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,3,0,vertex);
  sceGuEnable(GU_TEXTURE_2D);
 }
 else
 {
  // Allocate vertex
  vertex = (GraphicVC *) sceGuGetMemory(sizeof(GraphicVC) * 4);

  // Init vertices
  vertex[0].color = color;
  vertex[0].x = x1;
  vertex[0].y = y1;
  vertex[0].z = 0;

  vertex[1].color = color;
  vertex[1].x = x2;
  vertex[1].y = y2;
  vertex[1].z = 0;

  vertex[2].color = color;
  vertex[2].x = x3;
  vertex[2].y = y3;
  vertex[2].z = 0;

  vertex[3].color = color;
  vertex[3].x = x1;
  vertex[3].y = y1;
  vertex[3].z = 0;

  // GE draw the pixel
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_LINE_STRIP,GRAPHIC_VERTEX_FORMAT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,4,0,vertex);
  sceGuEnable(GU_TEXTURE_2D); 
 }
}
