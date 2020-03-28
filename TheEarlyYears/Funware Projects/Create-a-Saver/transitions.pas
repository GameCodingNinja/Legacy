unit transitions;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, constants, FileCtrl, Globals, gFunctin;

type
  TfrmTransitions = class(TForm)
    btnOK: TButton;
    btnCancel: TButton;
    chkTransitions: TCheckBox;
    scrTransSpeed: TScrollBar;
    lblTransSpeed: TLabel;
    GroupBox1: TGroupBox;
    rdoBlackFill: TRadioButton;
    rdoRandomFill: TRadioButton;
    edtDelay: TEdit;
    lblDelay1: TLabel;
    lblDelay2: TLabel;
    chkSizeBitmaps: TCheckBox;
    chkSoundDir: TCheckBox;
    rdoPaintNextScreen: TRadioButton;
    chkSoundPlayOrder: TCheckBox;
    chkTopText: TCheckBox;
    chkBottomText: TCheckBox;
    chkDontLetInter: TCheckBox;
    chkRandomBacks: TCheckBox;
    procedure btnCancelClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure scrTransSpeedChange(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure chkTransitionsClick(Sender: TObject);
    procedure edtDelayKeyPress(Sender: TObject; var Key: Char);
    procedure chkSoundDirClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmTransitions: TfrmTransitions;

implementation

{$R *.DFM}

{-----------------------------------------------------------------------}

procedure TfrmTransitions.btnCancelClick(Sender: TObject);
begin
     close;
end;

{-----------------------------------------------------------------------}

procedure TfrmTransitions.FormCreate(Sender: TObject);
begin
  // Set Trans on or off
  chkTransitions.Checked := gTransOnOff;

  // Set Size Bitmaps
  chkSizeBitmaps.Checked := gSizeBitmaps;

  // Fill TransDelay edit box
  edtDelay.Text := IntToStr(gTransDelay);

  // Set text settings
  chkTopText.Checked := gTopTextRand;
  chkBottomText.Checked := gBottomTextRand;

  // Do they want to randomize backgrounds
  chkRandomBacks.Checked := gRandomBacks;

  // Do they want sound files to interrupt?
  chkDontLetInter.Checked := gSoundFilesInterrupt;

  if gTransOnOff = false then begin
     chkTransitions.Caption := 'Transitions Off:';
     lblTransSpeed.Enabled := false;
     scrTransSpeed.Enabled := false;
     GroupBox1.Enabled := false;
     rdoBlackFill.Enabled := false;
     rdoRandomFill.Enabled := false;
     rdoPaintNextScreen.Enabled := False;
     //edtDelay.Enabled := False;
  end;

  // How do we want to fill it?
  if gTransFillColor = ERASE_FILL_BLACK then
     rdoBlackFill.Checked := True;
  if gTransFillColor = ERASE_FILL_COLORS then
     rdoRandomFill.Checked := True;
  if gTransFillColor = ERASE_WITH_NEXT_IMAGE then
     rdoPaintNextScreen.Checked := True;

  {if GetDeviceCaps( canvas.handle, BITSPIXEL ) <= 8 then begin
     rdoPaintNextScreen.Checked := False;
     rdoPaintNextScreen.Enabled := False;
     if gTransFillColor = ERASE_WITH_NEXT_IMAGE then
        rdoBlackFill.Checked := True;
  end;}

  // Set Sound Prefs
  chkSoundDir.Checked := gSoundsOnOff;
  if gSoundsOnOff then
    chkSoundDir.Caption := 'Sound Directory On:'
  else
    chkSoundDir.Caption := 'Sound Directory Off:';

  // Check to see if they picked a sound directory
  if gSoundsOnOff then begin
    if DirectoryExists(gSoundDir) then begin
       chkSoundPlayOrder.Enabled := True;
       chkSoundDir.Enabled := True;
    end else begin
       chkSoundPlayOrder.Enabled := False;
       chkSoundDir.Enabled := False;
       chkSoundPlayOrder.Enabled := False;
    end;
  end else begin
    chkSoundPlayOrder.Enabled := False;
    if not(DirectoryExists(gSoundDir)) then begin
      chkSoundDir.Enabled := False;
      chkSoundPlayOrder.Enabled := False;
    end;
  end;

  chkSoundPlayOrder.Checked := gSoundsRandomList;

  //How Fast do we want to go?
  scrTransSpeed.Position := gTransSpeed;
  case gTransSpeed of
     0: lblTransSpeed.Caption := 'Speed: Speedy';
     1: lblTransSpeed.Caption := 'Speed: Very Fast';
     2: lblTransSpeed.Caption := 'Speed: Fast';
     3: lblTransSpeed.Caption := 'Speed: Slow';
     4: lblTransSpeed.Caption := 'Speed: Slower';
     5: lblTransSpeed.Caption := 'Speed: Very Slow';
  end;

  // Check to see if they are using a text file
   if isPathValid(gTopText) then
     if FileExists(gTopText) then
        chkTopText.Enabled := True;

   if isPathValid(gBottomText) then
     if FileExists(gBottomText) then
        chkBottomText.Enabled := True;

   // Disable all the controls but the speed if Designer is on
   if gScreenDesignOnOff then begin
     lblDelay1.Enabled := False;
     lblDelay2.Enabled := False;
     edtDelay.Enabled := False;
     //chkTransitions.Enabled := False;
     chkSizeBitmaps.Enabled := False;
     chkSoundDir.Enabled := False;
     chkSoundPlayOrder.Enabled := False;
     chkRandomBacks.Enabled := false;
     chkTopText.Enabled := False;
     chkBottomText.Enabled := False;
     GroupBox1.Enabled := False;
     rdoBlackFill.Enabled := False;
     rdoRandomFill.Enabled := False;
     rdoPaintNextScreen.Enabled := False;
   end;
end;

{-----------------------------------------------------------------------}

procedure TfrmTransitions.scrTransSpeedChange(Sender: TObject);
begin
  case scrTransSpeed.Position of
     0: lblTransSpeed.Caption := 'Speed: Speedy';
     1: lblTransSpeed.Caption := 'Speed: Very Fast';
     2: lblTransSpeed.Caption := 'Speed: Fast';
     3: lblTransSpeed.Caption := 'Speed: Slow';
     4: lblTransSpeed.Caption := 'Speed: Slower';
     5: lblTransSpeed.Caption := 'Speed: Very Slow';
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmTransitions.btnOKClick(Sender: TObject);
begin
     // Save TransOnOff
     gTransOnOff := chkTransitions.Checked;

     gSoundFilesInterrupt := chkDontLetInter.Checked;

     // Save Random backgrounds
     gRandomBacks := chkRandomBacks.Checked;

     // Save Size Bitmaps
     gSizeBitmaps := chkSizeBitmaps.Checked;

     // Save Trans Speed
     gTransSpeed := scrTransSpeed.Position;

     // Save Sounds Random or Order
     gSoundsRandomList := chkSoundPlayOrder.Checked;

     // Set text settings
     gTopTextRand := chkTopText.Checked;
     gBottomTextRand := chkBottomText.Checked;

     // Save TransFillColor
     if rdoBlackFill.Checked then
       gTransFillColor := ERASE_FILL_BLACK;
     if rdoRandomFill.Checked then
       gTransFillColor := ERASE_FILL_COLORS;
     if rdoPaintNextScreen.Checked then
       gTransFillColor := ERASE_WITH_NEXT_IMAGE;

     // Save TransDelay
     if edtDelay.Text = '' then
       gTransDelay := 0
     else
       gTransDelay := StrToInt(edtDelay.Text);

     // Save sound on or off
     if chkSoundDir.Enabled then begin
       if chkSoundDir.Checked then
         gSoundsOnOff := True
       else
         gSoundsOnOff := False;
     end;

     close;

end;

{-----------------------------------------------------------------------}

procedure TfrmTransitions.chkTransitionsClick(Sender: TObject);
begin
  if chkTransitions.Checked then begin
     chkTransitions.Caption := 'Transitions On:';
     lblTransSpeed.Enabled := true;
     scrTransSpeed.Enabled := true;
     if not(gScreenDesignOnOff) then begin
       GroupBox1.Enabled := true;
       rdoBlackFill.Enabled := true;
       rdoRandomFill.Enabled := true;
       lblDelay1.Enabled := true;
       lblDelay2.Enabled := true;
       edtDelay.Enabled := true;
       if GetDeviceCaps( canvas.handle, BITSPIXEL ) > 8 then
         rdoPaintNextScreen.Enabled := true;
     end;
  end else begin
     chkTransitions.Caption := 'Transitions Off:';
     lblTransSpeed.Enabled := false;
     scrTransSpeed.Enabled := false;
     if not(gScreenDesignOnOff) then begin
       GroupBox1.Enabled := false;
       rdoBlackFill.Enabled := false;
       rdoRandomFill.Enabled := false;
       rdoPaintNextScreen.Enabled := False;
       lblDelay1.Enabled := false;
       lblDelay2.Enabled := false;
       edtDelay.Enabled := false;
     end;  
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmTransitions.edtDelayKeyPress(Sender: TObject; var Key: Char);
begin
     // Only let number keys through
     if Byte(Key) = 8 then exit; // Let backspace through
     if (Byte(Key) < 48) or (Byte(Key) > 57) then
       Key := Char(0);
end;

{-----------------------------------------------------------------------}

procedure TfrmTransitions.chkSoundDirClick(Sender: TObject);
begin
  if chkSoundDir.Checked then begin
     chkSoundPlayOrder.Enabled := True;
     chkSoundDir.Caption := 'Sound Directory On:'
  end else begin
     chkSoundPlayOrder.Enabled := False;
     chkSoundDir.Caption := 'Sound Directory Off:'
  end;
end;

end.
