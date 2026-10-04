@echo off

set "MINGW=C:\msys64\mingw64\bin"
set "PATH=%MINGW%;%PATH%"

cd /d "%~dp0src"

"%MINGW%\gcc.exe" -c Indexer/sql/sqlite3.c -o Indexer/sql/sqlite3.o

if errorlevel 1 (
    echo SQLite compilation failed.
    pause
    exit /b 1
)

"%MINGW%\g++.exe" -std=c++17 -O2 ^
-o ..\search.exe ^
main.cpp ^
Indexer/sql_indexer.cpp ^
SearchEngine/SearchEngine.cpp ^
Utils/utils.cpp ^
Indexer/sql/sqlite3.o

if errorlevel 1 (
    echo C++ compilation failed.
    pause
    exit /b 1
)

echo.
echo =========================
echo Build successful!
echo =========================
pause