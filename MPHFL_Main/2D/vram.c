
/*

  VRAM simple allocation functions source

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "vram.h"


// *** GLOBALS VARIABLES ***

VramItem vramTable = { VRAM_BASE, VRAM_SIZE, 0, NULL, NULL };


// *** FUNCTIONS ***

void *vramAlloc (u32 size)

/*

  Allocate a memory block in VRAM

  Parameters :	size		->	Size to allocate in bytes

  Return :		Pointer to memory allocated or NULL if error

*/

{
 VramItem *item, *pointer, *temp;


 // Init variables
 item = NULL;
 pointer = &vramTable;

 // Item loop
 while (pointer)
 {
  // Find a valid item
  if ((!(pointer->used)) && (pointer->size >= size))
  {
   // Find the smaller item for avoid many small blocks
   if (item)
   {
    if (pointer->size < item->size) item = pointer;
   }
   else
	item = pointer;
  }

  // Go to the next item
  pointer = pointer->next;
 }

 // If valid item found
 if (item)
 {
  // If memory remaining
  if (item->size > size)
  {
   // Allocate new item
   temp = (VramItem *) malloc(sizeof(VramItem));
   if (!(temp)) return NULL;

   // Init new item
   temp->prev = item;
   temp->next = item->next;
   temp->address = item->address + size;
   temp->size = item->size - size;
   temp->used = 0;

   // Init current item
   if (item->next) item->next->prev = temp;
   item->next = temp;
   item->size = size;
  }

  // Current item used
  item->used = 1;

  // return good item address
  return (void *) item->address;
 }

 // Not valid item found
 return NULL;
}

void vramFree (void *address)

/*

  Free a memory block in VRAM

  Parameters :	address		->	Address of allocated block to free

  Return :		None

*/

{
 VramItem *item, *pointer;


 // Init variable
 pointer = &vramTable;

 // Item loop
 while (pointer)
 {
  // Find the good item
  if (pointer->address == (u32) address)
  {
   // Check if we can regroup with previous block
   item = pointer->prev;

   if (item)
   {
    if (!(item->used))
	{
	 // Regroup with previous block
	 item->next = pointer->next;
	 if (pointer->next) pointer->next->prev = item;
	 item->size += pointer->size;

	 // Delete item
	 free(pointer);

	 // Point pointer to good item
	 pointer = item;
	}
   }

   // Check if we can regroup with next block
   item = pointer->next;

   if (item)
   {
    if (!(item->used))
	{
	 // Regroup with next block
	 pointer->next = item->next;
	 if (item->next) item->next->prev = pointer;
	 pointer->size += item->size;

	 // Delete item
	 free(item);
	}
   }

   // Current item not used
   pointer->used = 0;

   return;
  }

  // Go to the next item
  pointer = pointer->next;
 }
}

void vramFreeAll (void)

/*

  Free all memory blocks in VRAM

  Parameters :	None

  Return :		None

*/

{
 // Delete main item
 vramTable.used = 0;

 // Delete all items after main item
 while (vramTable.next) vramFree((void *) vramTable.next->address);
}
