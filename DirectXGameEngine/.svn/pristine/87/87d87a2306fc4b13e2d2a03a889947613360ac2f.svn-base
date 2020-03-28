
/************************************************************************
*    FILE NAME:       shadowjointanimationmesh.cpp
*
*    DESCRIPTION:     3D shadow joint animation mesh class
************************************************************************/ 

// Physical component dependency
#include "shadowjointanimationmesh.h"

// Game OS dependencies
#include <assert.h>

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
#include "xwindow.h"
#include "genfunc.h"
#include "materialmanager.h"
#include "xvertexbuffer.h"
#include "joint.h"
#include "jpoint.h"
#include "meshanimation.h"
#include "face.h"
#include "deletefuncs.h"
#include "spritebinaryfileheader.h"
#include "jointanimationmanager.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CShadowJointAnimMesh::CShadowJointAnimMesh()
                    : CShadowMesh(),
                      pJoint(NULL),
                      jointCount(0)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CShadowJointAnimMesh::~CShadowJointAnimMesh()
{
    DeleteArray( pJoint );

    jointCount = 0;

    // Free the joint multi-link lists
    DeleteMapPointers( jointLinkLst );

}   // Destructer


/************************************************************************
*    desc:  Load the mesh and animtions from XML
*  
*    param: string & _filePath - path to XML file
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowJointAnimMesh::LoadMeshAndAnimFromXML( string & path )
{
    // Save the file name
    filePath = path;

    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "jointAnim" );
    if( mainNode.isEmpty() )
    {
        return false;
    }

    // Load the mesh
    if( !LoadFromRSS( string(mainNode.getChildNode("mesh").getAttribute("file")) ) )
    {
        return false;
    }

    // Get the animations
    XMLNode animLstNode = mainNode.getChildNode("animLst");

    // Load the animations
    for( int i = 0; i < animLstNode.nChildNode(); i++ )
    {
        if( !LoadAnimFromRSA( string(animLstNode.getChildNode(i).getAttribute("name")),
                                     atof(animLstNode.getChildNode(i).getAttribute("fps")),
                                     string(animLstNode.getChildNode(i).getAttribute("file"))) )
        {
            return false;
        }
    }

    return true;

}	// LoadMeshAndAnimFromXML


/************************************************************************
*    desc:  Load the mesh from RSS file
*  
*    param: string & _filePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowJointAnimMesh::LoadFromRSS( string & path )
{
    bool result = false;

    // Setup pointer
/*	CJPoint * pVert = NULL;
    CBinaryVertFace * pBinaryFace = NULL;
    CBinaryJoint * pBinaryJoint = NULL;
    CSpriteBinaryFileHeader fileHeader;

    // Use these variables to calculate the radius of the object
    double maxDistance = 0.0;
    double curDistance = 0.0;

    // Open the file
    FILE * pFile = fopen (path.c_str(), "rb");
    if( pFile == NULL )
    {
        PostMsg( "Load File Error", "Error Loading file (%s).", path.c_str() );
        goto FILE_ERROR;
    }

    // Read in the file header
    fread( &fileHeader, 1, sizeof(fileHeader), pFile );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != SPRITE_FILE_HEADER )
    {
        PostMsg( "Load File Error", "File header mismatch (%s).", path.c_str() );
        goto FILE_ERROR;
    }

    // Get the counts
    int vertCount = fileHeader.vert_count;	
    jointCount = fileHeader.joint_count;

    // Set up a variable to test where we are in the file
    int tag_check;

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );

    // Check and make sure we are where we should be
    if( tag_check != VERT_LIST )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", path.c_str() );
        goto FILE_ERROR;
    }

    // Allocate the arrays
    pVert = new CJPoint[vertCount];
    pJoint = new CJoint[jointCount];
    pBinaryJoint = new CBinaryJoint[jointCount];

    // Load in the verts
    fread( pVert, vertCount, sizeof(CJPoint), pFile );

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );

    // Check and make sure we are where we should be
    if( tag_check != FACE_GROUP )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", path.c_str() );
        goto FILE_ERROR;
    }

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );

    // Check and make sure we are where we should be
    if( tag_check != FACE_LIST )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", path.c_str() );
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
    }

    // Read in new tag
    fread( &tag_check, 1, sizeof(tag_check), pFile );
    
    // Check and make sure we are where we should be
    if( tag_check != JOINT_LIST )
    {
        PostMsg( "Load File Error", "Tag check mismatch (%s).", path.c_str() );
        goto FILE_ERROR;
    }

    int jointVertCountTest = 0;

    // Read in all the joint information
    fread( pBinaryJoint, jointCount, sizeof(CBinaryJoint), pFile );

    // Load the joint info
    for( unsigned int i = 0; i < jointCount; i++ )
    {
        pJoint[i].vCount = pBinaryJoint[i].vert_count;
        pJoint[i].name = pBinaryJoint[i].name;
        pJoint[i].parent = pBinaryJoint[i].parentName;

        pJoint[i].pos.x = pBinaryJoint[i].x;
        pJoint[i].pos.y = pBinaryJoint[i].y;
        pJoint[i].pos.z = pBinaryJoint[i].z;

        //pJoint[i].pJVert = new CJVertex[ pJoint[i].vCount ];

        // Add up the joint count to make sure they equil the vert count
        //jointVertCountTest += pJoint[i].vCount;
    }

    /////////////////////////////////////////////////////////////
    // Here we are copying the verts to the joint[]->jvert class
    // for finding the normals and faces that apply. We'll
    // be using these classed to transform our verts later
    // for the different animations
    ////////////////////////////////////////////////////////////
    for( int i = 0; i < vertCount; i++ )
    {
        // Get the joint index
        unsigned int jIndex = pVert[i].j;

        // should always be less the total number of joints
        assert( jIndex < jointCount );

        // should always be less the total number of verts added
        assert( pJoint[jIndex].initCounter < pJoint[jIndex].vCount );

        // add the vert and inc the counter
        //pJoint[jIndex].pJVert[ pJoint[jIndex].initCounter ].vert = pVert[i];
        //pJoint[jIndex].pJVert[ pJoint[jIndex].initCounter ].trans_vert = pVert[i];
        //pJoint[jIndex].initCounter++;
    }

    // jointVertCountTest should match vertCount
    assert( jointVertCountTest == vertCount );

    // Test that all counters match
    // if they don't match then there's a problem
    for( unsigned int i = 0; i < jointCount; i++ )
    {
        assert( pJoint[i].initCounter == pJoint[i].vCount );
    }

    // compute the radius
    radiusSqrt = (float)sqrt( maxDistance );
    radius = maxDistance;

    // Add faces to the vector
    for( int i = 0; i < faceCount; i++ )
    {
        for( int j = 0; j < 3; j++ )
        {
            // Get the index into the vert list
            int vIndex = pBinaryFace[i].vert[j];

            // Get the joint index
            unsigned int jIndex = pVert[ vIndex ].j;

            // find the matching verts to copy over the normal
            // and associate the group/face/vert with the joint
            for( int jVert = 0; jVert < pJoint[jIndex].vCount; jVert++ )
            {
                if( pJoint[jIndex].pJVert[jVert].vert == pVert[ vIndex ] )
                {
                    // record where this vert lives in multiple places
                    //pJoint[jIndex].pJVert[jVert].faceIndexLst.push_back(i);
                    //pJoint[jIndex].pJVert[jVert].faceVertIndexLst.push_back(j);

                    break;
                }
            }

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

        // Calculate the radius of the polygon
        pFace[i].CalcBoundingPoly();
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
    DeleteArray( pBinaryJoint );*/

    return result;

}	// LoadFromRSS


