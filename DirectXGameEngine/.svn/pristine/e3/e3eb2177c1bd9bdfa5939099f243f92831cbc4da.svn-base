/************************************************************************
*    FILE NAME:       shadowsprite.cpp
*
*    DESCRIPTION:     3D shadow sprite class
************************************************************************/

// Physical component dependency
#include "shadowsprite.h"

// Game OS dependencies
#include <assert.h>

// Game lib dependencies
#include "meshmanager.h"
#include "xwindow.h"
#include "mesh.h"
#include "statcounter.h"
#include "face.h"
#include "genfunc.h"
#include "deletefuncs.h"

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CShadowSprite::CShadowSprite()
             : CSprite(),
               pTransFace(NULL),
               pEdge(NULL),
               pCap(NULL),
               edgeCounter(0),
               capCounter(0),
               projectDist(100.0f)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CShadowSprite::~CShadowSprite()
{
    FreeArrays();

    FreeMeshX();

}   // Destructer


/************************************************************************
*    desc:  Free the directX shadow mesh
************************************************************************/
void CShadowSprite::FreeMeshX()
{
    DeleteVectorPointers( shadowMeshX );

}	// FreeMeshX


/************************************************************************
*    desc:  Free the allocated arrays
************************************************************************/
void CShadowSprite::FreeArrays()
{
    DeleteArray( pTransFace );
    DeleteArray( pEdge );
    DeleteArray( pCap );
}


/************************************************************************
*    desc:  Init the sprite with the path to the mesh file
*  
*    param: std::string & meshPath - path to mesh xml
*    
*    return: bool - true on success or false on fail
************************************************************************/
bool CShadowSprite::Init( string & meshPath )
{
    pMesh = CMeshMgr::Instance().GetShadowMesh( meshPath );

    if( NULL == pMesh )
    {
        return false;
    }

    // Allocate the arrays
    AllocateArrays();

    return true;

}   // Init


/************************************************************************
*    desc:  Allocate the arrays
************************************************************************/
void CShadowSprite::AllocateArrays()
{
    // Allocate our arrays
    pTransFace = new CFace[pMesh->GetFaceCount()];
    pEdge = new CEdge[pMesh->GetFaceCount()];
    pCap = new CXShadowFace[pMesh->GetFaceCount()];
}


/************************************************************************
*    desc:  Build the mesh
*
*    NOTE: The shadow volume is build in world view
*
*    param: CLightLst & lightLst - light list class
************************************************************************/
void CShadowSprite::BuildMesh( CLightLst & lightLst )
{
    // Transform the center point
    Transform();

    // Do our rotations in a temporary matrix
    CMatrix scaleMatrix;

    // Handle the scaling
    scaleMatrix.Scale( scale );

    // Merge in the noScaleMatrix matrix to convert to world view
    scaleMatrix.MergeMatrix( noScaleMatrix );

    // Transform the mesh
    // NOTE: Since we are tranforming our mesh, you can't scale
    // normals. So for this, we have a seperate matix without scale.
    pMesh->Transform( pTransFace, scaleMatrix, noScaleMatrix );

    unsigned int shadowIndex = 0;

    // Builds the shadow mesh
    for( unsigned int i = 0; i < lightLst.GetCount(); i++ )
    {
        if( lightLst.GetLight(i)->castShadow && lightLst.GetLight(i)->enable )
        {
            Build( lightLst.GetLight(i)->GetPos(), shadowIndex++ );
        }
    }

}	// BuildMesh


