SETLOCAL enableDelayedExpansion
PUSHD %~dp0
::@echo off
SET "PATH=C:\Program Files (x86)\MSBuild\14.0\Bin;C:\Program Files\CMake\bin;C:\Program Files (x86)\CMake\bin;%PATH%"
SET MR_COMPILER="Visual Studio 14 2015"
SET "MR_INSTALL_PREFIX=Dependencies\install"

:: Debug compilation
SET MR_CONFIG=Debug
CALL :compileandinstallproj Dependencies\SFML-2.4.0 sfml.sln SFML

:: Release compilation
SET MR_CONFIG=Release
CALL :compileandinstallproj Dependencies\SFML-2.4.0 sfml.sln SFML

POPD
PAUSE
EXIT /B 0

:: Fonction qui compile un projet "simple"
:compileproj
SET MR_CURRENT_BUILD_FOLDER=Dependencies\build\%3\%MR_CONFIG%
IF NOT EXIST %MR_CURRENT_BUILD_FOLDER% md %MR_CURRENT_BUILD_FOLDER%
cmake -H%1 -B%MR_CURRENT_BUILD_FOLDER% "-DCMAKE_INSTALL_PREFIX=%MR_INSTALL_PREFIX%\%3" -G%MR_COMPILER%
msbuild /m:4 /p:Configuration=%MR_CONFIG% %MR_CURRENT_BUILD_FOLDER%\%2
EXIT /B 0

:: Fonction qui installe un projet "simple"
:installproj
SET MR_CURRENT_BUILD_FOLDER=Dependencies\build\%3\%MR_CONFIG%
msbuild /p:Configuration=%MR_CONFIG% %MR_CURRENT_BUILD_FOLDER%\INSTALL.vcxproj
EXIT /B 0

:: Fonction qui compile et installe un projet "simple"
:compileandinstallproj
CALL :compileproj %1 %2 %3
CALL :installproj %1 %2 %3
EXIT /B 0