unit designer;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs, runningSD,
  StdCtrls, Menus, FileCtrl, ExtCtrls, Buttons, constants, Loadpref, ImgLoad, hColor,
  PlaySnd, Globals, ComCtrls, gFunctin, fullscreen, usingSD, SoundWeb, move, mediaPlayer;

type

  TfrmDesigner = class(TForm)
    chkDesignerOnOff: TCheckBox;
    MainMenu1: TMainMenu;
    mnuFile: TMenuItem;
    mnuNew: TMenuItem;
    mnuOpen: TMenuItem;
    mnuSep1: TMenuItem;
    mnuExit: TMenuItem;
    pntPreview: TPaintBox;
    lblMessageTop: TLabel;
    btnTopLeft: TSpeedButton;
    btnTopCenter: TSpeedButton;
    btnTopRight: TSpeedButton;
    edtTop: TEdit;
    btnFontsTop: TSpeedButton;
    btnColorsTop: TSpeedButton;
    lblMessageBottom: TLabel;
    btnBottomLeft: TSpeedButton;
    btnBottomCenter: TSpeedButton;
    btnBottomRight: TSpeedButton;
    edtBottom: TEdit;
    btnFontsBottom: TSpeedButton;
    btnColorsBottom: TSpeedButton;
    FontDialog1: TFontDialog;
    ColorDialog1: TColorDialog;
    btnLeft: TSpeedButton;
    btnRight: TSpeedButton;
    btnDelete: TSpeedButton;
    btnNew: TSpeedButton;
    mnuSave: TMenuItem;
    lblDelay1: TLabel;
    edtDelay: TEdit;
    lblScreen: TLabel;
    chkSizeBitmap: TCheckBox;
    btnSound: TSpeedButton;
    cboDrawFill: TComboBox;
    lblFillStyle: TLabel;
    OpenDialog1: TOpenDialog;
    mnuSaveAs: TMenuItem;
    cboImages: TComboBox;
    cboBackgnd: TComboBox;
    Label4: TLabel;
    Label3: TLabel;
    StatusBar1: TStatusBar;
    mnuExtra: TMenuItem;
    mnuRandom: TMenuItem;
    seperator: TMenuItem;
    mnuSwitchLeft: TMenuItem;
    mnuSwitchRight: TMenuItem;
    SaveDialog1: TSaveDialog;
    seperator2: TMenuItem;
    mnuFullScreenView: TMenuItem;
    Timer1: TTimer;
    mnuHelp: TMenuItem;
    mnuContents: TMenuItem;
    mnuLoadMainImage: TMenuItem;
    mnuBackImage: TMenuItem;
    mnuSound: TMenuItem;
    mnuUnloadSound: TMenuItem;
    mnuSep2: TMenuItem;
    mnuSep3: TMenuItem;
    mnuSoundsOnOff: TMenuItem;
    mnuMove: TMenuItem;
    lblCurrentScr: TLabel;
    edtCurrent: TEdit;
    procedure mnuExitClick(Sender: TObject);
    procedure pntPreviewPaint(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure chkDesignerOnOffClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure mnuBackImageClick(Sender: TObject);
    procedure mnuLoadMainImageClick(Sender: TObject);
    procedure edtTopKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure edtBottomKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure btnFontsTopClick(Sender: TObject);
    procedure btnTopLeftClick(Sender: TObject);
    procedure cboDrawFillChange(Sender: TObject);
    procedure edtDelayKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure btnNewClick(Sender: TObject);
    procedure btnLeftClick(Sender: TObject);
    procedure btnRightClick(Sender: TObject);
    procedure mnuSoundClick(Sender: TObject);
    procedure btnSoundClick(Sender: TObject);
    procedure edtDelayKeyPress(Sender: TObject; var Key: Char);
    procedure mnuUnloadSoundClick(Sender: TObject);
    procedure chkSizeBitmapMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure mnuSaveClick(Sender: TObject);
    procedure mnuOpenClick(Sender: TObject);
    procedure mnuSaveAsClick(Sender: TObject);
    procedure mnuNewClick(Sender: TObject);
    procedure btnDeleteClick(Sender: TObject);
    procedure cboImagesChange(Sender: TObject);
    procedure cboBackgndChange(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure pntPreviewClick(Sender: TObject);
    procedure mnuSwitchLeftClick(Sender: TObject);
    procedure mnuRandomClick(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure mnuFullScreenViewClick(Sender: TObject);
    procedure chkDesignerOnOffMouseUp(Sender: TObject;
      Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure Timer1Timer(Sender: TObject);
    procedure mnuContentsClick(Sender: TObject);
    procedure mnuSoundsOnOffClick(Sender: TObject);
    procedure mnuMoveClick(Sender: TObject);
    procedure edtCurrentKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure edtCurrentKeyPress(Sender: TObject; var Key: Char);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    //artWMF, backWMF: TMetafile;
    //artBMP, backBMP, buffer: TBitmap;
    pScreenD, pScrBase: PTScreenDesign;
    LastSelected: TScreenDesign;
    CurrentScr, TotalScr: LongInt;
    Settings: TScrnSavSettings;
    ShowBox, ActiveDoc, OpenedDoc, ColorMode256: Boolean;
    FileLoaded: String;
    resList, resBack: TStringList;

    procedure EnableDisableControls( OnOff: Boolean );
    procedure UpdateScreen( pS: PTScreenDesign );
    procedure ReSetToDefault( pS: PTScreenDesign );
    procedure PaintPreviewWnd;
    procedure SaveLastSelected;
    function IsScreenBlank( pS: PTScreenDesign ): Boolean;
    function IsThereABlankPage: Boolean;
    function ResetMemory( I: Integer ): Boolean;
    function OpenScreenFile( FileNameStr: String ):Boolean;
    procedure DoWeNeedToSave;
    procedure LoadScreen;
    //procedure GetThePalettes;
    //procedure DeleteThePalettes;
  end;

var
  frmDesigner: TfrmDesigner;

implementation

{$R *.DFM}

procedure TfrmDesigner.mnuExitClick(Sender: TObject);
begin
  Close;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.pntPreviewPaint(Sender: TObject);
begin
  PaintPreviewWnd;
end; // pntPreviewPaint

{-----------------------------------------------------------------------}

procedure TfrmDesigner.FormCreate(Sender: TObject);
var
     t, res: PChar;
     size, I: longInt;
begin

  resList := TStringList.Create;
  resBack := TStringList.Create;
  ArtPalette := 0;
  BackPalette := 0;

  Settings := TScrnSavSettings.Create;

  cboDrawFill.Items.Add('Black Fill Color');
  cboDrawFill.Items.Add('Random Fill Color');
  cboDrawFill.Items.Add('Paint w/Next Screen');
  if GetDeviceCaps( canvas.handle, BITSPIXEL ) > 8 then
    ColorMode256 := False
  else
    ColorMode256 := True;
  //cboDrawFill.ItemIndex := 0;

  // Set these only once
  LastSelected.SizeBitmaps := False;
  LastSelected.TopFont := 'Arial';
  LastSelected.TopSize := 42;
  LastSelected.TopBold := True;
  LastSelected.TopItal := False;
  LastSelected.TopUnder := False;
  LastSelected.TopStrike := False;
  LastSelected.TopColor := clYellow;
  LastSelected.TopTextPos := CENTER_TEXT;

  LastSelected.BottomFont := 'Arial';
  LastSelected.BottomSize := 42;
  LastSelected.BottomBold := True;
  LastSelected.BottomItal := False;
  LastSelected.BottomUnder := False;
  LastSelected.BottomStrike := False;
  LastSelected.BottomColor := clLime;
  LastSelected.BottomTextPos := CENTER_TEXT;

  LastSelected.TransDelay := 7;
  LastSelected.TransFillColor := ERASE_WITH_NEXT_IMAGE;

   // Clear out the lists and add the defaults
   cboImages.clear;
   resList.Clear;
   cboImages.Items.Add('None');  // Give them the option of none.
   cboImages.Items.Add('Load your own...');
   resList.Add('None');
   resList.Add('Load your own...');
   res := StrNew('imagelist');

   size := loadSubjectList(t, res);
   if size > 0 then begin
     stripOutText(t, cboImages, resList, size);
     StrDispose(t);
   end;
   StrDispose(res);

  // Load up the Background Images combo box
  cboBackgnd.Items.Add('None');
  resBack.Add('None');
  cboBackgnd.Items.Add('Load your own...');
  resBack.Add('Load your own...');
  cboBackgnd.Items.Add('Solid Color');
  resBack.Add('Solid Color');
  for I := 1 to 39 do begin
     cboBackgnd.Items.Add('Background '+ IntToStr(I));
     resBack.Add('back'+ IntToStr(I));
  end;

  cboImages.ItemIndex := 0;
  cboBackgnd.ItemIndex := 0;


end; // FormCreate

{-----------------------------------------------------------------------}

procedure TfrmDesigner.EnableDisableControls( OnOff: Boolean );
begin
    if CurrentScr > 1 then begin
      btnLeft.Enabled := OnOff;
      mnuSwitchLeft.Enabled := OnOff;
    end;
    if TotalScr > 1 then begin
      btnRight.Enabled := OnOff;
      mnuSwitchRight.Enabled := OnOff;
    end;
    if TotalScr > 1 then
      btnDelete.Enabled := OnOff;

    mnuSave.Enabled := OnOff;
    mnuSaveAs.Enabled := OnOff;
    mnuLoadMainImage.Enabled := OnOff;
    mnuBackImage.Enabled := OnOff;
    mnuSound.Enabled := OnOff;
    mnuRandom.Enabled := OnOff;
    //mnuFullScreenView.Enabled := OnOff;
    btnNew.Enabled := OnOff;
    btnFontsTop.Enabled := OnOff;
    btnColorsTop.Enabled := OnOff;
    btnFontsBottom.Enabled := OnOff;
    btnColorsBottom.Enabled := OnOff;
    btnTopLeft.Enabled := OnOff;
    btnTopCenter.Enabled := OnOff;
    btnTopRight.Enabled := OnOff;
    btnBottomLeft.Enabled := OnOff;
    btnBottomCenter.Enabled := OnOff;
    btnBottomRight.Enabled := OnOff;
    edtTop.Enabled := OnOff;
    edtBottom.Enabled := OnOff;
    cboDrawFill.Enabled := OnOff;
    edtDelay.Enabled := OnOff;
    lblFillStyle.Enabled := OnOff;
    lblDelay1.Enabled := OnOff;
    lblMessageTop.Enabled := OnOff;
    lblMessageBottom.Enabled := OnOff;
    lblCurrentScr.Enabled := OnOff;
    cboImages.Enabled := OnOff;
    cboBackgnd.Enabled := OnOff;
end; // EnableDisableControls

{-----------------------------------------------------------------------}

procedure TfrmDesigner.chkDesignerOnOffClick(Sender: TObject);
begin
  gScreenDesignOnOff := chkDesignerOnOff.Checked;
  
  // Turn on or off all the controls
  if chkDesignerOnOff.Checked then begin
    chkDesignerOnOff.Caption := 'Screen Designer On:';
    EnableDisableControls( True );
  end else begin
    chkDesignerOnOff.Caption := 'Screen Designer Off:';
    EnableDisableControls( False );
    mnuFullScreenView.Enabled := False;
  end;
end; // chkDesignerOnOffClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.FormClose(Sender: TObject;
  var Action: TCloseAction);
var
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin

  if (TotalScr = 1) and not(ActiveDoc) and not(OpenedDoc) then
    gScreenDesignOnOff := False;

  {Get Show Window}
  StrPCopy(KeyName, 'Show Screen Designer Run');
  StrPCopy(aDefault, '1');
  GetPrivateProfileString(APP_NAME, KeyName, aDefault, temp, SizeOf(temp), FILE_NAME);
  ShowAgain := Boolean(StrToInt(String(temp)));

  if ShowAgain and gScreenDesignOnOff then begin
    frmRunningDesigner := TfrmRunningDesigner.Create(Application);
    frmRunningDesigner.ShowModal;
    frmRunningDesigner.Release;
  end;


  // Clean-up
  if ActiveDoc then begin
    //mnuNewClick(Sender);
    DoWeNeedToSave;

    // If they cancel or save, ActiveDoc will be false
    if ActiveDoc then begin
      Action := caNone;
      exit;
    end;

    if not(OpenedDoc) then
       gScreenDesignOnOff := False;

  end;

    // Unload the midi player
    frmMID.ClosePlayer;
end; // FormClose

{-----------------------------------------------------------------------}

procedure TfrmDesigner.ReSetToDefault( pS: PTScreenDesign );
begin
  pS^.MainImg := '';
  pS^.MainFType := NONE;
  pS^.MainResIndex := NONE;
  pS^.SizeBitmaps := LastSelected.SizeBitmaps;
  pS^.TopMsg := '';

  pS^.BackImg := '';
  pS^.BackFType := NONE;
  pS^.BackResIndex := NONE;
  pS^.BkFillColor := clWhite;
  pS^.BottomMsg := '';

  pS^.SoundFile := '';
  ps^.SoundOnOff := False;

  pScreenD^.TopFont := LastSelected.TopFont;
  pScreenD^.TopSize := LastSelected.TopSize;
  pScreenD^.TopBold := LastSelected.TopBold;
  pScreenD^.TopItal := LastSelected.TopItal;
  pScreenD^.TopUnder := LastSelected.TopUnder;
  pScreenD^.TopStrike := LastSelected.TopStrike;
  pScreenD^.TopColor := LastSelected.TopColor;
  pScreenD^.TopTextPos := LastSelected.TopTextPos;

  pScreenD^.BottomFont := LastSelected.BottomFont;
  pScreenD^.BottomSize := LastSelected.BottomSize;
  pScreenD^.BottomBold := LastSelected.BottomBold;
  pScreenD^.BottomItal := LastSelected.BottomItal;
  pScreenD^.BottomUnder := LastSelected.BottomUnder;
  pScreenD^.BottomStrike := LastSelected.BottomStrike;
  pScreenD^.BottomColor := LastSelected.BottomColor;
  pScreenD^.BottomTextPos := LastSelected.BottomTextPos;

  pScreenD^.TransDelay := LastSelected.TransDelay;
  pScreenD^.TransFillColor := LastSelected.TransFillColor;

end;  // ReSetToDefault

{-----------------------------------------------------------------------}

procedure TfrmDesigner.UpdateScreen( pS: PTScreenDesign );
var MainImgStr, BackImageStr, tempStr: string;
    RGBint: PLong;
    pRGB: PTRGB;
begin
  //Turn on hint for sound button
  if (pS^.SoundFile <> '') and pS^.SoundOnOff then begin
    btnSound.ShowHint := True;
    btnSound.Hint := pS^.SoundFile;
  end else begin
    btnSound.ShowHint := False;
    btnSound.Hint := '';
  end;  

  // Set the Size BMP button
  if pS^.MainFType > RESOURCE then begin
   chkSizeBitmap.Enabled := True;
   chkSizeBitmap.Checked := pS^.SizeBitmaps;
  end else begin
   chkSizeBitmap.Enabled := False;
   chkSizeBitmap.Checked := False;
  end;


  edtTop.Text := pS^.TopMsg;
  edtTop.Font.Name := pS^.TopFont;
  edtTop.Font.Style := [];
  if pS^.TopBold then
    edtTop.Font.Style := edtTop.Font.Style + [fsBold];
  if pS^.TopItal then
    edtTop.Font.Style := edtTop.Font.Style + [fsItalic];
  if pS^.TopUnder then
    edtTop.Font.Style := edtTop.Font.Style + [fsUnderline];
  if pS^.TopStrike then
    edtTop.Font.Style := edtTop.Font.Style + [fsStrikeout];

  // Set the justification buttons
  case pS^.TopTextPos of
     LEFT_JUST: btnTopLeft.Down := True;
     CENTER_TEXT: btnTopCenter.Down := True;
     RIGHT_JUST: btnTopRight.Down := True;
  end;

  edtBottom.Text := pS^.BottomMsg;
  edtBottom.Font.Name := pS^.BottomFont;
  edtBottom.Font.Style := [];
  if pS^.BottomBold then
    edtBottom.Font.Style := edtBottom.Font.Style + [fsBold];
  if pS^.BottomItal then
    edtBottom.Font.Style := edtBottom.Font.Style + [fsItalic];
  if pS^.BottomUnder then
    edtBottom.Font.Style := edtBottom.Font.Style + [fsUnderline];
  if pS^.BottomStrike then
    edtBottom.Font.Style := edtBottom.Font.Style + [fsStrikeout];

  // Set the justification buttons
  case pS^.BottomTextPos of
     LEFT_JUST: btnBottomLeft.Down := True;
     CENTER_TEXT: btnBottomCenter.Down := True;
     RIGHT_JUST: btnBottomRight.Down := True;
  end;

  edtDelay.Text := IntToStr(pS^.TransDelay);
  cboDrawFill.ItemIndex := pS^.TransFillColor;
  btnSound.Enabled := ps^.SoundOnOff;
  mnuUnloadSound.Enabled := ps^.SoundOnOff;

  cboImages.ItemIndex := ps^.MainResIndex;
  cboBackgnd.ItemIndex := ps^.BackResIndex;

  if ShowBox and not(gScreenDesignOnOff) then begin
    lblCurrentScr.Caption := 'of 0';
    edtCurrent.Text := '0';
  end else begin
    lblCurrentScr.Caption := 'of '+IntToStr(TotalScr);
    edtCurrent.Text := IntToStr(CurrentScr);
  end;

  case pS^.MainFType of
     NONE: MainImgStr := ' No Image Loaded';
     RESOURCE: MainImgStr := 'Main Image: Internal Stock Image';
     BMP: begin
       tempStr := pS^.MainImg;
       delete(tempStr, Length(tempStr)-2, 3);
       tempStr := tempStr + 'msk';
       if FileExists( tempStr ) then
         MainImgStr := 'Main/Transparent: ' + pS^.MainImg
       else
         MainImgStr := 'Main: ' + pS^.MainImg;
     end;
     WMF: MainImgStr := 'Main: ' + pS^.MainImg;
  end;

  case pS^.BackFType of
     NONE: BackImageStr := ' No Image Loaded';
     RESOURCE: BackImageStr := 'Tiled: Internal Stock Image';
     BMP, WMF: BackImageStr := 'Tiled: ' + pS^.BackImg;
     COLORS: begin // Show the RGB values
       New(RGBint);
       RGBint^ := ColorToRGB(pS^.BkFillColor);
       pRGB := PTRGB(RGBint);
       BackImageStr := 'Solid Color Fill:  Red ' + IntToStr(pRGB^.R);
       BackImageStr := BackImageStr + ',  Green ' + IntToStr(pRGB^.G);
       BackImageStr := BackImageStr + ',  Blue ' + IntToStr(pRGB^.B);
       Dispose(RGBint);
     end;
  end;

  if TotalScr > 3 then
    mnuMove.Enabled := True
  else
    mnuMove.Enabled := False;

  if (CurrentScr = 1) and (TotalScr > 1) then begin
    btnLeft.Enabled := False;
    mnuSwitchLeft.Enabled := False;
    btnRight.Enabled := True;
    mnuSwitchRight.Enabled := True;
  end;

  if (CurrentScr = TotalScr) and (TotalScr > 1) then begin
    btnLeft.Enabled := True;
    mnuSwitchLeft.Enabled := True;
    btnRight.Enabled := False;
    mnuSwitchRight.Enabled := False;
  end;

  if (CurrentScr > 1) and (CurrentScr < TotalScr) then begin
    btnLeft.Enabled := True;
    mnuSwitchLeft.Enabled := True;
    btnRight.Enabled := True;
    mnuSwitchRight.Enabled := True;
  end;    

  StatusBar1.Panels[0].Text := MainImgStr;
  StatusBar1.Panels[1].Text := BackImageStr;
end; // UpdateScreen

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuBackImageClick(Sender: TObject);
var FileTypeTemp: Integer;
begin
  // Init Global variables
  FileTypeTemp := NONE;
  gImageDirStr := pScreenD^.BackImg;
  gCancelPressed := False;
  //Make sure thay don't choose the same directory
  gOtherImageDirStr := pScreenD^.MainImg;
  gFileType := pScreenD^.BackFType;
  gSingleDir := LOAD;
  gDirRandomSort := SORTED_FILES_IN_DIR;
  gMainBack := BITMAP;
  gTileGraphic := True;
  gTurnOffImageModeSelections := True;

  //frmImageLoad := TfrmImageLoad.Create(Application);
  frmImageLoad.ShowModal;
  //frmImageLoad.Release;

  // They didn't do anything
  if (gImageDirStr = pScreenD^.BackImg) or gCancelPressed then exit;

  pScreenD^.BackImg := gImageDirStr;
  pScreenD^.BackFType := gFileType;
  Settings.BKImage := pScreenD^.BackImg;
  Settings.BkFileType := pScreenD^.BackFType;

  if not(gCancelPressed) and isPathValid(gImageDirStr) then begin

    if (BackPalette <> 0) and ColorMode256 then begin
      DeleteObject(BackPalette);
      BackPalette := 0;
    end;

    FileTypeTemp := loadFile(Settings.BKImage,backBMP,backWMF,SHOW_ERROR_MSG);
    if FileTypeTemp > 0 then begin
       Settings.BKFileType := FileTypeTemp;
       pScreenD^.BackFType := FileTypeTemp;
       if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
         if backBMP.PixelFormat = pf8bit then
           BackPalette := backBMP.ReleasePalette;
    end;
  end;

  {Paint the buffer with what the user last selected}
  if not(gCancelPressed) and  (FileTypeTemp > 0) then begin
    ActiveDoc := True;
    pScreenD^.BackResIndex := LOAD;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    ShowBox := False;
    UpdateScreen( pScreenD );
    PaintPreviewWnd;
  end;
end; // mnuBackImageClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.PaintPreviewWnd;
var r: TRect;
begin

 if ShowBox then begin
   mnuFullScreenView.Enabled := False;
   pntPreview.Canvas.Pen.Color := clBlack;
   if gScreenDesignOnOff then
     pntPreview.Canvas.Brush.Color := clWhite
   else
     pntPreview.Canvas.Brush.Color := clBtnFace;
   pntPreview.Canvas.Rectangle(0, 0, pntPreview.Width, pntPreview.Height);
 end else begin
   LoadBitmapPalettes( pntPreview.Canvas, artBMP.Palette, BackBMP.Palette, Settings );
   r := Rect(0,0,pntPreview.Width,pntPreview.Height);
   pntPreview.Canvas.StretchDraw(r, buffer);
   mnuFullScreenView.Enabled := True;
 end;

end; // PaintPreviewWnd

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuLoadMainImageClick(Sender: TObject);
var FileTypeTemp: Integer;
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin
  // Init Global variables
  FileTypeTemp := NONE;
  gCancelPressed := False;
  gImageDirStr := pScreenD^.MainImg;
  //Make sure thay don't choose the same directory
  gOtherImageDirStr := pScreenD^.BackImg;
  gFileType := pScreenD^.MainFType;
  gSingleDir := LOAD;
  gDirRandomSort := SORTED_FILES_IN_DIR;
  gMainBack := BITMAP; // For folder if directory is picked NA
  gTileGraphic := False;
  gTurnOffImageModeSelections := True;

  //frmImageLoad := TfrmImageLoad.Create(Application);
  frmImageLoad.ShowModal;
  //frmImageLoad.Release;

  // They didn't do anything
  if (gImageDirStr = pScreenD^.MainImg) or gCancelPressed then exit;

  pScreenD^.MainImg := gImageDirStr;
  pScreenD^.MainFType := gFileType;
  Settings.MainImage := pScreenD^.MainImg;
  Settings.FileType := pScreenD^.MainFType;

  if not(gCancelPressed) and isPathValid(gImageDirStr) then begin

    if (ArtPalette <> 0) and ColorMode256 then begin
       DeleteObject(ArtPalette);
       ArtPalette := 0;
    end;

    FileTypeTemp := loadFile(Settings.MainImage,ArtBMP,ArtWMF,SHOW_ERROR_MSG);
    if FileTypeTemp > 0 then begin
       Settings.FileType := FileTypeTemp;
       pScreenD^.MainFType := FileTypeTemp;
       if (Settings.FileType > WMF) and ColorMode256 then begin

         {Show Window 256 color warning}
          StrPCopy(KeyName, '256 Color Check');
          StrPCopy(aDefault, '1');
          GetPrivateProfileString(APP_NAME, KeyName, aDefault, temp, SizeOf(temp), FILE_NAME);
          ShowAgain := Boolean(StrToInt(String(temp)));

          if ShowAgain then begin
            frmHColor := TfrmHColor.Create(Application);
            frmHColor.ShowModal;
            frmHColor.Release;
          end;

         if ArtBMP.PixelFormat = pf8bit then
           ArtPalette := ArtBMP.ReleasePalette;
       end;    
    end;
  end;

  {Paint the buffer with what the user last selected}
  if not(gCancelPressed) and  (FileTypeTemp > 0) then begin
    ActiveDoc := True;
    pScreenD^.MainResIndex := LOAD;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    ShowBox := False;
    UpdateScreen( pScreenD );
    PaintPreviewWnd;
  end;
end; // mnuLoadMainImageClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.edtTopKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
 pScreenD^.TopMsg := trim(edtTop.Text);
 Settings.TopMessage := pScreenD^.TopMsg;
 ActiveDoc := True;
 if Key = vk_Return then begin
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    ShowBox := False;
    PaintPreviewWnd;
 end;

end; // edtTopKeyUp

{-----------------------------------------------------------------------}

procedure TfrmDesigner.edtBottomKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
 pScreenD^.BottomMsg := trim(edtBottom.Text);
 Settings.BottomMessage := pScreenD^.BottomMsg;
 ActiveDoc := True;
 if Key = vk_Return then begin
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    ShowBox := False;
    PaintPreviewWnd;
 end;
end; // edtBottomKeyUp

{-----------------------------------------------------------------------}

procedure TfrmDesigner.btnFontsTopClick(Sender: TObject);
begin
     case (Sender as TSpeedButton).Tag of
          TOP_FONT_DLG, BOTTOM_FONT_DLG: begin

          FontDialog1.Font.Style := []; // Clear it out just in case

           if ((Sender as TSpeedButton).Tag) = TOP_FONT_DLG then begin
             FontDialog1.Font.Name := Settings.TopFontName;
             FontDialog1.Font.Size := Settings.TopFontSize;

             // Setup the font in dialog box
             if (Settings.TopFontStyleBold) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsBold];
             if (Settings.TopFontStyleItal) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsItalic];
             if (Settings.TopFontStyleUnder) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsUnderline];
             if (Settings.TopFontStyleStrike) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsStrikeout];
           end else begin
             FontDialog1.Font.Name := Settings.BottomFontName;
             FontDialog1.Font.Size := Settings.BottomFontSize;

             // Setup the font in dialog box
             if (Settings.BottomFontStyleBold) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsBold];
             if (Settings.BottomFontStyleItal) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsItalic];
             if (Settings.BottomFontStyleUnder) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsUnderline];
             if (Settings.BottomFontStyleStrike) then
               FontDialog1.Font.Style := FontDialog1.Font.Style + [fsStrikeout];
           end;

           if FontDialog1.Execute then
             ActiveDoc := True
           else
             exit;

           if ((Sender as TSpeedButton).Tag) = TOP_FONT_DLG then begin
             // Copy new font information  Top Message
             pScreenD^.TopFont := FontDialog1.Font.Name;
             pScreenD^.TopSize := FontDialog1.Font.Size;
             Settings.TopFontName := FontDialog1.Font.Name;
             Settings.TopFontSize := FontDialog1.Font.Size;
             edtTop.Font.Name := Settings.TopFontName;
             // Setup the font in dialog box
             edtTop.Font.Style := [];
             edtTop.Font.Style := FontDialog1.Font.Style;

             //Clear it all out
             Settings.TopFontStyleBold := False;
             Settings.TopFontStyleItal := False;
             Settings.TopFontStyleUnder := False;
             Settings.TopFontStyleStrike := False;

             // Copy over the settings
             if fsBold in FontDialog1.Font.Style then
               Settings.TopFontStyleBold := True;
             if fsItalic in FontDialog1.Font.Style then
               Settings.TopFontStyleItal := True;
             if fsUnderline in FontDialog1.Font.Style then
               Settings.TopFontStyleUnder := True;
             if fsStrikeout in FontDialog1.Font.Style then
               Settings.TopFontStyleStrike := True;

             // Copy font settings to structure
             pScreenD^.TopBold := Settings.TopFontStyleBold;
             pScreenD^.TopItal := Settings.TopFontStyleItal;
             pScreenD^.TopUnder := Settings.TopFontStyleUnder;
             pScreenD^.TopStrike := Settings.TopFontStyleStrike;
            end else begin
             // Copy new font information Bottom Message
             pScreenD^.BottomFont := FontDialog1.Font.Name;
             pScreenD^.BottomSize := FontDialog1.Font.Size;
             Settings.BottomFontName := FontDialog1.Font.Name;
             Settings.BottomFontSize := FontDialog1.Font.Size;
             edtBottom.Font.Name := Settings.BottomFontName;
             // Setup the font in dialog box
             edtBottom.Font.Style := [];
             edtBottom.Font.Style := FontDialog1.Font.Style;

             //Clear it all out
             Settings.BottomFontStyleBold := False;
             Settings.BottomFontStyleItal := False;
             Settings.BottomFontStyleUnder := False;
             Settings.BottomFontStyleStrike := False;

             if fsBold in FontDialog1.Font.Style then
               Settings.BottomFontStyleBold := True;
             if fsItalic in FontDialog1.Font.Style then
               Settings.BottomFontStyleItal := True;
             if fsUnderline in FontDialog1.Font.Style then
               Settings.BottomFontStyleUnder := True;
             if fsStrikeout in FontDialog1.Font.Style then
               Settings.BottomFontStyleStrike := True;

             // Copy font settings to structure
             pScreenD^.BottomBold := Settings.BottomFontStyleBold;
             pScreenD^.BottomItal := Settings.BottomFontStyleItal;
             pScreenD^.BottomUnder := Settings.BottomFontStyleUnder;
             pScreenD^.BottomStrike := Settings.BottomFontStyleStrike;
            end;
          end;
          TOP_FONT_COLOR_DLG, BOTTOM_FONT_COLOR_DLG: begin
             // Setup default color
             ColorDialog1.Color := clNone;

             // Load Custom Colors from ini settings
             ColorDialog1.CustomColors.Text := gCustomColors.Text;

             if ColorDialog1.Execute then begin
               ActiveDoc := True;
               gCustomColors.Text := ColorDialog1.CustomColors.Text;
               if (Sender as TSpeedButton).Tag = TOP_FONT_COLOR_DLG then begin
                 Settings.TopFontColor := ColorDialog1.Color;
                 pScreenD^.TopColor := ColorDialog1.Color;
               end else begin
                 Settings.BottomFontColor := ColorDialog1.Color;
                 pScreenD^.BottomColor := ColorDialog1.Color;
               end;
             end;

          end;
     end;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    ShowBox := False;
    PaintPreviewWnd;
    UpdateScreen( pScreenD );
end; // btnFontsTopClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.btnTopLeftClick(Sender: TObject);
begin
     {This sets the justification of the two text messages}
     case (Sender as TSpeedButton).Tag of
       0: Settings.TopTextPos := LEFT_JUST;
       1: Settings.TopTextPos := CENTER_TEXT;
       2: Settings.TopTextPos := RIGHT_JUST;
       3: Settings.BottomTextPos := LEFT_JUST;
       4: Settings.BottomTextPos := CENTER_TEXT;
       5: Settings.BottomTextPos := RIGHT_JUST;
     end;
     pScreenD^.TopTextPos := Settings.TopTextPos;
     pScreenD^.BottomTextPos := Settings.BottomTextPos;
     UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
     ActiveDoc := True;
     ShowBox := False;
     PaintPreviewWnd;
end; // btnTopLeftClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.cboDrawFillChange(Sender: TObject);
begin
  // Keeps it from causing an error (-1) when user
  // clicks it the wrong way
  if cboDrawFill.ItemIndex < 0 then exit;

  ActiveDoc := True;
  pScreenD^.TransFillColor := cboDrawFill.ItemIndex;
  Settings.TransFillColor := pScreenD^.TransFillColor;
end; // cboDrawFillChange

{-----------------------------------------------------------------------}

