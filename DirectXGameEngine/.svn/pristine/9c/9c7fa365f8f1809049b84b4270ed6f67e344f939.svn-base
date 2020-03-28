/************************************************************************
*    FILE NAME:       visualsprite2d.cpp
*
*    DESCRIPTION:     2D DirectX sprite class
************************************************************************/

// Physical component dependency
#include <2d/visualsprite2d.h>

// Standard lib dependencies
#include <string>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <managers/meshmanager.h>
#include <managers/shader.h>
#include <system/xdevice.h>
#include <2d/visualmesh2d.h>
#include <2d/objectdata2d.h>
#include <2d/objectdatalist2d.h>
#include <utilities/cullfunc.h>
#include <utilities/statcounter.h>
#include <common/texture.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVisualSprite2D::CVisualSprite2D()
                : pObjectData(NULL),
                  pMesh(NULL),
                  projectionType(CSettings::EPT_ORTHOGRAPHIC),
                  currentFrame(0),
                  useCamera(false)

{
}   // Constructer

CVisualSprite2D::CVisualSprite2D( CObjectData2D * pObjData, bool camera, const std::string & meshUniqueTag )
                : pObjectData(pObjData),
                  pMesh(NULL),
                  projectionType(CSettings::EPT_ORTHOGRAPHIC),
                  currentFrame(0),
                  useCamera(false)

{
    Init( camera, meshUniqueTag );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CVisualSprite2D::~CVisualSprite2D()
{
}   // Destructer


/************************************************************************
*    desc:  Initialize the sprite with the object data
*  
*    param: bool camera			   - whether or not the sprite is transformed
*									 by the camera
*			string & meshUniqueTag - the unique tag for the mesh
************************************************************************/
void CVisualSprite2D::Init( const std::string & group,
                            const std::string & type,
                            bool camera,
                            const std::string & meshTag )
{
    // Init our pointer to this objects data. NOTE: We don't own this pointer
    pObjectData = CObjectDataList2D::Instance().GetData( group, type );

    Init( camera, meshTag );

}	// Init


/************************************************************************
*    desc:  Initialize the sprite with the object data
*  
*    param: bool camera			   - whether or not the sprite is transformed
*									 by the camera
*			string & meshUniqueTag - the unique tag for the mesh
************************************************************************/
void CVisualSprite2D::Init( bool camera, const std::string & meshUniqueTag )
{
    // NOTE: This class does not own this pointer.
    pMesh = CMeshMgr::Instance().GetVisualMesh2D( pObjectData, meshUniqueTag );

    if( GetVisualData().GetSize().w <= 0 || GetVisualData().GetSize().h <= 0 ) 
        pObjectData->GetVisualData().SetSize( pMesh->GetSize() );

    // Set the shader effect and technique
    SetEffectAndTechnique( GetVisualData().GetEffect(),
                           GetVisualData().GetTechnique() );

    // Set the default color
    spriteColor = GetVisualData().GetColor();

    // Set the projection type
    projectionType = GetVisualData().GetProjectionType();

    // Set the use camera flag
    useCamera = camera;

    // Set the radius
    SetRadius( pMesh->GetRadius() );

}   // Init


/************************************************************************
*    desc:  Set the shader effect and technique
*
*	 param:	const string & _effectStr    - effect to set to
*			const string & _techniqueStr - technique to set to
************************************************************************/
void CVisualSprite2D::SetEffectAndTechnique( const std::string & _effectStr, const std::string & _techniqueStr )
{
    effectStr = _effectStr;
    techniqueStr = _techniqueStr;

}	// SetEffectAndTechnique


/************************************************************************
*    desc:  Update the shader prior to rendering
*
*	 param:	const CColor & _color - color to multiply against the visual
*									sprite's color
************************************************************************/
void CVisualSprite2D::UpdateShader( const CColor & color )
{
    // Increment our stat counter to keep track of what is going on.
    CStatCounter::Instance().IncDisplayCounter();
    
    CWorldPoint finalPos = GetTransPos();

    // If this object is translated by the camera, add the camera's position
    // to its position
    if( useCamera ) 
        finalPos += CWorldCamera::Instance().GetPos();

    // Copy it to the DirectX matrix
    D3DXMATRIX scalCameraMatrix( GetScaledMatrix()() );
    scalCameraMatrix._41 = finalPos.x;
    scalCameraMatrix._42 = finalPos.y;
    scalCameraMatrix._43 = finalPos.z;

    // Create a projection matrix to send to the shader
    D3DXMATRIX cameraViewProjectionMatrix = scalCameraMatrix *
        CXDevice::Instance().GetProjectionMatrix( projectionType );

    // Set up the shader before the rendering
    CEffectData * pEffectData = CShader::Instance().SetEffectAndTechnique( effectStr, techniqueStr );

    // Copy the matrix to the shader
    CShader::Instance().SetEffectValue( pEffectData, "cameraViewProjMatrix", cameraViewProjectionMatrix );

    // Set the material color
    CShader::Instance().SetEffectValue( pEffectData, "materialColor", color() );

}	// UpdateShader


/************************************************************************
*    desc:  Transform the sprite
************************************************************************/
void CVisualSprite2D::Transform()
{
    // Transform the sprite
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
        CObject::Transform();

}	// Transform


/************************************************************************
*    desc:  Transform the sprite
*
*	 param: const CMatrix & _scaledMatrix   - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point       - passed in relative position
************************************************************************/
void CVisualSprite2D::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                                 const CWorldPoint & point )
{
    // Transform the sprite
    if( GetParameters().IsSet( CObject::TRANSFORM ) )
        CObject::Transform( _scaledMatrix, _unscaledMatrix, point );

}	// Transform


