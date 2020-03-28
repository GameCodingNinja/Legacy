/************************************************************************
*    FILE NAME:       color.h
*
*    DESCRIPTION:     Color class
************************************************************************/

#ifndef __color_h__
#define __color_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/format.hpp>

// DirectX lib dependencies
#include <d3dx9.h>

// Game lib dependencies
#include <common/defs.h>

const int COLOR_MAX = 1;
const int COLOR_MIN = 0;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
#pragma warning(disable : 4244)

/************************************************************************
*	Class: CBoolColor
*
*	Description: Class for remembering what rgba channels need to be set
************************************************************************/
class CBoolColor
{
public:

    CBoolColor() : r(false),g(false),b(false),a(false)
    {}

    bool r, g, b, a;
};


/************************************************************************
*	Class: CColor
*
*	Description: Class for holding color values
************************************************************************/
class CColor
{
public:

    CColor():r(1),g(1),b(1),a(1){};
    CColor(float _r, float _g, float _b, float _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    };

    float r, g, b, a;
    

    /************************************************************************                                                             
    *    desc:  Get functions
    ************************************************************************/
    float GetR() const
    { return r; }

    float GetG() const
    { return g; }

    float GetB() const
    { return b; }

    float GetA() const
    { return a; }

    /************************************************************************                                                             
    *    desc:  Clear the values
    ************************************************************************/
    void Clear()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    /************************************************************************                                                             
    *    desc:  Reset the values back to 1
    ************************************************************************/
    void Reset()
    {
        r = 1;
        g = 1;
        b = 1;
        a = 1;
    }

    /************************************************************************                                                             
    *    desc:	Returns the color in direct x vector form 
    *
    *    ret:	D3DXVECTOR4 - true or false
    ************************************************************************/
    D3DXVECTOR4 operator () () const
    {
        D3DXVECTOR4 color;
        color.x = r;
        color.y = g;
        color.z = b;
        color.w = a;

        return color;
    }

    /************************************************************************                                                             
    *    desc:  The equality operator 
    *
    *    param: CColor & color - color to equate
    *
    *    ret:	bool - true or false
    ************************************************************************/
    bool operator == ( const CColor & color ) const
    {
        if( (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a) )
        {
            return true;
        }
        return false;

    }   // operator ==

    /************************************************************************                                                             
    *    desc:  The inequality operator 
    *
    *    param: CColor & color - color to check
    *
    *    ret:	bool - true or false
    ************************************************************************/
    bool operator != ( const CColor & color ) const
    {
        if( ( color.r != r ) || ( color.g != g ) || ( color.b != b ) || ( color.a != a ) )
        {
            return true;
        }
        return false;

    }   // operator !=


    /********************************************************************************************                                                             
    *    SUBTRACTION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    *
    *    param: CColor & color - color to subtract
    *
    *    ret:	CColor - subtracted color
    ************************************************************************/
    CColor operator - ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r - color.r;
        tmp.g = g - color.g;
        tmp.b = b - color.b;
        tmp.a = a - color.a;

        //Confine(tmp);

