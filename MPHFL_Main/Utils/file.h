
#ifndef FILE_INCLUDED
#define FILE_INCLUDED

/*

  File functions manipulations header

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <pspiofilemgr.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>


// *** DEFINES ***

#define FILE_SIZE_BUFFER		0x40000										// Size of copy buffer (256 ko)
#define FILE_SIZE_PATH			0x100										// Size of a string path

#define	FILE_LIST_FILE			0x1											// Add files to the list
#define	FILE_LIST_FOLDER		0x2											// Add folders to the folders
#define	FILE_LIST_RECURSIVE		0x4											// Browse the sub folders
#define	FILE_LIST_WITHCURRENT	0x8											// Add current folder to the list
#define	FILE_LIST_WITHPARENT	0x10										// Add parent folder to the list

#define	FILE_DEVICE_MS			"ms0:/"										// Name of memory stick device
#define	FILE_DEVICE_FLASH0		"flash0:/"									// Name of flash0 device
#define	FILE_DEVICE_FLASH1		"flash1:/"									// Name of flash1 device
#define	FILE_DEVICE_UMD			"disc0:/"									// Name of umd device


// *** STRUCTURES ***

typedef u32 (*FileFunc)(void);	// File function called in recursive browse function

typedef struct FileList			// List of file name
{
 char **list;					// Array of string
 u32 count;						// Number of string in array
} FileList;


// *** FUNCTIONS DECLARATIONS ***

u32 fileSaveBuffer (const char *, u8 *, u32);								// Save a buffer into a file
u32 fileCopyFile (const char *, const char *, int *);						// Copy a file
u32 fileCopyFolder (const char *, const char *, const char *);				// Copy a folder
u32 fileGetList (const char *, const char *, u8, FileList *);				// Get the file/folder name list of a folder
u32 fileFreeList (FileList *);												// Free the result list

#endif
