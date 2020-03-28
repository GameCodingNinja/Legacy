/************************************************************************
*
*    PRODUCT:         Map texture editor
*
*    FILE NAME:       GDefines.h
*
*    DESCRIPTION:     Global defines
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __GDefines_H__
#define __GDefines_H__

// This keeps us from having to compile all the different render flavor functions.
// Uncomment the ones you don't need and comment the ones you use

//#define CHECK_TEXT_CEILING
//#define CHECK_TEXT_CEILING_TRANS

#define REDEFINE_MINZ  50.0F

//#define NO_16_TO_16_POS
#define NO_16_TO_16_POS_OR
//#define NO_16_TO_16_POS_TRANS
#define NO_16_TO_16_NEG
#define NO_16_TO_16_NEG_OR
#define NO_16_TO_16_NEG_TRANS
#define NO_16_TO_16_NO_Z_BUF
#define NO_16_TO_16_NO_Z_BUF_NO_SHTBL
#define NO_16_TO_16_NO_Z_BUF_OR
#define NO_16_TO_16_NO_Z_BUF_TRANS
//#define NO_16_TO_16_POS_SOLID
#define NO_16_TO_16_NEG_SOLID

//#define NO_8_TO_16_POS
#define NO_8_TO_16_POS_CHECK
#define NO_8_TO_16_POS_OR
//#define NO_8_TO_16_POS_TRANS
#define NO_8_TO_16_NEG
#define NO_8_TO_16_NEG_OR
#define NO_8_TO_16_NEG_TRANS
#define NO_8_TO_16_NO_Z_BUF
#define NO_8_TO_16_NO_Z_BUF_OR
#define NO_8_TO_16_NO_Z_BUF_TRANS
//#define NO_8_TO_16_POS_SOLID
#define NO_8_TO_16_NEG_SOLID

//#define NO_8_TO_8_POS
#define NO_8_TO_8_POS_CHECK
#define NO_8_TO_8_POS_OR
//#define NO_8_TO_8_POS_TRANS
#define NO_8_TO_8_NEG
#define NO_8_TO_8_NEG_OR
#define NO_8_TO_8_NEG_TRANS
#define NO_8_TO_8_NO_Z_BUF
#define NO_8_TO_8_NO_Z_BUF_OR
#define NO_8_TO_8_NO_Z_BUF_TRANS
//#define NO_8_TO_8_POS_SOLID
#define NO_8_TO_8_NEG_SOLID


// Misc defines
#define WINDOW_NAME            "EditMap"
#define WINDOW_TITLE           "3D Edit/Animation Tool ver 1.1.1"

// Misc Strings
#define OPEN_FILE_CAPTION      "Open Polygon Object File"
#define PROJECT_FILE_MASK      "Map Files (*.dxf; *.ply)\0*.dxf;*.ply\0\0"
#define PROJECT_EXT_MASK       "*.dxf"
#define PROJECT_EXT            "dxf"
#define MAIN_APP_TOOLS         "gtools.h"
#define RES_FILE_TYPES         "Texture Files (*.bmp)\0*.bmp\0\0"
#define BIN_FILE_TYPES         "Bin Files (*.bin)\0*.bin\0\0"
#define BIN_EXT                "bin"

#define INI_FILE_NAME          "3D-Tool.ini"
#define INI_SECTION_NAME       "EditMap"
#define OPEN_OBJ_MOVE_COOR     "Open Movement Coordinate File"
#define SAVE_OBJ_MOVE_COOR     "Save Movement Coordinate File"
#define COORDINATE_FILE_MASK   "Coordinate Files (*.mcf)\0*.mcf\0\0"
#define COORDINATE_EXT         "mcf"
#define RLE_ENCODED_BMP          "The bitmap file (%s) is a RLE encoded BMP file and can not be used. You will need to have the file re-saved as a standard uncompressed BMP file."
#define CAMERA_STR             "**CAMERA**"
#define CAMERA_MOVE_LST_ID     -1

// Picture load defines
#define NO_JPEG
#define NO_GIF
#define NO_TXT

#define CAMERA_Z_OFFSET        480
#define MAX_CAMERA_OFFSETS     6
#define ANIM_TIMER_HANDLE      3   

// Command defines
#define MNU_EXIT               90                
#define MNU_OPEN_POLYGON       91

#define MNU_SAVE_TXT_LIBRARY   100
#define MNU_SAVE_TXT_COORD     101
#define MNU_SAVEALL            102
#define MNU_CLOSE_POLYGON      103
#define MNU_REINIT_PANELS      104
#define MNU_FLIP_FLOP_ZBUFFER  105
#define MNU_RESET_TO_ZERO      106
#define MNU_ROTATE_BY_OBJECT   107
#define MNU_SAVE_OBJ_MOVE      108
#define MNU_SAVE_CAM_MOVE      109
#define MNU_OPEN_OBJ_MOVE      110
#define MNU_OPEN_CAM_MOVE      111
#define MNU_EXPORT_COMPRESSED  112
#define MNU_EXPORT_BYTE_LIST   113
#define MNU_EXPORT_WORD_LIST   114
#define MNU_EXPORT_INT_LIST    115
#define MNU_RESET_CAMERA       116
#define MNU_SORT_Z_ORDER       117
#define MNU_CALC_LIGHTING      118
#define MAX_MENU_CONTROLS      19

#define MNU_DELETE_TEXTURE     120
#define MNU_ADD_TEXT_TO_LIB    121
#define MNU_MOVE_TEXTURE       122
#define MNU_REBUILD_LIBRARY    123
#define MNU_DEL_DEAD_TXT       124
#define MNU_SELECT_ALL         125
#define MNU_ALL_FORWARD_POLYS  126
#define MNU_1_TO_1_RATIO       127
#define MAX_TEXT_MENU_CONTROLS 8

#define BTN_CANCEL             2

#define RDO_MOVE_CAMERA        200
#define RDO_MOVE_OBJECT        201
#define BTN_OFFSET_MOVE        202
#define BTN_CHANGE_OFFSET_MOVE 203
#define BTN_CAM_OFF_MOVE       204
#define BTN_CAM_CHANGE         205
#define CHK_MOVE_ACCUMULATIVE  206
#define CHK_TEXTURE_MODE       207
#define BTN_ADD_MOVE           208
#define CHK_TRAILS             209
#define BTN_MIN_X              210
#define BTN_PLUS_X             211
#define BTN_MIN_Z              212
#define BTN_PLUS_Z             213
#define BTN_PLUS_Y             214
#define BTN_MIN_Y              215
#define CHK_ROT                216
#define CHK_ROTATE_FROM_PT     217
#define CHK_WIRE_FRAME         218
#define CHK_AS_LIGHT_SOURCE    219
#define MAX_BUTTON_CONTROLS    20

// Move Animation
#define BTN_PLAY_MOVE          220
#define EDT_FPS                221

// Texture Controls
#define LST_TEXTURES           300
#define BTN_ROTATE             301
#define BTN_APPLY_TEXT         302
#define EDT_FRAME_COUNT        303
#define STC_FRAME_RATE         304
#define STC_FRAME_COUNT        305
#define EDT_INTENSITY          307
#define STC_INTENSITY          308
#define EDT_FRAME_RATE         309
#define CHK_TRANS_POLY         310
#define STC_U                  311
#define STC_V                  312
#define STC_U_WIDTH            313
#define STC_V_HEIGHT           314
#define EDT_U                  315
#define EDT_V                  316
#define EDT_U_WIDTH            317
#define EDT_V_HEIGHT           318
#define CHK_NO_CHECK_EXTEN     319
#define CHK_SOLID              320
#define MAX_TEXTURE_EDIT       21

#define DLG_APPLY_TEXTURE      400
#define DLG_MOVE_FRAMES        401

#define EDT_XYTRANS_MOVE_AMT   500
#define EDT_ZTRANS_MOVE_AMT    501
#define MAX_EDIT               2
               
#define GRP_MOVEMENT_BOX       601
#define GRP_OBJ_CRD_BOX        602
#define STC_POLYGON_COUNT      603
#define STC_POLY_RENDER        604
#define STC_RENDER_TIME        605
#define STC_POLYGON_INDEX      606
#define STC_Z                  607
#define STC_XY                 608
#define STC_TRANS_MOVE         609
#define STC_MAX_Z_DIST         610
#define STC_CAM_SIZE           611
#define STC_ANIM_LST           612
#define STC_OBJ_OFF_MOV        613
#define STC_CAM_OFF_MOV        614
#define STC_CAM_ZTRAN          615
#define STC_CAM_YTRAN          616
#define STC_CAM_XTRAN          617
#define STC_CAM_ZROT           618
#define STC_CAM_YROT           619
#define STC_CAM_XROT           620
#define STC_OBJ_ZROT           621
#define STC_OBJ_YROT           622
#define STC_OBJ_XROT           623
#define STC_OBJ_ZTRAN          624
#define STC_OBJ_YTRAN          625
#define STC_OBJ_XTRAN          626
#define STC_YTRAN              627
#define STC_XTRAN              628
#define STC_POLY_OBJ           629
#define STC_TXT_INFO           630
#define STC_CAMERA_COUNT       631
#define STC_OBJECT_MV_COUNT    632
#define STC_MAX_LIGHT_DIST     633
#define STC_WIRE_FRAME         634
#define MAX_STATIC_CONTROLS    34

// Subclassed controls
#define EDT_X_TRAN_CAM         700
#define EDT_Y_TRAN_CAM         701
#define EDT_Z_TRAN_CAM         702
#define EDT_X_ROT_CAM          703
#define EDT_Y_ROT_CAM          704
#define EDT_Z_ROT_CAM          705
#define EDT_X_TRAN_OBJ         706
#define EDT_Y_TRAN_OBJ         707
#define EDT_Z_TRAN_OBJ         708
#define EDT_X_ROT_OBJ          709
#define EDT_Y_ROT_OBJ          710
#define EDT_Z_ROT_OBJ          711
#define STC_VIEW_WND           712
#define LST_POLYGON_OBJS       713
#define LST_CAM_OFFSET_MOVE    714
#define LST_OFFSET_MOVE        715
#define EDT_CAMERA_SIZE        716
#define EDT_MAX_DISTANCE       717
#define EDT_MAX_LITE_DIST      718

#define LST_MOVEMENT           719
#define STC_TEXTURE_PREVIEW    720

// Misc controls
#define STC_PICTLOAD_IMAGE_WND 800
#define STC_PICTLOAD_INFO      801
#define BTN_OK                 802
#define CBO_MOVE_ITEM          803

enum
{
    X_TRAN_CAM=0,
    Y_TRAN_CAM,
    Z_TRAN_CAM,
    X_ROT_CAM,
    Y_ROT_CAM,
    Z_ROT_CAM,
    X_TRAN_OBJ,
    Y_TRAN_OBJ,
    Z_TRAN_OBJ,
    X_ROT_OBJ,
    Y_ROT_OBJ,
    Z_ROT_OBJ,
    MAX_OFFSET_EDT_CTRLS,
    STC_VIEWER=MAX_OFFSET_EDT_CTRLS,
    LST_POLYOBJS,
    LST_CAM_MOVE,
    LST_OFF_MOVE,
    EDT_CAM_SIZE,
    EDT_Z_DIS,
    EDT_LIGHT_DIST,
    LST_MOVE,
    STC_PREVIEW,
    MAX_SUBCLASS_CTRLS,
};

enum
{
    TRANSLATE=1,
    ROTATE,
};

enum
{
    CAMERA_MOVE=0,
    OBJECT_MOVE,
};


enum
{
    CAMERA_EDIT=0,
    MAX_Z_DISTANCE,
    TRANS_EDIT,
    ROT_EDIT,
    MAX_EDIT_CONTROLS,
};


enum
{
    X_TRANS_LBL=0,
    Y_TRANS_LBL,
    Z_TRANS_LBL,
    X_ROT_LBL,
    Y_ROT_LBL,
    Z_ROT_LBL,
    RENDER_TIME,
    POLY_COUNT,
    POLY_TOTAL,
    MAX_LABEL_CONTROLS,
};


#endif // __GDefines_H__