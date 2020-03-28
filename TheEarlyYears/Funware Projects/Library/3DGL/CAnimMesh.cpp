
/************************************************************************
*
*    PRODUCT:         Animated mesh class
*
*    FILE NAME:       CAnimMesh.cpp
*
*    DESCRIPTION:     Mesh container for animation frames
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "CAnimMesh.h"       // Header file for this *.cpp file.


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

CAnimMesh::CAnimMesh()
{
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

CAnimMesh::~CAnimMesh()
{

}   // Destructer


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

CPolygon * CAnimMesh::AllocatePolygonArray()
{
    pAnimPolyLst = new CAnimPolygon[ polyCount ];
    return pAnimPolyLst;

}   // AllocatePolygonArray


/************************************************************************
*    FUNCTION NAME:         LoadPolygonData()                                                             
*
*    DESCRIPTION:           Load the polygon data
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CAnimMesh::LoadPolygonData( FILE *hFile )
{
    // Cast to our animated poly class
    CAnimPolygon *pAnimPolyLst = (CAnimPolygon *)pList;

    // Load the polygon data
    for( int i = 0; i < polyCount; ++i )
    {
        if( !pAnimPolyLst[i].LoadPolygonData( hFile, vList ) )
            return false;     
    }

    return true;

}   // LoadPolygonData


/************************************************************************
*    FUNCTION NAME:         SavePolygonData()                                                             
*
*    DESCRIPTION:           Save the polygon data
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    12/13/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CAnimMesh::SavePolygonData( FILE *hFile )
{
    // Cast to our animated poly class
    CAnimPolygon *pAnimPolyLst = (CAnimPolygon *)pList;
    
    // Save the polygon data
    for( int i = 0; i < polyCount; ++i )
    {
        if( !pAnimPolyLst[i].SavePolygonData( hFile, vCount, vList ) )
            return false;         
    }

    return true;

}   // SavePolygonData


/************************************************************************
*    FUNCTION NAME:         Render()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer.
*
*    FUNCTION PARAMETERS:
*    Input:    Matrix3D &M - Matris to do all the translations
*              var2type Var2Name - Description
*
*    Output:   int - Number of polygons sent to be rendered
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    02/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CAnimMesh::RenderFrame( CMatrix &m )
{
    // Transform all the vertices
    m.Transform( vList, vCount );

    // Transform all the surface normals
    m.Transform( nList, polyCount );

    // Do we have textures?
    if( pTextLib != NULL )
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pAnimPolyLst[i].IsVisable() )
                {
                    // Render
                    pAnimPolyLst[i].RenderText( pTextLib );
                }
            }
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pAnimPolyLst[i].IsVisable() )
                {
                    // Calculate a new intensity
                    pAnimPolyLst[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pAnimPolyLst[i].RenderText( pTextLib );
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pAnimPolyLst[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pAnimPolyLst[i].GetUVRGB() );
                    
                    // Calculate a new intensity
                    pAnimPolyLst[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pAnimPolyLst[i].RenderText( pTextLib );
                }
            }
        }
    }
    else   // Render with no textures
    {
        if( pLightList == NULL || pLightList->GetDynamicLightType() == EDL_NO_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pAnimPolyLst[i].IsVisable() )
                {
                    // Render
                    pAnimPolyLst[i].Render();
                }
            }
        }                   
        else if( pLightList->GetDynamicLightType() == EDL_LOCAL_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pAnimPolyLst[i].IsVisable() )
                {
                    // Calculate a new intensity
                    pAnimPolyLst[i].CalcLightIntenStatic( pLightList );

                    // Render
                    pAnimPolyLst[i].Render();
                }
            }       
        }                    
        else if( pLightList->GetDynamicLightType() == EDL_WORLD_DYNAMIC_LIGHTNING )
        {
            // Display selected polygons in the off-screen buffer
            for( int i = 0; i < polyCount; ++i )
            {
                // Eliminate polygons that are not seen or have their backs to us
                if( pAnimPolyLst[i].IsVisable() )
                {
                    // Transform the vertex normals for lighting calculations
                    m.Transform( pAnimPolyLst[i].GetUVRGB() );
                    
                    // Calculate a new intensity
                    pAnimPolyLst[i].CalcLightIntenDynamic( pLightList );

                    // Render
                    pAnimPolyLst[i].Render();
                }
            }
        }
    }

}   // Render