procedure TfrmDesigner.edtDelayKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  // Only let number keys through
  if (Byte(Key) = 8) and (Length(edtDelay.Text) > 0) then begin // Let backspace through
    pScreenD^.TransDelay := StrToInt(edtDelay.Text);
    Settings.TransDelay := pScreenD^.TransDelay;
    ActiveDoc := True;
    exit;
  end;

  // Use virtual key codes
  if (Shift = []) and (((Key >= 48) and (Key <= 57)) or ((Key >= 96) and (Key <= 105))) then begin
    pScreenD^.TransDelay := StrToInt(edtDelay.Text);
    Settings.TransDelay := pScreenD^.TransDelay;
    ActiveDoc := True;
  end;

end; // edtDelayKeyUp

{-----------------------------------------------------------------------}

procedure TfrmDesigner.btnNewClick(Sender: TObject);
begin
  // Don't let them add another unless they did something
  if not(ActiveDoc) and not(OpenedDoc) then
    exit;

  // Don't let them keep adding blank screens
  if IsThereABlankPage then exit;

  //OpenedDoc := False;
  // Erase the screen
  btnSound.Enabled := False;
  ShowBox := True;
  PaintPreviewWnd;
  SaveLastSelected;

  inc(TotalScr);
  CurrentScr := TotalScr;

  // Set the pointer back to the first element
  pScreenD := pScrBase;
  ReallocMem( pScreenD, SizeOf(TScreenDesign) * TotalScr );

  // Check for an error
  if pScreenD = nil then begin
    Application.MessageBox('There has been an error allocating memory', 'Error', mb_OK);
    freemem(pScrBase, TotalScr-1 * sizeOf(TScreenDesign));
    exit;
  end;

  if TotalScr > 1 then
    btnDelete.Enabled := True;

  pScrBase := pScreenD; // re-init base
  inc(pScreenD, TotalScr-1);
  ReSetToDefault( pScreenD );
  CopySettings( Settings, pScreenD );
  UpdateScreen( pScreenD );

  btnLeft.Enabled := True;
  mnuSwitchLeft.Enabled := True;
  btnRight.Enabled := False;
  mnuSwitchRight.Enabled := False;
  cboImages.ItemIndex := 0;
  cboBackgnd.ItemIndex := 0;
end; // btnNewClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.btnLeftClick(Sender: TObject);
begin
  if CurrentScr = 1 then exit;

  dec(CurrentScr);

  LoadScreen;
  {if CurrentScr = 1 then begin
    btnLeft.Enabled := False;
    mnuSwitchLeft.Enabled := False;
  end;

  btnRight.Enabled := True;
  mnuSwitchRight.Enabled := True;}
  {dec(pScreenD);

  if IsScreenBlank( pScreenD ) then begin
    ShowBox := True;
    //mnuFullScreenView.Enabled := False;
  end else begin
    ShowBox := False;
    //mnuFullScreenView.Enabled := True;
  end;

  CopySettings( Settings, pScreenD );
  //DeleteThePalettes;
  LoadMainImage( Settings, ArtBMP, ArtWMF, SHOW_ERROR_MSG );
  LoadBackImage( Settings, backBMP, backWMF, SHOW_ERROR_MSG );
  //GetThePalettes;
  UpdateScreen( pScreenD );
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  PaintPreviewWnd;}
end; // btnLeftClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.btnRightClick(Sender: TObject);
begin
  if CurrentScr = TotalScr then exit;

  inc(CurrentScr);
  LoadScreen;
  {if CurrentScr = TotalScr then begin
    btnRight.Enabled := False;
    mnuSwitchRight.Enabled := False;
  end;

  btnLeft.Enabled := True;
  mnuSwitchLeft.Enabled := True;}
  {inc(pScreenD);

  if IsScreenBlank( pScreenD ) then begin
    ShowBox := True;
    //mnuFullScreenView.Enabled := False;
  end else begin
    ShowBox := False;
    //mnuFullScreenView.Enabled := True;
  end;

  CopySettings( Settings, pScreenD );
  //DeleteThePalettes;
  LoadMainImage( Settings, ArtBMP, ArtWMF, SHOW_ERROR_MSG );
  LoadBackImage( Settings, backBMP, backWMF, SHOW_ERROR_MSG );
  //GetThePalettes;
  UpdateScreen( pScreenD );
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);

  PaintPreviewWnd;}
end; // btnRightClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuSoundClick(Sender: TObject);
var
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin
    {Get Show Window}
  StrPCopy(KeyName, 'Show Download Sound');
  StrPCopy(aDefault, '1');
  GetPrivateProfileString(APP_NAME, KeyName, aDefault, temp, SizeOf(temp), FILE_NAME);
  ShowAgain := Boolean(StrToInt(String(temp)));

  if ShowAgain then begin
    frmAddingSounds := TfrmAddingSounds.Create(Application);
    frmAddingSounds.ShowModal;
    frmAddingSounds.Release;
  end;

  gSoundsOnOff := False;
  gSoundFilePath := pScreenD^.SoundFile;
  gCancelPressed := False;

  // Show Window
  //frmSoundDir := TfrmSoundDir.Create(Application);
  frmSoundDir.ShowModal;
  //frmSoundDir.Release;

  // They didn't do anything
  if (gSoundFilePath = pScreenD^.SoundFile) or gCancelPressed then exit;

  Settings.SoundFileDir := gSoundFilePath;
  Settings.SoundDirOnOff := gSoundsOnOff;
  pScreenD^.SoundFile := gSoundFilePath;
  pScreenD^.SoundOnOff := gSoundsOnOff;
  ActiveDoc := True;

  if gSoundsOnOff then begin
    btnSound.Enabled := True;
    btnSound.ShowHint := True;
    btnSound.Hint := pScreenD^.SoundFile;
    mnuUnloadSound.Enabled := True;
  end;

end; // mnuSoundClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.btnSoundClick(Sender: TObject);
var tempStr: String;

begin
  tempStr := pScreenD^.SoundFile;

  if FileExists( tempStr ) then begin

    case frmMID.GetPlayerStatus of
      Playing: begin
        frmMID.ClosePlayer;
      end;

      Seeking, Paused, Open, Stopped, Closed: begin
        frmMID.Play( tempStr );
      end;
    end;

  end else
      Application.MessageBox( PChar('The Sound file ( ' + tempStr + ' ) is missing'), PChar('Error'), MB_ICONEXCLAMATION );

end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.SaveLastSelected;
begin
  LastSelected.SizeBitmaps := pScreenD^.SizeBitmaps;
  LastSelected.TopFont := pScreenD^.TopFont;
  LastSelected.TopSize := pScreenD^.TopSize;
  LastSelected.TopBold := pScreenD^.TopBold;
  LastSelected.TopItal := pScreenD^.TopItal;
  LastSelected.TopUnder := pScreenD^.TopUnder;
  LastSelected.TopStrike := pScreenD^.TopStrike;
  LastSelected.TopColor := pScreenD^.TopColor;
  LastSelected.TopTextPos := pScreenD^.TopTextPos;

  LastSelected.BottomFont := pScreenD^.BottomFont;
  LastSelected.BottomSize := pScreenD^.BottomSize;
  LastSelected.BottomBold := pScreenD^.BottomBold;
  LastSelected.BottomItal := pScreenD^.BottomItal;
  LastSelected.BottomUnder := pScreenD^.BottomUnder;
  LastSelected.BottomStrike := pScreenD^.BottomStrike;
  LastSelected.BottomColor := pScreenD^.BottomColor;
  LastSelected.BottomTextPos := pScreenD^.BottomTextPos;

  LastSelected.TransDelay := pScreenD^.TransDelay;
  LastSelected.TransFillColor := pScreenD^.TransFillColor;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.edtDelayKeyPress(Sender: TObject; var Key: Char);
begin
  // Only let number keys through
  if Byte(Key) = 8 then exit; // Let backspace through
  if (Byte(Key) < 48) or (Byte(Key) > 57) then
     Key := Char(0);
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuUnloadSoundClick(Sender: TObject);
begin
  pScreenD^.SoundFile := '';
  pScreenD^.SoundOnOff := False;
  ActiveDoc := True;
  btnSound.ShowHint := False;
  btnSound.Hint := '';
  mnuUnloadSound.Enabled := False;
  btnSound.Enabled := False;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.chkSizeBitmapMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  pScreenD^.SizeBitmaps := chkSizeBitmap.Checked;
  Settings.SizeBitmaps := chkSizeBitmap.Checked;
  ActiveDoc := True;
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  PaintPreviewWnd;
end; // chkSizeBitmapMouseUp

{-----------------------------------------------------------------------}


procedure TfrmDesigner.mnuSaveClick(Sender: TObject);
var
  SaverFile: File of TScreenDesign;
  I: Integer;
  pScreenDTmp: PTScreenDesign;
  FileNameStr: String;
begin
  // Get out if there is nothing to save
  if (TotalScr = 1) and not(ActiveDoc) then
    exit;

  // Check to see if they are saving the file with a blank page
  if IsThereABlankPage and (TotalScr > 1) then begin
     if Application.MessageBox('Are you sure you want to save? One or more of your screens are blank.',
    'Question', MB_YESNO + mb_DefButton1 ) = IDNO then
       exit;
  end;

  if FileLoaded = '' then begin
    SaveDialog1.FileName := '';
    SaveDialog1.InitialDir := stripOutDirectory( gScreenDesignPath );
    SaveDialog1.Title := 'Save Create-A-Saver File';
    SaveDialog1.Filter := 'Create-A-Saver (*.cas)|*.cas' +
    '|All files (*.*)|*.*';

    if not(SaveDialog1.Execute) then exit;

    FileLoaded := SaveDialog1.FileName;
  end;

  if Pos( '.', FileLoaded ) = 0 then
    FileLoaded := FileLoaded + '.cas';

  gScreenDesignPath := FileLoaded;
  pScreenDTmp :=  pScreenD;
  pScreenD := pScrBase;

  System.Assign(SaverFile, FileLoaded);
  ReWrite(SaverFile);
  for I := 1 to TotalScr do begin
    System.Write(SaverFile, pScreenD^);
    inc(pScreenD);
  end;

  System.Close(SaverFile);
  ActiveDoc := False;
  OpenedDoc := True;
  pScreenD :=  pScreenDTmp;

  // Show the Name of the file in the top window frame
  FileNameStr := stripOutFileName(FileLoaded);
  frmDesigner.Caption := 'Screen Designer - ' + FileNameStr;
end; // mnuSaveClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuOpenClick(Sender: TObject);
var
  FilePathTmp: string;
begin
  if ActiveDoc then begin
    mnuNewClick(Sender);

    // If they click no then ActiveDoc will be false
    if ActiveDoc then
      exit;
  end;

  OpenDialog1.FileName := '';
  OpenDialog1.InitialDir := stripOutDirectory( gScreenDesignPath );
  OpenDialog1.Title := 'Open Create-A-Saver File';
  OpenDialog1.Filter := 'Create-A-Saver (*.cas)|*.cas' +
  '|All files (*.*)|*.*';

  if not(OpenDialog1.Execute) then exit;

  FilePathTmp := gScreenDesignPath;
  gScreenDesignPath := OpenDialog1.FileName;
  if not(OpenScreenFile( OpenDialog1.FileName ) ) then
    gScreenDesignPath := FilePathTmp;

