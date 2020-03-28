/************************************************************************
*    FILE NAME:       collisionmesh3d.cpp
*
*    DESCRIPTION:     3D collision mesh class
************************************************************************/

// Physical component dependency
#include <3d/collisionmesh3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <common/face.h>
#include <utilities/smartpointers.h>
#include <utilities/exceptionhandling.h>
#include <misc/spritebinaryfileheader.h>
#include <3d/objectdata3d.h>


// Turn off function unsafe warnings
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionMesh3D::CCollisionMesh3D()
                : faceCount(0)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollisionMesh3D::~CCollisionMesh3D()
{
}   // Destructer


/************************************************************************
*    desc:  Load the mesh from RSS file
*  
*    param: string & _filePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************/
void CCollisionMesh3D::LoadFromRSS( const CObjectData3D * pObjData )
{
    CSpriteBinaryFileHeader fileHeader;

    // Use these variables to calculate the radius of the object
    double maxDistance = 0.0;
    double curDistance = 0.0;

    // Open the file
    NSmart::scoped_filehandle_ptr<FILE> pFile( fopen( pObjData->GetCollisionData().GetFile().c_str(), "rb" ) );
    if( pFile.isNull() )
        throw NExcept::CCriticalException("Collision Mesh Load Error!",
            boost::str( boost::format("Error Loading file (%s).\n\n%s\nLine: %s") % pObjData->GetCollisionData().GetFile().c_str() % __FUNCTION__ % __LINE__ ));

    // Read in the file header
    fread( &fileHeader, 1, sizeof(fileHeader), pFile.get() );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != SPRITE_FILE_HEADER )
        throw NExcept::CCriticalException("Collision Mesh Load Error!",
            boost::str( boost::format("File header mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetCollisionData().GetFile().c_str() % __FUNCTION__ % __LINE__ ));

    // Get the counts
    int vertCount = fileHeader.vert_count;
    int faceGrpCount = fileHeader.face_group_count;	

    // Set up a variable to test where we are in the file
    int tag_check;

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != VERT_LIST )
        throw NExcept::CCriticalException("Collision Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetCollisionData().GetFile().c_str() % __FUNCTION__ % __LINE__ ));

    // Allocate the arrays
    boost::scoped_array<CPoint> pVert( new CPoint[vertCount] );

    // Load in the verts
    fread( pVert.get(), vertCount, sizeof(CPoint), pFile.get() );

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != FACE_GROUP )
        throw NExcept::CCriticalException("Collision Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetCollisionData().GetFile().c_str() % __FUNCTION__ % __LINE__ ));

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile.get() );

    // Check and make sure we are where we should be
    if( tag_check != FACE_LIST )
        throw NExcept::CCriticalException("Collision Mesh Load Error!",
            boost::str( boost::format("Tag check mismatch (%s).\n\n%s\nLine: %s") % pObjData->GetCollisionData().GetFile().c_str() % __FUNCTION__ % __LINE__ ));

    // Get the number faces in the group as well as the material index
    fread( &faceCount, 1, sizeof(faceCount), pFile.get() );

    // Make a face pointer
    spFace.reset( new CFace[faceCount] );

    // Make the temporary array for loading face indexes
    boost::scoped_array<CBinaryVertFace> pBinaryFace( new CBinaryVertFace[faceCount] );

    // Read in the vert indexes
    fread( pBinaryFace.get(), faceCount, sizeof(CBinaryVertFace), pFile.get() );

    // Load the faces from the rss file
    for( int i = 0; i < faceCount; ++i )
    {
        for( int j = 0; j < 3; ++j )
        {
            // Get the vertex values
            spFace[i].vert[j] = pVert[ pBinaryFace[i].vert[j] ];

            // Take this oppertunity to calculate this objects radius
            curDistance = spFace[i].vert[j].GetLengthSquared();

            // Have we found the longest distance?
            if( curDistance > maxDistance )
                maxDistance = curDistance;
        }

        // calculate the surface normal
        spFace[i].CalcSurfaceNormal();

        // Calculate the radius of the polygon
        spFace[i].CalcBoundingPoly();
    }

    // compute the radius
    radiusSqrt = (float)sqrt( maxDistance );
    radius = maxDistance;

}	// LoadFromRSS


/************************************************************************
*    desc:  Transform the collision mesh data
*
*    param: CFace * pTransFace - pointer to hold translated data
*           float scale - scale the radius
*			CMatrix & matrix - passed in matrix class
*           CMatrix & unscaledMatrix - non scaled matrix
************************************************************************/
void CCollisionMesh3D::Transform( CFace * pTransFace, float scale,
                                  CMatrix & matrix, CMatrix & unscaledMatrix )
{
    // Transform the face verts and normals
    for( int i = 0; i < faceCount; ++i )
    {
        // Transform the verts
        for( int j = 0; j < 3; ++j )
            matrix.Transform( pTransFace[i].vert[j], spFace[i].vert[j] );

        // Transform the face radius
        pTransFace[i].radius = spFace[i].radius * scale;

        // Transform the center of the face
        matrix.Transform( pTransFace[i].center, spFace[i].center );

        // Transform the normal of the face
        unscaledMatrix.Transform( pTransFace[i].normal, spFace[i].normal );
    }

}	// Transform


/***************************************************************************
*   desc:  Get the face count
*
*   parm: unsigned int index - index into face group
*
*   ret:  unsigned int - face count
 ****************************************************************************/
unsigned int CCollisionMesh3D::GetFaceCount( unsigned int index )
{
    return faceCount;

}	// GetFaceCount

