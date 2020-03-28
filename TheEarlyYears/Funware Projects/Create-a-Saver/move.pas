unit move;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, constants;

type
  TfrmMove = class(TForm)
    butOK: TButton;
    butCancel: TButton;
    Label1: TLabel;
    cboNewPos: TComboBox;
    procedure butCancelClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure butOKClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMove: TfrmMove;

implementation

{$R *.DFM}

procedure TfrmMove.butCancelClick(Sender: TObject);
begin
  close;
end;

procedure TfrmMove.FormCreate(Sender: TObject);
var I: Integer;
begin
  for I := 1 to gTotalScr do begin
     if I = gCurrentScr then
       cboNewPos.Items.Add(' Current Screen to Be Moved' )
     else
       cboNewPos.Items.Add(' Move to Screen No. '+ IntToStr(I));
  end;

  cboNewPos.ItemIndex := gCurrentScr - 1;
end;

procedure TfrmMove.butOKClick(Sender: TObject);
begin

  // Keeps it from causing an error (-1) when user
  // clicks it the wrong way
  if cboNewPos.ItemIndex < 0 then close;;

  gCurrentScr := cboNewPos.ItemIndex + 1;

  close;
end;

end.
