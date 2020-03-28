;************************************************************************
;
;    PRODUCT:         Animation data
;
;    FILE NAME:       DataRes.cpp
;
;    DESCRIPTION:     Offsets and data to animate the screen saver 
;
;
;    Copyright(c) by Howard C Rosenorn
;    All Rights Reserved
;
;***********************************************************************

IDEAL
p386
MODEL tchuge

PUBLIC C Resources


STRUC TFishData
	left	    DD   0
	top	        DD   0
	right	    DD   0
	bottom	    DD   0
	FrameCount  DD   2
	BottomOnly	DD   0
ENDS TFishData

CODESEG
p386


LABEL Resources
	DD  24 ; Total number of fish
	TFishData<   0,    0, 432, 125, 3, 0 >
	TFishData< 866,    1, 214,  54, 2, 0 >
	TFishData< 866,  112, 216, 111, 2, 0 >
	TFishData<   0,  250, 242, 125, 2, 0 >
	TFishData< 485,  250, 178,  95, 2, 0 >
	TFishData< 863,  334, 184,  89, 2, 0 >
	TFishData<   0,  500, 340, 135, 2, 0 >
	TFishData< 690,  515, 148,  82, 2, 0 >
	TFishData< 989,  524, 146,  71, 2, 0 >
	TFishData< 730,  687, 116,  44, 2, 0 >
	TFishData< 963,  687, 128,  48, 2, 0 >
	TFishData<   1,  773, 154,  54, 2, 0 >
	TFishData< 315,  773, 178,  63, 2, 0 >
	TFishData< 676,  785, 146,  59, 2, 0 >
	TFishData< 973,  785, 144,  65, 2, 0 >
	TFishData<   0,  905, 158,  69, 2, 0 >
	TFishData< 323,  905, 146,  64, 2, 0 >
	TFishData< 622,  920, 190,  69, 2, 0 >
	TFishData<   0, 1061, 204,  72, 2, 0 >
	TFishData< 418, 1061, 306, 100, 3, 1 >
	TFishData<   0, 1267, 219,  89, 3, 1 >
	TFishData< 450, 1267, 363, 117, 3, 0 >
	TFishData<   0, 1511, 540, 135, 3, 0 >
	TFishData<   0, 1788, 381, 115, 3, 0 >


END