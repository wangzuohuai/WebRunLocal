@ECHO OFF&(PUSHD "%~DP0")&(REG QUERY "HKU\S-1-5-19">NUL 2>&1)||(
powershell -Command "Start-Process '%~sdpnx0' -Verb RunAs"&&EXIT)

sc stop "Flash Helper Service" >NUL 2>NUL
taskkill /f /im FlashHelperService.exe >NUL 2>NUL
taskkill /f /im FlashPlayerUpdateService.exe >NUL 2>NUL
schtasks /delete /tn "Adobe Flash Player Updater" /f >NUL 2>NUL
schtasks /delete /tn "FlashHelper TaskMachineCore" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Macromedia\FlashHelper" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Macromedia\FlashHelper" /f >NUL 2>NUL
reg delete "HKLM\SYSTEM\CurrentControlSet\services\Flash Helper Service" /f >NUL 2>NUL
reg delete "HKLM\SYSTEM\CurrentControlSet\services\AdobeFlashPlayerUpdateSvc" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\FlashHelperService.exe" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\FlashPlayerUpdateService.exe" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION" /f /v "FlashHelperService.exe" >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\MAIN\FeatureControl\FEATURE_BROWSER_EMULATION" /f /v "FlashHelperService.exe" >NUL 2>NUL

ver|findstr "\<6\.[0-9]\.[0-9][0-9]*\> \<10\.[0-9]\.[0-9][0-9]*\>" > NUL && (
takeown /f "%WinDir%\System32\Macromed\Flash\*" /a /r /d y >NUL 2>NUL
takeown /f "%WinDir%\SysWOW64\Macromed\Flash\*" /a /r /d y >NUL 2>NUL
if exist "%WinDir%\System32\FlashPlayerApp.exe" takeown /f "%WinDir%\System32\FlashPlayerApp.exe" /a >NUL 2>NUL
if exist "%WinDir%\System32\FlashPlayerCPLApp.cpl" takeown /f "%WinDir%\System32\FlashPlayerCPLApp.cpl" /a >NUL 2>NUL
if exist "%WinDir%\SysWOW64\FlashPlayerApp.exe" takeown /f "%WinDir%\SysWOW64\FlashPlayerApp.exe" /a >NUL 2>NUL
if exist "%WinDir%\SysWOW64\FlashPlayerCPLApp.cpl" takeown /f "%WinDir%\SysWOW64\FlashPlayerCPLApp.cpl" /a >NUL 2>NUL
echo y|icacls "%WinDir%\System32\Macromed\*" /t /c /grant "Everyone:f" >NUL 2>NUL
echo y|icacls "%WinDir%\SysWOW64\Macromed\*" /t /c /grant "Everyone:f" >NUL 2>NUL
echo y|icacls "%WinDir%\System32\FlashPlayerApp.exe" /c /grant "Everyone:f" >NUL 2>NUL
echo y|icacls "%WinDir%\System32\FlashPlayerCPLApp.cpl" /c /grant "Everyone:f" >NUL 2>NUL
echo y|icacls "%WinDir%\SysWOW64\FlashPlayerApp.exe" /c /grant "Everyone:f" >NUL 2>NUL
echo y|icacls "%WinDir%\SysWOW64\FlashPlayerCPLApp.cpl" /c /grant "Everyone:f" >NUL 2>NUL
)

IF NOT EXIST "%ProgramW6432%" (
regsvr32 /s /u "%WinDir%\system32\Macromed\Flash\Flash.ocx"
) ELSE (
regsvr32 /s /u "%WinDir%\SysWOW64\Macromed\Flash\Flash.ocx"
regsvr32 /s /u "%WinDir%\system32\Macromed\Flash\Flash.ocx"
)

