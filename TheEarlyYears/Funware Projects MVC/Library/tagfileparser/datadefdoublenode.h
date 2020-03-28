//*****************************************************************************
/*!
 *  \file   datadefdoublenode.h
 *
 *  \brief  The node responsible for storing double definition information.
 *
 *****************************************************************************/

#ifndef _datadefdoublenode_h_
#define _datadefdoublenode_h_

#include "datadefnode.h"

//*****************************************************************************
/*!
 *  \class  CDataDefDoubleNode
 *
 *  \brief  Implements the node that handles storage and retrieval related to
 *          doubles.
 *
 *****************************************************************************/
class CDataDefDoubleNode : public CDataDefNode
{
public:
    // Constructor.
    CDataDefDoubleNode(const double initialData);

    // Destructor.
    virtual ~CDataDefDoubleNode();

    // Clone this node and its subtree recursively.
    virtual CDataDefNode * Clone() const;

    // Overrides CDataDefNode to enable double retrieval functionality.
    virtual const bool GetData(double & value) const;

    // double specific output functionality.
    virtual void Dump(const double indentLevel = 0.0) const;

private:
    // Storage for the double data.
    double dataDouble;
};

#endif

