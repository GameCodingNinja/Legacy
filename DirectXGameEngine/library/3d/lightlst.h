
/************************************************************************
*    FILE NAME:       lightlst.h
*
*    DESCRIPTION:     3D light list class
************************************************************************/

#ifndef __light_lst_h__
#define __light_lst_h__

// Standard lib dependencies
#include <map>
#include <vector>
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/point.h>
#include <3d/light.h>

// Forward declaration(s)
class CEffectData;

class CLightLst
{
public:

    CLightLst();
    ~CLightLst();

    // Load the lights from XML file
    void LoadFromXML( std::string & filePath );

    // Free the light list
    void Free();

    // Turn the light on or off
    void Enable( std::string & name,
                 bool enable = true );

    // Get the number of lights
    size_t GetCount();

    // Get the light
    CLight * GetLight( int index );

    // Check if the light is on
    bool IsLightOn( std::string & name );

    // Do any of these lights cast a shadow
    bool IsShadowCast();

    // Get the position of the shadow casting light
    CPoint & GetShadowCastLightPos();

    // Get the rotation of the shadow casting light
    CPoint & GetShadowCastLightRot();

    // Add a light to the light list
    void AddLight( std::string & name, CLight * pLight );

    // Set the shader lights
    void SetShaderLights( CEffectData * pEffectData );

private:

    // map list of lights
    boost::ptr_map<std::string, CLight> spLightLstMap;
    boost::ptr_map<std::string, CLight>::iterator lightLstMapIter;

    // Does NOT own pointer
    std::vector<CLight *> lightVecLst;

    // map list of light types
    std::map<std::string, ELightType> lightTypeMap;
    std::map<std::string, ELightType>::iterator lightTypeIter;

    // Shadow cast light
    CLight * shadowCastingLight;

    // Dummy class member
    CPoint lightToCamera;

};

#endif  // __light_lst_h__
