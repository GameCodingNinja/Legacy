unit savConf;
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs, ImgLoad,
  StdCtrls, ComCtrls, savInit, ExtCtrls, Globals, Loadpref, gFunctin, PlaySnd,
  Buttons, constants, password, transitions, MainForm, FileCtrl,
  loadtxt, designer, TwoWays, gOrderfm, hColor, SoundWeb;

type
  TfrmConfig = class(TForm)
    butOK: TButton;
    butCancel: TButton;
    imgSettings: TImage;
    FontDialog1: TFontDialog;
    ColorDialog1: TColorDialog;
    cboImages: TComboBox;
    Label1: TLabel;
    Label2: TLabel;
    btnFontsTop: TSpeedButton;
    btnColorsTop: TSpeedButton;
    btnFontsBottom: TSpeedButton;
    btnColorsBottom: TSpeedButton;
    edtTop: TEdit;
    edtBottom: TEdit;
    cboBackgnd: TComboBox;
    Label3: TLabel;
    pntImage: TPaintBox;
    pntBackgrnd: TPaintBox;
    btnTransitions: TSpeedButton;
    btnTopLeft: TSpeedButton;
    btnTopCenter: TSpeedButton;
    btnTopRight: TSpeedButton;
    btnBottomLeft: TSpeedButton;
    btnBottomCenter: TSpeedButton;
    btnBottomRight: TSpeedButton;
    pntPreview: TPaintBox;
    Label4: TLabel;
    chkPassword: TCheckBox;
    Label5: TLabel;
    lblRegNumber: TLabel;
    edtRegistrationNo: TEdit;
    btnOrderForm: TButton;
    btnTopTxt: TSpeedButton;
    btnBottomTxt: TSpeedButton;
    btnSound: TSpeedButton;
    btnDesigner: TBitBtn;
    btnHelp: TSpeedButton;
    OpenDialog1: TOpenDialog;
    GTimer: TTimer;
    Timer1: TTimer;
    procedure butOKClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure butCancelClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnFontsTopClick(Sender: TObject);
    procedure cboImagesChange(Sender: TObject);
    procedure pntImagePaint(Sender: TObject);
    procedure cboBackgndChange(Sender: TObject);
    procedure pntBackgrndPaint(Sender: TObject);
    procedure btnTopLeftClick(Sender: TObject);
    procedure pntPreviewPaint(Sender: TObject);
    procedure pntPreviewClick(Sender: TObject);
    procedure chkPasswordClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure btnTransitionsClick(Sender: TObject);
    procedure btnOrderFormClick(Sender: TObject);
    //procedure AdjustWindowPalette(canvas: TCanvas);
    procedure btnTopTxtClick(Sender: TObject);
    procedure btnSoundClick(Sender: TObject);
    procedure btnHelpClick(Sender: TObject);
    procedure btnDesignerClick(Sender: TObject);
    procedure GTimerTimer(Sender: TObject);
    procedure edtTopKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure edtBottomKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
    RegisteredUser: Boolean;
  public
    { Public declarations }
    resList, resBack: TStringList;
    Settings: TScrnSavSettings;
    ColorMode256, btnBlink, ScreenDsnExit: Boolean;
    procedure Initialize;
    procedure PaintPreviewWnd;
    procedure PaintImageWnd;
    procedure PaintBackgrndWnd;
    procedure EnableDisableWnd( I: Boolean );
    procedure BlinkButton;
    procedure LoadTheImageFiles;
    //function CreateTmpRegFile(FileName: String; I: Integer): Boolean;
    {it is very important that you leave this CreateParams intact, it is
     essential to making this window modal to the screen-properties-dialog}
    procedure CreateParams (var Params : TCreateParams); override;
  end;

var
  frmConfig: TfrmConfig;

implementation

uses OrderOption;

{$R *.DFM}

{-----------------------------------------------------------------------}

procedure TfrmConfig.butCancelClick(Sender: TObject);
begin
  close; {close and exit without saving conig}
end; // butCancelClick

{-----------------------------------------------------------------------}

{Click on OK saves my configuration to the ini file.}
procedure TfrmConfig.butOKClick(Sender: TObject);
begin
  if not RegisteredUser then
    Settings.Registration := ValidateRegNo( edtRegistrationNo.text );
  // Save changes to edit controls
  Settings.TopMessage := edtTop.Text;
  Settings.BottomMessage := edtBottom.Text;
  Settings.SaveINISettings; {Write the new settings to the INI file.}
  close;
end; // butOKClick

{-----------------------------------------------------------------------}

procedure TfrmConfig.CreateParams(var Params : TCreateParams);
begin {here comes the tricky part}
  inherited Createparams (Params);
  if ConfigParent <> 0 then begin
    {this is necessary to make myself a child window of the screen-properties-dialog,
     the effect is, when I click on my config-dialog, the screen-properties-dialog shows
     up behind my dialog}
    Params.WndParent := ConfigParent;
    EnableWindow (ConfigParent, false); {My dialog should be modal, so disable parent}
  end;
end; // CreateParams

{-----------------------------------------------------------------------}

procedure TfrmConfig.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  if ConfigParent <> 0 then begin
    EnableWindow (ConfigParent, true); {as I made myself modal, I should undo so now}
    SetForegroundWindow (ConfigParent);
  end;
  // Clean-up
  Settings.free;
  gCustomColors.free;
  resList.free;
  resBack.free;
  artWMF.free;
  backWMF.free;
  artBMP.free;
  backBMP.free;
  buffer.free;
  {if (ArtPalette <> 0) and ColorMode256 then begin
      DeleteObject(ArtPalette);
    ArtPalette := 0;
  end;

  if (BackPalette <> 0) and ColorMode256 then begin
    DeleteObject(BackPalette);
    BackPalette := 0;
  end;}