/************************************************************************
*    desc:  Load the mesh animation from RSA file
*  
*    param: string & name - name of animation
*           float - default animation speed
*           string & animFilePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowJointAnimMesh::LoadAnimFromRSA( string & name, float fps, string & animFilePath )
{
    /*CMeshAnim * pMeshAnim = NULL;

    // See if this joint animation has already been loaded
    // Meshes are shared so multiple animation loads will be attemped
    map<std::string, CJointLinkLst<CJointNode *> *>::iterator iter = jointLinkLst.find( name );

    // If it's not found, load the joint animation and add it to the list
    if( iter == jointLinkLst.end() )
    {
        if( (pMeshAnim = CJointAnimMgr::Instance().LoadFromFile( animFilePath )) == NULL )
        {
            return false;
        }

        // Do some sanity checks

        // Make sure we have the same number of joints
        assert( jointCount == pMeshAnim->jCount );

        // Make sure the joints all have the same name and parent name
        for( int i = 0; i < pMeshAnim->jCount; i++ )
        {
            assert( pJoint[i].name == pMeshAnim->pJointAnim[i].name );
            assert( pJoint[i].parent == pMeshAnim->pJointAnim[i].parent );
        }

        // Allocate a temporary multi-link list class
        CJointLinkLst<CJointNode *> * pJointMultiLink = new CJointLinkLst<CJointNode *>;

        pJointMultiLink->SetFrameCount( pMeshAnim->fCount );
        pJointMultiLink->SetSpeed( fps / 1000.f );

        // Create the multi-link list
        for( unsigned int i = 0; i < jointCount; i++ )
        {
            CJointNode * pTmpJoint = new CJointNode();

            pTmpJoint->id = pJoint[i].name;
            pTmpJoint->parentId = pJoint[i].parent;
            pTmpJoint->pJoint = &pJoint[i];
            pTmpJoint->pJointAnim = &pMeshAnim->pJointAnim[i];

            pJointMultiLink->AddNode( pTmpJoint );
        }

        // Add the link list class to our map
        jointLinkLst.insert( make_pair(name, pJointMultiLink) );
    }*/

    return true;

}	// LoadAnimFromRSA


