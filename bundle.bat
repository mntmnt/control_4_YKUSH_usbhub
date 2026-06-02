echo on
::# VERSION_SEARCH_TAG
SET VERSION=1_0

SET CURRENT_DIR=%~dp0
SET QT_BIN=d:\WinDev\Qt\6.5.1\msvc2019_64\bin\
SET TARGET_PATH=d:\WinDev\Riccardo\HIDTester\build\Desktop_Qt_6_5_1_MSVC2019_64bit-Release\release\
::c:\devel\vares\REmove2\build-myContentManipulation-Desktop_Qt_6_2_0_MSVC2019_64bit-Release

pushd .
	call "%QT_BIN%qtenv2.bat"
::	call D:\VS2019\IDE\VC\Auxiliary\Build\vcvarsall.bat x86_amd64
popd

SET DeployFolder=depl%VERSION%

mkdir "%DeployFolder%"
echo "%TARGET_PATH%\HIDTester.exe"
COPY /Y "%TARGET_PATH%\HIDTester.exe"        "%DeployFolder%"
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
	windeployqt.exe --qmldir "d:\WinDev\Riccardo\HIDTester" --no-translations  HIDTester.exe
popd

:END_FILE

pause
