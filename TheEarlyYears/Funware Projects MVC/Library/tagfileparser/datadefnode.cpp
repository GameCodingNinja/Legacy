//*****************************************************************************
/*!
 *  \file   datadefnode.cpp
 *
 *  \brief  The base data definition node.  Used by the parsing routines to
 *          store information in a tree hierarchy.  The tree can be queried to
 *          retrieve the values stored there.
 *          The system is composed of two conceptually different 'types' of
 *          nodes "label nodes" and "leaf nodes".  The leaf nodes are the ones
 *          that actually contain the strings and ints.  The label nodes
 *          contain links to the leaf nodes.  When a label node is queried for
 *          data, the only way it can respond is to forward the request to the
 *          leaf node if it has one.  The leaf node will override the retrieval
 *          function related to its storage type and fail for all other types.
 *
 *****************************************************************************/

#include "datadefnode.h"
#include <sstream>
#include <iostream>
#include <cassert>


//*****************************************************************************
/*!
 *  \brief  Default constructor.
 *
 *****************************************************************************/
CDataDefNode::CDataDefNode()
:   pParent(NULL),
    pData(NULL)
{
}


//*****************************************************************************
/*!
 *  \brief  Constructor.
 *
 *  \param  const string & initialId - the identifier for this node.
 *
 *****************************************************************************/
CDataDefNode::CDataDefNode(const std::string & initialId)
:   identifier(initialId),
    pParent(NULL),
    pData(NULL)
{
}


//*****************************************************************************
/*!
 *  \brief  Destructor.
 *
 *****************************************************************************/
CDataDefNode::~CDataDefNode()
{
    // Delete all children recursively.
    DeleteChildren();

    // Delete any associated data node.
    if (pData != NULL)
    {
        delete pData;
        pData = NULL;
    }
}


//*****************************************************************************
/*!
 *  \brief  Clone this node and its subtree recursively.
 *
 *  \return CDataDefNode * - A pointer to a heap allocated cloned subtree.
 *
 *****************************************************************************/
CDataDefNode * CDataDefNode::Clone() const
{
    // Allcoate the cloned object.
    CDataDefNode * pClone = new CDataDefNode();

    // Copy this object into the cloned version.
    pClone->CopyHelper(*this);

    // Return the clone to the caller (caller responsible for deletion).
    return (pClone);
}


//*****************************************************************************
/*!
 *  \brief  Delete all children of this data node, recursively.
 *
 *****************************************************************************/
void CDataDefNode::DeleteChildren()
{
    // Loop through each child node.
    for (TDataDefNodeMultiMapIter childIter = children.begin();
         childIter != children.end();
         childIter++)
    {
        // Ensure that no attempt is made to delete a NULL pointer.
        if (childIter->second != NULL)
        {
            // Free up the memory for the child node.
            delete childIter->second;
        }
    }

    children.clear();
}


//*****************************************************************************
/*!
 *  \brief  Return the identifier of this node.
 *
 *  \return const string & - the identifier of this node.
 *
 *****************************************************************************/
const std::string & CDataDefNode::GetIdentifier() const
{
    return (identifier);
}


//*****************************************************************************
/*!
 *  \brief  Set the data pointer for this node to be the one supplied.
 *
 *  \param  CDataDefNode * pNewData - a pointer to the new data object for this
 *          node.
 *
 *****************************************************************************/
