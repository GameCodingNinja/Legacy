
/************************************************************************
*    FILE NAME:       vertexbuffermanager2d.h
*
*    DESCRIPTION:     2D vertex buffer manager class singleton
************************************************************************/  

#ifndef __vertex_buffer_manager_2d_h__
#define __vertex_buffer_manager_2d_h__

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <common/size.h>
#include <common/xface2d.h>

class CVertBufMgr2D
{
public:

    // Get the instance of the singleton class
    static CVertBufMgr2D & Instance()
    {
        static CVertBufMgr2D VertBufMgr2D;
        return VertBufMgr2D;
    }

    // Delete a specific vert buffer
    void DeleteVertBuffer( const std::string & key );
    void DeleteVertBufferGroup( const std::string & group );

    // Create the vertex buffer
    LPDIRECT3DVERTEXBUFFER9 CreateVertBuffer( const std::string & group,
                                              const CSize<int> & size,  
                                              int rows = 1,
                                              int columns = 1,
                                              const std::string & bufUniqueTag = std::string("") );

    LPDIRECT3DVERTEXBUFFER9 CreateVertBuffer( const std::string & group,
                                              const std::vector<CXFace2D> & faceVector, 
                                              const CSize<int> & size, 
                                              const std::string & bufUniqueTag = std::string("") );

    // Dump out informatrion
    void Dump();

protected:

    // Display error information
    void DisplayError( HRESULT hr, const std::string & vertSizeStr );

    // map list of material path & texture pointers
    boost::ptr_map< const std::string, std::map< const std::string, CComPtr<IDirect3DVertexBuffer9> > > pVertBufMapMap;
    boost::ptr_map< const std::string, std::map< const std::string, CComPtr<IDirect3DVertexBuffer9> > >::iterator vertBufMapMapIter;
    std::map< const std::string, CComPtr<IDirect3DVertexBuffer9> >::iterator vertBufMapIter;

private:

    CVertBufMgr2D();
    ~CVertBufMgr2D();

    const int FACE_COUNT;

};


#endif  // __vertex_buffer_manager_2d_h__
