/************************************************************************
*    FILE NAME:       stage.cpp
*
*    DESCRIPTION:     Stage class
************************************************************************/

// Physical component dependency
#include "stage.h"

// DirectX lib dependencies
#include "d3dx9.h"

// Game lib dependencies
#include "xmlParser\\xmlParser.h"
#include "xwindow.h"
#include "misc\\genfunc.h"
#include "misc\\shader.h"
#include "misc\\deletefuncs.h"
#include "camera.h"
#include "objectdatalist.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CStage::CStage()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CStage::~CStage()
{
    DeleteVectorPointers(allGroupsVector);
    DeleteVectorPointers(warpSpotVector);

    //Delete(pSkyMesh);

}   // Destructer


/************************************************************************
*    desc:  load the meshs from xml file
*
*    param: String filePath - path to xml file
************************************************************************/
bool CStage::LoadFromXML( string & filePath )
{
    // this open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "gameObjects" );
    XMLNode objectListNode = mainNode.getChildNode("objectList");

    stageName = mainNode.getChildNode("stageName").getAttribute("name");

    string lightFile = mainNode.getChildNode("lightList").getAttribute("file");

    lightList.LoadFromXML( lightFile );

    XMLNode camPosNode = mainNode.getChildNode("defaultCameraPosition");
    CPoint defaultCamPos;
    defaultCamPos.x = atof(camPosNode.getAttribute( "x" ));
    defaultCamPos.y = atof(camPosNode.getAttribute( "y" ));
    defaultCamPos.z = atof(camPosNode.getAttribute( "z" ));

    XMLNode camRotNode = mainNode.getChildNode("defaultCameraRotation");
    CPoint defaultCamRot;
    defaultCamRot.x = atof(camRotNode.getAttribute( "x" ));
    defaultCamRot.y = atof(camRotNode.getAttribute( "y" ));
    defaultCamRot.z = atof(camRotNode.getAttribute( "z" ));

    // Obtain the default camera position for this stage. It's basically used as a failsafe.
    CCamera::Instance().Translate( defaultCamPos );
    CCamera::Instance().Rotate( defaultCamRot );

    //XMLNode skyNode = mainNode.getChildNode( "skyMesh" );
    //string skyFile = skyNode.getAttribute( "file" );
    //pSkyMesh = new CSpriteGroup();
    /*
    if( skyFile != "" )
    {
        pSkyMesh->InitVisual( skyFile );

        CPoint skyPos;
        skyPos.x = atof(skyNode.getChildNode("position").getAttributeValue( E_X));
        skyPos.y = atof(skyNode.getChildNode("position").getAttributeValue( E_Y ));
        skyPos.z = atof(skyNode.getChildNode("position").getAttributeValue( E_Z ));

        CPoint skyRot;
        skyRot.x = atof(skyNode.getChildNode("rotation").getAttributeValue( E_X));
        skyRot.y = atof(skyNode.getChildNode("rotation").getAttributeValue( E_Y ));
        skyRot.z = atof(skyNode.getChildNode("rotation").getAttributeValue( E_Z ));

        pSkyMesh->SetPos( skyPos );
        pSkyMesh->SetPreTransRot( skyRot );
    }*/

    for( int i = 0; i < objectListNode.nChildNode(); i++ )
    {
        XMLNode objectNode = objectListNode.getChildNode( "object", i );

        CGroup * pTmpGroup;

        string objName = objectNode.getAttribute( "name" );
        string objAnimated;

        if( objectNode.isAttributeSet("animated") )
            objAnimated = objectNode.getAttribute("animated");

        bool animated = false;

        if( objAnimated == "true" )
        {
            animated = true;
        }

        // News up a temporary group
        if( animated == true )
        {
            pTmpGroup = new CAnimatedSpriteGroup();
        }
        else
        {
            pTmpGroup = new CSpriteGroup();
        }

        // The pointer is placed within a vector and map for all objects
        allGroupsVector.push_back( pTmpGroup );
        allGroupsMap.insert( make_pair( objName, pTmpGroup ) );

        /***********************************************************
        *	fileNode
        *	Gets the file paths for the sprites in the group
        ***********************************************************/
        XMLNode fileNode = objectNode.getChildNode( "file" );

        string objVisual = fileNode.getAttribute( "visual" );
        string objCollision = fileNode.getAttribute( "collision" );
        
        if( objVisual != "" )
        {
            if( !pTmpGroup->InitVisual( objVisual ) )
            {
                return false;
            }                        
        }
        if( objCollision != "" )
        {
            if( !pTmpGroup->InitCollision( objCollision ) )
            {
                return false;
            }
        }


        /***********************************************************
        *	shaderNode
        *	Gets the shader info for each objects needs
        ***********************************************************/
        CObjectInfo::CShaderStrInfo effectsInfoStr;
        XMLNode shaderLightingNode = objectNode.getChildNode( "shaderLighting" );

        // The lighting shader needs to be there
        if( shaderLightingNode.isEmpty() )
            return false;

        // Set the strings for shader lighting
        effectsInfoStr.effectStr = shaderLightingNode.getAttribute( "effect" );
        effectsInfoStr.techniqueStr = shaderLightingNode.getAttribute( "technique" );
        pTmpGroup->SetShaderLighting( effectsInfoStr );

        // Check for specular light properties
        XMLNode specPropNode = shaderLightingNode.getChildNode( "specular" );
        if( !specPropNode.isEmpty() )
        {
            pTmpGroup->SetSpecularProperties(atof(specPropNode.getAttribute( "shinniness" )),
                                             atof(specPropNode.getAttribute( "intensity" )));

            pTmpGroup->GetVisualSprite().SetSpecularProperties(pTmpGroup->GetObjectInfo().specularShinniness,
                                                               pTmpGroup->GetObjectInfo().specularIntensity);
        }

        // Check for material color
        XMLNode materialColorNode = shaderLightingNode.getChildNode( "materialColor" );
        if( !materialColorNode.isEmpty() )
        {
            pTmpGroup->SetMaterialColor(atof(materialColorNode.getAttribute( "r" )),
                                        atof(materialColorNode.getAttribute( "g" )),
                                        atof(materialColorNode.getAttribute( "b" )),
                                        atof(materialColorNode.getAttribute( "a" )));

            pTmpGroup->GetVisualSprite().SetMaterialColor(pTmpGroup->GetObjectInfo().materialColor);
        }

        XMLNode shaderShadowMapNode = objectNode.getChildNode( "shaderShadowMap" );

        // The shadow map shader doesn't need to be there
        if( !shaderShadowMapNode.isEmpty() )
        {
            effectsInfoStr.effectStr = shaderShadowMapNode.getAttribute( "effect" );
            effectsInfoStr.techniqueStr = shaderShadowMapNode.getAttribute( "technique" );
            pTmpGroup->SetShaderShadowMap( effectsInfoStr );

            pTmpGroup->GetObjectInfo().castShadow = true;

            // Check for the type of projection
            XMLNode shadowPerspectProjNode = shaderShadowMapNode.getChildNode( "perspectiveShadowProj" );

            if( !shadowPerspectProjNode.isEmpty() )
            {
                pTmpGroup->GetObjectInfo().shadowProjType = CSettings::EPT_PERSPECTIVE;
                pTmpGroup->GetObjectInfo().shadowMinZDist = atof(shadowPerspectProjNode.getAttribute( "minZDist" ));
                pTmpGroup->GetObjectInfo().shadowMaxZDist = atof(shadowPerspectProjNode.getAttribute( "maxZDist" ));
                pTmpGroup->GetObjectInfo().shadowBias = atof(shadowPerspectProjNode.getAttribute( "bias" ));

                pTmpGroup->GetVisualSprite().SetPerspectiveProjMatrix( pTmpGroup->GetObjectInfo().shadowMinZDist,
                                                                       pTmpGroup->GetObjectInfo().shadowMaxZDist,
                                                                       pTmpGroup->GetObjectInfo().shadowBias );
            }

            XMLNode shadowOrthoNode = shaderShadowMapNode.getChildNode( "orthographicShadowProj" );

            if( !shadowOrthoNode.isEmpty() )
            {
                pTmpGroup->GetObjectInfo().shadowProjType = CSettings::EPT_ORTHOGRAPHIC;
                pTmpGroup->GetObjectInfo().shadowMinZDist = atof(shadowOrthoNode.getAttribute( "minZDist" ));
                pTmpGroup->GetObjectInfo().shadowMaxZDist = atof(shadowOrthoNode.getAttribute( "maxZDist" ));
                pTmpGroup->GetObjectInfo().shadowBias = atof(shadowOrthoNode.getAttribute( "bias" ));
                pTmpGroup->GetObjectInfo().shadowOthoScrnPercent = atof(shadowOrthoNode.getAttribute( "screenPercentage" ));

                pTmpGroup->GetVisualSprite().SetOrthographicProjMatrix( pTmpGroup->GetObjectInfo().shadowMinZDist,
                                                                        pTmpGroup->GetObjectInfo().shadowMaxZDist,
                                                                        pTmpGroup->GetObjectInfo().shadowOthoScrnPercent,
                                                                        pTmpGroup->GetObjectInfo().shadowBias );
            }
        }
        

        /***********************************************************
        *	collideNode & colideWithNode
        *	colideWithNode is the childNode of collideNode
        *   Contains the information used for the CCollisionStats 
        *	object and determines what the object can collide against.
        ***********************************************************/
        if( objCollision != "" )
        {
            XMLNode collideNode = objectNode.getChildNode( "collideWith" );
            string colPlayer = collideNode.getAttribute( "player" );
            float fPad = atof( collideNode.getAttribute( "fpad" ) );
            float wPad = atof( collideNode.getAttribute( "wpad" ) );

            pTmpGroup->GetCollisions().SetFloorPad( fPad );
            pTmpGroup->GetCollisions().SetWallPad( wPad );

            if( colPlayer == "true")
            {
                pTmpGroup->GetObjectInfo().collidePlayer = true;
            }

            if( collideNode.nChildNode() > 0 )
            {		
                // Reads in the statistics if the object does not colide with the player
                for(int k = 0; k < collideNode.nChildNode(); k++)
                {
                    XMLNode colideWithNode = collideNode.getChildNode(k);

                    string collide = colideWithNode.getAttributeValue(0);
                    
                    pTmpGroup->GetCollisions().AddColideWith( collide );
                }
            }
        }


        /***********************************************************
        *	otherNode
        *	Decides whether or not an object will be culled or fall
        *	with gravity
        ***********************************************************/            
        XMLNode otherNode = objectNode.getChildNode( "otherProperties" );
        string objGravity = otherNode.getAttribute( "gravity" );
        string objStatic = otherNode.getAttribute( "static" );
        string objAlpha = otherNode.getAttribute( "alpha" );
        string objDefaultAnim = otherNode.getAttribute( "defaultAnimation" );

        if( objGravity == "true" )
        {
            pTmpGroup->GetObjectInfo().gravity = true;
        }
        if( objStatic == "true" )
        {
            pTmpGroup->GetObjectInfo().staticObj = true;
        }
        if( animated == true )
        {
            pTmpGroup->GetObjectInfo().animated = true;
        }
        if( objDefaultAnim != "" )
        {
            pTmpGroup->SetAnimation( objDefaultAnim );
        }


        /***********************************************************
        *	positionNode
        *	Has the position of the sprite group
        ***********************************************************/
        XMLNode positionNode = objectNode.getChildNode( "position" );
        CPoint pos;
        pos.x = atof( positionNode.getAttribute( "x" ) );
        pos.y = atof( positionNode.getAttribute( "y" ) );
        pos.z = atof( positionNode.getAttribute( "z" ) );


        /***********************************************************
        *	preTransRotNode
        *	Has the pre-translation rotation of the sprite group
        ***********************************************************/
        XMLNode preTransRotNode = objectNode.getChildNode( "preTransRot" );
        CPoint preTransRot;
        preTransRot.x = atof( preTransRotNode.getAttribute( "x" ) );
        preTransRot.y = atof( preTransRotNode.getAttribute( "y" ) );
        preTransRot.z = atof( preTransRotNode.getAttribute( "z" ) );


        /**********************************************************
        *	scaleNode
        *	Has the object scale information. A scale of 1 = no change
        ***********************************************************/
        XMLNode scaleNode = objectNode.getChildNode( "scale" );
        float scale = atof( scaleNode.getAttribute( "scale" ) );

        // Apply the position and rotations to the pTmpSpriteGroup
        pTmpGroup->SetPos( pos );
        pTmpGroup->SetPreTransRot( preTransRot );
        pTmpGroup->SetScale( scale );

        // If the object collides with the player, place the pointer here
        if( pTmpGroup->GetObjectInfo().collidePlayer == true )
        {
            colPlayerVector.push_back( pTmpGroup );
        }

        // If the object collides with other objects, place the pointer here
        if( !pTmpGroup->GetCollisions().GetColideWith().empty() )
        {
            colObjVector.push_back( pTmpGroup );
        }

        // If the object has collision, place the pointer here
        if( objCollision != "" )
        {
            allColVector.push_back( pTmpGroup );
        }

        // If the object is static and will not move or animate, place the pointer here
        if( objStatic == "true" )
        {
            staticVector.push_back( pTmpGroup );
        }
        // Otherwise the pointer is placed here
        else
        {
            nonStaticVector.push_back( pTmpGroup );
        }

        // If the object contains alpha gradiant textures that need to be sorted, place the pointer here
        if( objAlpha == "true" )
        {
            alphaVector.push_back( pTmpGroup );
        }
        // Otherwise the pointer is placed here
        else
        {
            nonAlphaVector.push_back( pTmpGroup );
        }

        // If the object is animated, place the pointer here
        if( animated == true )
        {
            animatedVector.push_back( pTmpGroup );
        }

        // Add objects to be rendered to the shadow map buffer
        if( pTmpGroup->GetObjectInfo().castShadow )
            shadowCastVector.push_back( pTmpGroup );
    }


    /////////////////////////////////////////////////////////////
    //	HotSpots
    /////////////////////////////////////////////////////////////

    XMLNode hotSpotListNode = mainNode.getChildNode("hotSpotList");
    XMLNode hotSpotWarpNode = hotSpotListNode.getChildNode("hotSpotWarpList");

    for( int i = 0; i < hotSpotWarpNode.nChildNode(); i++ )
    {
        CHotSpotWarp * spot = new CHotSpotWarp();

        XMLNode hotSpotNode = hotSpotWarpNode.getChildNode(i);

        /***********************************************************
        *	destinationNode
        *	Gets the file path to the destination stage
        ***********************************************************/
        XMLNode destinationNode = hotSpotNode.getChildNode( "destination" );

        string destination = destinationNode.getAttribute( "file" );

        spot->SetDestination( destination );


        /***********************************************************
        *	spritegroupNode
        *	Gets the file path to the visual and collision sprites
        ***********************************************************/
        XMLNode spritegroupNode = hotSpotNode.getChildNode( "sprites" );
    
        string visual = spritegroupNode.getAttribute( "visual" );
        string collision = spritegroupNode.getAttribute( "collision" );

        if( !spot->SetSpriteGroup( visual, collision ) )
        {
            return false;
        }


        /***********************************************************
        *	positionNode
        *	Has the position of the sprite group
        ***********************************************************/
        XMLNode positionNode = hotSpotNode.getChildNode( "position" );
        CPoint pos;
        pos.x = atof( positionNode.getAttribute( "x" ) );
        pos.y = atof( positionNode.getAttribute( "y" ) );
        pos.z = atof( positionNode.getAttribute( "z" ) );

        spot->SetPos( pos );


        /***********************************************************
        *	preTransRotNode
        *	Has the pre-translation rotation of the sprite group
        ***********************************************************/
        XMLNode preTransRotNode = hotSpotNode.getChildNode( "preTransRot" );
        CPoint preTransRot;
        preTransRot.x = atof( preTransRotNode.getAttribute( "x" ) );
        preTransRot.y = atof( preTransRotNode.getAttribute( "y" ) );
        preTransRot.z = atof( preTransRotNode.getAttribute( "z" ) );

        spot->SetPreTransRot( preTransRot );


        /***********************************************************
        *	camPosNode
        *	Has the position of the camera after warping
        ***********************************************************/
        XMLNode camPosNode = hotSpotNode.getChildNode( "cameraPos" );
        CPoint camPos;
        camPos.x = atof( camPosNode.getAttribute( "x" ) );
        camPos.y = atof( camPosNode.getAttribute( "y" ) );
        camPos.z = atof( camPosNode.getAttribute( "z" ) );

        spot->SetCameraPos( camPos );


        /***********************************************************
        *	camRotNode
        *	Has the position of the sprite group
        ***********************************************************/
        XMLNode camRotNode = hotSpotNode.getChildNode( "cameraRot" );
        CPoint camRot;
        camRot.x = atof( camRotNode.getAttribute( "x" ) );
        camRot.y = atof( camRotNode.getAttribute( "y" ) );
        camRot.z = atof( camRotNode.getAttribute( "z" ) );

        spot->SetCameraRot( camRot );


        /***********************************************************
        *	activeNode
        *	Has the position of the sprite group
        ***********************************************************/
        XMLNode activeNode = hotSpotNode.getChildNode( "warpActive" );
        string active = activeNode.getAttribute( "active" );

        if( active == "true" )
        {
            spot->SetActive( true );
        }

        warpSpotVector.push_back( spot );
    }

    return true;

}	//LoadFromXML


