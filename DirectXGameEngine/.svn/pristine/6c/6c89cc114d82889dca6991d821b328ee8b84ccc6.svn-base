
/************************************************************************
*    FILE NAME:       shadowsprite.h
*
*    DESCRIPTION:     3D shadow sprite class
************************************************************************/

#ifndef __shadow_sprite_h__
#define __shadow_sprite_h__

// Standard lib dependencies
#include <vector>

// Physical component dependency
#include "sprite.h"
#include "edge.h"
#include "lightlst.h"
#include "xvertexshadowbuffer.h"

// Forward declaration(s)
class CFace;


class CShadowSprite : public CSprite
{
public:

    CShadowSprite();
    virtual ~CShadowSprite();

    // Init the sprite with the path to the mesh file
    virtual bool Init( std::string & meshPath );

    // Build the mesh
    void BuildMesh( CLightLst & lightLst );

    // Build the mesh but do a cull check first
    void BuildMesh( CMatrix & matrix, CLightLst & lightLst );

    // Construct the shadow and render it
    void Render( CMatrix & matrix );

    // Set the projected distance
    void SetProjectDist( float projDist );

    // Free the allocated arrays
    void FreeArrays();

protected:

    // Build the shadow mesh
    void Build( CPoint & point, unsigned int shadowIndex );

    // Add edges from this face
    void AddFaceEdge( CFace & face );

    // Add the face for building the caps
    void AddFaceToCap( CFace & face );

    // Create the DirectX Mesh
    bool CreateDirectvisualmesh( CPoint & point, unsigned int shadowIndex );

    // Free the directX shadow mesh
    void FreeMeshX();

    // Allocate the arrays
    void AllocateArrays();

    // Calculas the back cap radius
    float CalcBackCapRadius( CPoint & point, CPoint & backCapCenter );

    // Do the pre-build cull check
    bool PreBuildCullCheck( CMatrix & matrix, CPoint & point );

    // Translated mesh data
    CFace * pTransFace;

    // Edge array
    CEdge * pEdge;

    // Shadow cap
    CXShadowFace * pCap;

    // edge counter
    unsigned int edgeCounter;

    // Cap counter
    unsigned int capCounter;

    // Vector holding directX vertex buffer
    std::vector<CXVertShadowBuff *> shadowMeshX;

    // Shadow projection distance
    float projectDist;

};

#endif  // __shadow_sprite_h__
