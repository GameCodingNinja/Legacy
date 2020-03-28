
/************************************************************************
*
*    PRODUCT:         Mesh interior map class
*
*    FILE NAME:       CMeshRDMap.cpp
*
*    DESCRIPTION:     Mesh container for maps based on rooms and doors
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CMeshRDMap.h"      // Header file for this *.cpp file.
#include "CPolygonRDMap.h"   // Polygon class with map properties


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04             Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMeshRDMap::CMeshRDMap()
{
    // init class members
	memset( roomToDoor, 0, sizeof(roomToDoor) );

}   // Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CMeshRDMap::~CMeshRDMap()
{

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         LoadExtraMeshData()                                                             
*
*    DESCRIPTION:           Virtual function for loading inherited class
*                           specific data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshRDMap::LoadExtraMeshData( FILE *hFile )
{
    // read the room indexes
    if( fread( roomToDoor, sizeof( roomToDoor ), 1, hFile ) != 1 )
        return false;

    return true;

}   // LoadExtraMeshData


/************************************************************************
*    FUNCTION NAME:         SaveExtraMeshData()                                                             
*
*    DESCRIPTION:           Virtual function for savinging inherited class
*                           specific data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CMeshRDMap::SaveExtraMeshData( FILE *hFile )
{
    // Write out the room indexes
    if( fwrite( roomToDoor, sizeof( roomToDoor ), 1, hFile ) != 1 )
        return false;

    return true;

}   // SaveExtraMeshData


/************************************************************************
*    FUNCTION NAME:         AllocatePolygonArray()                                                             
*
*    DESCRIPTION:           Allocate the polygon array. Virtual call so that
*                           an inherited class can load up it's own polygon
*                           class.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CPolygon * CMeshRDMap::AllocatePolygonArray()
{
    return new CPolygonRDMap[ polyCount ];

}   // AllocatePolygonArray