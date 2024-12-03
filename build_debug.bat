@echo off

SET common_compiler_flags=/TC /std:clatest /nologo /FC /J

SET compiler_warning_options=/WX /W4 /wd4189 /wd4100 /wd4142 /wd4459 /wd4456  /wd4702

SET debug_flags=/Z7 /Od /MTd

SET compiler_flags=%compiler_warning_options% %common_compiler_flags% %debug_flags%
SET linker_flags=/incremental:no /opt:ref User32.lib Gdi32.lib ntdll.lib d3d11.lib windowscodecs.lib dxguid.lib

CL /DDEBUG %compiler_flags% /LD ..\code\game_debug.c /link %linker_flags% /PDB:game_%random%.pdb
CL /DDEBUG %compiler_flags% ..\code\windows_main.c /link %linker_flags% /OUT:windows_main_debug.exe

fxc /T vs_5_0 /E vertex_shader /Fo ../data/vertex_shader.cso /nologo ..\code\shader.hlsl
fxc /T ps_5_0 /E pixel_shader /Fo ../data/pixel_shader.cso /nologo ..\code\shader.hlsl