end; // mnuOpenClick

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuSaveAsClick(Sender: TObject);
var
  SaverFile: File of TScreenDesign;
  I: Integer;
  FileNameStr: string;
  pScreenDTmp: PTScreenDesign;

begin
  // Get out if there is nothing to save
  if (TotalScr = 1) and not(ActiveDoc) then
    exit;

  // Check to see if they are saving the file with a blank page
  if IsThereABlankPage and (TotalScr > 1) then begin
     if Application.MessageBox('Are you sure you want to save? One or more of your screens are blank.',
    'Question', MB_YESNO + mb_DefButton1 ) = IDNO then
       exit;
  end;


  SaveDialog1.InitialDir := stripOutDirectory( gScreenDesignPath );
  SaveDialog1.FileName := FileLoaded;
  SaveDialog1.Title := 'Save As';
  SaveDialog1.Filter := 'Create-A-Saver (*.cas)|*.cas' +
    '|All files (*.*)|*.*';
  SaveDialog1.Execute;

  if SaveDialog1.FileName = '' then exit;


  FileLoaded := SaveDialog1.FileName;

  if Pos( '.', FileLoaded ) = 0 then
    FileLoaded := FileLoaded + '.cas';

  gScreenDesignPath := FileLoaded;
  pScreenDTmp := pScreenD;
  pScreenD := pScrBase;

  System.Assign(SaverFile, FileLoaded);
  ReWrite(SaverFile);
  for I := 1 to TotalScr do begin
    System.Write(SaverFile, pScreenD^);
    inc(pScreenD);
  end;

  System.Close(SaverFile);
  ActiveDoc := False;
  OpenedDoc := True;
  pScreenD := pScreenDTmp;

  // Show the Name of the file in the top window frame
  FileNameStr := stripOutFileName(SaveDialog1.FileName);
  frmDesigner.Caption := 'Screen Designer - ' + FileNameStr;
end; // mnuSaveAsClick

{-----------------------------------------------------------------------}

//fix problem when doc is active and new is selected
procedure TfrmDesigner.mnuNewClick(Sender: TObject);
var
  responce: integer;
begin
  // Turn on the check box
  if not chkDesignerOnOff.Checked then begin
    chkDesignerOnOff.Checked := True;
    chkDesignerOnOffClick(Sender);
  end;

  // Get out if there is nothing to do
  if (TotalScr = 1) and not(ActiveDoc) and not(OpenedDoc) then begin
    ResetMemory(1);
    // reset the error strings
    Settings.MainFileLoadErrorStr := '';
    Settings.BackFileLoadErrorStr := '';
    exit;
  end;

  if ActiveDoc then begin
    responce := Application.MessageBox('Do you wish to save your changes?',
    'Question', MB_YESNOCANCEL + mb_DefButton1 );

    if responce = IDYES then
      mnuSaveClick(Sender);

    if responce = IDNO then
      ActiveDoc := False;

    if not(responce = IDCANCEL) and not(ActiveDoc) then begin
       // reset the error strings
       Settings.MainFileLoadErrorStr := '';
       Settings.BackFileLoadErrorStr := '';
      if not(ResetMemory(1)) then exit;
    end;
  end;

  if OpenedDoc and not(ActiveDoc) then begin
     // reset the error strings
     Settings.MainFileLoadErrorStr := '';
     Settings.BackFileLoadErrorStr := '';
     if not(ResetMemory(1)) then exit;
  end;

end; // mnuNewClick & Close

{-----------------------------------------------------------------------}

procedure TfrmDesigner.DoWeNeedToSave;
var
  responce: integer;
begin

  // Get out if there is nothing to do
  if (TotalScr = 1) and not(ActiveDoc) and not(OpenedDoc) then
    exit;

  if ActiveDoc then begin
    responce := Application.MessageBox('Do you wish to save your changes?',
    'Question', MB_YESNOCANCEL + mb_DefButton1 );

    if responce = IDYES then
      mnuSaveClick(nil);

    if responce = IDNO then
      ActiveDoc := False;
  end;
end;

{-----------------------------------------------------------------------}


procedure TfrmDesigner.btnDeleteClick(Sender: TObject);
var
  pScreenDTmp: PTScreenDesign;
begin

  // See if they want to delete the screen
  if not(IsScreenBlank( pScreenD )) then begin
    if Application.MessageBox('Are you sure you want to delete this screen?',
      'Delete Screen?', MB_YESNO ) = IDNO then
      exit;
  end;

  // If CurrentScr = TotalScr, it gets chopped off when the memory
  // gets reallocated
  if TotalScr > 1 then begin
     ActiveDoc := True;

     // Init pointer to base
     pScreenD := pScrBase;
     pScreenDTmp := pScreenD;

    if CurrentScr < TotalScr then begin
       Inc(pScreenD, CurrentScr - 1);
       Inc(pScreenDTmp, CurrentScr);
       CopyMemory( pScreenD, pScreenDTmp, SizeOf(TScreenDesign) * (TotalScr - CurrentScr));

       // Set the pointer back to the first element
       pScreenD := pScrBase;
    end;

    // The tail end gets chopped off
    ReallocMem( pScreenD, SizeOf(TScreenDesign) * (TotalScr - 1) );

    if pScreenD = nil then begin
       Application.MessageBox('There has been a memory reallocating error trying to delete the current screen. Can not delete screen.', 'Error', mb_OK);
       exit;
    end;

    // Re-init the base
    pScrBase := pScreenD;
  end;

  // Let it go through twice when only one screen left
  // This way it will clear the screen
  if TotalScr > 1 then begin
    Dec(TotalScr);
    if CurrentScr > TotalScr then
       CurrentScr := TotalScr;

    Inc( pScreenD, CurrentScr - 1 );

    if IsScreenBlank( pScreenD ) then
      ShowBox := True
    else begin
      ShowBox := False;

      CopySettings( Settings, pScreenD );
      //DeleteThePalettes;
      LoadMainImage( Settings, ArtBMP, ArtWMF, SHOW_ERROR_MSG );
      LoadBackImage( Settings, backBMP, backWMF, SHOW_ERROR_MSG );
      //GetThePalettes;
      UpdateScreen( pScreenD );
      UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    end;
  end else begin
    btnDelete.Enabled := False;
    ActiveDoc := False;
    OpenedDoc := False;
    ShowBox := True;
    ReSetToDefault( pScreenD );
    CopySettings( Settings, pScreenD );
    UpdateScreen( pScreenD );
  end;


  if CurrentScr > 1 then begin
    btnLeft.Enabled := True;
    mnuSwitchLeft.Enabled := True;
  end else begin
    btnLeft.Enabled := False;
    mnuSwitchLeft.Enabled := False;
  end;

  if CurrentScr < TotalScr then begin
    mnuSwitchRight.Enabled := True;
    btnRight.Enabled := True;
  end else begin
    btnRight.Enabled := False;
    mnuSwitchRight.Enabled := False;
  end;

  PaintPreviewWnd;
end; // btnDeleteClick

{-----------------------------------------------------------------------}

function TfrmDesigner.IsScreenBlank( pS: PTScreenDesign ): Boolean;
begin

  result := True;

  if not(pS^.MainImg = '') then
    result := False;

  if not(pS^.MainFType = NONE) then
    result := False;

  if not(pS^.TopMsg = '') then
    result := False;

  if not(pS^.BackImg = '') then
    result := False;

  if not(pS^.BackFType = NONE) then
    result := False;

  if not(pS^.BkFillColor = LongWord(clWhite)) then
    result := False;

  if not(pS^.BottomMsg = '') then
    result := False;

  if not(pS^.SoundFile = '') then
    result := False;
end;

{-----------------------------------------------------------------------}

function TfrmDesigner.IsThereABlankPage: Boolean;
var I: Integer;
    pScreenDTmp: PTScreenDesign;
begin
  result := False;

  // Init pointer to base
  pScreenDTmp := pScreenD;
  pScreenD := pScrBase;

  for I := 0 to TotalScr-1 do begin
    if IsScreenBlank( pScreenD ) then begin
      result := True;
      pScreenD := pScreenDTmp;
      exit;
    end;
    Inc( pScreenD );
  end;

  // Init pointer to base
  pScreenD := pScreenDTmp;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.cboImagesChange(Sender: TObject);
