
/************************************************************************
*    FILE NAME:       light.h
*
*    DESCRIPTION:     3D light class
************************************************************************/

#ifndef __light_h__
#define __light_h__

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <xmlParser/xmlParser.h>

// Make sure this matches in shader file
#define MAX_LIGHTS 3  // 12 max for shader 2

enum ELightType
{
    ELT_NONE=0,
    ELT_DIRECTIONAL,
    ELT_POINT_INFINITE,
    ELT_POINT_RADIUS,
    ELT_AMBIENT
};

class CLight : public CObject
{
public:

    // Constructor
    CLight( ELightType lightType );

    // Destructer
    ~CLight();

    // Load the light info from XML file
    void LoadFromXML( XMLNode & lightNode );

    // Set/Get enable
    void SetEnable( bool value );
    bool GetEnable();

    // Set/Get the light type
    void SetType( ELightType value );
    ELightType GetType();

    // Set/Get direction
    void SetDirection( CPoint & value );
    CPoint & GetDirection();

    // Set/Get the cast shadow
    void SetCastShadow( bool value );
    bool GetCastShadow();

    // Set/Get diffuse
    void SetDiffuse( CPoint & value );
    CPoint & GetDiffuse();

    // Set/Get rotation
    void SetRotation( CPoint & value );
    CPoint & GetRotation();

    // Set/Get the radius
    void SetRadius( float value );
    float GetRadius();

private:

    // Light type
    ELightType type;

    // Light direction
    CPoint direction;

    // is light on or off
    bool enable;

    // Does this light cast a shadow
    bool castShadow;

    // Defuse array
    CPoint diffuse;

    // Light rotation
    CPoint rotation;

    // Radius
    float radius;

};

#endif  // __light_h__
