//*****************************************************************************
/*!
 *  \file   datadefstringnode.h
 *
 *  \brief  The node responsible for storing string definition information.
 *
 *****************************************************************************/

#ifndef _datadefstringnode_h_
#define _datadefstringnode_h_

#include "datadefnode.h"

//*****************************************************************************
/*!
 *  \class  CDataDefStringNode
 *
 *  \brief  Implements the node that handles storage and retrieval related to
 *          strings.
 *
 *****************************************************************************/
class CDataDefStringNode : public CDataDefNode
{
public:
    // Constructor.
    CDataDefStringNode(const std::string & initialData);

    // Destructor.
    virtual ~CDataDefStringNode();

    // Clone this node and its subtree recursively.
    virtual CDataDefNode * Clone() const;

    // Overrides CDataDefNode to enable string retrieval functionality.
    virtual const bool GetData(std::string & value) const;

    // String specific output functionality.
    virtual void Dump(const unsigned int indentLevel = 0) const;

private:
    // storage for the string data.
    std::string dataString;
};

#endif

