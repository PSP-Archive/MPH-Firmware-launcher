
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

#include "psardump.h"


// *** VARIABLES ***

int fdInstall = 0;

u8 *g_dataPSAR, *g_dataOut, *g_dataOut2;

PSAR_PROC_DECODE g_decodeProc6;
PSAR_PROC_DECODE g_decodeProcE;
PSAR_PROC_MANGLE g_mangleProc;
PSAR_PROC_DEFLATE g_sceDeflateDecompress;
PSAR_PROC_DEFLATE g_sceGzipDecompress;

// use pre-calculated keys (step1 results)
static u32 const g_key0[] =
{
  0x7b21f3be, 0x299c5e1d, 0x1c9c5e71, 0x96cb4645, 0x3c9b1be0, 0xeb85de3d, 
  0x4a7f2022, 0xc2206eaa, 0xd50b3265, 0x55770567, 0x3c080840, 0x981d55f2, 
  0x5fd8f6f3, 0xee8eb0c5, 0x944d8152, 0xf8278651, 0x2705bafa, 0x8420e533, 
  0x27154ae9, 0x4819aa32, 0x59a3aa40, 0x2cb3cf65, 0xf274466d, 0x3a655605, 
  0x21b0f88f, 0xc5b18d26, 0x64c19051, 0xd669c94e, 0xe87035f2, 0x9d3a5909, 
  0x6f4e7102, 0xdca946ce, 0x8416881b, 0xbab097a5, 0x249125c6, 0xb34c0872, 
};

static u32 const g_key2[] =
{
  0xccfda932, 0x51c06f76, 0x046dcccf, 0x49e1821e, 0x7d3b024c, 0x9dda5865, 
  0xcc8c9825, 0xd1e97db5, 0x6874d8cb, 0x3471c987, 0x72edb3fc, 0x81c8365d, 
  0xe161e33a, 0xfc92db59, 0x2009b1ec, 0xb1a94ce4, 0x2f03696b, 0x87e236d8, 
  0x3b2b8ce9, 0x0305e784, 0xf9710883, 0xb039db39, 0x893bea37, 0xe74d6805, 
  0x2a5c38bd, 0xb08dc813, 0x15b32375, 0x46be4525, 0x0103fd90, 0xa90e87a2, 
  0x52aba66a, 0x85bf7b80, 0x45e8ce63, 0x4dd716d3, 0xf5e30d2d, 0xaf3ae456, 
};

static u32 const g_key3[] =
{
  0xa6c8f5ca, 0x6d67c080, 0x924f4d3a, 0x047ca06a, 0x08640297, 0x4fd4a758, 
  0xbd685a87, 0x9b2701c2, 0x83b62a35, 0x726b533c, 0xe522fa0c, 0xc24b06b4, 
  0x459d1cac, 0xa8c5417b, 0x4fea62a2, 0x0615d742, 0x30628d09, 0xc44fab14, 
  0x69ff715e, 0xd2d8837d, 0xbeed0b8b, 0x1e6e57ae, 0x61e8c402, 0xbe367a06, 
  0x543f2b5e, 0xdb3ec058, 0xbe852075, 0x1e7e4dcc, 0x1564ea55, 0xec7825b4, 
  0xc0538cad, 0x70f72c7f, 0x49e8c3d0, 0xeda97ec5, 0xf492b0a4, 0xe05eb02a, 
};

static u32 const g_key44[] =
{
  0xef80e005, 0x3a54689f, 0x43c99ccd, 0x1b7727be, 0x5cb80038, 0xdd2efe62, 
  0xf369f92c, 0x160f94c5, 0x29560019, 0xbf3c10c5, 0xf2ce5566, 0xcea2c626, 
  0xb601816f, 0x64e7481e, 0x0c34debd, 0x98f29cb0, 0x3fc504d7, 0xc8fb39f0, 
  0x0221b3d8, 0x63f936a2, 0x9a3a4800, 0x6ecc32e3, 0x8e120cfd, 0xb0361623, 
  0xaee1e689, 0x745502eb, 0xe4a6c61c, 0x74f23eb4, 0xd7fa5813, 0xb01916eb, 
  0x12328457, 0xd2bc97d2, 0x646425d8, 0x328380a5, 0x43da8ab1, 0x4b122ac9, 
};

static u32 const g_key20[] =
{
  0x33b50800, 0xf32f5fcd, 0x3c14881f, 0x6e8a2a95, 0x29feefd5, 0x1394eae3, 
  0xbd6bd443, 0x0821c083, 0xfab379d3, 0xe613e165, 0xf5a754d3, 0x108b2952, 
  0x0a4b1e15, 0x61eadeba, 0x557565df, 0x3b465301, 0xae54ecc3, 0x61423309, 
  0x70c9ff19, 0x5b0ae5ec, 0x989df126, 0x9d987a5f, 0x55bc750e, 0xc66eba27, 
  0x2de988e8, 0xf76600da, 0x0382dccb, 0x5569f5f2, 0x8e431262, 0x288fe3d3, 
  0x656f2187, 0x37d12e9c, 0x2f539eb4, 0xa492998e, 0xed3958f7, 0x39e96523, 
};

