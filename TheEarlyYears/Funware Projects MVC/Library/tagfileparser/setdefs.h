//****************************************************************************
/*!
 *  \file   setdefs.h
 *
 *  \brief  Common defines used with sets.
 *
 *  \version
 *      - D Murphy      11/25/2003
 *        Created.
 *
 *  Copyright (c) 2003 WMS Gaming
 *
 ****************************************************************************/
#ifndef _setdefs_h_
#define _setdefs_h_

// Standard lib dependencies.
#include <set>
#include <string>


/*! \brief  Creates a type name for a set of floats. */
typedef std::set<float> TFloatSet;

/*! \brief  Creates a type name for an iterator into a set of floats. */
typedef TFloatSet::iterator TFloatSetIter;

/*! \brief  Creates a type name for a const iterator into a set of floats. */
typedef TFloatSet::const_iterator TFloatSetConstIter;


/*! \brief  Creates a type name for a set of doubles. */
typedef std::set<double> TDoubleSet;

/*! \brief  Creates a type name for an iterator into a set of doubles. */
typedef TDoubleSet::iterator TDoubleSetIter;

/*! \brief  Creates a type name for a const iterator into a set of doubles. */
typedef TDoubleSet::const_iterator TDoubleSetConstIter;


/*! \brief  Creates a type name for a set of integers. */
typedef std::set<int> TIntSet;

/*! \brief  Creates a type name for an iterator into a set of integers. */
typedef TIntSet::iterator TIntSetIter;

/*! \brief  Creates a type name for a const iterator into a set of integers. */
typedef TIntSet::const_iterator TIntSetConstIter;

/*! \brief  Creates a type name for a set of unsigned integers. */
typedef std::set<unsigned int> TUintSet;

/*! \brief  Creates a type name for an iterator into a set of unsigned
 *  integers. */
typedef TUintSet::iterator TUintSetIter;

/*! \brief  Creates a type name for a const iterator into a set of unsigned
 *  integers. */
typedef TUintSet::const_iterator TUintSetConstIter;

/*! \brief  Creates a type name for a set of strings. */
typedef std::set<std::string> TStringSet;

/*! \brief  Creates a type name for an iterator into a set of strings. */
typedef TStringSet::iterator TStringSetIter;

/*! \brief  Creates a type name for a const iterator into a set of strings. */
typedef TStringSet::const_iterator TStringSetConstIter;

#endif // _setdefs_h_
