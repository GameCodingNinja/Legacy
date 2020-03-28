//*****************************************************************************
/*!
 *  \file   datadefintegernode.h
 *
 *  \brief  The node responsible for storing integer definition information.
 *
 *****************************************************************************/

#ifndef _datadefintegernode_h_
#define _datadefintegernode_h_

#include "datadefnode.h"

//*****************************************************************************
/*!
 *  \class  CDataDefIntegerNode
 *
 *  \brief  Implements the node that handles storage and retrieval related to
 *          integers.
 *
 *****************************************************************************/
class CDataDefIntegerNode : public CDataDefNode
{
public:
    // Constructor.
    CDataDefIntegerNode(const int initialData);

    // Destructor.
    virtual ~CDataDefIntegerNode();

    // Clone this node and its subtree recursively.
    virtual CDataDefNode * Clone() const;

    // Overrides CDataDefNode to enable integer retrieval functionality.
    virtual const bool GetData(int & value) const;

    // Integer specific output functionality.
    virtual void Dump(const unsigned int indentLevel = 0) const;

private:
    // Storage for the integer data.
    int dataInteger;
};

#endif

