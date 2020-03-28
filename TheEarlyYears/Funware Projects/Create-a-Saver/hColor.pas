unit hColor;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, constants;

type
  TfrmHColor = class(TForm)
    Label1: TLabel;
    Memo1: TMemo;
    chkDontShow: TCheckBox;
    Button1: TButton;
    Panel1: TPanel;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmHColor: TfrmHColor;

implementation

{$R *.DFM}

procedure TfrmHColor.FormClose(Sender: TObject; var Action: TCloseAction);
var KeyName: Array[0..50] of char;
    temp: Array[0..300] of char;
begin
  if chkDontShow.Checked then begin
    StrPCopy(KeyName, '256 Color Check');
    StrPCopy(temp, '0');
    WritePrivateProfileString(APP_NAME, KeyName, temp, FILE_NAME);
  end;
end;

procedure TfrmHColor.Button1Click(Sender: TObject);
begin
  close;
end;

end.
