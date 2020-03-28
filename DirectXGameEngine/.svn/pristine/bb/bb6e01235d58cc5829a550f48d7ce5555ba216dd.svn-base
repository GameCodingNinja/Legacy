/************************************************************************
*    FILE NAME:       visualsprite2d.h
*
*    DESCRIPTION:     2D DirectX sprite class
************************************************************************/

#ifndef __visualsprite_2d_h__
#define __visualsprite_2d_h__

// Physical component dependency
#include <common/object.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/matrix.h>
#include <common/size.h>
#include <common/color.h>
#include <common/xface2d.h>
#include <3d/worldcamera.h>
#include <misc/settings.h>

// Forward Declarations
class CVisualMesh2D;
class CObjectData2D;
class CObjectVisualData2D;
namespace NText { class CTextureFor2D; }

class CVisualSprite2D : public CObject
{
public:

    CVisualSprite2D();
    CVisualSprite2D( CObjectData2D * pObjData, bool camera = false, const std::string & meshUniqueTag = std::string("") );
    virtual ~CVisualSprite2D();

    // Initialize the sprite with the object data
    void Init( const std::string & group,
               const std::string & type,
               bool camera = false,
               const std::string & meshTag = std::string("") );
    void Init( bool camera = false,
               const std::string & meshTag = std::string("") );

    // Set the shader effect and technique
    void SetEffectAndTechnique( const std::string & _effectStr, const std::string & _techniqueStr );

    // Transform the sprite
    virtual void Transform();
    virtual void Transform( const CMatrix & _scaledMatrix,
                            const CMatrix & _unscaledMatrix, 
                            const CWorldPoint & point );

    // Get the object data components
    const CObjectVisualData2D & GetVisualData() const;

    // Render this mesh to the back buffer
    void Render( const CColor & color = CColor(1,1,1,1), bool inView = false );

    // Get a face
    CXFace2D & GetFace( int index );

    // Set the material color
    void SetSpriteColor( const CColor & color );
    const CColor & GetSpriteColor() const;

    // Is the sprite active
    bool IsActive() const;

    // Is the sprite in the view frustum
    bool InView();

    // Get the size of the sprite
    CSize<float> GetSize( bool scaled = true ) const;

    // Get the material
    LPDIRECT3DTEXTURE9 GetMaterial( int i );

    // Get the texture
    NText::CTextureFor2D * GetTexture( int i );

    // Set, get, and increment the current frame of animation
    void SetCurrentFrame( int frame );
    void IncCurrentFrame( int frame );
    int GetCurrentFrame() const;

    // Set, get, and increment the frame timer
    void SetFrameTimer( float value );
    void IncFrameTimer( float value );
    float GetFrameTimer() const;

    // Get the frame count
    int GetFrameCount() const;

    // Updates the mesh's vertex buffer
    void UpdateVertBuf();

    // Get the projection type
    CSettings::EProjectionType GetProjectionType();

    // Get the active texture
    NText::CTextureFor2D * GetActiveTexture();

    // Get the use camera boolean
    bool IsCameraUsed() const;

private:

    // Initialize the sprite with the object data
    void Init( bool camera, std::string & meshTag );

    // Update the shader prior to rendering
    virtual void UpdateShader( const CColor & _color = CColor(1,1,1,1) );

private:

    // Object's data. 
    // NOTE: This pointer belongs to the CObjectDataList2D singleton
    CObjectData2D * pObjectData;

    // Mesh that the sprite uses
    // NOTE: This class does not own this pointer.
    CVisualMesh2D * pMesh;

    // The color of the sprite
    CColor spriteColor;

    // Name of effect string
    std::string effectStr;

    // Name of shader technique to use with effect
    std::string techniqueStr;

    // The projection type of the visual sprite
    CSettings::EProjectionType projectionType;

    // If the sprite is transformed by the camera
    bool useCamera;

    // The current frame and frame timer
    int currentFrame;
    float frameTimer;
};

#endif  // __visualsprite_2d_h__
