/************************************************************************
*    FILE NAME:       visualmesh2d.cpp
*
*    DESCRIPTION:     Emulated 2D DirectX mesh class
************************************************************************/

// Physical component dependency
#include <2d/visualmesh2d.h>

// Standard lib dependencies
#include <assert.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <system/xdevice.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlparsehelper.h>
#include <managers/shader.h>
#include <misc/settings.h>
#include <common/xface2d.h>
#include <common/texture.h>
#include <common/uv.h>
#include <common/vertex2d.h>
#include <2d/objectdata2d.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager2d.h>

// Required namespace(s)
using namespace std;

// Number of faces in a mesh division
const int FACE_DIVISION_COUNT = 2;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// Turn off fopen deprecation warning
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVisualMesh2D::CVisualMesh2D()
             : pVertBuf(NULL),
               radius(0),
               radiusSquared(0)
{
    vertexForamtMask = D3DFVF_XYZ|D3DFVF_TEX1;
    vertexDataSize = sizeof(CVertex2D);

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CVisualMesh2D::~CVisualMesh2D()
{
}   // Destructer


/************************************************************************
*    desc:  Load the mesh from the object data
*
*    param: const CObjectData2D * pData - data to load from
*			const string & meshTag      - the unique tag for the mesh
************************************************************************/
void CVisualMesh2D::LoadFromData( const CObjectData2D * pData, const string & meshTag )
{
    bool sizeSet = false;
    XMLNode mainNode;

    // If the object data has a size for the rect, use it
    if( pData->GetVisualData().GetSize().w > 0 || pData->GetVisualData().GetSize().h > 0 )
    {
        size = pData->GetVisualData().GetSize();
        size.w *= pData->GetScale().x;
        size.h *= pData->GetScale().y;
        sizeSet = true;
    }

    if( !pData->GetVisualData().GetFile().empty() && pData->GetVisualData().GetFile() != "rect" )
    {
        // this open and parse the XML file:
        mainNode = XMLNode::openFileHelper( pData->GetVisualData().GetFile().c_str(), "mesh2D" );
    
        // Get the mesh's image(s)
        XMLNode fileNode = mainNode.getChildNode( "file" );

        // Reserve the number of children
        pTextureVector.reserve( fileNode.nChildNode("filePath") );

        for( int i = 0; i < fileNode.nChildNode(); ++i )
        {
            XMLNode filePathNode = fileNode.getChildNode( "filePath", i );
            string imageFile = filePathNode.getAttribute( "image" );

            // Get the material
            NText::CTextureFor2D * pTextureFor2D = CTextureMgr::Instance().LoadFor2D( pData->GetGroup(), imageFile );

            // Put the material into the
            pTextureVector.push_back( pTextureFor2D );
        }

        // If the size hasn't been set yet, set it to the size of the material
        if( !sizeSet )
        {
            size = pTextureVector.back()->size;
            size.w *= pData->GetScale().x;
            size.h *= pData->GetScale().y;
            sizeSet = true;
        }
    }

    // If the size hasn't been set by this point, there is a problem
    if( !sizeSet || size.h == 0 || size.w == 0)
    {
        throw NExcept::CCriticalException("Visual Mesh 2D Load Error!",
                boost::str( boost::format("Size not set (%s).\n\n%s\nLine: %s") % meshTag.c_str() % __FUNCTION__ % __LINE__ ));
    }

    // Do we have the xml specific mesh generation tag
    XMLNode genMeshNode = mainNode.getChildNode( "generateMesh" );
    if( !genMeshNode.isEmpty() )
    {
        // Get the xml defined size
        size = NParseHelper::LoadSize( genMeshNode.getChildNode( "size" ) );

        // See if we are to generate a scaled frame
        XMLNode scaledFrameNode = genMeshNode.getChildNode( "scaledFrame" );
        if( !scaledFrameNode.isEmpty() )
            CreateScaledFrameMesh( scaledFrameNode );

        // See if we have mesh data
        XMLNode meshDataNode = genMeshNode.getChildNode( "meshData" );
        if( !meshDataNode.isEmpty() )
            CreateFromMeshData( pData, meshDataNode );
    }
    else
    {
        // Create the local copy of the mesh
        CreateMesh( pData );
    }

    // Create the vertex buffer
    pVertBuf = CVertBufMgr2D::Instance().CreateVertBuffer( pData->GetGroup(), faceVector, CSize<int>( size.w, size.h ), meshTag );

    // Calculate the radii
    radiusSquared = pow(size.w / 2, 2) + pow(size.h / 2, 2);
    radius = sqrt( radiusSquared );

}	// LoadFromData


/************************************************************************
*    desc:  Create from mesh data
*
*    param: node - data used to create the mesh
************************************************************************/
void CVisualMesh2D::CreateFromMeshData( const CObjectData2D * pData, const XMLNode & node )
{
    // There has to be a texture loaded to create the mesh
    if( pTextureVector.empty() )
    {
        throw NExcept::CCriticalException("Visual Mesh 2D Load Error!",
                boost::str( boost::format("Texture not loaded to create scaled frame.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));
    }

    // Check if the width or height is even. If so, we offset 
    // by 0.5 for proper orthographic rendering
    float additionalOffsetX = 0;
    if( (int)size.w % 2 == 0 )
        additionalOffsetX = 0.5f;

    float additionalOffsetY = 0;
    if( (int)size.h % 2 == 0 )
        additionalOffsetY = 0.5f;

    // Size the texture
    const CSize<float> textSize = pTextureVector.back()->size;

    // This converts the data to a center aligned vertex buffer
    const CSize<float> centerAlignSize(-(size.w / 2), (size.h / 2));

    // Get the number of triangles
    const uint triCount = node.nChildNode("tri");

    for( uint tri = 0; tri < triCount; ++tri )
    {
        const XMLNode triNode = node.getChildNode( "tri", tri );

        const uint vertCount = triNode.nChildNode("vert");
        assert(vertCount == 3);

        CXFace2D face;

        for( uint v = 0; v < vertCount; ++v )
        {
            // Load the 2D vert
            CVertex2D vert2D = NParseHelper::LoadVertex2d( triNode.getChildNode( "vert", v ) );

            face.vert[v].vert.x = ( centerAlignSize.w + vert2D.vert.x + additionalOffsetX ) * pData->GetScale().x;
            face.vert[v].vert.y = ( centerAlignSize.h - vert2D.vert.y + additionalOffsetY ) * pData->GetScale().y;
            face.vert[v].uv.u = vert2D.uv.u / textSize.w;
            face.vert[v].uv.v = vert2D.uv.v / textSize.h;
        }

        faceVector.push_back(face);
    }

}	// CreateFromMeshData


/************************************************************************
*    desc:  Create a scaled frame mesh
*
*    param: node - data used to create the mesh
************************************************************************/
void CVisualMesh2D::CreateScaledFrameMesh( const XMLNode & node )
{
    // Size the frame border
    CSize<float> borderSize = NParseHelper::LoadSize( node );

    // There has to be a texture loaded to create a frame from
    if( pTextureVector.empty() )
    {
        throw NExcept::CCriticalException("Visual Mesh 2D Load Error!",
                boost::str( boost::format("Texture not loaded to create scaled frame.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));
    }

    // Size the texture
    const CSize<float> textSize = pTextureVector.back()->size;

    // Offsets to center the mesh
    const CPoint center((size.w / 2), (size.h / 2));

    const CSize<float> frameLgth( size.w - (borderSize.w * 2), size.h - (borderSize.h * 2) );
    const CSize<float> uvLgth( textSize.w - (borderSize.w * 2), textSize.h - (borderSize.h * 2) );

    // All frame pieces are created by default
    bool topLeft(true), topRight(true), botLeft(true), botRight(true), topFrame(true), botFrame(true), leftFrame(true), rightFrame(true), centerQuad(true);

    // See if any of the pieces are not to be created
    if( node.isAttributeSet( "topLeft" ) && string(node.getAttribute( "topLeft" )) == "false" )
        topLeft = false;

    if( node.isAttributeSet( "topRight" ) && string(node.getAttribute( "topRight" )) == "false" )
        topRight = false;

    if( node.isAttributeSet( "botLeft" ) && string(node.getAttribute( "botLeft" )) == "false" )
        botLeft = false;

    if( node.isAttributeSet( "botRight" ) && string(node.getAttribute( "botRight" )) == "false" )
        botRight = false;

    if( node.isAttributeSet( "topFrame" ) && string(node.getAttribute( "topFrame" )) == "false" )
        topFrame = false;

    if( node.isAttributeSet( "botFrame" ) && string(node.getAttribute( "botFrame" )) == "false" )
        botFrame = false;

    if( node.isAttributeSet( "leftFrame" ) && string(node.getAttribute( "leftFrame" )) == "false" )
        leftFrame = false;

    if( node.isAttributeSet( "rightFrame" ) && string(node.getAttribute( "rightFrame" )) == "false" )
        rightFrame = false;

    if( node.isAttributeSet( "centerQuad" ) && string(node.getAttribute( "centerQuad" )) == "false" )
        centerQuad = false;

    // Top left corner piece
    if( topLeft )
    {
        CreateQuad( CPoint(-center.x, center.y),
                    CSize<float>(borderSize.w, -borderSize.h),
                    CUV(0, 0),
                    CSize<float>(borderSize.w, borderSize.h),
                    textSize );
    }

    // Top right corner piece
    if( topRight )
    {
        CreateQuad( CPoint(center.x-borderSize.w, center.y),
                    CSize<float>(borderSize.w, -borderSize.h),
                    CUV(borderSize.w + uvLgth.w,0),
                    CSize<float>(borderSize.w, borderSize.h),
                    textSize );
    }

    // Bottom left corner piece
    if( botLeft )
    {
        CreateQuad( CPoint(-center.x, -(center.y-borderSize.h)),
                    CSize<float>(borderSize.w, -borderSize.h),
                    CUV(0, borderSize.h + uvLgth.h),
                    CSize<float>(borderSize.w, borderSize.h),
                    textSize );
    }

    // Bottom right corner piece
    if( botRight )
    {
        CreateQuad( CPoint(center.x-borderSize.w, -(center.y-borderSize.h)),
                    CSize<float>(borderSize.w, -borderSize.h),
                    CUV(borderSize.w + uvLgth.w, borderSize.h + uvLgth.h),
                    CSize<float>(borderSize.w, borderSize.h),
                    textSize );
    }

    if( frameLgth.w > 0 )
    {
        // Top frame piece
        if( topFrame )
        {
            CreateQuad( CPoint(-(center.x-borderSize.w), center.y),
                        CSize<float>(frameLgth.w, -borderSize.h),
                        CUV(borderSize.w, 0),
                        CSize<float>(uvLgth.w, borderSize.h),
                        textSize );
        }

        // Bottom frame piece
        if( botFrame )
        {
            CreateQuad( CPoint(-(center.x-borderSize.w), -(center.y-borderSize.h)),
                        CSize<float>(frameLgth.w, -borderSize.h),
                        CUV(borderSize.w, borderSize.h + uvLgth.h),
                        CSize<float>(uvLgth.w, borderSize.h),
                        textSize );
        }
    }

    if( frameLgth.h > 0 )
    {
        // Left frame piece
        if( leftFrame )
        {
            CreateQuad( CPoint(-center.x, center.y-borderSize.h),
                        CSize<float>(borderSize.w, -frameLgth.h),
                        CUV(0, borderSize.h),
                        CSize<float>(borderSize.w, uvLgth.h),
                        textSize );
        }

        // Right frame piece
        if( rightFrame )
        {
            CreateQuad( CPoint(center.x-borderSize.w, center.y-borderSize.h),
                        CSize<float>(borderSize.w, -frameLgth.h),
                        CUV(borderSize.w + uvLgth.w, borderSize.h),
                        CSize<float>(borderSize.w, uvLgth.h),
                        textSize );
        }
    }

    // De we want a center quad
    if( centerQuad )
    {
        // Center fill piece
        CreateQuad( CPoint(-(center.x-borderSize.w), center.y-borderSize.h),
                    CSize<float>(frameLgth.w, -frameLgth.h),
                    CUV(borderSize.w, borderSize.h),
                    CSize<float>(uvLgth.w, uvLgth.h),
                    textSize );
    }

}	// CreateScaledFrameMesh


/************************************************************************
*    desc:  Create a scaled frame corner
*
*    param: node - data used to create the mesh
************************************************************************/
void CVisualMesh2D::CreateQuad( const CPoint & vert,
                                const CSize<float> & vSize,
                                const CUV & uv,
                                const CSize<float> & uvSize,
                                const CSize<float> & textSize )
{
    // Check if the width or height is even. If so, we offset 
    // by 0.5 for proper orthographic rendering
    float additionalOffsetX = 0;
    if( (int)size.w % 2 == 0 )
        additionalOffsetX = 0.5f;

    float additionalOffsetY = 0;
    if( (int)size.h % 2 == 0 )
        additionalOffsetY = 0.5f;

    // Faces created
    CXFace2D face1;
    CXFace2D face2;

    // Calculate the first vertex of the first face
    face1.vert[0].vert.x = vert.x + additionalOffsetX;
    face1.vert[0].vert.y = vert.y + additionalOffsetY;
    face1.vert[0].uv.u = uv.u / textSize.w;
    face1.vert[0].uv.v = uv.v / textSize.h;

    // Calculate the second vertex of the first face
    face1.vert[1].vert.x = face1.vert[0].vert.x + vSize.w;
    face1.vert[1].vert.y = face1.vert[0].vert.y;
    face1.vert[1].uv.u = (uv.u + uvSize.w) / textSize.w;
    face1.vert[1].uv.v = face1.vert[0].uv.v;

    // The first vertex of the second face is the same as the
    // previously calculated vertex
    face2.vert[0] = face1.vert[1];

    // Calculate the third vertex of the first face
    face1.vert[2].vert.x = face1.vert[0].vert.x;
    face1.vert[2].vert.y = face1.vert[0].vert.y + vSize.h;
    face1.vert[2].uv.u = face1.vert[0].uv.u;
    face1.vert[2].uv.v = (uv.v + uvSize.h) / textSize.h; 

    // The third vertex of the second face is the same as the
    // previously calculated vertex
    face2.vert[2] = face1.vert[2];

    // Calculate the second vertex of the second face
    face2.vert[1].vert.x = face1.vert[1].vert.x;
    face2.vert[1].vert.y = face1.vert[2].vert.y;
    face2.vert[1].uv.u = face1.vert[1].uv.u;
    face2.vert[1].uv.v = face1.vert[2].uv.v;

    faceVector.push_back(face1);
    faceVector.push_back(face2);

}	// CreateScaledFrameSegment


/************************************************************************
*    desc:  Create the mesh
*
*    param: const CObjectData2D * pData - data used to create the mesh
************************************************************************/
void CVisualMesh2D::CreateMesh( const CObjectData2D * pData )
{
    // Get the number of faces in the mesh
    int faceCount = pData->GetVisualData().GetRows() * pData->GetVisualData().GetColumns() * FACE_DIVISION_COUNT;

    faceVector.reserve( faceCount );

    int columns = pData->GetVisualData().GetColumns();
    int rows = pData->GetVisualData().GetRows();

    float additionalOffsetX = 0;
    float additionalOffsetY = 0;

    // Check if the width or height is even. If so, we offset by 0.5
    if( (int)size.w % 2 == 0 )
        additionalOffsetX = 0.5f;

    if( (int)size.h % 2 == 0 )
        additionalOffsetY = 0.5f;

    // Offsets to center the mesh
    float offsetX = size.w / 2;
    float offsetY = size.h / 2;

    float xStart(-offsetX), yStart(offsetY);
    float xIncAmount = size.w / columns;
    float yIncAmount = size.h / rows;
    float uIncAmount = 1.f / rows;
    float vIncAmount = 1.f / columns;

    // Counters to increment through the loops
    float xCounter = xStart + additionalOffsetX;
    float yCounter = yStart + additionalOffsetY;
    float uCounter = 0;
    float vCounter = 0;

    for( int i = 0; i < rows; ++i )
    {
        for( int j = 0; j < columns; ++j )
        {
            // Faces created
            CXFace2D face1;
            CXFace2D face2;
            
            // Calculate the first vertex of the first face
            face1.vert[0].vert.x = xCounter;
            face1.vert[0].vert.y = yCounter;
            face1.vert[0].uv.u = uCounter;
            face1.vert[0].uv.v = vCounter;

            // Calculate the second vertex of the first face
            face1.vert[1].vert.x = xCounter + xIncAmount;
            face1.vert[1].vert.y = yCounter;
            face1.vert[1].uv.u = uCounter + uIncAmount;
            face1.vert[1].uv.v = vCounter;

            // The first vertex of the second face is the same as the
            // previously calculated vertex
            face2.vert[0] = face1.vert[1];

            // Calculate the third vertex of the first face
            face1.vert[2].vert.x = xCounter;
            face1.vert[2].vert.y = yCounter - yIncAmount;
            face1.vert[2].uv.u = uCounter;
            face1.vert[2].uv.v = vCounter + vIncAmount;

            // The third vertex of the second face is the same as the
            // previously calculated vertex
            face2.vert[2] = face1.vert[2];

            // Calculate the second vertex of the second face
            face2.vert[1].vert.x = xCounter + xIncAmount;
            face2.vert[1].vert.y = yCounter - yIncAmount;
            face2.vert[1].uv.u = uCounter + uIncAmount;
            face2.vert[1].uv.v = vCounter + vIncAmount;

            faceVector.push_back(face1);
            faceVector.push_back(face2);
            
            // Increment the x and u counter
            xCounter += xIncAmount;
            uCounter += uIncAmount;
        }

        // Increment the y and v counter
        yCounter -= yIncAmount;
        vCounter += vIncAmount;

        // Reset the x and u counter
        xCounter = xStart + additionalOffsetX;
        uCounter = 0;
    }

}	// CreateMesh


/************************************************************************
*    desc:  Render this mesh to the back buffer 
************************************************************************/
void CVisualMesh2D::Render( int frame )
{
    // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
    // save and restore all state modified by the effect.
    UINT uPasses;
    CShader::Instance().GetActiveShader()->Begin( &uPasses, 0 );

    // Give directx the vertex buffer
    CXDevice::Instance().GetXDevice()->SetStreamSource( 0, pVertBuf, 0, vertexDataSize );
    CXDevice::Instance().GetXDevice()->SetFVF( vertexForamtMask );

    for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
        if( !pTextureVector.empty() )
            CTextureMgr::Instance().SelectTexture( pTextureVector[frame]->spTexture );

        CShader::Instance().GetActiveShader()->BeginPass( uPass );
        CXDevice::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, static_cast<UINT>(faceVector.size()) );
        CShader::Instance().GetActiveShader()->EndPass();
    }

    CShader::Instance().GetActiveShader()->End();

}	// Render


/***************************************************************************
*   desc:  Get a face
*
*   ret:  CXFace2D & - face to return
 ****************************************************************************/
CXFace2D & CVisualMesh2D::GetFace( int index )
{
    return faceVector[index];

}	// GetFace


/***************************************************************************
*   desc:  Get the radius
*
*   ret:  float - value of radius
 ****************************************************************************/
float CVisualMesh2D::GetRadius() const
{
    return radius;

}	// GetRadius


/***************************************************************************
*   desc:  Get the radius squared
*
*   ret:  float - value of radius
 ****************************************************************************/
float CVisualMesh2D::GetRadiusSquared() const
{
    return radiusSquared;

}	// GetRadiusSquared


/***************************************************************************
*   desc:  Get the size of the mesh
*
*   ret:  CSize - size
 ****************************************************************************/
const CSize<float> & CVisualMesh2D::GetSize() const
{
    return size;

}	// GetSize


/***************************************************************************
*   desc:	Get the material
*
*   ret:	LPDIRECT3DTEXTURE9 - material to return
 ****************************************************************************/
LPDIRECT3DTEXTURE9 CVisualMesh2D::GetMaterial( int i )
{
    return pTextureVector[i]->spTexture;

}	// GetMaterial


/***************************************************************************
*   desc:	Get the texture
*
*   ret:	NText::CTextureFor2D * - texture to return
 ****************************************************************************/
NText::CTextureFor2D * CVisualMesh2D::GetTexture( int i )
{
    return pTextureVector[i];

}	// GetTexture


/***************************************************************************
*   desc:  Get the number of materials in the mesh
*
*   ret:  unsigned int - number of materials in the mesh
 ****************************************************************************/
size_t CVisualMesh2D::GetMaterialCount() const
{
    return pTextureVector.size();

}	// GetMaterialCount


/***************************************************************************
*   desc:  Get the number of faces in the mesh
*
*   ret:  unsigned int - number of faces in the mesh
 ****************************************************************************/
size_t CVisualMesh2D::GetFaceCount() const
{
    return faceVector.size();

}	// GetFaceCount


/***************************************************************************
*   desc:  Updates the vertex buffer by copying the face vector
 ****************************************************************************/
void CVisualMesh2D::UpdateVertBuf()
{
    // Make a face pointer
    CXFace2D * pFace;

    // Lock the vertex buffer for copying
    if( FAILED( pVertBuf->Lock( 0, 0, (void **)&pFace, 0 ) ) )
    {
        throw NExcept::CCriticalException("Vertex Buffer Lock Error!",
            boost::str( boost::format("Error locking vertex buffer.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));
    }

    // Copy the passed in faces to the vertex buffer. NOTE: using std::copy with a pointer causes a huge warning
    for( size_t i = 0; i < faceVector.size(); ++i )
        pFace[i] = faceVector[i];

    // Unlock the buffer so it can be used
    pVertBuf->Unlock();

}	// UpdateVertBuf