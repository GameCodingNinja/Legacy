unit password;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, constants;

type
  TfrmPassSetup = class(TForm)
    btnCancel: TButton;
    btnOK: TButton;
    lblPassword: TLabel;
    edtPass1: TEdit;
    edtPass2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    procedure btnCancelClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPassSetup: TfrmPassSetup;

implementation

{$R *.DFM}

{-----------------------------------------------------------------------}

procedure TfrmPassSetup.btnCancelClick(Sender: TObject);
begin
     //They didn't enter in the password so shut them down
     if (not(gAskForPassword)) then gPasswordEnabled := '';
     close; {close and exit without saving conig}
end;

{-----------------------------------------------------------------------}

procedure TfrmPassSetup.FormCreate(Sender: TObject);
begin
   if gAskForPassword then begin
     lblPassword.Width := (btnOK.Left + btnOK.Width) - lblPassword.Left;
     {Password protection copy}
     lblPassword.Caption := 'Because of the personal nature of this screen ' +
     'saver, you may want to password protect the preferences window. ' +
     'This will keep others from altering your choices or adding ' +
     'inappropriate material.';
   end;
end;

{-----------------------------------------------------------------------}

procedure TfrmPassSetup.btnOKClick(Sender: TObject);
begin
  if gAskForPassword then begin
     if (edtPass1.Text = '') or (edtPass2.Text = '') then begin
       Application.MessageBox('You need to type your password in both edit boxes.',
       'Password Protection Error', mb_OK + MB_ICONEXCLAMATION);
       edtPass1.SetFocus;
       exit;
     end;

     if edtPass1.Text = edtPass2.Text then begin
       gPasswordEnabled := edtPass1.Text;
       close;
     end else begin
       Application.MessageBox('the first password entered does not match ' +
       'the second password. Please re-enter your password.',
       'Password Protection Error', mb_OK + MB_ICONEXCLAMATION);
       edtPass1.Text := '';
       edtPass2.Text := '';
       edtPass1.SetFocus;
     end;
  end else begin
     if edtPass1.Text = '' then begin
       Application.MessageBox('You need to type in your password to gain ' +
       'access to the preferences window.',
       'Password Protection Error', mb_OK + MB_ICONEXCLAMATION);
       edtPass1.SetFocus;
     end else if edtPass1.Text = gPasswordEnabled then
       close
     else begin
       Application.MessageBox('The password you typed in is not correct, please ' +
       'type in the correct password.',
       'Password Protection Error', mb_OK + MB_ICONEXCLAMATION);
       edtPass1.Text := '';
       edtPass1.SetFocus;
     end;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmPassSetup.FormActivate(Sender: TObject);
begin
     edtPass1.SetFocus; // Give the first edit box the focus
end;

{-----------------------------------------------------------------------}

procedure TfrmPassSetup.FormShow(Sender: TObject);
begin
   if gAskForPassword = false then begin
     // Change window for single password entry
     lblPassword.AutoSize := True;
     edtPass2.Visible := false;
     Label1.Visible := false;
     Label2.Visible := false;
     edtPass1.Top := lblPassword.Top + lblPassword.Height + 16;
     edtPass1.Left := lblPassword.Left;
     btnOK.Left := edtPass1.Left + edtPass1.Width + 15;
     btnCancel.Left := btnOK.Left;
     btnCancel.Top := (edtPass1.Top + edtPass1.Height) - btnCancel.Height;
     btnOK.Top := btnCancel.Top - btnOK.Height - 7;
     lblPassword.Width := edtPass1.Width;
     frmPassSetup.ClientWidth := btnOK.Left + btnOK.Width + 16;
     frmPassSetup.ClientHeight := edtPass1.Top + edtPass1.Height + 16;
   end;
end;

end.
