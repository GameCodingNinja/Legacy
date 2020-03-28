/************************************************************************
*    FILE NAME:       spritebinaryfileheader.h
*
*    DESCRIPTION:     Header to sprite files outputed in binary
************************************************************************/

#ifndef __spritebinaryfileheader_h__
#define __spritebinaryfileheader_h__

#include <common/point.h>
#include <common/texture.h>

// Hex for RSS (Rabbid Squirrel Sprite)
const int SPRITE_FILE_HEADER = 0x535352;

// Hex for VTL (VerT List)
const int VERT_LIST = 0x4C5456;

// Hex for UVL (UV List)
const int UV_LIST = 0x4C5655;

// Hex for VNL (Vert Normal List)
const int VERT_NORM_LIST = 0x4C4E56;

// Hex for FG (Face Group)
const int FACE_GROUP = 0x4746;

// Hex for FL (Face List)
const int FACE_LIST = 0x4C46;

// Hex for ML (Material List)
const int MAT_LIST = 0x4C4D;

// Hex for JL (Joint List)
const int JOINT_LIST = 0x4C4A;

// Max character sizes for the texture path and joint name
const int TEXT_PATH_SIZE = 128;
const int JOINT_NAME_SIZE = 20;

// Header information for meshs
class CSpriteBinaryFileHeader
{
public:

    CSpriteBinaryFileHeader() : file_header(SPRITE_FILE_HEADER)
    {};
    
    int file_header;
    int vert_count;
    int uv_count;
    int vert_norm_count;
    int face_group_count;
    int text_count;
    int joint_count;

};


// class for reading in texture info
class CBinaryTexture
{
public:

    NText::ETextureType type;
    char path[TEXT_PATH_SIZE];
};


// Class for reading and writing the joint information to an animated mesh.
class CBinaryJoint
{
public:

    int vert_count;
    char name[JOINT_NAME_SIZE];
    char parentName[JOINT_NAME_SIZE];
    CPoint headPos;
    CPoint tailPos;
    float orientation[4][4];
};


// Hex for RSA (Rabid Squirrel Animation)
const int ANIMATION_FILE_HEADER = 0x415352;

// Hex for JNT (JoiNT)
const int JOINT = 0x544E4A;

// Hex for KEY (KEYframe)
const int KEY_FRAME = 0x59454B;

// Class for writing the header on an animation file
class CSpriteAnimationBinaryFileHeader
{
public:

    CSpriteAnimationBinaryFileHeader() : file_header(ANIMATION_FILE_HEADER)
    {};
    
    int file_header;
    int joint_count;
    int frame_count;

};

// Class for reading and writing the joint information to an animation file
class CBinaryJointAnimation
{
public:

    int keyframe_count;
    char name[JOINT_NAME_SIZE];
    char parentName[JOINT_NAME_SIZE];

};

// Class for reading and writing the keyframe information to an animation file
class CBinaryKeyFrame
{
public:

    float time, x, y, z, rx, ry, rz;

};

// Class for reading and writing the total face count within a group and the material
// it belongs to
class CBinaryFaceGroup
{
public:

    int groupFaceCount, textureCount;

};

// Class for reading and writing face information to a collision mesh or shadow mesh
class CBinaryVertFace
{
public:

    int vert[3];

};

// Class for reading and writing face information to a visual mesh or animated mesh
class CBinaryFace : public CBinaryVertFace
{
public:

    int uv[3];
    int normal[3];

};


#endif  // __spritebinaryfileheader_h__