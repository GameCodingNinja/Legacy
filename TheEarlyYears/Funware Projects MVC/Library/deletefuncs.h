//*****************************************************************************
/*!
 *  \file   deletefuncs.h
 *
 *  \brief  Delete functions for pointers, vectors m.m.
 *
 *****************************************************************************/

#ifndef _deletefuncs_h
#define _deletefuncs_h

// Standard lib dependencies.
#include <map>
#include <list>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>


//*****************************************************************************
/*!
 *  \brief  Templetized safe-delete function.
 *
 *  \param  T *& ptr - A reference to a pointer of type T
 *
 *****************************************************************************/
template <class T>
inline void Delete(T & ptr)
{
    // Check for NULL-pointer
    if (ptr != NULL)
    {
        // Delete object pointed to.
        delete ptr;
        ptr = NULL;
    }
}


//*****************************************************************************
/*!
 *  \brief  Templetized safe-delete array  function.
 *
 *  \param  T *& ptr - A reference to an array pointer of type T
 *
 *****************************************************************************/
template <class T>
inline void DeleteArray(T & ptr)
{
    // Check for NULL-pointer
    if (ptr != NULL)
    {
        // Delete object pointed to.
        delete [] ptr;
        ptr = NULL;
    }
}


//*****************************************************************************
/*!
 *  \brief  Templetized safe-delete function for vectors of pointers.
 *
 *  \param  vector<T> & obj - A reference to a vector of objects of type T.
 *
 *****************************************************************************/
template <class T>
inline void DeleteVectorPointers(std::vector<T> & obj)
{
    // Loop through the vector and delete all objects pointed to.
    for_each(obj.begin(), obj.end(), Delete<T>);

    // Clear out the vector.
    obj.clear();
}


//*****************************************************************************
/*!
 *  \brief  Templetized safe-delete function for a deque of pointers.
 *
 *  \param  deque<T> & obj - A reference to a deque of objects of type T.
 *
 *****************************************************************************/
template <class T>
inline void DeleteDequePointers(std::deque<T> & obj)
{
    // Loop through the vector and delete all objects pointed to.
    for_each(obj.begin(), obj.end(), Delete<T>);

    // Clear out the deque.
    obj.clear();
}


//*****************************************************************************
/*!
 *  \brief  Templetized safe-delete function for lists of pointers.
 *
 *  \param  list<T> & obj - A reference to a list of objects of type T.
 *
 *****************************************************************************/
template <class T>
inline void DeleteListPointers(std::list<T> & obj)
{
    // Loop through the vector and delete all objects pointed to.
    for_each(obj.begin(), obj.end(), Delete<T>);

    // Clear out the list.
    obj.clear();
}


//*****************************************************************************
/*!
 *  \brief  Templetized safe-delete function for maps where the 'second' is
 *          a pointer to an allocated object.
 *
 *  \param  map<T> & obj - A reference to a map where the 'second' is an object
 *                         of type T.
 *
 *****************************************************************************/
template <class key, class ptr>
inline void DeleteMapPointers(std::map<key, ptr> & obj)
{
    // Loop through the map and delete all objects pointed to.
    for (typename std::map<key, ptr>::iterator iter = obj.begin();
         iter != obj.end();
         ++iter)
    {
        // Delete object pointed to by the dereferenced iterator..
        Delete(iter->second);
    }

    // Clear out the map.
    obj.clear();
}


#endif // _deletefunc_h
