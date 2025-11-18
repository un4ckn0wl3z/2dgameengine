@echo off
setlocal

REM Debug.zip is in the same folder as this script
set ZIP_FILE="%~dp0Debug.zip"

REM Output directory ../x64/Debug relative to script location
set OUTPUT_DIR="%~dp0.\x64\Debug"

echo Extracting Debug.zip to %OUTPUT_DIR% ...

powershell -NoLogo -NoProfile -Command ^
    "Expand-Archive -LiteralPath %ZIP_FILE% -DestinationPath %OUTPUT_DIR% -Force"

echo Done.
pause
