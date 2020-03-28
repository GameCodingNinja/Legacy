
/************************************************************************
*
*    FILE NAME:       CGraphList.h
*
*    DESCRIPTION:     Dynamic linked tree list template class
*
************************************************************************/

#ifndef __CGRAPH_LIST_H__
#define __CGRAPH_LIST_H__

#include <set>
#include <algorithm>
#include "CMatrix.h"         // Matrix header

// Turn off depreciated calls
#pragma warning(disable : 4996)

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

template <class node_type>
class CGraphList
{
public:

	// Constructer
	CGraphList();
	// Destructer
	~CGraphList();

	// Add a node
	bool AddNode( node_type * node );
	// Add sub nodes to exsisting nodes
	bool AddSubNodes( std::vector<node_type *> &nodeLst );
	// Find a node using collision detection
	node_type * FindNodeByCollision( CPoint &pos, float posRadius );
	// Do a bounding sphere collision check between the node
    // and the point we are checking against.
	bool Collide_BoundSphere( node_type * node,
							  CPoint &pos,
							  float posRadius );
	// Same collision as above only without the Y
	bool Collide_BoundSphereXZ( node_type * node,
							    CPoint &pos,
							    float posRadius );
	// Scale the nodes
	void Scale( float x, float y, float z );
	// Load graph list data
	bool LoadFromFile( const char *filePath );
	// Print out a text file to check data
	void BuildDebugFile( char * filepath );
	// Get the head node
	node_type * GetHeadNode(){ return head; };
	// Get unique node nist
	std::set<node_type *> & GetUniqueNodeList(){ return uniqueNodeList; };

private:

	// node head
	node_type * head;
	// list of all pointers nodes. Can be use for
	// quick searching and deleting
	std::set<node_type *> uniqueNodeList;	
};


/************************************************************************
*    DESCRIPTION:   Constructer                                                             
************************************************************************/
template <class node_type>
CGraphList<node_type>::CGraphList()
           : head(NULL)
{

}   // Constructer


/************************************************************************
*    DESCRIPTION:  Destructer                                                             
************************************************************************/
template <class node_type>
CGraphList<node_type>::~CGraphList()
{
	for( std::set<node_type *>::iterator it = uniqueNodeList.begin(); it != uniqueNodeList.end(); ++it )
		delete (*it);
	
	uniqueNodeList.clear();

}   // Destructer


/************************************************************************                                                             
*    DESCRIPTION:  Add a unique node
*
*    Input:    node_type * node - new node
************************************************************************/
template <class node_type>
bool CGraphList<node_type>::AddNode( node_type * node )
{
	// get the address of your linked list head 
	node_type * tmpNode = head;

	// Add our first node
	if( head == NULL )
	{
		head = node;
	}
	else
	{
		// traverse by moving the address to the next node
		while( tmpNode->GetNextNode() != NULL )
		{	
			// each nood has to be uinque so check it
			if( *tmpNode == *node )
			{
				delete node;
				return false;
			}

			tmpNode = tmpNode->GetNextNode(); 
		}	
			
		// Set the previous node
		node->SetPrevNode( tmpNode );

	    // Set the next node
		tmpNode->SetNextNode( node );
	}

	// Add to the delete list
	uniqueNodeList.insert( node );

    return true;

}	// AddNode


