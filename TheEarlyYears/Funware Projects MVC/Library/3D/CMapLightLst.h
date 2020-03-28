/************************************************************************
*
*    FILE NAME:       CMapLightLst.h
*
*    DESCRIPTION:     Class for holding map lights
*
************************************************************************/

#ifndef _CMAPLIGHTLST_H_
#define _CMAPLIGHTLST_H_

#include "CLight.h"

class CMapLightList : public CLightList
{
public:

	int floorID;
	EOT_OBJECT_TYPE objectType;
	int mapID;

	CMapLightList()
	{
		floorID = 0;
		objectType = EOT_NO_TYPE;
		mapID = 0;
	};
};

#endif