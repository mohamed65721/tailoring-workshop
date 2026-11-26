@echo off
rem Double-click to run the Tailoring Order Workshop.
cd /d "%~dp0"
if not exist tailor.exe (
    echo tailor.exe not found. Run build.bat first.
    pause
    exit /b 1
)
"%~dp0tailor.exe"
pause
