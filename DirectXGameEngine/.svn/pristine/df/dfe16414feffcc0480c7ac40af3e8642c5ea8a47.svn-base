
/************************************************************************
*    FILE NAME:       actordata.h
*
*    DESCRIPTION:     CActorData Class
************************************************************************/

#ifndef __actor_data_h__
#define __actor_data_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/worldpoint.h>
#include <common/spritedata.h>
#include <common/defs.h>
#include <2d/objectcollisiondata2d.h>

// Forward Declarations
struct XMLNode;

class CActorData : public boost::noncopyable
{
public:

    // Constructor
    CActorData();

    // Destructor
    virtual ~CActorData();

    // Load thes actor data from node
    void LoadFromNode( const XMLNode & actorDataNode );

    // Get the position
    const CWorldPoint & GetPos() const;

    // Get the AI name
    const std::string & GetAIName() const;

    // Get the instance name
    const std::string & GetInstance() const;

    // Get sprite Data size
    size_t GetSpriteDataSize() const;

    // Get sprite Data - Allow outside functions to change data
    CSpriteData & GetSpriteData( size_t index );

    // Get the collision data
    const CObjectCollisionData2D & GetCollisionData() const;

private:

    // load the data from xml file
    void LoadFromXML( const std::string & filePath );

private:

    // Actor position
    CWorldPoint pos;

    // Name of the ai
    std::string aiName;

    // Name of the ai
    std::string instance;

    // actor data safe vector
    boost::ptr_vector<CSpriteData> spSpriteDataVec;

    // Actor collision data
    CObjectCollisionData2D colData;

};

#endif  // __actor_data_h__


