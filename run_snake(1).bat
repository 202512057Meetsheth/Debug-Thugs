@echo off
title Cyber-Neon Snake - Part 52
color 0B
echo.
echo Building Cyber-Neon Snake - Part 52 ...
echo -------------------------------------------
echo.
g++ part52.cpp -o part52.exe -std=c++17 -O2
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
part52.exe
echo.
echo Game exited.
color 07
pause
