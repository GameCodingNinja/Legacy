/************************************************************************
*    FILE NAME:       collisionsprite3d.cpp
*
*    DESCRIPTION:     3D collision sprite class
************************************************************************/

// Physical component dependency
#include <3d/collisionsprite3d.h>

// Game lib dependencies
#include <system/xdevice.h>
#include <managers/meshmanager.h>
#include <3d/mesh3d.h>
#include <3d/objectdatalist3d.h>
#include <3d/objectdata3d.h>
#include <common/face.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionSprite3D::CCollisionSprite3D()
                  : pObjectData(NULL)
{
}   // Constructer

CCollisionSprite3D::CCollisionSprite3D( CObjectData3D * pObjData )
                  : pObjectData(pObjData)
{
    Init();

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollisionSprite3D::~CCollisionSprite3D()
{
}   // Destructer


/************************************************************************
*    desc:  Init the sprite group
************************************************************************/
void CCollisionSprite3D::Init( const std::string & group, const std::string & type )
{
    pObjectData = CObjectDataList3D::Instance().GetData( group, type );

    Init();

}	// Init


/************************************************************************
*    desc:  Init the sprite
************************************************************************/
void CCollisionSprite3D::Init()
{
    // NOTE: This class does not own this pointer.
    pMesh = CMeshMgr::Instance().GetCollisionMesh3D( pObjectData );

    // Allocate our transformation face array
    spTransFace.reset( new CFace[pMesh->GetFaceCount()] );

}   // Init


/************************************************************************
*    desc:  Check for collision and react to it. Camera view collision
*
*    param: CMatric & matrix - passed in matrix class
*			CPoint center - Center point of the object
*           float radius - radius of the object
*           float floorPadding - padding for collision
*           float wallPadding - padding for collision
*
*    ret: CCollisionCheck & - All collision information
************************************************************************/
CCollisionCheck & CCollisionSprite3D::ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
                                                        float floorPadding, float wallPadding )
{
    // Transform the center point - needed for IsCollision_BoundingSphere()
    Transform( matrix );

    return Collision( center, radius, floorPadding, wallPadding );

}	// ReactToCollision


/************************************************************************
*    desc:  Check for collision and react to it. World view collision
*
*    param: CPoint center - Center point of the object
*           float radius - radius of the object
*           float floorPadding - padding for collision
*           float wallPadding - padding for collision
*
*    ret: CCollisionCheck & - All collision information
************************************************************************/
CCollisionCheck & CCollisionSprite3D::ReactToCollision( CPoint & center, float radius,
                                                        float floorPadding, float wallPadding )
{
    // Transform the center point - needed for IsCollision_BoundingSphere()
    Transform();

    return Collision( center, radius, floorPadding, wallPadding );

}	// ReactToCollision


/************************************************************************
*    desc:  Check for collision and react to it.
*
*    param: CPoint center - Center point of the object
*           float radius - radius of the object
*           float floorPadding - padding for collision
*           float wallPadding - padding for collision
*
*    ret: CCollisionCheck & - All collision information
************************************************************************/
CCollisionCheck & CCollisionSprite3D::Collision( CPoint & center, float radius,
                                                 float floorPadding, float wallPadding )
{
    // Clear the collision flag
    colCheck.ClearCollisionFlag();

    // See if we are colliding with the object before we
    // transform the data and do the real tests
    if( CSprite3D::IsCollision_BoundingSphere( center, radius ) )
    {
        CMatrix scaleMatrix;

        // Reset the collision class
        colCheck.Reset();

        // Handle the scaling
        scaleMatrix.Scale( GetScale() );

        // Merge in the unscaledMatrix matrix to convert to camera view
        scaleMatrix *= GetUnscaledMatrix();

        // Inc our stat counter to keep track of what is going on.
        CStatCounter::Instance().IncCollisionCounter();

        // Transform the mesh
        // NOTE: Since we are tranforming our collision mesh, you can't
        // scale normals. So for this, we have a seperate matix without scale.
        //pMesh->Transform( pTransFace, scale, scaleMatrix, unscaledMatrix );

        // Do the bounding sphere check on each of the poly faces
        if( IsCollision_BoundingSphere( center, radius ) )
        {
            // Do the intersection check
            IsCollision_Intersection( center, floorPadding, wallPadding );
        }
    }

    return colCheck;

}	// Collision


