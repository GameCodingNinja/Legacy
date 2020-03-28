//*****************************************************************************
/*!
 *  \file   datadefdoublenode.cpp
 *
 *  \brief  The node responsible for storing double definition information.
 *
 *****************************************************************************/

#include "datadefdoublenode.h"


//*****************************************************************************
/*!
 *  \brief  Constructor
 *
 *  \param  const double initialData - the double that this node will represent.
 *
 *****************************************************************************/
CDataDefDoubleNode::CDataDefDoubleNode(const double initialData)
:   dataDouble(initialData)
{
}


//*****************************************************************************
/*!
 *  \brief  Destructor.
 *
 *****************************************************************************/
CDataDefDoubleNode::~CDataDefDoubleNode()
{
}


//*****************************************************************************
/*!
 *  \brief  Clone this node and its subtree recursively.
 *
 *  \return CDataDefNode * - A pointer to a heap allocated cloned subtree.
 *
 *****************************************************************************/
CDataDefNode * CDataDefDoubleNode::Clone() const
{
    // Allcoate the cloned object.
    CDataDefDoubleNode * pClone = new CDataDefDoubleNode(dataDouble);

    // Copy this object into the cloned version.
    pClone->CopyHelper(*this);

    // Return the clone to the caller (caller responsible for deletion).
    return (pClone);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the value associated with this node.
 *
 *  \param  double & value - the destination for the value to be retrieved.
 *
 *  \return bool - overridden from CDataDefNode to return true because this
 *          node contains double information.
 *
 *****************************************************************************/
const bool CDataDefDoubleNode::GetData(double & value) const
{
    value = dataDouble;
    return (true);
}


//*****************************************************************************
/*!
 *  \brief  Output the contents of this node.
 *
 *  \param  const double indentLevel - how far to indent the output (aesthetic).
 *
 *****************************************************************************/
void CDataDefDoubleNode::Dump(const double indentLevel) const
{
    //cerr << string(indentLevel, '\t') << "Double(" << dataDouble << ")" << endl;
}



