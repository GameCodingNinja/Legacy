#************************************************************************
#    FILE NAME:       io_export_rabidsquirrel_header.py
#
#    DESCRIPTION:     Header to sprite files outputed in binary
#***********************************************************************/

from ctypes import *

# Hex for RSS (Rabbid Squirrel Sprite)
SPRITE_FILE_HEADER = 0x535352

# Hex for VTL (VerT List)
VERT_LIST = 0x4C5456

# Hex for UVL (UV List)
UV_LIST = 0x4C5655

# Hex for VNL (Vert Normal List)
VERT_NORM_LIST = 0x4C4E56

# Hex for FG (Face Group)
FACE_GROUP = 0x4746

# Hex for FL (Face List)
FACE_LIST = 0x4C46

# Hex for ML (Material List)
MAT_LIST = 0x4C4D

# Hex for JL (Joint List)
JOINT_LIST = 0x4C4A

# Max character sizes for the material path and joint name
TEXTURE_PATH_SIZE = 128
JOINT_NAME_SIZE = 20

# Hex for RSA (Rabid Squirrel Animation)
ANIMATION_FILE_HEADER = 0x415352

# Hex for JNT (Joint)
JOINT = 0x544E4A

# Hex for KEY (KEYframe)
KEY_FRAME = 0x59454B

# Texture types
TEXTURE_NULL           = 0
TEXTURE_DIFFUSE        = 1
TEXTURE_NORMAL         = 2
TEXTURE_SPECULAR       = 3
TEXTURE_DISPLACEMENT   = 4
TEXTURE_MAX_TEXT_TYPES = 5

# Header information for meshs
class CSpriteBinaryFileHeader(Structure):
     _fields_ = [ ('file_header',c_int),
                  ('vert_count',c_int),
                  ('uv_count',c_int),
                  ('vert_norm_count',c_int),
                  ('face_group_count',c_int),
                  ('mat_count',c_int),
                  ('joint_count',c_int) ]

# Class for writing the vert list
class CBinaryVert(Structure):
     _fields_ = [ ('x',c_float),
                  ('y',c_float),
                  ('z',c_float) ]

class CBinaryVertBone(Structure):
     _fields_ = [ ('x',c_float),
                  ('y',c_float),
                  ('z',c_float),
                  ('w',c_float),
                  ('b',c_int) ]

# Class for writing the uv list
class CBinaryUV(Structure):
     _fields_ = [ ('u',c_float),
                  ('v',c_float) ]

# Class for writing face information to a collision mesh or shadow mesh
class CBinaryVertFace(Structure):
     _fields_ = [ ('vert_1',c_int),
                  ('vert_2',c_int),
                  ('vert_3',c_int) ]

# Class for writing face information to a visual mesh or animated mesh
class CBinaryFace(Structure):
     _fields_ = [ ('vert_1',c_int),
                  ('vert_2',c_int),
                  ('vert_3',c_int),
                  ('uv_1',c_int),
                  ('uv_2',c_int),
                  ('uv_3',c_int),
                  ('norm_1',c_int),
                  ('norm_2',c_int),
                  ('norm_3',c_int) ]

# Class for writing the total face count within a group and the material it belongs to
class CBinaryFaceGroup(Structure):
     _fields_ = [ ('groupFaceCount',c_int),
                  ('textureCount',c_int) ]

# Class for writing texture indexes
class CBinaryTextIndex(Structure):
     _fields_ = [ ('textureIndex',c_int) ]

# Tag check
class CTagCheck(Structure):
     _fields_ = [ ('tagCheck',c_int) ]

# Face count
class CFaceCount(Structure):
     _fields_ = [ ('faceCount',c_int) ]

# Texture type and path
class CTexturePath(Structure):
     _fields_ = [ ('textureType',c_int),
                  ('path',c_char * TEXTURE_PATH_SIZE) ]

# Joint binary array
class CBinaryJoint(Structure):
     _fields_ = [ ('vert_count',c_int),
                  ('name',c_char * JOINT_NAME_SIZE),
                  ('parent',c_char * JOINT_NAME_SIZE),
                  ('headX',c_float), ('headY',c_float), ('headZ',c_float),
                  ('tailX',c_float), ('tailY',c_float), ('tailZ',c_float),
                  ('m00',c_float), ('m01',c_float), ('m02',c_float), ('m03',c_float),
                  ('m10',c_float), ('m11',c_float), ('m12',c_float), ('m13',c_float),
                  ('m20',c_float), ('m21',c_float), ('m22',c_float), ('m23',c_float),
                  ('m30',c_float), ('m31',c_float), ('m32',c_float), ('m33',c_float) ]

# Class for writing the header on an animation file
class CSpriteAnimationBinaryFileHeader(Structure):
     _fields_ = [ ('file_header',c_int),
                  ('bone_count',c_int),
                  ('frame_count',c_int)]

# Class for reading and writing the joint information to an animation file
class CBinaryJointAnimation(Structure):
     _fields_ = [ ('keyframe_count',c_int),
                  ('name',c_char * JOINT_NAME_SIZE),
                  ('parent',c_char * JOINT_NAME_SIZE)]

# Class for reading and writing the keyframe information to an animation file
class CBinaryKeyFrame(Structure):
     _fields_ = [ ('time',c_float),
                  ('x',c_float),
                  ('y',c_float),
                  ('z',c_float),
                  ('rx',c_float),
                  ('ry',c_float),
                  ('rz',c_float)]


