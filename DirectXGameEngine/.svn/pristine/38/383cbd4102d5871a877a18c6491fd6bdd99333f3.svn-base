
/************************************************************************
*    FILE NAME:       objectcollisiondata3d.cpp
*
*    DESCRIPTION:     Class containing the 3D object's collision data
************************************************************************/

// Physical component dependency
#include <3d/objectcollisiondata3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <common/color.h>
#include <utilities/xmlparsehelper.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CObjectCollisionData3D::CObjectCollisionData3D()
                      : mass(0),
                        inertia(0),
                        restitution(0)
{
}   // constructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CObjectCollisionData3D::LoadFromNode( const XMLNode & objectNode )
{
    XMLNode collisionNode = objectNode.getChildNode( "collision" );

    if( !collisionNode.isEmpty() )
    {
        // Get the file info
        if( collisionNode.isAttributeSet("file") )
            file = collisionNode.getAttribute( "file" );

        // Get the projection info
        XMLNode rigidBodyNode = collisionNode.getChildNode( "body" );

        if( !rigidBodyNode.isEmpty() )
        {
            if( rigidBodyNode.isAttributeSet("mass") )
                mass = atof(rigidBodyNode.getAttribute( "mass" ));

            if( rigidBodyNode.isAttributeSet("restitution") )
                restitution = atof(rigidBodyNode.getAttribute( "restitution" ));

            if( rigidBodyNode.isAttributeSet("inertia") )
                inertia = atof(rigidBodyNode.getAttribute( "inertia" ));
        }

        // Get the color info
        color = NParseHelper::LoadColor( collisionNode );
    }

}	// LoadFromNode


/************************************************************************
*    desc:  Get the file path
************************************************************************/
const std::string & CObjectCollisionData3D::GetFile() const 
{
    return file;
}


/************************************************************************
*    desc:  Get the mass
************************************************************************/
float CObjectCollisionData3D::GetMass() const
{
    return mass;
}


/************************************************************************
*    desc:  Get the inertia
************************************************************************/
float CObjectCollisionData3D::GetInertia() const
{
    return inertia;
}


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CObjectCollisionData3D::GetColor() const 
{
    return color;
}

