/************************************************************************
*    FILE NAME:       size.h
*
*    DESCRIPTION:     Size class
*
************************************************************************/  

#ifndef __size_h__
#define __size_h__

#include <common/point.h>

template <class type>
class CSize
{
public:

    // size values. 
    type w, h;

    // Fast init if class varaibles.
    CSize():w(0),h(0)
    {
    }

    // Copy constructor
    CSize( const CSize &size ):w(size.w),h(size.h)
    {
    }

    // Copy constructor
    CSize( type _w, type _h ):w(_w),h(_h)
    {
    }

    /************************************************************************                                                             
    *    desc:  Get functions
    ************************************************************************/
    type GetW() const
    { return w; }

    type GetH() const
    { return h; }


    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param:  CSize & size * size to multiply
    *
    *    return: CSize - multiplied size
    ************************************************************************/
    CSize operator * ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w * size.w;
        tmp.h = h * size.h;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param:  float value - value to multiply
    *
    *    return: CSize - multiplied size
    ************************************************************************/
    CSize operator * ( float value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w * value);
        tmp.h = static_cast<type>(h * value);

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param:  int value - value to multiply
    *
    *    return: CSize - multiplied size
    ************************************************************************/
    CSize operator * ( int value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w * value);
        tmp.h = static_cast<type>(h * value);

        return tmp;
    }


    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param:  const CPoint & point - value to multiply
    *
    *    return: CSize - multiplied size
    ************************************************************************/
    CSize operator * ( const CPoint & point ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w * point.x);
        tmp.h = static_cast<type>(h * point.y);

        return tmp;
    }


    /************************************************************************                                                             
    *    desc:  The division operator 
    *
    *    param:  CSize & size - size to divide
    *
    *    return: CSize - divided size
    ************************************************************************/
    CSize operator / ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w / size.w;
        tmp.h = h / size.h;

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    *
    *    param:  float value - value to divide
    *
    *    return: CSize - divided size
    ************************************************************************/
    CSize operator / ( float value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w / value);
        tmp.h = static_cast<type>(h / value);

        return tmp;
    }

    /************************************************************************                                                             
    *    desc:  The division operator 
    *
    *    param:  int value - value to divide
    *
    *    return: CSize - divided size
    ************************************************************************/
    CSize operator / ( int value ) const
    {
        CSize tmp;
        tmp.w = static_cast<type>(w / value);
        tmp.h = static_cast<type>(h / value);

        return tmp;
    }


    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param:  CSize & size - size to add
    *
    *    return: CSize - added size
    ************************************************************************/
    CSize operator + ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w + size.w;
        tmp.h = h + size.h;

        return tmp;

    }   // operator +


    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    *
    *    param:  CSize & size - size to subtract
    *
    *    return: CSize - subtracted size
    ************************************************************************/
    CSize operator - ( const CSize & size ) const
    {
        CSize tmp;
        tmp.w = w - size.w;
        tmp.h = h - size.h;

        return tmp;

    }   // operator -


    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param:  CSize & size - size to add
    *
    *    return: CSize - added size
    ************************************************************************/
    CSize operator += ( const CSize & size )
    {
        w += size.w;
        h += size.h;

        return *(this);

    }   // operator +=


    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    *
    *    param:  CSize & size - size to subtract
    *
    *    return: CSize - subtracted size
    ************************************************************************/
    CSize operator -= ( const CSize & size )
    {
        w -= size.w;
        h -= size.h;

        return *(this);

    }   // operator -=


    /************************************************************************                                                             
    *    desc:  The equate operator 
    *
    *    param:  CSize & size - size to compair
    *
    *    return: bool
    ************************************************************************/
    bool operator == ( const CSize & size ) const
    {
        if( (w == size.w) && (h == size.h) )
            return true;

        return false;

    }   // operator ==


    /************************************************************************                                                             
    *    desc:  The equate operator 
    *
    *    param:  CSize & size - size to compair
    *
    *    return: bool
    ************************************************************************/
    bool operator != ( const CSize & size ) const
    {
        if( (w == size.w) && (h == size.h) )
            return false;

        return true;

    }   // operator !=


    /************************************************************************                                                             
    *    desc:  Does this size not have any data? 
    *
    *    return: bool
    ************************************************************************/
    bool IsEmpty() const
    {
        return ( w == 0 ) &&  ( h == 0 );
    }

    /************************************************************************                                                             
    *    desc:  Reset the data 
    ************************************************************************/
    void Reset()
    {
        w = 0;
        h = 0;
    }

    /************************************************************************                                                             
    *    desc:  Round out the floating point number 
    ************************************************************************/
    void Round()
    {
        w = floor(w + 0.5);
        h = floor(h + 0.5);
    }
};

#endif  // __size_h__
