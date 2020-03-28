//*****************************************************************************
/*!
 *  \file   datadefnode.h
 *
 *  \brief  The base data definition node.  Used by the parsing routines to
 *          store information in a tree hierarchy.  The tree can be queried to
 *          retrieve the values stored there.
 *
 *****************************************************************************/

#ifndef _datadefnode_h_
#define _datadefnode_h_

#include <map>
#include <string>
#include <vector>

#include "tokenizer.h"
#include "listdefs.h"
#include "vectordefs.h"

// Forward declaration to allow the typedefing below.
class CDataDefNode;

// Type definitions to reduce complexity of access code.
typedef std::vector<CDataDefNode *> TDataDefNodeVector;
typedef TDataDefNodeVector::iterator TDataDefNodeVectorIter;
typedef TDataDefNodeVector::const_iterator TDataDefNodeVectorConstIter;
typedef std::multimap<std::string, CDataDefNode *> TDataDefNodeMultiMap;
typedef TDataDefNodeMultiMap::iterator TDataDefNodeMultiMapIter;
typedef TDataDefNodeMultiMap::const_iterator TDataDefNodeMultiMapConstIter;
typedef std::pair<TDataDefNodeMultiMapIter, TDataDefNodeMultiMapIter> TDataDefNodeMultiMapIterPair;
typedef std::pair<TDataDefNodeMultiMapConstIter, TDataDefNodeMultiMapConstIter> TDataDefNodeMultiMapConstIterPair;

//*****************************************************************************
/*!
 *  \class  CDataDefNode
 *
 *  \brief  Base definition node class.  This node allows a tree to be formed
 *          of definition nodes.  Any node that provides specific data storage
 *          and access functionality overrides the accessor functions provided
 *          here to accomplish it.
 *
 *****************************************************************************/
class CDataDefNode
{

public:
    // Default constructor.
    CDataDefNode();

    // Constructor.
    CDataDefNode(const std::string & initialId);

    // Destructor.
    virtual ~CDataDefNode();

    // Clone this node and its subtree recursively.
    virtual CDataDefNode * Clone() const;

    // Delete all children of this data node, recursively.
    void DeleteChildren();

    // Returns the string identifier of this node.
    const std::string & GetIdentifier() const;

    // Store the supplied node as the data node of this one.
    virtual void SetData(CDataDefNode * pNewData);

    // Retrieve an int.  Returns true if successful.
    virtual const bool GetData(int & value) const;

    // Retrieve an float.  Returns true if successful.
    virtual const bool GetData(float & value) const;

    // Retrieve an double.  Returns true if successful.
    virtual const bool GetData(double & value) const;

    // Retrieve a string.  Returns true if successful.
    virtual const bool GetData(std::string & value) const;

    // Retrieve a vector of ints.  Returns true if successful.
    virtual const bool GetData(TIntVector & value) const;

	// Retrieve a vector of ints.  Returns true if successful.
    virtual const bool GetData(TFloatVector & value) const;

	// Retrieve a vector of ints.  Returns true if successful.
    virtual const bool GetData(TDoubleVector & value) const;

    // Retrieve a vector of strings.  Returns true if successful.
    virtual const bool GetData(TStringVector & strings) const;

    // Retrieve an array of nodes.  Returns true if successful.
    virtual const bool GetData(TDataDefNodeVector & array) const;

    // Retrieve a list of ints.  Returns true if successful.
    virtual const bool GetData(TIntList & value) const;

	// Retrieve a list of ints.  Returns true if successful.
    virtual const bool GetData(TFloatList & value) const;

	// Retrieve a list of ints.  Returns true if successful.
    virtual const bool GetData(TDoubleList & value) const;

    // Retrieve a list of strings.  Returns true if successful.
    virtual const bool GetData(TStringList & strings) const;

    // Retrieve data of the template type.  Returns true if successful.
    template<typename T>
    const bool GetDataByName(const std::string & requestedID, T & value) const;

    // Retrieve data of the template type by node path.
    template<typename T>
    const bool GetDataByPath(const std::string & path, T & value) const;

    // Store the supplied node as a child of this node.
    virtual void AddChild(CDataDefNode * pChild);

    // Retrieve the named node from within the hierarchy of this node.
    CDataDefNode * GetChild(const std::string & requestString,
                            const bool recursive = true) const;

    // Output information about this node and, recursively, all children.
    virtual void Dump(const unsigned int indentLevel = 0) const;

    // Output information about this node's children.
    virtual void DumpChildren(const unsigned int indentLevel = 0) const;

protected:
    // Retrieve the specified node from within the hierarchy of this node.
    CDataDefNode * GetChildByName(const std::string & requestedID,
                                  const bool recursive = true) const;

    // Retrieve a pointer to a sub-node by node path.
    CDataDefNode * GetChildByPath(const std::string & path) const;

    // Retrieve a pointer to a sub-node by node path.
    CDataDefNode * GetChildByPath(TTokenList & nodeIDs) const;

    // Retrieve the specified node from this node's children. NOT RECURSIVE.
    CDataDefNode * GetChildByIndex(const std::string & requestedID, unsigned int index) const;

    // Helper function to facilitate a deep copy of a similar object.
    void CopyHelper(const CDataDefNode & copy);

private: // methods

    // These default methods are left undefined and should not be called.
    CDataDefNode(const CDataDefNode & copy);
    CDataDefNode & CDataDefNode::operator=(const CDataDefNode & rhs);

private:
    // The identifier of this node, used to retrieve info by ID.
    std::string identifier;

