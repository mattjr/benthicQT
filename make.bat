@echo off

set OSG_DIR=c:\OpenSceneGraph-2.8.2
set Qwt5_INCLUDE_DIR=c:\qwt-5.2.0\include
set Qwt5_Qt4_LIBRARY_DIR=c:\qwt-5.2.0\lib


if "%1"=="vs" goto VS
if "%1" == "debug" goto DEBUG
if "%1" == "clean" goto CLEAN

goto NMAKE


:NMAKE
set BDIR=build
if EXIST %BDIR% rmdir /s /q %BDIR%
mkdir %BDIR%
cd %BDIR%

cmake -G"NMake Makefiles" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%CD% -DOSG_DIR:PATH=%OSG_DIR%
nmake
nmake package 
GOTO FINISH

:DEBUG
set BDIR=build.debug
if EXIST %BDIR% rmdir /s /q %BDIR%
mkdir %BDIR%
cd %BDIR%
cmake -G"NMake Makefiles" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%CD% -DOSG_DIR:PATH=%OSG_DIR%
nmake
nmake package 
GOTO FINISH


:VS
set BDIR=build.vs
if EXIST %BDIR% rmdir /s /q %BDIR%
mkdir %BDIR%
cd %BDIR%
cmake -G"Visual Studio 9 2008" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%CD% -DOSG_DIR:PATH=%OSG_DIR%
start VisualizePhysics-Wave.sln
cd ..
GOTO FINISH


:CLEAN
if EXIST build rmdir /s build
if EXIST build.debug rmdir /s build.debug
if EXIST build.vs rmdir /s build.vs

:FINISH
echo Done.

