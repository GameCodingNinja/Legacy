//*****************************************************************************
/*!
 *  \file   datadeffloatnode.cpp
 *
 *  \brief  The node responsible for storing float definition information.
 *
 *****************************************************************************/

#include "datadeffloatnode.h"


//*****************************************************************************
/*!
 *  \brief  Constructor
 *
 *  \param  const float initialData - the float that this node will represent.
 *
 *****************************************************************************/
CDataDefFloatNode::CDataDefFloatNode(const float initialData)
:   dataFloat(initialData)
{
}


//*****************************************************************************
/*!
 *  \brief  Destructor.
 *
 *****************************************************************************/
CDataDefFloatNode::~CDataDefFloatNode()
{
}


//*****************************************************************************
/*!
 *  \brief  Clone this node and its subtree recursively.
 *
 *  \return CDataDefNode * - A pointer to a heap allocated cloned subtree.
 *
 *****************************************************************************/
CDataDefNode * CDataDefFloatNode::Clone() const
{
    // Allcoate the cloned object.
    CDataDefFloatNode * pClone = new CDataDefFloatNode(dataFloat);

    // Copy this object into the cloned version.
    pClone->CopyHelper(*this);

    // Return the clone to the caller (caller responsible for deletion).
    return (pClone);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the value associated with this node.
 *
 *  \param  float & value - the destination for the value to be retrieved.
 *
 *  \return bool - overridden from CDataDefNode to return true because this
 *          node contains float information.
 *
 *****************************************************************************/
const bool CDataDefFloatNode::GetData(float & value) const
{
    value = dataFloat;
    return (true);
}


//*****************************************************************************
/*!
 *  \brief  Output the contents of this node.
 *
 *  \param  const float indentLevel - how far to indent the output (aesthetic).
 *
 *****************************************************************************/
void CDataDefFloatNode::Dump(const float indentLevel) const
{
    //cerr << string(indentLevel, '\t') << "Float(" << dataFloat << ")" << endl;
}