/************************************************************************
*    desc:  Build the mesh but do a cull check first
*
*    NOTE: The shadow volume is build in world view
*
*    param: CMatric & matrix - passed in matrix class
*    param: CLightLst & lightLst - light list class
************************************************************************/
void CShadowSprite::BuildMesh( CMatrix & matrix, CLightLst & lightLst )
{
    // Transform the center point
    Transform();

    unsigned int shadowIndex = 0;

    // Builds the shadow mesh
    for( unsigned int i = 0; i < lightLst.GetCount(); i++ )
    {
        if( lightLst.GetLight(i)->castShadow && lightLst.GetLight(i)->enable )
        {
            if( !PreBuildCullCheck( matrix, lightLst.GetLight(i)->GetPos() ) )
            {
                // Do this only once for building all the chadows
                if( shadowIndex == 0 )
                {
                    // Do our rotations in a temporary matrix
                    CMatrix scaleMatrix;

                    // Handle the scaling
                    scaleMatrix.Scale( scale );

                    // Merge in the noScaleMatrix matrix to convert to world view
                    scaleMatrix.MergeMatrix( noScaleMatrix );

                    // Transform the mesh
                    // NOTE: Since we are tranforming our mesh, you can't scale
                    // normals. So for this, we have a seperate matix without scale.
                    pMesh->Transform( pTransFace, scaleMatrix, noScaleMatrix );
                }

                Build( lightLst.GetLight(i)->GetPos(), shadowIndex++ );
            }
        }
    }

}	// BuildMesh


/************************************************************************
*    desc:  Calculas the back cap radius
*
*    NOTE: The shadow volume is build in world view
*
*	 param: CPoint & point - point of the light
*	 param: CPoint & backCapCenter - center point of the back cap
************************************************************************/
float CShadowSprite::CalcBackCapRadius( CPoint & lightPoint, CPoint & backCapCenter )
{
    float theta = atan( GetRadiusSqrt() / sqrt( lightPoint.GetLength(trans_center) ) );

    return sqrt( lightPoint.GetLength(backCapCenter) ) * tan(theta);

}	// CalcBackCapRadius


/************************************************************************
*    desc:  Do the pre-build cull check
*
*    NOTE: This function assumes trans_center to be in world view
*
*	 param: CPoint & point - point of the light
************************************************************************/
bool CShadowSprite::PreBuildCullCheck( CMatrix & matrix, CPoint & point )
{
    // The trans_center is in world view so we are converting it to camera view
    CPoint frontCapCenter;
    matrix.Transform( frontCapCenter, trans_center );

    // See if we can cull the front center radius of the shadow volume
    int frontCullValue = CullMesh_BoundSphere( frontCapCenter, GetRadiusSqrt() );
    if( frontCullValue == 0 )
    {
        frontCullValue = -1;
    }

    // Project the center point
    CPoint tmpBackCapCenter = trans_center.GetDisplacement( point, projectDist );
    CPoint backCapCenter;
    matrix.Transform( backCapCenter, tmpBackCapCenter );

    int backCullValue = CullMesh_BoundSphere( backCapCenter, CalcBackCapRadius( point, tmpBackCapCenter ) );

    return (frontCullValue == backCullValue);

}	// PreBuildCullCheck


/************************************************************************
*    desc:  Build the shadow mesh
*
*    NOTE: The shadow volume is build in world view
*
*	 param: CPoint & point - point of the light
************************************************************************/
void CShadowSprite::Build( CPoint & point, unsigned int shadowIndex )
{
    edgeCounter = 0;
    capCounter = 0;

    for( unsigned int face = 0; face < pMesh->GetFaceCount(); face++ )
    {
        // Check if the light point is facing plane of the face
        if( pTransFace[face].IsFacingPlane( point ) )
        {
            // Try to add any of these face edges to the list
            AddFaceEdge( pTransFace[face] );
        }
        // Add the face for building the caps
        else if( !CXWindow::Instance().IsUsingZPassShadowMethod() )
        {
            AddFaceToCap( pTransFace[face] );
        }
    }

    // Project the edges to create the directX mesh
    CreateDirectvisualmesh( point, shadowIndex );

    // Inc our stat counter to keep track of what is going on.
    CStatCounter::Instance().IncShadowBuildCounter();

}	// Build