/************************************************************************
*    desc:  Get the animation
*  
*    param: string & name - name of animation
*
*    ret: CJointLinkLst<CJointNode *> * - pointer to link list animation
************************************************************************/
CJointLinkLst<CJointNode *> * CShadowJointAnimMesh::GetAnimation( std::string & name )
{
    return jointLinkLst[name];

}	// SetAnimation


/************************************************************************
*    desc:  Calculate the surface normals. Needed for rebuilding the
*           shadow based on it's positio to the light source
************************************************************************/
void CShadowJointAnimMesh::CalcSurfaceNormal()
{
    // Need to recalculate the surface normals
    for( int i = 0; i < faceCount; i++ )
    {
        pFace[i].CalcSurfaceNormal();
    }

}	// CalcSurfaceNormal


/************************************************************************
*    desc:  Load the mesh from XML file
*  
*    param: string & _filePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************
bool CShadowJointAnimMesh::LoadFromXML( string & _filePath )
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
    const int ATTR_J(3);

    const int JOINT_VERT_COUNT(0);
    const int JOINT_NAME(1);
    const int JOINT_PARENT(2);
    const int JOINT_X(3);
    const int JOINT_Y(4);
    const int JOINT_Z(5);

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

    // Get a node to the joint list
    XMLNode jointLstNode = mainNode.getChildNode("joint_list");

    // Get the counts
    int vertCount = vertLstNode.nChildNode();
    faceCount = faceLstNode.nChildNode();
    jointCount = jointLstNode.nChildNode();

    // Allocate the arrays
    CJPoint * pVert = new CJPoint[vertCount];
    pFace = new CFace[faceCount];
    pJoint = new CJoint[jointCount];

    int jointVertCountTest = 0;

    // Load the joint info
    for( unsigned int i = 0; i < jointCount; i++ )
    {
        XMLNode jointNode = jointLstNode.getChildNode(i);

        pJoint[i].vCount = atoi(jointNode.getAttributeValue(JOINT_VERT_COUNT));
        pJoint[i].name = jointNode.getAttributeValue(JOINT_NAME);
        pJoint[i].parent = jointNode.getAttributeValue(JOINT_PARENT);

        pJoint[i].pos.x = atof(jointNode.getAttributeValue(JOINT_X));
        pJoint[i].pos.y = atof(jointNode.getAttributeValue(JOINT_Y));
        pJoint[i].pos.z = atof(jointNode.getAttributeValue(JOINT_Z));

        pJoint[i].pJVert = new CJVertex[ pJoint[i].vCount ];

        // Add up the joint count to make sure they equil the vert count
        jointVertCountTest += pJoint[i].vCount;
    }

    // jointVertCountTest should match vertCount
    assert( jointVertCountTest == vertCount );

    // Load in the verts
    for( int i = 0; i < vertCount; i++ )
    {
        XMLNode vertNode = vertLstNode.getChildNode(i);
        pVert[i].x = atof(vertNode.getAttributeValue(ATTR_X));
        pVert[i].y = atof(vertNode.getAttributeValue(ATTR_Y));
        pVert[i].z = atof(vertNode.getAttributeValue(ATTR_Z));
        pVert[i].j = (unsigned int)atoi(vertNode.getAttributeValue(ATTR_J));

        /////////////////////////////////////////////////////////////
        // Here we are copying the verts to the joint[]->jvert class
        // for finding the normals and faces that apply. We'll
        // be using these classed to transform our verts later
        // for the different animations
        /////////////////////////////////////////////////////////////

        // Get the joint index
        unsigned int jIndex = pVert[i].j;

        // should always be less the total number of joints
        assert( jIndex < jointCount );

        // should always be less the total number of verts added
        assert( pJoint[jIndex].initCounter < pJoint[jIndex].vCount );

        // add the vert and inc the counter
        pJoint[jIndex].pJVert[ pJoint[jIndex].initCounter ].vert = pVert[i];
        pJoint[jIndex].pJVert[ pJoint[jIndex].initCounter ].trans_vert = pVert[i];
        pJoint[jIndex].initCounter++;
    }

    // Test that all counters match
    // if they don't match then there's a problem
    for( unsigned int i = 0; i < jointCount; i++ )
    {
        assert( pJoint[i].initCounter == pJoint[i].vCount );
    }

    // Load the data from the xml file
    for( int i = 0; i < faceCount; i++ )
    {
        // get a node to the face list
        XMLNode faceNode = faceLstNode.getChildNode(i);

        for( int j = 0; j < 3; j++ )
        {
            // Get the index into each of the lists
            int vIndex = atoi(faceNode.getAttributeValue(VERT_ATTR[j]));

            // Get the vertex values
            pFace[i].vert[j] = pVert[vIndex];

            // Get the joint index
            unsigned int jIndex = pVert[vIndex].j;

            // find the matching verts to copy over the face/vert index
            for( int jVert = 0; jVert < pJoint[jIndex].vCount; jVert++ )
            {
                if( pJoint[jIndex].pJVert[jVert].vert == pVert[vIndex] )
                {
                    // record where this vert lives in multiple places
                    pJoint[jIndex].pJVert[jVert].faceIndexLst.push_back(i);
                    pJoint[jIndex].pJVert[jVert].faceVertIndexLst.push_back(j);

                    break;
                }
            }

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

        // Calculate the radius of the polygon
        pFace[i].CalcBoundingPoly();
    }

    // compute the radius
    radiusSqrt = (float)sqrt( maxDistance );
    radius = maxDistance;

    // Delete the temporary arrays
    delete [] pVert;

    /*for( int i = 0; i < jointCount; i++ )
    {
        for( int j = 0; j < pJoint[i].count; j++ )
        {
            for( int w = 0; w < pJoint[i].pJVert[j].grpIndexLst.size(); w++ )
            {
                PostDebugMsg( "joint: %d, grpIndex: %d, faceIndex: %d, faceVertIndex: %d", 
                              i, pJoint[i].pJVert[j].grpIndexLst[w], 
                              pJoint[i].pJVert[j].faceIndexLst[w], 
                              pJoint[i].pJVert[j].faceVertIndexLst[w] );
            }
        }
    }*

    return true;

}	// LoadFromXML
*/