end; // FormClose

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnFontsTopClick(Sender: TObject);
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

           FontDialog1.Execute;

           if ((Sender as TSpeedButton).Tag) = TOP_FONT_DLG then begin
             // Copy new font information  Top Message
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

             if fsBold in FontDialog1.Font.Style then
               Settings.TopFontStyleBold := True;
             if fsItalic in FontDialog1.Font.Style then
               Settings.TopFontStyleItal := True;
             if fsUnderline in FontDialog1.Font.Style then
               Settings.TopFontStyleUnder := True;
             if fsStrikeout in FontDialog1.Font.Style then
               Settings.TopFontStyleStrike := True;
            end else begin
             // Copy new font information Bottom Message
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
            end;
          end;
          TOP_FONT_COLOR_DLG, BOTTOM_FONT_COLOR_DLG: begin
             // Setup default color
             ColorDialog1.Color := clNone;

             // Load Custom Colors from ini settings
             ColorDialog1.CustomColors.Text := Settings.CustomColors.Text;

             if ColorDialog1.Execute then begin
               Settings.CustomColors.Text := ColorDialog1.CustomColors.Text;
               if (Sender as TSpeedButton).Tag = TOP_FONT_COLOR_DLG then begin
                 Settings.TopFontColor := ColorDialog1.Color;
               end else begin
                 Settings.BottomFontColor := ColorDialog1.Color;
               end;
             end;

          end;
     end;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;     
end; // btnFontsTopClick

{-----------------------------------------------------------------------}

procedure TfrmConfig.FormCreate(Sender: TObject);
begin

  {Lets get the preferences now just to be safe}
  Settings := TScrnSavSettings.Create;
  Settings.GetINISettings;
  ScreenDsnExit := False; // For checking later

  DeleteFile(GetTheWindowsDirectory + HELP_FILE_NAME);
  DeleteFile(GetTheWindowsDirectory + HELP_FILE_JUNK);

  ArtPalette := 0;
  BackPalette := 0;

   // Check that the path is valid
   if Settings.ScreenDesignOnOff then begin
     if FileExists(Settings.ScreenDesignPath) then begin
       EnableDisableWnd( False );
       GTimer.Enabled := True;
     end else begin
       Settings.ScreenDesignOnOff := False;
       Settings.ScreenDesignPath := '';
     end;
   end;

  ColorMode256 := False;
  if GetDeviceCaps( canvas.handle, BITSPIXEL ) <= 8 then
    ColorMode256 := True;

  // To transfer custom colos to design window
  gCustomColors := TStringList.Create;

  Initialize; // Load it all up

end; // FormCreate

{-----------------------------------------------------------------------}

procedure TfrmConfig.Initialize;
var  I: Integer;
     t, res: PChar;
     size: longInt;
begin

  // check to see if they registered
  if Settings.Registration = REGISTRATION_VALIDATION then
    RegisteredUser := True
  else
    RegisteredUser := False;

  // Add more here when need be
  if RegisteredUser then begin
    edtRegistrationNo.Enabled := False;
    lblRegNumber.Enabled := False;
    edtRegistrationNo.Text := '  You Are Registered';
    edtRegistrationNo.Color := clBtnFace;
    edtRegistrationNo.Font.Color := clBtnHighlight;
    frmConfig.Caption := FULLVERSION_TITLE;
  end else begin
    frmConfig.Caption := SHAREWARE_TITLE;
  end;

  {Load and decode gif from resource}
  GifLoadFromResource( 'ScrnSettings', imgSettings.Picture.Bitmap );

  
  artWMF := TMetafile.Create;
  backWMF := TMetafile.Create;
  artBMP := TBitmap.Create;
  backBMP := TBitmap.Create;
  resList := TStringList.Create;
  resBack := TStringList.Create;

  {Lets Make the Buffer}
  buffer := TBitmap.Create;
  buffer.Width := Screen.Width;
  buffer.Height := Screen.Height;

  //artBMP.IgnorePalette := True;
  //backBMP.IgnorePalette := True;
  //buffer.IgnorePalette := True;

  // Set the justification buttons
  case Settings.TopTextPos of
     LEFT_JUST: btnTopLeft.Down := True;
     CENTER_TEXT: btnTopCenter.Down := True;
     RIGHT_JUST: btnTopRight.Down := True;
  end;

  case Settings.BottomTextPos of
     LEFT_JUST: btnBottomLeft.Down := True;
     CENTER_TEXT: btnBottomCenter.Down := True;
     RIGHT_JUST: btnBottomRight.Down := True;
  end;

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

  // Add Text to Top Message and all it's settings
  edtTop.Text := Settings.TopMessage;
  edtTop.Font.Name := Settings.TopFontName;

  // Setup the font
  if (Settings.TopFontStyleBold) then
     edtTop.Font.Style := [fsBold];
  if (Settings.TopFontStyleItal) then
     edtTop.Font.Style := edtTop.Font.Style + [fsItalic];
  if (Settings.TopFontStyleUnder) then
     edtTop.Font.Style := edtTop.Font.Style + [fsUnderline];
  if (Settings.TopFontStyleStrike) then
     edtTop.Font.Style := edtTop.Font.Style + [fsStrikeout];

  // Add Text to Bottom Message and all its settings
  edtBottom.Text := Settings.BottomMessage;
  edtBottom.Font.Name := Settings.BottomFontName;
  edtBottom.Font.Style := [];

  if (Settings.BottomFontStyleBold) then
     edtBottom.Font.Style := [fsBold];
  if (Settings.BottomFontStyleItal) then
     edtBottom.Font.Style := edtBottom.Font.Style + [fsItalic];
  if (Settings.BottomFontStyleUnder) then
     edtBottom.Font.Style := edtBottom.Font.Style + [fsUnderline];
  if (Settings.BottomFontStyleStrike) then
     edtBottom.Font.Style := edtBottom.Font.Style + [fsStrikeout];

  // Display last selected combobox selection
  if not(Settings.ScreenDesignOnOff) then
    LoadTheImageFiles;

  {Set password check box}
  if (not(Settings.Password = 'NONE')) then
     chkPassword.Checked := true
  else
     chkPassword.Checked := false;

   {Paint the buffer with what the user last selected}
   if not(Settings.ScreenDesignOnOff) then
     UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);

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

   // Highlight the last chosen image
   cboImages.ItemIndex := Settings.ImageListNo;

