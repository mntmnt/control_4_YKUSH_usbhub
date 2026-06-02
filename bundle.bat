@echo off
::# VERSION_SEARCH_TAG
SET VERSION=1_0

SET CURRENT_DIR=%~dp0

:: check __local_env.bat file and load it if present
IF EXIST "%CURRENT_DIR%__local_env.bat" (GOTO NoError)
echo
echo ERROR: There is no `__local_env.bat' around
echo    1. ---  Just copy `__local_env.bat.example' into `__local_env.bat'
echo    2. ---  Edit those environment variables inside
echo  Having that file is mandatory to run this script! And it's quite easy!
echo
EXIT /B 1

:NoError
call "%CURRENT_DIR%__local_env.bat"

:: put MSVC environment variables into current session
pushd .
	call "%MSVC_VARS_PATH%"  x64
popd

:: put Qt environment variables into current session
pushd .
	call "%QT_BIN%qtenv2.bat"
::	call D:\VS2019\IDE\VC\Auxiliary\Build\vcvarsall.bat x86_amd64
popd

:: simplify path
pushd .
	cd %CURRENT_DIR%
	SET CURRENT_DIR=%CD%
popd

SET DeployFolder=%CURRENT_DIR%/deploy/%VERSION%

mkdir "%DeployFolder%"
echo "%TARGET_PATH%\YepkitUSBSwitch.exe"
COPY /Y "%TARGET_PATH%\YepkitUSBSwitch.exe"  "%DeployFolder%"
COPY /Y "%QT_BIN%\Qt6QuickControls2Impl.dll" "%DeployFolder%"
COPY /Y "%QT_BIN%\Qt6QmlWorkerScript.dll"    "%DeployFolder%"

pushd .
	cd "%DeployFolder%"
	echo "Current path: %CD%"
	echo "Delpoy:"
	echo "Project: %CURRENT_DIR%"
	DIR "%CURRENT_DIR%"
	:: --qmldir "d:\WinDev\Qt\6.3.1\msvc2019_64\qml\"
	::windeployqt.exe -qmldir "%CURRENT_DIR%"  HIDTester.exe
	windeployqt.exe --qmldir "%CURRENT_DIR%" --no-translations YepkitUSBSwitch.exe
popd

:END_FILE

pause
