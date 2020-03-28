/************************************************************************
*    FILE NAME:       mesh3d.cpp
*
*    DESCRIPTION:     3D DirectX mesh class
************************************************************************/

// Physical component dependency
#include <3d/mesh3d.h>

// Game lib dependencies
#include <common/face.h>
#include <3d/joint.h>
#include <3d/jointnode.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CMesh3D::CMesh3D()
       : radius(0.0f),
         radiusSqrt(0.0f)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CMesh3D::~CMesh3D()
{
}   // Destructer


/************************************************************************
*    desc:  Render this mesh to the back buffer 
************************************************************************/
void CMesh3D::Render()
{
    // Base class has no action

}	// Render


/************************************************************************
*    desc:  Transform the mesh data
*
*    param: CFace * pTransFace - pointer to translated face list
*           float scale - scale the radius
*			CMatrix & matrix - passed in matrix class
*           CMatrix & unscaledMatrix - non scaled matrix
************************************************************************/
void CMesh3D::Transform( CFace * pTransFace, float scale,
                         CMatrix & matrix, CMatrix & unscaledMatrix )
{
    // Base class has no action

}	// Transform


/************************************************************************
*    desc:  Transform the mesh data
*
*    param: CFace * pTransFace - pointer to translated face list
*			CMatrix & matrix - passed in matrix class
*           CMatrix & unscaledMatrix - non scaled matrix
************************************************************************/
void CMesh3D::Transform( CFace * pTransFace,
                         CMatrix & matrix, CMatrix & unscaledMatrix )
{
    // Base class has no action

}	// Transform


/***************************************************************************
*   desc:  Get the radius
*
*   ret:  float - value of radius
 ****************************************************************************/
float CMesh3D::GetRadius()
{
    return radius;

}	// GetRadius


/***************************************************************************
*   desc:  Get the square rooted radius
*
*   ret:  float - value of radius
 ****************************************************************************/
float CMesh3D::GetRadiusSqrt()
{
    return radiusSqrt;

}	// GetRadiusSqrt


/***************************************************************************
*   desc:  Get the face count
*
*   parm: unsigned int index - index into face group
*
*   ret:  unsigned int - face count
 ****************************************************************************/
unsigned int CMesh3D::GetFaceCount( unsigned int index )
{
    // base clase has no action

    return 0;

}	// GetFaceCount


/***************************************************************************
*   desc:  Get the face group count
*
*   ret:  unsigned int - face count
 ****************************************************************************/
unsigned int CMesh3D::GetFaceGroupCount()
{
    // base clase has no action

    return 0;

}	// GetFaceGroupCount


/***************************************************************************
*   desc:  Load the mesh animation from XML file
*
*   ret:  bool
 ****************************************************************************/
void CMesh3D::LoadAnimFromRSA( string & name, float speed, string & animFilePath )
{
}	// LoadAnimFromRSA


/***************************************************************************
*   desc:  Get the animation
*
*   ret:  bool
 ****************************************************************************/
CJointLinkLst<CJointNode *> * CMesh3D::GetAnimation( std::string & name )
{
    return NULL;

}	// GetAnimation


/************************************************************************
*    desc:  Calculate the surface normals
************************************************************************/
void CMesh3D::CalcSurfaceNormal()
{
    // base clase has no action

}	// CalcSurfaceNormal


/************************************************************************
*    desc:  Get the joint
*
*    param: index - index into array
*
*    ret: CJoint & - pointer to the joint
************************************************************************/
CJoint * CMesh3D::GetJoint( unsigned int index )
{
    return NULL;

}	// GetJointArray


/************************************************************************
*    desc:  Get the joint count
*
*    ret: unsigned int - number of joints
************************************************************************/
unsigned int CMesh3D::GetJointCount()
{
    return 0;

}	// GetJointCount
