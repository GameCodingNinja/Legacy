unit ImgLoad;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  FileCtrl, StdCtrls, ExtCtrls, constants, Globals, gFunctin, Loadpref;

type
  TfrmImageLoad = class(TForm)
    FilterComboBox1: TFilterComboBox;
    DirectoryListBox1: TDirectoryListBox;
    FileListBox1: TFileListBox;
    butCancel: TButton;
    butOK: TButton;
    pntImage: TPaintBox;
    DriveComboBox1: TDriveComboBox;
    gbDirSelection: TGroupBox;
    rdoAlphaImages: TRadioButton;
    rdoRandomImages: TRadioButton;
    gbImageSelect: TGroupBox;
    rdoSingle: TRadioButton;
    rdoDirectory: TRadioButton;
    procedure butCancelClick(Sender: TObject);
    procedure DirectoryListBox1Change(Sender: TObject);
    procedure DriveComboBox1Change(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FileListBox1Click(Sender: TObject);
    procedure butOKClick(Sender: TObject);
    procedure rdoSingleClick(Sender: TObject);
    procedure rdoDirectoryClick(Sender: TObject);
    procedure pntImagePaint(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
    artBMP: TBitmap;
    artWMF: TMetafile;
    ImageType, gFileTypeTemp, gSingleDirTemp: Integer;
    procedure PaintSelectedImage;
  public
    { Public declarations }
  end;

var
  frmImageLoad: TfrmImageLoad;

implementation

{$R *.DFM}

procedure TfrmImageLoad.butCancelClick(Sender: TObject);
begin
  gCancelPressed := True;
  close;
end;

procedure TfrmImageLoad.DirectoryListBox1Change(Sender: TObject);
begin
  FileListBox1.Directory := DirectoryListBox1.Directory;
end;

procedure TfrmImageLoad.DriveComboBox1Change(Sender: TObject);
begin
  DirectoryListBox1.Drive := DriveComboBox1.Drive;
end;

procedure TfrmImageLoad.FormCreate(Sender: TObject);
begin

  artWMF := TMetafile.Create;
  artBMP := TBitmap.Create;

end;

procedure TfrmImageLoad.PaintSelectedImage;
var ShowBox: Boolean;
    tempStr: String;
    tempBMP, buffer: TBitmap;
    MaskType: Integer;
begin
 ShowBox := True;

 // We're building it into a buffer first then blitting
 // it to the screen
 buffer := TBitmap.Create;
 buffer.Width := pntImage.Width;
 buffer.Height := pntImage.Height;
 buffer.Canvas.Brush.Color := clBtnFace;
 buffer.Canvas.FillRect(Rect(0,0,buffer.Width,buffer.Height));

 if (FileListBox1.ItemIndex > -1) or rdoDirectory.Checked then begin
   case ImageType of
     WMF: begin
       if (artWMF.Width < buffer.Width) and (artWMF.Height < buffer.Height) then begin
          buffer.Canvas.StretchDraw(centerSquInSqu(buffer.Width,
               buffer.Height,artWMF.Width,artWMF.Height),ArtWMF);
       end else
          buffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               buffer.Height,artWMF.Width,artWMF.Height),ArtWMF);
       ShowBox := False;
     end;
     BMP: begin
       // If they are loading up a background, check to see if we need to tile it
       if gTileGraphic and (rdoDirectory.Checked = False) then begin
         // Check to see if image is too big
         if (artBMP.Width > pntImage.Width) and (artBMP.Height > pntImage.Height) then
            buffer.Canvas.StretchDraw(sizeItUp(buffer.Width,
               buffer.Height,artBMP.Width,artBMP.Height),artBMP)
         else
            tileBackground(buffer.canvas, artBMP, buffer.Width, buffer.Height, BMP);
       end else begin
         // Check for a mask file
         tempStr := FileListBox1.FileName;
         // Delete the last three charaters of the string
         delete(tempStr, Length(tempStr)-2, 3);
         tempStr := tempStr + 'msk';

         if (artBMP.Width < pntImage.Width) and (artBMP.Height < pntImage.Height) then begin
           // Check for mask file
           // Center it for this one
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
               buffer.Canvas.StretchDraw(centerSquInSqu(pntImage.Width,
                 pntImage.Height,tempBMP.Width,tempBMP.Height),tempBMP);
               buffer.Canvas.CopyMode := cmSrcPaint;
               buffer.Canvas.StretchDraw(centerSquInSqu(pntImage.Width,
                 pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
               buffer.Canvas.CopyMode := cmSrcCopy;
               pntImage.Canvas.Draw(0,0,buffer);
             end else begin
               buffer.Canvas.CopyMode := cmSrcCopy;
               buffer.Canvas.StretchDraw(centerSquInSqu(pntImage.Width,
                 pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
             end;
             tempBMP.free;
           end else
             buffer.Canvas.StretchDraw(centerSquInSqu(pntImage.Width,
               pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
         end else begin
           ShowBox := False;
           // Size it to fit on this one
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
               buffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
                 pntImage.Height,tempBMP.Width,tempBMP.Height),tempBMP);
               buffer.Canvas.CopyMode := cmSrcPaint;
               buffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
                 pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
               buffer.Canvas.CopyMode := cmSrcCopy;
             end else begin
               buffer.Canvas.CopyMode := cmSrcCopy;
               buffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
                 pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
               pntImage.Canvas.Draw(0,0,buffer);
             end;
             tempBMP.free;
           end else
             buffer.Canvas.StretchDraw(sizeItUp(pntImage.Width,
               pntImage.Height,artBMP.Width,artBMP.Height),ArtBMP);
         end;
       //RemapBitmapPalette(artBMP, pntImage.canvas, False);
       end;
     ShowBox := False;
     end;
   end;
 end;
 if ShowBox then begin
   buffer.Canvas.Pen.Color := clBlack;
   buffer.Canvas.Brush.Style := bsClear;
   buffer.Canvas.Rectangle(0, 0, buffer.Width, buffer.Height);
 end;
 pntImage.Canvas.Draw(0,0,buffer);
 buffer.free;
end;

procedure TfrmImageLoad.FileListBox1Click(Sender: TObject);
begin
  // Keeps it from causing an error (-1) when user
  // clicks it the wrong way
  if FileListBox1.ItemIndex < 0 then exit;

  if not(rdoSingle.Checked) and not(rdoDirectory.Checked) then
     rdoSingle.Checked := True;

  if (rdoSingle.Checked) and (FileListBox1.ItemIndex >= 0) then begin
    ImageType := loadFile(FileListBox1.FileName,artBMP,artWMF,SHOW_ERROR_MSG);
    //if ImageType = BMP then RemapBitmapPalette(artBMP, pntImage.canvas, False);
    PaintSelectedImage;
  end;
end;

procedure TfrmImageLoad.butOKClick(Sender: TObject);
var s: String;
begin
  //Make sure they don't choose the same directory for all their images
  if (gOtherImageDirStr = FileListBox1.Directory) and rdoDirectory.Checked then begin
     S := 'The directory choosen to hold your images is already in ' +
          'use to hold images for your other directory of images. For this screen ' +
          'saver to work correctly, you need a separate directory. One for the main ' +
          'images and one for the background images.';

          Application.MessageBox(PChar(S),'There has been an error! Duplicate Directory!',
             mb_OK + MB_ICONEXCLAMATION);
     exit;
  end;

  // Make sure you warn them that the directory chosen is empty
  if (FileListBox1.Items.count = 0) and rdoDirectory.Checked then begin
     S := 'The directory choosen to hold your images is empty. ' +
          'This screen saver will not work correctly if this directory remains empty. ' +
          'Are you sure this is the directory you will use to hold your images?';

     if Application.MessageBox(PChar(S),'There may be a problem. Directory Empty.',
             mb_YesNo + MB_ICONEXCLAMATION) = IDNO then
       exit;
  end;

  // See if they chose a single image but didn't select an image
  if rdoSingle.Checked and (FileListBox1.ItemIndex < 0) then begin
     S := 'An image file has not been selected. Please select an image file or ' +
          'click on the cancel button.';
          Application.MessageBox(PChar(S),'There has been an error!',
             mb_OK + MB_ICONEXCLAMATION);
     exit;
  end;

  // Save Image Selection Mode
  if rdoDirectory.Checked then
     gSingleDir := LOADDIR;

  if rdoSingle.Checked then
     gSingleDir := LOAD;

  {if (not(rdoDirectory.Checked) and not(rdoSingle.Checked)) or (FileListBox1.ItemIndex < 0) then begin
    //They didn't do anything while they were here so
    // we need to fake a cancel
    gCancelPressed := True;
    close;
    exit;
  end;}

  // Save Dir list options
  if rdoAlphaImages.Checked then
    gDirRandomSort := SORTED_FILES_IN_DIR
  else
    gDirRandomSort := RANDOM_FILES_IN_DIR;

  //Save if they have chosen a file or a directory
  if (FileListBox1.ItemIndex >= 0) and rdoSingle.Checked then begin
    gImageDirStr := FileListBox1.FileName;
    gFileType := gFileTypeTemp;
  end;

  if rdoDirectory.Checked then begin
    gImageDirStr := FileListBox1.Directory;
    gFileType := gFileTypeTemp;
  end;

  close;
end;

procedure TfrmImageLoad.rdoSingleClick(Sender: TObject);
begin
  gbDirSelection.Enabled := False;
  rdoAlphaImages.Enabled := False;
  rdoRandomImages.Enabled := False;
  FileListBox1.Enabled := True;

  if (FileListBox1.ItemIndex >= 0) and (FileListBox1.FileName <> '') then begin
    ImageType := loadFile(FileListBox1.FileName,artBMP,artWMF,SHOW_ERROR_MSG);
    //if ImageType = BMP then RemapBitmapPalette(artBMP, pntImage.canvas, False);
    PaintSelectedImage;
  end else begin
    pntImage.Canvas.Brush.Color := clBtnFace;
    pntImage.Canvas.FillRect(Rect(0,0,pntImage.Width,pntImage.Height));
  end;
end;

procedure TfrmImageLoad.rdoDirectoryClick(Sender: TObject);
begin
  gSingleDirTemp := LOADDIR;
  FileListBox1.Enabled := False;

  if gMainBack = METAFILE then begin
    loadWFMresource(pchar('folder'), artWMF);
    ImageType := WMF;
    gFileTypeTemp := ImageType;
  end else begin
    ImageType := BMP;
    gFileTypeTemp := ImageType;
    GifLoadFromResource( 'folderGIF', artBMP );
  end;

  gbDirSelection.Enabled := True;
  rdoAlphaImages.Enabled := True;
  rdoRandomImages.Enabled := True;

  PaintSelectedImage;
end;

procedure TfrmImageLoad.pntImagePaint(Sender: TObject);
begin
  PaintSelectedImage;
end;

procedure TfrmImageLoad.FormDestroy(Sender: TObject);
begin
  artWMF.free;
  artBMP.free;
end;

procedure TfrmImageLoad.FormShow(Sender: TObject);
begin

  ImageType := NONE;
  gCancelPressed := False;

  gSingleDirTemp := gSingleDir;
  gFileTypeTemp := gFileType;

  // Reset defaults
  rdoSingle.Checked := True;
  rdoDirectory.Checked := False;
  rdoAlphaImages.Checked := True;
  rdoRandomImages.Checked := False;
  gbDirSelection.Enabled := False;
  rdoAlphaImages.Enabled := False;
  rdoRandomImages.Enabled := False;
  FileListBox1.Update;

  // Set Dir list options
  if gDirRandomSort = SORTED_FILES_IN_DIR then
    rdoAlphaImages.Checked := True
  else
    rdoRandomImages.Checked := True;

  if (gSingleDirTemp = LOAD) then begin
        // Check to see if the file is there
        if FileExists( gImageDirStr ) then begin
          rdoSingle.Checked := True;
          FileListBox1.FileName := gImageDirStr;
          DirectoryListBox1.Directory := FileListBox1.Directory;
          DriveComboBox1.Drive := DirectoryListBox1.Drive;
          ImageType := loadFile(FileListBox1.FileName,artBMP,artWMF,NO_ERROR_MSG);
          gFileTypeTemp := ImageType;
          gbDirSelection.Enabled := False;
          rdoAlphaImages.Enabled := False;
          rdoRandomImages.Enabled := False;
        end;
  end;

  if (gSingleDirTemp = LOADDIR) then begin
    //Check to see if it is a valid path
    if DirectoryExists(gImageDirStr) then begin
       rdoDirectory.Checked := True;
       FileListBox1.Directory := gImageDirStr;
       DirectoryListBox1.Directory := FileListBox1.Directory;
       DriveComboBox1.Drive := DirectoryListBox1.Drive;

       if gMainBack = METAFILE then begin
         loadWFMresource(pchar('folder'), artWMF);
         ImageType := WMF;
         gFileTypeTemp := ImageType;
       end else begin
         ImageType := BMP;
         gFileTypeTemp := ImageType;
         GifLoadFromResource( 'folderGIF', artBMP );
       end;
    end;
  end;

  // Disable these for the Designer Screen
  if gTurnOffImageModeSelections then begin
    rdoSingle.Enabled := False;
    rdoDirectory.Enabled := False;
    rdoAlphaImages.Enabled := False;
    rdoRandomImages.Enabled := False;
  end;

end;

end.
