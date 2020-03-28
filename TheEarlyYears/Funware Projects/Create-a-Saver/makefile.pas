unit makefile;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TForm2 = class(TForm)
    Edit1: TEdit;
    Label1: TLabel;
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

uses main;

{$R *.DFM}

procedure TForm2.Button1Click(Sender: TObject);
var
   FileHandleTmp: Integer;
begin
   if not(Edit1.Text = '') then begin
       Form1.CurrentlyOpenedFile := Form1.FileListBox1.Directory + '\' + Edit1.Text + '.fcr';
       FileHandleTmp := FileCreate( Form1.CurrentlyOpenedFile );
       if FileHandleTmp > 0 then
          FileClose( FileHandleTmp );
       Form1.KeyPressTouch := True;
       Form1.ListBox1.Clear;
       Form1.ResponceList.Clear;
       Form1.TitleList.Clear;
       Form1.CurrentSelection := -1;
       Form1.mnuAddClick(nil);
   end;
   close;
end;

end.
