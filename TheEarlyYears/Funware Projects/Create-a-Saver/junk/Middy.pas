unit Middy;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  MPlayer, constants;

type
  TfrmMID = class(TForm)
    MediaPlayer1: TMediaPlayer;
  private
    { Private declarations }
  public
    { Public declarations }
    function GetPlayerStatus : Integer;
  end;

var
  frmMID: TfrmMID;

implementation

{$R *.DFM}

  function TfrmMID.GetPlayerStatus : Integer;
  begin
    case MediaPlayer1.Mode of
      mpNotReady:   result := NotReady;
      mpStopped:    result := Stopped;
      mpPlaying:    result := Playing;
      mpRecording:  result := Recording;
      mpSeeking:    result := Seeking;
      mpPaused:     result := Paused;
      mpOpen:       result := Open;
      else          result := -1;
    end;
  end;

end.