/************************************************************************
*    desc:  Returns the pointer to the group
*
*	 ret:	string & groupName - name of group to get
************************************************************************/
CGroup * CStage::GetSpriteGroup( string & groupName )
{
    return allGroupsMap[groupName];
}


/************************************************************************
*    desc:  Returns the name of the stage
*
*	 ret:	string & stageName - name of the stage
************************************************************************/
string & CStage::GetStageName()
{
    return stageName;

}	// GetStageName


/************************************************************************
*    desc:  Returns the light list
*
*	 ret:	CLightLst - lightList
************************************************************************/
CLightLst & CStage::GetLightList()
{
    return lightList;

}	// GetLightList


/************************************************************************
*    desc:  Resets the vectors by deleting all elements
************************************************************************/
void CStage::Reset()
{
    //Delete( pSkyMesh );
    DeleteVectorPointers( allGroupsVector );
    DeleteVectorPointers( warpSpotVector );

    allGroupsMap.clear();
    colPlayerVector.clear();
    colObjVector.clear();
    allColVector.clear();
    staticVector.clear();
    nonStaticVector.clear();
    alphaVector.clear();
    nonAlphaVector.clear();
    animatedVector.clear();

    lightList.Reset();

}	// Reset


/************************************************************************
*    desc:  Renders the stage objects
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CStage::Render( CMatrix & matrix )
{
    for(unsigned int i = 0; i < alphaVector.size(); i++)
    {
        CObjectInfo::CShaderStrInfo & shaderStr = alphaVector[i]->GetShaderLighting();
        CShader::Instance().SetEffectAndTechnique( shaderStr.effectStr, shaderStr.techniqueStr );
        alphaVector[i]->GetVisualSprite().Render( matrix );
    }
    
    for(unsigned int i = 0; i < nonAlphaVector.size(); i++)
    {
        CObjectInfo::CShaderStrInfo & shaderStr = nonAlphaVector[i]->GetShaderLighting();
        CShader::Instance().SetEffectAndTechnique( shaderStr.effectStr, shaderStr.techniqueStr );
        nonAlphaVector[i]->GetVisualSprite().Render( matrix );
    }

}	// Render


/************************************************************************
*    desc:  Render to shadow map
*
*	 param:	CMatrix & matrix - passed in matrix class
************************************************************************/
void CStage::RenderShadowMap( CMatrix & matrix )
{
    for(unsigned int i = 0; i < shadowCastVector.size(); i++)
    {
        CObjectInfo::CShaderStrInfo & shaderStr = shadowCastVector[i]->GetShaderShadowMap();
        CShader::Instance().SetEffectAndTechnique( shaderStr.effectStr, shaderStr.techniqueStr );
        shadowCastVector[i]->GetVisualSprite().RenderShadowMap( matrix );
    }

}	// RenderShadow


