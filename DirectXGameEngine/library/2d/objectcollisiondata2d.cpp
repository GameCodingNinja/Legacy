
/************************************************************************
*    FILE NAME:       objectcollisiondata2d.cpp
*
*    DESCRIPTION:     Class containing the 2D object's collision data
************************************************************************/

// Physical component dependency
#include <2d/objectcollisiondata2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <common/color.h>
#include <utilities/xmlparsehelper.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CObjectCollisionData2D::CObjectCollisionData2D()
                      : type(b2_staticBody),
                        topMod(0),
                        bottomMod(0),
                        rightMod(0),
                        leftMod(0),
                        genType(ECGT_NULL),
                        density(0),
                        restitution(0),
                        damping(0),
                        angDamping(0),
                        fixedRotation(false)
{
}   // constructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CObjectCollisionData2D::LoadFromNode( const XMLNode & objectNode )
{
    const XMLNode collisionNode = objectNode.getChildNode( "collision" );

    // Check if the object has any collision data
    if( !collisionNode.isEmpty() )
    {
        if( collisionNode.isAttributeSet("file") )
        {
            // Get the file name
            file = collisionNode.getAttribute( "file" );

            // Get the physics world name
            if( collisionNode.isAttributeSet( "world" ) )
                world = collisionNode.getAttribute( "world" );

            // Get the collision sprite type
            if( collisionNode.isAttributeSet( "type" ) )
            {
                string bodyType = collisionNode.getAttribute( "type" );

                if( bodyType == "dynamic" )
                    type = b2_dynamicBody;
                else if( bodyType == "kinematic" )
                    type = b2_kinematicBody;
            }

            // If the file is not a file but is asking to generate a file, we check
            // to see what kind of collision mesh it wants to generate
            if( file == "rect" )
            {
                // Default collision is a collision mesh that is the size of the visual mesh
                genType = ECGT_DEFAULT;

                // The other kinds of collision meshes that can be generate are triangular meshes
                // that take up a specific half of the visual mesh size
                if( collisionNode.isAttributeSet( "genType" ) )
                {
                    std::string genTypeStr = collisionNode.getAttribute( "genType" );

                    if( genTypeStr == "tl" || genTypeStr == "top_left" )
                        genType = ECGT_TOP_LEFT;

                    else if( genTypeStr == "tr"  || genTypeStr == "top_right" )
                        genType = ECGT_TOP_RIGHT;

                    else if( genTypeStr == "bl" || genTypeStr == "bottom_left" )
                        genType = ECGT_BOTTOM_LEFT;

                    else if( genTypeStr == "br" || genTypeStr == "bottom_right" )
                        genType = ECGT_BOTTOM_RIGHT;
                }
            }
        }

        // The size is the desired size of the collision mesh, which overrites the size of the image
        const XMLNode sizeNode = collisionNode.getChildNode( "size" );

        if( !sizeNode.isEmpty() )
            size = NParseHelper::LoadSize( sizeNode );

        // The size mod is how much the collision mesh should be adjusted on each side
        const XMLNode sizeModNode = collisionNode.getChildNode( "sizeMod" );

        // Check if the object has any collision data
        if( !sizeModNode.isEmpty() )
        {
            if( sizeModNode.isAttributeSet("top") )
                topMod = atof( sizeModNode.getAttribute( "top" ) );

            if( sizeModNode.isAttributeSet("bottom") )
                bottomMod = atof( sizeModNode.getAttribute( "bottom" ) );

            if( sizeModNode.isAttributeSet("left") )
                leftMod = atof( sizeModNode.getAttribute( "left" ) );

            if( sizeModNode.isAttributeSet("right") )
                rightMod = atof( sizeModNode.getAttribute( "right" ) );

            // If we've set the collision size, let's modify it
            if( !size.IsEmpty() )
            {
                size.w += rightMod + leftMod;
                size.h += topMod + bottomMod;
            }
        }

        // The body of the collision sprite used for physics
        const XMLNode bodyNode = collisionNode.getChildNode( "body" );

        if( !bodyNode.isEmpty() )
        {
            // The density is how much the object resists movement
            if( bodyNode.isAttributeSet("density") )
                density = atof( bodyNode.getAttribute( "density" ) );

            // The restitution is the percentage of velocity retained upon collision
            if( bodyNode.isAttributeSet("restitution") )
                restitution = atof( bodyNode.getAttribute( "restitution" ) );

            // The damping is the constant decceleration of movement and rotation
            if( bodyNode.isAttributeSet("damping") )
                damping = atof( bodyNode.getAttribute( "damping" ) );

            // The angular damping is the constant decceleration of rotation
            if( bodyNode.isAttributeSet("angDamping") )
                angDamping = atof( bodyNode.getAttribute( "angDamping" ) );

            // Whether the rotation due to collisions is fixed
            if( bodyNode.isAttributeSet("fixedRotation") )
                if( string( bodyNode.getAttribute("fixedRotation") ) == "true" )
                    fixedRotation = true;
        }

        // Set the color for testing purposes
        color = NParseHelper::LoadColor( collisionNode );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Get the file path
************************************************************************/
const std::string & CObjectCollisionData2D::GetFile() const 
{
    return file;
}


/************************************************************************
*    desc:  Get the type of collision sprite
************************************************************************/
b2BodyType CObjectCollisionData2D::GetType() const 
{
    return type;
}


/************************************************************************
*    desc:  Get the physics world name
************************************************************************/
const std::string & CObjectCollisionData2D::GetWorld() const 
{
    return world;
}


/************************************************************************
*    desc:  Get the size
************************************************************************/
const CSize<float> & CObjectCollisionData2D::GetSize() const
{
    return size;
}


/************************************************************************
*    desc:  Get the top mod
************************************************************************/
float CObjectCollisionData2D::GetTopMod() const
{
    return topMod;
}


/************************************************************************
*    desc:  Get the bottom mod
************************************************************************/
float CObjectCollisionData2D::GetBottomMod() const
{
    return bottomMod;
}


/************************************************************************
*    desc:  Get the left mod
************************************************************************/
float CObjectCollisionData2D::GetLeftMod() const
{
    return leftMod;
}


/************************************************************************
*    desc:  Get the left mod
************************************************************************/
float CObjectCollisionData2D::GetRightMod() const
{
    return rightMod;
}


/************************************************************************
*    desc:  Get the mass
************************************************************************/
EColGenType CObjectCollisionData2D::GetGenType() const
{
    return genType;
}


/************************************************************************
*    desc:  Get the density
************************************************************************/
float CObjectCollisionData2D::GetDensity() const
{
    return density;
}


/************************************************************************
*    desc:  Get the restitution
************************************************************************/
float CObjectCollisionData2D::GetRestitution() const
{
    return restitution;
}


/************************************************************************
*    desc:  Get the damping
************************************************************************/
float CObjectCollisionData2D::GetDamping() const
{
    return damping;
}


/************************************************************************
*    desc:  Get the angular damping
************************************************************************/
float CObjectCollisionData2D::GetAngDamping() const
{
    return angDamping;
}


/************************************************************************
*    desc:  Is the rotation fixed
************************************************************************/
bool CObjectCollisionData2D::IsRotationFixed() const
{
    return fixedRotation;
}


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CObjectCollisionData2D::GetColor() const 
{
    return color;
}