/************************************************************************
*    desc:  Render this mesh to the back buffer
*
*	 param:	const CColor & _color - color to multiply against
*			bool inView			  - if we've pre-determined that the sprite
*									is in view
************************************************************************/
void CVisualSprite2D::Render( const CColor & color, bool inView )
{
    if( IsVisible() )
    {
        // If it's in view, we render it
        if( inView || InView() )
        {
            UpdateShader( GetSpriteColor() * color );

            // Render the mesh
            pMesh->Render(currentFrame);
        }
    }

}	// Render


/***************************************************************************
*    desc:	Get a face
*
*	 param:	int index - index of the face
*
*    ret:	CXFace2D & - face to return
************************************************************************/
CXFace2D & CVisualSprite2D::GetFace( int index )
{
    return pMesh->GetFace(index);

}	// GetFace


/************************************************************************
*    desc:  Set the default color
*
*	 param:	const CColor & color - color to set to
************************************************************************/
void CVisualSprite2D::SetSpriteColor( const CColor & color )
{
    spriteColor = color;

}	// SetSpriteColor


/************************************************************************
*    desc:  Get the Sprite color
*
*	 ret:	const CColor & - color to return
************************************************************************/
const CColor & CVisualSprite2D::GetSpriteColor() const
{
    return spriteColor;

}	// GetSpriteColor


/************************************************************************
*	 desc:  Is this sprite active
*  
*    ret:	bool - if the sprite is active
************************************************************************/
bool CVisualSprite2D::IsActive() const
{
    return (pMesh != NULL);

}   // IsActive


/************************************************************************
*    desc:  Is the sprite in the view frustum
*  
*    ret:	bool - if the sprite is in the view frustum
************************************************************************/
bool CVisualSprite2D::InView()
{
    // Test if this object is within the view frustum
    bool inView = false;

    CPoint tmp;

    if( useCamera )
        tmp = CWorldCamera::Instance().GetPos() + GetTransPos();
    else
        tmp = GetTransPos();

    if( projectionType == CSettings::EPT_ORTHOGRAPHIC )
        inView = InOrthographicView( tmp, GetRadius() );
    else if( projectionType == CSettings::EPT_PERSPECTIVE )
        inView = InPerspectiveView( tmp, GetRadius() );

    return inView;

}	// InView


