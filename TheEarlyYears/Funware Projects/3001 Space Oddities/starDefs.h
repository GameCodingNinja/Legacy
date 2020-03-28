
/************************************************************************
*
*    PRODUCT:         Screen Saver
*
*    FILE NAME:       starDefs.h
*
*    DESCRIPTION:     Global defines for the star screen saver
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __StarDefines_H__
#define __StarDefines_H__

#define MAX_CYCLE_RENDER_WAIT         30

#define FLIP_LEFT     TRUE
#define FLIP_DOWN     FALSE

// Star defines
#define STAR1_WH    21
#define STAR2_WH    17
#define STAR3_WH    15
#define STAR4_WH    15
#define STAR5_WH    11
#define STAR6_WH    11
#define STAR7_WH    9
#define STAR8_WH    9
#define STAR9_WH    9
#define STAR10_WH   9
#define STAR11_WH   9
#define STAR12_WH   9
#define STAR13_WH   9
#define STAR14_WH   9
#define STAR15_WH   9
#define MAX_STAR_TYPES 28
#define MAX_BLINK_STATES 2

// Planet defines
#define Planet1X    0
#define Planet1Y    43
#define Planet1W    35
#define Planet1H    35

#define Planet2X    36
#define Planet2Y    34
#define Planet2W    45
#define Planet2H    42

#define Planet3X    0
#define Planet3Y    113
#define Planet3W    33
#define Planet3H    34

#define Planet4X    34
#define Planet4Y    118
#define Planet4W    46
#define Planet4H    47

#define Planet5X    82
#define Planet5Y    23
#define Planet5W    63
#define Planet5H    63

#define Planet6X    146
#define Planet6Y    19
#define Planet6W    76
#define Planet6H    79

#define Planet7X    223
#define Planet7Y    0
#define Planet7W    123
#define Planet7H    124

#define Planet8X    347
#define Planet8Y    0
#define Planet8W    132
#define Planet8H    133

#define Planet9X    480
#define Planet9Y    0
#define Planet9W    110
#define Planet9H    112

#define Planet10X   591
#define Planet10Y   0
#define Planet10W   65
#define Planet10H   64

#define Planet11X   590
#define Planet11Y   128
#define Planet11W   59
#define Planet11H   60

#define Planet12X   0
#define Planet12Y   212
#define Planet12W   95
#define Planet12H   95

#define Planet13X   96
#define Planet13Y   177
#define Planet13W   108
#define Planet13H   109

#define Planet14X   205
#define Planet14Y   248
#define Planet14W   87
#define Planet14H   86

#define Planet15X   293
#define Planet15Y   266
#define Planet15W   93
#define Planet15H   92

#define Planet16X   387
#define Planet16Y   266
#define Planet16W   71
#define Planet16H   73

#define Planet17X   479
#define Planet17Y   224
#define Planet17W   99
#define Planet17H   99

#define Planet18X   579
#define Planet18Y   248
#define Planet18W   75
#define Planet18H   76

// Ship defines
#define Ship1X    505
#define Ship1Y    0
#define Ship1W    77
#define Ship1H    197
#define Ship1CanFlip FALSE
#define Ship1FlipLeftUp FALSE
#define Ship1CanUpDown FALSE
#define Ship1CanLeftRight FALSE
#define Ship1NumFrames	 2
#define Ship1PixelSpeed  2
#define Ship1AnimDelay   70

#define Ship2X    0
#define Ship2Y    0
#define Ship2W    126
#define Ship2H    134
#define Ship2CanFlip FALSE
#define Ship2FlipLeftUp FALSE
#define Ship2CanUpDown TRUE
#define Ship2CanLeftRight TRUE
#define Ship2NumFrames	 4
#define Ship2PixelSpeed  1
#define Ship2AnimDelay   200

#define Ship3X    0
#define Ship3Y    268
#define Ship3W    78
#define Ship3H    48
#define Ship3CanFlip FALSE
#define Ship3FlipLeftUp FALSE
#define Ship3CanUpDown TRUE
#define Ship3CanLeftRight TRUE
#define Ship3NumFrames	 3
#define Ship3PixelSpeed  2
#define Ship3AnimDelay   100

#define Ship4X    235
#define Ship4Y    268
#define Ship4W    53
#define Ship4H    50
#define Ship4CanFlip TRUE
#define Ship4FlipLeftUp FLIP_LEFT
#define Ship4CanUpDown FALSE
#define Ship4CanLeftRight TRUE
#define Ship4NumFrames	 2
#define Ship4PixelSpeed  3
#define Ship4AnimDelay   70

#define Ship5X    0
#define Ship5Y    369
#define Ship5W    92
#define Ship5H    51
#define Ship5CanFlip TRUE
#define Ship5FlipLeftUp FLIP_LEFT
#define Ship5CanUpDown FALSE
#define Ship5CanLeftRight TRUE
#define Ship5NumFrames	 2
#define Ship5PixelSpeed  3
#define Ship5AnimDelay   70

#define Ship6X    369
#define Ship6Y    395
#define Ship6W    72
#define Ship6H    36
#define Ship6CanFlip TRUE
#define Ship6FlipLeftUp FLIP_LEFT
#define Ship6CanUpDown FALSE
#define Ship6CanLeftRight TRUE
#define Ship6NumFrames	 2
#define Ship6PixelSpeed  3
#define Ship6AnimDelay   70

#define Ship7X    0
#define Ship7Y    471
#define Ship7W    63
#define Ship7H    45
#define Ship7CanFlip TRUE
#define Ship7FlipLeftUp FLIP_LEFT
#define Ship7CanUpDown FALSE
#define Ship7CanLeftRight TRUE
#define Ship7NumFrames	 2
#define Ship7PixelSpeed  2
#define Ship7AnimDelay   70

#define Ship8X    253
#define Ship8Y    471
#define Ship8W    103
#define Ship8H    41
#define Ship8CanFlip TRUE
#define Ship8FlipLeftUp FLIP_LEFT
#define Ship8CanUpDown FALSE
#define Ship8CanLeftRight TRUE
#define Ship8NumFrames	 2
#define Ship8PixelSpeed  3
#define Ship8AnimDelay   70

#define Ship9X    0
#define Ship9Y    562
#define Ship9W    167
#define Ship9H    61
#define Ship9CanFlip TRUE
#define Ship9FlipLeftUp FLIP_LEFT
#define Ship9CanUpDown FALSE
#define Ship9CanLeftRight FALSE
#define Ship9NumFrames	 2
#define Ship9PixelSpeed  1
#define Ship9AnimDelay   100

#define Ship10X   0
#define Ship10Y   685
#define Ship10W   131
#define Ship10H   63
#define Ship10CanFlip TRUE
#define Ship10FlipLeftUp FLIP_LEFT
#define Ship10CanUpDown FALSE
#define Ship10CanLeftRight FALSE
#define Ship10NumFrames	 2
#define Ship10PixelSpeed  2
#define Ship10AnimDelay   100

#define Ship11X   0
#define Ship11Y   812
#define Ship11W   67
#define Ship11H   58
#define Ship11CanFlip TRUE
#define Ship11FlipLeftUp FLIP_LEFT
#define Ship11CanUpDown TRUE
#define Ship11CanLeftRight TRUE
#define Ship11NumFrames	 2
#define Ship11PixelSpeed  2
#define Ship11AnimDelay   70

#define Ship12X   268
#define Ship12Y   812
#define Ship12W   50
#define Ship12H   28
#define Ship12CanFlip TRUE
#define Ship12FlipLeftUp FLIP_LEFT
#define Ship12CanUpDown FALSE
#define Ship12CanLeftRight TRUE
#define Ship12NumFrames	 2
#define Ship12PixelSpeed  3
#define Ship12AnimDelay   70

#define Ship13X   468
#define Ship13Y   830
#define Ship13W   56
#define Ship13H   39
#define Ship13CanFlip TRUE
#define Ship13FlipLeftUp FLIP_LEFT
#define Ship13CanUpDown FALSE
#define Ship13CanLeftRight TRUE
#define Ship13NumFrames	 2
#define Ship13PixelSpeed  2
#define Ship13AnimDelay   100

#define Ship14X   0
#define Ship14Y   928
#define Ship14W   172
#define Ship14H   53
#define Ship14CanFlip TRUE
#define Ship14FlipLeftUp FLIP_DOWN
#define Ship14CanUpDown FALSE
#define Ship14CanLeftRight TRUE
#define Ship14NumFrames	 4
#define Ship14PixelSpeed  3
#define Ship14AnimDelay   70

#define Ship15X   0
#define Ship15Y   1141
#define Ship15W   57
#define Ship15H   69
#define Ship15CanFlip FALSE
#define Ship15FlipLeftUp FALSE
#define Ship15CanUpDown TRUE
#define Ship15CanLeftRight TRUE
#define Ship15NumFrames	 3
#define Ship15PixelSpeed  2
#define Ship15AnimDelay   200

#define Ship16X   172
#define Ship16Y   1141
#define Ship16W   102
#define Ship16H   51
#define Ship16CanFlip TRUE
#define Ship16FlipLeftUp FLIP_LEFT
#define Ship16CanUpDown FALSE
#define Ship16CanLeftRight TRUE
#define Ship16NumFrames	  2
#define Ship16PixelSpeed  1
#define Ship16AnimDelay   100

#define Ship17X   171
#define Ship17Y   1244
#define Ship17W   99
#define Ship17H   44
#define Ship17CanFlip TRUE
#define Ship17FlipLeftUp FLIP_LEFT
#define Ship17CanUpDown FALSE
#define Ship17CanLeftRight TRUE
#define Ship17NumFrames	  2
#define Ship17PixelSpeed  2
#define Ship17AnimDelay   70

#define Ship18X   0
#define Ship18Y   1333
#define Ship18W   103
#define Ship18H   45
#define Ship18CanFlip TRUE
#define Ship18FlipLeftUp FLIP_LEFT
#define Ship18CanUpDown FALSE
#define Ship18CanLeftRight TRUE
#define Ship18NumFrames	  2
#define Ship18PixelSpeed  2
#define Ship18AnimDelay   70

#define Ship19X   413
#define Ship19Y   1333
#define Ship19W   64
#define Ship19H   42
#define Ship19CanFlip TRUE
#define Ship19FlipLeftUp FLIP_LEFT
#define Ship19CanUpDown FALSE
#define Ship19CanLeftRight TRUE
#define Ship19NumFrames	  2
#define Ship19PixelSpeed  2
#define Ship19AnimDelay   70

#define NUM_PLANETS               18
#define NUM_SHIPS                 19
#define MIN_NUM_OF_SHIPS          3
#define SHIP_PIXEL_SPEED_MAX      3
#define SHIP_PIXEL_SPEED_MIN      1
#define SHIP_PICK_TIME_MIN        2000
#define SHIP_PICK_TIME_MAX        500
#define SHIP_ANIM_DELAY           50
#define PLANET_RANDOM_OCCURENCE   500
#define PLANET_MAGIC_NUMBER       5
#define PLANET_PIXEL_SPEED        1
#define PLANET_SPEED_DELAY_MAX    130//300
#define PLANET_SPEED_DELAY_MIN    40//150
#define PLANET_PICK_TIME_MIN      25000
#define PLANET_PICK_TIME_MAX      35000

#define CHECK_PLAYBACK_DONE_INTERVAL  7000
#define MAX_PREVIEW_TIMER_COUNTS  2

// Needs to be divisible by 10 and 15
#define TOTAL_NUM_STARS  1020

#define STAR_TRIM     3
#define STAR_SPEED  3

#define BLINK_DELAY   3000
#define BRIGHT_STAR_MIN    75
#define BRIGHT_STAR_MAX    150

#define ERASE_WH    50
#define NUM_CLOUDS   6

enum
{
    ERASE = 0,
    MOTION,
};

enum
{
	PLAY_MUSIC = 0,
	ERASE_BUFFER,
	UPDATE_ANIM,
	BLIT_TO_BUFFER,
	SHAREWARE_NOTICE,
	BLIT_TO_SCREEN,
	MAX_BLIT_OPERATIONS,
};


// Manages the star data
struct TStar
{
    RECT Dest;
    RECT Source;
    DWORD BlinkTimer;
    BOOL StarOnOff;
    int Speed;
};

typedef TStar *PTStar;


// Manages the planet data
struct TPlanet
{
    RECT Dest;
    RECT Source;
    DWORD SpeedDelay;
    DWORD SpeedTimer;
    BOOL Active;
};

typedef TPlanet *PTPlanet;

// Manages the ship data
struct TShip
{
    RECT Dest;
    RECT Source;
	RECT Art;
    BOOL Active;
	BOOL CanFlip;
	BOOL FlipLeftDown;
	BOOL CanGoUpDown;
	BOOL CanGoLeftRight;
	int Movement;
	int NumFrames;
	int PixelSpeed;
	DWORD AnimTimer;
	DWORD AnimDelay;
	int CurrentFrame;
};

typedef TShip *PTShip;


#endif  // __StarDefines_H__