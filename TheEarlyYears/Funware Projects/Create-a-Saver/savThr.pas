unit savThr;

interface

uses
   Windows, Classes, Graphics, Forms, SysUtils, gOrderfm, SoundMgr, FileCtrl, mediaPlayer,
   Messages, Dialogs, savInit, Loadpref, MMSystem, Globals, constants, gFunctin;

type
  PTPoint = ^TPoint;

type
  TMass = record
     W: Integer;
     H: Integer;
  end;

{this is the basic class, only change it if you want to change the logic of working}
type TSaverBasics = class (TThread)
private
       FForceRect : TRect;
       procedure SetForceRect (ARect : TRect);
protected
         Canvas: TCanvas; {the canvas to draw the saver action to}
         Left,                 {dimensions of the display window      }
         Top,                  {all drawing must be done inside       }
         Width,                {the here specified dimensions         }
         Height,
         {totalSpeed,} Mode, {time to go to sleep for Sleep function}
         smallCount: Integer;
         SmallCanvas, BigCanvas, PaintPicture, sleeper,
         firstTimeRun, SharewareUser, OnceThrough,
         BackDirLoad, MainDirLoad, ColorMode256,
         TopStrLoadFromFile, BottomStrLoadFromFile, SoundLoadFromDir: Boolean;
         artWMF, backWMF: TMetafile;
         buffer, smallBuffer, artBMP, backBMP, theLogo: TBitmap;
         PaintSquares, tempPS: PTPoint;
         randWH: TMass;
         PSSize, SquCount, MainImageCount, BackImageCount,
         TopStrCount, BottomStrCount, SoundFileCount, TotalSquAlloc: Integer;
         BottomStrList, TopStrList, BackDirList, MainDirList, SoundDirList: TStringList;
         r: TRect;
         ShareDelayTime, timerTmp, SquareTime, DelayTimer, SleepTime: DWORD;
         Settings: TScrnSavSettings;
         pScreenD, pScrBase: PTScreenDesign;
         TotalScr, CurrentScr, FastCounter, VirtualScreenWidth, VirtualScreenHeight: Integer;
         sort, tmpSort, tempSortI, tempSortJ: ^Integer;
         procedure AStep; //virtual;
         function CalculateSquares( var PaintSquares: PTPoint; randWH: TMass): Integer;
         procedure LoadFilesFromHardDrive;
         procedure ScreenDesignerImages;
         function GetRectInScreen( X, Y, W, H: Integer ):TRect;
         //procedure DeleteThePalettes;