/************************************************************************
*    desc:  Load the mesh animation from XML file
*  
*    param: string & name - name of animation
*           float - default animation speed
*           string & animFilePath - path to file
*    
*    return: bool - true on success or false on fail
************************************************************************/
/*bool CShadowJointAnimMesh::LoadAnimFromXML( string & name, float speed, string & animFilePath )
{
    CMeshAnim * pMeshAnim = NULL;

    // See if this joint animation has already been loaded
    // Meshes are shared so multiple animation loads will be attemped
    map<std::string, CJointLinkLst<CJointNode *> *>::iterator iter = jointLinkLst.find( name );

    // If it's not found, load the joint animation and add it to the list
    if( iter == jointLinkLst.end() )
    {
        if( (pMeshAnim = CJointAnimMgr::Instance().LoadFromFile( animFilePath )) == NULL )
        {
            return false;
        }

        // Do some sanity checks

        // Make sure we have the same number of joints
        assert( jointCount == pMeshAnim->jCount );

        // Make sure the joints all have the same name and parent name
        for( int i = 0; i < pMeshAnim->jCount; i++ )
        {
            assert( pJoint[i].name == pMeshAnim->pJointAnim[i].name );
            assert( pJoint[i].parent == pMeshAnim->pJointAnim[i].parent );
        }

        // Allocate a temporary multi-link list class
        CJointLinkLst<CJointNode *> * pJointMultiLink = new CJointLinkLst<CJointNode *>;

        pJointMultiLink->SetFrameCount( pMeshAnim->pJointAnim->keyFrameCount );
        pJointMultiLink->SetSpeed( speed );

        // Create the multi-link list
        for( unsigned int i = 0; i < jointCount; i++ )
        {
            CJointNode * pTmpJoint = new CJointNode();

            pTmpJoint->id = pJoint[i].name;
            pTmpJoint->parentId = pJoint[i].parent;
            pTmpJoint->pJoint = &pJoint[i];
            pTmpJoint->pJointAnim = &pMeshAnim->pJointAnim[i];

            pJointMultiLink->AddNode( pTmpJoint );
        }

        // Add the link list class to our map
        jointLinkLst.insert( make_pair(name, pJointMultiLink) );
    }

    return true;

}*/	// LoadAnimFromXML