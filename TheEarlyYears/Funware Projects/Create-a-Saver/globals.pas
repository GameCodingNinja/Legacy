{This is the Windows 95 version}
unit Globals;

interface

//uses Forms, Graphics;
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs, gFunctin,
  StdCtrls, ComCtrls, Registry, savInit, ExtCtrls, Loadpref, constants, jpeg;

var
   gSettings: TScrnSavSettings;

procedure LoadHelpFile;
procedure RandomizeScreens( pS: PTScreenDesign; Count: Integer );
function loadFile(t: String; b: TBitmap; w: TMetaFile; error: Boolean): integer;
function loadSubjectList(var t: pchar; r: pchar): LongInt;
procedure stripOutText(t: pchar; tList: TComboBox; resText: TStringList; size: LongInt);
procedure tileBackground(canvas: TCanvas; obj: TObject; Width, Height, FileType: Integer);
function wrapTextToSList(PrintWidth, LeftMargin, TopMargin: Integer; t: String;
     var list: TStringList; canvas: TCanvas) : Integer;
procedure UpdateBuffer(buffer, backBMP, artBMP: TBitmap;
     backWMF, artWMF: TMetafile; Settings: TScrnSavSettings);
procedure CopySettings( Settings: TScrnSavSettings; pS: PTScreenDesign );
function LoadBackImage( Settings: TScrnSavSettings; artBMP: TBitmap;
                         artWMF: TMetafile; ErrorMsg: Boolean ): Boolean;
function LoadMainImage( Settings: TScrnSavSettings; artBMP: TBitmap;
                         artWMF: TMetafile; ErrorMsg: Boolean ): Boolean;
procedure LoadBitmapPalettes( canvas: TCanvas; ArtPalette, BackPalette: THandle; Settings: TScrnSavSettings );
function IsMaskFileBMPorGIF( PathToMask: String ): Integer;


implementation

{-----------------------------------------------------------------------}

// If successful, it returns the file type, else it returns 0;
function loadFile(t: String; b: TBitmap; w: TMetaFile; error: Boolean): integer;
var
   FileTypeTemp, I: Integer;
   tmpStr: String;
   JPGImage: TJPegImage;
begin

     result := ERROR_UNKNOWN_FORMAT;

     // Convert to upper case;
     tmpStr := t;
     for I := 1 to Length(t) do
       tmpStr[I] := UpCase(t[I]);

     //Get file path and figure out what type of file it is
     //Check it over carefully if the user chose all files
     if Pos('.BMP', tmpStr) <> 0 then
         FileTypeTemp := BMP
     else if Pos('.GIF', tmpStr) <> 0 then
         FileTypeTemp := GIF
     else if Pos('.WMF', tmpStr) <> 0 then
         FileTypeTemp := WMF
     else if Pos('.EMF', tmpStr) <> 0 then
         FileTypeTemp := WMF
     else if Pos('.JPG', tmpStr) <> 0 then
         FileTypeTemp := JPG
     else if Pos('.JPEG', tmpStr) <> 0 then
         FileTypeTemp := JPG
     else begin
       if error then
         Application.MessageBox('The file you chose does not have the proper ' +
          'file extension(*.BMP, *.GIF, *.JPG, *.EMF, or *.WMF). The file may not be a Windows graphics file.',
           'There has been an error.', mb_OK + MB_ICONEXCLAMATION);

       exit;
     end;

     result := ERROR_FILE_MISSING;

     // Check to see if the file is there
     if not(FileExists(t)) then begin
        if error then
          Application.MessageBox(PChar('The file ' + t + ' is missing.'),
          'Can Not Find File', mb_OK + MB_ICONEXCLAMATION );

        exit;
      end;

     result := ERROR_BAD_DATA;

     // Lets load the file and make sure it's ok
     case FileTypeTemp of
     BMP: begin
       try
           b.LoadFromFile(t);
       except
         if error then begin
           Application.MessageBox('There has been an error in trying to load your' +
           ' file from disk. The file may be damaged or is not a true' +
           ' Windows bitmap file format.', 'There has been an error.', mb_OK +
           MB_ICONEXCLAMATION);
         end;
         exit;
       end;
     end;

     GIF: begin
       FileTypeTemp := BMP;
       try
          GifLoadFromFile( t, b );
       except
         if error then begin
           Application.MessageBox('There has been an error in trying to load your' +
           ' file from disk. The file may be damaged or is not a true' +
           ' Windows GIF file format.', 'There has been an error.', mb_OK +
           MB_ICONEXCLAMATION);
         end;
         exit;
       end;
     end;

     JPG: begin
       JPGImage := TJPegImage.Create;
       FileTypeTemp := BMP;
       try
          JPGImage.LoadFromFile( t );
          if not b.Empty then begin
            b.Dormant;
            b.FreeImage;
          end;
          b.Assign( JPGImage );
       except
         if error then begin
           Application.MessageBox('There has been an error in trying to load your' +
           ' file from disk. The file may be damaged or is not a true' +
           ' Windows JPG file format.', 'There has been an error.', mb_OK +
           MB_ICONEXCLAMATION);
         end;
         JPGImage.free;
         exit;
       end;
       JPGImage.free;
     end;

     WMF: begin
       try
           w.Clear;
           w.LoadFromFile(t);
       except
         if error then begin
           Application.MessageBox('There has been an error in trying to load your' +
           ' file from disk. The file may be damaged or is not a true' +
           ' Windows Metafile.', 'There has been an error.', mb_OK +
           MB_ICONEXCLAMATION);
         end;
       exit;
       end;
       end;
     end;

     result := FileTypeTemp; // if alls ok, return the file type