/************************************************************************
*    desc:  Reacts to collision against player
*
*	 param:	CMatrix & matrix - camera move matrix
*			CCollisionCheck & cCheck -
*			float radius - player radius
*			float fpad - floor pad
*			float wpad - wall pad
************************************************************************/
void CStage::ReactToPlayerCollision( CCollisionCheck & cCheck, float radius, float fpad, float wpad )
{
    //bool warpTouched = false;

    for( unsigned int i = 0; i < colPlayerVector.size(); i++ )
    {
        if( colPlayerVector[i]->GetObjectInfo().colideActive )
        {
            cCheck.Sort( colPlayerVector[i]->ReactToCollision( CCamera::Instance().GetMoveMatrix(), CPoint(0,0,0), radius, fpad, wpad ) );
        }
    }

}	// ReactToPlayerCollision


/************************************************************************
*    desc:  Reacts to collision against other objects
*
*	 param:	CCollisionCheck & cCheck 
************************************************************************/
void CStage::ReactToObjectCollision( CCollisionCheck & cCheck )
{
    for( unsigned int i = 0; i < colObjVector.size(); i++ )
    {
        if( colObjVector[i]->GetObjectInfo().colideActive )
        {
            cCheck.Reset();

            colObjVector[i]->GetCollisionSprite().Transform();

            CCollisionStats tmpStats = colObjVector[i]->GetCollisions();
            for(unsigned int j = 0; j < tmpStats.GetColideWith().size(); j++)
            {
                cCheck.Sort( allGroupsMap[tmpStats.GetColideWith()[j]]->ReactToCollision
                           ( colObjVector[i]->GetCollisionSprite().GetCenter(), 
                             colObjVector[i]->GetCollisionSprite().GetRadius(), 
                             colObjVector[i]->GetCollisions().GetFloorPad(), 
                             colObjVector[i]->GetCollisions().GetWallPad() ) );
            }

            colObjVector[i]->IncPos( cCheck.GetFinalObjectOffset() );
        }
    } 

}	// ReactToObjectCollision


