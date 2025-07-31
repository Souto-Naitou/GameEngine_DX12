@echo off
echo Running GoogleTest tests...

cd /d "%~dp0Tests"

if not exist "Bin\Debug\Tests.exe" (
    echo Tests.exe not found. Building tests first...
    cd /d "%~dp0Engine"
    cmd /c ""C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" && MSBuild ..\Tests\Tests.vcxproj /p:Configuration=Debug /p:Platform=x64"
    cd /d "%~dp0Tests"
)

if exist "Bin\Debug\Tests.exe" (
    echo Executing tests...
    .\Bin\Debug\Tests.exe
) else (
    echo Failed to build or find tests executable.
)

pause
