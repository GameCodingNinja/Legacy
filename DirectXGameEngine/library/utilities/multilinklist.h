
/************************************************************************
*
*    FILE NAME:       multilinklist.h
*
*    DESCRIPTION:     Dynamic linked tree list template class
*
************************************************************************/

#ifndef __MULTI_LINK_LIST_H__
#define __MULTI_LINK_LIST_H__

// Standard lib dependencies
#include <vector>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <utilities/deletefuncs.h>

// Turn off depreciated calls
//#pragma warning(disable : 4996)

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
//#pragma warning(disable : 4244)

template <class node_type>
class CMultiLinkLst : public boost::noncopyable
{
public:

    // Constructer
    CMultiLinkLst();

    // Destructer
    virtual ~CMultiLinkLst();

    // Add a node
    bool AddNode( node_type node );

    // Find a parent node
    node_type FindParent( node_type node, node_type searchNode );

    // Get the head node
    node_type GetHeadNode();

    // Get the node
    node_type GetNode( size_t index );

protected:

    // Reset the search iterators
    void ResetSearchIterators();

    // node head
    node_type head;

    // list of all pointer nodes. Can be use for
    // quick searching and deleting.
    std::vector<node_type> nodeList;
};


/************************************************************************
*    desc:   Constructer                                                             
************************************************************************/
template <class node_type>
CMultiLinkLst<node_type>::CMultiLinkLst()
                         : head(NULL)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
template <class node_type>
CMultiLinkLst<node_type>::~CMultiLinkLst()
{
    NDelFunc::DeleteVectorPointers( nodeList );

}   // Destructer


/************************************************************************                                                             
*    desc:  Add a unique node
*
*    param: node_type * node - new node
************************************************************************/
template <class node_type>
bool CMultiLinkLst<node_type>::AddNode( node_type node )
{
    // Add our first node
    if( head == NULL )
        head = node;

    else
    {
        // reset the node iterators for searching
        ResetSearchIterators();

        // Call a recursive function to find the parent node
        node_type pParentNode = FindParent( node, head );

        // Set the nodes
        if( pParentNode != NULL )
        {
            node->pPrevNode = pParentNode;
            pParentNode->nextNode.push_back(node);
        }
    }

    // Add to the node list
    nodeList.push_back( node );

    return true;

}	// AddNode


/************************************************************************                                                             
*    desc:  See if this is the node we are looking for
*           NOTE: This is a recursive function
*
*    param: node_type * node - new node
*           node_type * searchNode - node to check
************************************************************************/
template <class node_type>
node_type CMultiLinkLst<node_type>::FindParent( node_type node, node_type searchNode )
{
    node_type pResult = NULL;

    if( searchNode != NULL )
    {
        if( searchNode->id == node->parentId )
        {
            pResult = searchNode;
        }
        else
        {
            node_type nextNode;

            do
            {
                // get the next node 
                nextNode = searchNode->GetNextNode();

                if( nextNode != NULL )
                {
                    // Call a recursive function to find the parent node
                    pResult = FindParent( node, nextNode );
                }
            }
            while( nextNode != NULL );
        }
    }

    return pResult;

}	// FindParent


/************************************************************************                                                             
*    desc:  Reset the search iterators
************************************************************************/
template <class node_type>
void CMultiLinkLst<node_type>::ResetSearchIterators()
{
    for( std::vector<node_type>::iterator it = nodeList.begin(); 
             it != nodeList.end(); ++it )
    {
        (*it)->Reset();
    }
}	// ResetSearchIterators


/************************************************************************                                                             
*    desc:  Get the head node
************************************************************************/
template <class node_type>
node_type CMultiLinkLst<node_type>::GetHeadNode()
{
    return head;

}	// GetHeadNode


/************************************************************************                                                             
*    desc:  Get the head node
************************************************************************/
template <class node_type>
node_type CMultiLinkLst<node_type>::GetNode( size_t index )
{
    return nodeList[index];

}	// GetNode


#endif	// __MULTI_LINK_LIST_H__
