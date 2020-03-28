unit main;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, FileCtrl, StdCtrls;

type
  TfrmChecksum = class(TForm)
    MainMenu1: TMainMenu;
    mnuFile: TMenuItem;
    mnuExit: TMenuItem;
    mnuCheckSum: TMenuItem;
    DriveComboBox1: TDriveComboBox;
    DirectoryListBox1: TDirectoryListBox;
    FileListBox1: TFileListBox;
    btnChecksum: TButton;
    btnExit: TButton;
    FilterComboBox1: TFilterComboBox;
    procedure mnuExitClick(Sender: TObject);
    procedure DirectoryListBox1Change(Sender: TObject);
    procedure DriveComboBox1Change(Sender: TObject);
    procedure btnExitClick(Sender: TObject);
    procedure btnChecksumClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmChecksum: TfrmChecksum;

implementation

{$R *.DFM}

procedure TfrmChecksum.mnuExitClick(Sender: TObject);
begin
  close;
end;

procedure TfrmChecksum.DirectoryListBox1Change(Sender: TObject);
begin
  FileListBox1.Directory := DirectoryListBox1.Directory;
end;

procedure TfrmChecksum.DriveComboBox1Change(Sender: TObject);
begin
  DirectoryListBox1.Drive := DriveComboBox1.Drive;
end;

procedure TfrmChecksum.btnExitClick(Sender: TObject);
begin
  Close;
end;

procedure TfrmChecksum.btnChecksumClick(Sender: TObject);
var hFile, J, I, FileSize: Integer;
    buffer: PBYTE;
    bufDWORD: PDWORD;
    AddItUp, CheckSumResult: DWORD;
begin

  if FileListBox1.ItemIndex < 0 then exit;

  hFile := FileOpen( FileListBox1.FileName, fmOpenReadWrite or fmShareDenyNone);
  if hFile <= 0 then begin
    showMessage('Cant open file.');
    exit;
  end;

  FileSize := GetFileSize( hFile, nil );

  if Filesize mod sizeof(DWORD) <> 0 then begin
    if FileSeek(hFile,0,2) = HFILE_ERROR then begin
      showMessage('File pointer error');
      exit;
    end else begin
      J := 0;
      for I := 1 to (sizeof(DWORD)-(Filesize mod sizeof(DWORD))) do
        FileWrite( hFile, J, 1);
    end;
  end;

  // Get file size again
  FileSize := GetFileSize( hFile, nil );

  buffer := allocmem( FileSize );

  FileSeek( hFile,0,0);
  J := FileRead( hFile, buffer^, FileSize );
  if J <> FileSize then begin
    showMessage('File read error');
    FileClose(hFile);
    exit;
  end;

  bufDWORD := PDWORD(buffer);

  // Add it all up
  AddItUp := 0;
  for I := 0 to (FileSize div sizeof(DWORD))-1 do begin
    AddItUp := AddItUp + bufDWORD^;
    inc(bufDWORD);
  end;

  if AddItUp > 0 then begin

    CheckSumResult := $100000000 - AddItUp;
    FileSeek(hFile,0,2);

    // Write the Checksum result to the end of the file
    FileWrite( hFile, CheckSumResult, sizeof(DWORD));
    showMessage('File checked sumed, data was added');
  end else
    showMessage('File checked sumed, no data added');

  FileClose(hFile);
  freeMem( buffer, FileSize);
end;

end.