        return tmp;

    }   // operator -

    /************************************************************************                                                             
    *    desc:  The subtraction operator 
    *
    *    param: float * value - value to subtract
    *
    *    ret:	CColor - subtracted color
    ************************************************************************/
    CColor operator - ( float value ) const
    {
        CColor tmp;
        tmp.r = r - value;
        tmp.g = g - value;
        tmp.b = b - value;
        tmp.a = a - value;

        //Confine(tmp);

        return tmp;

    }   // operator -

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param:  CColor & color - color to add
    *
    *    return: CColor - added color
    ************************************************************************/
    CColor operator -= ( const CColor & color )
    {
        r -= color.r;
        g -= color.g;
        b -= color.b;
        a -= color.a;

        //Confine();

        return *this;

    }   // operator -=

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param: float value - value to add
    *
    *    ret:	CColor - added color
    ************************************************************************/
    CColor operator -= ( float value )
    {
        r -= value;
        g -= value;
        b -= value;
        a -= value;

        //Confine();

        return *this;

    }   // operator -=


    /********************************************************************************************                                                             
    *    ADDITION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param: CColor & color - color to add
    *
    *    ret:	CColor - added color
    ************************************************************************/
    CColor operator + ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r + color.r;
        tmp.g = g + color.g;
        tmp.b = b + color.b;
        tmp.a = a + color.a;

        //Confine(tmp);

        return tmp;

    }   // operator +

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param: float value - value to add
    *
    *    ret:	CColor - added color
    ************************************************************************/
    CColor operator + ( float value ) const
    {
        CColor tmp;
        tmp.r = r + value;
        tmp.g = g + value;
        tmp.b = b + value;
        tmp.a = a + value;

        //Confine(tmp);

        return tmp;

    }   // operator +

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param: CColor & color - color to add
    *
    *    ret:	CColor - added color
    ************************************************************************/
    CColor operator += ( const CColor & color )
    {
        r += color.r;
        g += color.g;
        b += color.b;
        a += color.a;

        //Confine();

        return *this;

    }   // operator +=

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param: float value - value to add
    *
    *    ret:	CColor - added color
    ************************************************************************/
    CColor operator += ( float value )
    {
        r += value;
        g += value;
        b += value;
        a += value;

        //Confine();

        return *this;

    }   // operator +=


    /********************************************************************************************                                                             
    *    DIVISION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The division operator 
    *
    *    param: CColor & color - color to divide by
    *
    *    ret:	CColor - divided by color
    ************************************************************************/
    CColor operator / ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r / color.r;
        tmp.g = g / color.g;
        tmp.b = b / color.b;
        tmp.a = a / color.a;

        //Confine(tmp);

        return tmp;

    }   // operator /

    /************************************************************************                                                             
    *    desc:  The addition operator 
    *
    *    param: float value - value to divide by
    *
    *    ret:	CColor - divided by color
    ************************************************************************/
    CColor operator / ( float value ) const
    {
        CColor tmp;
        tmp.r = r / value;
        tmp.g = g / value;
        tmp.b = b / value;
        tmp.a = a / value;

        //Confine(tmp);

        return tmp;

    }   // operator /

    /************************************************************************                                                             
    *    desc:  The division operator 
    *
    *    param: CColor & color / color to divide
    *
    *    ret:	CColor - divided color
    ************************************************************************/
    CColor operator /= ( const CColor & color )
    {
        r /= color.r;
        g /= color.g;
        b /= color.b;
        a /= color.a;

        //Confine();

        return *this;

    }   // operator *=

    /************************************************************************                                                             
    *    desc:  The division operator 
    *
    *    param: float value / value to divide
    *
    *    ret:	CColor - divided color
    ************************************************************************/
    CColor operator /= ( float value )
    {
        r /= value;
        g /= value;
        b /= value;
        a /= value;

        //Confine();

        return *this;

    }   // operator *=


    /********************************************************************************************                                                             
    *    MULTIPLICATION
    ********************************************************************************************/

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param: CColor & color * color to multiply
    *
    *    ret:	CColor - multiplied color
    ************************************************************************/
    CColor operator * ( const CColor & color ) const
    {
        CColor tmp;
        tmp.r = r * color.r;
        tmp.g = g * color.g;
        tmp.b = b * color.b;
        tmp.a = a * color.a;

        //Confine(tmp);

        return tmp;

    }   // operator *

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param: float value - value to multiply
    *
    *    ret:	CColor - multiplied color
    ************************************************************************/
    CColor operator * ( float value ) const
    {
        CColor tmp;
        tmp.r = r * value;
        tmp.g = g * value;
        tmp.b = b * value;
        tmp.a = a * value;

        //Confine(tmp);

        return tmp;

    }   // operator *

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param: CColor & point - color to multiply
    *
    *    ret:	CColor - multiplied color
    ************************************************************************/
    CColor operator *= ( const CColor & color )
    {
        r *= color.r;
        g *= color.g;
        b *= color.b;
        a *= color.a;

        //Confine();

        return *this;

    }   // operator *=

    /************************************************************************                                                             
    *    desc:  The multiplication operator 
    *
    *    param: float value * value to multiply
    *
    *    ret:	CColor - multiplied color
    ************************************************************************/
    CColor operator *= ( float value )
    {
        r *= value;
        g *= value;
        b *= value;
        a *= value;

        //Confine();

        return *this;

    }   // operator *=


    /************************************************************************                                                             
    *    desc:  Set the point data
    ************************************************************************/
    void Set( float _r, float _g, float _b, float _a )
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;

        //Confine();

    }   // Set

    /************************************************************************                                                             
    *    desc:  Confine to a value between the max and min value
    ************************************************************************/
    void Confine()
    {
        // Confine red value
        if( r > COLOR_MAX )
        {
            r = COLOR_MAX;
        }
        else if( r < COLOR_MIN )
        {
            r = COLOR_MIN;
        }

        // Confine green value
        if( g > COLOR_MAX )
        {
            g = COLOR_MAX;
        }
        else if( g < COLOR_MIN )
        {
            g = COLOR_MIN;
        }

        // Confine blue value
        if( b > COLOR_MAX )
        {
            b = COLOR_MAX;
        }
        else if( b < COLOR_MIN )
        {
            b = COLOR_MIN;
        }

        // Confine alpha value
        if( a > COLOR_MAX )
        {
            a = COLOR_MAX;
        }
        else if( a < COLOR_MIN )
        {
            a = COLOR_MIN;
        }

    }	// Confine

    /************************************************************************                                                             
    *    desc:  Confine to a value between the max and min value
    ************************************************************************/
    void Confine( CColor & color )
    {
        // Confine red value
        if( color.r > COLOR_MAX )
        {
            color.r = COLOR_MAX;
        }
        else if( color.r < COLOR_MIN )
        {
            color.r = COLOR_MIN;
        }

        // Confine green value
        if( color.g > COLOR_MAX )
        {
            color.g = COLOR_MAX;
        }
        else if( color.g < COLOR_MIN )
        {
            color.g = COLOR_MIN;
        }

        // Confine blue value
        if( color.b > COLOR_MAX )
        {
            color.b = COLOR_MAX;
        }
        else if( color.b < COLOR_MIN )
        {
            color.b = COLOR_MIN;
        }

        // Confine alpha value
        if( color.a > COLOR_MAX )
        {
            color.a = COLOR_MAX;
        }
        else if( color.a < COLOR_MIN )
        {
            color.a = COLOR_MIN;
        }

    }   // Confine


    /************************************************************************                                                             
    *    desc:  HSV transformation 
    *
    *    param: float hue - hue shift (in degrees)
    *    param: float sat - saturation multiplier (scalar)
    *    param: float val - value multiplier (scalar)
    *
    *    ret:	CColor - adjusted color
    ************************************************************************/
    CColor TransformHSV( float hue, float sat, float val )
    {
        float VSU = val * sat * cos(hue * DEG_TO_RAD);
        float VSW = val * sat * sin(hue * DEG_TO_RAD);

        CColor ret(r,g,b,a);

        ret.r = (.299 * val + .701 * VSU + .168 * VSW) * r
              + (.587 * val - .587 * VSU + .330 * VSW) * g
              + (.114 * val - .114 * VSU - .497 * VSW) * b;
        ret.g = (.299 * val - .299 * VSU - .328 * VSW) * r
              + (.587 * val + .413 * VSU + .035 * VSW) * g
              + (.114 * val - .114 * VSU + .292 * VSW) * b;
        ret.b = (.299 * val - .3 * VSU + 1.25 * VSW) * r
              + (.587 * val - .588 * VSU - 1.05 * VSW) * g
              + (.114 * val + .886 * VSU -.203 * VSW) * b;

        return ret;
    }


    /************************************************************************                                                             
    *    desc:  Get a string of the color's data
    *
    *	 ret:	string - string of the data
    ************************************************************************/
    std::string Dump() const
    {
        std::string printout = "Color( " +
                               str( boost::format("%d") % r ) + "r, " +
                               str( boost::format("%d") % g ) + "g, " +
                               str( boost::format("%d") % b ) + "b, " +
                               str( boost::format("%d") % a ) + "a )";

        return printout;

    }	// Dump

};


#endif  // __color_h__