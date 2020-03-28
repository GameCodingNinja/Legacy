/************************************************************************
*    FILE NAME:       stagedatalist.h
*
*    DESCRIPTION:     Class that holds a map of stage data used for
*					  later loading
************************************************************************/

#ifndef __stage_data_list_h__
#define __stage_data_list_h__

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Forward declaration(s)
class CStageData;

class CStageDataList
{
public:

    // Get the instance of the singleton class
    static CStageDataList & Instance()
    {
        static CStageDataList dataList;
        return dataList;
    }

    // Load all stage object information from an xml
    void LoadFromXML( const std::string & filePath );

    // Get a specific stage object's data
    CStageData * GetData( const std::string & name );

private:

    CStageDataList();
    ~CStageDataList();

    // Map of all the stages' data
    boost::ptr_map<std::string, CStageData> spStageMap;
    boost::ptr_map<std::string, CStageData>::iterator stageMapIter;

};

#endif  // __stage_data_list_h__