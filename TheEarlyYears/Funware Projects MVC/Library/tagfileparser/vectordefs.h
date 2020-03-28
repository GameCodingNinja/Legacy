//****************************************************************************
/*!
 *  \file   vectordefs.h
 *
 *  \brief  Common defines used with vectors.
 *
 *  \version
 *      - D Murphy      11/25/2003
 *        Created.
 *
 *  Copyright (c) 2003 WMS Gaming
 *
 ****************************************************************************/
#ifndef _vectordefs_h_
#define _vectordefs_h_

// Standard lib dependencies.
#include <string>
#include <vector>

/*! \brief  Creates a type name for a vector of floats. */
typedef std::vector<float> TFloatVector;

/*! \brief  Creates a type name for an iterator into a vector of floats. */
typedef TFloatVector::iterator TFloatVectorIter;

/*! \brief  Creates a type name for a const iterator into a vector of floats.
 */
typedef TFloatVector::const_iterator TFloatVectorConstIter;


/*! \brief  Creates a type name for a vector of doubles. */
typedef std::vector<double> TDoubleVector;

/*! \brief  Creates a type name for an iterator into a vector of doubles. */
typedef TDoubleVector::iterator TDoubleVectorIter;

/*! \brief  Creates a type name for a const iterator into a vector of doubles.
 */
typedef TDoubleVector::const_iterator TDoubleVectorConstIter;


/*! \brief  Creates a type name for a vector of integers. */
typedef std::vector<int> TIntVector;

/*! \brief  Creates a type name for an iterator into a vector of integers. */
typedef TIntVector::iterator TIntVectorIter;

/*! \brief  Creates a type name for a const iterator into a vector of integers.
 */
typedef TIntVector::const_iterator TIntVectorConstIter;

/*! \brief  Creates a type name for a vector of unsigned integers. */
typedef std::vector<unsigned int> TUintVector;

/*! \brief  Creates a type name for an iterator into a vector of unsigned
 *          integers.
 */
typedef TUintVector::iterator TUintVectorIter;

/*! \brief  Creates a type name for a const iterator into a vector of unsigned
 *          integers.
 */
typedef TUintVector::const_iterator TUintVectorConstIter;

/*! \brief  Creates a type name for a vector of strings. */
typedef std::vector<std::string> TStringVector;

/*! \brief  Creates a type name for an iterator into a vector of strings. */
typedef TStringVector::iterator TStringVectorIter;

/*! \brief  Creates a type name for a const iterator into a vector of strings.
 */
typedef TStringVector::const_iterator TStringVectorConstIter;

/*! \brief  Creates a type name for a vector of bools.
            char is used because of implementation problems. */
typedef std::vector<char> TBoolVector;

/*! \brief  Creates a type name for an iterator into a vector of booleans. */
typedef TBoolVector::iterator TBoolVectorIter;

/*! \brief  Creates a type name for a const iterator into a vector of booleans.
 */
typedef TBoolVector::const_iterator TBoolVectorConstIter;


#endif // _vectordefs_h_
