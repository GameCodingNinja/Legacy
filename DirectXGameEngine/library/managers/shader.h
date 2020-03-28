/************************************************************************
*    FILE NAME:       shader.h
*
*    DESCRIPTION:     shader class singleton
************************************************************************/

#ifndef __shader_h__
#define __shader_h__

// DirectX lib dependencies
#include <d3dx9effect.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>

// Game lib dependencies
#include <misc/effectdata.h>
#include <common/defs.h>

class CShader
{
public:

    // Define the boost signal
    typedef boost::signal<void (CEffectData *)> ShaderSignal;

    // Get the instance of the singleton class
    static CShader & Instance()
    {
        static CShader shader;
        return shader;
    }

    // Connect to the shader signal
    void Connect( const ShaderSignal::slot_type & slot );

    // Free the shaders
    void Free();

    // Load the shader from xml file path
    void LoadFromXML( const std::string & filePath );

    // Set the active shader effect
    void SetEffect( const std::string & effectStr );

    // Set the active shader technique
    void SetTechnique( CEffectData * pEffectData, const std::string & idStr );

    // Set the active shader effect and technique
    CEffectData * SetEffectAndTechnique( const std::string & effectStr, const std::string & techniqueStr );
    void SetEffectAndTechnique( CEffectData * pEffectData, const std::string & techniqueStr );

    // Get element count
    int GetElementCount( CEffectData * pEffectData, const std::string & variableStr );

    // Set the effect variable value
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const int value );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const bool value );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const float value );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const D3DXVECTOR4 & value );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const D3DXMATRIX & value );

    void SetEffectValue( const std::string & variableStr, LPDIRECT3DTEXTURE9 pValue );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, LPDIRECT3DTEXTURE9 pValue );

    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const uint arrayCount, bool * pValue );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const uint arrayCount, int * pValue );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const uint arrayCount, float * pValue );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const uint arrayCount, D3DXVECTOR2 * pValue );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const uint arrayCount, D3DXVECTOR4 * pValue );
    void SetEffectValue( CEffectData * pEffectData, const std::string & variableStr, const uint arrayCount, D3DXMATRIX * pValue );

    // Get the active shader
    LPD3DXEFFECT GetActiveShader();

    // Get the active effect data
    CEffectData * GetActiveEffectData();

    // Get the effect data
    CEffectData * GetEffectData( const std::string & effectStr );

    // Is a shader active
    bool IsShaderActive();

    // Ennumerate through all the loaded shaders to init them
    void EnumerateShaderInit();

private:

    // map list of material path & texture pointers
    boost::ptr_map<std::string, CEffectData> spEffectDataMap;
    boost::ptr_map<std::string, CEffectData>::iterator effectDataMapIter;

    // map of effects data types
    std::map<std::string, EEffectType> effectTypeMap;

    // map effect save ptr type iterator
    boost::ptr_map<std::string, CEffectType>::iterator effectTypeMapIter;

    // Active Effect
    CEffectData * pActiveEffect;
    std::string activeEffectStr;

    // Boost signal
    ShaderSignal signal;

private:

    // Constructor
    CShader();

    // Destructor
    ~CShader();

};

#endif  // __shader_h__
