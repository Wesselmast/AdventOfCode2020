@echo off

set compile_flags=-c -O2 -D_CRT_SECURE_NO_WARNINGS
set linker_paths=-L.\\lib
set include_paths=-I.\\include
set libraries=-lopengl32 -lglew32s -luser32 -lgdi32 

set version=%1
IF [%1] == [] set version=1
IF NOT EXIST .\bin\%version% mkdir .\bin\%version%
IF NOT EXIST .\int\%version% mkdir .\int\%version%

call duration -c .\clang\clang++ .\src\%version%\Main.cpp %compile_flags% %include_paths% -o .\int\%version%\Main.o
call duration -l .\clang\clang++ -o .\bin\%version%\Main.exe .\int\%version%\Main.o -Wl %linker_paths% %libraries%

robocopy .\res .\bin\%version%\res /E /NFL /NDL /NJH /NJS
