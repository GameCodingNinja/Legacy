/************************************************************************
*    FILE NAME:       edge.cpp
*
*    DESCRIPTION:     Edge class
************************************************************************/

// Physical component dependency
#include <common/edge.h>

// Game lib dependencies
#include <2d/collisionsprite2d.h>
#include <utilities/collisionfunc2d.h>
#include <common/collisionvertex.h>
#include <misc/settings.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CEdge::CEdge()
     : unique(false)
{
}	// Constructor */


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CEdge::CEdge(CCollisionVertex * pVert0, CCollisionVertex * pVert1)
     : unique(false)
{
    pVert[0] = pVert0;
    pVert[1] = pVert1;

    CalculateNormal();

}	// Constructor */


/************************************************************************                                                             
*    desc:  Calculate the edge normal
************************************************************************/
void CEdge::CalculateNormal()
{
    // Calculate the vector of the edge
    //CPoint tmpVector = pVert[1]->GetVert() - pVert[0]->GetVert();
    CPoint tmpVector = ( pVert[1]->GetPos() - pVert[0]->GetPos() );

    // Rotate the vector counter-clockwise by 90 degrees to get the temporary normal
    CPoint tmpNormal;
    tmpNormal.x = -tmpVector.y;
    tmpNormal.y = tmpVector.x;

    // Get the length of the normal
    float length = sqrt((tmpNormal.x * tmpNormal.x) + (tmpNormal.y * tmpNormal.y));

    // Normalize the normal.
    normal.x = tmpNormal.x / length;
    normal.y = tmpNormal.y / length;

}	// GetNormal */

/************************************************************************                                                             
*    desc:  Get the edge center point
************************************************************************/
CWorldPoint CEdge::GetCenter() const
{
    // Center of the edge
    CWorldPoint center = ( pVert[0]->GetPos() + pVert[1]->GetPos() ) / 2;

    //center.f_pos.x = (pVert[0]->GetPos().f_pos.x + pVert[1]->GetPos().f_pos.x) * 0.5f;
    //center.f_pos.y = (pVert[0]->GetPos().f_pos.y + pVert[1]->GetPos().f_pos.y) * 0.5f;

    //int tmp = pVert[0]->GetPos().i_pos.x + pVert[1]->GetPos().i_pos.x;

    //if( tmp % 2 != 0 )
        //center.f_pos.x += float( CSettings::Instance().GetSectorHalfSize() );

    //tmp = pVert[0]->GetPos().i_pos.y + pVert[1]->GetPos().i_pos.y;

    //if( tmp % 2 != 0 )
        //center.f_pos.y += float( CSettings::Instance().GetSectorHalfSize() );

    //center.i_pos.x = (pVert[0]->GetPos().i_pos.x + pVert[1]->GetPos().i_pos.x) >> 1;
    //center.i_pos.y = (pVert[0]->GetPos().i_pos.y + pVert[1]->GetPos().i_pos.y) >> 1;

    //center.x = (pVert[0]->GetVert().x + pVert[1]->GetVert().x) * 0.5f;
    //center.y = (pVert[0]->GetVert().y + pVert[1]->GetVert().y) * 0.5f;
    //center.z = (pVert[0]->GetVert().z + pVert[1]->GetVert().z) * 0.5f;

    return center;
}