    // The parent of this node.
    CDataDefNode * pParent;

    // The data node for this node.
    CDataDefNode * pData;

    // Storage for the children of this node.
    TDataDefNodeMultiMap children;
};


//*****************************************************************************
/*!
 *  \brief  Template function to allow access to the data contained within this
 *          node.
 *
 *  \param  const string & requestedID - the name of the node being requested.
 *  \param  T & value - The templatized parameter of this function.  Will be
 *          set to a retrieved value, if the data is found..
 *
 *  \return bool - true if the data was retrieved successfully.
 *
 *****************************************************************************/
template<typename T>
const bool CDataDefNode::GetDataByName(const std::string & requestedID, T & value) const
{
    bool found = false;

    const CDataDefNode * pRetrievedNode = GetChild(requestedID);

    if (pRetrievedNode != NULL)
    {
        found = pRetrievedNode->GetData(value);
    }

    return (found);
}


//*****************************************************************************
/*!
 *  \brief  Template method to access data contained in a node specified by a
 *          path from this node.
 *
 *  \param  const string & path - the path as described in GetChildByPath().
 *  \param  T & value - the templatized function output parameter, a reference
 *          to a variable in which the nodes data should be placed if found.
 *
 *  \return bool - True if the node was found and its data placed into the
 *          value output parameter, or false if the node was not found.
 *
 *****************************************************************************/
template<typename T>
const bool CDataDefNode::GetDataByPath(const std::string & path, T & value) const
{
    bool success = false;

    // Try to find the node specified
    CDataDefNode * pNode = GetChildByPath(path);

    // If it was found, get the data from that node.
    if (pNode != NULL)
    {
        // Attempt to get the data from the node that was retrieved.
        success = pNode->GetData(value);
    }

    return (success);
}


//*****************************************************************************
/*!
 *  \namespace  Parsing
 *
 *  \brief      Global methods that can be used on the CDataDefNode class.
 *
 *****************************************************************************/
namespace Parsing
{




//*****************************************************************************
/*!
 *  \brief  Returns the data value if present or the given default value if
 *          missing
 *
 *  \param  const CDataDefNode * const pNode - The node to be searched.
 *          const string & requestedID - The name of the node being requested.
 *          const T & defaultValue     - The default value to use if the node
 *                                       was not found
 *
 *  \return T - A templatized return type equal to the value of the requested
 *              node or the default value.
 *
 *****************************************************************************/
template<typename T>
T DefaultDataByName(const CDataDefNode * const pNode,
                    const std::string & requestedID,
                    const T & defaultValue)
{
    T value = defaultValue;
    pNode->GetDataByName(requestedID, value);
    return value;
}


//*****************************************************************************
/*!
 *  \brief  Returns the data value if present or the given default value if
 *          missing
 *
 *  \param  const CDataDefNode * const pNode - The node to be searched.
 *          const string & requestedID - The name of the node being requested.
 *          const T & defaultValue     - The default value to use if the node
 *                                       was not found
 *
 *  \return T - A templatized return type equal to the value of the requested
 *              node or the default value.
 *
 *****************************************************************************/
template<typename T>
T DefaultDataByPath(const CDataDefNode * const pNode,
                    const std::string & requestedID,
                    const T & defaultValue)
{
    T value = defaultValue;
    pNode->GetDataByPath(requestedID, value);
    return value;
}


//*****************************************************************************
/*!
 *  \brief  Asserts if the name does not exist or returns the data value.
 *
 *  \param  const CDataDefNode * const pNode - The node to be searched.
 *  \param  const string & requestedID - The name of the node being requested.
 *
 *  \return T - A templatized return type equal to the value of the requested
 *              node.
 *
 *****************************************************************************/
template<typename T>
T RequireDataByName(const CDataDefNode * const pNode,
                    const std::string & requestedID)
{
    T value;

    bool found = pNode->GetDataByName(requestedID, value);

    assert(found);

    return (value);
}


//*****************************************************************************
/*!
 *  \brief  Asserts if the path does not exist or returns the data value.
 *
 *  \param  const CDataDefNode * const pNode - The node to be searched.
 *  \param  const string & path - The path as described in GetChildByPath().
 *
 *  \return T - A templatized return type equal to the value of the requested
 *              node.
 *
 *****************************************************************************/
template<typename T>
T RequireDataByPath(const CDataDefNode * const pNode, const std::string & path)
{
    T value;

    bool found = pNode->GetDataByPath(path, value);

    assert(found);

    return (value);
}

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
                            const std::string & childName);



//*****************************************************************************
/*!
 *  \struct SNodeIdentifierFinder
 *
 *  \brief  For use in STL algorithms to find a node by identifier
 *
 *****************************************************************************/
struct SNodeIdentifierFinder : public std::unary_function<const CDataDefNode *, bool>
{
    SNodeIdentifierFinder(const std::string & name)
    :identifier(name){}

    bool operator()(const CDataDefNode * pNode)
    {
        return identifier == pNode->GetIdentifier();
    }

    const std::string identifier;
};


//*****************************************************************************
/*!
 *  \brief  Since the data def node only supports getting integers (not unsigned ints)
 *          from the config files, we often need to cast them to other types.
 *          This is handy for use in STL algorithms to do that for an entire
 *          container
 *
 *  \param  the source value to be casted
 *
 *  \return the source casted to the destination type
 *
 *****************************************************************************/
template<typename S, typename D>
D StaticCast(S source)
{
    return static_cast<D>(source);
}


} // namespace Parsing

#endif