/************************************************************************
*    desc:  Do bounding sphere face collision check. This is to find the
*           closest faces to do the intersection check against.
*
*           NOTE: If you're bouncing on the floor or when hitting the wall
*                 your radius is too small. You will bounce some when going
*                 into the corner.
*
*    param: CPoint center - Center point of the object
*           float radius - radius of the object
************************************************************************/
bool CCollisionSprite3D::IsCollision_BoundingSphere( CPoint & center, float radius )
{
    bool result = false;

    // Go through the entire mesh and find the closest collision
    for( unsigned int i = 0; i < pMesh->GetFaceCount(); ++i )
    {
        // Get the length from the face center to the passed in center
        float distance = spTransFace[i].center.GetLengthSquared( center );

        // If the distance is less then the two radiuses, we might be colliding
        if( distance < spTransFace[i].radius + 2000 )
        {
            // Check the y normal to see if it is a floor
            if( spTransFace[i].normal.y > 0.1f )
            {
                if( distance < colCheck.col[EC_FLOOR].distance )
                {
                    // Make sure the plane is not back facing us
                    if( spTransFace[i].IsFacingPlane( center ) )
                    {
                        if( spTransFace[i].PointInFloor( center ) )
                        {
                            colCheck.col[EC_FLOOR].distance = distance;
                            colCheck.col[EC_FLOOR].index = i;
                            result = true;
                            //break;
                        }
                    }
                }
            }
            // if it's not a floor, then it's a wall
            else
            {
                if( distance < colCheck.col[EC_WALL].distance )
                {
                    // Make sure the plane is not back facing us
                    if( spTransFace[i].IsFacingPlane( center ) )
                    {
                        if( spTransFace[i].PointInWall( center ) )	
                        {
                            colCheck.col[EC_WALL].distance = distance;
                            colCheck.col[EC_WALL].index = i;
                            result = true;
                            //break;
                        }
                    }
                }
            }
        }
    }

    return result;

}	// IsCollision_BoundingSphere


/************************************************************************
*    desc:  Last check for collision. Much more accurate then bounding
*           spheres and also much slower which is why this check is
*           is performed after a bounding sphere's tests are finished. 
*           Also, intersection collision is testing if the plane
*           was pierced by a point. We are talking a plain, not a
*           polygon. So the test is not confined to the size of the
*           polygon, but the plain it is on. A plain is infinite.
*
*    param: CCollisionCheck & colCheck - class for storing collision info
*			CPoint & center - point to check.              
*           float floorPadding - padding for collision
*           float wallPadding - padding for collision
*              
*           *NOTE: If in First Persion point of view, use 0,0,0 as your
*                  x,y,z point.
************************************************************************/
bool CCollisionSprite3D::IsCollision_Intersection( CPoint & center, float floorPadding,
                                         float wallPadding )
{
    // Do intersection check if we had a wall collision
    for( int i = 0; i < EC_MAX; ++i )
    {
        if( colCheck.col[i].IsCollision() )
        {
            float padding;

            if( i == EC_FLOOR )
            {
                padding = floorPadding;
            }
            else
            {
                padding = wallPadding;
            }

            // get the closest face we collided with during the bounding sphere check
            CFace * pFaceTmp = &spTransFace[colCheck.col[i].index];

            // Did we pierce the plane?
            colCheck.col[i].planePierceOffset = pFaceTmp->PointToPlaneDistance( center ) - padding;

            // Did we pierce the plane?
            if( colCheck.col[i].planePierceOffset < 0.0f )
            {
                CPoint offset;

                // Set the flag to indicate we had a collision
                colCheck.SetCollision( true );

                // Set the offset based on what we collided against
                if( i == EC_FLOOR )
                {
                    offset.y = colCheck.col[i].planePierceOffset * pFaceTmp->normal.y;
                }
                else
                {
                    offset.z = colCheck.col[i].planePierceOffset * pFaceTmp->normal.z;
                    offset.x = colCheck.col[i].planePierceOffset * pFaceTmp->normal.x;
                    offset.y = colCheck.col[i].planePierceOffset * pFaceTmp->normal.y;
                }

                // Record this information in case we need it later
                colCheck.col[i].offset = offset;
                colCheck.col[i].normal = pFaceTmp->normal;
                colCheck.col[i].center = pFaceTmp->center;
            }
        }
    }

    return colCheck.IsCollision();

}   // IsCollision_Intersection


