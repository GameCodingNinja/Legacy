; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Screen Savers To Go"
#define MyAppVersion "1.0.7"
#define MyAppPublisher "Howies Funware"
#define MyAppURL "http://www.howiesfunware.com/"
#define MyAppExeName "SSToGo.exe"
#define UninstallExeName "unins000.exe"
#define UninstallIconName "Uninstall"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{08A1E927-A613-4C0F-AC78-AB976C65B064}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\development\Funware Projects\SStoGo\Inno Installer\SSToGo.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\development\Funware Projects\SStoGo\Inno Installer\Background Files\*"; DestDir: "{app}\Background Files"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\development\Funware Projects\SStoGo\Inno Installer\Sample Screen Saver\*"; DestDir: "{app}\Sample Screen Saver"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{#UninstallIconName}"; Filename: "{app}\{#UninstallExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

