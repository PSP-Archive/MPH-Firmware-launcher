
#ifndef RAM_INCLUDED
#define RAM_INCLUDED

/*

  RAM simple check functions header

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <psptypes.h>
#include <malloc.h>


// *** DEFINES ***

#define RAM_BLOCK		(1024 * 1024)


// *** FUNCTIONS DECLARATIONS ***

u32 ramAvailableLineareMax (void);									// Get the maximum lineare ram available
u32 ramAvailable (void);											// Get the total ram available

#endif
