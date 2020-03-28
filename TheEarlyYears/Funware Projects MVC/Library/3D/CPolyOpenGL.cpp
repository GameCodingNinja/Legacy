
/************************************************************************
*
*    FILE NAME:       CPolyOpenGL.cpp
*
*    DESCRIPTION:     OpenGL Rendering functions
*
************************************************************************/

                           // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT             // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>       // Windows header file for creating windows programs. This is a must have.
#include "CPolygon.h"      // Header file for this *.cpp file.
#include <Math.H>

#include "gl/gl.h"         // standard OpenGL include
#include "gl/glu.h"        // OpenGL utilities

// Globals
unsigned int gOpenGLTextCurrentID=0;


/************************************************************************
*    DESCRIPTION:  bind a specific texture for rendering
*
*    Input:    PCTextureLib pTextLib - Texture library class
************************************************************************/
void _fastcall CPolygon::BindOpenGLText( CTextureLib * pTextLib )
{
    // Only switch states selecting a new texture if the needed texture is different
    if( gOpenGLTextCurrentID != pTextLib->pTMap[tIndex].GetOpenGLTxtID() )
    {
        glBindTexture( GL_TEXTURE_2D, pTextLib->pTMap[tIndex].GetOpenGLTxtID() );
        gOpenGLTextCurrentID = pTextLib->pTMap[tIndex].GetOpenGLTxtID();

        // see if the current blending state matches the what is needed
        if( (GLboolean)pTextLib->pTMap[tIndex].AlphaBlend() != glIsEnabled(GL_BLEND) )
		{
	        // enable and disable alpha blending
	        if( pTextLib->pTMap[tIndex].AlphaBlend() )
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}
    }

}   // BindOpenGLText


/************************************************************************
*    DESCRIPTION:  Send the info to OpenGL for rendering.
*
*    Input:    PCTextureLib pTextLib - Texture library class
************************************************************************/
void _fastcall CPolygon::RenderOpenGLText( CTextureLib * pTextLib )
{
    // Only switch states selecting a new texture if the needed texture is different
    if( gOpenGLTextCurrentID != pTextLib->pTMap[tIndex].GetOpenGLTxtID() )
    {
        glBindTexture( GL_TEXTURE_2D, pTextLib->pTMap[tIndex].GetOpenGLTxtID() );
        gOpenGLTextCurrentID = pTextLib->pTMap[tIndex].GetOpenGLTxtID();

        // see if the current blending state matches the what is needed
        if( (GLboolean)pTextLib->pTMap[tIndex].AlphaBlend() != glIsEnabled(GL_BLEND) )
		{
	        // enable and disable alpha blending
	        if( pTextLib->pTMap[tIndex].AlphaBlend() )
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}
    }

    // Tell OpenGL we are going to send it some data
    if( vertCount == TRI )
        glBegin( GL_TRIANGLES );
    else
        glBegin( GL_QUADS );

    for( int i = 0; i < vertCount; ++i )
    {
        glTexCoord2f( uvrgb[i].u, uvrgb[i].v );
        glColor3f( uvrgb[i].r, uvrgb[i].g, uvrgb[i].b );
        glVertex3f( pVPoint[i]->tx, pVPoint[i]->ty, pVPoint[i]->tz );
    }

    // Tell OpenGL we are done sending polygon data
    glEnd();

}   // RenderOpenGLText


/************************************************************************
*    DESCRIPTION:  Send the info to OpenGL for rendering.
*
*    Input:    PCTextureLib pTextLib - Texture library class
************************************************************************/
void _fastcall CPolygon::RenderOpenGLShadow( CPoint &shadowOffset )
{
    // Tell OpenGL we are going to send it some data
    if( vertCount == TRI )
        glBegin( GL_TRIANGLES );
    else
        glBegin( GL_QUADS );

    for( int i = 0; i < vertCount; ++i )
    {
        glTexCoord2f( uvrgb[i].u, uvrgb[i].v );
        glColor3f( uvrgb[i].r, uvrgb[i].g, uvrgb[i].b );
        glVertex3f( pVPoint[i]->tx + shadowOffset.x,
        			pVPoint[i]->ty + shadowOffset.y,
        			pVPoint[i]->tz + shadowOffset.z );
    }

    // Tell OpenGL we are done sending polygon data
    glEnd();

}   // RenderOpenGLShadow


/************************************************************************
*    DESCRIPTION:  Send the info to OpenGL for rendering.
************************************************************************/
void _fastcall CPolygon::RenderOpenGL()
{    
    // Tell OpenGL we are going to send it some data
    if( vertCount == TRI )
        glBegin( GL_TRIANGLES );
    else
        glBegin( GL_QUADS );

    for( int i = 0; i < vertCount; ++i )
    {
        glColor3f( uvrgb[i].r, uvrgb[i].g, uvrgb[i].b );
        glVertex3f( pVPoint[i]->tx, pVPoint[i]->ty, pVPoint[i]->tz );
    }

    // Tell OpenGL we are done sending polygon data
    glEnd();

}   // RenderOpenGL