rem First parameter is working path 
echo Setting working directory to %~dp1
cd %~f1

echo Make directory if doesn't exists
md D4D_LIBRARY

echo Copy all eGUI library files
copy DebugD4D\Exe\eGUI_Demo_d4dLib.a D4D_LIBRARY\eGUI_Demo_d4dLib_d.a /Y
copy D4D_Configuration\d4d_user_cfg.h D4D_LIBRARY\d4d_user_cfg.h /Y

xcopy ..\..\..\..\D4D\*.h D4D_LIBRARY\ /S /R /Y
echo eGUI Post-build operations done