end; // Initialize

{-----------------------------------------------------------------------}

procedure TfrmConfig.cboImagesChange(Sender: TObject);
var
     FileTypeTemp: integer;
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin
     // Keeps it from causing an error (-1) when user
     // clicks it the wrong way
     if cboImages.ItemIndex < 0 then exit;

     gCancelPressed := False;
     pntImage.ShowHint := False;
     FileTypeTemp := NONE;


     case cboImages.ItemIndex of
       NONE: begin
          Settings.FileType := NONE;
          Settings.MainSingleOrDir := NONE;
          PaintImageWnd;
          UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
          PaintPreviewWnd;
          end;
       LOAD: begin
          // Init Global variables
          gImageDirStr := Settings.MainImage;
          //Make sure thay don't choose the same directory
          gOtherImageDirStr := Settings.BKImage;
          gFileType := Settings.FileType;
          gSingleDir := Settings.MainSingleOrDir;
          gDirRandomSort := Settings.MainDirRandomSort;
          gMainBack := METAFILE;
          gTileGraphic := False;
          gTurnOffImageModeSelections := False;

          //frmImageLoad := TfrmImageLoad.Create(Application);
          frmImageLoad.ShowModal;
          //frmImageLoad.Release;

          Settings.MainImage := gImageDirStr;
          Settings.MainSingleOrDir := gSingleDir;
          Settings.FileType := gFileType;
          Settings.MainDirRandomSort := gDirRandomSort;
          FileTypeTemp := gFileType;

          // Have combo box display what was last selected if they cancel out
          if gCancelPressed then
             cboImages.ItemIndex := Settings.ImageListNo;

          if not(gCancelPressed) and isPathValid(gImageDirStr) then begin
            if Settings.MainSingleOrDir = LOADDIR then begin
              loadWFMresource(pchar('folder'), artWMF);
            end else begin

              if (ArtPalette <> 0) and ColorMode256 then begin
                DeleteObject(ArtPalette);
                ArtPalette := 0;
              end;

              FileTypeTemp := loadFile(Settings.MainImage,artBMP,artWMF,SHOW_ERROR_MSG);
              if FileTypeTemp > 0 then begin
                Settings.FileType := FileTypeTemp;
                // Shows path as part of hint
                pntImage.ShowHint := True;
                pntImage.Hint := Settings.MainImage;
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
            if not(gCancelPressed) then PaintImageWnd;
          end;
       end;
     else if not(gCancelPressed) then begin
          loadWFMresource(pchar(resList.Strings[cboImages.ItemIndex]), artWMF);
          Settings.FileType := RESOURCE;
          Settings.MainDirRandomSort := SORTED_FILES_IN_DIR;
          FileTypeTemp := RESOURCE;
          Settings.MainSingleOrDir := NONE;
          Settings.MainImage := resList.Strings[cboImages.ItemIndex];
          PaintImageWnd;
       end;
     end;

   Settings.ImageListNo := cboImages.ItemIndex;

  //Paint the buffer with what the user last selected
  if not(gCancelPressed) and (FileTypeTemp > 0) then begin
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;
  end;
end; // cboImagesChange

{-----------------------------------------------------------------------}

procedure TfrmConfig.cboBackgndChange(Sender: TObject);
var
     FileTypeTemp: integer;