begin
     // Keeps it from causing an error (-1) when user
     // clicks it the wrong way
     if cboImages.ItemIndex < 0 then exit;

     case cboImages.ItemIndex of
       NONE: begin
          pScreenD^.MainImg := '';
          pScreenD^.MainFType := NONE;
          Settings.FileType := NONE;
          Settings.MainImage := '';
          UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
          PaintPreviewWnd;
          end;
       LOAD: mnuLoadMainImageClick( Sender );
       //else if not(gCancelPressed) then begin
       else begin
          if not loadWFMresource(pchar(resList.Strings[cboImages.ItemIndex]), artWMF) then begin
            Settings.FileType := NONE;
            pScreenD^.MainFType := NONE;
            ShowBox := False;
            exit;
          end;
          pScreenD^.MainImg := resList.Strings[cboImages.ItemIndex];
          pScreenD^.MainFType := RESOURCE;
          Settings.FileType := RESOURCE;
          Settings.MainImage := resList.Strings[cboImages.ItemIndex];
          ShowBox := False;
          ActiveDoc := True;
          UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
          PaintPreviewWnd;
       end;
     end;

  // Save the index
  pScreenD^.MainResIndex := cboImages.ItemIndex;
  UpdateScreen( pScreenD );
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.cboBackgndChange(Sender: TObject);
begin
     // Keeps it from causing an error (-1) when user
     // clicks it the wrong way
     if cboBackgnd.ItemIndex < 0 then exit;

     case cboBackgnd.ItemIndex of
       NONE: begin
          pScreenD^.BackImg := '';
          pScreenD^.BackFType := NONE;
          Settings.BKImage := '';
          Settings.BkFileType := NONE;
          UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
          PaintPreviewWnd;
          end;
       LOAD:
         mnuBackImageClick(Sender);
       COLORS: begin
         ColorDialog1.CustomColors.Text := gCustomColors.Text;
         ColorDialog1.Color := clNone; // Clear out the color box
         ColorDialog1.Execute; // Show color box
         // Save Custom Colors to INI file
         if ColorDialog1.Color <> clNone then begin
           gCustomColors.Text := ColorDialog1.CustomColors.Text;
           Settings.BKFillColor := ColorDialog1.Color;
           Settings.BkFileType := COLORS;
           pScreenD^.BkFillColor := ColorDialog1.Color;
           pScreenD^.BackFType := COLORS;
           ShowBox := False;
           ActiveDoc := True;
           UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
           PaintPreviewWnd;
         end;
         end;
       else begin
         {Load and decode gif from resource}
         if (BackPalette <> 0) and ColorMode256 then begin
           DeleteObject(BackPalette);
           BackPalette := 0;
         end;

         GifLoadFromResource( resBack.Strings[cboBackgnd.ItemIndex], backBMP );

         if ColorMode256 then
           if backBMP.PixelFormat = pf8bit then
             BackPalette := backBMP.ReleasePalette;

         Settings.BKImage := resBack.Strings[cboBackgnd.ItemIndex];
         Settings.BkFileType := RESOURCE;
         pScreenD^.BackImg := Settings.BKImage;
         pScreenD^.BackFType := RESOURCE;
         ShowBox := False;
         ActiveDoc := True;
         UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
         PaintPreviewWnd;
       end;
     end;

  // Save the index
  pScreenD^.BackResIndex := cboBackgnd.ItemIndex;
  UpdateScreen( pScreenD );
end;

{-----------------------------------------------------------------------}

function TfrmDesigner.ResetMemory( I: Integer ):Boolean;
begin
  result := True;

  btnLeft.Enabled := False;
  btnRight.Enabled := False;
  mnuSwitchLeft.Enabled := False;
  mnuSwitchRight.Enabled := False;
  btnDelete.Enabled := False;
  pScreenD := pScrBase;

  if TotalScr > 0 then
     freemem(pScreenD, TotalScr * sizeOf(TScreenDesign));

  ShowBox := True;
  ActiveDoc := False;
  FileLoaded := '';
  OpenedDoc := False;

  CurrentScr := I;
  TotalScr := I;
  pScreenD := nil;
  if I > 0 then
    pScreenD := allocmem( SizeOf(TScreenDesign) );

  // Check for an error
  if (pScreenD = nil) and (I > 0) then begin
     Application.MessageBox('There has been an error allocating memory', 'Error', mb_OK);
     result := False;
     exit;
  end;

  if I > 0 then begin
   pScrBase := pScreenD; // Init the reference point
   ReSetToDefault( pScreenD );
   CopySettings( Settings, pScreenD );
   if gScreenDesignOnOff then
     EnableDisableControls( True )
   else
     EnableDisableControls( False );
  end;

  UpdateScreen( pScreenD );
  PaintPreviewWnd;
  if visible then
    frmDesigner.Caption := 'Screen Designer';

end; // ResetMemory

{-----------------------------------------------------------------------}

function TfrmDesigner.OpenScreenFile( FileNameStr: String ):Boolean;
var
  SaverFile: File of TScreenDesign;
  ErrorMsg, FileNameTmp: String;
begin

  result := False;
  pScreenD := pScrBase;

  if TotalScr > 0 then
     freemem(pScreenD, TotalScr * sizeOf(TScreenDesign));

  TotalScr := 0;
  pScreenD := nil;
  pScrBase := nil;
  ErrorMsg := '';
  btnLeft.Enabled := False;
  mnuSwitchLeft.Enabled := False;
  btnRight.Enabled := False;
  mnuSwitchRight.Enabled := False;
  btnDelete.Enabled := False;

  try
    System.Assign(SaverFile, FileNameStr);
    Reset(SaverFile);

    while not EOF(SaverFile) do begin
      pScreenD := pScrBase;
      inc(TotalScr);
      ReallocMem( pScreenD, SizeOf(TScreenDesign) * TotalScr );

      // Check for memory errors
      if pScreenD = nil then begin
        Application.MessageBox('There has been an error allocating memory.', 'Error', mb_OK);
        freemem(pScrBase, TotalScr-1 * sizeOf(TScreenDesign));
        break;
      end;

      pScrBase := pScreenD; // re-init base
      inc(pScreenD, TotalScr-1);

      System.Read(SaverFile, pScreenD^);
    end;
  except
      System.Close(SaverFile);
      ErrorMsg := 'There has been an error loading the file ('+ FileNameStr + ').';
      Application.MessageBox(PChar(ErrorMsg), 'Error', mb_OK);
      exit;
  end;

  System.Close(SaverFile);

  if TotalScr >= 1 then begin
    btnDelete.Enabled := True;
    CurrentScr := 1;
    //btnRight.Enabled := True;
  end;

  chkDesignerOnOff.Checked := TRUE;
  pScreenD := pScrBase; // re-init base
  ShowBox := False;

  CopySettings( Settings, pScreenD );
  //DeleteThePalettes;
  LoadMainImage( Settings, ArtBMP, ArtWMF, SHOW_ERROR_MSG );
  LoadBackImage( Settings, backBMP, backWMF, SHOW_ERROR_MSG );
  //GetThePalettes;
  UpdateScreen( pScreenD );
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  PaintPreviewWnd;
  OpenedDoc := True;

  // Show the Name of the file in the top window frame
  FileNameTmp := stripOutFileName(FileNameStr);
  if visible then // Error will cause if form is not visible
    frmDesigner.Caption := 'Screen Designer - ' + FileNameTmp;
  ActiveDoc := False;
  FileLoaded := FileNameStr;

  result := True;

end; // OpenScreenFile

{-----------------------------------------------------------------------}

procedure TfrmDesigner.FormShow(Sender: TObject);
var FileNameTmp: string;
begin

  // Show the info box
  Timer1.Enabled := True;

  // Turn on or off all the controls
  if gScreenDesignOnOff then begin
    if FileExists(gScreenDesignPath) then begin
      if( not(OpenScreenFile( gScreenDesignPath )) ) then
        gScreenDesignOnOff := False;
    end else
      gScreenDesignOnOff := False;
  end;

  if gScreenDesignOnOff then begin
    chkDesignerOnOff.Checked := True;
    chkDesignerOnOff.Caption := 'Screen Designer On:';
    EnableDisableControls( True );
  end else begin
    chkDesignerOnOff.Checked := False;
    chkDesignerOnOff.Caption := 'Screen Designer Off:';
    EnableDisableControls( False );
  end;

  if not(gScreenDesignOnOff) then
    if not(ResetMemory(1)) then exit;

  if not(gScreenDesignOnOff) then begin
    ReSetToDefault( pScreenD );
    CopySettings( Settings, pScreenD );
    UpdateScreen( pScreenD );
  end;

  if gScreenDesignRandom = SCREEN_DESIGNER_RANDOM then
    mnuRandom.Checked := True
  else
    mnuRandom.Checked := False;

  mnuSoundsOnOff.Checked := gSoundSDonOff;


  if FileLoaded <> '' then begin
    // Show the Name of the file in the top window frame
    FileNameTmp := stripOutFileName(FileLoaded);
    frmDesigner.Caption := 'Screen Designer - ' + FileNameTmp;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.pntPreviewClick(Sender: TObject);