/************************************************************************
*    desc:  Get the size of the sprite
*
*	 param: bool scaled - whether or not to return the size with or without
*						  scale
*
*    ret:	CSize<float> - size
************************************************************************/
CSize<float> CVisualSprite2D::GetSize( bool scaled ) const
{
    if( !scaled )
        return pMesh->GetSize();

    CSize<float> tmpSize = pMesh->GetSize();
    CPoint tmpScale = GetScale();
    tmpSize.w *= tmpScale.x;
    tmpSize.h *= tmpScale.y;

    return tmpSize;

}	// GetSize


/***************************************************************************
*	 desc:	Get the material
*
*	 param: int i - index of the material
*
*	 ret:   LPDIRECT3DTEXTURE9 - material to return
 ****************************************************************************/
LPDIRECT3DTEXTURE9 CVisualSprite2D::GetMaterial( int i )
{
    return pMesh->GetMaterial(i);

}	// GetMaterial


/***************************************************************************
*	 desc:	Get the texture
*
*    ret:	NText::CTextureFor2D * - texture to return
 ****************************************************************************/
NText::CTextureFor2D * CVisualSprite2D::GetTexture( int i )
{
    return pMesh->GetTexture(i);

}	// GetTexture


/************************************************************************
*    desc:  Get the frame count
*  
*    ret:	int - frame count
************************************************************************/
int CVisualSprite2D::GetFrameCount() const
{
    return static_cast<int>(pMesh->GetMaterialCount());

}	// GetFrameCount


/************************************************************************
*	 desc:  Set the current frame
*  
*    param:	int frame - frame to set to
************************************************************************/
void CVisualSprite2D::SetCurrentFrame( int frame )
{
    currentFrame = frame;

}	// SetCurrentFrame


/************************************************************************
*    desc:  Increment the current frame
*  
*    param: int frame - ammount to increment by
************************************************************************/
void CVisualSprite2D::IncCurrentFrame( int frame )
{
    currentFrame += frame;

}	// IncCurrentFrame


/************************************************************************
*    desc:  Get the current frame
*  
*    ret:	int - current frame
************************************************************************/
int CVisualSprite2D::GetCurrentFrame() const
{
    return currentFrame;

}	// GetCurrentFrame


/************************************************************************
*    desc:  Set the frame timer
*  
*    param: float value - time to set to
************************************************************************/
void CVisualSprite2D::SetFrameTimer( float value )
{
    frameTimer = value;

}	// SetFrameTimer


/************************************************************************
*    desc:  Increment the frame timer
*  
*    param: float value - time to increment by
************************************************************************/
void CVisualSprite2D::IncFrameTimer( float value )
{
    frameTimer += value;

}	// IncFrameTimer


/************************************************************************
*    desc:  Get the frame timer  
*
*	 ret:	float - time elapsed in the frame
************************************************************************/
float CVisualSprite2D::GetFrameTimer() const
{
    return frameTimer;

}	// GetFrameTimer


/****************************************************************************
*    desc:  Updates the mesh's vertex buffer
****************************************************************************/
void CVisualSprite2D::UpdateVertBuf()
{
    pMesh->UpdateVertBuf();

}	// UpdateVertBuf


/************************************************************************
*	 desc:  Get the projection type
*  
*	 ret:	CSettings::EProjectionType - projection type
************************************************************************/
CSettings::EProjectionType CVisualSprite2D::GetProjectionType()
{
    return projectionType;

}	// GetProjectionType


/************************************************************************
*    desc:  Get the active texture
*  
*    ret:	NText::CTextureFor2D * - projection type
************************************************************************/
NText::CTextureFor2D * CVisualSprite2D::GetActiveTexture()
{
    return pMesh->GetTexture(currentFrame);

}	// GetActiveTexture


/************************************************************************
*    desc:  If the camera transforms the sprite  
*
*	 ret:	bool - whether the camera is used
************************************************************************/
bool CVisualSprite2D::IsCameraUsed() const
{
    return useCamera;

}	// IsCameraUsed


/************************************************************************
*    desc:  Get the visual data
*
*    ret:	const CObjectVisualData2D & - visual data
************************************************************************/
const CObjectVisualData2D & CVisualSprite2D::GetVisualData() const
{
    return pObjectData->GetVisualData();

}	// GetVisualData