
/************************************************************************
*
*    FILE NAME:       CPolyDirectX.cpp
*
*    DESCRIPTION:     OpenGL Rendering functions
*
************************************************************************/

                           // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT             // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>       // Windows header file for creating windows programs. This is a must have.
#include "CPolygon.h"      // Header file for this *.cpp file.

#ifdef _USE_DIRECTX_9_
    #include "d3dx9.h"

	extern LPDIRECT3DVERTEXBUFFER9 g_DirectX_VB;
	extern LPDIRECT3DDEVICE9 g_pDXDevice;
	LPDIRECT3DTEXTURE9 g_pDXCurrentText=NULL;
#endif
#ifdef _USE_DIRECTX_8_
    #include "DX8/d3dx8.h"

    extern LPDIRECT3DVERTEXBUFFER8 g_DirectX_VB;
	extern LPDIRECT3DDEVICE8 g_pDXDevice;
	LPDIRECT3DTEXTURE8 g_pDXCurrentText=NULL;
#endif

// Globals
int gTriCounter=0;

CDirectXVert *pVert;


/************************************************************************
*    DESCRIPTION:  bind a specific texture for rendering
************************************************************************/
void _fastcall CPolygon::BindDirectXText( CTextureLib *pTextLib )
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    // Only switch states selecting a new texture if the needed texture is different
    if( g_pDXCurrentText != pTextLib->pTMap[tIndex].GetDirectXTextPointer() )
    {
        // Draw the current set of polygons before switching out the texture
        DrawDirectXPrimatives();

        g_pDXDevice->SetTexture( 0, pTextLib->pTMap[tIndex].GetDirectXTextPointer() );
        g_pDXCurrentText = pTextLib->pTMap[tIndex].GetDirectXTextPointer();

        unsigned long renderState;
		g_pDXDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, &renderState );

		// see if the current blending state matches the what is needed
		if( (unsigned int)pTextLib->pTMap[tIndex].AlphaBlend() != renderState )
		{
			// enable and disable alpha blending
			g_pDXDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, pTextLib->pTMap[tIndex].AlphaBlend() );
		}
    }

    #endif

}   // BindDirectXText


/************************************************************************
*    DESCRIPTION:  Send the info to DirectX for rendering.
************************************************************************/
void _fastcall CPolygon::RenderDirectXShadow( CPoint &shadowOffset )
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    int offset;
    int triOffset;

    if( gTriCounter == 0 )
    {
        #ifdef _USE_DIRECTX_9_
        g_DirectX_VB->Lock( 0, 0, (void **)&pVert, 0 );
		#endif
		#ifdef _USE_DIRECTX_8_
        g_DirectX_VB->Lock( 0, 0, (BYTE **)&pVert, 0 );
		#endif
    }

    if( vertCount == TRI )
    {  
        // Calc the offset into the array
        offset = gTriCounter * TRI;

        for( int i = 0; i < vertCount; ++i )
        {
            pVert[offset+i].x = pVPoint[i]->tx + shadowOffset.x;
            pVert[offset+i].y = pVPoint[i]->ty + shadowOffset.y;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -(pVPoint[i]->tz + shadowOffset.z);

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[i].r,
                                                   (int)uvrgb[i].g,
                                                   (int)uvrgb[i].b );

            pVert[offset+i].u = uvrgb[i].u;
            pVert[offset+i].v = uvrgb[i].v;

        }

        ++gTriCounter;
    }
    else if( vertCount == QUAD )
    {
        // Calc the offset into the array
        offset = gTriCounter * TRI;

        for( int i = 0; i < TRI; ++i )
        {
            pVert[offset+i].x = pVPoint[i]->tx + shadowOffset.x;
            pVert[offset+i].y = pVPoint[i]->ty + shadowOffset.y;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -(pVPoint[i]->tz + shadowOffset.z);

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[i].r,
                                                   (int)uvrgb[i].g,
                                                   (int)uvrgb[i].b );

            pVert[offset+i].u = uvrgb[i].u;
            pVert[offset+i].v = uvrgb[i].v;
        }

        // Calc the offset into the array
        offset = (gTriCounter * TRI) + TRI;
        
        for( int i = 0; i < TRI; ++i )
        {
            triOffset = (i+2) & TRI;

            pVert[offset+i].x = pVPoint[triOffset]->tx + shadowOffset.x;
            pVert[offset+i].y = pVPoint[triOffset]->ty + shadowOffset.y;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -(pVPoint[triOffset]->tz + shadowOffset.z);

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[triOffset].r,
                                                   (int)uvrgb[triOffset].g,
                                                   (int)uvrgb[triOffset].b );

            pVert[offset+i].u = uvrgb[triOffset].u;
            pVert[offset+i].v = uvrgb[triOffset].v;
        }

        // Two triangles per quad
        gTriCounter += 2;
    }

    // Render the polygons that have reached the max amount
    DrawDirectXPrimatives( MAX_POLYGON_COUNT - 1 );

    #endif

}   // RenderDirectXShadow