begin
  if ActiveDoc then begin
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuSwitchLeftClick(Sender: TObject);
var temp, pScreenDTmp: PTScreenDesign;
begin
  ActiveDoc := True;

  pScreenDTmp := pScreenD;

  if (Sender as TMenuItem).Tag = 1 then
    Inc(pScreenDTmp)
  else
    Dec(pScreenDTmp);

  if IsScreenBlank(pScreenD) or IsScreenBlank(pScreenDTmp) then
    exit;

  temp := allocmem(SizeOf(TScreenDesign));
  if temp = nil then begin
    Application.MessageBox('Memory allocation error was caused.',
           'There has been an error.', mb_OK + MB_ICONEXCLAMATION);
    exit;
  end;

  CopyMemory( temp, pScreenDTmp, SizeOf(TScreenDesign));
  CopyMemory( pScreenDTmp, pScreenD, SizeOf(TScreenDesign));
  CopyMemory( pScreenD, temp, SizeOf(TScreenDesign));

  FreeMem(temp, SizeOf(TScreenDesign));

  CopySettings( Settings, pScreenD );
  //DeleteThePalettes;
  LoadMainImage( Settings, ArtBMP, ArtWMF, SHOW_ERROR_MSG );
  LoadBackImage( Settings, backBMP, backWMF, SHOW_ERROR_MSG );
  //GetThePalettes;
  UpdateScreen( pScreenD );
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  PaintPreviewWnd;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuRandomClick(Sender: TObject);
begin
  if gScreenDesignRandom = SCREEN_DESIGNER_RANDOM then begin
    mnuRandom.Checked := False;
    gScreenDesignRandom := SCREEN_DESIGNER_ORDER;
  end else begin
    mnuRandom.Checked := True;
    gScreenDesignRandom := SCREEN_DESIGNER_RANDOM;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.FormPaint(Sender: TObject);
begin
  Canvas.Pen.Width := 1;
  Canvas.Pen.Color := clBtnShadow;
  Canvas.MoveTo(0, 0);
  Canvas.LineTo(frmDesigner.ClientWidth, 0);
  Canvas.Pen.Color := clBtnHighlight;
  Canvas.MoveTo(0, 1);
  Canvas.LineTo(frmDesigner.ClientWidth, 1);
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.mnuFullScreenViewClick(Sender: TObject);
begin
   // Show Window
   gSettings := Settings;
   frmFullScreenView := TfrmFullScreenView.Create(Application);
   frmFullScreenView.ShowModal;
   frmFullScreenView.Release;
end;

{-----------------------------------------------------------------------}

procedure TfrmDesigner.chkDesignerOnOffMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
   PaintPreviewWnd;
end;

{-----------------------------------------------------------------------}

{procedure TfrmDesigner.GetThePalettes;
begin

  if (Settings.FileType > WMF) and ColorMode256 then
    if ArtBMP.PixelFormat = pf8bit then
      ArtPalette := ArtBMP.ReleasePalette;

  if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
    if backBMP.PixelFormat = pf8bit then
      BackPalette := backBMP.ReleasePalette;
end;}

{-----------------------------------------------------------------------}

{procedure TfrmDesigner.DeleteThePalettes;
begin

  if (ArtPalette <> 0) and ColorMode256 then begin
    DeleteObject(ArtPalette);
    ArtPalette := 0;
  end;

  if (BackPalette <> 0) and ColorMode256 then begin
    DeleteObject(BackPalette);
    BackPalette := 0;
  end;

end;}

procedure TfrmDesigner.Timer1Timer(Sender: TObject);
var
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin
  Timer1.Enabled := False;

    {Get Show Window}
  StrPCopy(KeyName, 'Show Screen Designer Message');
  StrPCopy(aDefault, '1');
  GetPrivateProfileString(APP_NAME, KeyName, aDefault, temp, SizeOf(temp), FILE_NAME);
  ShowAgain := Boolean(StrToInt(String(temp)));

  if ShowAgain then begin
    frmUsingSD := TfrmUsingSD.Create(Application);
    frmUsingSD.ShowModal;
    frmUsingSD.Release;
  end;
end;

procedure TfrmDesigner.mnuContentsClick(Sender: TObject);
begin
  LoadHelpFile;
end;

procedure TfrmDesigner.mnuSoundsOnOffClick(Sender: TObject);
begin
  if mnuSoundsOnOff.Checked then begin
    gSoundSDonOff := False;
    mnuSoundsOnOff.Checked := gSoundSDonOff;
  end else begin
    gSoundSDonOff := True;
    mnuSoundsOnOff.Checked := gSoundSDonOff;
  end;  
end;

procedure TfrmDesigner.mnuMoveClick(Sender: TObject);
var
  pScreenDTmp, pScreenDTmpBase, pScreenPt, NewScreenPos: PTScreenDesign;
  I: Integer;
begin
  gTotalScr := TotalScr;
  gCurrentScr := CurrentScr;

  frmMove := TfrmMove.Create(Application);
  frmMove.ShowModal;
  frmMove.Release;

  // They picked the came screen or canceled
  // gCurrentScr holdes the new screen they want to use
  if gCurrentScr = CurrentScr then exit;

  // See if they have any blank screens
  if IsThereABlankPage then begin
    Application.MessageBox('One of your screens are blank. You will need to use or delete it.',
      'Blank Screen', mb_OK + MB_ICONEXCLAMATION );
      exit;
  end;

  // Alocate a temporary memory block
  pScreenDTmpBase := AllocMem( SizeOf(TScreenDesign) * TotalScr );

  // Check for memory errors
  if pScreenDTmpBase = nil then begin
    Application.MessageBox('There has been an error allocating memory.', 'Error', mb_OK);
    exit;
  end;

  ActiveDoc := True;
  pScreenDTmp := pScreenDTmpBase;
  pScreenPt := pScrBase;
  NewScreenPos := pScrBase;
  inc( NewScreenPos, CurrentScr - 1 );

  // gCurrentScr = new screen location
  // CurrentScr = old screen location

  for I := 1 to TotalScr do begin
    if (I <> gCurrentScr) and (I <> CurrentScr) then
      CopyMemory( pScreenDTmp, pScreenPt, SizeOf(TScreenDesign) );

    if I = CurrentScr then begin
      if gCurrentScr > CurrentScr then
        inc(pScreenPt);
        
      CopyMemory( pScreenDTmp, pScreenPt, SizeOf(TScreenDesign) );
      inc(pScreenPt);

      if gCurrentScr < CurrentScr then
        inc(pScreenPt);
      inc(pScreenDTmp);
    end;

    // Copy the chosen screen to it's new location
    if I = gCurrentScr then begin
      CopyMemory( pScreenDTmp, NewScreenPos, SizeOf(TScreenDesign) );
      inc(pScreenDTmp);
    end;

    if (I <> gCurrentScr) and (I <> CurrentScr) then begin
      inc(pScreenPt);
      inc(pScreenDTmp);
    end;  
  end;

  // We've copyied over all the memory. Now transfer it to the main block
  CopyMemory( pScrBase, pScreenDTmpBase, SizeOf(TScreenDesign) * TotalScr );

  // Now free the temporary memory
  freeMem( pScreenDTmpBase, SizeOf(TScreenDesign) * TotalScr );

  pScreenD := pScrBase;
  inc( pScreenD, gCurrentScr-1 );
  CurrentScr := gCurrentScr;

  CopySettings( Settings, pScreenD );
  UpdateScreen( pScreenD );

end;

procedure TfrmDesigner.edtCurrentKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if ((Byte(Key) = 8) and (edtCurrent.Text = '')) then exit; // Let backspace through
  if ((key > 47) and (Key < 58)) or (Key = 8) then begin //Get out if they are hitting letters
    if (StrToInt(edtCurrent.Text) >= 1) and (StrToInt(edtCurrent.Text) <= TotalScr) then begin
      CurrentScr := StrToInt(edtCurrent.Text);
      LoadScreen;
    end;
  end;
end;

procedure TfrmDesigner.LoadScreen;
begin
  pScreenD := pScrBase;

  inc(pScreenD, CurrentScr-1);

  if IsScreenBlank( pScreenD ) then
    ShowBox := True
  else
    ShowBox := False;

  CopySettings( Settings, pScreenD );
  //DeleteThePalettes;
  LoadMainImage( Settings, ArtBMP, ArtWMF, SHOW_ERROR_MSG );
  LoadBackImage( Settings, backBMP, backWMF, SHOW_ERROR_MSG );
  //GetThePalettes;
  UpdateScreen( pScreenD );
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  PaintPreviewWnd;
end;

procedure TfrmDesigner.edtCurrentKeyPress(Sender: TObject; var Key: Char);
begin
  // Only let number keys through
  if Byte(Key) = 8 then exit; // Let backspace through
  if (Byte(Key) < 48) or (Byte(Key) > 57) then
     Key := Char(0);
end;

procedure TfrmDesigner.FormDestroy(Sender: TObject);
begin
  Settings.free;
  resList.free;
  resBack.free;
  //DeleteThePalettes;
  pScreenD := pScrBase;
  if TotalScr > 0 then
     freemem(pScreenD, TotalScr * sizeOf(TScreenDesign));
end;

end.