static u32 const g_key3A[] =
{
  0x67877069, 0x3abd5617, 0xc23ab1dc, 0xab57507d, 0x066a7f40, 0x24def9b9, 
  0x06f759e4, 0xdcf524b1, 0x13793e5e, 0x0359022d, 0xaae7e1a2, 0x76b9b2fa, 
  0x9a160340, 0x87822fba, 0x19e28fbb, 0x9e338a02, 0xd8007e9a, 0xea317af1, 
  0x630671de, 0x0b67ca7c, 0x865192af, 0xea3c3526, 0x2b448c8e, 0x8b599254, 
  0x4602e9cb, 0x4de16cda, 0xe164d5bb, 0x07ecd88e, 0x99ffe5f8, 0x768800c1, 
  0x53b091ed, 0x84047434, 0xb426dbbc, 0x36f948bb, 0x46142158, 0x749bb492, 
};

static const PsarTagInfo g_tagInfo[] =
{
 // 1.x PRXs
 { 0x00000000, (u8 *) g_key0, 0x42 },
 { 0x02000000, (u8 *) g_key2, 0x45 },
 { 0x03000000, (u8 *) g_key3, 0x46 },

 // 2.0 PRXs
 { 0x4467415d, (u8 *) g_key44, 0x59, 0x59 },
 { 0x207bbf2f, (u8 *) g_key20, 0x5A, 0x5A },
 { 0x3ace4dce, (u8 *) g_key3A, 0x5B, 0x5B }
};

static int g_bOldSchool = 0; // true=> 1.00 "Bogus" update
int g_bWriteError = 0;
int psarSystemEntries = 0;	// Because InitSysEntries crashes after GE functions, must be called before


// *** FUNCTIONS ***

static u32 psarOpenInstallFile (const char *filename)

/*

  Create a file for writing informations on install

  Parameters :	filename	->	Name of file to create

  Return :		0			->	OK
				1			->	Unable to create file

*/

{
 fdInstall = sceIoOpen(filename,PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY,0777);

 return (fdInstall < 0) ? 1 : 0;
}

static void psarWriteInstallFile (const char *fmt, ...)

/*

  Write string in information file

  Parameters :	fmt			->	Format string (like printf)
				...			->	Optional arguments used by fmt (like printf)

  Return :		Nothing

*/

{
 va_list args;
 char str[1024];


 va_start(args, fmt);

 vsprintf(str,fmt,args);
 sceIoWrite(fdInstall,str,strlen(str));
 
 va_end(args);
}

static void psarCloseInstallFile (void)

/*

  Close the installation file

  Parameters :	Nothing

  Return :		Nothing

*/

{
 sceIoClose(fdInstall);
}

u32 psarFindMD5 (char *filename, char *result)

/*

  Calculate a MD5 file

  Parameters :	filename	->	Name of file to calculate MD5
				result		->	String which contains MD5 result

  Return :		0			->	OK
				1			->	Unable to open file
				2			->	Unable to allocate buffer
				3			->	Unable to read file

*/

{
 u8 *buffer;
 u8 digest[PSAR_SIZEDIGEST];
 int fd, size, x;
 char temp[3];


 // Init variables
 memset(digest,0,PSAR_SIZEDIGEST);

 // Open file
 fd = sceIoOpen(filename,PSP_O_RDONLY,0);
 if (fd < 0) return 1;

 // Find size
 size = sceIoLseek(fd,0,PSP_SEEK_END);
 sceIoLseek(fd,0,PSP_SEEK_SET);

 // Allocate memory
 buffer = (u8 *) malloc(size);
 if (!(buffer))
 {
  sceIoClose(fd);

  return 2;
 }

 // Read file
 if (sceIoRead(fd,buffer,size) != size)
 {
  sceIoClose(fd);
  free(buffer);

  return 3;
 }

 // Close file
 sceIoClose(fd);

 // Calculate MD5
 sceKernelUtilsMd5Digest(buffer,size,digest);

 // Free memory
 free(buffer);

 // Write MD5 in result
 result[0] = 0;

 for (x=0;x<PSAR_SIZEDIGEST;x++)
 {
  sprintf(temp,"%X",digest[x]);
  strcat(result,temp);
 }

 return 0;
}

u32 psarSaveFile (const char *filename, const void *buffer, int size)

/*

  Save a buffer to a file

  Parameters :	filename	->	Name of file to create
				buffer		->	Buffer to save
				size		->	Size of buffer

  Return :		0			->	OK
				1			->	Error on save buffer

*/

{
 u8 digest[PSAR_SIZEDIGEST];
 int x;


 // Save buffer
 if (fileSaveBuffer(filename,(u8 *) buffer,size))
 {
  g_bWriteError = 1;
  return 1;
 }

 // Calculate md5
 memset(digest,0,PSAR_SIZEDIGEST);
 sceKernelUtilsMd5Digest((u8 *) buffer,size,digest);

 // Write key in install file
 psarWriteInstallFile("\r\n[%s]\r\n\r\n%s = ",filename,PSAR_VARNAME_MD5);
 for (x=0;x<PSAR_SIZEDIGEST;x++) psarWriteInstallFile("%X",digest[x]);
 psarWriteInstallFile("\r\n\r\n");

 return 0;
}

u32 psarLoadPsarFile (const char *filename, int *size)

/*

  Load a PSAR file (in PBP file) to a buffer

  Parameters :	filename	->	Name of pbp file to load
				size		->	Pointer to variable which contains size

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to open file
				3			->	Unable to read header
				4			->	Not a PBP file
				5			->	PSAR not found
				6			->	Unable to read PSAR file

*/

