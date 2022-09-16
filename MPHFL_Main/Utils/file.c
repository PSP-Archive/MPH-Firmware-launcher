
/*

  File functions manipulations source

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "file.h"


// *** VARIABLES ***

u8 *fileBuffer;
char fileSrcString[FILE_SIZE_PATH];
char fileDestString[FILE_SIZE_PATH];
char fileFilterString[32];
FileList *fileFileList;
FileFunc fileFileFunc;
FileFunc fileFolderFunc;
u8 fileRecursive = 1;
u8 fileWithCurrent = 0;
u8 fileWithParent = 0;


// *** FUNCTIONS ***

u32 fileSaveBuffer (const char *filename, u8 *buffer, u32 size)

/*

  Save a buffer into a file

  Parameters :	filename		->	Name of file which contains buffer
				buffer			->	Buffer to save
				size			->	Size of buffer

  Return :		0				->	OK
				1				->	Unable to create file
				& 0x2			->	Error on write data
				& 0x4			->	Error on close file

*/

{
 int fd, ret;


 // Open file
 fd = sceIoOpen(filename,PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY,0777);
 if (fd < 0) return 1;

 ret = 0;

 // Write buffer
 if (sceIoWrite(fd,buffer,size) != size) ret |= 0x2;

 // Close file
 if (sceIoClose(fd)) ret |= 0x4;

 return ret;
}

u32 fileCopyFile (const char *src, const char *dest, int *size)

/*

  Copy a file

  Parameters :	src			->	File to copy
				dest		->	Name of destination file
				size		->	Pointer to variable which contains size of copy

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to allocate memory for copy buffer
				3			->	Unable to open source file
				4			->	Unable to open destination file
				5			->	Read error
				6			->	Write error

*/

{
 int fdin, fdout, fsize, s1, s2, x;


 // Verify parameters
 if ((!(src)) || (!(dest))) return 1;

 // Allocate copy buffer
 fileBuffer = (u8 *) malloc(FILE_SIZE_BUFFER);
 if (!(fileBuffer)) return 2;

 // Open source file
 fdin = sceIoOpen(src,PSP_O_RDONLY,0);
 if (fdin < 0)
 {
  free(fileBuffer);

  return 3;
 }

 // Open destination file
 fdout = sceIoOpen(dest,PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC,0777);
 if (fdout < 0)
 {
  sceIoClose(fdin);
  free(fileBuffer);

  return 4;
 }

 // size points to good variable
 if (!(size)) size = &s1;
 *size = 0;

 // Find source file size
 fsize = sceIoLseek(fdin,0,SEEK_END);
 sceIoLseek(fdin,0,SEEK_SET);

 // Copy file
 while (*size < fsize)
 {
  // Read data
  x = sceIoRead(fdin,fileBuffer,FILE_SIZE_BUFFER);

  // If error exit
  if (x <= 0)
  {
   sceIoClose(fdout);
   sceIoClose(fdin);
   free(fileBuffer);

   return 5;
  }

  // Write data
  s2 = sceIoWrite(fdout,fileBuffer,x);

  // Update size
  *size += x;

  // If error exit
  if (s2 != x)
  {
   sceIoClose(fdout);
   sceIoClose(fdin);
   free(fileBuffer);

   return 6;
  }
 }

 // Close files
 sceIoClose(fdout);
 sceIoClose(fdin);

 // Free buffer
 free(fileBuffer);

 return 0;
}

u32 fileBrowseFolder (void)

/*

  Recursive function to browse folder

  Parameters :	Nothing

  Return :		0			->	OK
				1			->	Unable to read directory entry
				2			->	Function called return errors

*/

{
 SceIoDirent dirEntry;
 int dir, srclen, destlen;
 int patchFlash;


 // Init variables
 srclen = strlen(fileSrcString);
 if (srclen)
 {
  if (fileSrcString[srclen - 1] != '/')
  {
   strcat(fileSrcString,"/");
   srclen++;
  }
 }

 destlen = strlen(fileDestString);
 if (destlen)
 {
  if (fileDestString[destlen - 1] != '/')
  {
   strcat(fileDestString,"/");
   destlen++;
  }
 }

 // Patch for flash device ('.' and '..' always exists even in root)
 fileSrcString[srclen - 1] = 0;
 patchFlash = (strchr(fileSrcString,'/')) ? 1 : 0;
 fileSrcString[srclen - 1] = '/'; 

 // Open directory entry
 dir = sceIoDopen(fileSrcString);
 if (dir < 0) return 1;

 memset(&dirEntry,0,sizeof(SceIoDirent));

 // Entry loop
 while (sceIoDread(dir,&dirEntry) > 0)
 {
  // Create source path
  strcat(fileSrcString,dirEntry.d_name);

  // Create destination path
  strcat(fileDestString,dirEntry.d_name);

  // Verify type
  switch (dirEntry.d_stat.st_mode & FIO_S_IFMT)
  {
   // Folder
   case FIO_S_IFDIR :

	// If current folder
	if (!(strcmp(dirEntry.d_name,".")))
	{
	 if ((patchFlash) && (fileWithCurrent)) fileFolderFunc();
	 break;
	}

	// If parent folder
	if (!(strcmp(dirEntry.d_name,"..")))
	{
	 if ((patchFlash) && (fileWithParent)) fileFolderFunc();
	 break;
	}

	// Call the folder function
	if (fileFolderFunc)
	{
	 if (fileFolderFunc())
	 {
	  sceIoDclose(dir);
	  return 2;
	 }
	}

	// Recursive for folders 
	if (fileRecursive)
	{
     if (fileBrowseFolder())
	 {
	  sceIoDclose(dir);
	  return 1;
	 }
	}

   break;

   // File
   case FIO_S_IFREG :

    // If filter, test it
	if (fileFilterString[0])
	{
	 if (strlen(dirEntry.d_name) < strlen(fileFilterString)) break;
	 if (strcasecmp(&dirEntry.d_name[strlen(dirEntry.d_name) - strlen(fileFilterString)],fileFilterString)) break;
	}

	// Call the file function
	if (fileFileFunc)
	{
	 if (fileFileFunc())
	 {
	  sceIoDclose(dir);
	  return 2;
	 }
	}

   break;
  }

  // Restore source string
  fileSrcString[srclen] = 0;

  // Restore destination string
  fileDestString[destlen] = 0;
 }

 // Close directory entry
 sceIoDclose(dir);

 return 0;
}

