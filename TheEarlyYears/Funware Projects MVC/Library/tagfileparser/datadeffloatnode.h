//*****************************************************************************
/*!
 *  \file   datadeffloatnode.h
 *
 *  \brief  The node responsible for storing float definition information.
 *
 *****************************************************************************/

#ifndef _datadeffloatnode_h_
#define _datadeffloatnode_h_

#include "datadefnode.h"

//*****************************************************************************
/*!
 *  \class  CDataDefFloatNode
 *
 *  \brief  Implements the node that handles storage and retrieval related to
 *          floats.
 *
 *****************************************************************************/
class CDataDefFloatNode : public CDataDefNode
{
public:
    // Constructor.
    CDataDefFloatNode(const float initialData);

    // Destructor.
    virtual ~CDataDefFloatNode();

    // Clone this node and its subtree recursively.
    virtual CDataDefNode * Clone() const;

    // Overrides CDataDefNode to enable float retrieval functionality.
    virtual const bool GetData(float & value) const;

    // float specific output functionality.
    virtual void Dump(const float indentLevel = 0.0) const;

private:
    // Storage for the float data.
    float dataFloat;
};

#endif

