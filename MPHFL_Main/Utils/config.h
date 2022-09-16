
#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

/*

  Configuration file reading/writing functions header

  Use dynamic memory (simple chained list)

  By MPH (mphtheone@hotmail.com)


  File language :
  ---------------

  ;	bla bla						<- For comments

  [key_name]					<- For key

  variable = value int			<- For number value
  variable = "value string"		<- For string value
  ou
  variable = value string		<- For string value (spaces truncated)

*/


// *** INCLUDES ***

#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


// *** DEFINES ***

#define CONFIG_WITHCASE									// For case sensive research with key and variable name

#define CONFIG_TYPE_STRING		0						// String variables
#define CONFIG_TYPE_INTEGER		1						// Integer variables
#define CONFIG_TYPE_FLOAT		2						// Float variables

#define CONFIG_CAR_KEYBEGIN		'['						// Key caracter for begin
#define CONFIG_CAR_KEYEND		']'						// Key caracter for end
#define CONFIG_CAR_COMMENT		';'						// Key caracter for comments
#define CONFIG_CAR_STRING		'"'						// Key caracter for string
#define CONFIG_CAR_ASSIGN		'='						// Key caracter for assign

#define CONFIG_KEY_ROOT			"*ROOT*"				// Root key for variables that don't entered in key


// *** STRUCTURES ***

typedef struct ConfigVariable
{
 char *name;					// Name of variable
 int type;						// Type of variable
 void *value;					// Pointer to variable value
 struct ConfigVariable *next;	// Next variable pointer
} ConfigVariable;

typedef struct ConfigKey
{
 char *name;					// Name of key
 ConfigVariable *variables;		// Pointer to variable chained list
 struct ConfigKey *next;		// Next key pointer
} ConfigKey;

typedef struct ConfigFile
{
 char *name;					// File name
 ConfigKey *keys;				// Pointer to key chained list
 int opened;					// Flag to know if file is already opened
} ConfigFile;


// *** FUNCTIONS DECLARATIONS ***

u32 configCreate (ConfigFile *, const char *);									// Create a new configuration file
u32 configLoad (ConfigFile *, const char *);									// Load an existing configuration file
u32 configClose (ConfigFile *, u32, const char *);								// Close the current opened configuration file

ConfigKey* configCreateKey(ConfigFile *, const char *);							// Create a new key
u32 configDeleteKey(ConfigFile *, ConfigKey *);									// Delete an existing key

ConfigVariable* configCreateVariable(ConfigKey *, const char *);				// Create a new variable
u32 configDeleteVariable(ConfigKey *, ConfigVariable *);						// Delete an existing variable

ConfigKey* configFindKey(ConfigFile *, const char *);							// Find a key in file
ConfigVariable* configFindVariable(ConfigKey *, const char *);					// Find a variable in key

u32 configRead (ConfigFile *, const char *, const char *, ConfigVariable **);	// Read a variable into the current opened configuration file
u32 configWrite (ConfigFile *, const char *, const char *, ConfigVariable *);	// Write a variable into the current opened configuration file

#endif
