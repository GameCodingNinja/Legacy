/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CBone.cpp
*
*    DESCRIPTION:     Bone animation class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "CBone.h"           // Header file for this *.cpp file.


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CBone::CBone()
{
	boneIndex = 0;
	boneParent = -1;
	transRotListCount = 0;
	pTransList = NULL;
	pRotList = NULL;

}	// Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CBone::~CBone()
{
	Free();

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         AllocateLists()                                                             
*
*    DESCRIPTION:           Allocate the bone animation lists
*
*    FUNCTION PARAMETERS:
*    Input:    int count - Number of lists
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/044      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CBone::AllocateLists( int count )
{
	// Free all allocations
	Free();

	pTransList = new CBoneOffsets[ count ];

	if( pTransList == NULL )
		return false;

	pRotList  = new CBoneOffsets[ count ];

	if( pTransList == NULL )
		return false;

	transRotListCount = count;

	return true;

}	// AllocateLists


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free the allocations
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/044      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CBone::Free()
{
	boneIndex = 0;
	boneParent = -1;
	transRotListCount = 0;

	if( pTransList != NULL )
	{
		delete [] pTransList;
		pTransList = NULL;
	}

	if( pRotList != NULL )
	{
		delete [] pRotList;
		pRotList = NULL;
	}

}   // Free





/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CBoneList::CBoneList()
{
	boneCount = 0;
	pBone = NULL;

}	// Constructer


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/04       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CBoneList::~CBoneList()
{
	Free();

}   // Destructer


/************************************************************************
*    FUNCTION NAME:         AllocateBoneList()                                                             
*
*    DESCRIPTION:           Allocate the bone lists
*
*    FUNCTION PARAMETERS:
*    Input:    int count - Number of lists
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/044      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CBoneList::AllocateBoneList( int count )
{
	// Free all allocations
	Free();

	pBone = new CBone[ count ];

	if( pBone == NULL )
		return false;

	boneCount = count;

	return true;

}	// AllocateBoneList


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free the allocations
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    10/02/044      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CBoneList::Free()
{
	boneCount = 0;

	if( pBone != NULL )
	{
		delete [] pBone;
		pBone = NULL;
	}

}   // Free