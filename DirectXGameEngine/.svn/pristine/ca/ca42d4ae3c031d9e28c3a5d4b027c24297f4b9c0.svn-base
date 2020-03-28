
/************************************************************************
*    FILE NAME:       lightlst.cpp
*
*    DESCRIPTION:     3D light list class
************************************************************************/

// Physical component dependency
#include <3d/lightlst.h>

// DirectX lib dependencies
#include <d3dx9.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <assert.h>

// Game lib dependencies
#include <xmlParser/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <managers/shader.h>
#include <misc/effectdata.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. double to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CLightLst::CLightLst()
         : shadowCastingLight(NULL)
{
    // Setup the light type map container
    lightTypeMap["directional"] = ELT_DIRECTIONAL;
    lightTypeMap["point_infinite"] = ELT_POINT_INFINITE;
    lightTypeMap["point_radius"] = ELT_POINT_RADIUS;
    lightTypeMap["ambient"] = ELT_AMBIENT;
    
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CLightLst::~CLightLst()
{
    Free();

}   // Destructer


/************************************************************************
*    desc:  Free the light list                                                           
************************************************************************/
void CLightLst::Free()
{	
    lightVecLst.clear();
    spLightLstMap.clear();
    shadowCastingLight = NULL;
    
}   // Free


/************************************************************************
*    desc:  Load the light info from XML file
*  
*    param: std::string filePath - path to file
************************************************************************/
void CLightLst::LoadFromXML( string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "lightList" );

    // Get the number lights defined used
    int lightCount = mainNode.nChildNode();

    for( int i = 0; i < lightCount; ++i )
    {
        // get a node to the light
        XMLNode lightNode = mainNode.getChildNode("light", i);

        string name = lightNode.getAttribute("name");
        string type = lightNode.getAttribute("type");

        lightTypeIter = lightTypeMap.find( type );

        if( lightTypeIter == lightTypeMap.end() )
            throw NExcept::CCriticalException("Light List Load Error!",
                boost::str( boost::format("Error Loading file (%s).\n\n%s\nLine: %s") % filePath.c_str() % __FUNCTION__ % __LINE__ ));

        // Allocate and set the type
        CLight * pLight = new CLight( lightTypeMap[type] );

        // add the light to the light container
        spLightLstMap.insert(name, pLight);
        lightVecLst.push_back(pLight);

        // Load the light from xml
        pLight->LoadFromXML( lightNode );

        // Does this light cast a shadow?
        // If so, hold on to the pointer for shadow map rendering
        if( pLight->GetCastShadow() )
            shadowCastingLight = pLight;
    }

}   // LoadFromXML


/************************************************************************
*    desc:  Turn the light on or off
*  
*    param: string &name = name of the light
*           bool enable - on or off
************************************************************************/
void CLightLst::Enable( string & name, bool enable )
{
    lightLstMapIter = spLightLstMap.find(name);

    if( lightLstMapIter != spLightLstMap.end() )
        lightLstMapIter->second->SetEnable( enable );

}	// Enable


/************************************************************************
*    desc:  Get the number of lights
*
*    ret: unsigned int - Number of lights loaded
************************************************************************/
size_t CLightLst::GetCount()
{
    return lightVecLst.size();
}


/************************************************************************
*    desc:  Get the light
*
*    param: int index - index into light list
*
*    ret: CLight - reference to light
************************************************************************/
CLight * CLightLst::GetLight( int index )
{
    return lightVecLst[index];
}


/************************************************************************
*    desc:  Check if the light is on
*  
*    param: string &name = name of the light
*    
*    return: bool - true if light is on
************************************************************************/
bool CLightLst::IsLightOn( string & name )
{
    lightLstMapIter = spLightLstMap.find(name);

    if( lightLstMapIter != spLightLstMap.end() )
        return lightLstMapIter->second->GetEnable();

    return false;
}


/************************************************************************
*    desc:  Do any of these lights cast a shadow
*  
*    return: bool - Is there a light that casts a shadow
************************************************************************/
bool CLightLst::IsShadowCast()
{
    return (shadowCastingLight != NULL);
}


