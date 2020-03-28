/************************************************************************
*    FILE NAME:       objectinfo.h
*
*    DESCRIPTION:     Actor Information class
************************************************************************/

#ifndef __objectinfo_h__
#define __objectinfo_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include "collisionstats.h"
#include "misc\\settings.h"

class CObjectInfo
{
public:

    class CShaderStrInfo
    {
    public:
        std::string effectStr;
        std::string techniqueStr;
    };

    CObjectInfo() : collidePlayer(false),
                    collideObj(false),
                    colideActive(true),
                    gravity(false),
                    staticObj(false),
                    castShadow(false),
                    animated(false),
                    scale(1.0f),
                    specularShinniness(20.f),
                    specularIntensity(1.f),
                    materialColor(1.f,1.f,1.f,1.f)
    {
    };

    ~CObjectInfo(){};

    void SetInfo( CObjectInfo * pInfo )
    {
        scale = pInfo->scale;
        colideActive = pInfo->colideActive;
        collideObj = pInfo->collideObj;
        collidePlayer = pInfo->collidePlayer;
        gravity = pInfo->gravity;
        castShadow = pInfo->castShadow;
        staticObj = pInfo->staticObj;
        defaultAnimation = pInfo->defaultAnimation;
        collisionStats.SetFloorPad( pInfo->collisionStats.GetFloorPad() );
        collisionStats.SetWallPad( pInfo->collisionStats.GetWallPad() );
        shaderLights = pInfo->shaderLights;
        shaderShadowMap = pInfo->shaderShadowMap;
        animated = pInfo->animated;

        visual_filePath = pInfo->visual_filePath;
        collision_filePath = pInfo->collision_filePath;
        other_filePath = pInfo->other_filePath;
        alpha_filePath = pInfo->alpha_filePath;

        shadowProjType = pInfo->shadowProjType;
        shadowMinZDist = pInfo->shadowMinZDist;
        shadowMaxZDist = pInfo->shadowMaxZDist;
        shadowBias = pInfo->shadowBias;
        shadowOthoScrnPercent = pInfo->shadowOthoScrnPercent;

        specularShinniness = pInfo->specularShinniness;
        specularIntensity = pInfo->specularIntensity;

        materialColor = pInfo->materialColor;
    }

    bool collidePlayer;
    bool collideObj;
    bool colideActive;
    bool gravity;
    bool staticObj;
    bool castShadow;
    bool animated;

    CShaderStrInfo shaderLights;
    CShaderStrInfo shaderShadowMap;

    std::string defaultAnimation;

    std::string visual_filePath;
    std::string collision_filePath;
    std::string other_filePath;
    std::string alpha_filePath;

    CCollisionStats collisionStats;

    float scale;

    CSettings::EProjectionType shadowProjType;
    float shadowMinZDist;
    float shadowMaxZDist;
    float shadowBias;
    float shadowOthoScrnPercent;

    // Specular light members
    float specularShinniness;
    float specularIntensity;

    // Material color
    D3DXVECTOR4 materialColor;

};
#endif  // __objectinfo_h__