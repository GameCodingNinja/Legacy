unit getfiles;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, FileCtrl, constants, Loadpref, Globals, gFunctin;

type
  TfrmGetFiles = class(TForm)
    FileListBox1: TFileListBox;
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmGetFiles: TfrmGetFiles;

implementation

{$R *.DFM}

procedure TfrmGetFiles.FormCreate(Sender: TObject);
var
    Settings: TScrnSavSettings;
    DirTemp: String;
    I: LongInt;
begin
   Settings := TScrnSavSettings.Create;
   Settings.GetINISettings;

// Get all the images in the main directory
  if Settings.MainSingleOrDir = LOADDIR then begin
    MainDirList := TStringList.Create;
    // Check to see if it is missing
    if DirectoryExists(Settings.MainImage) then begin
      DirTemp := checkDirIfRoot(Settings.MainImage);
      // Check to see if it is empty
      FileListBox1.Directory := Settings.MainImage;
      if FileListBox1.Items.count > 0 then begin
        for I := 0 to FileListBox1.Items.count-1 do
          MainDirList.Add(DirTemp + FileListBox1.Items[I]);
      end else
        MainDirList.Add(EMPTY_DIR);
    end else
      MainDirList.Add(MISSING_DIR);
  end;

  // Get all the images in the back directory
  if Settings.BKSingleOrDir = LOADDIR then begin
     BackDirList := TStringList.Create;
    // Check to see if it is missing
    if DirectoryExists(Settings.BKImage) then begin
      DirTemp := checkDirIfRoot(Settings.BKImage);
      FileListBox1.Directory := Settings.BKImage;
      // Check to see if it is empty
      if FileListBox1.Items.count > 0 then begin
        for I := 0 to FileListBox1.Items.count-1 do
          BackDirList.Add(DirTemp + FileListBox1.Items[I]);
      end else
        BackDirList.Add(EMPTY_DIR);
    end else
      BackDirList.Add(MISSING_DIR);
  end;

  settings.free;
  close;
end;

end.
