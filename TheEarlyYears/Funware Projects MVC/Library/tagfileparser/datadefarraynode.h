//*****************************************************************************
/*!
 *  \file   datadefarraynode.h
 *
 *  \brief  The node responsible for storing array definition information.
 *
 *****************************************************************************/

#ifndef _datadefarraynode_h_
#define _datadefarraynode_h_

#include "datadefnode.h"

//*****************************************************************************
/*!
 *  \class  CDataDefArrayNode
 *
 *  \brief  Implements the node that handles storage and retrieval related to
 *          arrays.
 *
 *****************************************************************************/
class CDataDefArrayNode : public CDataDefNode
{
public:
    // Uses compiler generated default constructor.

    // Destructor.
    virtual ~CDataDefArrayNode();

    // Clone this node and its subtree recursively.
    virtual CDataDefNode * Clone() const;

    // Functionality to add to the array's data element storage.
    virtual void SetData(CDataDefNode * pNewData);

    // Overridden from CDataDefNode to provide multiple integer retrieval.
    virtual const bool GetData(TIntVector & integers) const;

    // Overridden from CDataDefNode to provide multiple float retrieval.
    virtual const bool GetData(TFloatVector & floats) const;

    // Overridden from CDataDefNode to provide multiple double retrieval.
    virtual const bool GetData(TDoubleVector & doubles) const;

    // Overridden from CDataDefNode to provide multiple string retrieval.
    virtual const bool GetData(TStringVector & strings) const;

    // Overridden from CDataDefNode to provide array retrieval.
    virtual const bool GetData(TDataDefNodeVector & array) const;

    // Overridden from CDataDefNode to provide multiple integer retrieval.
    virtual const bool GetData(TIntList & integers) const;

    // Overridden from CDataDefNode to provide multiple float retrieval.
    virtual const bool GetData(TFloatList & floats) const;

    // Overridden from CDataDefNode to provide multiple double retrieval.
    virtual const bool GetData(TDoubleList & doubles) const;

    // Overridden from CDataDefNode to provide multiple string retrieval.
    virtual const bool GetData(TStringList & strings) const;

    // Store the supplied node as a child of this node.
    virtual void AddChild(CDataDefNode * pCompoundDataElement);

    // Overridden from CDataDefNode to output object info.
    virtual void Dump(const unsigned int indentLevel = 0) const;

private:
    // Storage for the vector of array elements.
    TDataDefNodeVector dataElements;
};

#endif

