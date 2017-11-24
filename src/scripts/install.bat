@echo off

@setlocal enableextensions disabledelayedexpansion

if "%~1"=="" goto missing_arg

if "%~2"=="" goto set_def_install

set "INSTALL_PATH=%2"

goto do_install

:set_def_install
set "INSTALL_PATH=%PROGRAMFILES(x86)%\Frontier"

goto do_install

:do_install
mkdir "%INSTALL_PATH%\tmp"
set "ERRLVL=%ERRORLEVEL%";
rmdir "%INSTALL_PATH%\tmp"

if "%ERRLVL%"=="1" goto not_admin

rmdir /Q /S "%INSTALL_PATH%\Flib"

mkdir "%INSTALL_PATH%\Flib"
mkdir "%INSTALL_PATH%\Flib\include"
mkdir "%INSTALL_PATH%\Flib\libWin"

echo Setting FPATH
setx FPATH "%INSTALL_PATH%\Flib"

echo Copying flib files
xcopy /Y /S /Q "%1\include" "%INSTALL_PATH%\Flib\include" >nul
xcopy /Y /S /Q "%1\libWin" "%INSTALL_PATH%\Flib\libWin" >nul

goto end

:missing_arg
echo Usage: %0 Install_source

goto end

:not_admin
echo Unable to install in %INSTALL_PATH% directory, please run as admin

goto end

:end