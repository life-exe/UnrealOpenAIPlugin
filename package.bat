@echo off

set "UATPath=C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat"
set PluginPath=%~dp0\OpenAI.uplugin
set BuildPath=%~dp0\Build

"%UATPath%" BuildPlugin -Plugin="%PluginPath%" -Package="%BuildPath%"