rd/s/q "%AppData%\Adobe\Flash Player"2>NUL
rd/s/q "%AppData%\Macromedia\Flash Player"2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\activex.vch" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\activex.vch" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\FlashPlayerUpdateService.exe" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\FlashPlayerUpdateService.exe" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\FlashUtil*_ActiveX.dll" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\FlashUtil*_ActiveX.dll" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\FlashUtil*_ActiveX.exe" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\FlashUtil*_ActiveX.exe" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\flashupdater.cfg" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\flashupdater.cfg" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\Flash*.ocx.bak_*" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\Flash*.ocx.bak_*" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\Flash*.ocx" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\Flash*.ocx" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\mms.cfg" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\mms.cfg" >NUL 2>NUL
del/f/q "%WinDir%\System32\Macromed\Flash\*.log" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\Macromed\Flash\*.log" >NUL 2>NUL
del/f/q "%WinDir%\System32\FlashPlayerApp.exe" >NUL 2>NUL
del/f/q "%WinDir%\System32\FlashPlayerCPLApp.cpl" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\FlashPlayerApp.exe" >NUL 2>NUL
del/f/q "%WinDir%\SysWOW64\FlashPlayerCPLApp.cpl" >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Macromedia\FlashPlayer" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Macromedia\FlashPlayerActiveX" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Macromedia\FlashPlayerActiveXReleaseType" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Macromedia\FlashPlayer" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Macromedia\FlashPlayerActiveX" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Macromedia\FlashPlayerActiveXReleaseType" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Internet Explorer\NavigatorPluginsList\Shockwave Flash" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Adobe Flash Player ActiveX" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\FlashPlayerApp.exe" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Adobe Flash Player ActiveX" /f >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Classes\.mfp" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\.sol" /f /v "Content Type" >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\.sor" /f /v "Content Type" >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\.spl" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\.swf" /f /v "Content Type" >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Classes\FlashFactory.FlashFactory" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\FlashFactory.FlashFactory.1" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\CLSID\{B019E3BF-E7E5-453C-A2E4-D2C18CA0866F}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\CLSID\{D27CDB6E-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\CLSID\{D27CDB70-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Interface\{299817DA-1FAC-4CE2-8F48-A108237013BD}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Interface\{307F64C0-621D-4D56-BBC6-91EFC13CE40D}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Interface\{57A0E747-3863-4D20-A811-950C84F1DB9B}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Interface\{86230738-D762-4C50-A2DE-A753E5B1686F}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Interface\{D27CDB6C-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Interface\{D27CDB6D-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\MIME\Database\Content Type\application/futuresplash" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\MIME\Database\Content Type\application/x-shockwave-flash" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\TypeLib\{57A0E746-3863-4D20-A811-950C84F1DB9B}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\TypeLib\{D27CDB6B-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\TypeLib\{FAB3E735-69C7-453B-A446-B6823C6DF1C9}" /f >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\CLSID\{B019E3BF-E7E5-453C-A2E4-D2C18CA0866F}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\CLSID\{D27CDB6E-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\CLSID\{D27CDB70-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\Interface\{299817DA-1FAC-4CE2-8F48-A108237013BD}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\Interface\{307F64C0-621D-4D56-BBC6-91EFC13CE40D}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\Interface\{57A0E747-3863-4D20-A811-950C84F1DB9B}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\Interface\{86230738-D762-4C50-A2DE-A753E5B1686F}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\Interface\{D27CDB6C-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\Wow6432Node\Interface\{D27CDB6D-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\FlashPlayerApp.exe" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\FlashPlayerUpdateService.exe" /f >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Microsoft\Internet Explorer\NavigatorPluginsList\Shockwave Flash" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Ext\PreApproved\{D27CDB6E-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Internet Explorer\ActiveX Compatibility\{D27CDB70-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Internet Explorer\ActiveX Compatibility\{D27CDB6E-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Internet Explorer\Low Rights\ElevationPolicy\{FAF199D2-BFA7-4394-A4DE-044A08E59B32}" /f >NUL 2>NUL

reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\NavigatorPluginsList\Shockwave Flash" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Ext\PreApproved\{D27CDB6E-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\ActiveX Compatibility\{D27CDB70-AE6D-11cf-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\WOW6432Node\Microsoft\Internet Explorer\ActiveX Compatibility\{D27CDB6E-AE6D-11CF-96B8-444553540000}" /f >NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Internet Explorer\Low Rights\ElevationPolicy\{FAF199D2-BFA7-4394-A4DE-044A08E59B32}" /f >NUL 2>NUL