/************************************************************************
*    desc:  Allow objects suseptible to gravity to fall
*
*	 param:	float speed - speed of gravity
************************************************************************/
void CStage::ReactToGravity( float speed, float elapsedTime )
{
    for(unsigned int i = 0; i < allGroupsVector.size(); i++)
    {
        if( allGroupsVector[i]->GetObjectInfo().gravity )
        {
            allGroupsVector[i]->IncPos( 0, -0.05f * elapsedTime, 0 );
        }
    }

}	// ReactToGravity


/************************************************************************
*    desc:  If the hotspot warp has been touched, go to the hotspot's
*			destination
*
*	 param:	CHotSpotWarp & spot
************************************************************************/
bool CStage::ReactToHotSpotWarp( CCollisionCheck & cCheck, float radius, float fpad, float wpad )
{
    bool warpTouched = false;

    // Check for any collision against hotspots
    for( unsigned int i = 0; i < warpSpotVector.size(); i++ )
    {
        if( warpSpotVector[i]->GetActive() )
        {
            warpTouched = warpSpotVector[i]->GetHotSpot().GetCollisionSprite().ReactToCollision( CCamera::Instance().GetMoveMatrix(),
                                                                                                 CPoint(0,0,0), 
                                                                                                 radius, 
                                                                                                 fpad, 
                                                                                                 wpad ).IsCollision();

            if(warpTouched)
            {
                // Once Reset() is called the warpSpot will exist no more so we need to take note of
                // the destination, position, and rotation to use.
                string dest = warpSpotVector[i]->GetDestination();
                CPoint pos = warpSpotVector[i]->GetCameraPos();
                CPoint rot = warpSpotVector[i]->GetCameraRot();
                
                Reset();

                LoadFromXML( dest );

                CCamera::Instance().Reset();

                CCamera::Instance().Translate(pos);
                CCamera::Instance().Rotate(rot);

                break;
            }
        }
    }

    return warpTouched;

}	// ReactToHotSpotWarp


/************************************************************************
*    desc:  Inc the animation timer
************************************************************************/
void CStage::IncAnimationTime()
{
    for(unsigned int i = 0; i < animatedVector.size(); i++)
        animatedVector[i]->IncAnimationTime();
}