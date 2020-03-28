/************************************************************************
*    FILE NAME:       xmlparsehelper.cpp
*
*    DESCRIPTION:     General xml parse helper functions
************************************************************************/            

// Physical component dependency
#include <utilities/xmlparsehelper.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

namespace NParseHelper
{
    /************************************************************************
    *    desc:  Load the 2d vertex
    *
    *    param: node - passed in node
    ************************************************************************/
    CVertex2D LoadVertex2d( const XMLNode & node )
    {
        CVertex2D vert2D;
    
        if( !node.isEmpty() )
        {
            if( node.isAttributeSet( "x" ) )
                vert2D.vert.x = atof( node.getAttribute( "x" ) );

            if( node.isAttributeSet( "y" ) )
                vert2D.vert.y = atof( node.getAttribute( "y" ) );

            if( node.isAttributeSet( "z" ) )
                vert2D.vert.z = atof( node.getAttribute( "z" ) );

            if( node.isAttributeSet( "u" ) )
                vert2D.uv.u = atof( node.getAttribute( "u" ) );

            if( node.isAttributeSet( "v" ) )
                vert2D.uv.v = atof( node.getAttribute( "v" ) );
        }

        return vert2D;

    }	// LoadVertex2d


    /************************************************************************
    *    desc:  Load the position
    *
    *    param: node - passed in node
    ************************************************************************/
    CWorldPoint LoadPosition( const XMLNode & node )
    {
        CWorldPoint point;

        XMLNode positionNode = node.getChildNode( "position" );
    
        if( !positionNode.isEmpty() )
        {
            if( positionNode.isAttributeSet( "xi" ) )
                point.x.i = atoi( positionNode.getAttribute( "xi" ) );

            if( positionNode.isAttributeSet( "yi" ) )
                point.y.i = atoi( positionNode.getAttribute( "yi" ) );

            if( positionNode.isAttributeSet( "zi" ) )
                point.z.i = atoi( positionNode.getAttribute( "zi" ) );

            if( positionNode.isAttributeSet( "xf" ) )
                point.x.f = atof( positionNode.getAttribute( "xf" ) );
            else if( positionNode.isAttributeSet( "x" ) )
                point.x.f = atof( positionNode.getAttribute( "x" ) );

            if( positionNode.isAttributeSet( "yf" ) )
                point.y.f = atof( positionNode.getAttribute( "yf" ) );
            else if( positionNode.isAttributeSet( "y" ) )
                point.y.f = atof( positionNode.getAttribute( "y" ) );

            if( positionNode.isAttributeSet( "zf" ) )
                point.z.f = atof( positionNode.getAttribute( "zf" ) );
            else if( positionNode.isAttributeSet( "z" ) )
                point.z.f = atof( positionNode.getAttribute( "z" ) );
        }

        return point;

    }	// LoadPosition


    /************************************************************************
    *    desc:  Load the position
    *
    *    param: node - passed in node
    ************************************************************************/
    CPointInt LoadPositionInt( const XMLNode & node )
    {
        CPointInt point;

        XMLNode positionNode = node.getChildNode( "position" );
    
        if( !positionNode.isEmpty() )
        {
            if( positionNode.isAttributeSet( "x" ) )
                point.x = atoi( positionNode.getAttribute( "x" ) );

            if( positionNode.isAttributeSet( "y" ) )
                point.y = atoi( positionNode.getAttribute( "y" ) );

            if( positionNode.isAttributeSet( "z" ) )
                point.z = atoi( positionNode.getAttribute( "z" ) );
        }

        return point;

    }	// LoadPosition


    /************************************************************************
    *    desc:  Load the rotation
    *
    *    param: node - passed in node
    ************************************************************************/
    CPoint LoadRotation( const XMLNode & node )
    {
        CPoint point;

        XMLNode rotationNode = node.getChildNode( "rotation" );
    
        if( !rotationNode.isEmpty() )
        {
            if( rotationNode.isAttributeSet( "x" ) )
                point.x = atof( rotationNode.getAttribute( "x" ) );

            if( rotationNode.isAttributeSet( "y" ) )
                point.y = atof( rotationNode.getAttribute( "y" ) );

            if( rotationNode.isAttributeSet( "z" ) )
                point.z = atof( rotationNode.getAttribute( "z" ) );
        }

        return point;

    }	// LoadPosition


