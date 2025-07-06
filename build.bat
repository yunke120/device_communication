@echo off
echo Setting up Qt environment...
call "D:\Qt\6.9.1\mingw_64\bin\qtenv2.bat"

echo Running qmake...
qmake device_communication.pro

echo Building project...
mingw32-make

echo Build complete!
pause 