begin
     // Keeps it from causing an error (-1) when user
     // clicks it the wrong way
     if cboBackgnd.ItemIndex < 0 then exit;
     pntBackgrnd.ShowHint := False;
     FileTypeTemp := NONE;
     gCancelPressed := False;

     case cboBackgnd.ItemIndex of
       NONE: begin
          Settings.BkFileType := NONE;
          Settings.BKSingleOrDir := NONE;
          //pntBackgrnd.repaint;
          PaintBackgrndWnd;
          // This is to just get it to repaint
          UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
          PaintPreviewWnd;
          end;
       LOAD: begin
          // Init Global variables
          gImageDirStr := Settings.BKImage;
          //Make sure thay don't choose the same directory
          gOtherImageDirStr := Settings.MainImage;
          gFileType := Settings.BKFileType;
          gSingleDir := Settings.BKSingleOrDir;
          gDirRandomSort := Settings.BKDirRandomSort;
          gMainBack := BITMAP;
          gTileGraphic := True;
          gTurnOffImageModeSelections := False;

          //frmImageLoad := TfrmImageLoad.Create(Application);
          frmImageLoad.ShowModal;
          //frmImageLoad.Release;

          Settings.BKImage := gImageDirStr;
          Settings.BKSingleOrDir := gSingleDir;
          Settings.BKFileType := gFileType;
          Settings.BKDirRandomSort := gDirRandomSort;
          FileTypeTemp := gFileType;

          // Have combo box display what was last selected if they cancel out
          if gCancelPressed then
             cboBackgnd.ItemIndex := Settings.BKImageListNo;

          if not(gCancelPressed) and isPathValid(gImageDirStr) then begin

            if (BackPalette <> 0) and ColorMode256 then begin
               DeleteObject(BackPalette);
               BackPalette := 0;
            end;

            if Settings.BKSingleOrDir = LOADDIR then begin
               GifLoadFromResource( 'folderGIF', backBMP );
               if ColorMode256 then
                 if backBMP.PixelFormat = pf8bit then
                   BackPalette := backBMP.ReleasePalette;
            end else begin

              FileTypeTemp := loadFile(Settings.BKImage,backBMP,backWMF,SHOW_ERROR_MSG);
              if FileTypeTemp > 0 then begin
                Settings.BKFileType := FileTypeTemp;
                // Shows path as part of hint
                pntBackgrnd.ShowHint := True;
                pntBackgrnd.Hint := Settings.BKImage;
                if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
                  if backBMP.PixelFormat = pf8bit then
                    BackPalette := backBMP.ReleasePalette;
              end;
            end;
            if not(gCancelPressed) and (FileTypeTemp > 0) then
              PaintBackgrndWnd;
              //pntBackgrnd.repaint;
          end;
          end;
       COLORS: begin
         Settings.BKSingleOrDir := NONE;
         ColorDialog1.CustomColors.Text := Settings.CustomColors.Text;
         ColorDialog1.Color := clNone; // Clear out the color box
         ColorDialog1.Execute; // Show color box

         // Save Custom Colors to INI file
         if ColorDialog1.Color <> clNone then begin
           FileTypeTemp := COLORS;
           Settings.CustomColors.Text := ColorDialog1.CustomColors.Text;
           Settings.BKFillColor := ColorDialog1.Color;
           Settings.BkFileType := COLORS;
           //pntBackgrnd.repaint;
           PaintBackgrndWnd;
         end;
         end;
       else begin

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
         Settings.BKDirRandomSort := SORTED_FILES_IN_DIR;
         Settings.BKSingleOrDir := NONE;
         FileTypeTemp := RESOURCE;
         //pntBackgrnd.repaint;
         PaintBackgrndWnd;
       end;
     end;
   Settings.BkImageListNo := cboBackgnd.ItemIndex;

  //Paint the buffer with what the user last selected
  if not(gCancelPressed) and (FileTypeTemp > 0) then begin
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;
  end;
end; // cboBackgndChange

{-----------------------------------------------------------------------}

procedure TfrmConfig.pntImagePaint(Sender: TObject);
begin
   PaintImageWnd;
   //if ColorMode256 then PaintBackgrndWnd;
end; // pntImagePaint

{-----------------------------------------------------------------------}

procedure TfrmConfig.PaintImageWnd;
var tBuffer, maskBMP: TBitmap;
    tempStr: String;
    MaskType: Integer;
begin

  if Settings.ScreenDesignOnOff then begin
    pntImage.Canvas.Pen.Color := clBlack;
    pntImage.Canvas.Brush.Color := clBtnFace;
    pntImage.Canvas.Rectangle(0, 0, pntImage.Width, pntImage.Height);
    exit;
  end;

  // We're building it into a tBuffer first then blitting
  // it to the screen
  tBuffer := TBitmap.Create;
  tBuffer.Width := pntImage.Width;
  tBuffer.Height := pntImage.Height;
  tBuffer.Canvas.Brush.Color := clBtnFace;
  tBuffer.Canvas.FillRect(Rect(0,0,tBuffer.Width,tBuffer.Height));

  //AdjustWindowPalette(pntImage.canvas);
  LoadBitmapPalettes( pntImage.canvas, ArtPalette, BackPalette, Settings );
  //RemapBitmapPalette( ArtPalette, pntImage.canvas, False );

  case Settings.FileType of
    RESOURCE, WMF: tBuffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               pntImage.Height,artWMF.Width,artWMF.Height),ArtWMF);
    BMP: begin
       // Check for a mask file
       tempStr := Settings.MainImage;
       // Delete the last three charaters of the string
       delete(tempStr, Length(tempStr)-2, 3);
       tempStr := tempStr + 'msk';

       if FileExists( tempStr ) then begin
         maskBMP := TBitmap.Create;

         MaskType := IsMaskFileBMPorGIF( tempStr );

         if MaskType = BMP then begin
           try
              maskBMP.LoadFromFile( tempStr );
           except
           end;
         end else if MaskType = GIF then begin
           try
              GifLoadFromFile( tempStr, maskBMP );
           except
           end;
         end;

         if not maskBMP.Empty then begin
           tBuffer.Canvas.CopyMode := cmSrcAnd;
           tBuffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               pntImage.Height,artBMP.Width,artBMP.Height),maskBMP);
           tBuffer.Canvas.CopyMode := cmSrcPaint;
           tBuffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
           tBuffer.Canvas.CopyMode := cmSrcCopy;
         end else begin
           tBuffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
         end;
         maskBMP.free;
       end else
         tBuffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
    end;
  end;

  pntImage.Canvas.Draw(0,0,tBuffer);
  tBuffer.free;
end; // PaintImageWnd

{-----------------------------------------------------------------------}

procedure TfrmConfig.pntBackgrndPaint(Sender: TObject);
begin
    PaintBackgrndWnd;
    //if ColorMode256 then PaintImageWnd;
end; // pntBackgrndPaint

{-----------------------------------------------------------------------}

