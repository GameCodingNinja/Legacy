/************************************************************************
*    FILE NAME:       rotationscriptdefs.h
*
*    DESCRIPTION:     Rotation script related classes
************************************************************************/  

#ifndef __rotation_script_defs_h__
#define __rotation_script_defs_h__

// Game lib dependencies
#include <scripting/scriptdefs.h>

//////////////////////////////////////
// Rotation Scripts
//////////////////////////////////////

class CScriptSetRotation : public CScript
{
public:

    CScriptSetRotation()
    { type = EST_SET_ROTATION; }
};

class CScriptIncRotation : public CScript
{
public:

    CScriptIncRotation()
    { type = EST_INC_ROTATION; }
};

class CScriptRotateTo : public CScript
{
public:

    CScriptRotateTo()
    { type = EST_ROTATE_TO; }
};

class CScriptRotate : public CScript
{
public:

    CScriptRotate()
    { type = EST_ROTATE; }
};

#endif  // __rotation_script_defs_h__