{
 int fd, x;
 PsarPbpHeader header;
 

 // Verify parameters
 if ((!(filename)) || (!(size))) return 1;

 // Open PBP file
 fd = sceIoOpen(filename,PSP_O_RDONLY,0);
 if (fd < 0) return 2;

 // Read header
 if (sceIoRead(fd,&header,sizeof(header)) != sizeof(header))
 {
  sceIoClose(fd);
  return 3;
 }

 // Validate header
 if (memcmp(header.magic,PSAR_PBP_MAGIC,strlen(PSAR_PBP_MAGIC)))
 {
  sceIoClose(fd);
  return 4;
 }

 // Find PSAR file
 x = sceIoLseek(fd,0,PSP_SEEK_END) - header.offset[7];
 if (x <= 0)
 {
  sceIoClose(fd);
  return 5;
 }

 // Read file
 sceIoLseek(fd,header.offset[7],PSP_SEEK_SET);
 *size = sceIoRead(fd,g_dataPSAR,x);

 if (*size != x)
 {
  sceIoClose(fd);
  return 6; 
 }

 // Close file
 sceIoClose(fd);

 return 0;
}

static u32 psarFindProc (const char *modname, const char *libname, u32 nid)

/*

  Find a function address in library with nid

  Parameters :	modname		->	Name of module which contains library
				libname		->	Name of library which contains nid
				nid			->	NID of function to find

  Return :		Address of function or 0 if error

*/

{
 SceModule *modP;
 SceLibraryEntryTable *entP;
 u32 *nidtable;
 int count, i;


 // Find module pointer
 modP = sceKernelFindModuleByName(modname);
 if (!(modP)) return 0;

 // Find entries
 entP = (SceLibraryEntryTable *) modP->ent_top;

 // Search NID
 while ((u32) entP < ((u32) modP->ent_top + modP->ent_size))
 {
  if ((entP->libname) && (!(strcmp(entP->libname,libname))))
  {
   count = entP->stubcount + entP->vstubcount;
   nidtable = (u32 *) entP->entrytable;

   for (i=0;i<count;i++)
   {
	// NID found
    if (nidtable[i] == nid)
    {
     return nidtable[count+i];
    }
   }
   return 0;
  }
  entP++;
 }

 return 0;
}

u32 psarInitSysEntries (void)

/*

  Get the decrypt functions in library

  Parameters :	Nothing

  Return :		0			->	OK
				1			->	Not all found functions

*/

{
 // If already initialized
 if (psarSystemEntries) return 0;

 // dynamic/explicit lookup

 g_decodeProc6 = (PSAR_PROC_DECODE) psarFindProc("sceMesgLed","sceNwman_driver",0x9555d68d);
 g_decodeProcE = (PSAR_PROC_DECODE) psarFindProc("sceMesgLed","sceMesgd_driver",0x102dc8af);
 g_mangleProc = (PSAR_PROC_MANGLE) psarFindProc("sceMemlmd","semaphore",0x4c537c72);
 g_sceDeflateDecompress = (PSAR_PROC_DEFLATE) psarFindProc("sceKernelUtils","UtilsForKernel",0xe8db3ce6);
 g_sceGzipDecompress = (PSAR_PROC_DEFLATE) psarFindProc("sceKernelUtils","UtilsForKernel",0x78934841);

 psarSystemEntries = ((g_decodeProc6) && (g_decodeProcE) && (g_mangleProc) && (g_sceDeflateDecompress) && (g_sceGzipDecompress)) ? 1 : 0;

 return (psarSystemEntries) ? 0 : 1;
}

static PsarTagInfo const* psarGetTagInfo (u32 tagFind)

/*

  Find a tag info

  Parameters :	tagFind		->	Number of tag to find

  Return :		Pointer to tag found or NULL if not found

*/

{
 int iTag, count;


 // Search tag
 count = sizeof(g_tagInfo) / sizeof(PsarTagInfo);
 for (iTag=0;iTag<count;iTag++)
 {
  if (g_tagInfo[iTag].tag == tagFind) return &g_tagInfo[iTag];
 }

 // Not found
 return NULL;
}

static void psarExtraV2Mangle (u8 *buffer1, u8 codeExtra)

/*

  ?

  Parameters :	buffer1		->	Buffer to ?
				codeExtra	->	?

  Return :		Nothing

*/

{
 static u8 g_dataTmp[20+0xA0] __attribute__((aligned(0x40)));
 u32 *pl;
 u8 *buffer2;


 buffer2 = g_dataTmp;
 memcpy(buffer2 + 20,buffer1,0xA0);

 pl = (u32 *) buffer2;
 pl[0] = 5;
 pl[1] = pl[2] = 0;
 pl[3] = codeExtra;
 pl[4] = 0xA0;

 (*g_mangleProc)(buffer2,20 + 0xA0,buffer2,20 + 0xA0,7);

 // copy result back
 memcpy(buffer1,buffer2,0xA0);
}

static u32 psarMyDecryptPRX (const u8 *pbIn, u8 *pbOut, int cbTotal, u32 tag)

/*

  Decrypt a encrypted module

  Parameters :	pbIn		->	Buffer to decrypt
				pbOut		->	Buffer which contains decrypted data
				cbTotal		->	Size of encrypted buffer
				tag			->	Tag used for decrypt

  Return :		Size of decrypted buffer or -1 if error

*/

