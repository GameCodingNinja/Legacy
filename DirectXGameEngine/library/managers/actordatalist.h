
/************************************************************************
*    FILE NAME:       actordatalist.h
*
*    DESCRIPTION:     CActorDataList Class
************************************************************************/

#ifndef __actor_data_list_h__
#define __actor_data_list_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Class Fowarding
class CActorData;

class CActorDataList
{
public:

    // Get the instance of the singleton class
    static CActorDataList & Instance()
    {
        static CActorDataList actorDataList;
        return actorDataList;
    }

    // Clear out the actor data
    void Clear();

    // Load all stage object information from an xml
    void LoadFromXML( const std::string & filePath );

    // Get a specific stage object's data
    CActorData * GetData( const std::string & name );

private:

    // Constructor
    CActorDataList();

    // Destructor
    ~CActorDataList();

private:

    boost::ptr_map<std::string, CActorData> spActorDataMap;
    boost::ptr_map<std::string, CActorData>::iterator actorDataMapIter;

};

#endif  // __actor_data_list_h__


