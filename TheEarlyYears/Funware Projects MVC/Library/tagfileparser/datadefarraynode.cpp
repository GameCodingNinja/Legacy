//*****************************************************************************
/*!
 *  \file   datadefarraynode.cpp
 *
 *  \brief  The node responsible for storing array definition information.
 *
 *****************************************************************************/

#include <cassert>
#include <algorithm>
#include <functional>
#include "datadefarraynode.h"
#include "deletefuncs.h"
#include <iostream>


//*****************************************************************************
/*!
 *  \brief  Destructor.
 *
 *****************************************************************************/
CDataDefArrayNode::~CDataDefArrayNode()
{
    DeleteVectorPointers(dataElements);
}


//*****************************************************************************
/*!
 *  \brief  Clone this node and its subtree recursively.
 *
 *  \return CDataDefNode * - A pointer to a heap allocated cloned subtree.
 *
 *****************************************************************************/
CDataDefNode * CDataDefArrayNode::Clone() const
{
    // Allcoate the cloned object.
    CDataDefArrayNode * pClone = new CDataDefArrayNode();

    // Copy this object into the cloned version.
    pClone->CopyHelper(*this);

    // Copy all data elements in the array.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        pClone->dataElements.push_back((*dataIter)->Clone());
    }

    // Return the clone to the caller (caller responsible for deletion).
    return (pClone);
}


//*****************************************************************************
/*!
 *  \brief  Overriden from CDataDefNode to allow the array node to store all
 *          data nodes as elements in the internal array.  This function will
 *          be called for the storage of all primitive types.
 *
 *  \param  CDataDefNode * const pDataNode - the node to be stored as an
 *          element of the data array.
 *
 *****************************************************************************/
void CDataDefArrayNode::SetData(CDataDefNode * pNewData)
{
    assert (pNewData != NULL);

    dataElements.push_back(pNewData);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the vector of integers stored within this array node.
 *          Appends the data to the supplied vector.  Does not clear it.
 *
 *  \param  TIntVector & integers - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TIntVector & integers) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
        int value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            integers.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the vector of floats stored within this array node.
 *          Appends the data to the supplied vector.  Does not clear it.
 *
 *  \param  TFloatVector & floats - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TFloatVector & floats) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
        float value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            floats.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}

//*****************************************************************************
/*!
 *  \brief  Retrieve the vector of integers stored within this array node.
 *          Appends the data to the supplied vector.  Does not clear it.
 *
 *  \param  TDoubleVector & doubles - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TDoubleVector & doubles) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
        double value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            doubles.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the vector of strings stored within this array node.
 *          Appends the data to the supplied vector.  Does not clear it.
 *
 *  \param  TStringVector & strings - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TStringVector & strings) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
		std::string value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            strings.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the children of this node.  Can be thought of conceptually
 *          as an "array".
 *
 *  \param  TDataDefNodeVector & array - the destination node array.
 *
 *  \return bool true - this operation always succeeds.  The return value is
 *          necessary because the other GetData functions have the same
 *          prototpye.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TDataDefNodeVector & array) const
{
    // Copy the data of this node into the supplied storage.
    array = dataElements;

    return (true);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the list of integers stored within this array node.
 *          Appends the data to the supplied list.  Does not clear it.
 *
 *  \param  TIntList & integers - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TIntList & integers) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
        int value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            integers.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the list of integers stored within this array node.
 *          Appends the data to the supplied list.  Does not clear it.
 *
 *  \param  TFloatList & floats - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TFloatList & floats) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
        float value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            floats.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}

//*****************************************************************************
/*!
 *  \brief  Retrieve the list of integers stored within this array node.
 *          Appends the data to the supplied list.  Does not clear it.
 *
 *  \param  TDoubleList & doubles - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TDoubleList & doubles) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
        double value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            doubles.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}


//*****************************************************************************
/*!
 *  \brief  Retrieve the list of strings stored within this array node.
 *          Appends the data to the supplied list.  Does not clear it.
 *
 *  \param  TStringList & strings - the destination storage vector.
 *
 *  \return bool - true if the operation was completely successful.  A return
 *          value of false indicates that at least one child was not of the
 *          required type.
 *
 *****************************************************************************/
const bool CDataDefArrayNode::GetData(TStringList & strings) const
{
    // Default the return value to true.
    bool returnValue = true;

    // Loop through each data element contained in this node of this node.
    for (TDataDefNodeVectorConstIter dataIter = dataElements.begin(); dataIter != dataElements.end(); ++dataIter)
    {
        // Storage for the retrieved value.
		std::string value;
        // Retrieve the value, storing the success of the operation locally.
        bool result = (*dataIter)->GetData(value);

        // Only store the value if the retrieval was valid
        if (result)
        {
            // Store the value.
            strings.push_back(value);
        }
        else
        {
            // Any incorrect retrieval will mean this function returns false.
            returnValue = false;
        }
    }

    return (returnValue);
}


//*****************************************************************************
/*!
 *  \brief  Overridden from CDataDefNode to allow the addition of elements of
 *          an array of compounds.  That is, an array of elements that are not
 *          primitive types.
 *
 *  \param  CDataDefNode * const pCompoundDataElement - the node
 *          representing a compound that is to be stored as an element of the
 *          local data array.
 *
 *****************************************************************************/
void CDataDefArrayNode::AddChild(CDataDefNode * pCompoundDataElement)
{
    assert (pCompoundDataElement != NULL);

    dataElements.push_back(pCompoundDataElement);
}


//*****************************************************************************
/*!
 *  \brief  Output the contents of this node.
 *
 *  \param  const unsigned int indentLevel - how far to indent the output (aesthetic).
 *
 *****************************************************************************/
void CDataDefArrayNode::Dump(const unsigned int indentLevel) const
{
	std::cerr << std::string(indentLevel, '\t') << "Array:" << std::endl;

    for_each(dataElements.begin(), dataElements.end(),
		std::bind2nd(std::mem_fun(&CDataDefNode::Dump), indentLevel + 1));
}

