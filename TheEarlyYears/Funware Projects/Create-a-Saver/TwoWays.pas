unit TwoWays;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, constants;

type
  TfrmTwoWays = class(TForm)
    Button1: TButton;
    Label1: TLabel;
    moTwoWars: TMemo;
    chkDontShow: TCheckBox;
    procedure Button1Click(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmTwoWays: TfrmTwoWays;

implementation

{$R *.DFM}

procedure TfrmTwoWays.Button1Click(Sender: TObject);
begin
  close;
end;

procedure TfrmTwoWays.FormClose(Sender: TObject; var Action: TCloseAction);
var KeyName: Array[0..50] of char;
    temp: Array[0..300] of char;
begin
  if chkDontShow.Checked then begin
    StrPCopy(KeyName, 'Show Intro Message');
    StrPCopy(temp, '0');
    WritePrivateProfileString(APP_NAME, KeyName, temp, FILE_NAME);
  end;
end;

end.
