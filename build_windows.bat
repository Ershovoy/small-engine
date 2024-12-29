@echo off

SET debug_info=1
SET no_c_runtime=1
SET fast_code=0
SET internal=1
SET embed_assets=0

IF %embed_assets%==1 (
    SET compiler_macros=/DEMBED_ASSETS
)

IF %no_c_runtime%==1 (
    SET compiler_macros=%compiler_macros% /DNO_C_RUNTIME

    REM Compiler options to get rid of C runtime library
    SET exclude_c_runtime_compiler_options=/GS- /Gs2147483647
    SET exclude_c_runtime_linker_options=/NODEFAULTLIB /STACK:0x100000,0x100000 /ENTRY:wWinMainCRTStartup /SUBSYSTEM:WINDOWS Kernel32.lib
)

SET common_compiler_flags=/TC /Za /nologo /FC /J

SET compiler_warning_options=/WX /W4 /wd4189 /wd4100 /wd4142 /wd4459 /wd4456 /wd4101

IF %debug_info%==1 (
    SET debug_flags=/Z7 /Od /MTd
)

IF %fast_code%==1 (
    SET optimization_flags=/Oi /Ot /Oy /Ob2 /GF /Gy
)

SET compiler_flags=%exclude_c_runtime_compiler_options% %common_compiler_flags% %compiler_warning_options% %optimization_flags% %debug_flags%
SET linker_flags=%exclude_c_runtime_linker_options% /incremental:no /opt:ref User32.lib Gdi32.lib ntdll.lib

IF %internal%==1 (
    CL %compiler_macros% %compiler_flags% ..\code\windows\main.c /link %linker_flags%
) ELSE (
    REM CL %compiler_macros% %compiler_flags% /LD ..\code\game_debug.c /link %linker_flags% /PDB:game_%random%.pdb
    REM CL %compiler_macros% %compiler_flags% ..\code\windows_main.c /link %linker_flags% /OUT:windows_main_debug.exe
)
