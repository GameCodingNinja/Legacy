
/************************************************************************
*    FILE NAME:       sprite3d.h
*
*    DESCRIPTION:     3D sprite base class
************************************************************************/

#ifndef __sprite_3d_h__
#define __sprite_3d_h__

// Physical component dependency
#include <common/object.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/point.h>
#include <common/matrix.h>

// Forward declaration(s)
class CMesh3D;

class CSprite3D : public CObject
{
public:

    CSprite3D();
    virtual ~CSprite3D();

    // Get the radius
    float GetRadius();

    // Get the squar root radius
    float GetRadiusSqrt();

    // Test position of object in frustum
    int CullMesh_BoundSphere( CPoint & centerPt, float radius );

    // Do bounding sphere collision check on object center point
    // and radius. This is a squared check so no sqrt is used for speed.
    virtual bool IsCollision_BoundingSphere( CPoint & center, float radius );

    // Is sprite active
    bool IsActive();

    // Render functions
    virtual void Render( CMatrix & matrix ){};
    virtual void RenderShadowMap( CMatrix & matrix ){};

    // Generate perspective projection matrix
    virtual void SetPerspectiveProjMatrix( float minZDist,
                                           float maxZDist,
                                           float shadowBias ){};

    // Generate orthographic projection matrix
    virtual void SetOrthographicProjMatrix( float minZDist,
                                            float maxZDist,
                                            float screenPercentage,
                                            float shadowBias ){};

    // Set the specular properties
    virtual void SetSpecularProperties( float shinniness,
                                        float intensity ){};

    // Set the material color
    virtual void SetMaterialColor( D3DXVECTOR4 & mColor ){};

protected:

    // Mesh pointer
    CMesh3D * pMesh;

};

#endif  // __sprite_h__