{
 PsarTagInfo const *pti;
 u32 *pl;
 u8 buffer1[0x150];
 int iXOR;


 pti = psarGetTagInfo(tag);
 if (!(pti)) return -1;

 // build conversion into pbOut
 memcpy(pbOut,pbIn,cbTotal);
 memset(pbOut,0,0x150);
 memset(pbOut,0x55,0x40);			// first $40 bytes ignored

 // step3 demangle in place
 pl = (u32 *) (pbOut + 0x2C);
 pl[0] = 5;							// number of ulongs in the header
 pl[1] = pl[2] = 0;
 pl[3] = pti->code;					// initial seed for PRX
 pl[4] = 0x70;						// size

 // redo part of the SIG check (step2)
 memcpy(buffer1 + 0x00,pbIn + 0xD0,0x80);
 memcpy(buffer1 + 0x80,pbIn + 0x80,0x50);
 memcpy(buffer1 + 0xD0,pbIn + 0x00,0x80);

 if (pti->codeExtra) psarExtraV2Mangle(buffer1 + 0x10,pti->codeExtra);

 memcpy(pbOut + 0x40,buffer1 + 0x40,0x40);
    
 for (iXOR=0;iXOR<0x70;iXOR++) pbOut[0x40 + iXOR] = pbOut[0x40 + iXOR] ^ pti->key[0x14 + iXOR];

 if ((*g_mangleProc)(pbOut + 0x2C,20 + 0x70,pbOut + 0x2C,20 + 0x70,7)) return -1;

 for (iXOR=0x6F;iXOR>=0;iXOR--) pbOut[0x40 + iXOR] = pbOut[0x2C + iXOR] ^ pti->key[0x20 + iXOR];

 memset(pbOut + 0x80,0,0x30); // $40 bytes kept, clean up
 pbOut[0xA0] = 1;

 // copy unscrambled parts from header
 memcpy(pbOut + 0xB0,pbIn + 0xB0,0x20); // file size + lots of zeros
 memcpy(pbOut + 0xD0,pbIn + 0x00,0x80); // ~PSP header

 // step4: do the actual decryption of code block
 //  point 0x40 bytes into the buffer to key info
 if ((*g_mangleProc)(pbOut,cbTotal,pbOut + 0x40,cbTotal - 0x40,0x1)) return -1;

 return *((u32 *) &pbIn[0xB0]);		// size of actual data (fix thanks to Vampire)
}

static u32 psarDecryptIPL1 (const u8 *pbIn, u8 *pbOut, int cbIn)

/*

  IPL decode logic - thanks to 'nem' for more details see "http://forums.ps2dev.org/viewtopic.php?t=3573" part1_/part2_ and part3_ prefix different steps of decryption process
  Decrypt the part1 of IPL file

  Parameters :	pbIn		->	Buffer to decrypt
				pbOut		->	Buffer which contains decrypted data
				cbIn		->	Size of encrypted buffer

  Return :		Size of decrypted buffer

*/

{
 static u8 g_dataTmp[0x1040] __attribute__((aligned(0x40)));		// 0x1000 pages
 int cbOut = 0;


 while (cbIn >= 0x1000)
 {
  // Copy data in buffer
  memcpy(g_dataTmp+0x40, pbIn, 0x1000);

  // Decrypt
  if ((*g_mangleProc)(g_dataTmp,0x1040,g_dataTmp + 0x40,0x1000,1)) break; // stop, save what we can

  // Copy result
  memcpy(pbOut, g_dataTmp, 0x1000);

  // Update pointer and counter
  pbIn += 0x1000;
  cbIn -= 0x1000;
  pbOut += 0x1000;
  cbOut += 0x1000;
 }

 return cbOut;
}

static u32 psarLinearizeIPL2 (const u8 *pbIn, u8 *pbOut, int cbIn)

/*

  Linearize the part2 of IPL file

  Parameters :	pbIn		->	Buffer to linearize
				pbOut		->	Buffer which contains linearized data
				cbIn		->	Size of initial buffer

  Return :		Size of linearized buffer or 0 if error

*/

{
 u32 nextAddr = 0, addr, count;
 int cbOut = 0;
 u32* pl;


 while (cbIn > 0)
 {
  pl = (u32 *) pbIn;
  addr = pl[0];

  if ((addr != nextAddr) && (nextAddr)) return 0;   // error

  count = pl[1];
  nextAddr = addr + count;
  memcpy(pbOut,pbIn + 0x10,count);

  pbOut += count;
  cbOut += count;
  pbIn += 0x1000;
  cbIn -= 0x1000;
 }

 return cbOut;
}

static u32 psarDecryptIPL3 (const u8 *pbIn, u8 *pbOut, int cbIn)

/*

  Decrypt the part3 of IPL file

  Parameters :	pbIn		->	Buffer to decrypt
				pbOut		->	Buffer which contains decrypted data
				cbIn		->	Size of encrypted buffer

  Return :		Size of decrypted buffer or 0 if error

*/

{
 // all together now (pbIn/pbOut must be aligned)
 pbIn += 0x10000;
 cbIn -= 0x10000;

 memcpy(pbOut + 0x40,pbIn,cbIn);

 if ((*g_mangleProc)(pbOut,cbIn + 0x40,pbOut + 0x40,cbIn,1)) return 0;

 return *((u32 *) &pbIn[0x70]);		// true size
}

static void psarDemangle (const u8 *pbIn, u8 *pbOut)

/*

  Demangle a buffer

  Parameters :	pbIn		->	Buffer to demangle
				pbOut		->	Buffer which contains demangled data
				cbIn		->	Size of initial buffer

  Return :		Nothing

*/

{
 u8 buffer[20 + 0x130];
 u32 *pl;


 memcpy(buffer + 20,pbIn,0x130);
 pl = (u32 *) buffer;				// first 20 bytes

 pl[0] = 5;
 pl[1] = pl[2] = 0;
 pl[3] = 0x55;
 pl[4] = 0x130;

 (*g_mangleProc)(buffer,20 + 0x130,buffer,20 + 0x130,7);

 memcpy(pbOut,buffer,0x130);
}