procedure TfrmConfig.PaintBackgrndWnd;
var t: TRect;
    tBuffer: TBitmap;
begin
  if Settings.ScreenDesignOnOff then begin
    pntBackgrnd.Canvas.Pen.Color := clBlack;
    pntBackgrnd.Canvas.Brush.Color := clBtnFace;
    pntBackgrnd.Canvas.Rectangle(0, 0, pntBackgrnd.Width, pntBackgrnd.Height);
    exit;
  end;

    // We're building it into a buffer first then blitting
  // it to the screen
  if Settings.BkFileType <> COLORS then begin
    tBuffer := TBitmap.Create;
    tBuffer.Width := pntBackgrnd.Width;
    tBuffer.Height := pntBackgrnd.Height;
    tBuffer.Canvas.Brush.Color := clBtnFace;
    tBuffer.Canvas.FillRect(Rect(0,0,pntBackgrnd.Width,pntBackgrnd.Height));

    //AdjustWindowPalette(pntBackgrnd.canvas);
    LoadBitmapPalettes( pntBackgrnd.canvas, ArtPalette, BackPalette, Settings );
    //RemapBitmapPalette( BackPalette, pntBackgrnd.canvas, True );
  end;

     case Settings.BkFileType of
       RESOURCE, BMP: begin
         // Check to see if image is too big
         if (backBMP.Width > pntBackgrnd.Width) or (backBMP.Height > pntBackgrnd.Height) then
           tBuffer.Canvas.StretchDraw(sizeItUp(pntBackgrnd.Width,
               pntBackgrnd.Height,backBMP.Width,backBMP.Height),backBMP)
         else
           tileBackground(tBuffer.canvas, backBMP, pntBackgrnd.Width, pntBackgrnd.Height, BMP);
       end;
       WMF: begin
         // Check to see if image is too big
         if (backWMF.Width > pntBackgrnd.Width) or (backWMF.Height > pntBackgrnd.Height) then
           tBuffer.Canvas.StretchDraw(sizeItUp(pntBackgrnd.Width,
               pntBackgrnd.Height,backWMF.Width,backWMF.Height),backWMF)
         else
         tileBackground(tBuffer.canvas, backWMF, pntBackgrnd.Width, pntBackgrnd.Height, WMF);
       end;
       COLORS: begin
         t := Rect(0,0,pntBackgrnd.Width,pntBackgrnd.Height);
         pntBackgrnd.canvas.brush.color := Settings.BKFillColor;
         pntBackgrnd.canvas.FillRect(t);
       end;
     end;

  if Settings.BkFileType <> COLORS then begin
    pntBackgrnd.Canvas.Draw(0,0,tBuffer);
    tBuffer.free;
  end;  

end; // PaintBackgrndWnd

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnTopLeftClick(Sender: TObject);
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

    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;

end; // btnTopLeftClick

{-----------------------------------------------------------------------}

procedure TfrmConfig.pntPreviewPaint(Sender: TObject);
begin
     PaintPreviewWnd;
end;

procedure TfrmConfig.PaintPreviewWnd;
var r: TRect;
  I: Integer;
begin
     r := Rect(0,0,pntPreview.Width,pntPreview.Height);
     if Settings.ScreenDesignOnOff then begin
       pntPreview.Canvas.Pen.Color := clBlack;
       pntPreview.Canvas.Brush.Color := clBtnFace;
       pntPreview.Canvas.Rectangle(0, 0, pntPreview.Width, pntPreview.Height);
       pntPreview.Canvas.Font.Name := 'Arial';
       pntPreview.Canvas.Font.Style := [fsBold];
       pntPreview.Canvas.Font.Size := 25;
       pntPreview.Canvas.Font.Color := clBtnShadow;
       I := (pntPreview.Width-pntPreview.Canvas.TextWidth('Screen')) div 2;
       pntPreview.Canvas.TextOut(I,2,'Screen');
       I := (pntPreview.Width-pntPreview.Canvas.TextWidth('Designer')) div 2;
       pntPreview.Canvas.TextOut(I,40,'Designer');
       I := (pntPreview.Width-pntPreview.Canvas.TextWidth('Active')) div 2;
       pntPreview.Canvas.TextOut(I,80,'Active');
     end else begin
       LoadBitmapPalettes( pntPreview.Canvas, ArtPalette, BackPalette, Settings );
       pntPreview.Canvas.StretchDraw(r, buffer);
     end;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.pntPreviewClick(Sender: TObject);
var r: TRect;
begin
  // update preview
  if not(Settings.ScreenDesignOnOff) then begin
     Settings.TopMessage := edtTop.Text;
     Settings.BottomMessage := edtBottom.Text;

     UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);

     r := Rect(0,0,pntPreview.Width,pntPreview.Height);
     pntPreview.Canvas.StretchDraw(r, buffer);
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.chkPasswordClick(Sender: TObject);
begin
    if (chkPassword.Checked) and (Settings.Password = 'NONE') then begin
       gPasswordEnabled := ''; //init password string
       gAskForPassword := true;
       frmPassSetup := TfrmPassSetup.Create(Application);
       frmPassSetup.ShowModal;
       frmPassSetup.Release;
       if gPasswordEnabled = '' then begin
         Settings.Password := 'NONE';
         chkPassword.Checked := false;
       end else
         Settings.Password := gPasswordEnabled;
    end else if (not(chkPassword.Checked)) and (not(Settings.Password = 'NONE')) then
     Settings.Password := 'NONE';
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.FormActivate(Sender: TObject);
{var
     temp: Array[0..300] of char;
     KeyName, Default: Array[0..50] of char;
     ShowAgain: Boolean;}
