; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{370674BC-FCD0-4C4D-9B55-49A6EFC3DAC6}
AppName=Howie's Quick Screen Capture
AppVerName=Howie's Quick Screen Capture 1.1.1
AppPublisher=Howies Funware
AppPublisherURL=http://www.howiesfunware.com/
AppSupportURL=http://www.howiesfunware.com/
AppUpdatesURL=http://www.howiesfunware.com/
DefaultDirName={pf}\Howie's Quick Screen Capture
DefaultGroupName=Howie's Quick Screen Capture
OutputBaseFilename=install - Howies Screen Capture
SetupIconFile=C:\Funware Projects\Quick Screen Capture\starter.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Funware Projects\Quick Screen Capture\Release\HQScreen.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Funware Projects\Quick Screen Capture\Release\FreeImage.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Funware Projects\Quick Screen Capture\Release\HQScreen.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Howie's Quick Screen Capture"; Filename: "{app}\HQScreen.exe"
Name: "{group}\Uninstall"; Filename: "{app}\unins000.exe"
Name: "{commondesktop}\Howie's Quick Screen Capture"; Filename: "{app}\HQScreen.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Howie's Quick Screen Capture"; Filename: "{app}\HQScreen.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\HQScreen.exe"; Description: "{cm:LaunchProgram,Howie's Quick Screen Capture}"; Flags: nowait postinstall skipifsilent