end; // loadFile

{-----------------------------------------------------------------------}

function loadSubjectList(var t: pchar; r: pchar): LongInt;
var
   //resFind, resLoad: THandle;
   //resSize,
   I: LongInt;
   temp: pchar;
   cRes: TCustomRes;
begin
     {resFind := FindResource(HInstance, r, 'LST');
     if resFind = 0 then
     begin
         result := 0;
         exit;
     end;
     resLoad := LoadResource(HInstance, resFind);
     if resLoad = 0 then
     begin
         result := 0;
         exit;
     end;
     resSize := SizeOfResource(HInstance, resFind);

     temp := pchar(LockResource(resLoad));
     if temp = nil then
     begin
         result := 0;
         FreeResource(resLoad);
         exit;
     end;}

     // Load the custom resource and return a structure of it's pointer and size
     cRes := LoadCustomResource( r, 'LST' );
     if cRes.pRes = nil then exit;

     // Allocate memory for string, add 1 for terminator
     t := StrAlloc(cRes.size + 1);

     // Typecast to a pchar
     temp := pChar(cRes.pRes);

     // Copy contents of string into allocated string
     for I := 0 to cRes.size - 1 do
       t[I] := temp[I];

     t[cRes.size] := char(0); // finish off with null terminator

     // Clear out the memory
     //UnlockResource(resLoad);
     //FreeResource(resLoad);

     result := cRes.size + 1;

end;  // loadSubjectList

{-----------------------------------------------------------------------}

procedure stripOutText(t: pchar; tList: TComboBox; resText: TStringList; size: LongInt);
var
     I, J: LongInt;
     temp: array[0..100] of char;
     txtFlag: Boolean;
begin
     I := 0; J := 0;

   repeat

       // Name that shows up in list
       if (t[I] = '@') or (t[I] = '#') then begin

         if t[I] = '@' then
           txtFlag := true
         else
           txtFlag := false;
         inc(I);
         repeat
           temp[J] := t[I];
           inc(I);
           inc(J);
         until (t[I] = char(13)) or (t[I] = char(0)); 
         temp[J] := char(0); // add terminator
         j := 0;
         if txtFlag then
           tList.Items.Add(temp)
         else
           resText.Add(temp);
       end;

       inc(I);
   until I >= size;
end;

{-----------------------------------------------------------------------}

