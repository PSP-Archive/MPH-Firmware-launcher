
#ifndef PSARDUMP_INCLUDED
#define PSARDUMP_INCLUDED

/*

  PSAR dumper for Updater data - Version 2A
  (c) 2005 PspPet

  WORKING WITH:
   1.00 Bogus update (old school, including V1 DecryptPRX)
   1.50 update, 1.51 update, 1.52 update (including V1 DecryptPRX)
   NOTE: no longer using firmware 'sceKernelCheckExecFile'

   2.00, 2.01, 2.50 update (NOW WITH V2 DecryptPRX !!!)

  2A a few fixes and IPL decrypting

  Modified to use with MPHFL

*/


// *** INCLUDES ***

#include <pspkernel.h>
#include <pspdebug.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "file.h"
#include "config.h"


// *** DEFINES ***

#define PSAR_DELETE_PBP
//#define PSAR_USE_ENCRYPTED

#define PSAR_OVERHEAD		0x150					// Size of encryption block overhead
#define PSAR_SIZE_A			0x110					// Size of uncompressed file entry = 272 bytes

#define PSAR_PSAR_MAGIC		"PSAR"					// PSAR signature
#define PSAR_PSP_MAGIC		"~PSP"					// PSP signature

#define PSAR_PBP_MAGIC		"\0PBP"					// PBP signature
#define PSAR_PBP_VERSION	0x00010000				// PBP version

#define PSAR_SIZEDIGEST		16						// Size of MD5

#define PSAR_NAME_INSTALL	"install.inf"			// File name for install

#define PSAR_KEYNAME_FOLDER	"FOLDER#"				// Name of folder key in installation file

#define PSAR_VARNAME_MD5	"md5"					// Name of md5 file variable in installation file
#define PSAR_VARNAME_FOLDER	"location"				// Name of location folder variable in installation file


// *** STRUCTURES ***

typedef int (*PSAR_PROC_DECODE) (void* r4in, u32 r5in, int* r6Ret);						// Decodes in place
typedef int (*PSAR_PROC_MANGLE) (void* r4, u32 r5, void* r6, u32 r7, u32 r8);			// Secret access to hardware decryption AKA "semaphore_2" r8=opcode: 7=>block cypher/scramble (many uses), 11=>SHA1, 1=>magic decode of real PRX code
typedef u32 (*PSAR_PROC_DEFLATE) (void* destP, u32 cb, const void* scrP, u32* retP);	// Unzip ?

typedef struct
{
 u32 tag;	// 4 byte value at offset 0xD0 in the PRX file
 u8 *key;	// "step1_result" use for XOR step
 u8 code;
 u8 codeExtra;
} PsarTagInfo;

typedef struct
{
 char magic[4];
 u32 version;
 u32 offset[8];
} PsarPbpHeader;


// *** FUNCTIONS DECLARATIONS ***

u32 psarInitSysEntries (void);														// Init system entries (must be called before graphics function else crashes !?)
u32 psarInstallFirmware (const char *, const char *, const char *, u32);			// Install a firmware
u32 psarUninstallFirmware (const char *);											// Uninstall a firmware
u32 psarCheckFirmware (const char *, char *, char *, u32 *, u32 *, u32 *);			// Check a firmware

u32 psarCreateInstallFile (const char *, const char *);								// Create an install.inf file with a flash0 folder

#endif
