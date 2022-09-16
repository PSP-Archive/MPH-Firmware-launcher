
/*

  Configuration file reading/writing functions source

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

#include "config.h"


// *** FUNCTIONS ***

u32 configStrToNum (const char *string, void *value)

/*

  Convert a string to a number (integer or float)

  Parameters :	string	->	String to convert
				value	->	Result of the conversion

  Return :		The type of value

*/

{
 int x, v, base, basetest;


 // Find base
 for (x=0,v=0,base=1,basetest=0;x<strlen(string);x++)
 {
  if (string[x] != '.')
  {
   if (basetest) base *= 10;
   v *= 10;
   v += string[x] - '0';
  }
  else
   basetest = 1;
 }

 // Integer
 if (!(basetest))
 {
  *((u32 *) value) = v;

  return CONFIG_TYPE_INTEGER;
 }

 // Float
 *((SceFloat *) value) = (SceFloat) (((SceFloat) v) / base);

 return CONFIG_TYPE_FLOAT;
}

ConfigKey* configCreateKey(ConfigFile *file, const char *name)

/*

  Create a key in file

  Parameters :	file	->	File which contains the key
				name	->	Name of the key to create

  Return :		Key pointer or NULL if error

*/

{
 ConfigKey *key, *temp;


 // If bad parameters
 if ((!(file)) || (!(name))) return NULL;
 if (!(file->opened)) return NULL;

 // Allocate key
 key = (ConfigKey *) malloc(sizeof(ConfigKey));
 if (!(key)) return NULL;

 // Allocate name
 key->name = (char *) malloc(strlen(name) + 1);
 if (!(key->name))
 {
  free(key);
  return NULL;
 }

 // Init values
 strcpy(key->name,name);
 key->variables = NULL;
 key->next = NULL;

 // Add key in file
 if (!(file->keys))
 {
  file->keys = key;
  return key;
 }

 temp = file->keys;
 while (temp->next) temp = temp->next;

 temp->next = key;

 return key;
}

u32 configDeleteKey(ConfigFile *file, ConfigKey *key)

/*

  Delete a key in file

  Parameters :	file	->	File which contains the key
				key		->	Key pointer to delete

  Return :		0		->	OK
				1		->	Bad parameters
				2		->	Key not found in file

*/

{
 ConfigKey *temp;


 // If bad parameters
 if ((!(file)) || (!(key))) return 1;
 if (!(file->opened)) return 1;

 // Search key from list key in file
 if (file->keys == key)
  file->keys = key->next;
 else
 {
  temp = file->keys;
  while (temp)
  {
   if (temp->next == key) break;

   temp = temp->next;
  }

  // If not found
  if (temp->next != key) return 2;

  // Erase from list
  temp->next = key->next;
 }

 // Erase variables
 while (key->variables) configDeleteVariable(key,key->variables);

 // Free string name
 if (key->name) free(key->name);

 // Free key
 free(key);

 return 0;
}

ConfigVariable* configCreateVariable(ConfigKey *key, const char *name)

/*

  Create a variable in key

  Parameters :	key		->	Key which contains the variable
				name	->	Name of the variable to create

  Return :		Variable pointer or NULL if error

*/

{
 ConfigVariable *var, *temp;


 // If bad parameters
 if ((!(key)) || (!(name))) return NULL;

 // Allocate variable
 var = (ConfigVariable *) malloc(sizeof(ConfigVariable));
 if (!(var)) return NULL;

 // Allocate name
 var->name = (char *) malloc(strlen(name) + 1);
 if (!(var->name))
 {
  free(var);
  return NULL;
 }

 // Init values
 strcpy(var->name,name);
 var->value = NULL;
 var->type = 0;
 var->next = NULL;

 // Add variable in key
 if (!(key->variables))
 {
  key->variables = var;
  return var;
 }

 temp = key->variables;
 while (temp->next) temp = temp->next;

 temp->next = var;

 return var;
}

u32 configDeleteVariable(ConfigKey *key, ConfigVariable *var)

/*

  Delete a variable in key

  Parameters :	key			->	Key which contains the variable
				var			->	Variable pointer to delete

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Variable not found in key

*/

{
 ConfigVariable *temp;


 // If bad parameters
 if ((!(key)) || (!(var))) return 1;

 // Search variable from list variable in key
 if (key->variables == var)
  key->variables = var->next;
 else
 {
  temp = key->variables;
  while (temp)
  {
   if (temp->next == var) break;

   temp = temp->next;
  }

  // If not found
  if (temp->next != var) return 2;

  // Erase from list
  temp->next = var->next;
 }

 // Erase value
 if (var->value) free(var->value);

 // Free string name
 if (var->name) free(var->name);

 // Free key
 free(var);

 return 0;
}