procedure tileBackground(canvas: TCanvas; obj: TObject; Width, Height, FileType: Integer);
var paintX, paintY, I, J, W, H: Integer;
begin
     // Exit if something went wrong
     if Width <= 0 then exit;
     if Height <= 0 then exit;

     if FileType = BMP then begin
       W := (obj as TBitmap).Width;
       H := (obj as TBitmap).Height;
     end else begin
       W := (obj as TMetaFile).Width;
       H := (obj as TMetaFile).Height;
     end;

     // Exit if something went wrong
     if W <= 0 then exit;
     if H <= 0 then exit;

     {Calculate background Tiling, if any}
     if W < Width then begin
       if (Width mod W) = 0 then
         paintX := Width div W
       else
         paintX := Width div W + 1;
     end else
         paintX := 1;

     if H < Height then begin
       if (Height mod H) = 0 then
         paintY := Height div H
       else
         paintY := Height div H + 1;
     end else
       paintY := 1;

     {Tile the background}
     for I := 0 to paintY-1 do begin
       for J := 0 to paintX-1 do begin
         if FileType = BMP then
           canvas.Draw(J*W, I*H, (obj as TBitmap))
         else
           canvas.Draw(J*W, I*H, (obj as TMetaFile));
       end;
     end;
end;

{-----------------------------------------------------------------------}

// Returns the number of lines in a stringlist
Function wrapTextToSList(PrintWidth, LeftMargin, TopMargin: Integer; t: String;
     var list: TStringList; canvas: TCanvas) : Integer;
var
    W, J, TxtLgth, {StepIt, X,} NoOfLines: Integer;
    LineOfTxt, currWord, Temp: String;

const
    RETURN = #13;
    SPACE = #32;
    FAKE_RETURN = #124;
begin

result := 0;
list.Clear;
NoOfLines := 0;
TxtLgth := Length(t);
W := 1;

//repeat
while W < TxtLgth+1 do begin

    currWord := '';  {Clear out current word}
    J := 1;
    if W > TxtLgth then break;

    while (t[W] > SPACE) do begin
       if t[W] = FAKE_RETURN then break; // Fake return
       SetLength(currWord, J);
       currWord[J] := t[W];
       inc(W); inc(J);
       if W > TxtLgth then break;
    end;

    temp := LineOfTxt + currWord;

    if canvas.TextWidth(temp) >= PrintWidth then begin
       //canvas.TextOut(LeftMargin ,x, LineOfTxt);
       list.Add(Trim(LineOfTxt)); // Trim off any leading or trailing char
       inc(NoOfLines);
       LineOfTxt := '';
       //inc(x, Stepit);
    end;

    LineOfTxt := LineOfTxt + currWord;
    if W > TxtLgth then break;

    // Process whitespace and any control characters
    while (t[W] <= SPACE) or (t[W] = FAKE_RETURN) do begin
        if W > TxtLgth then break;
            case t[W] of
                SPACE: LineOfTxt := LineOfTxt + ' ';
                RETURN, FAKE_RETURN: Begin
                    list.Add(LineOfTxt);
                    inc(NoOfLines);
                    LineOfTxt := '';
                    end;
            end;
        inc(W);
        if W > TxtLgth then break;
    end;

end;
   if length(LineOfTxt) > 1 then begin
       list.Add(LineOfTxt);
       inc(NoOfLines);
      //canvas.TextOut(LeftMargin ,X ,LineOfTxt);
   end;

    result := NoOfLines;

end;

{-----------------------------------------------------------------------}

procedure UpdateBuffer(buffer, backBMP, artBMP: TBitmap;
     backWMF, artWMF: TMetafile; Settings: TScrnSavSettings);
var
     r: TRect;
     list: TStringList;
     tempBMP: TBitmap;
     linesOfText, I, W, topTextHeight, pictureTop, pictureBottom, ErrorTxtY,
     bottomTextHeight, TextX, lineSpacing, X, Y, MarginXY, MaskType: Integer;
     tempStr: String;
     SizeBigBMP: Boolean;
