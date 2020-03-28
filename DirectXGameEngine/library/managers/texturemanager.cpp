
/************************************************************************
*    FILE NAME:       texturemanager.h
*
*    DESCRIPTION:     3D texture class singleton
************************************************************************/

// Physical component dependency
#include <managers/texturemanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <system/xdevice.h>
#include <managers/shader.h>
#include <common/texture.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CTextureMgr::CTextureMgr()
           : pActiveTexture(NULL)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CTextureMgr::~CTextureMgr()
{
}   // Destructer


/************************************************************************
*    desc:  Load the texture from file path
*  
*    param: string & filePath - path to texture file
* 
*    return: LPDIRECT3DTEXTURE9 - NULL on fail
************************************************************************/
LPDIRECT3DTEXTURE9 CTextureMgr::LoadFor3D( const std::string & group, const std::string & filePath )
{
    // Create the map group if it doesn't already exist
    textureFor3DMapMapIter = spTextureFor3DMapMap.find( group );
    if( textureFor3DMapMapIter == spTextureFor3DMapMap.end() )
    {
        spTextureFor3DMapMap.insert( group, new std::map< const std::string, CComPtr<IDirect3DTexture9> > );
        textureFor3DMapMapIter = spTextureFor3DMapMap.find( group );
    }

    // See if this texture has already been loaded
    textureFor3DIter = textureFor3DMapMapIter->second->find( filePath );

    // If it's not found, load the texture and add it to the list
    if( textureFor3DIter == textureFor3DMapMapIter->second->end() )
    {
        CComPtr<IDirect3DTexture9> spTexture;
        HRESULT hr;

        // Try to load the texture into memory from file
        if( FAILED( hr = D3DXCreateTextureFromFile( CXDevice::Instance().GetXDevice(), filePath.c_str(), &spTexture ) ) )
            DisplayError( hr, filePath );

        // Add the texture to our list
        textureFor3DMapMapIter->second->insert( make_pair(filePath, spTexture) );

        // Find it again
        return spTexture;
    }

    return textureFor3DIter->second;

}	// LoadFor3D


/************************************************************************
*    desc:  Load the texture from file path
*  
*    param: string & filePath - path to texture file
*    param: string & powerOfTwo - defaults to D3DX_DEFAULT_NONPOW2
* 
*    return: CTextureFor2D * - NULL on fail
************************************************************************/
NText::CTextureFor2D * CTextureMgr::LoadFor2D( const std::string & group, const std::string & filePath, int powerOfTwo )
{
    // Create the map group if it doesn't already exist
    textureFor2DMapMapIter = spTextureFor2DMapMap.find( group );
    if( textureFor2DMapMapIter == spTextureFor2DMapMap.end() )
    {
        spTextureFor2DMapMap.insert( group, new boost::ptr_map< const std::string, NText::CTextureFor2D > );
        textureFor2DMapMapIter = spTextureFor2DMapMap.find( group );
    }

    // See if this texture has already been loaded
    textureFor2DIter = textureFor2DMapMapIter->second->find( filePath );

    // If it's not found, load the texture and add it to the list
    if( textureFor2DIter == textureFor2DMapMapIter->second->end() )
    {
        LPDIRECT3DTEXTURE9 pTexture;
        D3DXIMAGE_INFO imageInfo;
        HRESULT hr;

        // Try to load the texture into memory
        if( FAILED( hr = D3DXCreateTextureFromFileEx(
                            CXDevice::Instance().GetXDevice(),	// pDevice
                            filePath.c_str(),					// pSrcFile
                            powerOfTwo,				            // Width
                            powerOfTwo,				            // Height
                            1,									// MipLevels
                            0,									// Usage
                            D3DFMT_UNKNOWN,						// Format
                            D3DPOOL_MANAGED,					// Pool
                            D3DX_DEFAULT,						// Filter
                            D3DX_DEFAULT,						// MipFilter
                            0,									// ColorKey
                            &imageInfo,							// pSrcInfo
                            NULL,								// pPalette
                            &pTexture) ) )				        // ppTexture
        {
            DisplayError( hr, filePath );
        }

        // Fill out the texture class
        NText::CTextureFor2D * pTextureFor2D = new NText::CTextureFor2D;
        pTextureFor2D->spTexture.Attach( pTexture );
        pTextureFor2D->size.w = static_cast<float>(imageInfo.Width);
        pTextureFor2D->size.h = static_cast<float>(imageInfo.Height);

        // Add the texture to our safe map
        textureFor2DMapMapIter->second->insert( filePath, pTextureFor2D );

        //NGenFunc::PostDebugMsg("Created Texture: %s - %s", group.c_str(), filePath.c_str() );

        return pTextureFor2D;
    }

    return textureFor2DIter->second;

}	// LoadFor2D


