//*****************************************************************************
/*!
 *  \file   smartpointers.h
 *
 *  \brief  Smart pointer template classes
 *
 *****************************************************************************/

#ifndef _smart_pointers_h_
#define _smart_pointers_h_

// Boost lib dependencies
#include <boost/noncopyable.hpp>

namespace NSmart
{
    //*****************************************************************************
    /*!
     *  \brief  Smart pointer for file handlers
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    class scoped_filehandle_ptr : boost::noncopyable
    {
    public:

        explicit scoped_filehandle_ptr(T * p = NULL) : ptr(p)
        {}

        ~scoped_filehandle_ptr()
        { freePtr(); }

        void reset(T * p = NULL)
        {
            freePtr();
            ptr = p;
        }

        T* release()
        {
            T *t = ptr;
            ptr = NULL;
            return t;
        }

        bool isNull()
        { return (ptr == NULL); }

        T * get()
        { return ptr; }

        T * operator->()
        { return ptr; }

    private:

        void freePtr()
        {
            if( !isNull() )
            {
                fclose( ptr );
                ptr = NULL;
            }
        }

    private:

        T * ptr;
    };


    //*****************************************************************************
    /*!
     *  \brief  Smart pointer for safe-release Windows handles
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    class scoped_win_handle_ptr : boost::noncopyable
    {
    public:

        // Explicit constructor
        explicit scoped_win_handle_ptr(T * p = INVALID_HANDLE_VALUE) : ptr(p)
        {}

        ~scoped_win_handle_ptr()
        { freePtr(); }

        void reset(T * p = NULL)
        {
            freePtr();
            ptr = p;
        }

        T* release()
        {
            T *t = ptr;
            ptr = NULL;
            return t;
        }

        bool isValid()
        { return (ptr != INVALID_HANDLE_VALUE); }

        bool isNull()
        { return (ptr == NULL); }

        T * get()
        { return ptr; }

        T * operator->()
        { return ptr; }

    private:

        // Free the pointer
        void freePtr()
        {
            if( isValid() )
            {
                CloseHandle( ptr );
                ptr = NULL;
            }
        }

    private:

        T * ptr;
    };


    //*****************************************************************************
    /*!
     *  \brief  Smart pointer for safe-release comm objects, copyable
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    /*template <class T>
    class scoped_com_ptr
    {
    public:

        explicit scoped_com_ptr(T * p = NULL) : ptr(p)
        {}

        ~scoped_com_ptr()
        { freePtr(); }

        // Copy ownership
        scoped_com_ptr & operator = (scoped_com_ptr<T> & source)
        {
            // check for self reference
            if( ptr != source.ptr )
            {
                // flush the existing memory
                freePtr();
            }

            // Copy the pointer
            ptr = source.ptr;

            // Since the ownership gets transferred lets point the source to 0
            source.ptr = NULL;

            return *this;
        }

        void reset(T * p = NULL)
        {
            freePtr();
            ptr = p;
        }

        bool isNull()
        { return (ptr == NULL); }

        T * get()
        { return ptr; }

        T * operator->()
        { return ptr; }

    private:

        void freePtr()
        {
            if( !isNull() )
            {
                ptr->Release();
                ptr = NULL;
            }
        }

    private:

        T * ptr;
    };*/


    //*****************************************************************************
    /*!
     *  \brief  Base smart pointer - NOTE: Do not call this class directly!
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    /*template <class T>
    class scoped_global_handle : boost::noncopyable
    {
    public:

        // Explicit constructor
        explicit scoped_global_handle(T h = NULL) : handle(h), pGlobalData(NULL)
        {}

        // Destructor
        ~scoped_global_handle()
        { freeHandle(); }

        // Get the pointer to the global memory
        LPVOID GetGlobalPtr()
        {
            if( pGlobalData == NULL )
                pGlobalData = GlobalLock( get() );

            return pGlobalData;
        }

        // Reset pointer
        void reset(T h = NULL)
        {
            freeHandle();
            handle = h;
        }

        bool isNull()
        { return (handle == NULL); }

        T & get()
        { return handle; }
    
        T & operator&()
        { return handle; }

    private:

        // Free the handle
        void freeHandle()
        {
            if( pGlobalData != NULL )
            {
                GlobalUnlock( handle );
                pGlobalData = NULL;
            }

            // Free the memory
            if( handle != NULL )
                GlobalFree( handle );
        }

    private:

        T handle;
        LPVOID pGlobalData;
    };*/

}


#endif // _smart_pointers_h_