begin

   tempStr := '';
   SizeBigBMP := FALSE;

   // Setup margins depending on screen rez
   MarginXY := 15;
   TextX := MarginXY;
   if Screen.Width <= 800 then MarginXY := 20;
   if Screen.Width >= 1024 then MarginXY := 25;

   list := TStringList.Create;

   // Paint the background white
   r := Rect(0,0,Screen.Width,Screen.Height);
   buffer.canvas.brush.color := clWhite;
   buffer.canvas.FillRect(r);

   //Lets paint the background
   case Settings.BkFileType of
     RESOURCE, BMP:
       tileBackground(buffer.canvas, backBMP, buffer.Width, buffer.Height, BMP);
     WMF:
       tileBackground(buffer.canvas, backWMF, buffer.Width, buffer.Height, WMF);
     COLORS: begin
         r := Rect(0,0,Screen.Width,Screen.Height);
         buffer.canvas.brush.color := Settings.BKFillColor;
         buffer.canvas.FillRect(r);
     end;
   end;

   // See if we need to size the image to the screen because it is too big
   if (Settings.FileType = BMP) then begin
      if (artBMP.Width > Screen.Width) or (artBMP.Height > Screen.Height) then
         SizeBigBMP := true;
   end;

    // If we're loading up their BMP, just center it on the screen
    if (Settings.FileType = BMP) and not(Settings.SizeBitmaps) and not(SizeBigBMP) then begin
       if artBMP.Width >= Screen.Width then
         X := 0
       else
         X := (Screen.Width - artBMP.Width) div 2;
       if artBMP.Height >= Screen.Height then
         Y := 0
       else
         Y := (Screen.Height - artBMP.Height) div 2;

       // Check for a mask file
       tempStr := Settings.MainImage;
       // Delete the last three charaters of the string
       delete(tempStr, Length(tempStr)-2, 3);
       tempStr := tempStr + 'msk';
       if FileExists( tempStr ) then begin
         tempBMP := TBitmap.Create;

         MaskType := IsMaskFileBMPorGIF( tempStr );

         if MaskType = BMP then begin
           try
              tempBMP.LoadFromFile( tempStr );
           except
           end;
         end else if MaskType = GIF then begin
           try
              GifLoadFromFile( tempStr, tempBMP );
           except
           end;
         end;

         if not tempBMP.Empty then begin
           buffer.Canvas.CopyMode := cmSrcAnd;
           buffer.canvas.Draw(X,Y,tempBMP);
           buffer.Canvas.CopyMode := cmSrcPaint;
           buffer.canvas.Draw(X,Y,artBMP);
           buffer.Canvas.CopyMode := cmSrcCopy;
         end else
           buffer.canvas.Draw(X,Y,artBMP);

         tempBMP.free;
       end else
         buffer.canvas.Draw(X,Y,artBMP);
    end;

   pictureTop := MarginXY;//10; //init it here incase they don't have Top text

  //Setup Top Font
  //Lets add the font info to the buffer canvas
  if Settings.TopMessage <> '' then begin
     buffer.canvas.Font.Name := Settings.TopFontName;
     buffer.canvas.Font.Size := Settings.TopFontSize;
     buffer.canvas.Font.Color := Settings.TopFontColor;
     buffer.canvas.brush.style := bsClear;
     // Clear it out first
     buffer.canvas.Font.Style := [];
     if (Settings.TopFontStyleBold) then
       buffer.canvas.Font.Style := [fsBold];
     if (Settings.TopFontStyleItal) then
       buffer.canvas.Font.Style := buffer.canvas.Font.Style + [fsItalic];
     if (Settings.TopFontStyleUnder) then
       buffer.canvas.Font.Style := buffer.canvas.Font.Style + [fsUnderline];
     if (Settings.TopFontStyleStrike) then
       buffer.canvas.Font.Style := buffer.canvas.Font.Style + [fsStrikeout];

     linesOfText := wrapTextToSList(Screen.Width - (MarginXY*2), 0, 0, Settings.TopMessage,
       list, buffer.canvas);

     topTextHeight := buffer.canvas.TextHeight('T');
     lineSpacing := (topTextHeight div 12);
     W := 0;

     // Paint text to buffer
     for I := 0 to linesOfText-1 do begin
       case Settings.TopTextPos of
         LEFT_JUST: TextX := MarginXY;
         CENTER_TEXT: begin
           TextX := buffer.canvas.TextWidth(list.Strings[I]);
           TextX := (Screen.Width - TextX) div 2;
         end;
         RIGHT_JUST: begin
           TextX := buffer.canvas.TextWidth(list.Strings[I]);
           TextX := (Screen.Width - TextX) - MarginXY;
         end;
       end;
       buffer.canvas.TextOut(TextX,lineSpacing+W,list.Strings[I]);
       Inc(W, topTextHeight);
       if W > Screen.Height then begin
         list.free;
         exit; // stop if we are outside of the screen
       end;
     end;
     pictureTop := lineSpacing + W + MarginXY; // Get value for picture placement
   end;

   list.clear;
   pictureBottom := Screen.Height - MarginXY;  //init it here incase they don't have Bottom text

  //Setup Bottom Font
  //Lets add the font info to the buffer canvas
  if Settings.BottomMessage <> '' then begin
     buffer.canvas.Font.Name := Settings.BottomFontName;
     buffer.canvas.Font.Size := Settings.BottomFontSize;
     buffer.canvas.Font.Color := Settings.BottomFontColor;
     buffer.canvas.brush.style := bsClear;
     // Clear it out first
     buffer.canvas.Font.Style := [];
     if (Settings.BottomFontStyleBold) then
       buffer.canvas.Font.Style := [fsBold];
     if (Settings.BottomFontStyleItal) then
       buffer.canvas.Font.Style := buffer.canvas.Font.Style + [fsItalic];
     if (Settings.BottomFontStyleUnder) then
       buffer.canvas.Font.Style := buffer.canvas.Font.Style + [fsUnderline];
     if (Settings.BottomFontStyleStrike) then
       buffer.canvas.Font.Style := buffer.canvas.Font.Style + [fsStrikeout];

     linesOfText := wrapTextToSList(Screen.Width - (MarginXY*2), 0, 0, Settings.BottomMessage,
       list, buffer.canvas);

     bottomTextHeight := buffer.canvas.TextHeight('T');
     lineSpacing := (bottomTextHeight div 12);
     W := (bottomTextHeight * linesOfText) + (lineSpacing * linesOfText);
     W := Screen.Height - W - MarginXY;

     pictureBottom := W;  // Get value for picture placement

     // Paint text to buffer
     for I := 0 to linesOfText-1 do begin
       case Settings.BottomTextPos of
         LEFT_JUST: TextX := MarginXY;
         CENTER_TEXT: begin
           TextX := buffer.canvas.TextWidth(list.Strings[I]);
           TextX := (Screen.Width - TextX) div 2;
         end;
         RIGHT_JUST: begin
           TextX := buffer.canvas.TextWidth(list.Strings[I]);
           TextX := (Screen.Width - TextX) - MarginXY;
         end;
       end;
       buffer.canvas.TextOut(TextX,lineSpacing+W,list.Strings[I]);
       Inc(W, bottomTextHeight);
       if W > Screen.Height then begin
         list.free;
         exit; // stop if we are outside of the screen
       end;
     end;
  end;

   //Place picture between text    (Settings.FileType = RESOURCE) or (Settings.FileType = WMF)
    if not(Settings.FileType = NONE) then begin
       // Make sure the size of the image is worth it
       // and that it's not a negative number
       if (Settings.FileType = RESOURCE) or (Settings.FileType = WMF) then begin
          //if pictureBottom-pictureTop > MarginXY then begin
            r := sizeItUp(Screen.Width, pictureBottom-pictureTop, artWMF.Width, artWMF.Height);
            Inc(r.Top, PictureTop); Inc(r.Bottom, PictureTop);
            buffer.canvas.StretchDraw(r,artWMF);
          //end;
       end else if (Settings.FileType = BMP) and (Settings.SizeBitmaps or SizeBigBMP) then begin
          if (Settings.TopMessage = '') and (Settings.BottomMessage = '') then
            r := sizeItUp(Screen.Width, Screen.Height, artBMP.Width, artBMP.Height)
          else begin
            r := sizeItUp(Screen.Width, pictureBottom-pictureTop, artBMP.Width, artBMP.Height);
            Inc(r.Top, pictureTop); Inc(r.Bottom, PictureTop); // push picture down to center it
          end;

            // Check for a mask file
            tempStr := Settings.MainImage;
            // Delete the last three charaters of the string
            delete(tempStr, Length(tempStr)-2, 3);
            tempStr := tempStr + 'msk';

            // See if there is a mask file
            if FileExists( tempStr ) then begin
              tempBMP := TBitmap.Create;

              MaskType := IsMaskFileBMPorGIF( tempStr );

              if MaskType = BMP then begin
                 try
                    tempBMP.LoadFromFile( tempStr );
                 except
                 end;
              end else if MaskType = GIF then begin
                 try
                    GifLoadFromFile( tempStr, tempBMP );
                 except
                 end;
              end;

              if not tempBMP.Empty then begin
                buffer.Canvas.CopyMode := cmSrcAnd;
                buffer.canvas.StretchDraw(r,tempBMP);
                buffer.Canvas.CopyMode := cmSrcPaint;
                buffer.canvas.StretchDraw(r,artBMP);
                buffer.Canvas.CopyMode := cmSrcCopy;
              end else begin
                buffer.canvas.StretchDraw(r,artBMP);
              end;
              tempBMP.free;
            end else
              buffer.canvas.StretchDraw(r,artBMP);
          end;
       //end;
  end;

  // Check to see if we are have file loading problems
  if not(Settings.SoundFileLoadErrorStr = '') then begin
    buffer.canvas.Font.Style := [];
    buffer.canvas.Font.Style := [fsBold];
    buffer.canvas.brush.style := bsSolid;
    buffer.canvas.brush.color := clBlack;
    buffer.canvas.Font.Name := 'MS Sans Serif';
    buffer.canvas.Font.Size := 14;
    buffer.canvas.Font.Color := clYellow;
    ErrorTxtY := 0;
    if not(Settings.MainFileLoadErrorStr = '') then
       inc( ErrorTxtY, buffer.canvas.TextHeight('T')+5 );

    if not(Settings.BackFileLoadErrorStr = '') then
       inc( ErrorTxtY, buffer.canvas.TextHeight('T')+5 );

    buffer.canvas.TextOut(0, ErrorTxtY, Settings.SoundFileLoadErrorStr+' ');
    // Clear it out just in case
    //Settings.SoundFileLoadErrorStr := '';
  end;

  // Check to see if we are have file loading problems
  if not(Settings.BackFileLoadErrorStr = '') then begin
    buffer.canvas.Font.Style := [];
    buffer.canvas.Font.Style := [fsBold];
    buffer.canvas.brush.style := bsSolid;
    buffer.canvas.brush.color := clBlack;
    buffer.canvas.Font.Name := 'MS Sans Serif';
    buffer.canvas.Font.Size := 14;
    buffer.canvas.Font.Color := clLime;
    ErrorTxtY := 0;
    if not(Settings.MainFileLoadErrorStr = '') then
       ErrorTxtY := buffer.canvas.TextHeight('T')+5;

    buffer.canvas.TextOut(0, ErrorTxtY, Settings.BackFileLoadErrorStr+' ');
    // Clear it out just in case
    //Settings.BackFileLoadErrorStr := '';
  end;

  // Check to see if we are have file loading problems
  if not(Settings.MainFileLoadErrorStr = '') then begin
    buffer.canvas.Font.Style := [];
    buffer.canvas.Font.Style := [fsBold];
    buffer.canvas.brush.style := bsSolid;
    buffer.canvas.brush.color := clBlack;
    buffer.canvas.Font.Name := 'MS Sans Serif';
    buffer.canvas.Font.Size := 14;
    buffer.canvas.Font.Color := clRED;
    buffer.canvas.TextOut(0, 0, Settings.MainFileLoadErrorStr+' ');
    // Clear it out just in case
    //Settings.MainFileLoadErrorStr := '';
  end;

  list.free;
