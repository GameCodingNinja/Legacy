/************************************************************************
*    FILE NAME:       visualmesh3d.cpp
*
*    DESCRIPTION:     3D DirectX mesh class
************************************************************************/

// Physical component dependency
#include <3d/visualmesh3d.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <system/xdevice.h>
#include <managers/texturemanager.h>
#include <managers/shader.h>
#include <misc/spritebinaryfileheader.h>
#include <common/texture.h>
#include <common/xface.h>
#include <common/xvertexbuffer.h>
#include <utilities/smartpointers.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <3d/objectdata3d.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// Turn off fopen deprecation warning
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVisualMesh3D::CVisualMesh3D()
             : faceGrpCount(0)
{
    vertexForamtMask = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
    vertexDataSize = sizeof(CVertex);

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CVisualMesh3D::~CVisualMesh3D()
{
}   // Destructer


/************************************************************************
*    desc:  Load the mesh from RSS file
*  
*    param: const CObjectData3D * pObjData - object data pointer
*    
*    return: bool - true on success or false on fail
************************************************************************/
void CVisualMesh3D::LoadFromRSS( const CObjectData3D * pObjData )
{
    // Open the file
    NSmart::scoped_filehandle_ptr<FILE> pFile( fopen( pObjData->GetVisualData().GetFile().c_str(), "rb" ) );
    if( pFile.isNull() )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("Error Loading file (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Read in the file header
    CSpriteBinaryFileHeader fileHeader;
    fread( &fileHeader, 1, sizeof(fileHeader), pFile.get() );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != SPRITE_FILE_HEADER )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("File header mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Get the counts
    faceGrpCount = fileHeader.face_group_count;	

    // Set up a variable to test where we are in the file
    int tag_check;

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != VERT_LIST )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Allocate local smart pointer arrays
    boost::scoped_array<CPoint> pVert( new CPoint[fileHeader.vert_count] );
    boost::scoped_array<CUV> pUV( new CUV[fileHeader.uv_count] );
    boost::scoped_array<CNormal> pVNormal( new CNormal[fileHeader.vert_norm_count] );

    // Allocate class smart pointer arrays
    spXVertBuf.reset( new CXVertBuff[faceGrpCount] );

    // Load in the verts
    fread( pVert.get(), fileHeader.vert_count, sizeof(CPoint), pFile.get() );

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != UV_LIST )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Load in the uvs
    fread( pUV.get(), fileHeader.uv_count, sizeof(CUV), pFile.get() );

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != VERT_NORM_LIST )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Load in the uvs
    fread( pVNormal.get(), fileHeader.vert_norm_count, sizeof(CNormal), pFile.get() );

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != FACE_GROUP )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Use these variables to calculate the radius of the object
    double maxDistance = 0.0;
    double curDistance = 0.0;

    // temporary texture index array container
    vector<vector<int>> textIndexVec;

    // Add faces to the vector
    for( unsigned int i = 0; i < faceGrpCount; ++i )
    {
        CBinaryFaceGroup group;

        // Read in new tag
        fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

        // Check and make sure we are where we should be
        if( tag_check != FACE_LIST )
            throw NExcept::CCriticalException("Visual Mesh Load Error!",
                boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

        // Get the number faces in the group as well as the material index
        fread( &group, 1, sizeof(group), pFile.get() );

        // Texture or face count can't be zero
        if( (group.textureCount <= 0) || (group.groupFaceCount <= 0) )
            throw NExcept::CCriticalException("Visual Mesh Load Error!",
                boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

        spXVertBuf[i].SetFaceCount( group.groupFaceCount );

        // Collect all the texture indexes for this face
        vector<int> indexVec;
        for( int j = 0; j < group.textureCount; ++j )
        {
            int textIndex;
            fread( &textIndex, 1, sizeof(textIndex), pFile.get() );
            indexVec.push_back(textIndex);
        }

        textIndexVec.push_back(indexVec);

        // create the vertex buffer
        if( D3D_OK != CXDevice::Instance().GetXDevice()->CreateVertexBuffer( spXVertBuf[i].GetFaceCount() * sizeof(CXFace),
                                                                             D3DUSAGE_WRITEONLY,
                                                                             0,
                                                                             D3DPOOL_MANAGED,
                                                                             spXVertBuf[i].GetDblPtrVertBuffer(),
                                                                             NULL ) )
        {
            throw NExcept::CCriticalException("Visual Mesh Load Error!",
                boost::str( boost::format("Error creating vertex buffer (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));
        }

        // Make a face pointer
        CXFace * pFace;

        // Lock the vertex buffer for copying
        if( FAILED(spXVertBuf[i].GetVertBuffer()->Lock( 0, 0, (void **)&pFace, 0 )) )
            throw NExcept::CCriticalException("Visual Mesh Load Error!",
                boost::str( boost::format("Error locking vertex buffer (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

        // Allocate and load in this group of faces
        boost::scoped_array<CBinaryFace> pBinFace( new CBinaryFace[spXVertBuf[i].GetFaceCount()] );
        fread( pBinFace.get(), spXVertBuf[i].GetFaceCount(), sizeof(CBinaryFace), pFile.get() );

        // Load the data from the xml file
        for( unsigned int j = 0; j < spXVertBuf[i].GetFaceCount(); ++j )
        {
            for( int k = 0; k < 3; ++k )
            {
                // Get the index into each of the lists
                int vIndex = pBinFace[j].vert[k];
                int uvIndex = pBinFace[j].uv[k];
                int vnIndex = pBinFace[j].normal[k];

                // Get the vertex values
                pFace[j].vert[k].vert = pVert[vIndex];

                // Get the vertex normal values
                pFace[j].vert[k].vnorm = pVNormal[vnIndex];
    
                // Get the uv values
                pFace[j].vert[k].uv = pUV[uvIndex];

                // Take this oppertunity to calculate this objects radius
                curDistance = pFace[j].vert[k].vert.GetLengthSquared();

                // Have we found the longest distance?
                if( curDistance > maxDistance )
                {
                    maxDistance = curDistance;
                }
            }
        }

        // Unlock the buffer so it can be used
        spXVertBuf[i].GetVertBuffer()->Unlock();
    }
        
    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != MAT_LIST )
        throw NExcept::CCriticalException("Visual Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));

    // Load all the textures associated with this mesh
    vector<NText::CTextureFor3D> textureLstVec;
    for(int mIndex = 0; mIndex < fileHeader.text_count; ++mIndex)
    {
        CBinaryTexture binaryTexture;

        // Read in the texture type and path
        fread( &binaryTexture, 1, sizeof(binaryTexture), pFile.get() );

        // Keep a local copy of the texture pointer and it's type
        // The Material manager owns the texture pointers and will free them
        string texturePath = binaryTexture.path;
        NText::CTextureFor3D texture;
        texture.pTexture = CTextureMgr::Instance().LoadFor3D( pObjData->GetGroup(), texturePath );
        texture.type = binaryTexture.type;

        textureLstVec.push_back(texture);

        if( NULL == texture.pTexture )
            throw NExcept::CCriticalException("Visual Mesh Load Error!",
                boost::str( boost::format("Texture is NULL (%s).\n\n%s\nLine: %s") % pObjData->GetVisualData().GetFile() % __FUNCTION__ % __LINE__ ));
    }

    // associate the textures with the faces
    for( unsigned int i = 0; i < faceGrpCount; ++i )
        for( size_t j = 0; j < textIndexVec[i].size(); ++j )
            spXVertBuf[i].SetTexture( textureLstVec[ textIndexVec[i][j] ] );

    // compute the radius
    radiusSqrt = (float)sqrt( maxDistance );
    radius = maxDistance;

}	// LoadFromRSS


/************************************************************************
*    desc:  Render this mesh to the back buffer 
************************************************************************/
void CVisualMesh3D::Render()
{
    // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
    // save and restore all state modified by the effect.
    UINT uPasses;
    CShader::Instance().GetActiveShader()->Begin( &uPasses, 0 );

    // get the elapsed time
    float elapsedTime = CHighResTimer::Instance().GetElapsedTime();

    //for( UINT uPass = 0; uPass < uPasses; ++uPass )
    for( unsigned int i = 0; i < faceGrpCount; ++i )
    {
        CXDevice::Instance().GetXDevice()->SetStreamSource( 0, spXVertBuf[i].GetVertBuffer(), 0, vertexDataSize );
        CXDevice::Instance().GetXDevice()->SetFVF( vertexForamtMask );

        //for( unsigned int i = 0; i < faceGrpCount; ++i )
        for( UINT uPass = 0; uPass < uPasses; ++uPass )
        {
            CTextureMgr::Instance().SelectTexture( spXVertBuf[i].GetMaterial() );

            CShader::Instance().GetActiveShader()->BeginPass( uPass );

            CXDevice::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, spXVertBuf[i].GetFaceCount() );

            CShader::Instance().GetActiveShader()->EndPass();
        }
    }

    CShader::Instance().GetActiveShader()->End();

}	// Render


/************************************************************************
*    desc:  Get the face group count
*
*    return: unsigned int - face group count
************************************************************************/
unsigned int CVisualMesh3D::GetFaceGroupCount()
{
    return faceGrpCount;

}	// GetFaceGroupCount


/***************************************************************************
*   desc:  Get the face count
*
*   parm: unsigned int index - index into face group
*
*   ret:  unsigned int - face count
 ****************************************************************************/
unsigned int CVisualMesh3D::GetFaceCount( unsigned int index )
{
    unsigned int result = 0;

    if( spXVertBuf != NULL && index < faceGrpCount )
    {
        result = spXVertBuf[index].GetFaceCount();
    }

    return result;

}	// GetFaceCount