/************************************************************************
*    desc:  Add edges from this face
*
*    NOTE: The shadow volume is build in world view
*
*	 param: CFace & face - reference to face to see if we can add any of
*			               these edges to the list
************************************************************************/
void CShadowSprite::AddFaceEdge( CFace & face )
{
    for( int i = 0; i < 3; i++ )
    {
        CEdge tmpEdge;
        bool found = false;

        // Get the edge from the face
        face.GetEdge( i, tmpEdge );

        // go through all the edges until we find a match
        for( unsigned int j = 0; j < edgeCounter; j++ )
        {
            // If we found an edge, remove it from the vector edge list
            if( pEdge[j] == tmpEdge )
            {
                found = true;

                // copy the last edge into the deleted edge's spot
                edgeCounter--;
                pEdge[j] = pEdge[edgeCounter];

                break;
            }
        }

        // If we didn't find this edge, add it in.
        if( !found )
        {
            assert(edgeCounter < pMesh->GetFaceCount());
            pEdge[edgeCounter] = tmpEdge;
            edgeCounter++;
        }
    }

}	// AddFaceEdge


/************************************************************************
*    desc:  Add the face for building the caps
*
*    NOTE: The shadow volume is build in world view
*
*	 param: CFace & face - reference to face to see if we can add any of
*			               these edges to the list
************************************************************************/
void CShadowSprite::AddFaceToCap( CFace & face )
{
    assert(capCounter < pMesh->GetFaceCount() );

    // Copy over face
    pCap[capCounter].vert[0] = face.vert[0];
    pCap[capCounter].vert[1] = face.vert[1];
    pCap[capCounter].vert[2] = face.vert[2];

    capCounter++;

}	// AddFaceToCap


/************************************************************************
*    desc:  Create the DirectX Mesh
*
*    NOTE: The shadow volume is build in world view
*
*	 param: CPoint & point - light position
************************************************************************/
bool CShadowSprite::CreateDirectvisualmesh( CPoint & point, unsigned int shadowIndex )
{
    CXVertShadowBuff * pShadowBuf;

    if( shadowIndex == shadowMeshX.size() )
    {
        pShadowBuf = new CXVertShadowBuff;
    
        // create the vertex buffer We are assuming 2 * the number of faces will be enough
        if( D3D_OK != CXWindow::Instance().GetXDevice()->CreateVertexBuffer( pMesh->GetFaceCount() * sizeof(CXShadowFace) * 2,
                                                                             D3DUSAGE_WRITEONLY,
                                                                             D3DFVF_XYZ,
                                                                             D3DPOOL_MANAGED,
                                                                             &pShadowBuf->xVertBuf,
                                                                             NULL ) )
        {
            PostMsg( "Init Error", 
                     "Error creating vertex shadow buffer. Your system resources may be low so try restarting your computer." );

            return false;
        }

        // Add to our vector
        shadowMeshX.push_back( pShadowBuf );
    }
    else
    {
        pShadowBuf = shadowMeshX[shadowIndex];
    }

    // Each edge represents a quad
    pShadowBuf->fcount = (edgeCounter * 2) + (capCounter * 2);

    // Do a sanity check that we don't overrun our buffer
    assert(pShadowBuf->fcount <= pMesh->GetFaceCount() * 2);

    // Make a face pointer
    CXShadowFace * pFace;

    // Lock the vertex buffer for copying
    pShadowBuf->xVertBuf->Lock( 0, 0, (void **)&pFace, 0 );

    // Use each edge to build two triangles
    for( unsigned int i = 0; i < edgeCounter; i++ )
    {
        CPoint v1 = pEdge[i].vert[0];
        CPoint v2 = pEdge[i].vert[1];

        CPoint v3 = v1.GetDisplacement( point, projectDist );
        CPoint v4 = v2.GetDisplacement( point, projectDist );

        // First triangle
        pFace[i*2+0].vert[0] = v3;
        pFace[i*2+0].vert[1] = v2;
        pFace[i*2+0].vert[2] = v1;

        // second triangle
        pFace[i*2+1].vert[0] = v3;
        pFace[i*2+1].vert[1] = v4;
        pFace[i*2+1].vert[2] = v2;
    }

    int faceIndex = edgeCounter * 2;
    for( unsigned int i = 0; i < capCounter; i++ )
    {
        // Add faces for the front cap
        pFace[faceIndex].vert[0] = pCap[i].vert[2];
        pFace[faceIndex].vert[1] = pCap[i].vert[1];
        pFace[faceIndex].vert[2] = pCap[i].vert[0];
        faceIndex++;

        // Add faces for back cap
        pFace[faceIndex].vert[0] = pCap[i].vert[0].GetDisplacement( point, projectDist );
        pFace[faceIndex].vert[1] = pCap[i].vert[1].GetDisplacement( point, projectDist );
        pFace[faceIndex].vert[2] = pCap[i].vert[2].GetDisplacement( point, projectDist );
        faceIndex++;
    }

    // Unlock the buffer so it can be used
    pShadowBuf->xVertBuf->Unlock();

    // Project the center point
    pShadowBuf->center = trans_center.GetDisplacement( point, projectDist );

    // compute the radius
    pShadowBuf->backCapRadius = CalcBackCapRadius( point, pShadowBuf->center );

    return true;

}	// CreateDirectvisualmesh


