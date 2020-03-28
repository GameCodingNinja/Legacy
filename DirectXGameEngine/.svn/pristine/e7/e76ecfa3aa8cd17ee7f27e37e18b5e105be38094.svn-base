
// Library dependencies
#include <stdio.h>


/***********************************************************************************
*	CLASS:	The CDoublyLinkedList class is the class that holds the first and last
*			of a series of CNode objects.
***********************************************************************************/
class CDoublyLinkedList
{
public:

    /***********************************************************************************
    *	CLASS:	The CNode class is the class that holds the data stored in the doubly 
    *			linked list. It is a class that exists within the CDoublyLinkedList class
    ***********************************************************************************/
    class CNode
    {
    public:

        // Constructor. We default the previous and next nodes to null
        CNode():pPrevNode(NULL),
                pNextNode(NULL),
                data(0){}

        // Data that the node holds
        int data;

        // Pointers to the previous and next nodes
        CNode * pPrevNode;
        CNode * pNextNode;
    };


    /***********************************************************************************
    *	desc:	Constructor. We default the first and last nodes to null
    ***********************************************************************************/
    CDoublyLinkedList():pFirstNode(NULL),
                        pLastNode(NULL),
                        listSize(0){}


    /***********************************************************************************
    *	desc:	Destructor. Make sure we free up all the memory we allocated
    ***********************************************************************************/
    ~CDoublyLinkedList()
    {
        clear();

    }	// Destructor


    /***********************************************************************************
    *	desc:	Get the node at a certain spot in the list
    *
    *	param:	unsigned int index - index into the list to get the node
    ***********************************************************************************/
    CNode * getNode( unsigned int index )
    {
        // Node to travel the list
        CNode * pTmpNode;

        // We figure out how many nodes we'd have to travel if we traveled backwards from
        // the last node
        unsigned int backIndex = (listSize - 1) - index;

        // Take the shortest path to that node's index. We go in here if it would be quicker
        // to travel from the first node to the node at the desired index
        if( backIndex > index )
        {
            pTmpNode = pFirstNode;
            
            // Travel the list
            for( unsigned int i = 0; i < index; i++ )
                pTmpNode = pTmpNode->pNextNode;
        }
        // We go in here if it would be quicker to travel from the last node to the node
        // at the desired index
        else
        {
            pTmpNode = pLastNode;

            // Travel the list
            for( unsigned int i = 0; i < backIndex; i++ )
                pTmpNode = pTmpNode->pPrevNode;
        }

        return pTmpNode;

    }	// getNode


    /***********************************************************************************
    *	desc:	Insert a data element to the back of linked list
    *
    *	param:	int data - data to insert
    ***********************************************************************************/
    void insertBack( int data )
    {
        // If our first node is null, we new it up and give it the data passed in
        if( pFirstNode == NULL )
        {
            pFirstNode = new CNode();	// Create the node
            pFirstNode->data = data;	// Set the data
        }
        // If our last node is null, we new it up and give it the data passed in
        else if( pLastNode == NULL )
        {
            pLastNode = new CNode();			// Create the node
            pLastNode->data = data;				// Set the data
            pLastNode->pPrevNode = pFirstNode;	// The last node's previous node is the first node
            pFirstNode->pNextNode = pLastNode;	// The first node's next node is the last node
        }
        // If both are already newed up, we new up a new node and have it switch places with
        // the last node
        else
        {
            CNode * pNewNode = new CNode();		// Create the node
            pNewNode->data = data;				// Set the data
            pNewNode->pPrevNode = pLastNode;	// It's previous node will be the current last node
            pLastNode->pNextNode = pNewNode;	// The current last node's next node will be the one we just made
            pLastNode = pNewNode;				// The node we just made is now the last node
        }

        // Increment the size counter
        listSize++;

    }	// insertBack


    /***********************************************************************************
    *	desc:	Insert a data element to the front of linked list
    *
    *	param:	int data - data to insert
    ***********************************************************************************/
    void insertFront( int data )
    {
        // If our first node is null, we new it up and give it the data passed in
        if( pFirstNode == NULL )
        {
            pFirstNode = new CNode();	// Create the node
            pFirstNode->data = data;	// Set the data
        }
        // If our last node is null, we new it up and give it the data passed in
        else if( pLastNode == NULL )
        {
            pLastNode = new CNode();			// Create the node
            pLastNode->data = data;				// Set the data
            pLastNode->pPrevNode = pFirstNode;	// The last node's previous node is the first node
            pFirstNode->pNextNode = pLastNode;	// The first node's next node is the last node
        }
        // If both are already newed up, we new up a new node and have it switch places with
        // the first node
        else
        {
            CNode * pNewNode = new CNode();		// Create the node
            pNewNode->data = data;				// Set the data
            pNewNode->pNextNode = pFirstNode;	// It's next node will be the current first node
            pFirstNode->pPrevNode = pNewNode;	// The current first node's previous node will be the one we just made
            pFirstNode = pNewNode;				// The node we just made is now the first node
        }

        // Increment the size counter
        listSize++;

    }	// insertFront