/************************************************************************                                                             
*    DESCRIPTION:  Add sub nodes to exsisting nodes
*
*    Input:    std::vector<node_type *> &nodeLst - sub node	list
************************************************************************/
template <class node_type>
bool CGraphList<node_type>::AddSubNodes( std::vector<node_type *> &nodeLst )
{
	unsigned int subNodeCounter(0);
	bool mainNodeFound(false);
	float mainNodeKey(0.0);
	float subCounter(0.0);

	// get the address of your linked list head 
	node_type * tmpNode = head;

    node_type * lastNodeMatch = NULL;

    // Holds the starting main node
	node_type * mainNode = NULL;

    // Search for the node that doesn't match
    while( tmpNode != NULL && subNodeCounter == 0 )
	{
		// Find the main node first
		if( !mainNodeFound )
		{
			if( *tmpNode == *nodeLst[ 0 ] )
			{
				mainNodeFound = true;

                // record the main node
				mainNode = tmpNode;

                // Get the head node index to label all the sub nodes the same index
				mainNodeKey = tmpNode->GetKey();

				continue;
			}
			
			tmpNode = tmpNode->GetNextNode();			
		}
		else
		{		
			// Work our way through the tree until we don't match
			for( unsigned int i = 0; i < nodeLst.size(); ++i )
			{
				bool nodeFound(false);

				if( *tmpNode == *nodeLst[ subNodeCounter ] )
				{
					// Record the last known node match
					lastNodeMatch = tmpNode;

					// get the next greatest node
					tmpNode = tmpNode->GetNextNode(tmpNode->GetNextNodeCount()-1);

					++subNodeCounter;

					nodeFound = true;                    
				}

				if( nodeFound == false )
					break;
			}
		}
	}

    // There's a problem if we don't find any matches
	if( subNodeCounter == 0 || subNodeCounter >= nodeLst.size() )
		return false;

    // Delete the duplicate nodes
	for( unsigned int i = 0; i < subNodeCounter; ++i )
	{
		delete nodeLst[i];	
	}

	// Add the remaing nodes to the list
	for( unsigned int i = subNodeCounter; i < nodeLst.size(); ++i )
	{
		// Set the previous node
		nodeLst[i]->SetPrevNode( lastNodeMatch );

		// Inc the counter
		++subCounter;

		// Check if the last node reconnects to the main nodes
		if( (mainNode != NULL) &&
		    (i == nodeLst.size()-1) &&
		    (*mainNode == *nodeLst[i]) )
		{
			// Set the next node
			lastNodeMatch->SetNextNode( mainNode );

            // delete that last node because it's not being used
			delete nodeLst[i];
		}
		else
		{
		    // Set the next node
			lastNodeMatch->SetNextNode( nodeLst[i] );

	        lastNodeMatch = nodeLst[i];

			// Set the node index same as the head node
			lastNodeMatch->SetKey( node_type::GenerateSubKey( mainNodeKey ) );
				
			// Add to the delete list
			uniqueNodeList.insert( nodeLst[i] );
		}
	}

	return true;

}	// AddSubNodes


/************************************************************************
*    DESCRIPTION:  Load graph list data
*
*    Input:    char *fileName - Path to load file
*
*    Output:   bool - true on success, false on error
************************************************************************/
template <class node_type>   
bool CGraphList<node_type>::LoadFromFile( const char *filePath )
{
	bool result = false;
	char * lineStr;
	FILE *hFile;
	float nodeCounter(0);

	// Open the file
    hFile = fopen( filePath, "rt" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Allocate our fetch string
    lineStr = new char[node_type::MAX_LINE];

    // Loop until we come to the end of the file
    while( 1 )
    {
        std::vector<CPathNode *> nodeLst;

        // Get a line of text
        int strCount = node_type::GetLine( hFile, lineStr );

        // Did we reach the end of the file?
        if( strCount == EOF )
            break;

        // Is there nothing to look at?
		// Is it a comment.
        else if( strCount == 0 || lineStr[0] == '#' )
            continue;

        if( !node_type::Decode_Node( lineStr, strCount, nodeLst ) )
			goto HANDLE_ERROR;
			
		// Add the node
		if( nodeLst.size() == 1 )
		{
			nodeLst[0]->SetKey( node_type::GenerateKey() );

			if( !AddNode( nodeLst[0] ) )
				goto HANDLE_ERROR;
		}
		else
		{
			if( !AddSubNodes( nodeLst ) )
				goto HANDLE_ERROR;
		} 
	}

    BuildDebugFile("graphDebug.txt");

	// If we got this far, everything's ok
    result = true;

HANDLE_ERROR:;

    // delete our string array
    delete [] lineStr;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}	// LoadFromFile


/************************************************************************
*    DESCRIPTION:  Print out a text file to check data
************************************************************************/
template <class node_type>
void CGraphList<node_type>::BuildDebugFile( char * filepath )
{
	FILE *hFile;
	node_type * tmpNode = head;

	// Open the file
    hFile = fopen( filepath, "wt" );
    if( hFile == NULL )
    {
        goto HANDLE_ERROR;
    }

	while( tmpNode != NULL )
	{
		tmpNode->PrintDataToFile( hFile );

		tmpNode = tmpNode->GetNextNode();
	}

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );

}	// BuildDebugFile


