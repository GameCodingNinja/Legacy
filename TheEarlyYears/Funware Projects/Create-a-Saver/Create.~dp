program Create;

uses
  savInit in '..\hlibrary\savInit.pas',
  Forms,
  Windows,
  savRun in '..\hlibrary\savRun.pas' {frmFullScreen},
  savThr in 'savThr.pas',
  savConf in 'savConf.pas',
  constants in 'constants.pas',
  Globals in 'globals.pas',
  MainForm in '..\hlibrary\Order Form\MainForm.pas' {frmOrderForm},
  noPrinterAv in '..\hlibrary\Order Form\noPrinterAv.pas' {frmNoPrinter},
  printingToPrinter in '..\hlibrary\Order Form\printingToPrinter.pas' {frmPrintingOrderForm},
  gOrderfm in '..\hlibrary\Order Form\gOrderfm.pas',
  usingSD in 'usingSD.pas' {frmUsingSD},
  DecodeGif in '..\hlibrary\DecodeGif.pas',
  gFunctin in '..\hlibrary\gFunctin.pas',
  SoundMgr in '..\hlibrary\soundMgr.pas',
  PlaySnd in '..\hlibrary\PlaySnd.pas' {frmSoundDir},
  designer in 'designer.pas' {frmDesigner},
  TwoWays in 'TwoWays.pas' {frmTwoWays},
  fullscreen in 'fullscreen.pas' {frmFullScreenView},
  TempRegInfoWnd in '..\hlibrary\Order Form\TempRegInfoWnd.pas' {frmTempRegWnd},
  runningSD in 'runningSD.pas' {frmRunningDesigner},
  hColor in 'hColor.pas' {frmHColor},
  SoundWeb in 'SoundWeb.pas' {frmAddingSounds},
  move in 'move.pas' {frmMove},
  mediaPlayer in '..\hlibrary\mediaPlayer.pas',
  ImgLoad in 'ImgLoad.pas',
  OrderOption in '..\hlibrary\Order Form\OrderOption.pas' {frmChoice};


{$R *.RES}
{$R AllRes.RES}
{$R CreateAt.RES}

begin
   Randomize;
  // For stepping through saver code
  if DEBUGIT then begin
      Application.Initialize;
      Application.Title := 'Create-A-Saver';
      Application.CreateForm(TfrmFullScreen, frmFullScreen);
  Application.CreateForm(TfrmMID, frmMID);
  Application.CreateForm(TfrmChoice, frmChoice);
  Application.Run;
      SetForegroundWindow (ConfigParent);
  end else begin
  if SaverMode = savModeExecute then begin {do the full-screen-mode}
      Application.Initialize;
      Application.Title := 'Create-A-Saver';
      Application.CreateForm(TfrmFullScreen, frmFullScreen);
      Application.CreateForm(TfrmMID, frmMID);
      Application.Run;
      {and after doing the full-screen-mode, reactivate a previously active window}
      if (SaverMode = savModeExecute) and (ConfigParent <> 0) then
        SetForegroundWindow (ConfigParent);
    end;
    if (SaverMode = savModeConfigure) or (SaverMode = savModeConfigureStd) then begin
      Application.Initialize; {do the config-dialog}
      Application.CreateForm(TfrmConfig, frmConfig);
      Application.CreateForm(TfrmMID, frmMID);
      Application.CreateForm( TfrmImageLoad, frmImageLoad );
      Application.CreateForm( TfrmSoundDir, frmSoundDir );
      Application.CreateForm( TfrmDesigner, frmDesigner );
      Application.Run;
    end;
    if (SaverMode = savModePreview) and (ParamHandle <> 0) then begin
      ExecPreview; {run in preview mode}
    end;
    if (SaverMode = savModeSetPwd) and (ParamHandle <> 0) then
      ExecSetPwd; {show the password-change-dialog}
  end;
end.
