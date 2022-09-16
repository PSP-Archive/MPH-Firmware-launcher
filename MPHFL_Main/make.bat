
@REM Ne pas afficher les commandes
@ECHO OFF

ECHO Compilation du projet

SET path=%path%;C:\cygwin\bin;C:\cygwin\usr/local/pspdev/bin
SET PSPSDK=C:\cygwin\usr/local/pspdev

make.exe

ECHO Effacer les fichiers inutiles
DEL *.o

ECHO Trouver le bon repertoire

REM SET DRIVE=.\
SET DRIVE=G:\PSP\GAME\



ECHO Création du PBP
MD %DRIVE%MPHFL%%
Elf2pbpm -FILEOUT=%DRIVE%MPHFL%%\EBOOT.PBP -TITLE="MPH Firmware launcher 1.4.0" -ICON0=".\PBP\ICON0.PNG" -PIC1=".\PBP\PIC1.PNG"

ECHO Renommer en EBOOT.PBP
MD %DRIVE%MPHFL
MOVE out.elf %DRIVE%MPHFL\EBOOT.PBP

pause