    /***********************************************************************************
    *	desc:	Insert a data element to a certain spot in the linked list
    *
    *	param:	unsigned int index - index into the list to insert the data
    *			int data		   - data to insert
    ***********************************************************************************/
    void insertIndex( unsigned int index, int data )
    {
        // If our first node is null, we new it up and give it the data passed in
        if( pFirstNode == NULL )
        {
            pFirstNode = new CNode();	// Create the node
            pFirstNode->data = data;	// Set the data
        }
        // If we're placing the data in an index larger or equal to the size of the list,
        // we just insert the data to the back of the list
        else if( index >= listSize )
        {
            insertBack( data );		
        }
        // If the index is 0, we'll just insert the data to the front of the list
        else if( index == 0 )
        {
            insertFront( data );
        }
        // If the index lands somewhere in the middle of our list, we insert the
        // data to that spot
        else
        {
            // Set up a temporary node to traverse the list
            //CNode * pTmpNode = pFirstNode;
            CNode * pTmpNode = getNode(index);

            // Allocate a new node to hold the data
            CNode * pNewNode = new CNode();

            pNewNode->data = data;			// Set the data
            pNewNode->pNextNode = pTmpNode;	// The tmp node is the new node's next node
            
            // The node before the tmp node is the new node's previous node
            pNewNode->pPrevNode = pTmpNode->pPrevNode;

            // Make sure the node after the new node is pointing to the new node
            pNewNode->pNextNode->pPrevNode = pNewNode;

            // Make sure the node before the new node is pointing to the new node
            pNewNode->pPrevNode->pNextNode = pNewNode;	
        }

        // Increment the size counter
        listSize++;

    }	// insertIndex


    /***********************************************************************************
    *	desc:	Get the data at a certain spot in the list
    *
    *	param:	unsigned int index - index into the list to get the data
    *
    *	ret:	int & - reference to the data
    ***********************************************************************************/
    int & getData( unsigned int index )
    {
        return getNode(index)->data;

    }	// getData


    /***********************************************************************************
    *	desc:	Remove an element from the list
    *
    *	param:	unsigned int index - index into the list
    ***********************************************************************************/
    void remove( unsigned int index )
    {
        // Make sure we're not trying to remove something that doesn't exist
        if( index < listSize )
        {
            // Get the node at the index
            CNode * pTmpNode = getNode(index);

            if( pTmpNode == pFirstNode )
            {
                pTmpNode->pNextNode->pPrevNode = NULL;
                pFirstNode = pTmpNode->pNextNode;
            }
            else if( pTmpNode == pLastNode )
            {
                pTmpNode->pPrevNode->pNextNode = NULL;
                pLastNode = pTmpNode->pPrevNode;
            }
            else
            {
                // Get the next node's previous node pointer to point to the tmp node's previous pointer
                pTmpNode->pNextNode->pPrevNode = pTmpNode->pPrevNode;

                // Get the previous node's next node pointer to point to the tmp node's next pointer
                pTmpNode->pPrevNode->pNextNode = pTmpNode->pNextNode;
            }

            // Delete the tmp node
            delete pTmpNode;

            // Decrement the list size
            listSize--;
        }
    }


    /***********************************************************************************
    *	desc:	Clear the entire list
    ***********************************************************************************/
    void clear()
    {
        // We use a temporary node to traverse the list and delete each node. We
        // start with the last node and work our way backwards
        CNode * pTmpNode = pLastNode;

        // We will loop until our temporary node becomes null
        while( pTmpNode != NULL )
        {
            // If the temp node's previous node isn't null, we want to move
            // the temp node back one, delete the node the temp one was originally
            // pointing at, and set it to null
            if( pTmpNode->pPrevNode != NULL )
            {
                pTmpNode = pTmpNode->pPrevNode;
                delete pTmpNode->pNextNode;
                pTmpNode->pNextNode = NULL;
            }
            // Once the previous node equals null, we delete the temporary node itself
            // and set it to null so that we can exit the loop
            else
            {
                delete pTmpNode;
                pTmpNode = NULL;
            }
        }

        pFirstNode = NULL;
        pLastNode = NULL;

        // Reset the list size
        listSize = 0;
    }


    /***********************************************************************************
    *	desc:	Get the size of the list
    *
    *	ret:	int - list size
    ***********************************************************************************/
    unsigned int size()
    {
        return listSize;

    }	// size


    /***********************************************************************************
    *	desc:	Print out the contents of the linked list starting with the front
    ***********************************************************************************/
    void printForward()
    {
        // Start with the first node
        CNode * pTmpNode = pFirstNode;

        // Travel through the nodes printing out the data until we hit the end
        while( pTmpNode != NULL )
        {
            printf( "%d ", pTmpNode->data );
            pTmpNode = pTmpNode->pNextNode;
        }

        // If the first node is null, the list is empty
        if( pFirstNode == NULL )
            printf("List is empty.");

        printf("\n");

    }	// printForward


    /***********************************************************************************
    *	desc:	Print out the contents of the linked list starting with the back
    ***********************************************************************************/
    void printBackward()
    {
        // Start with the last node
        CNode * pTmpNode = pLastNode;

        // Travel through the nodes printing out the data until we hit the beginning
        while( pTmpNode != NULL )
        {
            printf( "%d ", pTmpNode->data );
            pTmpNode = pTmpNode->pPrevNode;
        }

        // If the first node is null, the list is empty
        if( pFirstNode == NULL )
            printf("List is empty.");

        printf("\n");

    }	// printBackward

private:

    // Pointers to the first and last nodes in the list
    CNode * pFirstNode;
    CNode * pLastNode;

    // The size of the list
    unsigned int listSize;

};