u32 psarDecodeBlock (const u8* pbIn, int cbIn, u8* pbOut)

/*

  Decode encrypted data blocks, minimal support for PSAR files keys #6 (old) or #14 (new) for 1.50 and later, they mangled the plaintext parts of the header

  Parameters :	pbIn		->	Buffer to decode
				cbIn		->	Size of encoded buffer
				pbOut		->	Buffer which contains decoded data

  Return :		Size of decoded buffer or -1 if error

*/

{
 int cbOut;


 // pOut also used as temporary buffer for mangled input
 // assert((((u32)pOut) & 0x3F) == 0); // must be aligned

 // copy a little more for $10 page alignment
 memcpy(pbOut,pbIn,cbIn + 0x10);

 if (g_bOldSchool)
 {
  // old style - unscrambled PSAR (1.00 "BOGUS" update)
  if (*((u32 *) &pbOut[0xd0]) != 0x06000000) return -1;

  (*g_decodeProc6)(pbOut,cbIn,&cbOut);
 }
 else
 {
  // new style - scrambled PSAR (works with 1.5x and 2.00)
  psarDemangle(pbIn + 0x20,pbOut + 0x20);							// demangle the inside $130 bytes
  if (*((u32 *) &pbOut[0xd0]) != 0x0E000000) return -1;

  (*g_decodeProcE)(pbOut,cbIn,&cbOut);
 }

 return cbOut;
}

static void psarPrintHeader (void)

/*

  Print installation file header

  Parameters :	Nothing

  Return :		Nothing

*/

{
 psarWriteInstallFile(";\r\n");
 psarWriteInstallFile("; MPHFL Firmware installation file\r\n");
 psarWriteInstallFile(";\r\n");
 psarWriteInstallFile("; Created with modified PSAR Dump .02A\r\n");
 psarWriteInstallFile(";  a PspPet utility\r\n");
 psarWriteInstallFile(";\r\n");
 psarWriteInstallFile("; LEGAL NOTICE:\r\n");
 psarWriteInstallFile(";\r\n");
 psarWriteInstallFile(";  the files extracted are Sony copyrighted material\r\n");
 psarWriteInstallFile(";  do not post them on the web or share them\r\n");
 psarWriteInstallFile(";  for legal DMCA compatible \"fair use\" uses only\r\n");
 psarWriteInstallFile(";\r\n\r\n\r\n");
}

u32 psarInstallFirmware (const char *pbpfile, const char *destfolder, const char *f0folder, u32 delpbp)

/*

  Install a firmware to a folder

  Parameters :	pbpfile		->	Name of PBP file to install
				destfolder	->	Name of destination folder (without '/' at end)
				f0folder	->	Name of flash0 folder (in destination folder, without '/')
				delpbp		->	If != 0, erase the pbp file source

  Return :		0			->	OK
				1			->	Bad parameters
				2			->	Unable to initialize decrypt functions
				3			->	Unable to allocate buffers for decrypt functions
				4			->	Unable to load PSAR file in PBP file
				5			->	Not a PSAR file in PBP file
				6			->	Unable to create installation file
				7			->	Failed to decode first data chunk
				8			->	Start PSAR chunk has invalid size
				9			->	Unable to save file first data chunk
				10			->	Failed to decode second data chunk
				11			->	Unable to save file second data chunk
				12			->	Process terminated but not all the files were saved

*/

