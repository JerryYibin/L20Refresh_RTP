ECHO OFF
Title Set up GSX-P1 Development Environment (SR2107)
:Ask
set PWD=%~dp0
echo Current Path:%PWD%

echo Step-1: Enter WindRiver Path to vxWorks-7 SR2107 (IE: C:\WindRiver\vxworks)
set INPUTWR=
set /P INPUTWR=Type Location: %=%
echo Your WindRiver Path: %INPUTWR%

echo Step-2: Enter BRANSON_DRV_LAYER Path (IE: C:\Users\Me\MyWorkspaces\_Branson_Driver_Layer)

set INPUTLAYER=%WIND_LAYER_PATHS%
echo Your Previous BRANSON_DRV_LAYER Path: %INPUTLAYER%

set INPUTLAYER=%WIND_LAYER_PATHS%;%PWD%BRANSON_DRV_2_0_0_0
echo Your Current BRANSON_DRV_LAYER Path: %INPUTLAYER%

%SystemRoot%\system32\setx WIND_LAYER_PATHS %INPUTLAYER% /m
SET PATH = %WIND_LAYER_PATHS%


echo Step-3: Settings Bsp path variables for SR2107...!

set INPUTBSP=%WIND_BSP_PATHS%
echo Your Previous BRANSON_BSP Path: %INPUTBSP%
 
set SRC_BSP=%INPUTBSP%;%PWD%BRANSON_BSP\BSP_5708_SR2107
echo Your Current BRANSON_BSP Path: %SRC_BSP%

:: Set Path variable
%SystemRoot%\system32\setx WIND_BSP_PATHS %SRC_BSP% /m
:: Set Path variable
SET PATH = %WIND_BSP_PATHS%

echo Step-4: Copy Branson Syscalls %PWD%SYSCALL To VxWorks System syscalls Folder...
xcopy %PWD%\SYSCALL %INPUTWR%\21.07\os\core\syscalls /e /y /s /q > NUL


echo Done!
echo Please Reboot your workstation now.
PAUSE 
exit
