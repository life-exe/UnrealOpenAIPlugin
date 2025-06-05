@echo off

set "UATPath=C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\RunUAT.bat"
set PluginPath=%~dp0\OpenAI.uplugin
set BuildPath=%~dp0\Build
set "ZipPath=c:\Program Files\7-Zip\7z.exe"

:: package plugin
call "%UATPath%" BuildPlugin -Plugin="%PluginPath%" -Package="%BuildPath%"

:: update uplugin file
python plugin_descriptor_updater.py "%BuildPath%\OpenAI.uplugin"

:: zip
cd "%BuildPath%"
"%ZipPath%" a -tzip "OpenAI.zip"
cd ..
