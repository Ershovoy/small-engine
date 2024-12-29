@echo off

IF NOT EXIST .\data MKDIR.\data
IF NOT EXIST .\build MKDIR .\build

PUSHD .\build

DEL *.pdb > NUL 2> NUL

ECHO(
ECHO BUILDING
CALL ../build_windows.bat

POPD
