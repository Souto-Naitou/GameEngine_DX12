@echo off
echo Building GoogleTest...

cd /d "%~dp0Engine\Externals\googletest"

if exist "build" (
    echo Removing existing build directory...
    rmdir /s /q "build"
)

mkdir build
cd build

echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64 -Dgtest_force_shared_crt=OFF

echo Building Debug configuration...
cmake --build . --config Debug

echo Building Release configuration...
cmake --build . --config Release

echo GoogleTest build completed!
pause
