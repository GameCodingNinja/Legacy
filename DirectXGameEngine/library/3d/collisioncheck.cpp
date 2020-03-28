/************************************************************************
*    FILE NAME:       collisioncheck.cpp
*
*    DESCRIPTION:     collision class
************************************************************************/

// Physical component dependency
#include <3d/collisioncheck.h>

// Standard lib dependencies
#include <math.h>

// Game lib dependencies
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCollisionCheck::CCollisionCheck()
{
    Reset();

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCollisionCheck::~CCollisionCheck()
{
}   // Destructer


/************************************************************************
*    desc:  Clear the collision flag                                                             
************************************************************************/
void CCollisionCheck::ClearCollisionFlag()
{
    collision = false;

}	// Reset


/************************************************************************
*    desc:  Reset the class data                                                             
************************************************************************/
void CCollisionCheck::Reset()
{
    col[EC_FLOOR].Reset();
    col[EC_WALL].Reset();
    offset.Clear();
    collision = false;

}	// Reset


/************************************************************************
*    desc:  Was there a collision?
*
*    ret:  bool - true if there was a collision
************************************************************************/
bool CCollisionCheck::IsCollision()
{
    return collision;

}	// IsCollision


/************************************************************************
*    desc:  Sort through the passed in collisions and keep the
*			closest floor or wall
*
*	 param: CCollisionCheck & colSort - passed in class to check
************************************************************************/
void CCollisionCheck::Sort( CCollisionCheck & colSort )
{
    // Was there a collision with the passed in class
    if( colSort.IsCollision() )
    {
        // Check the floor and wall to see if there was a collision
        // and if so, see if it was closer the what we currently have
        for( int i = 0; i < EC_MAX; ++i )
        {
            // Was there a collision and is it closer then what we have saved
            if( colSort.col[i].IsPlanePierce() && 
                colSort.col[i].distance < col[i].distance )
            {
                collision = true;

                col[i] = colSort.col[i];

                // Save the offset info
                if( i == EC_FLOOR )
                {
                    offset.y = col[i].offset.y;
                }
                else
                {
                    offset.z = col[i].offset.z;
                    offset.x = col[i].offset.x;
                }
            }
        }
    }

}	// Sort


/************************************************************************
*    desc:  Get view collision offset 
*
*	 param: CPoint & - Collision offset
************************************************************************/
CPoint & CCollisionCheck::GetFinalViewOffset()
{
    return offset;

}	// GetFinalViewOffset


/************************************************************************
*    desc:  Get object collision offset 
*
*	 param: CPoint & - Collision offset
************************************************************************/
CPoint & CCollisionCheck::GetFinalObjectOffset()
{
    // Copy the offset to our temporary point
    offsetConversion = offset;

    // Signs need to be flipped for object collision
    offsetConversion.FlipSigns();

    return offsetConversion;

}	// GetFinalObjectOffset


/************************************************************************
*    desc:  Set the collision flag. What this means is floor, wall or
*           both have a collision
*
*	 param: bool value - true or false for collision
************************************************************************/
void CCollisionCheck::SetCollision( bool value )
{
    collision = value;

}	// SetCollision

