
/************************************************************************
*    FILE NAME:       spritedata.h
*
*    DESCRIPTION:     Sprite Class
************************************************************************/

#ifndef __sprite_data_h__
#define __sprite_data_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/point.h>
#include <common/worldpoint.h>
#include <common/transformdata.h>

// Forward Declarations
struct XMLNode;

class CSpriteData
{
public:

    // Constructer
    CSpriteData();

    // Load the sector data from node
    void LoadFromNode( const XMLNode & spriteDataNode );

    // Get the transform data
    const CTransformData & GetTransformData() const;

    // Set-Get the group
    void SetGroup( const std::string & str );
    const std::string & GetGroup() const;

    // Set-Get the type
    void SetType( const std::string & str );
    const std::string & GetType() const;

    // Set-Get the unique mesh tag
    void SetUniqueMeshTag( const std::string & str );
    const std::string & GetUniqueMeshTag() const;

    // Set-Get the name
    void SetName( const std::string & str );
    const std::string & GetName() const;

    // Does this sprite use the camera
    void SetUseCamera( bool value );
    bool UseCamera() const;

private:

    CTransformData transData;

    std::string group;
    std::string type;
    std::string uniqueMeshTag;
    std::string name;

    bool useCamera;
};

#endif  // __sprite_data_h__