u32 fileCopyFolder_FileFunc (void)

/*

  File function for copy folder

  Parameters :	None

  Return :		Same that fileCopyFile

*/

{
 return fileCopyFile(fileSrcString,fileDestString,NULL);
}

u32 fileCopyFolder_FolderFunc (void)

/*

  Folder function for copy folder

  Parameters :	None

  Return :		Always 0

*/

{
 sceIoMkdir(fileDestString,0777);

 return 0;
}

u32 fileCopyFolder (const char *src, const char *dest, const char *filter)

/*

  Copy a folder

  Parameters :	src			->	Folder to copy
				dest		->	Name of destination folder

  Return :		Same that fileBrowseFolder

*/

{
 // Verify parameters
 if ((!(src)) || (!(dest))) return 1;

 // Copy parameters functions
 strncpy(fileSrcString,src,FILE_SIZE_PATH - 1);
 fileSrcString[FILE_SIZE_PATH - 1] = 0;

 strncpy(fileDestString,dest,FILE_SIZE_PATH - 1);
 fileDestString[FILE_SIZE_PATH - 1] = 0;

 fileFileFunc = fileCopyFolder_FileFunc;
 fileFolderFunc = fileCopyFolder_FolderFunc;
 fileRecursive = 1;
 fileWithCurrent = 0;
 fileWithParent = 0;

 if (filter)
 {
  strncpy(fileFilterString,filter,31);
  fileFilterString[31] = 0;
 }
 else
  fileFilterString[0] = 0;

 // Create destination folder if not exist
 sceIoMkdir(dest,0777);

 // Call recursive function
 return fileBrowseFolder();
}

u32 fileGetList_FileFunc (void)

/*

  Function called by fileGetList for add file name to array

  Parameters :	None

  Return :		0			->	OK
				1			->	Unable to allocate memory for array
				2			->	Unable to allocate memory for string

*/

{
 char **temp;


 // Add value in array
 if (!(fileFileList->count % 5))
 {
  // Allocate new value
  temp = (char **) realloc(fileFileList->list,sizeof(char *) * (fileFileList->count + 5));

  // Verify
  if (!(temp)) return 1;

  // Copy temp
  fileFileList->list = temp;
 }

 // Allocate new string
 fileFileList->list[fileFileList->count] = malloc(strlen(fileSrcString) + 1);
 if (!(fileFileList->list[fileFileList->count])) return 2;

 // Copy string
 strcpy(fileFileList->list[fileFileList->count],fileSrcString);

 // Update count
 fileFileList->count++;

 return 0;
}

u32 fileGetList_FolderFunc (void)

/*

  Function called by fileGetList for add folder name to array

  Parameters :	None

  Return :		0			->	OK
				1			->	Unable to allocate memory for array
				2			->	Unable to allocate memory for string

*/

{
 // Add last '/'
 strcat(fileSrcString,"/");

 return fileGetList_FileFunc();
}

u32 fileGetList (const char *folder, const char *filter, u8 flags, FileList *list)

/*

  Get the list of a folder

  Parameters :	folder		->	Base folder to get list
				filter		->	If != NULL, filter for list
				flags		->	Flags for make list
				list		->	Pointer to list variable which contains list

  Return :		1 if bad parameters of same of fileBrowseFolder

*/

{
 // Verify parameters
 if ((!(folder)) || (!(list))) return 1;

 // Reset list
 memset(list,0,sizeof(FileList));

 // Copy parameters functions
 strncpy(fileSrcString,folder,FILE_SIZE_PATH - 1);
 fileSrcString[FILE_SIZE_PATH - 1] = 0;

 fileDestString[0] = 0;

 fileFileList = list;
 fileFileFunc = (flags & FILE_LIST_FILE) ? fileGetList_FileFunc : NULL;
 fileFolderFunc = (flags & FILE_LIST_FOLDER) ? fileGetList_FolderFunc : NULL;
 fileRecursive = (flags & FILE_LIST_RECURSIVE) ? 1 : 0;
 fileWithCurrent = (flags & FILE_LIST_WITHCURRENT) ? 1 : 0;
 fileWithParent = (flags & FILE_LIST_WITHPARENT) ? 1 : 0;

 if (filter)
 {
  strncpy(fileFilterString,filter,31);
  fileFilterString[31] = 0;
 }
 else
  fileFilterString[0] = 0;

 // Call recursive function
 return fileBrowseFolder();
}

u32 fileFreeList (FileList *list)

/*

  Free a list

  Parameters :	list		->	List to free

  Return :		0			->	OK
				1			->	Bad parameters

*/

{
 int x;


 // Verify parameters
 if (!(list)) return 1;

 if (list->list)
 {
  // Delete strings
  for (x=0;x<list->count;x++)
  {
   if (list->list[x]) free(list->list[x]);
  }

  // Delete array
  free(list->list);
 }

 // Init variables
 list->count = 0;
 list->list = NULL;

 return 0;
}
