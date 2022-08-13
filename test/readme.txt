1.
add defines "-DDEBUG_YANG" into RTP project "L20_RTP"
rebuild the project to "L20_RTP.vxe"

2.
create new RTP project with file "rtp.c", build it to "test.vxe"

3.
run "L20_RTP.vxe" to wait for messages

4.
run "test.vxe" to send messages in sequence

the commands should be like following
-> cmd
[vxWorks *]# L20_RTP.vxe &
[vxWorks *]# 
MAIN_T: Create SD: OK
dbqID[0] = 0x10046
MAIN_T: Create MsgQ: OK
Main_T : CTRL_T task Id: 65619
Main_T : AuxClk set at Rate = 1000
Main_T : Connected Aux Clock ISR successful 
System Clock Value = 60
EmrsnSocket::Bind: Port ID: 0: 1600
HMI Socket listening on Port: 1600
0x2023ceb8 (tLogger): <LC: 1> ~ < E > < Line No: 301 > < File: E:/vx7_2107/workspace/L20_RTP2/L20_RTP/SSLWrapper.cpp > : SSLSocket: Unable to use Certificate file 
0x2023ceb8 (tLogger): <LC: 2> ~ < E > < Line No: 44 > < File: E:/vx7_2107/workspace/L20_RTP2/L20_RTP/CommunicationInterfaceHMI.cpp > : HMI Socket:-------SSL initialization allocation failed----------- 

[vxWorks *]# test.vxe c
TO_DATA_TASK_WELD_RECIPE_CLEAR: 0
TO_DATA_TASK_WELD_RECIPE_CLEAR: 0
TO_DATA_TASK_WELD_SIGN_CLEAR: 0
TO_DATA_TASK_WELD_SIGN_CLEAR: 0
TO_DATA_TASK_WELD_RESULT_CLEAR: 0
TO_DATA_TASK_WELD_RESULT_CLEAR: 0
[vxWorks *]# 
[vxWorks *]# t.vxe 2
##StoreWeldResult: result 0 - insert into WeldResultTable (CreatedTime, RecipeID, WeldEnergy, TriggerPressure, WeldPressure, WeldAmplitude, WeldTime, WeldPeakPower, TriggerHeight, WeldHeight, AlarmFlag, SequenceID, CycleCounter, partID) values (1505149803,456,0,0,0,0,0,0,0,0,0,0,0,'');
DataTask: query data from db WeldResultTable: 1,,1505149803,456,,0,0,0,0,0,0,0,0,0,0,0
[vxWorks *]# 
[vxWorks *]# t.vxe 1
##StoreWeldRecipe: result 0 - insert into WeldRecipeTable (UserID, IsValidate, Amplitude, Width, WeldPressure, TriggerPressure, TimePlus, TimeMinus, PeakPowerPlus, PeakPowerMinus, TriggerHeightPlus, TriggerHeightMinus, WeldHeightPlus, WeldHeightMinus, WeldMode, ModeValue, PreBurst, HoldTime, SqueezeTime, AfterBurstDelay, AfterBurstDuration, AfterBurstAmplitude, WeldHeight, MeasuredHeight, StepWeldMode, EnergyToStep, TimeToStep, PowerToStep, RecipeName, DateTime, PresetPicPath) values (0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2139299548, '','','','2022-08-11 00:00:00','','DataTask: query data from db WeldResultTable: 1,,1505149803,456,,0,0,0,0,0,0,0,0,0,0,0
nergy, TriggerPressure, WeldPressure, WeldAmplitude, WeldTime, WeldPeakPower, TriggerHeight, WeldHeight, AlarmFlag, SequenceID, CycleCounter, partID) values (1505149803,456,0,0,0,0,0,0,0,0,0,0,0,'');
');
DataTask: query data from db WeldRecipeTable: 1,2022-08-11 00:00:00,,0,DataTask: query data from db WeldResultTable: 1,,1505149803,456,,0,0,0,0,0,0,0,0,0,0,0
nergy, TriggerPressure, WeldPressure, WeldAmplitude, WeldTime, WeldPeakPower, TriggerHeight, WeldHeight, AlarmFlag, SequenceID, CycleCounter, partID) values (1505149803,456,0,0,0,0,0,0,0,0,0,0,0,');
,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2139299548,,,
[vxWorks *]# 
[vxWorks *]# t.vxe 3
##StoreWeldSignature: result 0 - insert into WeldResultSignature (WeldResultID, WeldGraph) values (1,'');
DataTask: query data from db WeldResultSignature: 1,1,
[vxWorks *]# 
