//*****************************************************************************
/*!
 *  \file   datadefstringnode.cpp
 *
 *  \brief  The node responsible for storing string definition information.
 *
 *****************************************************************************/

#include "datadefstringnode.h"
#include <iostream>


//*****************************************************************************
/*!
 *  \brief  Constructor
 *
 *  \param  const string & initialData - the startup value for the internally
 *          stored string information that this node represents.
 *
 *****************************************************************************/
CDataDefStringNode::CDataDefStringNode(const std::string & initialData)
:   dataString(initialData)
{
}


//*****************************************************************************
/*!
 *  \brief  Destructor.
 *
 *****************************************************************************/
CDataDefStringNode::~CDataDefStringNode()
{
}


//*****************************************************************************
/*!
 *  \brief  Clone this node and its subtree recursively.
 *
 *  \return CDataDefNode * - A pointer to a heap allocated cloned subtree.
 *
 *****************************************************************************/
CDataDefNode * CDataDefStringNode::Clone() const
{
    // Allcoate the cloned object.
    CDataDefStringNode * pClone = new CDataDefStringNode(dataString);

    // Copy this object into the cloned version.
    pClone->CopyHelper(*this);

    // Return the clone to the caller (caller responsible for deletion).
    return (pClone);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the value associated with this node.
 *
 *  \param  string & value - the destination for the value to be retrieved.
 *
 *  \return bool - overridden from CDataDefNode to return true because this
 *          node contains string information.
 *
 *****************************************************************************/
const bool CDataDefStringNode::GetData(std::string & value) const
{
    value = dataString;
    return (true);
}


//*****************************************************************************
/*!
 *  \brief  Output the contents of this node.
 *
 *  \param  unsigned int indentLevel - how far to indent the output (aesthetic).
 *
 *****************************************************************************/
void CDataDefStringNode::Dump(unsigned int indentLevel) const
{
    std::cerr << std::string(indentLevel, '\t') << "String(" << dataString << ")" << std::endl;
}