ConfigKey* configFindKey(ConfigFile *file, const char *name)

/*

  Find a key in file

  Parameters :	file	->	File which contains the key
				name	->	Name of the key to find

  Return :		Pointer key or NULL if error

*/

{
 ConfigKey *temp;


 // If bad parameters
 if ((!(file)) || (!(name))) return NULL;
 if (!(file->opened)) return NULL;

 // Search name
 temp = file->keys;
 while (temp)
 {
  if (temp->name)
  {
#ifdef CONFIG_WITHCASE
   if (!(strcmp(temp->name,name))) return temp;
#else
   if (!(strcasecmp(temp->name,name))) return temp;
#endif
  }

  temp = temp->next;
 }

 return NULL;
}

ConfigVariable* configFindVariable(ConfigKey *key, const char *name)

/*

  Find a variable in key

  Parameters :	key		->	File which contains the variable
				name	->	Name of the variable to find

  Return :		Pointer variable or NULL if error

*/

{
 ConfigVariable *temp;


 // If bad parameters
 if ((!(key)) || (!(name))) return NULL;

 // Search name
 temp = key->variables;
 while (temp)
 {
  if (temp->name)
  {
#ifdef CONFIG_WITHCASE
   if (!(strcmp(temp->name,name))) return temp;
#else
   if (!(strcasecmp(temp->name,name))) return temp;
#endif
  }

  temp = temp->next;
 }

 return NULL;
}

u32 configCreate (ConfigFile *file, const char *filename)

/*

  Create a configuration file

  Parameters :	file		->	Pointer to file structure
				filename	->	Name of the file to create

  Return :		0			->	OK
				1			->	Incorrect parameters or a file is already openend
				2			->	Unable to create file

*/

{
 // if already opened or error parameters, exit
 if ((file->opened) || (!(filename))) return 1;

 // Reset memory
 memset(file,0,sizeof(ConfigFile));

 // Allocate string buffer
 file->name = (char *) malloc(strlen(filename) + 1);
 if (!(file->name)) return 2;

 // Set informations
 strcpy(file->name,filename);
 file->opened = 1;

 return 0;
}

u32 configLoad (ConfigFile *file, const char *filename)

/*

  Load a configuration file

  Parameters :	file		->	Pointer to file structure
				filename	->	Name of the file to load

  Return :		0			->	OK
				1			->	Incorrect parameters or a file is already openend
				2			->	Unable to load file
				3			->	Unable to create key
				4			->	Unable to create variable
				5			->	Unable to allocate value variable

*/