end; // UpdateBuffer


{-----------------------------------------------------------------------}

procedure CopySettings( Settings: TScrnSavSettings; pS: PTScreenDesign );
begin
  Settings.MainImage := pS^.MainImg;
  Settings.FileType := pS^.MainFType;
  Settings.SizeBitmaps := pS^.SizeBitmaps;
  Settings.TopMessage := pS^.TopMsg;
  Settings.TopFontName := pS^.TopFont;
  Settings.TopFontSize := pS^.TopSize;
  Settings.TopFontStyleBold := pS^.TopBold;
  Settings.TopFontStyleItal := pS^.TopItal;
  Settings.TopFontStyleUnder := pS^.TopUnder;
  Settings.TopFontStyleStrike := pS^.TopStrike;
  Settings.TopFontColor := pS^.TopColor;
  Settings.TopTextPos := pS^.TopTextPos;

  Settings.BKImage := pS^.BackImg;
  Settings.BkFileType := pS^.BackFType;
  Settings.BKFillColor := pS^.BkFillColor;
  Settings.BottomMessage := pS^.BottomMsg;
  Settings.BottomFontName := pS^.BottomFont;
  Settings.BottomFontSize := pS^.BottomSize;
  Settings.BottomFontStyleBold := pS^.BottomBold;
  Settings.BottomFontStyleItal := pS^.BottomItal;
  Settings.BottomFontStyleUnder := pS^.BottomUnder;
  Settings.BottomFontStyleStrike := pS^.BottomStrike;
  Settings.BottomFontColor := pS^.BottomColor;
  Settings.BottomTextPos := pS^.BottomTextPos;

  Settings.TransDelay := pS^.TransDelay;
  Settings.TransFillColor := pS^.TransFillColor;
  Settings.SoundFile := pS^.SoundFile;
  Settings.SoundDirOnOff := False;
