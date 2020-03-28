unit loadtxt;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, DecodeGif, constants;

type
  TfrmTextLoad = class(TForm)
    imgTextLoad: TImage;
    btnDone: TButton;
    moInstructions: TMemo;
    chkDontShow: TCheckBox;
    procedure btnDoneClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmTextLoad: TfrmTextLoad;

implementation

{$R *.DFM}

procedure TfrmTextLoad.btnDoneClick(Sender: TObject);
begin
  close;
end;

procedure TfrmTextLoad.FormCreate(Sender: TObject);
var  DecGif: TGif;
begin

  {Load and decode gif from resource}
  DecGif := TGif.Create;
  DecGif.GifResourceToBitmap('textdemo', 'GIF', imgTextLoad.Picture.Bitmap);
end;

procedure TfrmTextLoad.FormClose(Sender: TObject; var Action: TCloseAction);
var
    KeyName: Array[0..50] of char;
    temp: Array[0..300] of char;
begin
  {Save TransOnOff}
  if chkDontShow.Checked then begin
    StrPCopy(KeyName, 'Show Textload');
    StrPCopy(temp, '0');
    WritePrivateProfileString(APP_NAME, KeyName, temp, FILE_NAME);
  end;
end;

end.
