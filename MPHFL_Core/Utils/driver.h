
#ifndef DRIVER_INCLUDED
#define DRIVER_INCLUDED

/*

  Driver functions

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <pspiofilemgr_kernel.h>
#include <stdio.h>
#include <string.h>


// *** DEFINES ***

#define DRIVER_DEFAULT_NAME			"mphfl"
#define DRIVER_DEFAULT_NAMECAPS		"MPHFL"
#define DRIVER_DEFAULT_PATCHED		"flash"
#define DRIVER_DEFAULT_PHYS			"msstor0p1:"
#define DRIVER_DEFAULT_FS			"fatms0:"
#define DRIVER_DEFAULT_LOGFILE		"ms0:/mphfl_driver.log"

#define DRIVER_DEFAULT_FOLDERF0		"/flash0"
#define DRIVER_DEFAULT_FOLDERF1		"/flash1"

#define	DRIVER_SIZE_NAME			0x20
#define	DRIVER_SIZE_PATH			0x100


// *** STRUCTURES ***

typedef struct DriverConf				// Driver configuration
{
 char name[DRIVER_SIZE_NAME];			// Name of driver
 char nameCaps[DRIVER_SIZE_NAME];		// Name of driver capsed
 char patched[DRIVER_SIZE_NAME];		// Name of device patched
 char phys[DRIVER_SIZE_NAME];			// Name of physical device used by driver
 char fs[DRIVER_SIZE_NAME];				// Name of filesystem device used by driver
 char logfile[DRIVER_SIZE_PATH];		// Name of log file
 char folderF0[DRIVER_SIZE_PATH];		// Name of flash0 folder
 char folderF1[DRIVER_SIZE_PATH];		// Name of flash1 folder
} DriverConf;


// FUNCTIONS DECLARATIONS

u32 driverInstall (DriverConf *, u32);
u32 driverUninstall (u32);

#endif
