/************************************************************************
*    FILE NAME:       stageobjectdata.h
*
*    DESCRIPTION:     Class that holds a stage object's data for later
*					  loading
************************************************************************/

#ifndef __stage_object_data_h__
#define __stage_object_data_h__

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <common/point.h>

class CStageObjectData
{
public:

	CStageObjectData():name(""),scale(0)
	{};

	// Name of the stage object
	std::string name;

	// Position of the stage object
	CPoint position;

	// Rotation of the stage object
	CPoint rotation;

	// Scale of the stage object
	float scale;

};

#endif  // __stage_object_data_h__
