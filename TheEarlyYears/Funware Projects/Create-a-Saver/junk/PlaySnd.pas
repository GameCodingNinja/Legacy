unit PlaySnd;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, StdCtrls, FileCtrl, constants, Middy;

type
  TfrmSoundDir = class(TForm)
    DriveComboBox1: TDriveComboBox;
    DirectoryListBox1: TDirectoryListBox;
    FileListBox1: TFileListBox;
    FilterComboBox1: TFilterComboBox;
    btnOK: TButton;
    btnCancel: TButton;
    btnPlaySnd: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure DriveComboBox1Change(Sender: TObject);
    procedure DirectoryListBox1Change(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure btnPlaySndClick(Sender: TObject);
    procedure FileListBox1Change(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSoundDir: TfrmSoundDir;

implementation

{$R *.DFM}

procedure TfrmSoundDir.FormCreate(Sender: TObject);
begin
  {FilterComboBox1.Filter := 'Sound Files (*.wav)' +
          '|*.WAV' +
          '|All files (*.*)|*.*';}

  // Show the files of the sound directory
  if gSoundsOnOff then begin
    // Check to see if it is missing
    if DirectoryExists(gSoundDir) then begin
       FileListBox1.FileName := gSoundDir;
       DirectoryListBox1.Directory := FileListBox1.Directory;
       DriveComboBox1.Drive := DirectoryListBox1.Drive;
    end;
  end;
end;

procedure TfrmSoundDir.DriveComboBox1Change(Sender: TObject);
begin
  DirectoryListBox1.Drive := DriveComboBox1.Drive;
end;

procedure TfrmSoundDir.DirectoryListBox1Change(Sender: TObject);
begin
  FileListBox1.Directory := DirectoryListBox1.Directory;
end;

procedure TfrmSoundDir.btnCancelClick(Sender: TObject);
begin
  gCancelPressed := True;
  close;
end;

procedure TfrmSoundDir.btnOKClick(Sender: TObject);
begin
  if FileListBox1.Items.count > 0 then begin
     gSoundDir := FileListBox1.Directory;
     gSoundFilePath := FileListBox1.FileName;
     gSoundsOnOff := True;
  end;
  close;
end;

procedure TfrmSoundDir.btnPlaySndClick(Sender: TObject);
begin
  {if (FileListBox1.ItemIndex >= 0) and (FilterComboBox1.ItemIndex = 0) then begin
    sndPlaySound(PChar(FileListBox1.FileName), SND_NODEFAULT + SND_ASYNC);
  end;}

  {if FileListBox1.ItemIndex >= 0 then begin
    if frmMID.GetPlayerStatus > -1 then
      frmMID.MediaPlayer1.Close;
    frmMID.MediaPlayer1.FileName := FileListBox1.FileName;
    frmMID.MediaPlayer1.Open;
    frmMID.MediaPlayer1.Play;
  end;}

  case frmMID.GetPlayerStatus of
    Playing: begin
      frmMID.MediaPlayer1.Stop;
      frmMID.MediaPlayer1.Close;
    end;

    Seeking, Paused: begin
      frmMID.MediaPlayer1.Stop;
      frmMID.MediaPlayer1.Close;
      frmMID.MediaPlayer1.FileName := FileListBox1.FileName;
      frmMID.MediaPlayer1.Open;
      frmMID.MediaPlayer1.Play;
    end;

    Open, Stopped: begin
      frmMID.MediaPlayer1.Close;
      frmMID.MediaPlayer1.FileName := FileListBox1.FileName;
      frmMID.MediaPlayer1.Open;
      frmMID.MediaPlayer1.Play;
    end;

    Closed: begin
      frmMID.MediaPlayer1.FileName := FileListBox1.FileName;
      frmMID.MediaPlayer1.Open;
      frmMID.MediaPlayer1.Play;
    end;
  end;
    
end;

procedure TfrmSoundDir.FileListBox1Change(Sender: TObject);
begin
  if FileListBox1.Items.count > 0 then
    btnPlaySnd.enabled := True
  else
    btnPlaySnd.enabled := False;
end;

procedure TfrmSoundDir.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  // Unload the midi player   
  if frmMID.GetPlayerStatus > Closed then begin
    if frmMID.GetPlayerStatus = Playing then
       frmMID.MediaPlayer1.Stop;
    frmMID.MediaPlayer1.Close;
  end;
end;

end.