/************************************************************************
*    desc:  Get the collision data
*
*    ret:	const CObjectCollisionData2D & - collision data
************************************************************************/
const CObjectCollisionData3D & CCollisionSprite3D::GetCollisionData() const
{
    return pObjectData->GetCollisionData();

}	// GetCollisionData





/*bool CCollisionSprite3D::IsCameraCollision_BoundingSphere( CPoint & center, float radius )
{
    bool result = false;

    // Go through the entire mesh and find the closest collision
    for( unsigned int i = 0; i < pMesh->GetFaceCount(); ++i )
    {
        // Get the length from the face center to the passed in center
        float distance = pTransFace[i].center.GetLength( center );

        // If the distance is less then the two radiuses, we might be colliding
        if( distance < pTransFace[i].radius + radius )
        {
            if( distance < colCheck.col[EC_FLOOR].distance )
            {
                // Make sure the plane is not back facing us
                if( pTransFace[i].IsFacingPlane( center ) )
                {
                    colCheck.col[EC_FLOOR].distance = distance;
                    colCheck.col[EC_FLOOR].index = i;
                    result = true;
                }
            }
        }
    }

    return result;
}   // IsCameraCollision_BoundingSphere


CCollisionCheck & CCollisionSprite3D::ReactToCameraCollision( CMatrix & matrix, CPoint & center, float radius,
                                                              float padding )
{
    // Reset the collision class
    colCheck.Reset();

    // Do our translations and rotations in a temporary matrix
    CMatrix unscaledMatrix;
    CMatrix scaleMatrix;

    // Handle the scaling
    scaleMatrix.Scale( scale );

    // Set the matrix to world view
    SetMatrixToWorldView( unscaledMatrix );
    SetMatrixToWorldView( scaleMatrix );

    // Merge in the passed in matrix to convert to camera view
    // This camera view does not include x rotations
    scaleMatrix.MergeMatrix( matrix );
    unscaledMatrix.MergeMatrix( matrix );

    // Transform the center point
    CSprite::Transform( scaleMatrix );

    // See if we are colliding with the object before we
    // transform the data and do the real tests
    if( CSprite::IsCollision_BoundingSphere( center, radius ) )
    {
        // Inc our stat counter to keep track of what is going on.
        CStatCounter::Instance().IncCollisionCounter();

        // Transform the mesh
        // NOTE: Since we are tranforming our collision mesh, you can't
        // scale normals. So for this, we have a seperate matix without scale.
        pMesh->Transform( pTransFace, scale, scaleMatrix, unscaledMatrix );

        // Do the bounding sphere check on each of the poly faces
        if( IsCameraCollision_BoundingSphere( center, radius ) )
        {
            // Do the intersection check
            IsCameraCollision_Intersection( center, padding );
        }
    }

    return colCheck;

}	// ReactToCameraCollision


bool CCollisionSprite3D::IsCameraCollision_Intersection( CPoint & center, float padding )
{
    if( colCheck.col[EC_FLOOR].IsCollision() )
    {
        // get the closest face we collided with during the bounding sphere check
        CFace * pFaceTmp = &pTransFace[colCheck.col[EC_FLOOR].index];

        // Did we pierce the plane?
        colCheck.col[EC_FLOOR].planePierceOffset = pFaceTmp->AngleToPlane( center ) + -padding;

        // Did we pierce the plane?
        if( colCheck.col[EC_FLOOR].planePierceOffset < 0.0f )
        {
            CPoint offset;

            // Set the flag to indicate we had a collision
            colCheck.SetCollision( true );

            // Compute the y delta
            if( pFaceTmp->normal.y != 0.0f )
            {
                offset.y = (colCheck.col[EC_FLOOR].planePierceOffset / pFaceTmp->normal.y);
            }
            else
            {
                offset.y = colCheck.col[EC_FLOOR].planePierceOffset;
            }

            // Record this information in case we need it later
            colCheck.col[EC_FLOOR].offset = offset;
            colCheck.col[EC_FLOOR].normal = pFaceTmp->normal;
            colCheck.col[EC_FLOOR].center = pFaceTmp->center;
        }
    }

    return colCheck.IsCollision();

}   // IsCameraCollision_Intersection*/