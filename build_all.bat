@echo off

FOR /L %%v IN (1, 1, 25) DO IF EXIST .\src\%%v build.bat %%v

exit
