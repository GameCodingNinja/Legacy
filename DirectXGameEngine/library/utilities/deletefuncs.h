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
#include <unordered_map>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>

namespace NDelFunc
{
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
     *  \brief  Templetized safe-close function.
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    inline void CloseWinHandle(T & ptr)
    {
        // Check for NULL-pointer
        if (ptr != NULL)
        {
            // Delete object pointed to.
            CloseHandle( ptr );
            ptr = NULL;
        }
    }


    //*****************************************************************************
    /*!
     *  \brief  Templetized safe-release comm function.
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    inline void Release(T & ptr)
    {
        // Check for NULL-pointer
        if (ptr != NULL)
        {
            // Release comm object.
            ptr->Release();
            ptr = NULL;
        }
    }


    //*****************************************************************************
    /*!
     *  \brief  Close file.
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    inline void CloseFileHandle(T & ptr)
    {
        // Check for NULL-pointer
        if (ptr != NULL)
        {
            // Close file pointed to.
            fclose(ptr);
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
     *  \brief  Templetized safe-release function for DirectX vector pointers.
     *
     *  \param  vector<T> & obj - A reference to a vector of objects of type T.
     *
     *****************************************************************************/
    template <class T>
    inline void ReleaseVectorPointers(std::vector<T> & obj)
    {
        // Loop through the list and delete all objects pointed to.
        for( std::vector<T>::iterator iter = obj.begin(); 
             iter != obj.end(); ++iter )
        {
            // Have directX release the data
            Release( *iter );
        }

        // Clear out the vector.
        obj.clear();
    }


    //*****************************************************************************
    /*!
     *  \brief  Templetized safe-delete function for vectors of pointers.
     *
     *  \param  vector<T> & obj - A reference to a vector of objects of type T.
     *
     *****************************************************************************/
    template <class T>
    inline void DeleteVectorPointerArrays(std::vector<T> & obj)
    {
        // Loop through the vector and delete all objects pointed to.
        for_each(obj.begin(), obj.end(), DeleteArray<T>);

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
        // Loop through the list and delete all objects pointed to.
        for_each(obj.begin(), obj.end(), Delete<T>);

        // Clear out the list.
        obj.clear();
    }


    //*****************************************************************************
    /*!
     *  \brief  Templetized safe-delete function for sets of pointers.
     *
     *			NOTE: Sets are a very unique beast and can't be deleted
     *                in the same fashion as lists, vectors, Deques etc.
     *
     *  \param  list<T> & obj - A reference to a list of objects of type T.
     *
     *****************************************************************************/
    template <class T>
    inline void DeleteSetPointers(std::set<T> & obj)
    {
        // Loop through the list and delete all objects pointed to.
        for( std::set<T>::iterator it = obj.begin(); 
             it != obj.end(); ++it )
        {
            delete (*it);
        }

        // Clear out the set.
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
    inline void DeleteUnorderedMapPointers(std::unordered_map<key, ptr> & obj)
    {
        // Loop through the map and delete all objects pointed to.
        for (typename std::unordered_map<key, ptr>::iterator iter = obj.begin();
             iter != obj.end();
             ++iter)
        {
            // Delete object pointed to by the dereferenced iterator..
            Delete(iter->second);
        }

        // Clear out the map.
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
    inline void ReleaseMapPointers(std::map<key, ptr> & obj)
    {
        // Loop through the map and delete all objects pointed to.
        for (typename std::map<key, ptr>::iterator iter = obj.begin();
             iter != obj.end();
             ++iter)
        {
            // Have directX release the data
            Release( iter->second );
        }

        // Clear out the map.
        obj.clear();
    }
}

#endif // _deletefunc_h