{
 u32 *pl;
 char folder[256], temp[1024];
 char *szFileBase;
 int cbFile, cbOut, iBase, ret;
 u32 cbDataChunk, cbExpanded;
 u32 pbEnd;
 const u8 *pbIn;

#ifndef PSAR_USE_ENCRYPTED

 u32 tag;
 int cbDecrypted;
 u8 *pbToSave;
 int cbToSave;
 int cbExp;

#endif

 int isIpl;					// For ipl
 int cb1, cb2, cb3;
 int nSaved = 0;			// For stats
 int nSavedX = 0;
 int nErr = 0;
 int folderId = 0;			// ID of current folder


 // Verify parameters
 if ((!(pbpfile)) || (!(destfolder)) || (!(f0folder))) return 1;

 // if system entries not initialized
 if (!(psarSystemEntries)) return 2;

 // Allocate buffers
 g_dataPSAR = memalign(0x40,14000000);			// big enough for the full PSAR file
 if (!(g_dataPSAR)) return 3;

 g_dataOut = memalign(0x40,3000000);			// big enough for the largest (multiple uses)
 if (!(g_dataOut))
 {
  free(g_dataPSAR);

  return 3;
 }

 g_dataOut2 = memalign(0x40,3000000);			// for deflate output
 if (!(g_dataOut2))
 {
  free(g_dataOut);
  free(g_dataPSAR);

  return 3;
 }

 // Get file buffer
 if (psarLoadPsarFile(pbpfile,&cbFile))
 {
  free(g_dataOut2);
  free(g_dataOut);
  free(g_dataPSAR);

  return 4;
 }

 // Verify header
 if (memcmp(g_dataPSAR,PSAR_PSAR_MAGIC,strlen(PSAR_PSAR_MAGIC)))
 {
  free(g_dataOut2);
  free(g_dataOut);
  free(g_dataPSAR);

  return 5;
 }

 // Make destination folder
 strcpy(folder,destfolder);

 sceIoMkdir(folder,0777);
 strcat(folder,"/");

 strcpy(temp,folder);
 strcat(temp,PSAR_NAME_INSTALL);

 // Open install file
 if (psarOpenInstallFile(temp))
 {
  free(g_dataOut2);
  free(g_dataOut);
  free(g_dataPSAR);

  return 6;
 }

 // Delete PBP file
 if (delpbp) sceIoRemove(pbpfile);

 // Write install header
 psarPrintHeader();

 // Write main folder in installation file
 psarWriteInstallFile("[%s%d]\r\n\r\n%s = \"%s\"\r\n\r\n\r\n",PSAR_KEYNAME_FOLDER,folderId++,PSAR_VARNAME_FOLDER,destfolder);

 // at the start of the PSAR file,
 // there are one or two special version data chunks
 psarWriteInstallFile("; Special PSAR records\r\n\r\n");

 // old "bogus" update
 g_bOldSchool = (g_dataPSAR[4] == 1);

 cbOut = psarDecodeBlock(&g_dataPSAR[0x10],PSAR_OVERHEAD + PSAR_SIZE_A,g_dataOut);

 if (cbOut <= 0)
 {
  free(g_dataOut2);
  free(g_dataOut);
  free(g_dataPSAR);

  return 7;
 }

 if (cbOut != PSAR_SIZE_A)
 {
  free(g_dataOut2);
  free(g_dataOut);
  free(g_dataPSAR);

  return 8;
 }

 strcpy(temp,folder);
 strcat(temp,"data0.bin");

 if (psarSaveFile(temp,g_dataOut,cbOut))
 {
  free(g_dataOut2);
  free(g_dataOut);
  free(g_dataPSAR);

  return 9;
 }

 // After first entry iBase points to the next block to decode (0x10 aligned)
 iBase = 0x10 + PSAR_OVERHEAD + PSAR_SIZE_A;

 if (!(g_bOldSchool))
 {
  // second block
  cbOut = psarDecodeBlock(&g_dataPSAR[0x10 + PSAR_OVERHEAD + PSAR_SIZE_A],PSAR_OVERHEAD + 100,g_dataOut);
  if (cbOut <= 0)
  {
   free(g_dataOut2);
   free(g_dataOut);
   free(g_dataPSAR);

   return 10;
  }

  strcpy(temp,folder);
  strcat(temp,"data1.bin");

  if (psarSaveFile(temp,g_dataOut,cbOut))
  {
   free(g_dataOut2);
   free(g_dataOut);
   free(g_dataPSAR);

   return 11;
  }

  iBase += PSAR_OVERHEAD + ((cbOut + 15) & 0xFFFFFFF0);
 }

 // File loop
 strcat(folder,f0folder);

 // Create flash0 folder
 sceIoMkdir(folder,0777);

 // Write flash0 folder in installation file
 psarWriteInstallFile("\r\n[%s%d]\r\n\r\n%s = \"%s\"\r\n\r\n\r\n",PSAR_KEYNAME_FOLDER,folderId++,PSAR_VARNAME_FOLDER,folder);

 strcat(folder,"/");

 psarWriteInstallFile("; Regular Files\r\n\r\n");

 while (iBase < (cbFile - PSAR_OVERHEAD))
 {
  // Find name file
  cbOut = psarDecodeBlock(&g_dataPSAR[iBase],PSAR_OVERHEAD + PSAR_SIZE_A,g_dataOut);

  if (cbOut <= 0) break;
  if (cbOut != PSAR_SIZE_A) break;

  // Make good name
  strcpy(temp,folder);

  // Test for ipl files
  isIpl = (strncmp((const char *) &g_dataOut[4],"ipl:",4)) ? 0 : 1;

  szFileBase = strchr((const char *) &g_dataOut[4],'/');

  if (szFileBase)
  {
   strcat(temp,&szFileBase[1]);		 // after slash
   szFileBase = strchr(temp,'/');

   // Create folders
   while (szFileBase != NULL)
   {
	// Find next '/'
	szFileBase = strchr(&szFileBase[1],'/');

	// If found
	if (szFileBase)
	{
	 // Replace with 0
	 szFileBase[0] = 0;

	 // Create folder
	 if (!(sceIoMkdir(temp,0777))) psarWriteInstallFile("\r\n[%s%d]\r\n\r\n%s = \"%s\"\r\n\r\n",PSAR_KEYNAME_FOLDER,folderId++,PSAR_VARNAME_FOLDER,temp);

	 // Rewrite /
	 szFileBase[0] = '/';
	}
   }
  }
  else
   strcat(temp,"err.err");

  // pl[0] is 0
  // pl[1] is the PSAR chunk size (including PSAR_OVERHEAD)
  // pl[2] is true file size (TypeA=272=PSAR_SIZE_A, TypeB=size when expanded)
  // pl[3] is flags or version?
  pl = (u32 *) &g_dataOut[0x100];
  if (pl[0]) break;

  iBase += (PSAR_OVERHEAD + PSAR_SIZE_A);
  cbDataChunk = pl[1];				// size of next data chunk (including PSAR_OVERHEAD)
  cbExpanded = pl[2];				// size of file when expanded

  if (cbExpanded > 0)
  {
   cbOut = psarDecodeBlock(&g_dataPSAR[iBase],cbDataChunk,g_dataOut);

   // Deflate header
   if ((cbOut > 10) && (g_dataOut[0] == 0x78) && (g_dataOut[1] == 0x9C))
   {
    pbIn = &g_dataOut[2]; // after header
    
    ret = (*g_sceDeflateDecompress)(g_dataOut2,cbExpanded,pbIn,&pbEnd);
    if (ret == cbExpanded)
    {
	 // File encrypted
     if (!(memcmp(g_dataOut2,PSAR_PSP_MAGIC,strlen(PSAR_PSP_MAGIC))))
     {

#ifdef PSAR_USE_ENCRYPTED

      if (psarSaveFile(temp,g_dataOut2,cbExpanded))
      {
       psarWriteInstallFile("; WRITE ERROR - process aborted\r\n\r\n");
       break;
      }

      nSaved++;

#else

      tag = *((u32 *) &g_dataOut2[0xD0]);
      cbDecrypted = psarMyDecryptPRX(g_dataOut2,g_dataOut,cbExpanded,tag);		// output goes back to main buffer trashed 'g_dataOut2'

      if (cbDecrypted > 0)
	  {
       pbToSave = g_dataOut;
       cbToSave = cbDecrypted;

       if ((g_dataOut[0] == 0x1F) && (g_dataOut[1] == 0x8B))
       {
        cbExp = (*g_sceGzipDecompress)(g_dataOut2,/*sizeof(g_dataOut)*/3000000,g_dataOut,NULL);	// expanded result to g_dataOut

        if (cbExp > 0)
        {
         pbToSave = g_dataOut2;
         cbToSave = cbExp;
        }
        else
        {
         psarWriteInstallFile("; GZIP DECOMPRESS ERROR '%s' - saving what I can\r\n\r\n",temp);
         nErr++;
        }
       }

       if (psarSaveFile(temp,pbToSave,cbToSave))
       {
        psarWriteInstallFile("; WRITE ERROR - process aborted\r\n\r\n");
        nErr++;
        break;
       }

       nSavedX++;
      }
      else
      {
       psarWriteInstallFile("; DECRYPT ERROR\r\n\r\n");
       nErr++;
      }

#endif

     }
     else
	 {
      if (isIpl)
      {
	   // Decrypt part1
       cb1 = psarDecryptIPL1(g_dataOut2,g_dataOut,cbExpanded);
	   strcat(temp,".part1");

	   if ((cb1 > 0) && (!(psarSaveFile(temp,g_dataOut,cb1))))
       {
	    // Linearize part2
        cb2 = psarLinearizeIPL2(g_dataOut,g_dataOut2,cb1);
	    temp[strlen(temp) - 1] = '2';

	    psarSaveFile(temp,g_dataOut2,cb2);

	    // Decrypt part3
        cb3 = psarDecryptIPL3(g_dataOut2,g_dataOut,cb2);
	    temp[strlen(temp) - 1] = '3';

        psarSaveFile(temp,g_dataOut,cb3);
	   }
	  }
	  else
	  {
	   if (psarSaveFile(temp,g_dataOut2,cbExpanded))
       {
        psarWriteInstallFile("; WRITE ERROR - process aborted\r\n\r\n");
        break;
       }
       nSaved++;
	  }
	 }
	}
    else
     nErr++;
   }
   else
    nErr++;
  }
  else
  if (cbExpanded) break;

  // skip over data chunk
  iBase += cbDataChunk;
 }

 psarWriteInstallFile("; %d data files saved\r\n\r\n",nSaved);
 psarWriteInstallFile("; %d errors\r\n",nErr);

 // Close install file
 psarCloseInstallFile();

 // Free buffers
 free(g_dataOut2);
 free(g_dataOut);
 free(g_dataPSAR);

 return (g_bWriteError) ? 12 : 0;
}

