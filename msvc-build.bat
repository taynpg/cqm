@echo off
echo =============================================================
echo You should run this bat file in the ternimal named
echo Developer Command Prompt for VS 2022 or other version
echo or named such as VS2015 x64 Developer Tool etc.
echo then it works.
echo =============================================================
pause
rem if exist build (
rem    rd /s /q build
rem )
if exist cqm.exe (
    del /q cqm.exe
)
%~d0
cd  %~dp0
cl /EHsc /Fe%~dp0cqm.exe /source-charset:utf-8 /execution-charset:gbk /nologo  /I./include src/qimenzpzr.c src/qimenyp.c src/qimenbasic.c src/data.c src/comline.c src/calendar.c main.c
del *.obj
echo ===============================
echo Done!
echo ===============================
pause
