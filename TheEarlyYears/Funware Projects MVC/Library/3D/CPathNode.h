
/************************************************************************
*
*    FILE NAME:       CPathNode.h
*
*    DESCRIPTION:     Dynamic linked tree list node
*
************************************************************************/

#ifndef __CPATH_NODE_H__
#define __CPATH_NODE_H__

#include <vector>
#include "CPoint.h"

class CPathNode
{
public:

    static const int MAX_LINE = 300;

	// Constructer
    CPathNode( CPoint &point );

	// Reads a line of text from a text file
	static int GetLine( FILE *hFile, char *str );
	// decode node from passed string
	static bool Decode_Node( char *strData, int strCount, std::vector<CPathNode *> &nodeLst );
	// geterate the main node key
	static float GenerateKey(){ return ++mainNodeKeyCounter; };
	// geterate the sub node key
	static float GenerateSubKey( float mainKey );

	// Print this nodes data to a file
	void PrintDataToFile( FILE *hFile );
	// Scale the node
	void _fastcall Scale( float x, float y, float z );

	/////////////////
    // Set functions
    /////////////////

    // Set the previous node
	void SetPrevNode( CPathNode * node ){ prevNode = node; };
	// Set the next node
	void SetNextNode( CPathNode * node ){ nextNode.push_back( node ); };
	// Set the X
	void SetX( float value ){ pos.x = value; };
	// Set the Y
	void SetY( float value ){ pos.y = value; };
	// Set the Z
	void SetZ( float value ){ pos.z = value; };
	// Set radius
	void SetRadius( float value ){ radius = value; };
	// Set as main node
	void SetMainFlag( bool value ){ mainNode = value; };
	// Set the node index
	void SetKey( float _key ){ key = _key; };


	/////////////////
    // Get functions
    /////////////////

	// Get the previous node
	CPathNode * GetPrevNode(){ return prevNode; };	
	// get the number of next nodes
	unsigned int GetNextNodeCount(){ return nextNode.size(); };	
	// Get the X
	float GetX(){ return pos.x; };	
	// Get the Y
	float GetY(){ return pos.y; };	
	// Get the Z
	float GetZ(){ return pos.z; };
	// Get the Z
	float GetRadius(){ return radius; };
	// Get the Z
	bool GetMainFlag(){ return mainNode; };	
	// Get the node index
	float GetKey(){ return key; };
    // Get the next node. First node is considered to be the main node
	CPathNode * GetNextNode( unsigned int index = 0 );
	// Get the position of the node
	CPoint & GetPos(){ return pos; };

	/////////////////////
    // operator functions
    /////////////////////

	// Does node = node?
	bool operator == ( CPathNode &node );
    // The inequality operator
	bool operator != ( CPathNode *node );

private:

	// previous node
	CPathNode * prevNode;
	// position Data
	CPoint pos;
	// node index
	float key;
	// radius of node
	float radius;
	// flag to indicate this is a main node
	bool mainNode;
	// next node
	std::vector<CPathNode *> nextNode;

	// Static node key counters
	static float mainNodeKeyCounter;
	static float subNodeCounter;
	static float lastMainKey;
};

#endif	// __CGRAPH_NODE_H__