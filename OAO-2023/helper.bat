@echo off
ml /nologo /c in.txt.asm
link /NOLOGO /out:asm.exe in.txt.obj ..\CES-2020\Debug\CES-2020L.lib /SUBSYSTEM:CONSOLE /IGNORE:4098 /IGNORE:4075
cls
call asm.exe
