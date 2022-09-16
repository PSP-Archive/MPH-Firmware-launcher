
#ifndef VRAM_INCLUDED
#define VRAM_INCLUDED

/*

  VRAM simple allocation functions header

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <psptypes.h>
#include <malloc.h>


// *** DEFINES ***

#define VRAM_BASE		0x04000000	// Maybe 0x44000000
#define VRAM_SIZE		0x00200000


// *** STRUCTURES ***

typedef struct VramItem				// Vram Item
{
 u32 address;						// Address of allocation
 u32 size;							// Size of allocation
 u8 used;							// Use flag
 struct VramItem *prev, *next;		// Chained list
} VramItem;


// *** FUNCTIONS DECLARATIONS ***

void *vramAlloc (u32);												// Allocate a memory block in VRAM
void vramFree (void *);												// Free a memory block in VRAM
void vramFreeAll (void);											// Free all memory blocks in VRAM

#endif
