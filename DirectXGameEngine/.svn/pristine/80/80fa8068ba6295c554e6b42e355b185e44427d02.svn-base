
/************************************************************************
*    FILE NAME:       visualsprite3d.h
*
*    DESCRIPTION:     3D DirectX sprite class
************************************************************************/

#ifndef __visualsprite_3d_h__
#define __visualsprite_3d_h__

// Physical component dependency
#include <3d/sprite3d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <3d/objectdata3d.h>
#include <misc/settings.h>

// Forward Declarations
class CEffectData;
class CObjectData3D;
class CObjectVisualData3D;

class CVisualSprite3D : public CSprite3D
{
public:

    CVisualSprite3D();
    CVisualSprite3D( CObjectData3D * pObjData );
    virtual ~CVisualSprite3D();

    // Init the sprite with the object data
    virtual void Init( const std::string & group, const std::string & type );

    // Set the shader effect and technique
    void SetEffectAndTechnique( const std::string & _effectStr, const std::string & _techniqueStr );

    // Set the material color
    void SetMaterialColor( const CColor & color );

    // Render this mesh to the back buffer
    virtual void Render( CMatrix & matrix );

    // Render this mesh to the shadow map buffer
    virtual void RenderShadowMap( CMatrix & matrix );

    // Generate perspective projection matrix
    virtual void SetPerspectiveProjMatrix( float minZDist,
                                           float maxZDist,
                                           float shadowBias );

    // Generate orthographic projection matrix
    virtual void SetOrthographicProjMatrix( float minZDist,
                                            float maxZDist,
                                            float screenPercentage,
                                            float shadowBias );

    // Set the specular properties
    virtual void SetSpecularProperties( float shine,
                                        float intensity );

    // Get the object data components
    const CObjectVisualData3D & GetVisualData() const;

protected:

    // Update the shader prior to rendering
    virtual void UpdateShader( CMatrix & matrix );

    // Update the shader for light projection
    virtual void UpdateShadowMapShader();

private:

    // Init the sprite
    void Init();

protected:

    // Object's data. 
    // NOTE: This pointer belongs to the CObjectDataList2D singleton
    CObjectData3D * pObjectData;

    // light view projection matrix
    D3DXMATRIX lightViewProjMatrix;

    // Generated projection matrix
    D3DXMATRIX shadowMapProjMatrix;
    D3DXMATRIX shadowMapProjMatrixBias;

    // Specular light members
    float specularShine;
    float specularIntensity;

    // Material color
    D3DXVECTOR4 materialColor;

    // Name of effect string
    std::string effectStr;

    // Name of shader technique to use with effect
    std::string techniqueStr;

    // The projection type of the visual sprite
    CSettings::EProjectionType projectionType;

};

#endif  // __visualsprite_h__