void CDataDefNode::SetData(CDataDefNode * pNewData)
{
    if (pData != NULL)
    {
        // Delete any data that was previously attached to this node.
        delete pData;
    }

    pData = pNewData;
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain integer data.
 *
 *  \param  int & value - the variable to store the integer data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(int & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain integer data.
 *
 *  \param  float & value - the variable to store the integer data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(float & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain integer data.
 *
 *  \param  float & value - the variable to store the integer data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(double & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain string data.
 *
 *  \param  string & value - the variable to store the string data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(std::string & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array integer data.
 *
 *  \param  TIntVector & value - used to store the array integer data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TIntVector & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array integer data.
 *
 *  \param  TFloatVector & value - used to store the array float data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TFloatVector & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}

//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array integer data.
 *
 *  \param  TDblVector & value - used to store the array double data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TDoubleVector & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array string data.
 *
 *  \param  TStringList & value - the variable to store the array string data
 *          in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TStringVector & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain an array of data.
 *
 *  \param  TDataDefNodeVector & array - destination array.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TDataDefNodeVector & array) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(array);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array integer data.
 *
 *  \param  TIntList & value - used to store the array integer data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TIntList & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array integer data.
 *
 *  \param  TFloatList & value - used to store the array float data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TFloatList & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}

//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array integer data.
 *
 *  \param  TDoubleList & value - used to store the array double data in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TDoubleList & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Default functionality, intended to be overridden by any derived
 *          classes that contain array string data.
 *
 *  \param  TStringList & value - the variable to store the array string data
 *          in.
 *
 *  \return bool - true if the value was set, false otherwise.  This base class
 *          defaults to a return value of false, indicating that nothing gets
 *          set when this function is called.
 *
 *****************************************************************************/
const bool CDataDefNode::GetData(TStringList & value) const
{
    bool found = false;

    if (pData != NULL)
    {
        found = pData->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Store a node as a child of this node.
 *
 *  \param  CDataDefNode * pChild - the child to store.
 *
 *****************************************************************************/
void CDataDefNode::AddChild(CDataDefNode * pChild)
{
    pChild->pParent = this;

    children.insert(make_pair(pChild->GetIdentifier(), pChild));
}


//*****************************************************************************
/*!
 *  \brief  The function that clients use to request nodes from within the
 *          hierarchy that this node is the root of.
 *
 *  \param  const string & requestString - the string used to specify the node
 *          that is being requested.
 *  \param  const bool recursive - true if the client is requesting a recursive
 *          search of all nodes in the hierarchy.  False will result in only
 *          the direct children of this node being tested for a name match.
 *
 *  \return CDataDefNode * - A pointer to the requested node.  NULL if not
 *          found.
 *
 *****************************************************************************/
CDataDefNode * CDataDefNode::GetChild(const std::string & requestString,
                                      const bool recursive) const
{
    CDataDefNode * pRetrievedNode = NULL;

    // Search to see if there are paths or indexes involved.
    if (requestString.find_first_of("/[]") != std::string::npos)
    {
        // Handle the path and index retrieval of the requested node.
        pRetrievedNode = GetChildByPath(requestString);
    }
    else
    {
        // Do a potentially recursive search for the requested child.
        pRetrievedNode = GetChildByName(requestString, recursive);
    }

    return (pRetrievedNode);
}


//*****************************************************************************
/*!
 *  \brief  Searches the node hierarchy, using this node as the root.  Attempts
 *          to find the node that has the supplied ID and returns that.  Will
 *          return NULL if the node is not found.
 *
 *  \param  const string & requestedID - the ID of the requested node.
 *  \param  const bool recursive - true if the client is requesting a recursive
 *          search of all nodes in the hierarchy.  False results in only the
 *          direct children of this node being tested for a name match.
 *
 *  \return CDataDefNode * - the specified node, NULL if not found.
 *
 *****************************************************************************/
CDataDefNode * CDataDefNode::GetChildByName(const std::string & requestedID,
                                            const bool recursive) const
{
    CDataDefNode * pResultNode = NULL;

    // Retrieve an instance of a matching child.
    TDataDefNodeMultiMapConstIter retrievedEntryIter = children.find(requestedID);

    // Test to see whether a matching node was found.
    if (retrievedEntryIter != children.end())
    {
        // Set the result node to be the one retrieved.
        pResultNode = retrievedEntryIter->second;
    }
    else if (recursive)
    {
        // Loop through all children of this node, recursively searching their children.
        for (TDataDefNodeMultiMapConstIter childMapIter = children.begin();
             ((childMapIter != children.end()) && (pResultNode == NULL));
             childMapIter++)
        {
            CDataDefNode * pChild = childMapIter->second;

            if (pChild != NULL)
            {
                // Recursively search each child for the requested node.
                pResultNode = pChild->GetChildByName(requestedID);
            }
        }
    }

    return (pResultNode);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve a pointer to a sub-node by node path.
 *
 *          GetChildByPath() can be used to explicitly describe a node within a
 *          node tree hiearchy with a single string.  This is best demonstrated
 *          by example.  Consider the following tag file (node tree):
 *
 *          <TopNode>
 *              <Identifier> bunchofentries </Identifier>
 *              <EntryList>
 *                  <Entry> alpha </Entry>
 *                  <Entry> beta </Entry>
 *              </EntryList>
 *          </TopNode>
 *
 *          Some examples of legal paths from the root node (which exists
 *          implicitly above the TopNode are:
 *              TopNode
 *              TopNode/Identifier
 *              TopNode/EntryList/Entry[0]
 *              TopNode/EntryList/Entry[1]
 *
 *          Likewise, valid paths from the EntryList node are:
 *              Entry[0]
 *              Entry[1]
 *
 *  \param  const string & path - the path as described above.
 *
 *  \return CDataDefNode * - a pointer to the CDataDefNode specified by the
 *          path parameter, or NULL if the path did not exist.
 *
 *****************************************************************************/
CDataDefNode * CDataDefNode::GetChildByPath(const std::string & path) const
{
    // Storage for the tokenized node path.
    TTokenList nodeIDs;

    // Break the supplied string into its component IDs.
    CTokenizer::TokenizeString(nodeIDs, path.c_str(), "/");

    // Use the tokenized path to retrieve the specified node.
    CDataDefNode * pRetrievedNode = GetChildByPath(nodeIDs);

    return (pRetrievedNode);
}


//*****************************************************************************
/*!
 *  \brief  The strings that describe each node in the required path are
 *          contained in the nodeIDs token list.  This list will be traversed
 *          recursively.  This function will also detect
 *
 *  \param  TTokenList & nodeIDs - the list of tokens that represent the nodes
 *          that make up the specified path.
 *
 *  \return CDataDefNode * - the node that has been requested.
 *
 *****************************************************************************/
CDataDefNode * CDataDefNode::GetChildByPath(TTokenList & nodeIDs) const
{
    // This variable will hold the node to recurse to (or the final node if
    // there is no further path).
    CDataDefNode * pNextNode = NULL;

    if (!nodeIDs.empty())
    {
        // Retrieve the first node ID from the list supplied.
		std::string firstNodeID = nodeIDs.front();

        // Reduce the list to allow the recursive call to be made later.
        nodeIDs.pop_front();

        // Storage for the tokenized node specification string.
        TTokenList childAndIndex;

        // Now the first path component is checked for an array index contained in
        // square brackets (i.e. TagName[0]).  The CTokenizer class is used to
        // tokenize the first path part into a node name and an index (if any).
        // After this section, the variable nodeName holds the node's name.
        CTokenizer::TokenizeString(childAndIndex, firstNodeID.c_str(), "[]");

        if (!childAndIndex.empty())
        {
			std::string nodeName = childAndIndex.front();

            // If the size is greater than 1 there was an index involved.
            if (childAndIndex.size() > 1)
            {
                // Retrieve the string that contains the index.
				std::string indexString = childAndIndex.back();

                // The string stream that will be used to retrieve the index.
                std::istringstream stringStream(indexString);

                // Storage for the retrieved index.
                unsigned int index;

                // Retrieve the integer value from the string stream.
				stringStream >> index;

                // Retrieve the specified node using the index based function.
                pNextNode = GetChildByIndex(nodeName, index);
            }
            else
            {
                // ID is not indexed, search ONLY the children of this node.
                pNextNode = GetChildByName(nodeName, false);
            }
        }
    }

    // Either the requested child node for this path part has been found, or
    // pNextNode is still NULL if not.  If it has been found, return it or
    // recurse if there is more path left.
    if ((pNextNode != NULL) && !nodeIDs.empty())
    {
        pNextNode = pNextNode->GetChildByPath(nodeIDs);
    }

    return (pNextNode);
}


//*****************************************************************************
/*!
 *  \brief  Get a child of this node, specified by its ID and index. e.g.
 *          Entry[0] - If this node is an array, this will return the first
 *                     element.
 *                   - If Entry is not an array, this will return the first
 *                     child of this node that is name Entry.
 *
 *  \param  const string & requestedID - the name of the node requested.
 *  \param  const unsigned int index - the index of the node requested.
 *
 *  \return CDataDefNode * - the node that has been requested.
 *
 *****************************************************************************/
CDataDefNode * CDataDefNode::GetChildByIndex(const std::string & requestedID,
                                             const unsigned int index) const
{
    // Assume that the node won't be found.
    CDataDefNode * pRetrievedNode = NULL;

    // Storage for the potential retrieval of the array information.
    TDataDefNodeVector nodes;

    // Determine whether this node is an array, retrieve data at the same time.
    if (GetData(nodes))
    {
        // Sanity check the index value.
        if (index < nodes.size())
        {
            // Index is within range, use this indexed node as the found node.
            pRetrievedNode = nodes[index];
        }
    }
    else
    {
        // Node isn't an array node, retrieve all children that match the ID.
        // The equal_range function of a multimap will return an pair of
        // iterators.  One to the start of the matching range and one to the
        // end of the range.
        TDataDefNodeMultiMapConstIterPair retrievedIterPair = children.equal_range(requestedID);

        // The storage type of our multimap is "pair" objects that contain a
        // string that acts as a key and a CDataDefNode pointer that is the
        // value.  We use the iterators determined above to create a vector
        // of the same type as that stored in the multimap (i.e. pairs of
        // strings and CDataDefNode *).  The trick is that because the
        // iterators retrieved above define the matching range, all items in
        // the new vector will match the requested ID.

        // Create a vector using the iterators defining the matching range.
		std::vector< std::pair<std::string, CDataDefNode *> > retrievedNodes(retrievedIterPair.first, retrievedIterPair.second);

        // Sanity check the index value.
        if (index < retrievedNodes.size())
        {
            // Index is within range, use this indexed node as the found node.
            pRetrievedNode = retrievedNodes[index].second;
        }
    }

    if (pRetrievedNode == NULL)
    {
        // The node was not found, index is out of range.
		std::cerr << requestedID << "[" << index << "].  Index is out of range "  << std::endl;
    }

    return (pRetrievedNode);
}


//*****************************************************************************
/*!
 *  \brief  Ouput information about this node and any children.
 *
 *  \param  const unsigned int indentLevel - how far to indent the information visually.
 *
 *****************************************************************************/
void CDataDefNode::Dump(const unsigned int indentLevel) const
{
	std::cerr << std::string(indentLevel, '\t') << GetIdentifier() << std::endl;

    if (pData != NULL)
    {
        pData->Dump(indentLevel + 1);
    }
    else
    {
        DumpChildren(indentLevel);
    }
}


//*****************************************************************************
/*!
 *  \brief  Output the children of this node.
 *
 *  \param  const unsigned int indentLevel - how far to indent the information visually.
 *
 *****************************************************************************/
void CDataDefNode::DumpChildren(const unsigned int indentLevel) const
{
    for (TDataDefNodeMultiMapConstIter childIter = children.begin(); childIter != children.end(); childIter++)
    {
        CDataDefNode * pChild = childIter->second;

        if (pChild != NULL)
        {
            pChild->Dump(indentLevel + 1);
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Helper function to facilitate a deep copy of a similar object.
 *
 *  \param  const CDataDefNode & copy - The object to be copied.
 *
 *****************************************************************************/
void CDataDefNode::CopyHelper(const CDataDefNode & copy)
{
    DeleteChildren();

    // Delete the pData of this object if it exists (will be replaced by copy).
    if (pData != NULL)
    {
        delete pData;
        pData = NULL;
    }

    // Copy the identifier.
    identifier = copy.identifier;

    // Copy the data, if there is any (left NULL if not).
    if(copy.pData != NULL)
    {
        SetData(copy.pData->Clone());
    }

    // Clone the children if they're available.
    for (TDataDefNodeMultiMapConstIter childIter = copy.children.begin(); childIter != copy.children.end(); childIter++)
    {
        if (childIter->second != NULL)
        {
            AddChild(childIter->second->Clone());
        }
    }
}


// Definition of non-templated parsing helpers
namespace Parsing
{
//*****************************************************************************
/*!
 *  \brief  Wrapper for requiring a child from a datadefnode pointer
 *
 *  \param  - pointer to datadefnode to search
 *  \param  - the child name required
 *
 *  \return - the child
 *
 *****************************************************************************/
CDataDefNode * RequireChild(CDataDefNode const * pNode,
                            const std::string & childName)
{
    CDataDefNode * pChild(pNode->GetChild(childName));
    assert (NULL != pChild);
    return (pChild);
}

}

