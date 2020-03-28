
/************************************************************************
*    FILE NAME:       texturemanager.h
*
*    DESCRIPTION:     3D texture class singleton
************************************************************************/  

#ifndef __texture_manager_h__
#define __texture_manager_h__

// Windows lib dependencies
#include <atlbase.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>
//#include <boost/container/vector.hpp>

// Forward declaration(s)
namespace NText
{
    class CTextureFor2D;
    class CMaterial;
}

class CTextureMgr
{
public:

    // Get the instance of the singleton class
    static CTextureMgr & Instance()
    {
        static CTextureMgr textMgr;
        return textMgr;
    }

    // Load the texture from file path
    LPDIRECT3DTEXTURE9 LoadFor3D( const std::string & group, const std::string & filePath );
    NText::CTextureFor2D * LoadFor2D( const std::string & group, const std::string & filePath, int powerOfTwo = D3DX_DEFAULT_NONPOW2 );

    // Select the texture for blitting
    void SelectTexture( NText::CMaterial & material );
    void SelectTexture( LPDIRECT3DTEXTURE9 pTexture );

    // Texture deleting
    void DeleteTextureGroupFor3D( const std::string & group );
    void DeleteTextureGroupFor2D( const std::string & group );

    // Get all of the textures in a group
    void GetGroupTextures( const std::string & group, std::vector<NText::CTextureFor2D *> & pvector );

    // Dump out informatrion
    void Dump();

private:

    CTextureMgr();
    ~CTextureMgr();

    // Display error information
    void DisplayError( HRESULT hr, const std::string & filePath );

private:

    // map list of texture path & texture pointers
    boost::ptr_map< const std::string, std::map< const std::string, CComPtr<IDirect3DTexture9> > > spTextureFor3DMapMap;
    boost::ptr_map< const std::string, std::map< const std::string, CComPtr<IDirect3DTexture9> > >::iterator textureFor3DMapMapIter;
    std::map< const std::string, CComPtr<IDirect3DTexture9> >::iterator textureFor3DIter;

    boost::ptr_map< const std::string, boost::ptr_map< const std::string, NText::CTextureFor2D > > spTextureFor2DMapMap;
    boost::ptr_map< const std::string, boost::ptr_map< const std::string, NText::CTextureFor2D > >::iterator textureFor2DMapMapIter;
    boost::ptr_map< const std::string, NText::CTextureFor2D >::iterator textureFor2DIter;

    // The currently selected texture
    LPDIRECT3DTEXTURE9 pActiveTexture;

};

#endif  // __texture_manager_h__
