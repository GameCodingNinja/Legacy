unit fullscreen;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  constants, gFunctin, Globals;

type
  TfrmFullScreenView = class(TForm)
    procedure FormClick(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
    procedure FormCreate(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure WMEraseBkgnd (var Msg : TMessage); message WM_ERASEBKGND;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmFullScreenView: TfrmFullScreenView;

implementation

{$R *.DFM}

procedure TfrmFullScreenView.FormClick(Sender: TObject);
begin
  close;
end;

procedure TfrmFullScreenView.FormKeyPress(Sender: TObject; var Key: Char);
begin
    close;
end;

procedure TfrmFullScreenView.FormCreate(Sender: TObject);
begin
  Top := 0;
  Left := 0;
  Height := Screen.Height;
  Width := Screen.Width;
end;

procedure TfrmFullScreenView.FormPaint(Sender: TObject);
var r: TRect;
begin
  LoadBitmapPalettes( Canvas, ArtPalette, BackPalette, gSettings );
  r := Rect(0, 0, Screen.Width, Screen.Height);
  Canvas.CopyRect(r, buffer.Canvas, r);
end;

procedure TfrmFullScreenView.WMEraseBkGnd;
begin
  Msg.Result := 0; {don't write the background}
end;

procedure TfrmFullScreenView.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
     Release;
end;

end.
