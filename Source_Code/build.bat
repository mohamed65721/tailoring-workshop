@echo off
rem Rebuilds tailor.exe from the source files (run after editing the code).
rem Uses g++ if it is on PATH, otherwise Visual Studio 2022's compiler.
cd /d "%~dp0"
set SRC=main.cpp Workshop.cpp OrderBST.cpp CustomerList.cpp InputUtils.cpp

where g++ >nul 2>nul
if %errorlevel%==0 (
    echo Building with g++ ...
    g++ -std=c++17 -Wall -Wextra -o tailor.exe %SRC%
    goto done
)

set VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
if exist "%VCVARS%" (
    echo Building with Visual Studio ...
    call "%VCVARS%" >nul
    if not exist obj mkdir obj
    cl /nologo /std:c++17 /EHsc /W4 %SRC% /Fe:tailor.exe /Fo:obj\
    goto done
)

echo No C++ compiler found. Install MinGW g++ or Visual Studio with C++ tools.
pause
exit /b 1

:done
if %errorlevel%==0 (echo Build succeeded: tailor.exe) else (echo Build FAILED.)
pause
