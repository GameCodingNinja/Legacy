/************************************************************************
*    FILE NAME:       visualsprite3d.cpp
*
*    DESCRIPTION:     3D DirectX sprite class
************************************************************************/

// Physical component dependency
#include <3d/visualsprite3d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <managers/meshmanager.h>
#include <managers/shader.h>
#include <system/xdevice.h>
#include <utilities/statcounter.h>
#include <utilities/cullfunc.h>
#include <misc/settings.h>
#include <3d/visualmesh3D.h>
#include <3d/objectdata3d.h>
#include <3d/objectdatalist3d.h>
#include <common/camera.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVisualSprite3D::CVisualSprite3D()
               : pObjectData(NULL),
                 specularShine(50.f),
                 specularIntensity(1.f),
                 materialColor(1.f,1.f,1.f,1.f),
                 projectionType(CSettings::EPT_PERSPECTIVE)
{
}   // Constructer

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVisualSprite3D::CVisualSprite3D( CObjectData3D * pObjData )
               : pObjectData(pObjData),
                 specularShine(50.f),
                 specularIntensity(1.f),
                 materialColor(1.f,1.f,1.f,1.f),
                 projectionType(CSettings::EPT_PERSPECTIVE)
{
    Init();

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CVisualSprite3D::~CVisualSprite3D()
{
}   // Destructer


/************************************************************************
*    desc:  Init the sprite
************************************************************************/
void CVisualSprite3D::Init( const std::string & group, const std::string & type )
{
    pObjectData = CObjectDataList3D::Instance().GetData( group, type );

    Init();

}   // Init


/************************************************************************
*    desc:  Init the sprite
************************************************************************/
void CVisualSprite3D::Init()
{
    pMesh = CMeshMgr::Instance().GetVisualMesh3D( pObjectData );

    // Set the shader effect and technique
    SetEffectAndTechnique( GetVisualData().GetEffect(),
                           GetVisualData().GetTechnique() );

    // Set the color
    SetMaterialColor( GetVisualData().GetColor() );

    // Set up specular lighting
    SetSpecularProperties( GetVisualData().GetSpecularShine(),
                           GetVisualData().GetSpecularIntensity() );

    // Set the projection type
    projectionType = GetVisualData().GetProjectionType();

}   // Init


/************************************************************************
*    desc:  Set the shader effect and technique
************************************************************************/
void CVisualSprite3D::SetEffectAndTechnique( const string & _effectStr, const string & _techniqueStr )
{
    effectStr = _effectStr;
    techniqueStr = _techniqueStr;

}	// SetEffectAndTechnique


/************************************************************************
*    desc:  Render this mesh to the back buffer - This function
*           prepares a matrix for shader lighing
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CVisualSprite3D::Render( CMatrix & matrix )
{
    // Transform the center point - needed for CullMesh_BoundSphere()
    Transform( matrix );

    // Test if this object is within the view frustum
    //if( InPerspectiveView( trans_center, GetRadiusSqrt() ) )
    CPoint tmpTransPos = GetTransPos();
    if( InPerspectiveView( tmpTransPos, GetRadiusSqrt() ) )
    {
        UpdateShader( matrix );

        // Render the mesh
        pMesh->Render();
    }

}	// Render


/************************************************************************
*    desc:  Render this mesh to the shadow map buffer
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CVisualSprite3D::RenderShadowMap( CMatrix & matrix )
{
    // Transform the center point - needed for CullMesh_BoundSphere()
    Transform( matrix );

    // Test if this object is within the view frustum
    //if( InPerspectiveView( trans_center, GetRadiusSqrt() ) )
    CPoint tmpTransPos = GetTransPos();
    if( InPerspectiveView( tmpTransPos, GetRadiusSqrt() ) )
    {
        // Update the shader prior to rendering
        UpdateShadowMapShader();

        // Render the mesh
        pMesh->Render();
    }

}	// Render


/************************************************************************
*    desc:  Update the shader prior to rendering
*
*    param: CMatrix & matrix - passed in matrix class
************************************************************************/
void CVisualSprite3D::UpdateShader( CMatrix & matrix )
{
    // Do our translations and rotations in a temporary matrix
    CMatrix noScaleWorldMatrix;
    CMatrix scaledWorldMatrix;

    // Add in the scaling
    scaledWorldMatrix.Scale( GetScale() );

    // Inc our stat counter to keep track of what is going on.
    CStatCounter::Instance().IncDisplayCounter();

    // Create a world matrix for the shader for lighting
    //SetMatrixToWorldView( noScaleWorldMatrix );
    //SetMatrixToWorldView( scaledWorldMatrix );
    // Do the per-translation rotation
    noScaleWorldMatrix.Rotate( GetRot() );

    // Do the post-rotation translation
    noScaleWorldMatrix.Translate( GetPos() );

    // Apply translation and rotation to the scaled matrix
    scaledWorldMatrix *= noScaleWorldMatrix;

    // Copy it to the DirectX matrix
    D3DXMATRIX worldMatrix( noScaleWorldMatrix() );
    D3DXMATRIX worldMatrixScaled( scaledWorldMatrix() );
    D3DXMATRIX scalCameraMatrix( GetScaledMatrix()() );

    // Create a projection matrix to send to the shader
    D3DXMATRIX cameraViewProjectionMatrix = scalCameraMatrix *
        CXDevice::Instance().GetProjectionMatrix( projectionType );

    // Set up the shader before the rendering
    CEffectData * pEffectData =
        CShader::Instance().SetEffectAndTechnique( effectStr, techniqueStr );

    // Copy the matrix to the shader
    CShader::Instance().SetEffectValue( pEffectData, "worldMatrix", worldMatrix );
    CShader::Instance().SetEffectValue( pEffectData, "scaledWorldMatrix", worldMatrixScaled );
    CShader::Instance().SetEffectValue( pEffectData, "cameraViewProjMatrix", cameraViewProjectionMatrix );
    CShader::Instance().SetEffectValue( pEffectData, "lightViewProjMatrix", lightViewProjMatrix );

    // Copy the specular properties to the shader
    CShader::Instance().SetEffectValue( pEffectData, "specularShine", specularShine );
    CShader::Instance().SetEffectValue( pEffectData, "specularIntensity", specularIntensity );

    // Set the material color
    CShader::Instance().SetEffectValue( pEffectData, "materialColor", materialColor );

    // Get the world view position of the camera
    CPoint cameraPos = matrix.GetMatrixPoint();
    D3DXVECTOR4 cameraPosition;
    //cameraPosition.x = cameraPos.x;
    //cameraPosition.y = cameraPos.y;
    //cameraPosition.z = cameraPos.z;
    cameraPosition.x = CCamera::Instance().GetPos().x;
    cameraPosition.y = CCamera::Instance().GetPos().y;
    cameraPosition.z = CCamera::Instance().GetPos().z;
    cameraPosition.w = 1.f;

    // Send the camera position up to the shader
    CShader::Instance().SetEffectValue( pEffectData, "cameraPosition", cameraPosition );

}	// UpdateShader


/************************************************************************
*    desc:  Update the shader prior to rendering
************************************************************************/
void CVisualSprite3D::UpdateShadowMapShader()
{
    // Do our translations and rotations in a temporary matrix
    CMatrix scaledLightMatrix;

    // Add in the scaling
    scaledLightMatrix.Scale( GetScale() );

    // Merge in the unscaledMatrix matrix to convert to camera view
    scaledLightMatrix *= GetUnscaledMatrix();

    // Copy it to the DirectX matrix
    D3DXMATRIX lightViewMatrix( scaledLightMatrix() );

    // Set a projection matrix to send to the shader
    D3DXMATRIX lightViewProjMatrixBias = lightViewMatrix * shadowMapProjMatrixBias;

    // Set up the shader before the rendering
    CEffectData * pEffectData =
        CShader::Instance().SetEffectAndTechnique( effectStr, techniqueStr );

    // Copy the matrix to the shader
    CShader::Instance().SetEffectValue( pEffectData, "lightViewProjMatrixMinMaxBias", lightViewProjMatrixBias );

    // Set this class member needed for visual render
    lightViewProjMatrix = lightViewMatrix * shadowMapProjMatrix;

}	// UpdateShadowMapShader


/************************************************************************
*    desc:  Generate perspective projection matrix
************************************************************************/
void CVisualSprite3D::SetPerspectiveProjMatrix( float minZDist,
                                                float maxZDist,
                                                float shadowBias )
{
    D3DXMatrixPerspectiveFovLH( &shadowMapProjMatrixBias,
                                CXDevice::Instance().GetViewAngle(),
                                1,
                                minZDist + shadowBias,
                                maxZDist );

    D3DXMatrixPerspectiveFovLH( &shadowMapProjMatrix,
                                CXDevice::Instance().GetViewAngle(),
                                1,
                                minZDist,
                                maxZDist );

}	// SetPerspectiveProjMatrix


/************************************************************************
*    desc:  Generate orthographic projection matrix
************************************************************************/
void CVisualSprite3D::SetOrthographicProjMatrix( float minZDist,
                                                 float maxZDist,
                                                 float screenPercentage,
                                                 float shadowBias )
{
    D3DXMatrixOrthoLH( &shadowMapProjMatrixBias,
        screenPercentage * CSettings::Instance().GetShadowMapBufferSize().w,
        screenPercentage * CSettings::Instance().GetShadowMapBufferSize().h,
        minZDist + shadowBias,
        maxZDist);

    D3DXMatrixOrthoLH( &shadowMapProjMatrix,
        screenPercentage * CSettings::Instance().GetShadowMapBufferSize().w,
        screenPercentage * CSettings::Instance().GetShadowMapBufferSize().h,
        minZDist,
        maxZDist);

}	// SetPerspectiveProjMatrix


/************************************************************************
*    desc:  Set the specular properties
************************************************************************/
void CVisualSprite3D::SetSpecularProperties( float shine, float intensity )
{
    specularShine = shine;
    specularIntensity = intensity;
}


/************************************************************************
*    desc:  Set the material color
*
*	 param:	CColor & color - color to set to
************************************************************************/
void CVisualSprite3D::SetMaterialColor( const CColor & color )
{
    materialColor.x = color.r;
    materialColor.y = color.g;
    materialColor.z = color.b;
    materialColor.w = color.a;

}	// SetMaterialColor


/************************************************************************
*    desc:  Get the visual data
*
*    ret:	const CObjectVisualData2D & - visual data
************************************************************************/
const CObjectVisualData3D & CVisualSprite3D::GetVisualData() const
{
    return pObjectData->GetVisualData();

}	// GetVisualData