/************************************************************************
*    desc:  Select the texture as the active texture for shader
*  
*    param: material - material to select
************************************************************************/
void CTextureMgr::SelectTexture( NText::CMaterial & material )
{
    // Check the defuse texture to see if this texture has been loaded
    if( material.textureLstVec[0].pTexture != pActiveTexture )
    {
        for( size_t i = 0; i < material.textureLstVec.size(); ++i )
        {
            if( material.textureLstVec[i].type == NText::ETT_DIFFUSE )
                CShader::Instance().SetEffectValue("diffuseTexture", material.textureLstVec[i].pTexture);

            else if( material.textureLstVec[i].type == NText::ETT_NORMAL )
                CShader::Instance().SetEffectValue("normalTexture", material.textureLstVec[i].pTexture);

            else if( material.textureLstVec[i].type == NText::ETT_SPECULAR )
                CShader::Instance().SetEffectValue("normalSpecular", material.textureLstVec[i].pTexture);

            else if( material.textureLstVec[i].type == NText::ETT_DISPLACEMENT )
                CShader::Instance().SetEffectValue("normalDisplacement", material.textureLstVec[i].pTexture);
        }

        pActiveTexture = material.textureLstVec[0].pTexture;
    }
}	// SelectTexture


/************************************************************************
*    desc:  Select the texture as the active texture for shader
*  
*    param: texture - texture to select
************************************************************************/
void CTextureMgr::SelectTexture( LPDIRECT3DTEXTURE9 pTexture )
{
    // Check the defuse texture to see if this texture has been loaded
    if( pTexture != pActiveTexture )
    {
        CShader::Instance().SetEffectValue("diffuseTexture", pTexture);

        pActiveTexture = pTexture;
    }

}	// SelectTexture


/************************************************************************
*    desc:  Delete a texture in a group
*  
*    param: string & filePath - path to texture file
************************************************************************/
void CTextureMgr::DeleteTextureGroupFor2D( const std::string & group )
{
    // Free the texture group if it exists
    textureFor2DMapMapIter = spTextureFor2DMapMap.find( group );
    if( textureFor2DMapMapIter != spTextureFor2DMapMap.end() )
        spTextureFor2DMapMap.erase( textureFor2DMapMapIter );

}	// DeleteGroupTexturesFor2D


/************************************************************************
*    desc:  Delete a specific texture - file path is the key
*  
*    param: string & filePath - path to texture file
************************************************************************/
void CTextureMgr::DeleteTextureGroupFor3D( const std::string & group )
{
    // Free the texture group if it exists
    textureFor3DMapMapIter = spTextureFor3DMapMap.find( group );
    if( textureFor3DMapMapIter != spTextureFor3DMapMap.end() )
        spTextureFor3DMapMap.erase( textureFor3DMapMapIter );

}	// DeleteTextureGroupFor3D


