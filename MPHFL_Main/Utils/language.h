
#ifndef LANGUAGE_INCLUDED
#define LANGUAGE_INCLUDED

/*

  Language functions header

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "config.h"


// *** DEFINES ***

#define	LANGUAGE_DEFAULT_NAME		"ENGLISH"
#define	LANGUAGE_DEFAULT_AUTHOR		"MPH"
#define	LANGUAGE_DEFAULT_OSKID		1

#define	LANGUAGE_FOLDER_PATH		"/language"
#define	LANGUAGE_FILE_EXTENSION		".lng"

#define	LANGUAGE_KEYNAME_HEADER		"HEADER"
#define	LANGUAGE_KEYNAME_TEXT		"TEXT"

#define	LANGUAGE_VARNAME_NAME		"Name"
#define	LANGUAGE_VARNAME_AUTHOR		"Author"
#define	LANGUAGE_VARNAME_OSKID		"OskId"


// *** STRUCTURES ***

typedef struct LanguageHeader			// Language header
{
 char *name;							// Name of language
 char *author;							// Author of language
 char *filename;						// Name of language file
} LanguageHeader;

typedef struct LanguageList				// Language list
{
 LanguageHeader *headers;				// Array of header
 u32 count;								// Number of header
} LanguageList;


// *** FUNCTIONS DECLARATIONS ***

u32 languageGetList (const char *, LanguageList *);						// Get the language list of language folder
void languageFreeList (LanguageList *);									// Free a language list
char *languageGetListFilename (LanguageList *, const char *);			// Get file name of a language
char *languageGetListAuthor (LanguageList *, const char *);				// Get author of a language
u32 languageLoad (const char *);										// Load a language
void languageFree (void);												// Free the current language
char *languageGetString (u32);											// Get a string of current language loaded
u32 languageGetOskId (void);											// Get the osk id of current language loaded

#endif