begin

  {Check for password protection}
  if (not(Settings.Password = 'NONE')) then begin
     gPasswordEnabled := Settings.Password;
     gAskForPassword := false;
     frmPassSetup := TfrmPassSetup.Create(Application);
     frmPassSetup.ShowModal;
     frmPassSetup.Release;
     if gPasswordEnabled = '' then begin
       close;
       exit;
     end;
  end;

    {Get Show Window}
  {StrPCopy(KeyName, 'Show Intro Message');
  StrPCopy(Default, '1');
  GetPrivateProfileString(APP_NAME, KeyName, Default, temp, SizeOf(temp), FILE_NAME);
  ShowAgain := Boolean(StrToInt(String(temp)));

  if ShowAgain then begin
    frmTwoWays := TfrmTwoWays.Create(Application);
    frmTwoWays.ShowModal;
  end;}
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnTransitionsClick(Sender: TObject);
begin
     // We're passing values back and forth through global
     // variables to keep it simple.

     // Load up global variables
     gSoundFilesInterrupt := Settings.SoundInterrupt;
     gTransOnOff := Settings.TransOnOff;
     gTransSpeed := Settings.TransSpeed;
     gTransFillColor := Settings.TransFillColor;
     gTransDelay := Settings.TransDelay;
     gSizeBitmaps := Settings.SizeBitmaps;
     gSoundDir := Settings.SoundFileDir;
     gSoundsOnOff := Settings.SoundDirOnOff;
     gSoundsRandomList := Settings.SoundRandList;
     gBottomText := edtBottom.Text;
     gTopText := edtTop.Text;
     gTopTextRand := Settings.TopTextRandom;
     gBottomTextRand := Settings.BottomTextRandom;
     gScreenDesignOnOff := Settings.ScreenDesignOnOff;
     gRandomBacks := Settings.RandomBackgrnds;

     // Show Window
     frmTransitions := TfrmTransitions.Create(Application);
     frmTransitions.ShowModal;
     frmTransitions.Release;

     // Save new settings
     Settings.TopTextRandom := gTopTextRand;
     Settings.BottomTextRandom := gBottomTextRand;
     Settings.TransOnOff := gTransOnOff;
     Settings.TransSpeed := gTransSpeed;
     Settings.TransFillColor := gTransFillColor;
     Settings.TransDelay := gTransDelay;
     Settings.SizeBitmaps := gSizeBitmaps;
     Settings.SoundFileDir := gSoundDir;
     Settings.SoundDirOnOff := gSoundsOnOff;
     Settings.SoundRandList := gSoundsRandomList;
     Settings.SoundInterrupt := gSoundFilesInterrupt;
     Settings.RandomBackgrnds := gRandomBacks;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnOrderFormClick(Sender: TObject);
begin
    //frmOrderForm := TfrmOrderForm.Create(Application);
    //frmOrderForm.ShowModal;
    //frmOrderForm.Release;

    // Give them the option to order from web or us mail
    frmChoice := TfrmChoice.Create(Application);
    frmChoice.ShowModal;
    frmChoice.Release;
end;

{-----------------------------------------------------------------------}

{procedure TfrmConfig.AdjustWindowPalette(canvas: TCanvas);
begin
  if ColorPalette256 then begin
    //UnrealizeObject(canvas.handle);

    // Both have palettes
    if (Settings.FileType = BMP) and
       ((Settings.BKFileType = BMP) or (Settings.BKFileType = RESOURCE)) then begin
       RemapBitmapPalette(ArtPalette, canvas, False);
       RemapBitmapPalette(BackPalette, canvas, True);
    end;

    // Background only has palette
    if (Settings.BKFileType = BMP) or (Settings.BKFileType = RESOURCE) then
       RemapBitmapPalette(BackPalette, canvas, False);

    // Main image only has palette
    if Settings.FileType = BMP then
       RemapBitmapPalette(ArtPalette, canvas, False);
  end;
end;}

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnTopTxtClick(Sender: TObject);
var
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin

  {Get Show Window}
  StrPCopy(KeyName, 'Show Textload');
  StrPCopy(aDefault, '1');
  GetPrivateProfileString(APP_NAME, KeyName, aDefault, temp, SizeOf(temp), FILE_NAME);
  ShowAgain := Boolean(StrToInt(String(temp)));

  if ShowAgain then begin
    frmTextLoad := TfrmTextLoad.Create(Application);
    frmTextLoad.ShowModal;
    frmTextLoad.Release;
  end;

  OpenDialog1.FileName := '';
  OpenDialog1.Filter := 'Text files (*.TXT)|*.txt' +
  '|All files (*.*)|*.*';

  if OpenDialog1.Execute then begin
    case (Sender as TSpeedButton).Tag of
      0: begin
         edtTop.Text := OpenDialog1.FileName;
         Settings.TopMessage := edtTop.Text;
      end;
      1: begin
         edtBottom.Text := OpenDialog1.FileName;
         Settings.BottomMessage := edtBottom.Text;
      end;
    end;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;
  end;
end;


{-----------------------------------------------------------------------}

procedure TfrmConfig.btnSoundClick(Sender: TObject);
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

  gSoundDir := Settings.SoundFileDir;
  gSoundsOnOff := Settings.SoundDirOnOff;
  gSoundsOnOff := True;

  // Show Window
  //frmSoundDir := TfrmSoundDir.Create(Application);
  frmSoundDir.ShowModal;
  //frmSoundDir.Release;

  Settings.SoundFileDir := gSoundDir;
  Settings.SoundDirOnOff := gSoundsOnOff;

end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnHelpClick(Sender: TObject);
begin

  LoadHelpFile;