u32 psarUninstallFirmware (const char *install)

/*

  Uninstall a firmware

  Parameters :	install		->	Name of installation file of firmware to uninstall

  Return :		0			->	OK
				1			->	Unable to load install file

*/

{
 ConfigFile file;
 ConfigKey *key;
 ConfigVariable *var;
 char md5[PSAR_SIZEDIGEST << 1];
 char folder[1024];
 int x;


 // Initialize variable
 file.opened = 0;

 // Load installation file
 if (configLoad(&file,install)) return 1;

 // File loop
 key = file.keys;

 while (key)
 {
  // Find variable
  var = configFindVariable(key,PSAR_VARNAME_MD5);
  
  if (var)
  {
   // Find md5
   if (!(psarFindMD5(key->name,md5)))
   {
    // Verify md5 and erase file
	if (!(strcasecmp((char *) var->value,md5))) sceIoRemove(key->name);
   }
  }

  // Next key
  key = key->next;
 }

 // Erase installation file
 sceIoRemove(install);

 // Count folders
 for (x=0;;x++)
 {
  // Create folder key name
  sprintf(folder,"%s%d",PSAR_KEYNAME_FOLDER,x);

  // Find key
  key = configFindKey(&file,folder);
  if (!(key)) break;
 }

 // Erase folders in invert order
 for (x--;x>=0;x--)
 {
  // Create folder key name
  sprintf(folder,"%s%d",PSAR_KEYNAME_FOLDER,x);

  // Find key
  key = configFindKey(&file,folder);
  if (!(key)) break;

  // Find variable
  var = configFindVariable(key,PSAR_VARNAME_FOLDER);

  // If variable found, erase folder
  if (var) sceIoRmdir((char *) var->value);
 }

 // Close file
 configClose(&file,0,NULL);

 return 0;
}

u32 psarCheckFirmware (const char *install, char *installfolder, char *version, u32 *good, u32 *lost, u32 *modified)

