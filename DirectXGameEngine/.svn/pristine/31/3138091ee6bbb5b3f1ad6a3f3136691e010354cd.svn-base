
/************************************************************************
*    FILE NAME:       vertexbuffermanager2d.cpp
*
*    DESCRIPTION:     2D material class singleton
************************************************************************/

// Physical component dependency
#include <managers/vertexbuffermanager2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <system/xdevice.h>


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CVertBufMgr2D::CVertBufMgr2D() : FACE_COUNT(2)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CVertBufMgr2D::~CVertBufMgr2D()
{
}   // Destructer


/************************************************************************
*    desc:  Load the material from file path
*  
*    param: vector<CXFace2D> & faceVector - a vector of faces to create
*											the vertex buffer from
*			const CSize<int> & size       - dimensional size of the mesh
*			string & bufUniqueTag         - a unique string added to the 
*											end ofthe vertex buffer map key
* 
*    return: CMaterialMgr::CMaterial * - NULL on fail
************************************************************************/
LPDIRECT3DVERTEXBUFFER9 CVertBufMgr2D::CreateVertBuffer( 
    const std::string & group, const std::vector<CXFace2D> & faceVector, const CSize<int> & size, const std::string & bufUniqueTag )
{
    // Create the map group if it doesn't already exist
    vertBufMapMapIter = pVertBufMapMap.find( group );
    if( vertBufMapMapIter == pVertBufMapMap.end() )
    {
        pVertBufMapMap.insert( group, new std::map< const std::string, CComPtr<IDirect3DVertexBuffer9> > );
        vertBufMapMapIter = pVertBufMapMap.find( group );
    }

    // Create the map key from the passed in information
    const std::string vertBufKey = bufUniqueTag + boost::str( boost::format("%dx%d_%d") % size.w % size.h % faceVector.size() );

    // Throw an exception is there is no size
    if( size.IsEmpty() )
        throw NExcept::CCriticalException("Vertex Buffer Mgr key size of 0x0!",
            boost::str( boost::format("Vertex Buffer Mgr key has no size (%s, %s).\n\n%s\nLine: %s") % group % vertBufKey % __FUNCTION__ % __LINE__ ));

    // See if this material has already been loaded
    vertBufMapIter = vertBufMapMapIter->second->find( vertBufKey );

    // If it's not found, load the texture and add it to the list
    if( vertBufMapIter == vertBufMapMapIter->second->end() )
    {
        HRESULT hr;
        CComPtr<IDirect3DVertexBuffer9> spVertBuf;

        // Create the vertex buffer
        if( FAILED( hr = CXDevice::Instance().GetXDevice()->CreateVertexBuffer(
            static_cast<UINT>(faceVector.size() * sizeof(CXFace2D)),
            D3DUSAGE_WRITEONLY,
            0,
            D3DPOOL_MANAGED,
            &spVertBuf,
            NULL ) ) )
        {
            DisplayError( hr, vertBufKey );
        }

        // Make a face pointer
        CXFace2D * pFace;

        // Lock the vertex buffer for copying
        if( FAILED( spVertBuf->Lock( 0, 0, (void **)&pFace, 0 ) ) )
        {
            throw NExcept::CCriticalException("Vertex Buffer Mgr Error!",
                boost::str( boost::format("Error locking vertex buffer (%s).\n\n%s\nLine: %s") % vertBufKey.c_str() % __FUNCTION__ % __LINE__ ));
        }

        // Copy the passed in faces to the vertex buffer. NOTE: using std::copy with a pointer causes a huge warning
        for( size_t i = 0; i < faceVector.size(); ++i )
            pFace[i] = faceVector[i];

        // Unlock the buffer so it can be used
        spVertBuf->Unlock();

        // Add the material to our list
        vertBufMapMapIter->second->insert( make_pair(vertBufKey, spVertBuf) );

        //NGenFunc::PostDebugMsg("Created Vertex Buffer: %s - %s", group.c_str(), vertBufKey.c_str() );

        return spVertBuf;
    }

    return vertBufMapIter->second;

}	// CreateVertBuffer