end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.btnDesignerClick(Sender: TObject);
begin
   gScreenDesignOnOff := Settings.ScreenDesignOnOff;
   Settings.ScreenDesignOnOff := True;
   gScreenDesignPath := Settings.ScreenDesignPath;
   gScreenDesignRandom := Settings.ScreenDesignRandom;
   gCustomColors.Text := Settings.CustomColors.Text;
   gSoundSDonOff := Settings.SoundSDOnOff;

   btnBlink := False;
   GTimer.Enabled := False;
   btnBlink := False;
   BlinkButton;

   // Show Window
   //frmDesigner := TfrmDesigner.Create(Application);
   frmDesigner.ShowModal;
   //frmDesigner.Release;

   ScreenDsnExit := True;

   // Check that the path is valid
   if gScreenDesignOnOff then begin
     if FileExists(gScreenDesignPath) then begin
       EnableDisableWnd( False );
       GTimer.Enabled := True;
     end else begin
       gScreenDesignOnOff := False;
       gScreenDesignPath := '';
     end;
   end;

   // Reload all the files so that all is the same
   if not(gScreenDesignOnOff) then begin
     btnBlink := False;
     BlinkButton;
     LoadTheImageFiles;
     UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
     EnableDisableWnd( True );
   end;

   Settings.ScreenDesignPath := gScreenDesignPath;
   Settings.CustomColors.Text := gCustomColors.Text;
   Settings.ScreenDesignOnOff := gScreenDesignOnOff;
   Settings.ScreenDesignRandom := gScreenDesignRandom;
   Settings.SoundSDOnOff := gSoundSDonOff;

   // Reset the above settings before repainting
   pntBackgrnd.Repaint;
   pntImage.Repaint;
   pntPreview.Repaint;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.LoadTheImageFiles;
var  S: String;
begin
  // Display last selected combobox selection
  //cboImages.ItemIndex := Settings.ImageListNo;
  case Settings.FileType of
     RESOURCE: begin
       if Settings.MainSingleOrDir = LOADDIR then
          loadWFMresource(pchar('folder'), artWMF)
       else begin
          // Make sure there is not an error and this is a path  cluge fix
          if not( isPathValid( Settings.MainImage )) then
            loadWFMresource(pchar(Settings.MainImage), artWMF)
          else
            Settings.FileType := NONE; // set it to none if there is a problem
       end;
     end;
     BMP, WMF: begin
      // If they are loading up a directory, just show folder
      if Settings.MainSingleOrDir = LOADDIR then begin
        if DirectoryExists(Settings.MainImage) then begin
           Settings.FileType := WMF; //RESOURCE;
           loadWFMresource(pchar('folder'), artWMF);
           pntImage.ShowHint := True;  // Setup the Hint
           pntImage.Hint := Settings.MainImage;
        end else begin
           Settings.FileType := NONE;
           Settings.ImageListNo := NONE;
           Settings.MainSingleOrDir := NONE;
           S := 'The directory that was choosen from your hard drive "' +
           Settings.MainImage + '" is now missing. You will need to' +
           ' find it again or use a different directory.';

          Application.MessageBox(PChar(S),'There has been an error! Missing directory!',
             mb_OK + MB_ICONEXCLAMATION);
        end;
      end else begin
        if FileExists(Settings.MainImage) then begin
          if (ArtPalette <> 0) and ColorMode256 then begin
             DeleteObject(ArtPalette);
             ArtPalette := 0;
          end;
           if loadFile(Settings.MainImage,artBMP,artWMF,SHOW_ERROR_MSG)>0 then begin
             if (Settings.FileType > WMF) and ColorMode256 then
               if ArtBMP.PixelFormat = pf8bit then
                 ArtPalette := ArtBMP.ReleasePalette;
             pntImage.ShowHint := True;  // Setup the Hint
             pntImage.Hint := Settings.MainImage;
           end else begin
              Settings.FileType := NONE;
              Settings.ImageListNo := NONE;
              cboImages.ItemIndex := Settings.ImageListNo;
           end;
        end else begin
          Settings.FileType := NONE;
          Settings.ImageListNo := NONE;
          cboImages.ItemIndex := Settings.ImageListNo;
          S := 'The file that was choosen from your hard drive "' +
          Settings.MainImage + '" is now missing. You will need to' +
          ' find it again or use a different image.';

          Application.MessageBox(PChar(S),'There has been an error! Missing Image!',
             mb_OK + MB_ICONEXCLAMATION);
        end;
       end;
     end;
  end;

  // Display last selected image list no
  cboBackgnd.ItemIndex := Settings.BkImageListNo;
  case Settings.BkFileType of
  RESOURCE: begin
    if (BackPalette <> 0) and ColorMode256 then begin
       DeleteObject(BackPalette);
       BackPalette := 0;
    end;

    if Settings.BKSingleOrDir = LOADDIR then
      GifLoadFromResource( 'folderGIF', backBMP )
    else begin
      // Make sure there is not an error and this is a path  cluge fix
      if not( isPathValid( Settings.BKImage )) then
         GifLoadFromResource( resBack.Strings[cboBackgnd.ItemIndex], backBMP )
      else
          Settings.BkFileType := NONE; // set it to none if there is a problem
    end;

    if ColorMode256 then
      if backBMP.PixelFormat = pf8bit then
        BackPalette := backBMP.ReleasePalette;
  end;
  BMP, WMF: begin
    // If they are loading up a directory, just show folder
    if Settings.BKSingleOrDir = LOADDIR then begin
       if DirectoryExists(Settings.BKImage) then begin
         if (BackPalette <> 0) and ColorMode256 then begin
            DeleteObject(BackPalette);
            BackPalette := 0;
         end;
         Settings.BkFileType := BMP; //RESOURCE;
         GifLoadFromResource( 'folderGIF', backBMP );
         if ColorMode256 then
           if backBMP.PixelFormat = pf8bit then
             BackPalette := backBMP.ReleasePalette;
         pntBackgrnd.ShowHint := True;  // Setup the Hint
         pntBackgrnd.Hint := Settings.BKImage;
       end else begin
         Settings.BKFileType := NONE;
         Settings.BKImageListNo := NONE;
         Settings.BKSingleOrDir := NONE;
         S := 'The directory that was choosen from your hard drive "' +
         Settings.BKImage + '" is now missing. You will need to' +
         ' find it again or use a different directory.';

         Application.MessageBox(PChar(S),'There has been an error! Missing directory!',
             mb_OK + MB_ICONEXCLAMATION);
       end;
    end else begin
       if FileExists(Settings.BKImage) then begin
          if (BackPalette <> 0) and ColorMode256 then begin
             DeleteObject(BackPalette);
             BackPalette := 0;
          end;
           if loadFile(Settings.BKImage,backBMP,backWMF,SHOW_ERROR_MSG)>0 then begin
             pntBackgrnd.ShowHint := True;  // Setup the Hint
             pntBackgrnd.Hint := Settings.BKImage;
             if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
               if backBMP.PixelFormat = pf8bit then
                 BackPalette := backBMP.ReleasePalette;
           end else begin
             Settings.BkFileType := None; // There has been an error
             Settings.BkImageListNo := None;
             cboBackgnd.ItemIndex := Settings.BkImageListNo;
           end;
       end else begin
          Settings.BkFileType := None;
          Settings.BkImageListNo := None;
          cboBackgnd.ItemIndex := Settings.BkImageListNo;
          S := 'The file that was choosen from your hard drive "' +
          Settings.BKImage + '" is now missing. You will need to' +
          ' find it again or use a different image.';

         Application.MessageBox(PChar(S),'There has been an error! Missing Image!',
           mb_OK + MB_ICONEXCLAMATION);
       end;
     end;
   end;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.EnableDisableWnd( I: Boolean );
