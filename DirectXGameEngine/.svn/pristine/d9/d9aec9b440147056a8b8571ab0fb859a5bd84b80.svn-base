/************************************************************************
*    FILE NAME:       effectdata.h
*
*    DESCRIPTION:     effects data class
************************************************************************/

#ifndef __effect_data_h__
#define __effect_data_h__

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include "d3dx9effect.h"

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

enum EEffectType
{
    EET_NULL,
    EET_BOOL,
    EET_INT,
    EET_FLOAT,
    EET_VECTOR,
    EET_MATRIX,
    EET_TEXTURE,
    EET_BOOL_ARRAY,
    EET_INT_ARRAY,
    EET_FLOAT_ARRAY,
    EET_VECTOR_ARRAY,
    EET_MATRIX_ARRAY,
    MAX_EFFECTS_TYPES
};


class CEffectType
{
public:

    CEffectType()
    {
        handle = NULL;
        type = EET_NULL;
        arrayCount = 1;
    }

    // Handle to effects variable
    // It doesn't seem like this handle needs to be released
    D3DXHANDLE handle;

    // effects data type
    EEffectType type;

    // array count
    unsigned int arrayCount;
};


class CEffectData : public boost::noncopyable
{
public:

    // Active effects
    CComPtr<ID3DXEffect> spEffect;

    // String id of effect for easy access to effect name
    std::string effectNameStr;

    // Currently active technique for this effect
    std::string activeTechniqueStr;

    // map list of effects data types
    boost::ptr_map<std::string, CEffectType> spEffectTypeMap;
};

#endif  // __effect_data_h__
