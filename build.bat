@echo off
setlocal
set "ROOT=%~dp0"

where gcc >nul 2>nul
if errorlevel 1 (
    echo Error: GCC MinGW tidak ditemukan di PATH.
    exit /b 1
)

if not exist "%ROOT%include\raylib.h" (
    echo Error: Header include\raylib.h tidak ditemukan.
    exit /b 1
)
if not exist "%ROOT%include\raygui.h" (
    echo Error: Header include\raygui.h tidak ditemukan.
    exit /b 1
)
if not exist "%ROOT%lib\libraylib.a" (
    echo Error: Library lib\libraylib.a tidak ditemukan.
    exit /b 1
)
if not exist "%ROOT%assets\Inter.ttf" (
    echo Error: Font assets\Inter.ttf tidak ditemukan.
    exit /b 1
)

gcc -std=c99 -O2 -Wall -Wextra -mwindows "%ROOT%src\main.c" ^
    "%ROOT%src\finance.c" "%ROOT%src\ui.c" ^
    -I"%ROOT%src" -I"%ROOT%include" -L"%ROOT%lib" -o "%ROOT%finance_tracker.exe" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm
if errorlevel 1 (
    echo Build gagal.
    exit /b 1
)

echo Build berhasil: "%ROOT%finance_tracker.exe"
endlocal
