@echo off

SET common_compiler_flags=/TC /std:clatest /nologo /FC /J

SET compiler_warning_options=/WX /W4 /wd4189 /wd4100 /wd4142 /wd4459 /wd4456

SET debug_flags=/Z7 /Od /MTd

SET compiler_flags=%compiler_warning_options% %common_compiler_flags% %debug_flags%
SET linker_flags=/incremental:no /opt:ref

PUSHD .\build

CL %compiler_flags% ..\code\meta.c /link %linker_flags% /OUT:embed_assets.exe

POPD

PUSHD .\data

CALL ..\build\embed_assets.exe .\

MOVE .\virtual_file_system.h ..\code

POPD