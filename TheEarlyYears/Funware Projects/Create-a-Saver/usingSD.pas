unit usingSD;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, constants;

type
  TfrmUsingSD = class(TForm)
    Label1: TLabel;
    Panel1: TPanel;
    Memo1: TMemo;
    chkDontShow: TCheckBox;
    Button1: TButton;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmUsingSD: TfrmUsingSD;

implementation

{$R *.DFM}

procedure TfrmUsingSD.FormClose(Sender: TObject; var Action: TCloseAction);
var KeyName: Array[0..50] of char;
    temp: Array[0..300] of char;
begin
  if chkDontShow.Checked then begin
    StrPCopy(KeyName, 'Show Screen Designer Message');
    StrPCopy(temp, '0');
    WritePrivateProfileString(APP_NAME, KeyName, temp, FILE_NAME);
  end;
end;

procedure TfrmUsingSD.Button1Click(Sender: TObject);
begin
  Close;
end;

end.
