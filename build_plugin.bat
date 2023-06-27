set PLUGINNAME=Smartsuit
set ENGINEDIR=H:\Program Files\Epic Games\UE_5.2
set RUNUAT=%ENGINEDIR%\Engine\Build\BatchFiles\RunUAT.bat
set package_dir=%~dp0Build\Plugins\%PLUGINNAME%
set plugin_dir=%~dp0Plugins\%PLUGINNAME%

rmdir %package_dir% /s /q

call "%RUNUAT%" BuildPlugin -Plugin="%plugin_dir%\%PLUGINNAME%.uplugin" -Package=%package_dir% -Rocket

rmdir %plugin_dir%\Binaries /s /q
rmdir %plugin_dir%\Intermediate /s /q

Robocopy "%package_dir%\Binaries" "%plugin_dir%\Binaries" /E

pause