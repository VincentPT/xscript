@echo off
set VC_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
set CUR_DIRR=%cd%

cd %VC_DIR%
call vcvarsall.bat x64

cd %CUR_DIRR%

nmake -f Makefile

@echo on