public
      constructor Create(ACanvas : TCanvas; DrawRect : TRect; BigSmallCanvas: Boolean); //virtual;
      {ACanvas holds the canvas to draw the saver action to and the
      DrawRect holds the wanted rectangle, usually this is the full canvas}
      procedure Execute; override;
      destructor Destroy; override;
      {don't care about this}
      procedure ClearForceRect;
      {call this if you have redrawn the ForceRect to set it to 0,0,0,0}
      property ForceRect : TRect read FForceRect write SetForceRect;
      {this holds a ForceRect and gets called whenever a rectangle needs
      painting. This is caused by a password dialog dragged over the window
      or windows being dragged over the preview window. Make sure you draw
      the ForceRect in your AStep function to make your saver look nice. But
      you can also ignore this}
end;

{these functions are very essentiyl, study them, don't change them}
procedure ExecPreview;
procedure ExecSetPwd;

implementation

uses savRun;

{-----------------------------------------------------------------------}

destructor TSaverBasics.Destroy;
begin

  // Clear out the last sound if a sound was playing
  {if BigCanvas then begin
    if frmMID.GetPlayerStatus > Closed then begin
       if frmMID.GetPlayerStatus = Playing then
          frmMID.MediaPlayer1.Stop;
       frmMID.MediaPlayer1.Close;
    end;
  end;}

  {Free all the objects here}
  artWMF.free;
  backWMF.free;
  buffer.free;
  artBMP.free;
  backBMP.free;
  Settings.free;

  if Assigned(pScrBase) then
    freemem(pScrBase, TotalScr * sizeOf(TScreenDesign));
  
  if Assigned(TopStrList) then
    TopStrList.free;

  if Assigned(BottomStrList) then
    BottomStrList.free;

  if Assigned(theLogo) then
    theLogo.Free;

  if Assigned(smallBuffer) then
     smallBuffer.free;

  if TotalSquAlloc > 0 then begin
     if Assigned(PaintSquares) then
       freeMem(PaintSquares, TotalSquAlloc * sizeOf(TPoint));
     if Assigned(sort) then
       freeMem(sort, SizeOf(Integer) * TotalSquAlloc);
  end;

  if Assigned(MainDirList) then
    MainDirList.free;

  if Assigned(BackDirList) then
    BackDirList.free;

  if Assigned(SoundDirList) then
    SoundDirList.free;

end;


{-----------------------------------------------------------------------}

constructor TSaverBasics.Create (ACanvas : TCanvas; DrawRect : TRect; BigSmallCanvas: Boolean );
var
     ErrorScr, FileOpened: Boolean;
     txtFile: System.Text;
     S, TempStr, PathStr: string;
     SaverFile: File of TScreenDesign;
     WinDir: array[0..100] of char;
     tempHorz, tmpVer: Integer;
begin

  {set variables}
  Canvas := ACanvas;
  Left   := DrawRect.Left;
  Top    := DrawRect.Top;
  Width  := DrawRect.Right  - DrawRect.Left;
  Height := DrawRect.Bottom - DrawRect.Left;
  FForceRect := DrawRect;
  ScreenW := Screen.Width;
  ScreenH := Screen.Height;

  // Init all theses just to be safe
  TopStrLoadFromFile := False;
  BottomStrLoadFromFile := False;
  MainImageCount := 0;
  FastCounter := 0;
  TotalSquAlloc := 0;
  BackImageCount := 0;
  BackDirLoad := False;
  MainDirLoad := False;
  TopStrCount := 0;
  BottomStrCount := 0;
  smallCount := 0;
  PaintPicture := true;
  SoundLoadFromDir := False;
  ErrorScr := False;
  TotalScr := 0;
  CurrentScr := 0;
  FileOpened := False;
  PSSize := 0;
  SquCount := 0;
  SoundFileCount := 0;
  timerTmp := timeGetTime;
  S := '';
  firstTimeRun := True;
  pScreenD := nil;
  pScrBase := nil;
  TopStrList := nil;
  BottomStrList := nil;
  theLogo := nil;
  smallBuffer := nil;
  MainDirList := nil;
  BackDirList := nil;
  SoundDirList := nil;
  PaintSquares := nil;
  sort := nil;
  OnceThrough := False;
  DelayTimer := 0;

  if GetDeviceCaps( canvas.handle, BITSPIXEL ) <= 8 then
    ColorMode256 := True
  else
    ColorMode256 := False;

  //Setup variables for canvas size
  if BigSmallCanvas then begin
   SmallCanvas := false;
   BigCanvas := true;
  end else begin
   SmallCanvas := true;
   BigCanvas := false;
   //if they are running their screen in higher res
   //than 1024 x 768, set it to that for preview window
   //if ScreenW > 1024 then begin
   //   ScreenW := 1024;
   //   ScreenH := 768;
   //end;
  end;

  // If we have duel monitor support, get the virtual desktop size
  // Because we will need to size the window bigger and move it
  if BigCanvas and (GetSystemMetrics( SM_CMONITORS ) > 1) then begin

     VirtualScreenWidth := GetSystemMetrics( SM_CXVIRTUALSCREEN );
     VirtualScreenHeight := GetSystemMetrics( SM_CYVIRTUALSCREEN );

     if (VirtualScreenWidth > ScreenW) or (VirtualScreenHeight > ScreenH) then begin
        frmFullScreen.Left := GetSystemMetrics( SM_XVIRTUALSCREEN );
        frmFullScreen.Top := GetSystemMetrics( SM_YVIRTUALSCREEN );
        frmFullScreen.Width := VirtualScreenWidth;
        frmFullScreen.Height := VirtualScreenHeight;
     end;
     
  end else begin
     VirtualScreenWidth := ScreenW;
     VirtualScreenHeight := ScreenH;
  end;

  // Don't show error message the windows way
  if BigCanvas then
      frmMID.ShowErrorMessages := False;

   {Load INI settings}
   Settings := TScrnSavSettings.Create;
   Settings.GetINISettings;

   // Don't let them erase with next image if system has changed
   if ColorMode256 and (Settings.TransFillColor = ERASE_WITH_NEXT_IMAGE) then
     Settings.TransFillColor := ERASE_FILL_BLACK;

   // check to see if they registered
   if Settings.Registration = REGISTRATION_VALIDATION then
     SharewareUser := false
   else begin
     SharewareUser := true;
     // Check for temp registration

     TempStr := GetTheWindowsDirectory;

     PathStr := TempStr + LoadStr( TMP_REG_FILE1 );
     if FileExists(PathStr) then
       if IsTmpRegFileValid(PathStr, TMP1_REG_AMT) then
         SharewareUser := false;

     PathStr := TempStr + LoadStr( TMP_REG_FILE2 );
     if FileExists(PathStr) then
       if IsTmpRegFileValid(PathStr, TMP2_REG_AMT) then
         SharewareUser := false;
   end;

   //if Settings.ScreenDesignOnOff then
     //Settings.TransOnOff := True;

  // Create another buffer if we are
  // running in preview mode
  if (SmallCanvas and Settings.TransOnOff) or DEBUGIT then begin
     smallBuffer := TBitmap.Create;
     smallBuffer.Width := ScreenW;
     smallBuffer.Height := ScreenH;
     smallBuffer.Canvas.Brush.Color := clBlack;
     r := Rect(0,0,ScreenW,ScreenH);
     smallBuffer.Canvas.FillRect(r);
  end;

   {Lets Make the Buffer}
   buffer := TBitmap.Create;
   buffer.Width := ScreenW;
   buffer.Height := ScreenH;
   buffer.Canvas.Brush.Color := clBlack;
   r := Rect(0,0,ScreenW,ScreenH);
   buffer.Canvas.FillRect(r);

   artWMF := TMetafile.Create;
   backWMF := TMetafile.Create;
   artBMP := TBitmap.Create;
   backBMP := TBitmap.Create;

   //ArtPalette := ArtBMP.ReleasePalette;
   //BackPalette := BackBMP.ReleasePalette;

  //artBMP.IgnorePalette := True;
  //backBMP.IgnorePalette := True;
  //buffer.IgnorePalette := True;

   // Set to empty strings so that we check them later
   Settings.MainFileLoadErrorStr := '';
   Settings.BackFileLoadErrorStr := '';
   Settings.SoundFileLoadErrorStr := '';

   // If they are using the screen designer, load it up
   if Settings.ScreenDesignOnOff and FileExists(Settings.ScreenDesignPath) then begin

     // init the string list for sound paths
     if Settings.SoundInterrupt and BigCanvas and Settings.SoundSDOnOff then
        SoundDirList := TStringList.Create;

     try
       System.Assign(SaverFile, Settings.ScreenDesignPath);
       Reset(SaverFile);

       FileOpened := True;

       while not EOF(SaverFile) or ErrorScr do begin
         pScreenD := pScrBase;
         inc(TotalScr);
         ReallocMem( pScreenD, SizeOf(TScreenDesign) * TotalScr );

         // Check for memory errors
         if pScreenD = nil then begin
           //Application.MessageBox('There has been an error allocating memory.', 'Error', mb_OK);
           freemem(pScrBase, TotalScr-1 * sizeOf(TScreenDesign));
           pScrBase := nil;
           Settings.ScreenDesignOnOff := False;
           ErrorScr := True;
           break;
         end;

         pScrBase := pScreenD; // re-init base
         inc(pScreenD, TotalScr-1);

         System.Read(SaverFile, pScreenD^);

         // If the sound files are played to completion so we need to save all
         // the paths because we will need to load them from this string list
           if pScreenD^.SoundOnOff and assigned(SoundDirList) then
             SoundDirList.Add( pScreenD^.SoundFile );
       end;

       // Free it if there are no sound files
       if assigned(SoundDirList) then begin
          if SoundDirList.Count = 0 then begin
             SoundDirList.free;
             SoundDirList := nil;
          end;
       end;

     except
       ErrorScr := True;
     end;

     if not(ErrorScr) then
       System.Close(SaverFile)
     else begin
       if FileOpened then
         System.Close(SaverFile);
       Settings.ScreenDesignOnOff := False;
       TotalScr := 0;
       Settings.MainFileLoadErrorStr := 'ERROR: Reading File ' +
       Settings.ScreenDesignPath + ' into memory.';
     end;

     pScreenD := pScrBase;

     // Just in case they set up only one screen
     if TotalScr = 1 then begin
       CopySettings( Settings, pScreenD );
       //Settings.ScreenDesignOnOff := False;
       //freemem(pScreenD, TotalScr-1 * sizeOf(TScreenDesign));
       //TotalScr := 0;
     end;
   end;

   // They lost the screen designer file
   if Settings.ScreenDesignOnOff and not(FileExists(Settings.ScreenDesignPath)) then
     Settings.ScreenDesignOnOff := False;

   //Lets see if they are loading a file of string lists
   //Top List
  if not(Settings.ScreenDesignOnOff) then begin
   if isPathValid(Settings.TopMessage) then begin
     if FileExists(Settings.TopMessage) then begin
       TopStrList := TStringList.Create;
       FileOpened := False;
       TopStrLoadFromFile := True;
       try
         System.Assign(txtFile, Settings.TopMessage);
         Reset(txtFile);

         FileOpened := True;

         while not EOF(txtFile) do begin
            ReadLn(txtFile, S);
            TopStrList.Add(S);
         end;
       except
          Settings.TopMessage := 'Error loading ' + Settings.TopMessage;
          TopStrList.free;
          TopStrList := nil;
          TopStrLoadFromFile := False;
       end;

       if FileOpened then
         System.Close(txtFile);

       // There file was empty
       if S = '' then begin
         Settings.TopMessage := Settings.TopMessage + ' is empty';
         TopStrLoadFromFile := False;
         TopStrList.free;
         TopStrList := nil;
       end;

       // Just grab the first string if they turned transsition off
       //if not(Settings.TransOnOff) then begin
         //TopStrLoadFromFile := False;
         Settings.TopMessage := TopStrList.Strings[0];
       //end;
     end else // Tell them they have an error
       Settings.TopMessage := 'Text File Missing: ' + Settings.TopMessage;
   end;

   S := ''; // Reset to check if we loaded an empty string

   //Lets see if they are loading a file of string lists
   //Bottom List
   if isPathValid(Settings.BottomMessage) then begin
     if FileExists(Settings.BottomMessage) then begin
       BottomStrList := TStringList.Create;
       BottomStrLoadFromFile := True;
       FileOpened := False;
       try
         System.Assign(txtFile, Settings.BottomMessage);
         Reset(txtFile);
         FileOpened := True;
         while not EOF(txtFile) do begin
            ReadLn(txtFile, S);
            BottomStrList.Add(S);
         end;
       except
          Settings.BottomMessage := 'Error loading ' + Settings.BottomMessage;
          BottomStrList.free;
          BottomStrList := nil;
          BottomStrLoadFromFile := False;
       end;

       if FileOpened then
         System.Close(txtFile);

       // There file was empty
       if S = '' then begin
         Settings.BottomMessage := Settings.BottomMessage + ' is empty';
         BottomStrLoadFromFile := False;
         BottomStrList.free;
         BottomStrList := nil;
       end;  

       // Just grab the first string if they turned transsition off
       //if not(Settings.TransOnOff) then begin
         //BottomStrLoadFromFile := False;
         Settings.BottomMessage := BottomStrList.Strings[0];
       //end;
     end else // Tell them they have an error
       Settings.BottomMessage := 'Text File Missing: ' + Settings.BottomMessage;
   end;
  end; // Settings.ScreenDesignOnOff


  //Get Background image and tile it or paint it a solid color
  if not(Settings.ScreenDesignOnOff) then begin
   case Settings.BkFileType of
       RESOURCE: begin
         // Do they want to randomize the backgrounds
         if Settings.RandomBackgrnds then
            GifLoadFromResource( 'Back'+ IntToStr(Random(39)+1), backBMP )
         else begin
            // Make sure there is not an error and this is a path  cluge fix
            if not( isPathValid( Settings.BKImage )) then
               GifLoadFromResource( Settings.BKImage, backBMP )
            else
               Settings.BkFileType := NONE; // make it none if there was an error
         end;
         {if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
           if backBMP.PixelFormat = pf8bit then
             BackPalette := backBMP.ReleasePalette;}
       end;
     BMP, WMF: begin // load selected file
       // If they are loading up a directory
       if Settings.BKSingleOrDir = LOADDIR then begin
          BackDirLoad := True;
          BackDirList := TStringList.Create;
          // Checked in savRun
          if not DirectoryExists(Settings.BKImage) then begin
            Settings.BkFileType := RESOURCE;
            Settings.RandomBackgrnds := False;
            Settings.BKSingleOrDir := NONE;
            Settings.BackFileLoadErrorStr := Settings.BKImage;
            BackDirLoad := False;
            GifLoadFromResource( 'dirMisGIF', backBMP );
          end;

          // Get all the possible file names
          if BackDirLoad then begin
            GetAllFileNamesInDir( BackDirList, Settings.BKImage, '*.bmp' );
            GetAllFileNamesInDir( BackDirList, Settings.BKImage, '*.gif' );
            GetAllFileNamesInDir( BackDirList, Settings.BKImage, '*.jpg' );
            GetAllFileNamesInDir( BackDirList, Settings.BKImage, '*.jpeg' );
            GetAllFileNamesInDir( BackDirList, Settings.BKImage, '*.wmf' );
            GetAllFileNamesInDir( BackDirList, Settings.BKImage, '*.emf' );
            if BackDirList.Count > 0 then begin
              BackDirList.Sorted := True;
              BackDirList.Sorted := False;
            end;
          end;

          if (BackDirList.Count = 0) and BackDirLoad then begin
            Settings.BkFileType := RESOURCE;
            Settings.RandomBackgrnds := False;
            Settings.BKSingleOrDir := NONE;
            Settings.BackFileLoadErrorStr := Settings.BKImage;
            GifLoadFromResource( 'dirEmptyGIF', backBMP );
            BackDirLoad := False;
          end;
          // Check to see that they have more than one image in this directory
          //if ((BackDirList.Count = 1) and BackDirLoad) or not(Settings.TransOnOff) then begin
          if (BackDirList.Count = 1) and BackDirLoad then begin
            BackDirLoad := False;
            Settings.BKImage := BackDirList.Strings[0];
            LoadBackImage( Settings, backBMP, backWMF, NO_ERROR_MSG );
            Settings.BKSingleOrDir := NONE;
            // Set it to a single file load if they turned transition off
            if not(Settings.TransOnOff) then
              Settings.BKSingleOrDir := LOAD;
          end;
       end else
         LoadBackImage( Settings, backBMP, backWMF, NO_ERROR_MSG );

       {if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
         if backBMP.PixelFormat = pf8bit then
           BackPalette := backBMP.ReleasePalette;}
     end;
   end;

   // Load forground Image
   case Settings.FileType of
     RESOURCE: begin
       // Make sure there is not an error and this is a path  cluge fix
       if not( isPathValid( Settings.MainImage )) then
           loadWFMresource(pchar(Settings.MainImage), artWMF)
       else
           Settings.FileType := NONE; // set it to none if there is a problem
     end;
     BMP, WMF: begin
       // If they are loading up a directory
       if Settings.MainSingleOrDir = LOADDIR then begin
          MainDirLoad := True;
          MainDirList := TStringList.Create;

          if not DirectoryExists(Settings.MainImage) then begin
            loadWFMresource('dirMissing', artWMF);
            Settings.FileType := RESOURCE;
            Settings.MainFileLoadErrorStr := Settings.MainImage;
            Settings.MainSingleOrDir := NONE;
            MainDirLoad := False;
          end;

          // Get all the possible file names
          if MainDirLoad then begin
            GetAllFileNamesInDir( MainDirList, Settings.MainImage, '*.bmp' );
            GetAllFileNamesInDir( MainDirList, Settings.MainImage, '*.gif' );
            GetAllFileNamesInDir( MainDirList, Settings.MainImage, '*.jpg' );
            GetAllFileNamesInDir( MainDirList, Settings.MainImage, '*.jpeg' );
            GetAllFileNamesInDir( MainDirList, Settings.MainImage, '*.wmf' );
            GetAllFileNamesInDir( MainDirList, Settings.MainImage, '*.emf' );
            if MainDirList.Count > 0 then begin
              MainDirList.Sorted := True;
              MainDirList.Sorted := False;
            end;
          end;

          if (MainDirList.Count = 0) and MainDirLoad then begin
            loadWFMresource('dirEmpty', artWMF);
            Settings.FileType := RESOURCE;
            Settings.MainFileLoadErrorStr := Settings.MainImage;
            MainDirLoad := False;
            Settings.MainSingleOrDir := NONE;
          end;
          // Check to see that they have more than one image in this directory
          // or they set the trans to off
          //if ((MainDirList.Count = 1) and MainDirLoad) or not(Settings.TransOnOff) then begin
          if (MainDirList.Count = 1) and MainDirLoad then begin
            MainDirLoad := False;
            Settings.MainImage := MainDirList.Strings[0];
            LoadMainImage( Settings, artBMP, artWMF, NO_ERROR_MSG );
            Settings.MainSingleOrDir := NONE;
            // Set it to a single file load if they turned transition off
            if not(Settings.TransOnOff) then
               Settings.MainSingleOrDir := LOAD;
          end;
       end else // load single file
         LoadMainImage( Settings, artBMP, artWMF, NO_ERROR_MSG );

       {if (Settings.FileType > WMF) and ColorMode256 then
         if ArtBMP.PixelFormat = pf8bit then
          ArtPalette := ArtBMP.ReleasePalette;}
     end;
    end;
  end; // if not(Settings.ScreenDesignOnOff) then begin

  // Get all the sound file names
  if not(Settings.ScreenDesignOnOff) then begin
   if Settings.SoundDirOnOff and BigCanvas then begin
     SoundDirList := TStringList.Create;
     GetAllFileNamesInDir( SoundDirList, Settings.SoundFileDir, '*.wav' );
     GetAllFileNamesInDir( SoundDirList, Settings.SoundFileDir, '*.mid' );
     GetAllFileNamesInDir( SoundDirList, Settings.SoundFileDir, '*.rmi' );
     GetAllFileNamesInDir( SoundDirList, Settings.SoundFileDir, '*.mp3' );
     if SoundDirList.Count > 0 then begin
        SoundLoadFromDir := True;
        SoundDirList.Sorted := True;
        SoundDirList.Sorted := False;
     end else begin// Free it if there are no sound files
        SoundDirList.free;
        SoundDirList := nil;
     end;

   end;
  end;

   // Paint the screen saver to the buffer
  if not(Settings.ScreenDesignOnOff) then begin
   if (not(Settings.MainSingleOrDir = LOADDIR) and
      not(Settings.BKSingleOrDir = LOADDIR)) and (not(TopStrLoadFromFile) and
      not(BottomStrLoadFromFile)) then
         UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  end;


   // Check to see if we need to change it to black fill because they have only
   // one image to do a transition on
   if (Settings.TransFillColor = ERASE_WITH_NEXT_IMAGE) and Settings.TransOnOff then begin
     if not(BackDirLoad) and not(MainDirLoad) and not(Settings.ScreenDesignOnOff) and
       (not(Settings.RandomBackgrnds) or (Settings.BKSingleOrDir = LOAD)) then
       Settings.TransFillColor := ERASE_FILL_BLACK;

     // Check MainDirList and all its possibilities
     if assigned(MainDirList) then begin
        if assigned(BackDirList) then begin
           if (MainDirList.Count = 1) and (BackDirList.Count = 1) then
              Settings.TransFillColor := ERASE_FILL_BLACK;
        end else
           if MainDirList.Count = 1 then
             Settings.TransFillColor := ERASE_FILL_BLACK;
     end;

      // Check BackDirList and all its possibilities
     if assigned(BackDirList) then begin
        if assigned(MainDirList) then begin
           if (MainDirList.Count = 1) and (BackDirList.Count = 1) then
              Settings.TransFillColor := ERASE_FILL_BLACK;
        end else
           if BackDirList.Count = 1 then
             Settings.TransFillColor := ERASE_FILL_BLACK;
     end;

     if Settings.ScreenDesignOnOff and (TotalScr = 1) then begin
       Settings.TransFillColor := ERASE_FILL_BLACK;
       pScreenD^.TransFillColor := ERASE_FILL_BLACK;
     end;
   end;


   {Set delay if need be}
   case Settings.TransSpeed of
      0: SleepTime := 0;
      1: SleepTime := 2;//5;
      2: SleepTime := 4;//20;
      3: SleepTime := 8;//50;
      4: SleepTime := 16;//80;
      5: SleepTime := 32;//120;
      else SleepTime := 0;
   end;

   // Setup sound object for Shareware Sounds
   //Load Logo Artwork from resource gif
   if SharewareUser and (BigCanvas or DEBUGIT) then begin
     theLogo := TBitmap.Create;
     GifLoadFromResource( 'logo', theLogo );
     
     //if ColorMode256 then
       //hSharePalette := theLogo.ReleasePalette;
   end;

   {Set sleeper variable}
   if SleepTime = 0 then
      sleeper := false
   else
      sleeper := true;

   // Init Transitions and allocate memory
   if Settings.TransOnOff then begin

     tempHorz := ScreenW div (SMALLEST_SQUARE_SIZE - 2); // Add one to be safe
     tmpVer := ScreenH div (SMALLEST_SQUARE_SIZE - 2); // Add one to be safe

     //Add one more if there is a remainder
     if (ScreenW mod (SMALLEST_SQUARE_SIZE - 2)) <> 0 then
           Inc(tempHorz);

     //Add one more if there is a remainder
     if (ScreenH mod (SMALLEST_SQUARE_SIZE - 2)) <> 0 then
           Inc(tmpVer);

     // Alocate memory for the TPoint & sort
     TotalSquAlloc := tempHorz * tmpVer;
     PaintSquares := allocmem(SizeOf(TPoint) * TotalSquAlloc);
     sort := allocmem(SizeOf(Integer) * TotalSquAlloc);
     if (PaintSquares = nil) or (sort = nil) then begin // Allocation Failed
       TotalSquAlloc := 0;
       Raise SaverException.Create('Memory Allocation Failed!');
       Terminate;
     end;
     randWH.W := random(SQUARE_SIZE_RAND)+SMALLEST_SQUARE_SIZE;
     randWH.H := random(SQUARE_SIZE_RAND)+SMALLEST_SQUARE_SIZE;
     PSSize := CalculateSquares( PaintSquares, randWH );
     tempPS := PaintSquares;
   end;

   Canvas.Brush.Color := clBlack; // Set the canvas brush

  SquareTime := timeGetTime;
  ShareDelayTime := SquareTime;// + SHAREWARE_MESSAGE_DELAY;
  DelayTimer := SquareTime;

  {initialize the thread as starting immediately}
  inherited Create (false);

  {and now initialize some thread properties to the appropriate values}
  //FreeOnTerminate := True;
end; // Create

{-----------------------------------------------------------------------}

procedure TSaverBasics.Execute;
var W: Integer;
    delay: DWORD;
begin
  if assigned (Canvas) then begin {do nothing if there is no canvas to draw to}
    while not Terminated do begin
      timerTmp := timeGetTime;
      if SquareTime < timerTmp then begin
        W := randWH.W * randWH.H;
        case W of
          2166..2600: delay := 5;
          1735..2165: delay := 4;
          1300..1734:  delay := 3;
          867..1299:   delay := 2;
          434..866:   delay := 1;
          0..433:     delay := 0;
          else        delay := 0;
        end;

        SquareTime := timerTmp + delay + SleepTime;
        Synchronize (AStep);
      end;
     end;
  end;

end; // Execute

{-----------------------------------------------------------------------}

procedure TSaverBasics.ScreenDesignerImages;
var  SoundMode: Integer;
     pScreenDTmp: PTScreenDesign;
begin

  pScreenD := pScrBase;

  if ((Settings.ScreenDesignRandom = SCREEN_DESIGNER_RANDOM) and (CurrentScr = 0)) then
    RandomizeScreens( pScreenD, TotalScr );

  inc(pScreenD, CurrentScr);
  CopySettings( Settings, pScreenD );
    
  //DeleteThePalettes;

  LoadMainImage( Settings, ArtBMP, ArtWMF, NO_ERROR_MSG );
  if Terminated then exit; // Quick exit if thread terminated
  LoadBackImage( Settings, backBMP, backWMF, NO_ERROR_MSG );

  if Terminated then exit; // Quick exit if thread terminated
  UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);

  if (pScreenD^.SoundOnOff or Settings.SoundInterrupt) and Settings.SoundSDOnOff and BigCanvas then begin

    SoundMode := frmMID.GetPlayerStatus;

    if not(SoundMode = NotReady) then begin // Let it go if its not ready

      if not(Settings.SoundInterrupt) then begin // Sound gets interrupted
        if SoundMode > Closed then begin
          frmMID.Stop;
          frmMID.Close;
        end;

        if FileExists( pScreenD^.SoundFile ) then begin
           try
              frmMID.Play( pScreenD^.SoundFile );
           except
              Settings.SoundFileLoadErrorStr := ERUPF + pScreenD^.SoundFile;
           end;
        end else
           Settings.SoundFileLoadErrorStr := ERSF + pScreenD^.SoundFile;
        
        inc(SoundFileCount);
      end else if assigned(SoundDirList) then begin  // Sound does not get interrupted
        if ((SoundMode = Closed) or not(SoundMode = Playing)) then begin

          if (Settings.ScreenDesignRandom = SCREEN_DESIGNER_RANDOM) and (SoundFileCount = 0) then
            RandomizeList( SoundDirList );

          pScreenDTmp := pScrBase;
          inc(pScreenDTmp, SoundFileCount);

          if SoundMode > Closed then begin
            frmMID.ClosePlayer;
          end;

          if FileExists( SoundDirList.Strings[SoundFileCount] ) then begin
             try
                frmMID.Play( SoundDirList.Strings[SoundFileCount] );
             except
                Settings.SoundFileLoadErrorStr := ERUPF + SoundDirList.Strings[SoundFileCount];
             end;
          end else
             Settings.SoundFileLoadErrorStr := ERSF + SoundDirList.Strings[SoundFileCount];

          inc(SoundFileCount);
          if SoundFileCount >= SoundDirList.Count then
            SoundFileCount := 0;
        end;
      end;
      
    end;
  end;

  // Don't let them erase with next image if system has changed
  if (ColorMode256 and (Settings.TransFillColor = ERASE_WITH_NEXT_IMAGE)) then
     Settings.TransFillColor := ERASE_FILL_BLACK;

  inc(CurrentScr);
  if CurrentScr >= TotalScr then CurrentScr := 0;

  //if not NoImageERROR then
    //Settings.ScreenDesignOnOff := False;
end; // ScreenDesignerImages

{-----------------------------------------------------------------------}

procedure TSaverBasics.LoadFilesFromHardDrive;
var SoundMode: Integer;
begin

  if TopStrLoadFromFile then begin
    // Send the list off to be sorted into a new order
    if Settings.TopTextRandom and (TopStrCount = 0) then
       RandomizeList( TopStrList );
    Settings.TopMessage := TopStrList.Strings[TopStrCount];
    inc(TopStrCount);
    if TopStrCount >= TopStrList.Count then TopStrCount := 0;
  end;

  if BottomStrLoadFromFile then begin
    // Send the list off to be sorted into a new order
    if Settings.BottomTextRandom and (BottomStrCount = 0) then
       RandomizeList( BottomStrList );
    Settings.BottomMessage := BottomStrList.Strings[BottomStrCount];
    inc(BottomStrCount);
    if BottomStrCount >= BottomStrList.Count then BottomStrCount := 0;
  end;

  if BackDirLoad then begin

    {if (BackPalette <> 0) and ColorMode256 then begin
       DeleteObject(BackPalette);
       BackPalette := 0;
    end;}

    // Send the list off to be sorted into a new order
    if (Settings.BKDirRandomSort = RANDOM_FILES_IN_DIR) and (BackImageCount = 0) then
        RandomizeList( BackDirList );

    if Terminated then exit; // Quick exit if thread terminated
    Settings.BKImage := BackDirList.Strings[BackImageCount];
    LoadBackImage( Settings, backBMP, backWMF, NO_ERROR_MSG );

    {if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
      if backBMP.PixelFormat = pf8bit then
       BackPalette := backBMP.ReleasePalette;}

    inc(BackImageCount);
    if BackImageCount >= BackDirList.Count then BackImageCount := 0;
  end else if (Settings.BkFileType = RESOURCE) and Settings.RandomBackgrnds then begin
      // Randomly loads background images
      GifLoadFromResource( 'Back'+ IntToStr(Random(39)+1), backBMP );
  end;

  if MainDirLoad then begin

    {if (ArtPalette <> 0) and ColorMode256 then begin
       DeleteObject(ArtPalette);
       ArtPalette := 0;
    end;}

    // Send the list off to be sorted into a new order
    if (Settings.MainDirRandomSort = RANDOM_FILES_IN_DIR) and (MainImageCount = 0) then
       RandomizeList( MainDirList );

    Settings.MainImage := MainDirList.Strings[MainImageCount];

    if Terminated then exit; // Quick exit if thread terminated
    LoadMainImage( Settings, ArtBMP, ArtWMF, NO_ERROR_MSG );

    {if (Settings.FileType > WMF) and ColorMode256 then
      if ArtBMP.PixelFormat = pf8bit then
       ArtPalette := ArtBMP.ReleasePalette;}

    inc(MainImageCount);
    if MainImageCount >= MainDirList.Count then MainImageCount := 0;
  end;

  if MainDirLoad or BackDirLoad or TopStrLoadFromFile or BottomStrLoadFromFile or
     ((Settings.BkFileType = RESOURCE) and Settings.RandomBackgrnds) then begin
    if Terminated then exit; // Quick exit if thread terminated
    UpdateBuffer(buffer, backBMP, artBMP, backWMF, artWMF, Settings);
  end;

  //if not NoImageERROR then begin
  //    MainDirLoad := False;
  //    BackDirLoad := False;
  //end;

  // Play the sound files of the directory they've choosen
  if assigned( SoundDirList ) then begin
    if Settings.SoundRandList and (SoundFileCount = 0) then
       RandomizeList( SoundDirList );

    SoundMode := frmMID.GetPlayerStatus;

    if not(SoundMode = NotReady) then begin // Let it go if its not ready

      if not(Settings.SoundInterrupt) then begin // Sound gets interrupted
        if SoundMode > Closed then begin
          frmMID.ClosePlayer;
        end;

        if FileExists( SoundDirList.Strings[SoundFileCount] ) then begin
           try
              frmMID.Play( SoundDirList.Strings[SoundFileCount] );
           except
              Settings.SoundFileLoadErrorStr := ERUPF + SoundDirList.Strings[SoundFileCount];
           end;
        end else
           Settings.SoundFileLoadErrorStr := ERSF + SoundDirList.Strings[SoundFileCount];
        
        inc(SoundFileCount);
      end else begin                    // Sound does not get interrupted
        if ((SoundMode = Closed) or not(SoundMode = Playing)) then begin

          if SoundMode > Closed then begin
            frmMID.ClosePlayer;
          end;

          if FileExists( SoundDirList.Strings[SoundFileCount] ) then begin
             try
                frmMID.Play( SoundDirList.Strings[SoundFileCount] );
             except
                Settings.SoundFileLoadErrorStr := ERUPF + SoundDirList.Strings[SoundFileCount];
             end;
          end else
             Settings.SoundFileLoadErrorStr := ERSF + SoundDirList.Strings[SoundFileCount];

          inc(SoundFileCount);
        end;
      end;
      
      if SoundFileCount >= SoundDirList.Count then SoundFileCount := 0;

    end;
  end;
end; // LoadFilesFromHardDrive

{-----------------------------------------------------------------------}

procedure TSaverBasics.AStep;
begin

if ForceRect.Bottom = 0 then Begin {Check Forced rect for password window}

  {For Shareware message}
 if (ShareDelayTime < timerTmp) and Assigned(theLogo) and
    (SquCount = 0) and (PaintPicture = true) then begin

     if OnceThrough then begin
        SharewareMessage( canvas, theLogo, buffer );
     end else begin
        OnceThrough := True;
        // Black out the screens for duel monitors
        if (VirtualScreenWidth > ScreenW) or (VirtualScreenHeight > ScreenH) then begin
           r := Rect(0,0,VirtualScreenWidth,VirtualScreenHeight);
           Canvas.FillRect(r);
        end;
     end;

     ShareDelayTime := timeGetTime + SHAREWARE_MESSAGE_DELAY; {reset time variable}
 end;

  if Settings.TransOnOff then begin
       // When SquCount is = or > PSSize we painted all the squares
       if (SquCount < PSSize) then begin
         if PaintPicture then begin // Painting in users image

           // Load another image and realize the palette if need be
           if (SquCount = 0) then begin
             if Settings.ScreenDesignOnOff then
               ScreenDesignerImages
             else
               LoadFilesFromHardDrive;

             //Time to realize the palettes
             if ColorMode256 then
               LoadBitmapPalettes( Canvas, artBMP.Palette, BackBMP.Palette, Settings );
           end;

           r := GetRectInScreen(tempPS^.X, tempPS^.Y, randWH.W, randWH.H);
           //r := Rect(tempPS^.X, tempPS^.Y,
           //  tempPS^.X + randWH.W, tempPS^.Y + randWH.H);

           // do things differently for small display window
           if SmallCanvas or DEBUGIT then begin

             if FAST_RUN_DEBUG then
                smallCount := SMALL_SKIP + 1
             else
                 smallBuffer.Canvas.CopyRect(r, buffer.Canvas, r);
                
             if SquCount = (PSSize - 1) then
                smallCount := SMALL_SKIP + 2;

             if smallCount > SMALL_SKIP then begin
               smallCount := 0;

               r := Rect(0,0,width,height);
               //copy from work area to canvas
               if FAST_RUN_DEBUG then begin // Fast test model
                 SquCount := PSSize;
                 DelayTimer := timerTmp;
                 PaintPicture := false;
                 Inc(FastCounter);
                 canvas.brush.color := clRed;
                 Canvas.FillRect(Rect(5,35,width,canvas.textheight('T')));
                 Canvas.FillRect(Rect(5,50,width,canvas.textheight('T')));
                 canvas.textout( 5, 5, ' ' + intToStr(FastCounter) + ' ');
                 canvas.textout( 5, 20, ' ' + Settings.MainImage);
                 canvas.textout( 5, 35, ' ' + Settings.BKImage);
               end else begin
                 Canvas.StretchDraw(r, smallBuffer);
               end;
             end;
           end else
                Canvas.CopyRect(r, buffer.Canvas, r);

           // Set the time to it's delay
           if SquCount = PSSize-1 then begin
             DelayTimer := timerTmp + (Settings.TransDelay * 1000);
           end;

         end else begin // Painting in black squares

           //r := Rect(tempPS^.X, tempPS^.Y,
             //tempPS^.X + randWH.W, tempPS^.Y + randWH.H);

           r := GetRectInScreen(tempPS^.X, tempPS^.Y, randWH.W, randWH.H);

           // reset the timer so that there's no delay
           DelayTimer := timerTmp;

           // do things differently for display window
           if SmallCanvas or DEBUGIT then begin
             if Settings.TransFillColor = ERASE_FILL_COLORS then // Fill with random colors
               smallBuffer.Canvas.Brush.Color := RGB(Random(256),Random(256),Random(256));

             if Settings.TransFillColor = ERASE_FILL_BLACK then
               smallBuffer.Canvas.Brush.Color := clBlack;
               
             if SquCount = (PSSize - 1) then smallCount := SMALL_SKIP + 2;
             smallBuffer.Canvas.FillRect(r);
             if smallCount > SMALL_SKIP then begin
               smallCount := 0;
               r := Rect(0,0,width,height);
               //copy from work area to canvas
               Canvas.StretchDraw(r, smallBuffer);
             end;
           end else begin
             if Settings.TransFillColor = ERASE_FILL_COLORS then // Fill with random colors
               Canvas.Brush.Color := RGB(Random(256),Random(256),Random(256));

             if Settings.TransFillColor = ERASE_FILL_BLACK then
               Canvas.Brush.Color := clBlack;
             Canvas.FillRect(r); // erasing screen saver
           end;
         end;
         inc(tempPS);
         inc(SquCount);
         inc(smallCount);
       end else begin // re-init everything and create new squares, but make it wait
         if DelayTimer < timerTmp then begin
           if PaintPicture then
             PaintPicture := false
           else
             PaintPicture := true;

           // Start repainting the next picture over the last if they choose
           if Settings.TransFillColor = ERASE_WITH_NEXT_IMAGE then
             PaintPicture := true;

           randWH.W := random(SQUARE_SIZE_RAND)+SMALLEST_SQUARE_SIZE;
           randWH.H := random(SQUARE_SIZE_RAND)+SMALLEST_SQUARE_SIZE;
           PSSize := CalculateSquares(PaintSquares, randWH);

           SquCount := 0;
           tempPS := PaintSquares;
         end;
       end;
     end else begin // if Settings.TransOnOff then begin
       // for Trans off  Typical slide show saver
       if DelayTimer < timerTmp then begin
         if Settings.ScreenDesignOnOff then
           ScreenDesignerImages
         else
           LoadFilesFromHardDrive;
         r := Rect(0,0,width,height);
         if SmallCanvas or DEBUGIT then begin
           if not FAST_RUN_DEBUG then
               Canvas.StretchDraw(r, buffer);
         end else
           Canvas.CopyRect(r, buffer.Canvas, r);
         DelayTimer := timerTmp + (Settings.TransDelay * 1000);
       end;
     end;

   ClearForceRect;
end else begin   // Small Preview Window
   if SmallCanvas then begin
     r := Rect(0,0,width,height);
     //copy from work area to canvas

        if Settings.TransOnOff then
           Canvas.StretchDraw(r, smallBuffer)
        else
           Canvas.StretchDraw(r, buffer);

     ClearForceRect;
   end else begin
     //If user cancels the password window, redraw the contents of the window
     r := Rect(0,0,ScreenW,ScreenH);
     if Settings.TransOnOff then begin
       if firstTimeRun then
         if not(Settings.TransFillColor = ERASE_WITH_NEXT_IMAGE) then // Don't black out screen on start-up
           Canvas.FillRect(r);
       if gPasswordDlgShown then begin // Password dlg showing
         SquCount := PSSize;
         if PaintPicture then begin
           DelayTimer := timerTmp + (Settings.TransDelay * 1000);
           Canvas.CopyRect(r, buffer.Canvas, r);
         end else begin
           Canvas.FillRect(r);
         end;
         gPasswordDlgShown := False;
       end;
     end else begin
       // The shareware screen draws the window then its not trans
       if SharewareUser then
         Canvas.FillRect(r)
       else begin
         //Time to realize the palettes
         if ColorMode256 then
           LoadBitmapPalettes( Canvas, artBMP.Palette, BackBMP.Palette, Settings );
         Canvas.CopyRect(r, buffer.Canvas, r);
       end;
     end;

     ClearForceRect;
     firstTimeRun := False; // Set the first time run flag
   end;
end;

end; // AStep

{-----------------------------------------------------------------------}

function TSaverBasics.GetRectInScreen( X, Y, W, H: Integer ):TRect;
var TmpR: TRect;
begin

  if (X + W) > ScreenW then
    TmpR.Right := X + (ScreenW-X)
  else
    TmpR.Right := X+W;

  if (Y + H) > ScreenH then
    TmpR.Bottom := Y + (ScreenH-Y)
  else
    TmpR.Bottom := Y+H;

  TmpR.Left := X;
  TmpR.Top := Y;

  result := TmpR;
end;

{-----------------------------------------------------------------------}

{This function takes a pointer to a TPoint structure, the size in bytes of the
TPoint array, the screen width and height, and the size of the square being used.
The function allocates and reallocates memory to store the array of TPoint. It
calculates random X & Y positions and sizes of all the squares and returns, in bytes
the size of the allocated memory so that it can be released.
Returns a 0 if the memory can't be allocated.
Returns a -1 if the memory in the function can't be freed}
function TSaverBasics.CalculateSquares( var PaintSquares: PTPoint; randWH: TMass ):Integer;
var
     size, HorSquares, VerSquares: Integer;
     //tmpSort, tempSortI, tempSortJ: ^Integer;
     pTemp, tempPSI, tempPSJ: PTPoint;
     animateType: Byte;
     I, J: integer;
     W, M, offset: Integer;
     inOrder, count: Boolean;
begin
     {Calculate Digital re-paint}
     HorSquares := ScreenW div randWH.W;
     //Add one more if there is a remainder
     if (ScreenW mod randWH.W) <> 0 then
           Inc(HorSquares);
     VerSquares := ScreenH div randWH.H;
     //Add one more if there is a remainder
     if (ScreenH mod randWH.H) <> 0 then
           Inc(VerSquares);

     result := -1;
     count := true;
     // Alocate memory for the TPoint & sort
     size := HorSquares * VerSquares;

     tempPSI := PaintSquares;
     tempSortI := sort;

     w := 0;
     M := size;
     animateType := Random(6);
     for I := 0 to VerSquares-1 do begin
       for J := 0 to HorSquares-1 do begin

         if Terminated then exit; // Quick exit if thread terminated

         tempPSI^.X := J * randWH.W;
         tempPSI^.Y := I * randWH.H;
         case animateType of
           // Random placement of squares
           0: tempSortI^ := Random(90000);
           //Diagional sweep
           1: tempSortI^ := I + J;
           2: begin //solid sweep
             if count then begin
               count := false;
               tempSortI^ := I + J + size;
             end else begin
               count := true;
               tempSortI^ := I + J;
             end;
           end;
           3: begin //Checkered sweep
               if count then begin
                 count := false;
                 tempSortI^ := size - w;
               end else begin
                 count := true;
                 tempSortI^ := w + size;
               end;
           end;
           4: begin // center to top shift
             if w > (size div 2) then begin
               dec(M);
               if count then begin
                 count := false;
                 tempSortI^ := M + size;
               end else begin
                 count := true;
                 tempSortI^ := size - M;
               end;
             end else begin
               inc(M);
               if count then begin
                 count := false;
                 tempSortI^ := M + size;
               end else begin
                 count := true;
                 tempSortI^ := size - M;
               end;
             end;
           end;
          5: begin // Top & bottom shift
             if w > (size div 2) then begin
               inc(M);
               if count then begin
                 count := false;
                 tempSortI^ := M + size;
               end else begin
                 count := true;
                 tempSortI^ := size - M;
               end;
             end else begin
               dec(M);
               if count then begin
                 count := false;
                 tempSortI^ := M + size;
               end else begin
                 count := true;
                 tempSortI^ := size - M;
               end;
             end;
           end;
         end;
         inc(tempSortI);
         inc(tempPSI);
         inc(W);
       end;
     end;
     Dec(W);

     // pointers that pass memory around
     try
       New(pTemp);
       New(tmpSort);
     except  // Get out if there is a memory free error
       exit; // by getting out here, the function returns a -1
     end;

     // Randomize how it will sort
     if Random(51) > 25 then
       count := false
     else
       count := true;

     {Now lets sort it which will put all the squares out of order}
     inOrder := false;
     offset := W;
     Repeat
       offset := (8 * offset) div 11;
       If offset = 0 Then
         offset := 1;
       inOrder := True;
       J := offset;
       tempSortI := sort;
       tempSortJ := sort;
       tempPSI := PaintSquares;
       tempPSJ := PaintSquares;
       inc(tempSortJ, J);
       inc(tempPSJ, J);
       for I := 0 To (W - offset) do begin

         if Terminated then begin // Quick exit if thread terminated
           Dispose(pTemp);
           Dispose(tmpSort);
           exit;
         end;

         if count then begin
           if tempSortI^ > tempSortJ^ Then begin
             inOrder := False;
             // Sort the guage
             tmpSort^ := tempSortI^;
             tempSortI^ := tempSortJ^;
             tempSortJ^ := tmpSort^;

             pTemp^ := tempPSI^;
             tempPSI^ := tempPSJ^;
             tempPSJ^ := pTemp^;
           end;
         end else begin
           if tempSortI^ < tempSortJ^ Then begin
             inOrder := False;
             // Sort the guage
             tmpSort^ := tempSortI^;
             tempSortI^ := tempSortJ^;
             tempSortJ^ := tmpSort^;

             pTemp^ := tempPSI^;
             tempPSI^ := tempPSJ^;
             tempPSJ^ := pTemp^;
           end;
         end;
         inc(tempSortI);
         inc(tempSortJ);
         inc(tempPSI);
         inc(tempPSJ);
        end;
     Until ((offset = 1) and (inOrder = true));

     try
       //FreeMem(pTemp, SizeOf(TPoint));
       //FreeMem(tmpSort, SizeOf(Integer));
       Dispose(pTemp);
       Dispose(tmpSort);
     except  // Get out if there is a memory free error
       exit; // by getting out here, the function returns a -1
     end;

   result := size;

end; // CalculateSquares

{-----------------------------------------------------------------------}

{procedure TSaverBasics.GetThePalettes;
begin

  if (Settings.FileType > WMF) and ColorMode256 then
    if ArtBMP.PixelFormat = pf8bit then begin
      //ArtPalette := ArtBMP.ReleasePalette;
      //ArtPalette := ArtBMP.Palette;
    end;

  if ((Settings.BkFileType > WMF) or (Settings.BkFileType = RESOURCE)) and ColorMode256 then
    if backBMP.PixelFormat = pf8bit then begin
      //BackPalette := backBMP.ReleasePalette;
      //BackPalette := backBMP.Palette;
    end;
end; // GetThePalettes }

{-----------------------------------------------------------------------}

{procedure TSaverBasics.DeleteThePalettes;
begin

  if (ArtPalette <> 0) and ColorMode256 then begin
    //DeleteObject(ArtPalette);
    //ReleaseObject(ArtPalette);
    //ArtPalette := 0;
  end;

  if (BackPalette <> 0) and ColorMode256 then begin
    //DeleteObject(BackPalette);
  end;

end; // GetThePalettes }

{-----------------------------------------------------------------------}

procedure TSaverBasics.SetForceRect (ARect : TRect);
begin {only enlare this rectangle, never shrink it}
  if FForceRect.Left > ARect.Left then
    FForceRect.Left := ARect.Left;
  if FForceRect.Top > ARect.Top then
    FForceRect.Top := ARect.Top;
  if FForceRect.Right < ARect.Right then
    FForceRect.Right := ARect.Right;
  if FForceRect.Bottom < ARect.Bottom then
    FForceRect.Bottom := ARect.Bottom;
end;

{-----------------------------------------------------------------------}

procedure TSaverBasics.ClearForceRect;
begin
    FForceRect := Rect (0, 0, 0, 0)
end;

{-----------------------------------------------------------------------}

{Part 3 implementation of the preview window for the TSaverThread clas}

var
  MyThread : TSaverBasics;
  PrevRect : TRect;

{this is the window-proc for the preview window, it only implements 3 messages}
function MyWndProc (Wnd : HWnd; Msg : Integer; wParam : Word; lParam : Integer) : Integer; far; stdcall;
begin {I don't know if it is ok to quit on WM_CLOSE, but I had no problems doing so}
  if (Msg = WM_DESTROY) or (Msg = WM_CLOSE) then
    PostMessage (Wnd, WM_QUIT, 0, 0)
  else if Msg = WM_PAINT then {on paint is always the complete background now}
    MyThread.ForceRect := PrevRect;
  result := DefWindowProc (Wnd, Msg, wParam, lParam);
end;

{-----------------------------------------------------------------------}

{This executes the preview window,
 and goes in a message loop to keep it running}
procedure ExecPreview;
var
  PreviewCanvas : TCanvas;
  WndClass : TWndClass;
  DC       : hDC;
  MyWnd    : hWnd;
  Msg      : TMsg;
begin
  {create a new window class}
  with WndClass do begin
    style := CS_PARENTDC;
    lpfnWndProc := @MyWndProc;
    cbClsExtra := 0;
    cbWndExtra := 0;
    hIcon := 0;
    hCursor := 0;
    hbrBackground := 0;
    lpszMenuName := nil;
    lpszClassName := 'Delphi2ScreenSaverPreview';
  end;
  WndClass.hInstance := hInstance;
  Windows.RegisterClass (WndClass);
  {get some info on parent window}
  GetWindowRect (ParamHandle, PrevRect);
  PrevRect.Right := PrevRect.Right - PrevRect.Left;
  PrevRect.Bottom := PrevRect.Bottom - PrevREct.Top;
  PrevRect.Left := 0;
  PrevRect.Top := 0;
  {and now create the window as child of the window given in ParamHandle}
  MyWnd := CreateWindow ('Delphi2ScreenSaverPreview', 'FunSaver', WS_CHILD or WS_DISABLED or WS_VISIBLE, 0, 0, PrevRect.Right, PrevRect.Bottom,
                         ParamHandle, 0, hInstance, nil);
  {get the DC for the new created window}
  DC := GetDC (MyWnd);
  PreviewCanvas := TCanvas.Create;
  PreviewCanvas.Handle := DC;
  MyThread := TSaverBasics.Create (PreviewCanvas, PrevRect, False);
  {and now do the message loop, I think I cannot use any predifined loop here}
  while GetMessage(Msg, 0, 0, 0) do begin
    TranslateMessage(Msg);
    DispatchMessage(Msg);
  end;
  MyThread.Terminate;
  MyThread.WaitFor;
  MyThread.Free;
  PreviewCanvas.Free;
  releaseDC(MyWnd, DC);
  {I don't care about other resources here, as my app terminates immediately and
   Win95 cleans up after me}
end;

{-----------------------------------------------------------------------}

{Part 4 implementation of the password change dialog}

procedure ExecSetPwd;
var
  hLib : THandle;
  P : function (a : PChar; ParentHandle : THandle; b, c : Integer) : Integer; stdcall;
  SysDir : String;
  NewLength : Integer;
  //i : Integer;
begin
  SetLength (SysDir, MAX_PATH);
  NewLength := GetSystemDirectory (PChar (SysDir), MAX_PATH);
  SetLength (SysDir, NewLength);
  if (length (SysDir) > 0) and (SysDir [length (SysDir)] <> '\') then
    SysDir := SysDir + '\';
  hLib := LoadLibrary (PChar (SysDir + 'MPR.DLL'));
  if hLib <> 0 then begin
    P := GetProcAddress (hLib, 'PwdChangePasswordA');
    if assigned (P) then
      P ('SCRSAVE', ParamHandle, 0, 0);
    FreeLibrary (hLib);
  end;
end;

{-----------------------------------------------------------------------}

end.
