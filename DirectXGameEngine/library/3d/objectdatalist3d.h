/************************************************************************
*    FILE NAME:       objectdatalist3d.h
*
*    DESCRIPTION:     Class that holds a map of all object data used for
*					  later loading
************************************************************************/

#ifndef __object_data_list_3d_h__
#define __object_data_list_3d_h__

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/scoped_ptr.hpp>

// Forward declaration(s)
class CObjectData3D;
struct XMLNode;

class CObjectDataList3D
{
public:

    // Get the instance of the singleton class
    static CObjectDataList3D & Instance()
    {
        static CObjectDataList3D dataList;
        return dataList;
    }

    // Load the data list table
    void LoadListTable( const std::string & filePath );

    // Load all of the meshes and materials of a specific data group
    void LoadDataGroup( const std::string & group );

    // Free all of the meshes and materials of a specific data group
    void FreeDataGroup( const std::string & group );

    // Get a specific stage object's data
    CObjectData3D * GetData( const std::string & group, const std::string & name );

private:

    CObjectDataList3D();
    ~CObjectDataList3D();

    // Load the object information from the passed in node
    void LoadObjectFromNode( const XMLNode & objectNode, 
                             CObjectData3D * pObjectData );

    // Load all stage object information from an xml
    void LoadFromXML( const std::string & group, const std::string & filePath );

private:

    // Holds the data list table map
    std::map<const std::string, const std::string> dataListTableMap;
    std::map<const std::string, const std::string>::iterator dataListTableIter;

    // Map in a map of all the objects' data
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CObjectData3D>> spObjectDataMapMap;

    // Two iterators. One for the group map, and one for the data map inside of the group map
    boost::ptr_map<const std::string, boost::ptr_map<const std::string, CObjectData3D>>::iterator groupMapIter;
    boost::ptr_map<const std::string, CObjectData3D>::iterator dataMapIter;

};

#endif  // __object_data_list_h__
