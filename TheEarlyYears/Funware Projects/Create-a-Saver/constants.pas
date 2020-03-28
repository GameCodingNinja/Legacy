unit constants;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ComCtrls, savInit, ExtCtrls;

type
  TScreenDesign = Record
    MainImg: String[255];
    MainFType: LongWord;
    MainResIndex: LongWord;
    SizeBitmaps: Boolean;
    TopMsg: String[255];
    TopFont: String[100];
    TopSize: LongWord;
    TopBold: Boolean;
    TopItal: Boolean;
    TopUnder: Boolean;
    TopStrike: Boolean;
    TopColor: LongWord;
    TopTextPos: LongWord;

    BackImg: String[255];
    BackFType: LongWord;
    BackResIndex: LongWord;
    BkFillColor: LongWord;
    BottomMsg: String[255];
    BottomFont: String[100];
    BottomSize: LongWord;
    BottomBold: Boolean;
    BottomItal: Boolean;
    BottomUnder: Boolean;
    BottomStrike: Boolean;
    BottomColor: LongWord;
    BottomTextPos: LongWord;

    TransFillColor: LongWord;
    TransDelay: LongWord;
    SoundFile: String[255];
    SoundOnOff: Boolean;

    FutureAddStr1: String[255];
    FutureAddStr2: String[255];
    FutureAddStr3: String[255];
    FutureAddStr4: String[255];
    FutureAddBool1: LongWord;
    FutureAddBool2: LongWord;
    FutureAddBool3: LongWord;
    FutureAddBool4: LongWord;
    FutureAddBool5: LongWord;
    FutureAddInt1: LongWord;
    FutureAddInt2: LongWord;
    FutureAddInt3: LongWord;
    FutureAddInt4: LongWord;
    FutureAddInt5: LongWord;
  end;

  PTScreenDesign = ^TScreenDesign;

  type
    TRGB = Record
    R: BYTE;
    G: BYTE;
    B: BYTE;
  end;

  PTRGB = ^TRGB;

  type PLong = ^LongInt;

  type SaverException = class(Exception);

var
     gPasswordEnabled, gImageDirStr, gOtherImageDirStr, gSoundDir,
     gSoundFilePath, gBottomText, gTopText, gScreenDesignPath: String;
     gAskForPassword, gTransOnOff, gCancelPressed, gSizeBitmaps, gTopTextRand,
     gSoundsOnOff, gSoundsRandomList, gTileGraphic, gBottomTextRand, gSoundSDonOff,
     gScreenDesignOnOff, gTurnOffImageModeSelections, gPasswordDlgShown,
     gSoundFilesInterrupt, gRandomBacks: Boolean;
     gTransSpeed, gTransFillColor, gTransDelay, gScreenDesignRandom, gTotalScr,
     gSingleDir, gFileType, gDirRandomSort, gMainBack, gCurrentScr: Integer;
     gCustomColors: TStringList;
     artWMF, backWMF: TMetafile;
     artBMP, backBMP, buffer: TBitmap;
     ScreenW, ScreenH: Integer;
     ArtPalette, BackPalette: THandle;

const
     DEBUGIT = False;
     //DEBUGIT = True;

     FAST_RUN_DEBUG = False;
     //FAST_RUN_DEBUG = True;

    //ON_OFF_TEST = TRUE;
    ON_OFF_TEST = FALSE;     

  ALLOW_DISK_ORDER = True;

  PRODUCT_COST = 10.00;
  REDUCED_RATE = 7.00;

  // Windows 98 GetSystemMetrics #defines
  SM_XVIRTUALSCREEN    =  76;
  SM_YVIRTUALSCREEN    =  77;
  SM_CXVIRTUALSCREEN   =  78;
  SM_CYVIRTUALSCREEN   =  79;
  SM_CMONITORS         =  80;

  //Constants to change when re-using code for a different project
  APP_NAME = 'Create-A-Saver';
  FILE_NAME = 'Create-A-Saver.ini';  // ini file name
  ORDER_FORM_NAME = 'Create-A-Saver Order Form'; // For ini file
  ORDER_FORM_PRINT_OUT = 'Create-A-Saver Screen Saver  Ver. 1.8.4';
  SHAREWARE_TITLE = 'Create-A-Saver  Quick Setup   Ver. 1.8.4  Shareware';
  FULLVERSION_TITLE = 'Create-A-Saver  Quick Setup   Ver. 1.8.4  Registered';

  programToSell = 'Create-A-Saver';   //Name of product to sell
  orderFormIntro = 'Create-A-Saver';      //Just the name

  HELP_FILE_NAME = 'Create-A-Saver.hlp';
  HELP_FILE_JUNK = 'Create-A-Saver.GID';

  REGISTRATION_VALIDATION = 2671;
  REGISTRATION_FAKE_VALIDATION = 2642;

  //SHAREWARE_MESSAGES = 17;
  EASY_REGISTER_MSG = 100;
  SHAREWARE_MESSAGE_DELAY = 30000;  //90000  100000;
  //SHAREWARE_MESSAGE_DELAY = 1000;  //90000  100000;
  SHAREWARE_MESSAGE_SLEEP = 10000;
  LOGOXY_OFFSET = 30;

  ENCRYPT = 0;
  UNENCRYPT = 1;

  ERASE_FILL_BLACK = 0;
  ERASE_FILL_COLORS = 1;
  ERASE_WITH_NEXT_IMAGE = 2;

  //File Filters
  NONE = 0;
  LOAD = 1;
  COLORS = 2;
  LOADDIR = 3;
  RESOURCE = 4;
  WMF = 5;
  BMP = 6;
  GIF = 7;
  JPG = 8;


  BITMAP = 0;
  METAFILE = 1;

  // Dialog boxes
  TOP_FONT_DLG = 0;
  TOP_FONT_COLOR_DLG = 1;
  BOTTOM_FONT_DLG = 2;
  BOTTOM_FONT_COLOR_DLG = 3;

  // Text Positions
  LEFT_JUST = 0;
  CENTER_TEXT = 1;
  RIGHT_JUST = 2;

  // Error Messages Should we show the error message
  SHOW_ERROR_MSG = true;
  NO_ERROR_MSG = false;
  EMPTY_DIR = 'empty';
  MISSING_DIR = 'missing';
  ERFM = ' ERROR!! File Missing!  -  ';
  ERCF = ' ERROR!! File Corrupt!  -  ';
  ERUF = ' ERROR!! Unknown File Format!  -  ';
  ERSF = ' ERROR!! Sound File Missing!  -  ';
  ERUPF = ' ERROR!! Unable to play sound file!  -  ';
  ERROR_UNKNOWN_FORMAT = 0;
  ERROR_FILE_MISSING = -1;
  ERROR_BAD_DATA = -2;


  // Transitions
  PAINT_SQUARES = 1;

  SQUARE_SIZE_RAND = 40;//50;
  SMALLEST_SQUARE_SIZE = 10;//7
  DUMMY_TRANS = 1;

  SINGLE_FILE = 0;
  DIRECTORY = 1;
  SORTED_FILES_IN_DIR = 0;
  RANDOM_FILES_IN_DIR = 1;
  SCREEN_DESIGNER_RANDOM = 1;
  SCREEN_DESIGNER_ORDER = 0;

  SMALL_SKIP = 30;
  //SMALL_SKIP = 150;

implementation

end.
 