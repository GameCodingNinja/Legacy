
/************************************************************************
*    FILE NAME:       basesavefile.h
*
*    DESCRIPTION:     Class for saving game saves
************************************************************************/

#ifndef __base_save_file_h__
#define __base_save_file_h__

// Standard lib dependencies
#include <vector>
#include <map>
#include <string>

// Boost lib dependencies
#include "boost/date_time/posix_time/posix_time.hpp"

// Game lib dependencies
#include <common/defs.h>

class CBaseSaveFile
{
public:

    // Constructor
    CBaseSaveFile();

    // Get all the save file names
    void GetSaveFileNames();

    // Get the file name vector
    const std::multimap<std::time_t, std::string> & GetFileNameMap() const;

    // Get the file save count
    size_t GetfileSaveCount() const
    { return fileNamesMap.size(); }

    // Get/Set file save string
    const std::string & GetFileName() const;
    void SetFileName( const std::string & fileName );

    // Get/Set file extension
    const std::string & GetFileExtension() const;
    void SetFileExtension( const std::string & fileExt );

    // Get the save file path
    const std::string & GetSaveFilePath() const;

protected:

    // Start/Stop the game time
    void StartGameTime();
    void StopGameTime();

    // Inc the duration
    void IncDuration( const boost::posix_time::time_duration & duration );

    // Set the duration
    void SetDuration( const boost::posix_time::time_duration & duration );

    // Get the duration
    const boost::posix_time::time_duration & GetDuration() const;

private:

    // Game save file name
    std::string fileNameStr;

    // Start time
    boost::posix_time::ptime startTime;

    // The accumulation of time
    boost::posix_time::time_duration timeDuration;

    // File name map
    std::multimap<std::time_t, std::string> fileNamesMap;

    // FIle extension
    std::string fileExtension;

    // save file path
    std::string saveFilePath;

};

#endif  // __base_save_file_h__


