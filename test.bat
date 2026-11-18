@echo off
setlocal
set "ROOT=%~dp0"

where gcc >nul 2>nul
if errorlevel 1 (
    echo Error: GCC MinGW tidak ditemukan di PATH.
    exit /b 1
)

gcc -std=c99 -O2 -Wall -Wextra "%ROOT%src\finance.c" "%ROOT%tests\test_finance.c" ^
    -I"%ROOT%src" -o "%TEMP%\finance_tracker_tests.exe"
if errorlevel 1 (
    echo Kompilasi tes gagal.
    exit /b 1
)

"%TEMP%\finance_tracker_tests.exe"
set "TEST_RESULT=%ERRORLEVEL%"
del "%TEMP%\finance_tracker_tests.exe"
exit /b %TEST_RESULT%
