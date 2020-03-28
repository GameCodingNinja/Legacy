/************************************************************************
*    FILE NAME:       objectdatalist2d.h
*
*    DESCRIPTION:     Class that holds a map of all 2D object data used 
*					  for later loading
************************************************************************/

#ifndef __object_data_list_2d_h__
#define __object_data_list_2d_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/scoped_ptr.hpp>

// Game lib dependencies


// Forward declaration(s)
class CObjectData2D;
struct XMLNode;

class CObjectDataList2D
{
public:

    // Get the instance of the singleton class
    static CObjectDataList2D & Instance()
    {
        static CObjectDataList2D dataList;
        return dataList;
    }

    // Load the data list table
    void LoadListTable( const std::string & filePath );

    // Get a specific stage object's data
    CObjectData2D * GetData( const std::string & group, const std::string & name );

    // Load all of the meshes and materials of a specific data group
    void LoadDataGroup( const std::string & group );

    // Free all of the meshes and materials of a specific data group
    void FreeDataGroup( const std::string & group );

private:

    CObjectDataList2D();
    ~CObjectDataList2D();

    // Load all object information from an xml
    void LoadFromXML( const std::string & group, const std::string & filePath );

    // Load an object from the node
    void LoadObjectFromNode( const XMLNode & objectNode, 
                             CObjectData2D * pObjectData );

private:

    // Holds the data list table map
    std::map<const std::string, const std::string> dataListTableMap;
    std::map<const std::string, const std::string>::iterator dataListTableIter;

    // Map in a map of all the objects' data
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CObjectData2D>> spObjectDataMapMap;

    // Two iterators. One for the group map, and one for the data map inside of the group map
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CObjectData2D>>::iterator groupMapIter;
    boost::ptr_map<const std::string, CObjectData2D>::iterator dataMapIter;

};

#endif  // __object_data_list_2d_h__