    /************************************************************************
    *    desc:  Load the scale
    *
    *    param: node - passed in node
    ************************************************************************/
    CPoint LoadScale( const XMLNode & node )
    {
        CPoint point(1,1,1);

        XMLNode scaleNode = node.getChildNode( "scale" );
        if( !scaleNode.isEmpty() )
        {
            if( scaleNode.isAttributeSet( "uniform" ) )
                point.Set( atof( scaleNode.getAttribute( "uniform" ) ) );

            else
            {
                if( scaleNode.isAttributeSet( "x" ) )
                    point.x = atof( scaleNode.getAttribute( "x" ) );

                if( scaleNode.isAttributeSet( "y" ) )
                    point.y = atof( scaleNode.getAttribute( "y" ) );

                if( scaleNode.isAttributeSet( "z" ) )
                    point.z = atof( scaleNode.getAttribute( "z" ) );
            }
        }

        return point;

    }	// LoadScale


    /************************************************************************
    *    desc:  Load the transform data
    *
    *    param: node - passed in node
    ************************************************************************/
    CTransformData LoadTransformData( const XMLNode & node )
    {
        CTransformData transData;

        if( !node.getChildNode( "position" ).isEmpty() )
            transData.SetPos( LoadPosition( node ) );

        if( !node.getChildNode( "rotation" ).isEmpty() )
            transData.SetRot( LoadRotation( node ) );

        if( !node.getChildNode( "Scale" ).isEmpty() )
            transData.SetScale( LoadScale( node ) );

        return transData;

    }	// LoadTransformData


    /************************************************************************
    *    desc:  Load the color
    *
    *    param: node - passed in node
    ************************************************************************/
    CColor LoadColor( const XMLNode & node )
    {
        CColor color;

        XMLNode colorNode = node.getChildNode( "color" );
        if( !colorNode.isEmpty() )
        {
            if( colorNode.isAttributeSet( "r" ) )
                color.r = atof(colorNode.getAttribute( "r" ));

            if( colorNode.isAttributeSet( "g" ) )
                color.g = atof(colorNode.getAttribute( "g" ));

            if( colorNode.isAttributeSet( "b" ) )
                color.b = atof(colorNode.getAttribute( "b" ));

            if( colorNode.isAttributeSet( "a" ) )
                color.a = atof(colorNode.getAttribute( "a" ));

            // See if we are using whole RGB numbers and do the conversion
            // 0.00390625f = 1 / 256;
            if( color.r > 1.5f )
                color.r = color.r * 0.00390625f;

            if( color.g > 1.5f )
                color.g = color.g * 0.00390625f;

            if( color.b > 1.5f )
                color.b = color.b * 0.00390625f;

            if( color.a > 1.5f )
                color.a = color.a * 0.00390625f;
        }

        return color;

    }	// LoadScale


    /************************************************************************
    *    desc:  Load the rotation
    *
    *    param: node - passed in node
    ************************************************************************/
    CSize<float> LoadSize( const XMLNode & node )
    {
        CSize<float> size;

        if( node.isAttributeSet("width") )
            size.w = atof(node.getAttribute( "width" ));

        if( node.isAttributeSet("height") )
            size.h = atof(node.getAttribute( "height" ));

        return size;

    }	// LoadSize


    /************************************************************************
    *    desc:  Load the rotation
    *
    *    param: node - passed in node
    ************************************************************************/
    pair<int,int> LoadDivisions( const XMLNode & node )
    {
        pair<int,int> divisions;

        if( node.isAttributeSet("rows") )
            divisions.first = atoi(node.getAttribute( "rows" ));

        if( node.isAttributeSet("columns") )
            divisions.second = atoi(node.getAttribute( "columns" ));

        return divisions;

    }	// LoadSize