/*

  Check a firmware

  Parameters :	install			->	Name of installation file of firmware to check
				installfolder	->	if != NULL, will contains the install folder name
				version			->	if != NULL, will contains the firmware version
				good			->	if != NULL, will contains the number of good files
				lost			->	if != NULL, will contains the number of lost files
				modified		->	if != NULL, will contains the number of modified files

  Return :		0				->	OK
				1				->	Unable to load install file

*/

{
 ConfigFile file;
 ConfigKey *key, *keyver;
 ConfigVariable *var;
 char md5[PSAR_SIZEDIGEST << 1];
 char folder[16];
 int fd;


 // Initialize variable
 file.opened = 0;
 if (installfolder) installfolder[0] = 0;
 if (version) version[0] = 0;
 if (good) (*good) = 0;
 if (lost) (*lost) = 0;
 if (modified) (*modified) = 0;
 keyver = NULL;

 // Load installation file
 if (configLoad(&file,install)) return 1;

 // Find install folder name
 if (installfolder)
 {
  sprintf(folder,"%s0",PSAR_KEYNAME_FOLDER);
  if (!(configRead(&file,folder,PSAR_VARNAME_FOLDER,&var)))
  {
   if (var->type == CONFIG_TYPE_STRING) strcpy(installfolder,(const char *) var->value);
  }
 }

 // If key loop required
 if ((version) || (good) || (lost) || (modified))
 {
  // Key loop
  key = file.keys;
  while (key)
  {
   // If file key
   if (!(strncasecmp(key->name,"ms0:/",5)))
   {
    // Find version file
    if (strlen(key->name) > 11)
    {
     if (!(strcasecmp(&key->name[strlen(key->name) - 11],"version.txt"))) keyver = key;
    }

	// If md5 required
	if ((good) || (lost) || (modified))
	{
     // Find md5
     if (!(psarFindMD5(key->name,md5)))
     {
      // Check md5
	  var = configFindVariable(key,PSAR_VARNAME_MD5);

	  if (var)
	  { 
	   if (!(strncasecmp(md5,var->value,sizeof(md5))))
	   {
	    if (good) (*good)++;
	   }
	   else
	   {
	    if (modified) (*modified)++;
	   }
	  }
	  else
	  {
	   if (lost) (*lost)++;
	  }
     }
     else
     {
 	 if (lost) (*lost)++;
     }
	}
   }

   // Go to the next key
   key = key->next;
  }
 }

 // Find version
 if ((keyver) && (version))
 {
  fd = sceIoOpen(keyver->name,PSP_O_RDONLY,0);
  if (fd)
  {
   sceIoLseek(fd,8,SEEK_SET);

   sceIoRead(fd,version,4);
   sceIoClose(fd);

   version[4] = 0;
  }
 }

 // Close file
 configClose(&file,0,NULL);

 return 0;
}

u32 psarCreateInstallFile (const char *install, const char *f0folder)

/*

  Create an install.inf file with a flash0 folder

  Parameters :	install			->	Folder of installation file
				f0folder		->	Flash0 folder

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to create install file
				3				->	Error on listing files and folders

*/

{
 FileList list;
 char string[256];
 int x, folderId, error;


 // Verify parameters
 if ((!(install)) || (!(f0folder))) return 1;

 // Initialize variables
 folderId = 0;
 error = 0;

 // Create install file
 sprintf(string,"%s/%s",install,PSAR_NAME_INSTALL);

 // Open install file
 if (psarOpenInstallFile(string)) return 2;

 // Write install header
 psarPrintHeader();

 // Get the folder list
 if (!(fileGetList(f0folder,NULL,FILE_LIST_FOLDER | FILE_LIST_RECURSIVE,&list)))
 {
  psarWriteInstallFile("; Folders\r\n\r\n");

  // Write main folder in installation file
  psarWriteInstallFile("[%s%d]\r\n\r\n%s = \"%s\"\r\n\r\n\r\n",PSAR_KEYNAME_FOLDER,folderId++,PSAR_VARNAME_FOLDER,install);

  // Write flash0 folder in installation file
  psarWriteInstallFile("\r\n[%s%d]\r\n\r\n%s = \"%s\"\r\n\r\n\r\n",PSAR_KEYNAME_FOLDER,folderId++,PSAR_VARNAME_FOLDER,f0folder);

  // Write all folders
  for (x=0;x<list.count;x++)
  {
   // Remove last '/' in path
   list.list[x][strlen(list.list[x]) - 1] = 0;

   psarWriteInstallFile("\r\n[%s%d]\r\n\r\n%s = \"%s\"\r\n\r\n",PSAR_KEYNAME_FOLDER,folderId++,PSAR_VARNAME_FOLDER,list.list[x]);
  }
 }
 else
  error = 1;

 // Free the list
 fileFreeList(&list);

 // Get the file list
 if (!(fileGetList(f0folder,NULL,FILE_LIST_FILE | FILE_LIST_RECURSIVE,&list)))
 {
  psarWriteInstallFile("; Regular Files\r\n\r\n");

  // Write all files except install.inf
  for (x=0;x<list.count;x++)
  {
   if (!(strstr(list.list[x],PSAR_NAME_INSTALL)))
   {
    // Find md5
    psarFindMD5(list.list[x],string);

    // Write key in install file
    psarWriteInstallFile("\r\n[%s]\r\n\r\n%s = %s\r\n\r\n",list.list[x],PSAR_VARNAME_MD5,string);
   }
  }
 }
 else
  error = 1;

 // Free the list
 fileFreeList(&list);

 // Close install file
 psarCloseInstallFile();

 return (error) ? 3 : 0;
}
