//****************************************************************************
/*!
 *  \file   wmslistdefs.h
 *
 *  \brief  Common defines used with lists.
 *
 *  \version
 *      - D Murphy      11/25/2003
 *        Created.
 *
 *  Copyright (c) 2003 WMS Gaming
 *
 ****************************************************************************/
#ifndef _listdefs_h_
#define _istdefs_h_


// Standard lib dependencies.
#include <list>
#include <string>

/*! \brief  Creates a type name for a list of floats. */
typedef std::list<float> TFloatList;

/*! \brief   Creates a type name for an iterator into a list of floats. */
typedef TFloatList::iterator TFloatListIter;

/*! \brief   Creates a type name for a const iterator into a list of floats.
 */
typedef TFloatList::const_iterator TFloatListConstIter;


/*! \brief  Creates a type name for a list of doubles. */
typedef std::list<double> TDoubleList;

/*! \brief   Creates a type name for an iterator into a list of doubles. */
typedef TDoubleList::iterator TDoubleListIter;

/*! \brief   Creates a type name for a const iterator into a list of doubles.
 */
typedef TDoubleList::const_iterator TDoubleListConstIter;


/*! \brief  Creates a type name for a list of integers. */
typedef std::list<int> TIntList;

/*! \brief   Creates a type name for an iterator into a list of integers. */
typedef TIntList::iterator TIntListIter;

/*! \brief   Creates a type name for a const iterator into a list of integers.
 */
typedef TIntList::const_iterator TIntListConstIter;

/*! \brief  Creates a type name for a list of unsigned integers. */
typedef std::list<unsigned int> TUintList;

/*! \brief  Creates a type name for an iterator into a list of unsigned
 *          integers.
 */
typedef TUintList::iterator TUintListIter;

/*! \brief  Creates a type name for a const iterator into a list of unsigned
 *          integers.
 */
typedef TUintList::const_iterator TUintListConstIter;

/*! \brief  Creates a type name for a list of strings. */
typedef std::list<std::string> TStringList;

/*! \brief   Creates a type name for an iterator into a list of strings. */
typedef TStringList::iterator TStringListIter;

/*! \brief   Creates a type name for a const iterator into a list of strings.*/
typedef TStringList::const_iterator TStringListConstIter;

#endif // _listdefs_h_