end; // CopySettings


{-----------------------------------------------------------------------}

function LoadMainImage( Settings: TScrnSavSettings; artBMP: TBitmap;
                         artWMF: TMetafile; ErrorMsg: Boolean ): Boolean;
var FileTypeTemp: Integer;
    T: String;
begin

result := False;

  // Load in the main image
  if Settings.FileType = RESOURCE then begin
    T := Settings.MainImage;
    artWMF.Clear;
    if not loadWFMresource(PChar(T), artWMF) then
      exit;
  end;

  if Settings.FileType > RESOURCE then begin

    FileTypeTemp := loadFile( Settings.MainImage, artBMP, artWMF, ErrorMsg );

    if FileTypeTemp > 0 then
       Settings.FileType := FileTypeTemp;

    // If they have a bad file, let them know about it
    if FileTypeTemp <= 0 then begin

      if FileTypeTemp = ERROR_UNKNOWN_FORMAT then begin
         Settings.MainFileLoadErrorStr := ERUF + Settings.MainImage;
         loadWFMresource('errorWMF', artWMF);
      end;

      if FileTypeTemp = ERROR_FILE_MISSING then begin
         Settings.MainFileLoadErrorStr := ERFM + Settings.MainImage;
         loadWFMresource('missingWMF', artWMF);
      end;

      if FileTypeTemp = ERROR_BAD_DATA then begin
         Settings.MainFileLoadErrorStr := ERCF + Settings.MainImage;
         loadWFMresource('errorWMF', artWMF);
      end;
       Settings.FileType := RESOURCE;
       exit;
    end;
  end;
  result := True;