{
 char string[4096], *ptr, car;
 ConfigKey *key;
 ConfigVariable *var;
 int fd, x, r, size;


 // if already opened or error parameters, exit
 if (configCreate(file,filename)) return 1;

 // Ouvrir le fichier
 fd = sceIoOpen(filename,PSP_O_RDONLY,0);

 // Vérifier les erreurs
 if (fd <= 0)
 {
  configClose(file,0,NULL);
  return 2;
 }

 // Lire le fichier
 sceIoLseek(fd,0,PSP_SEEK_SET);

 key = NULL;
 var = NULL;

 for (;;)
 {
  // Init variables
  ptr = string;

  // Erase begin space and tab
  car = ' ';
  while ((car == ' ') || (car == '\t'))
  {
   // Read one caracter
   r = sceIoRead(fd,&car,1);

   // Exit if end file
   if (!(r))
   {
	// Close file
	sceIoClose(fd);

	return 0;
   }
  }

  // Read one line
  while ((car != '\r') && (car != '\n'))
  {
   // Copy caracter in string
   *ptr++ = car;

   // Read one caracter
   r = sceIoRead(fd,&car,1);

   // Break if end file
   if (!(r)) break;
  }

  // End terminating string
  *ptr = 0;

  // Erase possible comments
  size = strlen(string);
  for (x=0,r=0;x<size;x++)
  {
   if (string[x] == CONFIG_CAR_STRING)
    r = !r;
   else
   {
    if ((string[x] == CONFIG_CAR_COMMENT) && (!(r)))
	{
	 string[x] = 0;
	 break;
	}
   }
  }

  // If string not empty
  if (string[0])
  {
   // Erase end space and tab
   while ((string[strlen(string) - 1] == ' ') || (string[strlen(string) - 1] == '\t')) string[strlen(string) - 1] = 0;
 
   // Decode string
   if (string[0] == CONFIG_CAR_KEYBEGIN)		// Possible key
   {
    // Point to first caracter valid
    ptr = &string[1];

    // Find the end key car
    size = strlen(ptr);
    for (x=0,r=0;x<size;x++)
    {
     if (ptr[x] == CONFIG_CAR_KEYEND)
	 {
	  ptr[x] = 0;
	  r = 1;

	  break;
	 }

	 // On peut rajouter ici d'autres tests pour les caracteres valides comme nom de clé et si un caractere pas valide -> ptr[0] = 0 pour empecher de continuer avec ce nom de clé
    }

    // If key name valid
    if ((ptr[0]) && (r))
    {
     // Search key
     key = configFindKey(file,ptr);

     // If key not found, create it
     if (!(key))
     {
      key = configCreateKey(file,ptr);

      // If key not created, exit with error
      if (!(key))
	  {
	   sceIoClose(fd);
	   configClose(file,0,NULL);

	   return 3;
	  }
	 }
    }
   }
   else
   {
    // Point to first caracter valid
    ptr = string;

    // Find assign car to split string
	size = strlen(ptr);
	for (x=0,r=0;x<size;x++)
	{
	 if (ptr[x] == CONFIG_CAR_ASSIGN)
	 {
	  ptr[x] = 0;
	  ptr = &ptr[x + 1];
	  r = 1;

	  break;
	 }

	 // On peut rajouter ici d'autres tests pour les caracteres valides comme nom de clé et si un caractere pas valide -> ptr[0] = 0 pour empecher de continuer avec ce nom de clé
	}

    // Erase end space and tab (variable name)
    if (string[0]) while ((string[strlen(string) - 1] == ' ') || (string[strlen(string) - 1] == '\t')) string[strlen(string) - 1] = 0;

	// If variable name correct
	if (string[0])
	{
	 // Erase begin space and tab (value)
	 size = strlen(ptr);
	 for (x=0;x<size;x++)
	 {
	  if ((ptr[x] != ' ') && (ptr[x] != '\t')) break;
	 }

	 ptr = &ptr[x];

	 // If variable valid
	 if ((ptr[0]) && (r))
	 {
	  // If not in key, create in root key
	  if (!(key)) key = configCreateKey(file,CONFIG_KEY_ROOT);

      // Search key
      var = configFindVariable(key,string);

      // If variable not found, create it
      if (!(var))
      {
       var = configCreateVariable(key,string);

       // If variable not created, exit with error
       if (!(var))
	   {
	    sceIoClose(fd);
		configClose(file,0,NULL);

	    return 4;
	   }
	  }

	  // Save value in string
	  strcpy(string,ptr);

	  // Find value type
	  var->type = CONFIG_TYPE_INTEGER;

	  size = strlen(string);
	  for (x=0;x<size;x++)
	  {
       if (string[x] == '.')
        var->type = (var->type == CONFIG_TYPE_INTEGER) ? CONFIG_TYPE_FLOAT : CONFIG_TYPE_STRING;
       else
        if ((string[x] < '0') || (string[x] > '9')) var->type = CONFIG_TYPE_STRING;
	  }

      // Calculate value
      switch (var->type)
	  {
       case CONFIG_TYPE_STRING :

        if (string[0] == CONFIG_CAR_STRING)
		{
         // Eliminate end string limit caracter
         ptr = &string[1];
         while ((*ptr != CONFIG_CAR_STRING) && (*ptr != 0)) ptr++;
         *ptr = 0;

		 size = strlen(string);
		 var->value = malloc(size);

		 if (!(var->value))
		 {
	      sceIoClose(fd);
		  configClose(file,0,NULL);

	      return 5;
		 }

         memcpy(var->value,&string[1],size);
		}
        else
		{
		 size = strlen(string) + 1;
		 var->value = malloc(size);

		 if (!(var->value))
		 {
	      sceIoClose(fd);
		  configClose(file,0,NULL);

	      return 5;
		 }

         memcpy(var->value,string,size);
		}

       break;
  
       case CONFIG_TYPE_FLOAT :

        size = sizeof(SceFloat);
	    var->value = malloc(size);

        if (!(var->value))
		{
	     sceIoClose(fd);
		 configClose(file,0,NULL);

	     return 5;
		}

        configStrToNum(string,var->value);

	   break;

	   case CONFIG_TYPE_INTEGER :

        size = sizeof(u32);
	    var->value = malloc(size);

        if (!(var->value))
		{
	     sceIoClose(fd);
		 configClose(file,0,NULL);

	     return 5;
		}

        configStrToNum(string,var->value);

       break;
	  }
	 }
	}
   }
  }
 }
}

