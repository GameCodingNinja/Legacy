
/************************************************************************
*    FILE NAME:       shadowmesh.cpp
*
*    DESCRIPTION:     3D shadow mesh class
************************************************************************/

// Physical component dependency
#include "shadowmesh.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
#include "xwindow.h"
#include "genfunc.h"
#include "face.h"
#include "deletefuncs.h"
#include "spritebinaryfileheader.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. double to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it.
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CShadowMesh::CShadowMesh()
           : CMesh(),
             pFace(NULL),
             faceCount(0)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CShadowMesh::~CShadowMesh()
{
    DeleteArray( pFace );

}   // Destructer


/************************************************************************
*    desc:  Load the mesh from XML file
*  
*    param: string & _filePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowMesh::LoadFromXML( string & _filePath )
{
    // Use these variables to calculate the radius of the object
    double maxDistance = 0.0;
    double curDistance;

    // Save the file name
    filePath = _filePath;

    // Setup the constant attributes
    const int VERT_ATTR[] = {0, 1, 2}; // "v1", "v2", "v3"

    const int ATTR_X(0);
    const int ATTR_Y(1);
    const int ATTR_Z(2);

    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "mesh" );

    if( mainNode.isEmpty() )
    {
        return false;
    }

    // get a node to the vert list
    XMLNode vertLstNode = mainNode.getChildNode("vert_list");

    // get a node to the face group list
    XMLNode faceGrpNode = mainNode.getChildNode("face_group");

    // get a node to the face list
    XMLNode faceLstNode = faceGrpNode.getChildNode("face_list", 0);

    // Get the counts
    int vertCount = vertLstNode.nChildNode();
    faceCount = faceLstNode.nChildNode();

    // Allocate the arrays
    pFace = new CFace[faceCount];
    CPoint * pVert = new CPoint[vertCount];

    // Load in the verts
    for( int i = 0; i < vertCount; i++ )
    {
        XMLNode vertNode = vertLstNode.getChildNode(i);
        pVert[i].x = atof(vertNode.getAttributeValue(ATTR_X));
        pVert[i].y = atof(vertNode.getAttributeValue(ATTR_Y));
        pVert[i].z = atof(vertNode.getAttributeValue(ATTR_Z));
    }

    // Load the data from the xml file
    for( int i = 0; i < faceCount; i++ )
    {
        XMLNode faceNode = faceLstNode.getChildNode(i);

        for( int j = 0; j < 3; j++ )
        {
            // Get the index into each of the lists
            int vIndex = atoi(faceNode.getAttributeValue(VERT_ATTR[j]));

            // Get the vertex values
            pFace[i].vert[j] = pVert[vIndex];

            // Take this oppertunity to calculate this objects radius
            curDistance = pFace[i].vert[j].GetLength();

            // Have we found the longest distance?
            if( curDistance > maxDistance )
            {
                maxDistance = curDistance;
            }
        }

        // calculate the surface normal
        pFace[i].CalcSurfaceNormal();
    }

    // compute the radius
    radiusSqrt = sqrt( maxDistance );
    radius = maxDistance;

    // Delete the temporary array
    delete [] pVert;

    return true;

}	// LoadFromXML


/************************************************************************
*    desc:  Load the mesh from RSS file
*  
*    param: string & _filePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowMesh::LoadFromRSS( string & _filePath )
{
    bool result = false;

    CPoint * pVert = NULL;
    CBinaryVertFace * pBinaryFace = NULL;
    CSpriteBinaryFileHeader fileHeader;

    // Use these variables to calculate the radius of the object
    double maxDistance = 0.0;
    double curDistance = 0.0;

    // Save the file name
    filePath = _filePath;

    // Open the file
    FILE * pFile = fopen (filePath.c_str(), "rb");
    if( pFile == NULL )
    {
        PostMsg( "Load File Error", "Error Loading file (%s).", filePath.c_str() );
        goto FILE_ERROR;
    }

    // Read in the file header
    fread( &fileHeader, 1, sizeof(fileHeader), pFile );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != SPRITE_FILE_HEADER )
    {
        PostMsg( "Load File Error", "File header mismatch (%s).", filePath.c_str() );
        goto FILE_ERROR;
    }

    // Get the counts
    int vertCount = fileHeader.vert_count;
    int faceGrpCount = fileHeader.face_group_count;	

    // Set up a variable to test where we are in the file
    int tag_check;

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );

    // Check and make sure we are where we should be
    if( tag_check != VERT_LIST )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", filePath.c_str() );
        goto FILE_ERROR;
    }

    // Allocate the arrays
    pVert = new CPoint[vertCount];

    // Load in the verts
    fread( pVert, vertCount, sizeof(CPoint), pFile );

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );

    // Check and make sure we are where we should be
    if( tag_check != FACE_GROUP )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", filePath.c_str() );
        goto FILE_ERROR;
    }

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );

    // Check and make sure we are where we should be
    if( tag_check != FACE_LIST )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", filePath.c_str() );
        goto FILE_ERROR;
    }

    // Get the number faces in the group as well as the material index
    fread( &faceCount, 1, sizeof(faceCount), pFile );

    // Make a face pointer
    pFace = new CFace[faceCount];

    // Make the temporary array for loading face indexes
    pBinaryFace = new CBinaryVertFace[faceCount];

    // Read in the vert indexes
    fread( pBinaryFace, faceCount, sizeof(CBinaryVertFace), pFile );

    // Load the faces from the rss file
    for( int i = 0; i < faceCount; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            // Get the vertex values
            pFace[i].vert[j] = pVert[ pBinaryFace[i].vert[j] ];

            // Take this oppertunity to calculate this objects radius
            curDistance = pFace[i].vert[j].GetLength();

            // Have we found the longest distance?
            if( curDistance > maxDistance )
            {
                maxDistance = curDistance;
            }
        }

        // calculate the surface normal
        pFace[i].CalcSurfaceNormal();
    }

    // compute the radius
    radiusSqrt = (float)sqrt( maxDistance );
    radius = maxDistance;

    result = true;

// If there is a problem with the file we jump to here and delete all pointers
FILE_ERROR:

    CloseFileHandle( pFile );
    DeleteArray( pVert );
    DeleteArray( pBinaryFace );

    return result;

}	// LoadFromRSS


/************************************************************************
*    desc:  Transform the mesh data
*
*    param: CFace * pTransFace - pointer to hold translated data
*			CMatrix & matrix - passed in matrix class
*           CMatrix & noScaleMatrix - non scaled matrix
************************************************************************/
void CShadowMesh::Transform( CFace * pTransFace,
                             CMatrix & matrix,
                             CMatrix & noScaleMatrix )
{
    // Transform the face verts and normals
    for( int i = 0; i < faceCount; i++ )
    {
        // Transform the verts
        for( int j = 0; j < 3; j++ )
        {
            matrix.Transform( pTransFace[i].vert[j], pFace[i].vert[j] );
        }

        // Transform the normal of the face
        noScaleMatrix.Transform( pTransFace[i].normal, pFace[i].normal );
    }

}	// Transform


/***************************************************************************
*   desc:  Get the face count
*
*   parm: unsigned int index - index into face group
*
*   ret:  unsigned int - face count
 ****************************************************************************/
unsigned int CShadowMesh::GetFaceCount( unsigned int index )
{
    return faceCount;

}	// GetFaceCount


/***************************************************************************
*   desc:  Get the face pointer
*
*   ret:  CFace * - face pointer
 ****************************************************************************/
CFace * CShadowMesh::GetFacePtr()
{
    return pFace;

}	// GetFacePtr

