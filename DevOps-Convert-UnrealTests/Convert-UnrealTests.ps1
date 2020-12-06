Param ($inDir, $outFile)  
  
$CurrentWorkingDirectory = Get-Location
Write-Host "Current working directory is $CurrentWorkingDirectory"

$ScriptDir = $PSScriptRoot
Write-Host "Current script directory is $ScriptDir"

$PythonExe = $ScriptDir+"\python39\python.exe"
Write-Host "Python executeable is $PythonExe"

$PythonScript = $ScriptDir+"\Convert-UnrealTests.py"
Write-Host "Python script is $PythonScript"

Write-Host "Test directory to convert is $inDir"

Write-Host "Test output file (.xrt) is is $outFile"

& $PythonExe $PythonScript $inDir $outFile

Write-Host "End Task"