u32 configClose (ConfigFile *file, u32 save, const char *header)

/*

  Close a configuration file

  Parameters :	file	->	Pointer to file structure
				save	->	save file if != 0
				header	->	Header string

  Return :		0		->	OK
				1		->	File not opened
				2		->	Closed OK but file not saved

*/

{
 ConfigKey *key;
 ConfigVariable *var;
 char string[4096];
 int fd, x;


 // If not opened, exit
 if (!(file->opened)) return 1;

 x = 0;

 if (save)
 {
  // Save in file
  fd = sceIoOpen(file->name,PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC,0777);

  if (fd > 0)
  {
   if (header)
   {
    // Write header
    sprintf(string,"%s\r\n\r\n",header);
    sceIoWrite(fd,string,strlen(string));
   }

   key = file->keys;

   // Write keys
   while (key)
   {
    sprintf(string,"\r\n\r\n[%s]\r\n\r\n",key->name);
    sceIoWrite(fd,string,strlen(string));

    var = key->variables;

    // Write variables
    while (var)
    {
     if (var->type == CONFIG_TYPE_STRING)
      sprintf(string,"%s = \"%s\"\r\n",var->name,((char *) var->value));
     else
     {
 	  if (var->type == CONFIG_TYPE_INTEGER)
	   sprintf(string,"%s = %d\r\n",var->name,(*((u32 *) var->value)));
	  else
	   sprintf(string,"%s = %f\r\n",var->name,(*((SceFloat *) var->value)));
     }

	 sceIoWrite(fd,string,strlen(string));

     var = var->next;
    }
    key = key->next;
   }

   // Close file
   sceIoClose(fd);
  }
  else
   x = 2;
 }

 // Erase keys and variables
 while (file->keys) configDeleteKey(file,file->keys);

 // Free string name
 if (file->name) free(file->name);

 // File is closed
 file->opened = 0;

 return x;
}


u32 configRead (ConfigFile *file, const char *keyname, const char *varname, ConfigVariable **value)

/*

  Read a variable into opened file

  Parameters :	file	->	File which contains the variable
				keyname	->	Name of the key which contains the variable
				varname	->	Name of the variable to find
				value	->	Will contain value of the variable		

  Return :		0		->	OK
				1		->	Bad parameters
				2		->	Key not found
				3		->	Variable not found

*/

{
 ConfigKey *key;
 ConfigVariable *var;


 // If bad parameters
 if (!(value)) return 1;

 // Find key
 key = configFindKey(file,keyname);
 if (!(key)) return 2;

 // Find variable
 var = configFindVariable(key,varname);
 if (!(var)) return 3;

 // Copy value
 *value = var;

 return 0;
}


u32 configWrite (ConfigFile *file, const char *keyname, const char *varname, ConfigVariable *value)

/*

  Write a variable into opened file

  Parameters :	file	->	File which contains the variable
				keyname	->	Name of the key which contains the variable
				varname	->	Name of the variable to write
				value	->	Contain value of the variable

  Return :		0		->	OK
				1		->	Bad parameters
				2		->	Unable to create/find key
				3		->	Unable to create/find variable
				4		->	Unable to allocate name
				5		->	Unable to allocate value

*/

{
 ConfigKey *key;
 ConfigVariable *var;
 int size;


 // If bad parameters
 if (!(value)) return 1;
 if (!(value->value)) return 1;

 // Find key
 key = configFindKey(file,keyname);

 if (!(key))
 {
  // Create key if not found
  key = configCreateKey(file,keyname);

  if (!(key)) return 2;
 }

 // Find variable
 var = configFindVariable(key,varname);

 if (!(var))
 {
  // Create variable if not found
  var = configCreateVariable(key,varname);

  if (!(var)) return 3;
 }

 // Erase possible old value
 if (var->value) free(var->value);

 // Copy new type
 var->type = value->type;

 // Find size value
 switch (var->type)
 {
  case CONFIG_TYPE_STRING :

   size = strlen(value->value) + 1;

  break;

  case CONFIG_TYPE_INTEGER :

   size = sizeof(u32);

  break;

  case CONFIG_TYPE_FLOAT :

   size = sizeof(SceFloat);

  break;

  default :

   size = sizeof(u32);

  break;
 }

 // Allocate value
 var->value = malloc(size);
 if (!(var->value)) return 5;

 // Copy value
 memcpy(var->value,value->value,size);

 return 0;
}
