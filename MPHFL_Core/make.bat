
@REM Ne pas afficher les commandes
@ECHO OFF

ECHO Compilation du projet

SET path=%path%;C:\cygwin\bin;C:\cygwin\usr/local/pspdev/bin
SET PSPSDK=C:\cygwin\usr/local/pspdev

make.exe

ECHO Effacer les fichiers inutiles
DEL *.o

ECHO Trouver le bon repertoire

SET DRIVE=G:\PSP\GAME\
REM SET DRIVE=.\

MOVE out.elf %DRIVE%MPHFL\mphfl.elf

pause