end; // LoadMainImage


{-----------------------------------------------------------------------}

function LoadBackImage( Settings: TScrnSavSettings; artBMP: TBitmap;
                         artWMF: TMetafile; ErrorMsg: Boolean ): Boolean;
var FileTypeTemp: Integer;
    T: String;
begin

result := True;

  // Load in the main image
  if Settings.BkFileType = RESOURCE then begin
    T := Settings.BKImage;
    GifLoadFromResource( T, artBMP );
  end;

  if Settings.BkFileType > RESOURCE then begin
    FileTypeTemp := loadFile( Settings.BKImage, artBMP, artWMF, ErrorMsg );

    if FileTypeTemp > 0 then
       Settings.BkFileType := FileTypeTemp;

    // If they have a bad file, let them know about it
    if FileTypeTemp <= 0 then begin

      result := False;

      if FileTypeTemp = ERROR_UNKNOWN_FORMAT then begin
         Settings.BackFileLoadErrorStr := ERUF + Settings.BKImage;
         GifLoadFromResource( 'errorGIF', artBMP );
      end;

      if FileTypeTemp = ERROR_FILE_MISSING then begin
         Settings.BackFileLoadErrorStr := ERFM + Settings.BKImage;
         GifLoadFromResource( 'missingGIF', artBMP );
      end;

      if FileTypeTemp = ERROR_BAD_DATA then begin
         Settings.BackFileLoadErrorStr := ERCF + Settings.BKImage;
         GifLoadFromResource( 'errorGIF', artBMP );
      end;
       Settings.BkFileType := RESOURCE;
    end;
  end;

end; // LoadBackImage

{-----------------------------------------------------------------------}

// Go through the list and mix it up
procedure RandomizeScreens( pS: PTScreenDesign; Count: Integer );
var temp, pScreenDTmpI, pScreenDTmpJ: PTScreenDesign;
  I, J: Integer;
