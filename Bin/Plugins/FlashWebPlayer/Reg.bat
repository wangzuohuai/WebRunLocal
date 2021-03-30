IF NOT EXIST "%ProgramW6432%" (
regsvr32 /s "%WinDir%\system32\Macromed\Flash\Flash.ocx"
) ELSE (
regsvr32 /s "%WinDir%\SysWOW64\Macromed\Flash\Flash.ocx"
regsvr32 /s "%WinDir%\system32\Macromed\Flash\Flash.ocx"
)