/************************************************************************
*    DESCRIPTION:  Send the info to DirectX for rendering.
************************************************************************/
void _fastcall CPolygon::RenderDirectXText( CTextureLib *pTextLib )
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    int offset;
    int triOffset;

    // Only switch states selecting a new texture if the needed texture is different
    if( g_pDXCurrentText != pTextLib->pTMap[tIndex].GetDirectXTextPointer() )
    {
        // Draw the current set of polygons before switching out the texture
        DrawDirectXPrimatives();

        g_pDXDevice->SetTexture( 0, pTextLib->pTMap[tIndex].GetDirectXTextPointer() );
        g_pDXCurrentText = pTextLib->pTMap[tIndex].GetDirectXTextPointer();

        unsigned long renderState;
		g_pDXDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, &renderState );

		// see if the current blending state matches the what is needed
		if( (unsigned int)pTextLib->pTMap[tIndex].AlphaBlend() != renderState )
		{
			// enable and disable alpha blending
			g_pDXDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, pTextLib->pTMap[tIndex].AlphaBlend() );
		}
    }

    if( gTriCounter == 0 )
    {
        #ifdef _USE_DIRECTX_9_
        g_DirectX_VB->Lock( 0, 0, (void **)&pVert, 0 );
		#endif
		#ifdef _USE_DIRECTX_8_
        g_DirectX_VB->Lock( 0, 0, (BYTE **)&pVert, 0 );
		#endif
    }

    if( vertCount == TRI )
    {  
        // Calc the offset into the array
        offset = gTriCounter * TRI;

        for( int i = 0; i < vertCount; ++i )
        {
            pVert[offset+i].x = pVPoint[i]->tx;
            pVert[offset+i].y = pVPoint[i]->ty;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -pVPoint[i]->tz;

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[i].r,
                                                   (int)uvrgb[i].g,
                                                   (int)uvrgb[i].b );

            pVert[offset+i].u = uvrgb[i].u;
            pVert[offset+i].v = uvrgb[i].v;

        }

        ++gTriCounter;
    }
    else if( vertCount == QUAD )
    {
        // Calc the offset into the array
        offset = gTriCounter * TRI;

        for( int i = 0; i < TRI; ++i )
        {
            pVert[offset+i].x = pVPoint[i]->tx;
            pVert[offset+i].y = pVPoint[i]->ty;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -pVPoint[i]->tz;

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[i].r,
                                                   (int)uvrgb[i].g,
                                                   (int)uvrgb[i].b );

            pVert[offset+i].u = uvrgb[i].u;
            pVert[offset+i].v = uvrgb[i].v;
        }

        // Calc the offset into the array
        offset = (gTriCounter * TRI) + TRI;
        
        for( int i = 0; i < TRI; ++i )
        {
            triOffset = (i+2) & TRI;

            pVert[offset+i].x = pVPoint[triOffset]->tx;
            pVert[offset+i].y = pVPoint[triOffset]->ty;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -pVPoint[triOffset]->tz;

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[triOffset].r,
                                                   (int)uvrgb[triOffset].g,
                                                   (int)uvrgb[triOffset].b );

            pVert[offset+i].u = uvrgb[triOffset].u;
            pVert[offset+i].v = uvrgb[triOffset].v;
        }

        // Two triangles per quad
        gTriCounter += 2;
    }

    // Render the polygons that have reached the max amount
    DrawDirectXPrimatives( MAX_POLYGON_COUNT - 1 );

    #endif

}   // RenderDirectXText