/************************************************************************
*    desc:  Get the position of the shadow casting light
*  
*    return: position
************************************************************************/
CPoint & CLightLst::GetShadowCastLightPos()
{
    if( shadowCastingLight != NULL )
        //lightToCamera = shadowCastingLight->GetPos().GetInvert();
        lightToCamera = -shadowCastingLight->GetPos();

    return lightToCamera;
}


/************************************************************************
*    desc:  Get the rotation of the shadow casting light
*  
*    return: rotation
************************************************************************/
CPoint & CLightLst::GetShadowCastLightRot()
{
    if( shadowCastingLight != NULL )
        lightToCamera = -shadowCastingLight->GetRotation();

    return lightToCamera;
}


/************************************************************************
*    desc:  Add a light to the light list                                                        
************************************************************************/
void CLightLst::AddLight( string & name, CLight * pLight )
{	
    spLightLstMap.insert(name, pLight);
    lightVecLst.push_back(pLight);

    // Does this light cast a shadow?
    // If so, hold on to the pointer for shadow map rendering
    if( pLight->GetCastShadow() )
        shadowCastingLight = pLight;

}   // AddLight


/************************************************************************
*    desc:  Set the shader lights                                                          
************************************************************************/
void CLightLst::SetShaderLights( CEffectData * pEffectData )
{
    int lightType[MAX_LIGHTS];
    D3DXVECTOR4 lightPos[MAX_LIGHTS];
    D3DXVECTOR4 lightDif[MAX_LIGHTS];
    float lightRadius[MAX_LIGHTS];
    D3DXVECTOR4 ambient(0,0,0,1);

    int lightCounter(0);

    for( size_t i = 0; i < lightVecLst.size(); ++i )
    {
        // Ambient is handled seperately
        if( lightVecLst[i]->GetType() != ELT_AMBIENT )
        {
            //CPoint pos = lightVecLst[i]->GetPos();
            CPoint pos = lightVecLst[i]->GetPos();
            CPoint diffuse = lightVecLst[i]->GetDiffuse();

            // Use position for direction to save on shader space
            if( lightVecLst[i]->GetType() == ELT_DIRECTIONAL )
                pos = lightVecLst[i]->GetDirection();

            // only use the enabled lights
            if(lightVecLst[i]->GetEnable())
            {
                assert(lightCounter < MAX_LIGHTS);

                lightPos[lightCounter].x = pos.x;
                lightPos[lightCounter].y = pos.y;
                lightPos[lightCounter].z = pos.z;
                lightPos[lightCounter].w = 1.f;

                lightDif[lightCounter].x = diffuse.x;
                lightDif[lightCounter].y = diffuse.y;
                lightDif[lightCounter].z = diffuse.z;
                lightDif[lightCounter].w = 1.f;

                lightRadius[lightCounter] = lightVecLst[i]->GetRadius();
                lightType[lightCounter] = lightVecLst[i]->GetType();

                ++lightCounter;
            }
        }
        else
        {
            // Only use one ambient light
            if(lightVecLst[i]->GetEnable())
            {
                CPoint diffuse = lightVecLst[i]->GetDiffuse();

                ambient.x = diffuse.x;
                ambient.y = diffuse.y;
                ambient.z = diffuse.z;
                ambient.w = 1.f;
            }
        }
    }

    // Get the number of shader lights
    int shaderLightCount = CShader::Instance().GetElementCount( pEffectData, "lightType");
    if( lightCounter > shaderLightCount )
        throw NExcept::CCriticalException("Light List Load Error!",
                    boost::str( boost::format("Number of lights exceeds defined shader light count (%d/%d).\n\n%s\nLine: %s") % lightCounter % shaderLightCount % __FUNCTION__ % __LINE__ ));

    // Copy the data to the shader
    CShader::Instance().SetEffectValue( pEffectData, "lightCount", lightCounter );
    CShader::Instance().SetEffectValue( pEffectData, "lightType", lightCounter, &lightType[0] );
    CShader::Instance().SetEffectValue( pEffectData, "lightPos", lightCounter, &lightPos[0] );
    CShader::Instance().SetEffectValue( pEffectData, "lightDif", lightCounter, &lightDif[0] );
    CShader::Instance().SetEffectValue( pEffectData, "lightRadius", lightCounter, &lightRadius[0] );
    CShader::Instance().SetEffectValue( pEffectData, "ambient", ambient );

}	// SetShaderLights