begin
  cboImages.Enabled := I;
  edtTop.Enabled := I;
  edtBottom.Enabled := I;
  btnTopLeft.Enabled := I;
  btnTopCenter.Enabled := I;
  btnTopRight.Enabled := I;
  btnBottomLeft.Enabled := I;
  btnBottomCenter.Enabled := I;
  btnBottomRight.Enabled := I;
  cboBackgnd.Enabled := I;
  btnFontsTop.Enabled := I;
  btnColorsTop.Enabled := I;
  btnTopTxt.Enabled := I;
  btnFontsBottom.Enabled := I;
  btnColorsBottom.Enabled := I;
  btnBottomTxt.Enabled := I;
  btnSound.Enabled := I;
  cboBackgnd.Enabled := I;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.GTimerTimer(Sender: TObject);
begin
  BlinkButton;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.BlinkButton;
begin
  if btnBlink then begin
    Canvas.Pen.Color := clBtnShadow;
    btnBlink := False;
  end else begin
    Canvas.Pen.Color := clBtnFace;
    btnBlink := True;
  end;

  Canvas.Pen.Width := 4;
  Canvas.Brush.Style := bsClear;
  Canvas.RoundRect(btnDesigner.Left - 2, btnDesigner.Top - 2,
                   btnDesigner.Width + btnDesigner.Left + 2,
                   btnDesigner.Height + btnDesigner.Top + 2, 2, 2);
end;

{-----------------------------------------------------------------------}

{function TfrmConfig.CreateTmpRegFile(FileName: String; I: Integer): Boolean;
var hFile, J: Integer;
begin
  result := True;

  hFile := FileCreate(FileName);
  if hFile <= 0 then begin
    Application.MessageBox( PChar(LoadStr( TMP_REG_ERROR )),'Error', mb_OK);
    DeleteFile(FileName);
    result := False;
    exit;
  end;

  J := FileWrite( hFile, I, sizeof(Integer) );
  if (J <= 0) or not(J = sizeof(Integer) ) then begin
    Application.MessageBox( PChar(LoadStr( TMP_REG_ERROR )),'Error', mb_OK);
    DeleteFile(FileName);
    result := False;
  end;

  FileClose(hFile);

end;}

{-----------------------------------------------------------------------}

procedure TfrmConfig.edtTopKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  Settings.TopMessage := trim(edtTop.Text);
  if Key = vk_Return then begin
    Key := 0;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.edtBottomKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  Settings.BottomMessage := trim(edtBottom.Text);
  if Key = vk_Return then begin
    Key := 0;
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
    PaintPreviewWnd;
  end;
end;

{-----------------------------------------------------------------------}

procedure TfrmConfig.Timer1Timer(Sender: TObject);
var
     temp: Array[0..300] of char;
     KeyName, aDefault: Array[0..50] of char;
     ShowAgain: Boolean;
begin
  Timer1.Enabled := False;

    {Get Show Window}
  StrPCopy(KeyName, 'Show Intro Message');
  StrPCopy(aDefault, '1');
  GetPrivateProfileString(APP_NAME, KeyName, aDefault, temp, SizeOf(temp), FILE_NAME);
  ShowAgain := Boolean(StrToInt(String(temp)));

  if ShowAgain then begin
    frmTwoWays := TfrmTwoWays.Create(Application);
    frmTwoWays.ShowModal;
    frmTwoWays.Release;
  end;

end;

{-----------------------------------------------------------------------}

end.
