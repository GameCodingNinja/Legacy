//*****************************************************************************
/*!
 *  \file   datadefintegernode.cpp
 *
 *  \brief  The node responsible for storing integer definition information.
 *
 *****************************************************************************/

#include "datadefintegernode.h"
#include <iostream>


//*****************************************************************************
/*!
 *  \brief  Constructor
 *
 *  \param  const int initialData - the integer that this node will represent.
 *
 *****************************************************************************/
CDataDefIntegerNode::CDataDefIntegerNode(const int initialData)
:   dataInteger(initialData)
{
}


//*****************************************************************************
/*!
 *  \brief  Destructor.
 *
 *****************************************************************************/
CDataDefIntegerNode::~CDataDefIntegerNode()
{
}


//*****************************************************************************
/*!
 *  \brief  Clone this node and its subtree recursively.
 *
 *  \return CDataDefNode * - A pointer to a heap allocated cloned subtree.
 *
 *****************************************************************************/
CDataDefNode * CDataDefIntegerNode::Clone() const
{
    // Allcoate the cloned object.
    CDataDefIntegerNode * pClone = new CDataDefIntegerNode(dataInteger);

    // Copy this object into the cloned version.
    pClone->CopyHelper(*this);

    // Return the clone to the caller (caller responsible for deletion).
    return (pClone);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the value associated with this node.
 *
 *  \param  int & value - the destination for the value to be retrieved.
 *
 *  \return bool - overridden from CDataDefNode to return true because this
 *          node contains integer information.
 *
 *****************************************************************************/
const bool CDataDefIntegerNode::GetData(int & value) const
{
    value = dataInteger;
    return (true);
}


//*****************************************************************************
/*!
 *  \brief  Output the contents of this node.
 *
 *  \param  const unsigned int indentLevel - how far to indent the output (aesthetic).
 *
 *****************************************************************************/
void CDataDefIntegerNode::Dump(const unsigned int indentLevel) const
{
	std::cerr << std::string(indentLevel, '\t') << "Int(" << dataInteger << ")" << std::endl;
}