/************************************************************************
*    desc:  Load the material from file path
*  
*    param: const CSize<int> & size - dimensional size of the mesh
*			int rows, columns       - the rows and columns that make up 
*									  the mesh
*			string & bufUniqueTag   - a unique string added to vertex
*								      buffer map key
*			int copies				- number of copies made of the mesh.
* 
*    return: CMaterialMgr::CMaterial * - NULL on fail
************************************************************************/
LPDIRECT3DVERTEXBUFFER9 CVertBufMgr2D::CreateVertBuffer( 
    const std::string & group, const CSize<int> & size, int rows, int columns, const std::string & bufUniqueTag )
{
    // Create the map group if it doesn't already exist
    vertBufMapMapIter = pVertBufMapMap.find( group );
    if( vertBufMapMapIter == pVertBufMapMap.end() )
    {
        pVertBufMapMap.insert( group, new std::map< const std::string, CComPtr<IDirect3DVertexBuffer9> > );
        vertBufMapMapIter = pVertBufMapMap.find( group );
    }

    // Create the map key from the passed in information
    const std::string vertBufKey = bufUniqueTag + boost::str( boost::format("%dx%d") % size.w % size.h );

    // Throw an exception is there is no size
    if( size.IsEmpty() )
        throw NExcept::CCriticalException("Vertex Buffer Mgr key size of 0x0!",
            boost::str( boost::format("Vertex Buffer Mgr key has no size (%s, %s).\n\n%s\nLine: %s") % group % vertBufKey % __FUNCTION__ % __LINE__ ));

    // See if this material has already been loaded
    vertBufMapIter = vertBufMapMapIter->second->find( vertBufKey );

    // If it's not found, load the texture and add it to the list
    if( vertBufMapIter == vertBufMapMapIter->second->end() )
    {
        HRESULT hr;
        CComPtr<IDirect3DVertexBuffer9> spVertBuf;

        // Create the vertex buffer
        if( FAILED( hr = CXDevice::Instance().GetXDevice()->CreateVertexBuffer(
            rows * columns * FACE_COUNT * sizeof(CXFace2D),
            D3DUSAGE_WRITEONLY,
            0,
            D3DPOOL_MANAGED,
            &spVertBuf,
            NULL ) ) )
        {
            DisplayError( hr, vertBufKey );
        }

        // Make a face pointer
        CXFace2D * pFace;

        // Lock the vertex buffer for copying
        if( FAILED(spVertBuf->Lock( 0, 0, (void **)&pFace, 0 ) ) )
        {
            throw NExcept::CCriticalException("Vertex Buffer Mgr Error!",
                boost::str( boost::format("Error locking vertex buffer (%s).\n\n%s\nLine: %s") % vertBufKey.c_str() % __FUNCTION__ % __LINE__ ));
        }

        int tmp_w = size.w;
        int tmp_h = size.h;

        float additionalOffsetX = 0;
        float additionalOffsetY = 0;

        // Check if the width or height is even. If so, we offset by 0.5
        if( tmp_w % 2 == 0 )
            additionalOffsetX = 0.5f;

        if( tmp_h % 2 == 0 )
            additionalOffsetY = 0.5f;

        // Face indexes
        int face1 = 0; 
        int	face2 = 0;

        // Offsets to center the mesh
        float offsetX = size.w / 2;
        float offsetY = size.h / 2;

        float xStart(-offsetX), yStart(offsetY);
        float xIncAmount = size.w / columns;
        float yIncAmount = size.h / rows;
        float uIncAmount = 1.f / rows;
        float vIncAmount = 1.f / columns;

        // Counters to increment through the loops
        float xCounter = xStart + additionalOffsetX;
        float yCounter = yStart + additionalOffsetY;
        float uCounter = 0;
        float vCounter = 0;

        for( int i = 0; i < rows; ++i )
        {
            for( int j = 0; j < columns; ++j )
            {
                face1 = ((i * columns) + j) * 2;
                face2 = face1 + 1;

                // Calculate the first vertex of the first face
                pFace[face1].vert[0].vert.x = xCounter;
                pFace[face1].vert[0].vert.y = yCounter;
                pFace[face1].vert[0].uv.u = uCounter;
                pFace[face1].vert[0].uv.v = vCounter;

                // Calculate the second vertex of the first face
                pFace[face1].vert[1].vert.x = xCounter + xIncAmount;
                pFace[face1].vert[1].vert.y = yCounter;
                pFace[face1].vert[1].uv.u = uCounter + uIncAmount;
                pFace[face1].vert[1].uv.v = vCounter;

                // The first vertex of the second face is the same as the
                // previously calculated vertex
                pFace[face2].vert[0] = pFace[face1].vert[1];

                // Calculate the third vertex of the first face
                pFace[face1].vert[2].vert.x = xCounter;
                pFace[face1].vert[2].vert.y = yCounter - yIncAmount;
                pFace[face1].vert[2].uv.u = uCounter;
                pFace[face1].vert[2].uv.v = vCounter + vIncAmount;

                // The third vertex of the second face is the same as the
                // previously calculated vertex
                pFace[face2].vert[2] = pFace[face1].vert[2];

                // Calculate the second vertex of the second face
                pFace[face2].vert[1].vert.x = xCounter + xIncAmount;
                pFace[face2].vert[1].vert.y = yCounter - yIncAmount;
                pFace[face2].vert[1].uv.u = uCounter + uIncAmount;
                pFace[face2].vert[1].uv.v = vCounter + vIncAmount;

                // Increment x and u counters
                xCounter += xIncAmount;
                uCounter += uIncAmount;

                // Increment the face counter
                face1 += 2;
            }

            // Increment y and v counter
            yCounter -= yIncAmount;
            vCounter += vIncAmount;

            // Reset the x and u counter
            xCounter = xStart;// + additionalOffsetX;
            uCounter = 0;

        }

        // Unlock the buffer so it can be used
        spVertBuf->Unlock();

        // Add the material to our list
        vertBufMapMapIter->second->insert( make_pair(vertBufKey, spVertBuf) );

        //NGenFunc::PostDebugMsg("Created Vertex Buffer: %s - %s", group.c_str(), vertBufKey.c_str() );

        return spVertBuf;
    }

    return vertBufMapIter->second;

}	// CreateVertBuffer


