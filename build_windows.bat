@echo off

SET no_c_runtime=0
SET no_c_extention=0
SET debug_info=1
SET fast_code=0
SET embed_assets=0
SET internal=1

IF %embed_assets%==1 (
    SET compiler_macros=/DEMBED_ASSETS
)

IF %no_c_runtime%==1 (
    SET compiler_macros=%compiler_macros% /DNO_C_RUNTIME

    REM Compiler options to get rid of C runtime library
    SET exclude_c_runtime_compiler_options=/GS- /Gs2147483647
    SET exclude_c_runtime_linker_options=/NODEFAULTLIB /STACK:0x100000,0x100000 /ENTRY:wWinMainCRTStartup /SUBSYSTEM:WINDOWS Kernel32.lib
    REM  Should I remove Kernel32.lib?
    SET no_c_runtime_debug_linker_flag=/NODEFAULTLIB /ENTRY:DllMainCRTStartup Kernel32.lib
)

IF %no_c_extention%==1 (
    SET compiler_macros=%compiler_macros% /DNO_C_EXTENTION
    SET others_flags=/Za
)

SET common_compiler_flags=/TC /nologo /FC /J

SET compiler_warning_options=/WX /W4 /wd4189 /wd4100 /wd4142 /wd4459 /wd4456 /wd4101

IF %debug_info%==1 (
    SET debug_flags=/Z7 /Od /MTd
)

IF %fast_code%==1 (
    SET optimization_flags=/Oi /Ot /Oy /Ob2 /GF /Gy
)

IF %internal%==1 (
    SET compiler_macros=%compiler_macros% /DINTERNAL
)

SET compiler_flags=%exclude_c_runtime_compiler_options% %common_compiler_flags% %compiler_warning_options% %optimization_flags% %debug_flags% %others_flags%
SET linker_flags=%exclude_c_runtime_linker_options% /incremental:no /opt:ref User32.lib Gdi32.lib ntdll.lib Ws2_32.lib Ole32.lib
SET debug_linker_flags=%no_c_runtime_debug_linker_flag% /incremental:no /opt:ref

IF %internal%==1 (
    CL %compiler_macros% %compiler_flags% ..\code\windows\main.c /link %linker_flags% /OUT:main.exe
    CL %compiler_macros% %compiler_flags% ..\code\windows\main.c /link %linker_flags% /OUT:main2.exe
    CL %compiler_macros% %compiler_flags% ..\code\windows\main.c /link %linker_flags% /OUT:main3.exe
) ELSE (
    CL %compiler_macros% %compiler_flags% /LD ..\code\windows\game_dll.c /link %debug_linker_flags% /PDB:game_%random%.pdb /OUT:game.dll
    ECHO(
    CL %compiler_macros% %compiler_flags% ..\code\windows\main.c /link %linker_flags% /OUT:main.exe
    REM
)

REM Remove rich signature header https://www.ntcore.com/files/richsign.htm /emittoolversioninfo:no /emitpogophaseinfo /emitvolatilemetadata:no
REM https://www.reddit.com/r/programming/comments/uo79gn/msvc_linker_option_to_remove_rich_headers_from/
REM https://github.com/ayaka14732/TinyPE-on-Win10
REM https://keyj.emphy.de/win32-pe/
REM https://learn.microsoft.com/en-us/archive/blogs/xiangfan/minimize-the-size-of-your-program-high-level
REM https://i.sstatic.net/sSU7U.png
REM https://learn.microsoft.com/en-us/cpp/build/reference/stub-ms-dos-stub-file-name?view=msvc-170
REM https://chatgpt.com/c/682990c3-7c90-800b-97c6-6f60bb82865f
REM https://youtu.be/5_UCkcb7iGY
REM /emittoolversioninfo:no /emitpogophaseinfo /emitvolatilemetadata:no /LTCG /nocoffgrpinfo /MERGE:.rdata=. /MERGE:.pdata=. /MERGE:.text=. /SECTION:.,ER
