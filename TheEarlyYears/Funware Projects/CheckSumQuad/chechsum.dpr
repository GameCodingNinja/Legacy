program chechsum;

uses
  Forms,
  main in 'main.pas' {frmChecksum};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TfrmChecksum, frmChecksum);
  Application.Run;
end.
