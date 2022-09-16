
/*

  Language functions source

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "language.h"
#include "language_default.h"


// *** VARIABLES ***

ConfigFile languageFile = { NULL, NULL, 0 };


// *** FUNCTIONS ***

u32 languageGetList (const char *folder, LanguageList *list)

/*

  Get the language list of language folder

  Parameters :	folder		->	Language files folder
				list		->	Address of list which contains list of language

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Error on open language directory
				3			->	Unable to allocate memory for list headers, but results maybe correct
				4			->	Unable to allocate memory for filename string
				5			->	Unable to allocate memory for name string
				6			->	Unable to allocate memory for author string

*/

{
 SceIoDirent dirEntry;
 ConfigFile file;
 ConfigVariable *value;
 char name[256];
 int dir, len;
 LanguageHeader *temp;


 // Verify parameters
 if ((!(folder)) || (!(list))) return 1;

 languageFreeList(list);

 // Create path
 strcpy(name,folder);
 strcat(name,"/");

 // Open directory entry
 dir = sceIoDopen(name);
 if (dir < 0) return 2;

 // Init variables
 len = strlen(name);
 file.opened = 0;
 memset(&dirEntry,0,sizeof(SceIoDirent));

 // Entry loop
 while (sceIoDread(dir,&dirEntry) > 0)
 {
  // Verify type
  switch (dirEntry.d_stat.st_mode & FIO_S_IFMT)
  {
   // File
   case FIO_S_IFREG :

    // Create file name
    strcat(name,dirEntry.d_name);

	// Verify extension
	if (strcasecmp(&name[strlen(name) - 4],LANGUAGE_FILE_EXTENSION)) break;

	// Open file
	if (configLoad(&file,name)) break;

	// Read variables
    if (configRead(&file,LANGUAGE_KEYNAME_HEADER,LANGUAGE_VARNAME_NAME,&value))
	{
	 configClose(&file,0,NULL);
	 break;
	}

	// Add value in array
	if (!(list->count % 5))
	{
	 // Allocate new value
	 temp = (LanguageHeader *) realloc(list->headers,sizeof(LanguageHeader) * (list->count + 5));

	 // Verify
	 if (!(temp))
	 {
	  configClose(&file,0,NULL);
	  return 3;
	 }

	 // Copy temp
	 list->headers = temp;
	}

	// Initialize pointer to NULL
	memset(&list->headers[list->count],0,sizeof(LanguageHeader));

	// Allocate filename
	list->headers[list->count].filename = (char *) malloc(strlen(name) + 1);
	if (!(list->headers[list->count].filename))
	{
	 configClose(&file,0,NULL);

	 return 4;
	}

	strcpy(list->headers[list->count].filename,name);

	// Allocate name
	list->headers[list->count].name = (char *) malloc(strlen((const char *) value->value) + 1);
	if (!(list->headers[list->count].name))
	{
	 list->count++;
	 configClose(&file,0,NULL);

	 return 5;
	}

	strcpy(list->headers[list->count].name,(const char *) value->value);

	// Allocate author
	if (!(configRead(&file,LANGUAGE_KEYNAME_HEADER,LANGUAGE_VARNAME_AUTHOR,&value)))
	{
 	 list->headers[list->count].author = (char *) malloc(strlen((const char *) value->value) + 1);
	 if (!(list->headers[list->count].author))
	 {
	  list->count++;
	  configClose(&file,0,NULL);

	  return 6;
	 }

	 strcpy(list->headers[list->count].author,(const char *) value->value);
	}

	list->count++;

	// Close file
	configClose(&file,0,NULL);

   break;
  }

  // Restore name
  name[len] = 0;
 }

 // Close directory entry
 sceIoDclose(dir);

 return 0;
}

void languageFreeList (LanguageList *list)

/*

  Free a language list

  Parameters :	list		->	Address of list to free

  Return :		None

*/

{
 int x;


 // Verify parameters
 if (!(list)) return;

 // Free headers
 if (list->headers)
 {
  // Free text
  for (x=0;x<list->count;x++)
  {
   if (list->headers[x].filename) free(list->headers[x].filename);
   if (list->headers[x].name) free(list->headers[x].name);
   if (list->headers[x].author) free(list->headers[x].author);
  }

  free(list->headers);
 }

 // Initialize list
 list->headers = NULL;
 list->count = 0;
}

char *languageGetListFilename (LanguageList *list, const char *name)

/*

  Get file name of a language

  Parameters :	list		->	Address of list which contains list of language
				name		->	Name of language to get file name

  Return :		File name string or NULL if error

*/

{
 int x;


 // Verify parameters
 if ((!(list)) || (!(name))) return NULL;

 // Verify header
 if (!(list->headers)) return NULL;

 // Find name in list
 for (x=0;x<list->count;x++)
 {
  if (!(strcasecmp(list->headers[x].name,name))) return list->headers[x].filename;
 }

 // If not found
 return NULL;
}

char *languageGetListAuthor (LanguageList *list, const char *name)

/*

  Get author of a language

  Parameters :	list		->	Address of list which contains list of language
				name		->	Name of language to get author

  Return :		Author string or NULL if error

*/

{
 int x;


 // Verify parameters
 if ((!(list)) || (!(name))) return NULL;

 // Verify header
 if (!(list->headers)) return NULL;

 // Find name in list
 for (x=0;x<list->count;x++)
 {
  if (!(strcasecmp(list->headers[x].name,name))) return list->headers[x].author;
 }

 // Return default author if default name choosed
 if (!(strcasecmp(LANGUAGE_DEFAULT_NAME,name))) return LANGUAGE_DEFAULT_AUTHOR;

 // If not found
 return NULL;
}

u32 languageLoad (const char *filename)
{
 // Close possible previous language
 languageFree();

 // Load new language
 return configLoad(&languageFile,filename);
}

void languageFree (void)

/*

  Free the current language

  Parameters :	None

  Return :		None

*/

{
 configClose(&languageFile,0,NULL);
}

char *languageGetString (u32 id)

/*

  Get a string of current language loaded

  Parameters :	id			->	ID of string to get

  Return :		Language string or NULL if error

*/

{
 char *string;
 char sid[16];
 ConfigVariable *value;


 // Make id string
 sprintf(sid,"%d",id);

 // Make default string
 string = (id < LANGUAGE_DEFAULT_SIZE) ? LANGUAGE_DEFAULT_TEXT[id] : NULL;

 // Load string
 if (configRead(&languageFile,LANGUAGE_KEYNAME_TEXT,sid,&value)) return string;

 // Verify if type is string
 if (value->type != CONFIG_TYPE_STRING) return string;

 // Copy value
 string = (char *) value->value;

 return string;
}

u32 languageGetOskId (void)

/*

  Get the osk id of current language loaded

  Parameters :	None

  Return :		Osk id

*/

{
 ConfigVariable *value;


 // Load id
 if (configRead(&languageFile,LANGUAGE_KEYNAME_HEADER,LANGUAGE_VARNAME_OSKID,&value)) return LANGUAGE_DEFAULT_OSKID;

 // Verify if type is integer
 if (value->type != CONFIG_TYPE_INTEGER) return LANGUAGE_DEFAULT_OSKID;

 return *((u32 *) value->value);
}
