
/************************************************************************
*
*    PRODUCT:         zOrder list
*
*    FILE NAME:       zOrderList.h
*
*    DESCRIPTION:     Object for holding lists of indexes and z orders
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __ZORDER_LIST_H__
#define __ZORDER_LIST_H__ 

#include "TObjList.h"        // Parent object class


/************************************************************************
*
*                        Storyboard list object
*
************************************************************************/


/***************************************************************
*
*  TStoryBoard - Structure to handle game animation data.
*                Holds all the relevant animation data for a 
*                frame of animation.
*
****************************************************************/
struct SZOrder    // Structure size = 412 bytes
{
    int index;      // Animation Index
    int zOrder;     // Zorder

    SZOrder()
    {
        index = 0;
        zOrder = 0; 
    };
};
typedef SZOrder * PSZOrder;


class CzOrderList : public TObjList
{

public:
    // Common Functions

    // Constructor
    CzOrderList();
    // Destructor - Put all your cleanup code here
    ~CzOrderList();
    // Dummy function. Not used
    virtual BOOL Add(){ return FALSE; };
    // Add a string
    virtual BOOL Add( int value );
    // Frees the pointer from the object class
    void Free();
    // Sort all the items in the list by the Z Order value
    void SortByZorder();

    // Get the index
    int GetIndex( int Index ){ return ( Count() ? PSZOrder(GetObj( Index ))->index : 0); };
    // Get the Z Order
    int GetZorder( int Index ){ return ( Count() ? PSZOrder(GetObj( Index ))->zOrder : 0); };

    // Set the X Offset
    void SetIndex( int Index, int Value ){ if( Count() ) PSZOrder(GetObj( Index ))->index = Value; };
    // Set the Y Offset
    void SetZorder( int Index, int Value ){ if( Count() ) PSZOrder(GetObj( Index ))->zOrder = Value; };
};

typedef CzOrderList *PCzOrderList;


#endif  /* __ZORDER_LIST_H__ */