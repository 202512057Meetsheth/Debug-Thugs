@echo off
title Cyber-Neon Snake - Part 5
color 0B
echo.
echo Building Cyber-Neon Snake - Part 5 ...
echo -------------------------------------------
echo.
g++ part5.cpp -o part5.exe -std=c++17 -O2
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b
)
echo Compilation successful!
echo.
echo Launching game...
echo.
chcp 65001 >nul
part5.exe
echo.
echo Game exited.
color 07
pause
