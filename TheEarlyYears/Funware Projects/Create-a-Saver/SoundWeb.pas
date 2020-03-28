unit SoundWeb;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, constants;

type
  TfrmAddingSounds = class(TForm)
    Label1: TLabel;
    Memo1: TMemo;
    chkDontShow: TCheckBox;
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAddingSounds: TfrmAddingSounds;

implementation

{$R *.DFM}

procedure TfrmAddingSounds.Button1Click(Sender: TObject);
begin
  Close;
end;

procedure TfrmAddingSounds.FormClose(Sender: TObject; var Action: TCloseAction);
var KeyName: Array[0..50] of char;
    temp: Array[0..300] of char;
begin
  if chkDontShow.Checked then begin
    StrPCopy(KeyName, 'Show Download Sound');
    StrPCopy(temp, '0');
    WritePrivateProfileString(APP_NAME, KeyName, temp, FILE_NAME);
  end;
end;

end.
