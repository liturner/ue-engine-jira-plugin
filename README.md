# JiraEnginePlugin

Developed with Unreal Engine 4

# Development Environment
* Visual Studio 2019
    * [Workload] Game development with C++
    * C++ profiling tools
    * C++ AddressSanitizer
    * Windows 10.0.20348.0 SDK


# Build Command Cheat Sheet
* Run Tests
    * Working Directory: '[install-dir]\Engine\Binaries\Win64\'
    * ```UE4Editor.exe C:\Users\lukei\Source\UE-Plugins\JiraEnginePlugin.uproject -ExecCmds="Automation RunTests FTEST_FunctionalTests" -unattended -nopause -testexit="Automation Test Queue Empty" -ReportOutputPath="C:\Users\lukei\Source\UE-Plugins\TestLog"```
* Build Plugin
    * Working Directory: G:\Epic Games\UE_4.27\Engine\Build\BatchFiles
    * ```RunUAT.bat BuildPlugin -Plugin="C:\Users\lukei\Source\UE-Plugins\Plugins\JiraPlugin\JiraPlugin.uplugin" -Package="C:\Users\lukei\Source\UE-Plugins\Build" -CreateSubFolder -VS2019 -TargetPlatforms=Win32+Win64+HoloLens```