/************************************************************************
*    desc:  Construct the shadow and render it
*
*    NOTE: The shadow volume was created in world view
*
*    param: CMatric & matrix - passed in matrix class
************************************************************************/
void CShadowSprite::Render( CMatrix & matrix )
{
    bool matrixSet = false;

    // Transform the center point that's in world view - needed for CullMesh_BoundSphere()
    CPoint frontCapCenter;
    matrix.Transform( frontCapCenter, trans_center );

    // See if we can cull the front center radius of the shadow volume
    int frontCullValue = CullMesh_BoundSphere( frontCapCenter, GetRadiusSqrt() );
    if( frontCullValue == 0 )
    {
        frontCullValue = -1;
    }

    // Render the mesh
    for( unsigned int i = 0; i < shadowMeshX.size(); i++ )
    {
        CPoint backCapCenter;
        matrix.Transform( backCapCenter, shadowMeshX[i]->center );

        int backCullValue = CullMesh_BoundSphere( backCapCenter, shadowMeshX[i]->backCapRadius );

        // Test if is shadow is within the view frustum
        if( frontCullValue != backCullValue )
        {
            // set the matrix only once
            if( !matrixSet )
            {
                matrixSet = true;

                // Copy it to the DirectX matrix
                D3DXMATRIX xMatrix( matrix.GetMatrix() );

                // Set the world view transformation to DirectX
                CXWindow::Instance().GetXDevice()->SetTransform( D3DTS_VIEW, &xMatrix ); // D3DTS_WORLD D3DTS_VIEW
            }

            // Inc our stat counter to keep track of what is going on.
            CStatCounter::Instance().IncShadowCounter();

            // Use the two sided stencil for shadows
            if( CXWindow::Instance().IsTwoSidedStencil() )
            {
                CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
                CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, true );
            }
            else // Do traiditional two pass shadow stencil
            {
                // render back faces
                CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

                if( CXWindow::Instance().IsUsingZPassShadowMethod() )
                {
                    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_INCR );
                }
                else
                {
                    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR );
                }
                
                // Render the shadow
                CXWindow::Instance().GetXDevice()->SetStreamSource( 0, shadowMeshX[i]->xVertBuf, 0, sizeof(CPoint) );
                CXWindow::Instance().GetXDevice()->SetFVF( D3DFVF_XYZ );
                CXWindow::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, shadowMeshX[i]->fcount );

                // render front faces
                CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

                if( CXWindow::Instance().IsUsingZPassShadowMethod() )
                {
                    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );
                }
                else
                {
                    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR );
                }
            }

            // Render the shadow
            CXWindow::Instance().GetXDevice()->SetStreamSource( 0, shadowMeshX[i]->xVertBuf, 0, sizeof(CPoint) );
            CXWindow::Instance().GetXDevice()->SetFVF( D3DFVF_XYZ );
            CXWindow::Instance().GetXDevice()->DrawPrimitive( D3DPT_TRIANGLELIST, 0, shadowMeshX[i]->fcount );
        }
    }

    // Turn off two sided stencil mode if enambled
    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, false );

    // Reset the cull mode
    CXWindow::Instance().GetXDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

}	// Render


/************************************************************************
*    desc:  Set the projected distance
*
*    param: float projDist - value of the new projected distance
************************************************************************/
void CShadowSprite::SetProjectDist( float projDist )
{
    projectDist = projDist;

}	// SetProjectDist
