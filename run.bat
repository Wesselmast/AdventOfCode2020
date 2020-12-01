@echo off

set version=%1
IF [%1] == [] set version=1

call bin\%version%\Main.exe
exit
