@ECHO OFF
@TITLE Configuring your workspace

REM Auto Start and set up WindRiver Workbench environment every time
REM Version 1.0
REM Date 24.2.2021
REM Arturo Huerta
REM Copyright (c) Branson Ultrasonics Corporation, 1996-2018


REM Set your Development Workspace
REM SET Workspace=C:\Users\PFitzmaurice\MyWorkspaces_NextGen\Branson_Dev
SET Workspace=C:\Arturo\GSX-P1-ASC

REM Set your WindRiver Home
SET WindRiver=C:\WindRiverSR650

REM =======================
REM !!!! DO NOT MODIFY !!!!
REM !!!!  CODE BELOW   !!!!
REM =======================

REM Verify Workspace and WindRiver Exist
IF NOT EXIST %WindRiver%\vxworks-7-backup (
 	@ECHO Doing a backup of vxworks-7 for the first time
	xcopy %WindRiver%\vxworks-7 %WindRiver%\vxworks-7-backup /i /e /y /s /q > NUL
)

REM This sets up the correct environment path for the current workspace
SET WIND_BSP_PATHS=%Workspace%\_Branson_Driver_Layer_SR0650\BRANSON_BSP\BSP_5728_SR0650
SET WIND_LAYER_PATHS=%Workspace%\_Branson_Driver_Layer_SR0650\BRANSON_DRV_2_0_0_0
SET WIND_WRTOOL_WORKSPACE=%Workspace%
SET PATH=%PATH%;%WindRiver%\workbench-4

REM This verifies that all files have been removed and are being copied to the custom syscall folder
rmdir /s /q %WindRiver%\vxworks-7\pkgs_v2\os\core-3.1.3.0\syscalls\custom > NUL
xcopy %Workspace%\_Branson_Driver_Layer_SR0650\SYSCALL %WindRiver%\vxworks-7\pkgs_v2\os\core-3.1.3.0\syscalls /i /e /y /s /q > NUL

REM Change to Workspace folder`
cd %Workspace%
 
REM This will automatically start WindRiver Workbench with your currently defined workspace
%WindRiver%\workbench-4\startWorkbench.bat -data %Workspace%