begin

  // Get out if we don't have anything to randomize
  if Count <= 1 then exit;

  temp := allocmem(SizeOf(TScreenDesign));
  if temp = nil then begin
    Application.MessageBox('Memory allocation error was caused with temp var.',
           'There has been an error.', mb_OK + MB_ICONEXCLAMATION);
    exit;
  end;

  for I := 0 to Count-1 do begin
    J := Random(Count);
    if not(I = J) then begin
      pScreenDTmpI := pS;
      pScreenDTmpJ := pS;
      Inc(pScreenDTmpI, I);
      Inc(pScreenDTmpJ, J);
      CopyMemory( temp, pScreenDTmpI, SizeOf(TScreenDesign) );
      CopyMemory( pScreenDTmpI, pScreenDTmpJ, SizeOf(TScreenDesign) );
      CopyMemory( pScreenDTmpJ, temp, SizeOf(TScreenDesign) );
    end;
  end;

  FreeMem(temp, SizeOf(TScreenDesign));

end;

{-----------------------------------------------------------------------}

procedure LoadBitmapPalettes( canvas: TCanvas; ArtPalette, BackPalette: THandle; Settings: TScrnSavSettings );
begin

  if GetDeviceCaps( canvas.handle, BITSPIXEL ) > 8 then
    exit;

  if (Settings.FileType = BMP) and // Both have palettes
     ((Settings.BKFileType = BMP) or (Settings.BKFileType = RESOURCE)) then begin
     RemapBitmapPalette(ArtPalette, canvas, False);
     RemapBitmapPalette(ArtPalette, canvas, True);
     RemapBitmapPalette( BackPalette, canvas, True);
  end;

  if not(Settings.FileType = BMP) and // Background only has palette
        ((Settings.BKFileType = BMP) or (Settings.BKFileType = RESOURCE)) then begin
     RemapBitmapPalette( BackPalette, canvas, False);
     RemapBitmapPalette( BackPalette, canvas, True);
  end;

  if (Settings.FileType = BMP) and // Main image only has palette
      not((Settings.BKFileType = BMP) or (Settings.BKFileType = RESOURCE)) then begin
      RemapBitmapPalette(ArtPalette, canvas, False);
      RemapBitmapPalette(ArtPalette, canvas, True);
  end;
end; // LoadBitmapPalettes

{-----------------------------------------------------------------------}

procedure LoadHelpFile;
var
    PathToHelp: String;
    resFind, resLoad: THandle;
    resSize: LongInt;
    data: ^Byte;
    hFile, J: Integer;
begin
  // Check for help file
  PathToHelp := GetTheWindowsDirectory + HELP_FILE_NAME;

  if not FileExists(PathToHelp) then begin
     resFind := FindResource(HInstance, 'helpFile', 'HLP');
     if resFind = 0 then
     begin
       showMessage('Help file could not be found.');
       exit;
     end;
     resLoad := LoadResource(HInstance, resFind);
     resSize := SizeOfResource(HInstance, resFind);

     data := LockResource(resLoad);
     if data = nil then begin
       showMessage('Could not lock help file resource.');
       FreeResource(resLoad);
       exit;
     end;

     hFile := FileCreate(PathToHelp);
     if hFile <= 0 then begin
       showMessage('Could not create help file.');
       UnlockResource(resLoad);
       FreeResource(resLoad);
       exit;
     end;

    J := FileWrite( hFile, data^, resSize );
    if (J <= 0) or not(J = resSize ) then begin
      showMessage('Could not save help file to disk.');
      UnlockResource(resLoad);
      FreeResource(resLoad);
      FileClose(hFile);
      exit;
    end;

    FileClose(hFile);

     // Clear out the memory
     UnlockResource(resLoad);
     FreeResource(resLoad);
  end;

  Application.HelpFile := PathToHelp;
  Application.HelpContext(1);
end;

{-----------------------------------------------------------------------}

function IsMaskFileBMPorGIF( PathToMask: String ): Integer;
var hFile: Integer;
    I, HeaderType: WORD;
begin
  result := NONE;

  hFile := FileOpen( PathToMask, fmOpenReadWrite or fmShareDenyNone);
  if hFile <= 0 then
    exit;

  I := FileRead( hFile, HeaderType, sizeof(WORD) );
  FileClose(hFile);

  if (I <= 0) or not(I = sizeof(WORD) ) then
    exit;

  if HeaderType = $4D42 then
    result := BMP;

  if HeaderType = $4947 then
    result := GIF;

end;

{-----------------------------------------------------------------------}

end.
