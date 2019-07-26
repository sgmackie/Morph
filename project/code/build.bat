echo Clang x64

@echo off

:: Set name of the platform .cpp file for unity building
set Platform=morph

:: Set build directory relative to current drive and path
set BuildDir="%~dp0..\build\win32"

:: Create build path if it doesn't exist
if not exist %BuildDir% mkdir %BuildDir%

:: Move to build directory
pushd %BuildDir%

:: Set compiler arguments
set PlatformFiles="%~dp0%Platform%.cpp"
set ObjDir=.\obj\

:: Create Object path if it doesn't exist
if not exist %ObjDir% mkdir %ObjDir%

:: Set compiler flags:
set CompilerFlags=-g -gcodeview -pedantic

:: Set warning labels:
set CommonWarnings=-Wall -Werror -Wno-four-char-constants -Wno-deprecated-declarations

:: Set Compiler optimsation level
set CompilerOpt=-O0

:: Set logging flags
set LogFlags=

:: Set profile flags
set ProfileFlags=

:: Set win32 libraries
set Libs=-lWs2_32.lib

:: Run Clang compiler
clang %CompilerFlags% %CommonWarnings% %CompilerOpt% %LogFlags% %ProfileFlags% %Libs% %PlatformFiles% -o %Platform%.exe

:: Exit
popd

REM I:\Projects\Current\Programming\Morph\project\data\BOOM_A I:\Projects\Current\Programming\Morph\project\data\BOOM_B 4 I:\Projects\Current\Programming\Morph\project\data\Output 1 0.5 0.9