    /************************************************************************
    *    desc:  Load the horizontal alignment
    *
    *    param: node - passed in node
    *           horzAlignment - passed in default alignment
    ************************************************************************/
    EHorzAlignment LoadHorzAlignment( const XMLNode & node, EHorzAlignment horzAlignment )
    {
        EHorzAlignment horzAlign = horzAlignment;

        if( node.isAttributeSet( "horzAlign" ) )
        {
            string horzAlignStr = node.getAttribute( "horzAlign" );
            if( horzAlignStr == "left" )
                horzAlign = EHA_HORZ_LEFT;

            else if( horzAlignStr == "center" )
                horzAlign = EHA_HORZ_CENTER;

            else if( horzAlignStr == "right" )
                horzAlign = EHA_HORZ_RIGHT;
        }

        return horzAlign;

    }	// LoadHorzAlignment


    /************************************************************************
    *    desc:  Load the vertical alignment
    *
    *    param: node - passed in node
    *           horzAlignment - passed in default alignment
    ************************************************************************/
    EVertAlignment LoadVertAlignment( const XMLNode & node, EVertAlignment vertAlignment )
    {
        EVertAlignment vertAlign = vertAlignment;

        if( node.isAttributeSet( "vertAlign" ) )
        {
            string vertAlignStr = node.getAttribute( "vertAlign" );
            if( vertAlignStr == "top" )
                vertAlign = EVA_VERT_TOP;

            else if( vertAlignStr == "center" )
                vertAlign = EVA_VERT_CENTER;

            else if( vertAlignStr == "bottom" )
                vertAlign = EVA_VERT_BOTTOM;
        }

        return vertAlign;

    }	// LoadVertAlignment


    /************************************************************************
    *    desc:  Load the dynamic offset
    *
    *    param: node - passed in node
    *           dynamicOffset - bitmask to set
    *           dynamicOffsetPt - point to set
    ************************************************************************/
    CDynamicOffset LoadDynamicOffset( const XMLNode & node )
    {
        CDynamicOffset dynamicOffset;

        XMLNode dynamicOffsetNode = node.getChildNode( "dynamicOffset" );
    
        if( !dynamicOffsetNode.isEmpty() )
        {
            if( dynamicOffsetNode.isAttributeSet( "left" ) )
            {
                dynamicOffset.AndBitMask( CDynamicOffset::EDO_LEFT );
                dynamicOffset.SetX( atof( dynamicOffsetNode.getAttribute( "left" ) ) );
            }
            else if( dynamicOffsetNode.isAttributeSet( "right" ) )
            {
                dynamicOffset.AndBitMask( CDynamicOffset::EDO_RIGHT );
                dynamicOffset.SetX( atof( dynamicOffsetNode.getAttribute( "right" ) ) );
            }
            else if( dynamicOffsetNode.isAttributeSet( "horzCenter" ) )
            {
                dynamicOffset.AndBitMask( CDynamicOffset::EDO_HORZ_CENTER );
                dynamicOffset.SetX( atof( dynamicOffsetNode.getAttribute( "horzCenter" ) ) );
            }

            if( dynamicOffsetNode.isAttributeSet( "top" ) )
            {
                dynamicOffset.AndBitMask( CDynamicOffset::EDO_TOP );
                dynamicOffset.SetY( atof( dynamicOffsetNode.getAttribute( "top" ) ) );
            }

            else if( dynamicOffsetNode.isAttributeSet( "bottom" ) )
            {
                dynamicOffset.AndBitMask( CDynamicOffset::EDO_BOTTOM );
                dynamicOffset.SetY( atof( dynamicOffsetNode.getAttribute( "bottom" ) ) );
            }
            else if( dynamicOffsetNode.isAttributeSet( "vertCenter" ) )
            {
                dynamicOffset.AndBitMask( CDynamicOffset::EDO_VERT_CENTER );
                dynamicOffset.SetX( atof( dynamicOffsetNode.getAttribute( "vertCenter" ) ) );
            }
        }

        return dynamicOffset;

    }	// LoadDynamicOffset

}	// NGenFunc

