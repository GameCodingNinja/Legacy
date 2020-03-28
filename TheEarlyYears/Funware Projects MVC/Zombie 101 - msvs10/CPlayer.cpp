/************************************************************************
*
*    FILE NAME:       CPlayer.cpp
*
*    DESCRIPTION:     Player class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#include "CPlayer.h"

#include "tagfileparser/datadefnode.h"
#include "tagfileparser/tokenizer.h"
#include "tagfileparser/tagfileparser.h"

#include <string>
#include <fstream>

/************************************************************************
*    FUNCTION NAME: Constructer                                                             
************************************************************************/
CPlayer::CPlayer() : playerNode(NULL)
{
    // Set the starting direction
    dir.z = -20.0f;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
CPlayer::~CPlayer()
{
}   // Destructer 


/************************************************************************
*    DESCRIPTION:  Save the rotation in the opposite direction so that
*                  the rotation can be reversed in the view matrix to
*                  get a usuble point
*
*    Input:    float x - X rotation
*              float y - Y rotation
*              float z - Z rotation
************************************************************************/  
void CPlayer::SetOppositeRotate( float x, float y, float z )
{
    oppositeRotMatrix.Rotate( x, y, z );

}   // SetOppositeRotate


/************************************************************************
*    DESCRIPTION:  Update player object
*
*    Input:  Matrix3D &m - Matrix to update the pos and dir
*            CMapMgr<CMeshMap> &map - map class
************************************************************************/  
void CPlayer::Update( CMatrix &m, CMapMgr<CMeshMap> &map )
{
    // Update the player's pos and direction
    UpdatePosDir( m );

    // Find the path node closest to the player in the area they are standing
    FindClosestPathNode( map );

}   // Update


/************************************************************************
*    DESCRIPTION:  Update the position and the direction
*
*    Input:  Matrix3D &m - Matrix to update the pos and dir
************************************************************************/  
void CPlayer::UpdatePosDir( CMatrix &m )
{
    CMatrix tmpMatrix;
    
    // Setup the martix
    tmpMatrix.MergeMatrix( m.matrix );
    tmpMatrix.MergeMatrix( oppositeRotMatrix.matrix );

    tmpMatrix.Transform( dir );
    tmpMatrix.Transform( pos );

    // Need to reverse the sign
    pos.tx = -pos.tx;
    pos.ty = -pos.ty;
    pos.tz = -pos.tz;

    dir.tx = -dir.tx;
    dir.ty = -dir.ty;
    dir.tz = -dir.tz;

}   // UpdatePosDir


/************************************************************************
*    DESCRIPTION:  Update the position and the direction
*                  We're not using sqare root (sqrt) for speed.
*
*    Input:  CMapMgr<CMeshMap> &map - map class
************************************************************************/
void CPlayer::FindClosestPathNode( CMapMgr<CMeshMap> &map )
{
    std::deque<CPathNode *> &nodeLst = map.GetNodeLst( mapType );
    
    if( nodeLst.size() > 0 )
    {
        // Get the room radius to use as the largest distance to check against.
        float currentDist( map.GetMesh( mapType )->GetRadius() *
                           map.GetMesh( mapType )->GetRadius() );
        float dist;
        CPathNode * currentNode(NULL);

        // search for the closest node.
        for( std::deque<CPathNode *>::iterator it = nodeLst.begin(); it != nodeLst.end(); ++it )
        {
            // get the distance
            dist = (*it)->GetPos().GetLengthDy( pos );
            
            // Is this distance less then the last distance?
            // Record the player node
            if( dist <= currentDist )
            {
                currentDist = dist;
                playerNode = (*it);
            }   
        }
    }       
}   // FindClosestPathNode


/************************************************************************
*    DESCRIPTION:  Load players spawn point
************************************************************************/  
bool CPlayer::LoadSpawnPoint( const std::string & tagFilename,
                              CMatrix &matrix,
                              CMapMgr<CMeshMap> &map )
{
    TFloatVector trans;
    TFloatVector rot;
	TFloatVector mapScale;
    std::string objType;
	int floorId;
    int mapId;
    SObjectType objStrType;
    CPoint transPoint, rotPoint, mapScalePoint;
	CMatrix tmpMatrix;
	TDataDefNodeVector data;

    // Parse the config file.
	CDataDefNode * pTree = CTagFileParser::GetInstance().ParseFile( tagFilename );
    if( pTree == NULL )
		return false;
    
    // Retrieve all of the spawn data.    
    if( !pTree->GetDataByName("spawnList", data) )
		return false;

    if( data.size() == 0 )
		return false;

    // Get the spawn data
	data[0]->GetDataByName("floorId", floorId);
    data[0]->GetDataByName("objectType", objType);
    data[0]->GetDataByName("mapId", mapId);
    data[0]->GetDataByName("trans", trans);
    data[0]->GetDataByName("rot", rot);

    CDataDefNode * pChild = pTree->GetChild("map");
    if( pChild == NULL )
		return false;

    pChild->GetDataByName("moveSpeed", moveSpeed);
	pChild->GetDataByName("strafeSpeed", strafeSpeed);
	pChild->GetDataByName("radious", radius);
	pChild->GetDataByName("gravity", gravity);
	pChild->GetDataByName("distFromFloor", distFromFloor);
	pChild->GetDataByName("distFromWall", distFromWall);
	pChild->GetDataByName("mapScale", mapScale);

    Delete(pTree);

    transPoint.Init( trans );
	rotPoint.Init( rot );
	mapScalePoint.Init( mapScale );

    tmpMatrix.Scale( mapScalePoint );
	tmpMatrix.Transform( transPoint );
	transPoint.MakePermanent();

    // find the type
	mapType.activeType = objStrType.GetType( objType );

    return map.SetSpawnPoint( mapType, matrix, transPoint, rotPoint, floorId, mapId, 1 );

}   // LoadSpawnPoint