/************************************************************************
*    desc:  Get all of the textures in a group
*  
*    param: const string & group - texture group
*			vector<NText::CTextureFor2D *> & pTextureVector
*								 - vector to fill with the textures
************************************************************************/
void CTextureMgr::GetGroupTextures( const std::string & group, std::vector<NText::CTextureFor2D *> & pTextureVector )
{
    // See if the texture group exists
    textureFor2DMapMapIter = spTextureFor2DMapMap.find( group );
    if( textureFor2DMapMapIter != spTextureFor2DMapMap.end() )
    {
        pTextureVector.reserve( textureFor2DMapMapIter->second->size() );
        textureFor2DIter = textureFor2DMapMapIter->second->begin();

        // Begin placing all of the textures from the group to the passed in vector
        while( textureFor2DIter != textureFor2DMapMapIter->second->end() )
        {
            pTextureVector.push_back( textureFor2DIter->second );
            ++textureFor2DIter;
        }
    }

}	// GetGroupTextures


/************************************************************************
*    desc:  Dump out informatrion
************************************************************************/
void CTextureMgr::Dump()
{
    NGenFunc::PostDebugMsg( "Texture Manager Dump" );

    if( !spTextureFor3DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Textures for 3D Map - Group Count: %d", spTextureFor3DMapMap.size() );

        for( textureFor3DMapMapIter = spTextureFor3DMapMap.begin();
             textureFor3DMapMapIter != spTextureFor3DMapMap.end();
             ++textureFor3DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", textureFor3DMapMapIter->first.c_str(), textureFor3DMapMapIter->second->size() );

            for( textureFor3DIter = textureFor3DMapMapIter->second->begin();
                 textureFor3DIter != textureFor3DMapMapIter->second->end();
                 ++textureFor3DIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", textureFor3DIter->first.c_str() );
            }
        }
    }

    if( !spTextureFor2DMapMap.empty() )
    {
        NGenFunc::PostDebugMsg( "Textures for 2D Map - Group Count: %d", spTextureFor2DMapMap.size() );

        for( textureFor2DMapMapIter = spTextureFor2DMapMap.begin();
             textureFor2DMapMapIter != spTextureFor2DMapMap.end();
             ++textureFor2DMapMapIter )
        {
            NGenFunc::PostDebugMsg( "  Group: %s, count: %d", textureFor2DMapMapIter->first.c_str(), textureFor2DMapMapIter->second->size() );

            for( textureFor2DIter = textureFor2DMapMapIter->second->begin();
                 textureFor2DIter != textureFor2DMapMapIter->second->end();
                 ++textureFor2DIter )
            {
                NGenFunc::PostDebugMsg( "    Name: %s", textureFor2DIter->first.c_str() );
            }
        }
    }

}	// Dump


/************************************************************************
*    desc:  Display error information
*
*    param: HRESULT hr - return result from function call
************************************************************************/
void CTextureMgr::DisplayError( HRESULT hr, const std::string & filePath )
{
    switch( hr )
    {
        case D3DERR_NOTAVAILABLE:
        {
            throw NExcept::CCriticalException("Load Texture Error!", 
                boost::str( boost::format("Error creating texture (%s). This device does not support the queried technique.") % filePath ));

            break;
        }
        case D3DERR_OUTOFVIDEOMEMORY:
        {
            throw NExcept::CCriticalException("Load Texture Error!", 
                boost::str( boost::format("Error creating texture (%s). Does not have enough display memory to load texture.") % filePath ));
    
            break;
        }
        case D3DERR_INVALIDCALL:
        {
            throw NExcept::CCriticalException("Load Texture Error!", 
                boost::str( boost::format("Error creating texture (%s). The method call is invalid.") % filePath ));
    
            break;
        }
        case D3DXERR_INVALIDDATA:
        {
            throw NExcept::CCriticalException("Load Texture Error!", 
                boost::str( boost::format("Error creating texture (%s). The data is invalid.") % filePath ));
    
            break;
        }
        case E_OUTOFMEMORY:
        {
            throw NExcept::CCriticalException("Load Material Error!", 
                boost::str( boost::format("Error creating texture (%s). Direct3D could not allocate sufficient memory to load texture.") % filePath ));
    
            break;
        }
        default:
        {
            throw NExcept::CCriticalException("Load Texture Error!", 
                boost::str( boost::format("Error loading material (%s). Unknow error.") % filePath ));
    
            break;
        }
    }

}	// DisplayError