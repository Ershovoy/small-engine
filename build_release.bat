@echo off

REM Compiler options to get rid of C runtime library
SET exclude_c_runtime_compiler_options=/GS- /Gs2147483647
SET exclude_c_runtime_linker_options=/NODEFAULTLIB /STACK:0x100000,0x100000 /ENTRY:wWinMainCRTStartup /SUBSYSTEM:WINDOWS Kernel32.lib

SET common_compiler_flags=/TC /Za /nologo /FC /J

SET compiler_warning_options=/WX /W4 /wd4189 /wd4100 /wd4142 /wd4459 /wd4456

SET debug_flags=/Z7 /Od /MTd

REM SET optimization_flags=/Oi /Ot /Oy /Ob2 /GF /Gy

SET compiler_flags=%exclude_c_runtime_compiler_options% %common_compiler_flags% %compiler_warning_options% %optimization_flags% %debug_flags%
SET linker_flags=%exclude_c_runtime_linker_options% /incremental:no /opt:ref User32.lib Gdi32.lib ntdll.lib

CL /DRELEASE %compiler_flags% ..\code\windows\main.c /link %linker_flags% /OUT:windows_main_release.exe
