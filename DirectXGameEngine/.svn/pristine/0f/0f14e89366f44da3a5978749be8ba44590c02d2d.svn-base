
/************************************************************************
*    FILE NAME:       basesavefile.cpp
*
*    DESCRIPTION:     Class for saving game saves
************************************************************************/

// Physical component dependency
#include <managers/basesavefile.h>

// Standard lib dependencies
#include <fstream>

// Boost lib dependencies
#include <boost/archive/xml_oarchive.hpp> // saving
#include <boost/archive/xml_iarchive.hpp> // loading
#include <boost/filesystem.hpp>

// Game lib dependencies
#include <misc/settings.h>

// Game dependencies

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CBaseSaveFile::CBaseSaveFile()
{
}   // constructor


/************************************************************************
*    desc:  Get all the save file names
************************************************************************/
void CBaseSaveFile::GetSaveFileNames()
{
    fileNamesMap.clear();

    // "." causes the search directory to be the executable directory
    saveFilePath = "./";

    // If the path is not empty, use the documents folder path
    if( !CSettings::Instance().GetFolderPath().empty() )
        saveFilePath = CSettings::Instance().GetFolderPath();

    // Set the path
    boost::filesystem::path directory( saveFilePath );

    // Setup the iterator for searching the directory
    boost::filesystem::directory_iterator iter(directory), end;
    
    // Get all the game save files
    for( ; iter != end; ++iter )
        if( iter->path().extension() == fileExtension )
            fileNamesMap.insert( make_pair(boost::filesystem::last_write_time(saveFilePath + iter->path().filename().string()), iter->path().filename().string()) );

}	// GetSaveFileNames


/************************************************************************
*    desc:  Start the game time counter
************************************************************************/
void CBaseSaveFile::StartGameTime()
{
    startTime = boost::posix_time::second_clock::local_time();

}	// StartGameTime


/************************************************************************
*    desc:  Stop the game time counter
************************************************************************/
void CBaseSaveFile::StopGameTime()
{
    if( !startTime.is_not_a_date_time() )
        timeDuration += boost::posix_time::second_clock::local_time() - startTime;

}	// StartGameTime


/************************************************************************
*    desc:  Get the file name vector
************************************************************************/
const multimap<std::time_t, std::string> & CBaseSaveFile::GetFileNameMap() const
{
    return fileNamesMap;

}	// GetFileNameVec


/************************************************************************
*    desc:  Inc the duration
************************************************************************/
void CBaseSaveFile::IncDuration( const boost::posix_time::time_duration & duration )
{
    timeDuration += duration;

}	// IncDuration


/************************************************************************
*    desc:  Set the duration
************************************************************************/
void CBaseSaveFile::SetDuration( const boost::posix_time::time_duration & duration )
{
    timeDuration = duration;

}	// IncDuration


/************************************************************************
*    desc:  Get the duration
************************************************************************/
const boost::posix_time::time_duration & CBaseSaveFile::GetDuration() const
{
    return timeDuration;

}	// GetDuration


/************************************************************************
*    desc:  Get/Set file save string
************************************************************************/
const std::string & CBaseSaveFile::GetFileName() const
{
    return fileNameStr;
}

void CBaseSaveFile::SetFileName( const std::string & fileName )
{
    fileNameStr = fileName;
}


/************************************************************************
*    desc:  Get/Set file extension
************************************************************************/
const std::string & CBaseSaveFile::GetFileExtension() const
{
    return fileExtension;
}

void CBaseSaveFile::SetFileExtension( const std::string & fileExt )
{
    fileExtension = fileExt;
}


/************************************************************************
*    desc:  Get the save file path
************************************************************************/
const std::string & CBaseSaveFile::GetSaveFilePath() const
{
    return saveFilePath;
}







