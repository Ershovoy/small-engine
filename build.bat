@echo off

IF NOT EXIST .\data MKDIR.\data
IF NOT EXIST .\build MKDIR .\build

PUSHD .\build

DEL *.pdb > NUL 2> NUL

ECHO(
ECHO BUILDING RELEASE
CALL ../build_release.bat

REM ECHO(
REM ECHO BUILDING DEBUG
REM CALL ../build_debug.bat

POPD
