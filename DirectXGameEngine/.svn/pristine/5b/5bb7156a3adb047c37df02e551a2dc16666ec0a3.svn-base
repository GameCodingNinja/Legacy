/************************************************************************
*    FILE NAME:       rect.h
*
*    DESCRIPTION:     Rect class
*
************************************************************************/  

#ifndef __rect_h__
#define __rect_h__

template <class type>
class CRect
{
public:

    // size values.
    type x1, y1, x2, y2;

    // Fast init if class variables.
    CRect():x1(0),y1(0),x2(0),y2(0)
    { }

    // Copy constructor
    CRect( const CRect &rect ):x1(rect.x1),y1(rect.y1),x2(rect.x2),y2(rect.y2)
    { }

    // Copy constructor
    CRect( type _x1, type _y1, type _x2, type _y2 ):x1(_x1),y1(_y1),x2(_x2),y2(_y2)
    { }

    /************************************************************************                                                             
    *    desc:  Get functions
    ************************************************************************/
    type GetX1() const
    { return x1; }

    type GetY1() const
    { return y1; }

    type GetX2() const
    { return x2; }

    type GetY2() const
    { return y2; }

    // Sets the rect's x and y value based on an index and three counts
    void SetRectXY( int index, int columnCount, int rowCount, type textureHeight, type textureWidth )
    {
        int heightIndex = 0;
        int widthIndex = 0;

        for( int i = 0; i < rowCount; ++i )
        {
            if( columnCount <= index )
            {
                index -= columnCount;
            }
            else
            {
                heightIndex = i;
                widthIndex = index;
                break;
            }
        }

        type frameWidth = textureWidth / columnCount;
        type frameHeight = textureHeight / rowCount ;

        x1 = widthIndex * frameWidth;
        y1 = heightIndex * frameHeight;

        x2 = x1 + frameWidth;
        y2 = y1 + frameHeight;
    }

    /************************************************************************                                                             
    *    desc:  Does this size not have any data? 
    *
    *    return: bool
    ************************************************************************/
    bool IsEmpty() const
    {
        return ( x == 0 ) &&  ( y == 0 ) && ( w == 0 ) &&  ( h == 0 );
    }
};

#endif  // __rect_h__
