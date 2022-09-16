# Microsoft Developer Studio Project File - Name="MPHFL_Main" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=MPHFL_Main - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MPHFL_Main.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MPHFL_Main.mak" CFG="MPHFL_Main - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MPHFL_Main - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "MPHFL_Main - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "MPHFL_Main - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f MPHFL_Main.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "MPHFL_Main.exe"
# PROP BASE Bsc_Name "MPHFL_Main.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line ""make.bat""
# PROP Rebuild_Opt ""
# PROP Target_File "EBOOT.PBP"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "MPHFL_Main - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f MPHFL_Main.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "MPHFL_Main.exe"
# PROP BASE Bsc_Name "MPHFL_Main.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line ""make.bat""
# PROP Rebuild_Opt "/debug"
# PROP Target_File "EBOOT.PBP"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "MPHFL_Main - Win32 Release"
# Name "MPHFL_Main - Win32 Debug"

!IF  "$(CFG)" == "MPHFL_Main - Win32 Release"

!ELSEIF  "$(CFG)" == "MPHFL_Main - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "2D"

# PROP Default_Filter ""
# Begin Group "format"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\2D\format\imagebmp.c
# End Source File
# Begin Source File

SOURCE=.\2D\format\imagebmp.h
# End Source File
# Begin Source File

SOURCE=.\2D\format\imagepng.c
# End Source File
# Begin Source File

SOURCE=.\2D\format\imagepng.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\2D\font.c
# End Source File
# Begin Source File

SOURCE=.\2D\font.h
# End Source File
# Begin Source File

SOURCE=.\2D\graphic.c
# End Source File
# Begin Source File

SOURCE=.\2D\graphic.h
# End Source File
# Begin Source File

SOURCE=.\2D\image.c
# End Source File
# Begin Source File

SOURCE=.\2D\image.h
# End Source File
# Begin Source File

SOURCE=.\2D\vram.c
# End Source File
# Begin Source File

SOURCE=.\2D\vram.h
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utils\config.c
# End Source File
# Begin Source File

SOURCE=.\Utils\config.h
# End Source File
# Begin Source File

SOURCE=.\Utils\file.c
# End Source File
# Begin Source File

SOURCE=.\Utils\file.h
# End Source File
# Begin Source File

SOURCE=.\Utils\language.c
# End Source File
# Begin Source File

SOURCE=.\Utils\language.h
# End Source File
# Begin Source File

SOURCE=.\Utils\language_default.h
# End Source File
# Begin Source File

SOURCE=.\Utils\menu.c
# End Source File
# Begin Source File

SOURCE=.\Utils\menu.h
# End Source File
# Begin Source File

SOURCE=.\Utils\psardump.c
# End Source File
# Begin Source File

SOURCE=.\Utils\psardump.h
# End Source File
# Begin Source File

SOURCE=.\Utils\ram.c
# End Source File
# Begin Source File

SOURCE=.\Utils\ram.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\makefile
# End Source File
# End Target
# End Project
