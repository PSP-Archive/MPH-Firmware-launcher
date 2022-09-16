
/*

  Driver functions

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "driver.h"


// GLOBALS VARIABLES

DriverConf drvConf;			// Driver configuration
int drvLog = -1;			// Fd of log file and also used to know if driverInstall was already called
char drvString[512];		// Temporary string to use


// *** FUNCTIONS ***

char* createPath (const char *in, int fs_num)
{
 strcpy(drvString,drvConf.fs);												// {drvConf.fs}
 strcat(drvString,((fs_num & 0x1) ? drvConf.folderF1 : drvConf.folderF0));	// {drvConf.fs}{drvConf.folderF0,1}
 strcat(drvString,in);														// {drvConf.fs}{drvConf.folderF0,1}{in}

 return drvString;
}

int driver_IoInit (PspIoDrvArg* arg)
{
 return 0x0;
}

int driver_IoExit (PspIoDrvArg* arg)
{
 return 0x0;
}

SceUID driver_IoOpen (PspIoDrvFileArg *arg, char *file, int flags, SceMode mode)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoOpen -> (name: %s), (flags: 0x%X), (mode: 0x%X)\r\n",file,flags,(int) mode);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 arg->arg = (void *) sceIoOpen(createPath(file,arg->fs_num),flags,mode);

 return (SceUID) arg->arg;
}

int driver_IoClose (PspIoDrvFileArg *arg)
{
 int r;


 // log
 if (drvLog)
 {
  sprintf(drvString,"IoClose -> (fd: 0x%X)\r\n",(SceUID) arg->arg);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 // Close fd and save return
 r = sceIoClose((SceUID) arg->arg);

 // Set fd to 0
 arg->arg = (void *) 0x0;

 return r;
}

int driver_IoRead (PspIoDrvFileArg *arg, char *data, int len)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoRead -> (fd: 0x%X), (data addr: 0x%X), (len: 0x%X)\r\n",(SceUID) arg->arg,(int) data,len);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoRead((SceUID) arg->arg,data,len);
}

int driver_IoWrite (PspIoDrvFileArg *arg, const char *data, int len)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoWrite -> (fd: 0x%X), (data: %s), (len: 0x%X)\r\n",(SceUID) arg->arg,data,len);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoWrite((SceUID) arg->arg,data,len);
}

SceOff driver_IoLseek (PspIoDrvFileArg *arg, SceOff ofs, int whence)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoLseek -> (fd: 0x%X), (ofs: 0x%X), (whence: 0x%X)\r\n",(SceUID) arg->arg,(int) ofs,whence);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoLseek((SceUID) arg->arg,ofs,whence);
}

int driver_IoIoctl (PspIoDrvFileArg *arg, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen)
{
// A tester
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoIoctl -> (fd: 0x%X), (cmd: 0x%X), (indata addr: 0x%X), (inlen: 0x%X), (outdata addr: 0x%X), (outlen: 0x%X)\r\n",(SceUID) arg->arg,cmd,(int) indata,inlen,(int) outdata,outlen);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoIoctl((SceUID) arg->arg,cmd,indata,inlen,outdata,outlen);
}

int driver_IoRemove (PspIoDrvFileArg *arg, const char *name)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoRemove -> (name: %s)\r\n",name);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoRemove(createPath(name,arg->fs_num));
}

int driver_IoMkdir (PspIoDrvFileArg *arg, const char *name, SceMode mode)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoMkdir -> (name: %s), (mode: 0x%X)\r\n",name,(int) mode);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoMkdir(createPath(name,arg->fs_num),mode);
}

int driver_IoRmdir (PspIoDrvFileArg *arg, const char *name)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoRmdir -> (name: %s)\r\n",name);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoRmdir(createPath(name,arg->fs_num));
}

SceUID driver_IoDopen (PspIoDrvFileArg *arg, const char *dirname)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoDopen -> (dirname: %s)\r\n",dirname);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 arg->arg = (void *) sceIoDopen(createPath(dirname,arg->fs_num));

 return (SceUID) arg->arg;
}

int driver_IoDclose (PspIoDrvFileArg *arg)
{
 int r;


 // log
 if (drvLog)
 {
  sprintf(drvString,"IoDclose -> (fd: 0x%X)\r\n",(SceUID) arg->arg);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 // Close fd and save return
 r = sceIoDclose((SceUID) arg->arg);

 // Set fd to 0
 arg->arg = (void *) 0x0;

 return r;
}

int driver_IoDread (PspIoDrvFileArg *arg, SceIoDirent *dir)
{
// a verifier
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoDread -> (fd: 0x%X), (dir addr: 0x%X)\r\n",(SceUID) arg->arg,(int) dir);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoDread((SceUID) arg->arg,dir);
}

int driver_IoGetstat (PspIoDrvFileArg *arg, const char *file, SceIoStat *stat)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoGetstat -> (file: %s), (stat addr: 0x%X)\r\n",file,(int) stat);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoGetstat(createPath(file,arg->fs_num),stat);
}

int driver_IoChstat (PspIoDrvFileArg *arg, const char *file, SceIoStat *stat, int bits)
{
// A verifier
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoChstat -> (file: %s), (stat addr: 0x%X), (bits: 0x%X)\r\n",file,(int) stat,bits);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoChstat(createPath(file,arg->fs_num),stat,bits);
}

int driver_IoRename (PspIoDrvFileArg *arg, const char *oldname, const char *newname)
{
 char old[DRIVER_SIZE_PATH];


 // log
 if (drvLog)
 {
  sprintf(drvString,"IoRename -> (oldname: %s), (newname: %s)\r\n",oldname,newname);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 // Create the old name
 strcpy(old,createPath(oldname,arg->fs_num));

 return sceIoRename(old,createPath(newname,arg->fs_num));
}

int driver_IoChdir (PspIoDrvFileArg *arg, const char *dir)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoChdir :-> (dir: %s)\r\n",dir);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoChdir(createPath(dir,arg->fs_num));		// chdir don't called ?
}

int driver_IoMount (PspIoDrvFileArg *arg)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoMount\r\n");
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return 0x0;
}

int driver_IoUmount (PspIoDrvFileArg *arg)
{
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoUmount\r\n");
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return 0x0;
}

int driver_IoDevctl (PspIoDrvFileArg *arg, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen)
{
// A tester
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoDevctl -> (cmd: 0x%X), (indata addr: 0x%X), (inlen: 0x%X), (outdata addr: 0x%X), (outlen: 0x%X)\r\n",cmd,(int) indata,inlen,(int) outdata,outlen);
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return sceIoDevctl(createPath("",arg->fs_num)/*"ms0:"*/,cmd,indata,inlen,outdata,outlen);
}