/************************************************************************
*    DESCRIPTION:  Send the info to DirectX for rendering.
************************************************************************/
void _fastcall CPolygon::RenderDirectX()
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    int offset;
    int triOffset;

    if( gTriCounter == 0 )
    {
        #ifdef _USE_DIRECTX_9_
        g_DirectX_VB->Lock( 0, 0, (void **)&pVert, 0 );
		#endif
		#ifdef _USE_DIRECTX_8_
        g_DirectX_VB->Lock( 0, 0, (BYTE **)&pVert, 0 );
		#endif
    }

    if( vertCount == TRI )
    {  
        // Calc the offset into the array
        offset = gTriCounter * TRI;

        for( int i = 0; i < vertCount; ++i )
        {
            pVert[offset+i].x = pVPoint[i]->tx;
            pVert[offset+i].y = pVPoint[i]->ty;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -pVPoint[i]->tz;

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[i].r,
                                                   (int)uvrgb[i].g,
                                                   (int)uvrgb[i].b );

            pVert[offset+i].u = 0.0;
            pVert[offset+i].v = 0.0;
        }

        ++gTriCounter;
    }
    else if( vertCount == QUAD )
    {
        // Calc the offset into the array
        offset = gTriCounter * TRI;

        for( int i = 0; i < TRI; ++i )
        {
            pVert[offset+i].x = pVPoint[i]->tx;
            pVert[offset+i].y = pVPoint[i]->ty;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -pVPoint[i]->tz;

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[i].r,
                                                   (int)uvrgb[i].g,
                                                   (int)uvrgb[i].b );

            pVert[offset+i].u = 0.0;
            pVert[offset+i].v = 0.0;
        }

        // Calc the offset into the array
        offset = (gTriCounter * TRI) + TRI;
        
        for( int i = 0; i < TRI; ++i )
        {
            triOffset = (i+2) & TRI;

            pVert[offset+i].x = pVPoint[triOffset]->tx;
            pVert[offset+i].y = pVPoint[triOffset]->ty;
            // DirectX requires a positive Z value
            pVert[offset+i].z = -pVPoint[triOffset]->tz;

            pVert[offset+i].color = D3DCOLOR_XRGB( (int)uvrgb[triOffset].r,
                                                   (int)uvrgb[triOffset].g,
                                                   (int)uvrgb[triOffset].b );

            pVert[offset+i].u = 0.0;
            pVert[offset+i].v = 0.0;
        }

        // Two triangles per quad
        gTriCounter += 2;
    }

    // Render the polygons that have reached the max amount
    DrawDirectXPrimatives( MAX_POLYGON_COUNT - 1 );

    #endif

}   // RenderDirectX


/************************************************************************
*    DESCRIPTION:  Draw the current set of active primatives
************************************************************************/
void _fastcall DrawDirectXPrimatives( int maxRenderAmount )
{
    #if defined(_USE_DIRECTX_8_) || defined(_USE_DIRECTX_9_)

    if( gTriCounter > maxRenderAmount )
    {
        g_DirectX_VB->Unlock();

        // Render the vertex buffer contents
        #ifdef _USE_DIRECTX_9_
        g_pDXDevice->SetStreamSource( 0, g_DirectX_VB, 0, sizeof(CDirectXVert) );
		g_pDXDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
		#endif
		#ifdef _USE_DIRECTX_8_
        g_pDXDevice->SetStreamSource( 0, g_DirectX_VB, sizeof(CDirectXVert) );
		g_pDXDevice->SetVertexShader( D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
		#endif
        
        g_pDXDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, gTriCounter );

        gTriCounter = 0;
    }

    #endif

}   // DrawDirectXPrimatives