/************************************************************************
*    desc:  Delete a vert buffer based on a group
*  
*    param: string & filePath - path to material file
************************************************************************/
void CVertBufMgr2D::DeleteVertBufferGroup( const std::string & group )
{
    // Free the vert buffer group if it exists
    vertBufMapMapIter = pVertBufMapMap.find( group );
    if( vertBufMapMapIter != pVertBufMapMap.end() )
        pVertBufMapMap.erase( vertBufMapMapIter );

}	// DeleteVertBufferGroup


/************************************************************************
*    desc:  Dump out informatrion
************************************************************************/
void CVertBufMgr2D::Dump()
{
    NGenFunc::PostDebugMsg( "Vertext Buffer 2D Dump - Group Count: %d", pVertBufMapMap.size() );

    for( vertBufMapMapIter = pVertBufMapMap.begin();
         vertBufMapMapIter != pVertBufMapMap.end();
         ++vertBufMapMapIter )
    {
        NGenFunc::PostDebugMsg( "  Group: %s, count: %d", vertBufMapMapIter->first.c_str(), vertBufMapMapIter->second->size() );

        for( vertBufMapIter = vertBufMapMapIter->second->begin();
             vertBufMapIter != vertBufMapMapIter->second->end();
             ++vertBufMapIter )
        {
            NGenFunc::PostDebugMsg( "    Name: %s", vertBufMapIter->first.c_str() );
        }
    }

}	// Dump


/************************************************************************
*    desc:  Display error information
*
*    param: HRESULT hr - return result from function call
************************************************************************/
void CVertBufMgr2D::DisplayError( HRESULT hr, const std::string & vertSizeStr )
{
    switch( hr )
    {
        case D3DERR_OUTOFVIDEOMEMORY:
        {
            throw NExcept::CCriticalException("Vertex Buffer Mgr Error!",
                boost::str( boost::format("Error creating vertex buffer (%s). Does not have enough display memory to load texture.") % vertSizeStr.c_str() ));
    
            break;
        }
        case D3DERR_INVALIDCALL:
        {
            throw NExcept::CCriticalException("Vertex Buffer Mgr Error!",
                boost::str( boost::format("Error creating vertex buffer (%s). The method call is invalid.") % vertSizeStr.c_str() ));
    
            break;
        }
        case E_OUTOFMEMORY:
        {
            throw NExcept::CCriticalException("Vertex Buffer Mgr Error!",
                boost::str( boost::format("Error creating vertex buffer (%s). Direct3D could not allocate sufficient memory to load texture.") % vertSizeStr.c_str() ));
    
            break;
        }
        default:
        {
            throw NExcept::CCriticalException("Vertex Buffer Mgr Error!",
                boost::str( boost::format("Error creating vertex buffer (%s). Unknow error.") % vertSizeStr.c_str() ));
    
            break;
        }
    }

}	// DisplayError