int driver_IoUnk21 (PspIoDrvFileArg *arg)
{
 // ?
 // log
 if (drvLog)
 {
  sprintf(drvString,"IoUnk21\r\n");
  sceIoWrite(drvLog,drvString,strlen(drvString));
 }

 return 0x0;
}

u32 driverInstall (DriverConf *conf, u32 logfile)
{
 static PspIoDrv driver;		// Static declaration because sceIoAddDrv use address of this variable instead to make a copy
 static PspIoDrvFuncs drvfuncs;	// Static declaration because sceIoAddDrv use address of this variable instead to make a copy
 char drvname[DRIVER_SIZE_NAME], drvpatch[DRIVER_SIZE_NAME];
 int x;
 

 // Set configuration
 strcpy(drvConf.name,DRIVER_DEFAULT_NAME);
 strcpy(drvConf.nameCaps,DRIVER_DEFAULT_NAMECAPS);
 strcpy(drvConf.patched,DRIVER_DEFAULT_PATCHED);
 strcpy(drvConf.phys,DRIVER_DEFAULT_PHYS);
 strcpy(drvConf.fs,DRIVER_DEFAULT_FS);
 strcpy(drvConf.logfile,DRIVER_DEFAULT_LOGFILE);
 strcpy(drvConf.folderF0,DRIVER_DEFAULT_FOLDERF0);
 strcpy(drvConf.folderF1,DRIVER_DEFAULT_FOLDERF1);

 if (conf)
 {
  if (conf->name[0]) strcpy(drvConf.name,conf->name);
  if (conf->nameCaps[0]) strcpy(drvConf.nameCaps,conf->nameCaps);
  if (conf->patched[0]) strcpy(drvConf.patched,conf->patched);
  if (conf->phys[0]) strcpy(drvConf.phys,conf->phys);
  if (conf->fs[0]) strcpy(drvConf.fs,conf->fs);
  if (conf->logfile[0]) strcpy(drvConf.logfile,conf->logfile);
  if (conf->folderF0[0]) strcpy(drvConf.folderF0,conf->folderF0);
  if (conf->folderF1[0]) strcpy(drvConf.folderF1,conf->folderF1);
 }

 // Correct the path
 if (drvConf.folderF0[0] != '/')
 {
  sprintf(drvString,"/%s",drvConf.folderF0);
  strcpy(drvConf.folderF0,drvString);
 }

 if (drvConf.folderF0[strlen(drvConf.folderF0) - 1] == '/') drvConf.folderF0[strlen(drvConf.folderF0) - 1] = 0;

 if (drvConf.folderF1[0] != '/')
 {
  sprintf(drvString,"/%s",drvConf.folderF1);
  strcpy(drvConf.folderF1,drvString);
 }

 if (drvConf.folderF1[strlen(drvConf.folderF1) - 1] == '/') drvConf.folderF1[strlen(drvConf.folderF1) - 1] = 0;


 // Driver functions
 memset(&drvfuncs,0,sizeof(drvfuncs));

 drvfuncs.IoInit = driver_IoInit;
 drvfuncs.IoExit = driver_IoExit;
 drvfuncs.IoOpen = driver_IoOpen;
 drvfuncs.IoClose = driver_IoClose;
 drvfuncs.IoRead = driver_IoRead;
 drvfuncs.IoWrite = driver_IoWrite;
 drvfuncs.IoLseek = (int (*)(PspIoDrvFileArg *, u32, long long, int)) driver_IoLseek;
 drvfuncs.IoIoctl = driver_IoIoctl;
 drvfuncs.IoRemove = driver_IoRemove;
 drvfuncs.IoMkdir = driver_IoMkdir;
 drvfuncs.IoRmdir = driver_IoRmdir;
 drvfuncs.IoDopen = driver_IoDopen;
 drvfuncs.IoDclose = driver_IoDclose;
 drvfuncs.IoDread = driver_IoDread;
 drvfuncs.IoGetstat = driver_IoGetstat;
 drvfuncs.IoChstat = driver_IoChstat;
 drvfuncs.IoRename = driver_IoRename;
 drvfuncs.IoChdir = driver_IoChdir;
 drvfuncs.IoMount = driver_IoMount;
 drvfuncs.IoUmount = driver_IoUmount;
 drvfuncs.IoDevctl = driver_IoDevctl;
 drvfuncs.IoUnk21 = driver_IoUnk21;

 // init structure
 driver.name = drvConf.name;
 driver.dev_type = 0x10;
 driver.unk2 = 0x800;
 driver.name2 = drvConf.nameCaps;
 driver.funcs = &drvfuncs;

 // Delete possible old driver
 drvLog = 0;
 driverUninstall(1);

 // Create the new driver
 if (sceIoAddDrv(&driver)) return -1;

 // Assign flash (0 a 9)
 for (x=0;x<10;x++)
 {
  // Find driver name
  sprintf(drvname,"%s%d:",drvConf.name,(x & 0x1));

  // Find driver patch
  sprintf(drvpatch,"%s%d:",drvConf.patched,x);

  // Assign driver
  sceIoUnassign(drvpatch);
  sceIoAssign(drvpatch,drvConf.phys,drvname,IOASSIGN_RDWR,NULL,0);
 }

 // Open log file if wanted
 if ((logfile) && (drvConf.logfile[0]))
 {
  drvLog = sceIoOpen(drvConf.logfile,PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC,0777);
  if (drvLog <= 0) drvLog = 0;
 }

 return 0;
}

u32 driverUninstall (u32 deldrv)
{
 char drvpatch[DRIVER_SIZE_NAME];
 int x;


 // Don't call it if you don't call driverInstall before (for drvConf data coherency)
 if (drvLog == -1) return 1;

 // Close logfile
 if (drvLog) sceIoClose(drvLog);
 drvLog = 0;

 // Unassign flash (0 to 9)
 for (x=0;x<10;x++)
 {
  // Find driver patch
  sprintf(drvpatch,"%s%d:",drvConf.patched,x);

  // Unassign driver
  sceIoUnassign(drvpatch);
 }

 if (deldrv) return sceIoDelDrv(drvConf.name); // ne supporte pas, peut etre a cause de trop de fichiers ouvert avec ce driver (modules chargés) ??

 return 0;
}
