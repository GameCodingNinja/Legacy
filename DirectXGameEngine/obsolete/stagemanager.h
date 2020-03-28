/************************************************************************
*    FILE NAME:       stagemanager.h
*
*    DESCRIPTION:     Class that manages stage objects
************************************************************************/

#ifndef __stage_manager_h__
#define __stage_manager_h__

// Game lib dependencies
#include "newstage.h"

// Standard lib dependencies
#include <string>

class CStageManager
{
public:

    CStageManager();
    ~CStageManager();

    // Load the stage object list
    void LoadStage( std::string & filePath );

protected:

    CNewStage stage;

};

#endif  // __stage_manager_h__