/************************************************************************
*    DESCRIPTION:  Find a node using collision detection
*
*    Input:    CPoint &pos - position to do collision check with
*              float posRadius - radius to use for collision checking
*
*    Output:   node_type * - return the found pointer or NULL
************************************************************************/
template <class node_type>
node_type * CGraphList<node_type>::FindNodeByCollision( CPoint &pos, float posRadius )
{
	node_type * result = NULL;

	// We are cheating by doing a simple for loop to find the node
	for( std::set<node_type *>::iterator it = uniqueNodeList.begin(); 
	     it != uniqueNodeList.end();
	     ++it )
	{
		if( Collide_BoundSphere( (*it), pos, posRadius ) )
		{
			result = (*it);
			break;
		}
	}

	return result;

}	// FindNodeByCollision


/************************************************************************
*    DESCRIPTION:  Do a bounding sphere collision check between the node
*                  and the point we are checking against.
*
*    Input:    node_type * node - pointer to node
*              CPoint &pos -  position to check node against
*              float posRadius -  radius of position
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class node_type>
bool CGraphList<node_type>::Collide_BoundSphere( node_type * node,
												 CPoint &pos,
												 float posRadius )
{
    float dist;
    bool result = false;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( pos.tx - node->GetX() ) * ( pos.tx - node->GetX() )) +
	             (( pos.ty - node->GetY() ) * ( pos.ty - node->GetY() )) +
                 (( pos.tz - node->GetZ() ) * ( pos.tz - node->GetZ() )) );

    // Check for possible collision by using bounding sphere
    if( dist <= posRadius + node->GetRadius() )
    {
        result = true;
    }
    
    return result;

}   // Collide_BoundSphere


/************************************************************************
*    DESCRIPTION:  Do a bounding sphere collision check between the node
*                  and the point we are checking against only using the
*                  x and z offsets.
*
*    Input:    node_type * node - pointer to node
*              CPoint &pos -  position to check node against
*              float posRadius -  radius of position
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class node_type>
bool CGraphList<node_type>::Collide_BoundSphereXZ( node_type * node,
												 CPoint &pos,
												 float posRadius )
{
    float dist;
    bool result = false;

    // Calculate the distance between the center points of both objects
    dist = sqrt( (( pos.tx - node->GetX() ) * ( pos.tx - node->GetX() )) +
                 (( pos.tz - node->GetZ() ) * ( pos.tz - node->GetZ() )) );

    // Check for possible collision by using bounding sphere
    if( dist <= posRadius + node->GetRadius() )
    {
        result = true;
    }
    
    return result;

}   // Collide_BoundSphere


/************************************************************************
*    DESCRIPTION:  Scale the nodes.
*
*    Input:    float x, y, z: Scale amounts
************************************************************************/
template <class node_type>
void CGraphList<node_type>::Scale( float x, float y, float z )
{
	// We are cheating by doing a simple for loop to find the node
	for( std::set<node_type *>::iterator it = uniqueNodeList.begin(); 
	     it != uniqueNodeList.end();
	     ++it )
	{
		(*it)->Scale( x, y, z );
	}

    BuildDebugFile("graphDebugScale.txt");

}	// Scale

#endif	// __CGRAPH_LIST_H__
