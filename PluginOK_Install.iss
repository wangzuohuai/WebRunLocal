; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "PluginOK中间件"
#define MyAppVersion "1.3.5.2"
#define MyAppPublisher "成都佐罗软件有限公司"
#define MyAppURL "http://www.zorrosoft.com/"
#define MyAppExeName "WrlSetup.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (若要生成新的 GUID，可在菜单中点击 "工具|生成 GUID"。)
AppId={{20BAC19B-5557-453A-877A-3B6DEAC4C907}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\PluginOK
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
; 以下行修改为实际的路径。
LicenseFile=F:\WebRunLocal\Bin\license.txt
InfoAfterFile=F:\WebRunLocal\Bin\TestWrl.txt
; 以下行取消注释，以在非管理安装模式下运行（仅为当前用户安装）。
PrivilegesRequired=admin
OutputDir=F:\WebRunLocal
OutputBaseFilename=PluginOK
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "F:\WebRunLocal\Bin\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\WrlSetup.exe";Parameters: "/S /I";Description: "安装服务";StatusMsg:"正在安装服务，请稍后..."

[UninstallRun]
Filename: "{app}\WrlSetup.exe";Parameters: "/S /U";StatusMsg:"正在停止服务并卸载，请稍后..."

