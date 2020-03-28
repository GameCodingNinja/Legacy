#************************************************************************
#    FILE NAME:       fileExportHeader.py
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
MAT_PATH_SIZE = 128
JOINT_NAME_SIZE = 16

# Hex for RSA (Rabid Squirrel Animation)
ANIMATION_FILE_HEADER = 0x415352

# Hex for JNT (JoiNT)
JOINT = 0x544E4A

# Hex for KEY (KEYframe)
KEY_FRAME = 0x59454B

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

# Class for writing the uv list
class CBinaryUV(Structure):
     _fields_ = [ ('u',c_float),
                  ('v',c_float) ]

# Class for writing face information to a collision mesh or shadow mesh
class CBinaryVertFace(Structure):
     _fields_ = [ ('vert_x',c_int),
                  ('vert_y',c_int),
                  ('vert_z',c_int) ]

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

class CBinaryKeyFrame(Structure):
     _fields_ = [ ('time',c_float),
                  ('x',c_float),
                  ('y',c_float),
                  ('z',c_float),
                  ('rx',c_float),
                  ('ry',c_float),
                  ('rz',c_float) ]

# Class for writing the total face count within a group and the material it belongs to
class CBinaryFaceGroup(Structure):
     _fields_ = [ ('groupFaceCount',c_int),
                  ('matIndex',c_int) ]

# Tag check
class CTagCheck(Structure):
     _fields_ = [ ('tagCheck',c_int) ]

# Material path char array
class CMaterialPath(Structure):
     _fields_ = [ ('path',c_char * MAT_PATH_SIZE) ]


