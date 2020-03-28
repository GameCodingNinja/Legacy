/************************************************************************
*    FILE NAME:       scriptstep.cpp
*
*    DESCRIPTION:    Class used to precess certain script actions at 
*					 the same time
************************************************************************/

// Physical component dependency
#include <scripting/scriptstep.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependency
//#include <scripting/script.h>
#include <scripting/generalscriptdefs.h>
#include <scripting/framescriptdefs.h>
#include <scripting/colorscriptdefs.h>
#include <scripting/positionscriptdefs.h>
#include <scripting/rotationscriptdefs.h>
#include <scripting/scalescriptdefs.h>
#include <scripting/soundscriptdefs.h>
//#include <scripting/iscriptobject.h>
#include <scripting/scriptobject.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CScriptStep::CScriptStep()
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CScriptStep::~CScriptStep()
{
}   // Destructer


/************************************************************************
*    desc:  Load the script from node
*  
*    param: node
************************************************************************/
void CScriptStep::LoadFromNode( XMLNode & node )
{
    // Figure out which child nodes are present
    XMLNode setFrameNode = node.getChildNode("SetFrame");
    if( !setFrameNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptSetFrame() );
        spScriptVec.back().SetFrame( atoi(setFrameNode.getAttribute("frame")) );
    }

    XMLNode setVisibleNode = node.getChildNode("SetVisible");
    if( !setVisibleNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptSetVisible() );

        if( string(setVisibleNode.getAttribute("visible")) == "true" )
            spScriptVec.back().SetVisible( true );
    }

    XMLNode setHoldNode = node.getChildNode("SetHold");
    if( !setHoldNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptSetHold() );
        spScriptVec.back().SetTime( atof(setHoldNode.getAttribute("hold")) );
    }

    XMLNode setAnimateFrame = node.getChildNode("AnimateFrame");
    if( !setAnimateFrame.isEmpty() )
    {
        spScriptVec.push_back( new CScriptAnimateFrame() );
        spScriptVec.back().SetStartFrame( atoi(setAnimateFrame.getAttribute("frameStart")) );
        spScriptVec.back().SetEndFrame( atoi(setAnimateFrame.getAttribute("frameEnd")) );
        spScriptVec.back().SetTime( 1000 / atof(setAnimateFrame.getAttribute("fps")) );
    }

    XMLNode setAnimate = node.getChildNode("Animate");
    if( !setAnimate.isEmpty() )
    {
        spScriptVec.push_back( new CScriptAnimate() );
        spScriptVec.back().SetTime( 1000 / atof( setAnimate.getAttribute("fps") ) );
    }

    //////////////////////////////////////
    // Color Scripts
    //////////////////////////////////////
                
    // SetColor Script - Sets the color values to a specific amount when called.
    XMLNode setColorNode = node.getChildNode("SetColor");
    if( !setColorNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptSetColor() );

        if( setColorNode.isAttributeSet("r") )
            spScriptVec.back().SetR( atof(setColorNode.getAttribute("r")) );

        if( setColorNode.isAttributeSet("g") )
            spScriptVec.back().SetG( atof(setColorNode.getAttribute("g")) );

        if( setColorNode.isAttributeSet("b") )
            spScriptVec.back().SetB( atof(setColorNode.getAttribute("b")) );

        if( setColorNode.isAttributeSet("a") )
            spScriptVec.back().SetA( atof(setColorNode.getAttribute("a")) );

        if( setColorNode.isAttributeSet("resetDefault") && (string(setColorNode.getAttribute("resetDefault")) == "true") )
            spScriptVec.back().ResetDefaultColor();

    }

    // SetColorToDefault Script - Resets the color back to its default color
    XMLNode setColorToDefaultNode = node.getChildNode("SetColorToDefault");
    if( !setColorToDefaultNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptSetColorToDefault() );

        if( setColorToDefaultNode.isAttributeSet("r") )
            spScriptVec.back().SetFlagR();

        if( setColorToDefaultNode.isAttributeSet("g") )
            spScriptVec.back().SetFlagG();

        if( setColorToDefaultNode.isAttributeSet("b") )
            spScriptVec.back().SetFlagB();

        if( setColorToDefaultNode.isAttributeSet("a") )
            spScriptVec.back().SetFlagA();
    }
                
    // IncColor Script - Adds a specific amount to the current color value when called.
    XMLNode incColorNode = node.getChildNode("IncColor");
    if( !incColorNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptIncColor() );

        if( incColorNode.isAttributeSet("r") )
            spScriptVec.back().SetR( atof(incColorNode.getAttribute("r")) );

        if( incColorNode.isAttributeSet("g") )
            spScriptVec.back().SetG( atof(incColorNode.getAttribute("g")) );

        if( incColorNode.isAttributeSet("b") )
            spScriptVec.back().SetB( atof(incColorNode.getAttribute("b")) );

        if( incColorNode.isAttributeSet("a") )
            spScriptVec.back().SetA( atof(incColorNode.getAttribute("a")) );
    }
                
    // ColorTo Script - Increments the current color to a specific color at a set speed or time.
    XMLNode colorToNode = node.getChildNode("ColorTo");
    if( !colorToNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptColorTo() );

        string method = colorToNode.getAttribute("method");

        if( method == "time" )
        {
            spScriptVec.back().SetTime( atof(colorToNode.getAttribute("time")) );
        }
        else if ( method == "speed" )
        {
            spScriptVec.back().SetSpeed( atof(colorToNode.getAttribute("speed")) );
        }
        else
            throw NExcept::CCriticalException("Script Step Load Error!",
                boost::str( boost::format("Neither time nor speed was specified as a method for a script call: ColorTo.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        if( colorToNode.isAttributeSet("r") )
            spScriptVec.back().SetR( atof(colorToNode.getAttribute("r")) );

        if( colorToNode.isAttributeSet("g") )
            spScriptVec.back().SetG( atof(colorToNode.getAttribute("g")) );

        if( colorToNode.isAttributeSet("b") )
            spScriptVec.back().SetB( atof(colorToNode.getAttribute("b")) );

        if( colorToNode.isAttributeSet("a") )
            spScriptVec.back().SetA( atof(colorToNode.getAttribute("a")) );
    }

    // ColorToDefault Script - Increments the current color to its default color at a set speed or time.
    XMLNode colorToDefaultNode = node.getChildNode("ColorToDefault");
    if( !colorToDefaultNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptColorToDefault() );

        string method = colorToDefaultNode.getAttribute("method");

        if( method == "time" )
        {
            spScriptVec.back().SetTime( atof(colorToDefaultNode.getAttribute("time")) );
        }
        else if ( method == "speed" )
        {
            spScriptVec.back().SetSpeed( atof(colorToDefaultNode.getAttribute("speed")) );
        }
        else
            throw NExcept::CCriticalException("Script Step Load Error!",
                boost::str( boost::format("Neither time nor speed was specified as a method for a script call: ColorToDefault.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        // Since all we are doing is setting a flag, it doesn't matter what the actual values are
        if( colorToDefaultNode.isAttributeSet("r") )
            spScriptVec.back().SetFlagR();

        if( colorToDefaultNode.isAttributeSet("g") )
            spScriptVec.back().SetFlagG();

        if( colorToDefaultNode.isAttributeSet("b") )
            spScriptVec.back().SetFlagB();

        if( colorToDefaultNode.isAttributeSet("a") )
            spScriptVec.back().SetFlagA();
    }

    // Color Script - Increments the current color a specific amount at a set speed or time.
    XMLNode colorNode = node.getChildNode("Color");
    if( !colorNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptColor() );

        string method = colorNode.getAttribute("method");

        if( method == "time" )
        {
            spScriptVec.back().SetTime( atof(colorNode.getAttribute("time")) );
        }
        else if ( method == "speed" )
        {
            spScriptVec.back().SetSpeed( atof(colorNode.getAttribute("speed")) );
        }
        else
            throw NExcept::CCriticalException("Script Step Load Error!",
                boost::str( boost::format("Neither time nor speed was specified as a method for a script call: Color.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        if( colorNode.isAttributeSet("r") )
            spScriptVec.back().SetR( atof(colorNode.getAttribute("r")) );

        if( colorNode.isAttributeSet("g") )
            spScriptVec.back().SetG( atof(colorNode.getAttribute("g")) );

        if( colorNode.isAttributeSet("b") )
            spScriptVec.back().SetB( atof(colorNode.getAttribute("b")) );

        if( colorNode.isAttributeSet("a") )
            spScriptVec.back().SetA( atof(colorNode.getAttribute("a")) );
    }

    //////////////////////////////////////
    // Sound Scripts
    //////////////////////////////////////

    // PlaySound Script - Plays a specified sound cue.
    XMLNode playSoundNode = node.getChildNode("PlaySound");
    if( !playSoundNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptPlaySound() );

        string sound = playSoundNode.getAttribute("cue");

        spScriptVec.back().SetSound( sound );
    }

    // StopSound Script - Stops a specified sound cue.
    XMLNode stopSoundNode = node.getChildNode("StopSound");
    if( !stopSoundNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptStopSound() );

        string sound = stopSoundNode.getAttribute("cue");

        spScriptVec.back().SetSound( sound );
    }

    // PauseSound Script - Pauses a specified sound cue.
    XMLNode pauseSoundNode = node.getChildNode("PauseSound");
    if( !pauseSoundNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptPauseSound() );

        string sound = pauseSoundNode.getAttribute("cue");

        spScriptVec.back().SetSound( sound );
    }

    // UnpauseSound Script - Unpauses a specified sound cue.
    XMLNode unpauseSoundNode = node.getChildNode("UnpauseSound");
    if( !unpauseSoundNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptUnpauseSound() );

        string sound = unpauseSoundNode.getAttribute("cue");

        spScriptVec.back().SetSound( sound );
    }

    //////////////////////////////////////
    // Position Commands
    //////////////////////////////////////
    
    // SetPosition Script - Sets the position values to a specific amount when called.
    XMLNode setPosNode = node.getChildNode("SetPosition");
    if( !setPosNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptSetPosition() );

        if( setPosNode.isAttributeSet("f_x") )
            spScriptVec.back().SetX( (float)atof(setPosNode.getAttribute("f_x")) );

        if( setPosNode.isAttributeSet("f_y") )
            spScriptVec.back().SetY( (float)atof(setPosNode.getAttribute("f_y")) );

        if( setPosNode.isAttributeSet("f_z") )
            spScriptVec.back().SetZ( (float)atof(setPosNode.getAttribute("f_z")) );

        if( setPosNode.isAttributeSet("i_x") )
            spScriptVec.back().SetX( atoi(setPosNode.getAttribute("i_x")) );

        if( setPosNode.isAttributeSet("i_y") )
            spScriptVec.back().SetY( atoi(setPosNode.getAttribute("i_y")) );

        if( setPosNode.isAttributeSet("i_z") )
            spScriptVec.back().SetZ( atoi(setPosNode.getAttribute("i_z")) );

    }
                
    // IncPosition Script - Adds a specific amount to the current position value when called.
    XMLNode incPosNode = node.getChildNode("IncPosition");
    if( !incPosNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptIncPosition() );

        if( incPosNode.isAttributeSet("f_x") )
            spScriptVec.back().SetX( (float)atof(incPosNode.getAttribute("f_x")) );

        if( incPosNode.isAttributeSet("f_y") )
            spScriptVec.back().SetY( (float)atof(incPosNode.getAttribute("f_y")) );

        if( incPosNode.isAttributeSet("f_z") )
            spScriptVec.back().SetZ( (float)atof(incPosNode.getAttribute("f_z")) );

        if( incPosNode.isAttributeSet("i_x") )
            spScriptVec.back().SetX( atoi(incPosNode.getAttribute("i_x")) );

        if( incPosNode.isAttributeSet("i_y") )
            spScriptVec.back().SetY( atoi(incPosNode.getAttribute("i_y")) );

        if( incPosNode.isAttributeSet("i_z") )
            spScriptVec.back().SetZ( atoi(incPosNode.getAttribute("i_z")) );
    }
                
    // MoveTo Script - Increments the current position to a specific position at a set speed or time.
    XMLNode moveToNode = node.getChildNode("MoveTo");
    if( !moveToNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptMoveTo() );

        string method = moveToNode.getAttribute("method");

        if( method == "time" )
        {
            spScriptVec.back().SetTime( atof(moveToNode.getAttribute("time")) );
        }
        else if ( method == "speed" )
        {
            spScriptVec.back().SetSpeed( atof(moveToNode.getAttribute("speed")) );
        }
        else
            throw NExcept::CCriticalException("Script Step Load Error!",
                boost::str( boost::format("Neither time nor speed was specified as a method for a script call: ColorTo.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        if( moveToNode.isAttributeSet("f_x") )
            spScriptVec.back().SetX( (float)atof(moveToNode.getAttribute("f_x")) );

        if( moveToNode.isAttributeSet("f_y") )
            spScriptVec.back().SetY( (float)atof(moveToNode.getAttribute("f_y")) );

        if( moveToNode.isAttributeSet("f_z") )
            spScriptVec.back().SetZ( (float)atof(moveToNode.getAttribute("f_z")) );

        if( moveToNode.isAttributeSet("i_x") )
            spScriptVec.back().SetX( atoi(moveToNode.getAttribute("i_x")) );

        if( moveToNode.isAttributeSet("i_y") )
            spScriptVec.back().SetY( atoi(moveToNode.getAttribute("i_y")) );

        if( moveToNode.isAttributeSet("i_z") )
            spScriptVec.back().SetZ( atoi(moveToNode.getAttribute("i_z")) );
    }

    // Move Script - Increments the current position a specific amount at a set speed or time.
    XMLNode moveNode = node.getChildNode("Move");
    if( !moveNode.isEmpty() )
    {
        spScriptVec.push_back( new CScriptMove() );

        string method = moveNode.getAttribute("method");

        if( method == "time" )
        {
            spScriptVec.back().SetTime( atof(moveNode.getAttribute("time")) );
        }
        else if ( method == "speed" )
        {
            spScriptVec.back().SetSpeed( atof(moveNode.getAttribute("speed")) );
        }
        else
            throw NExcept::CCriticalException("Script Step Load Error!",
                boost::str( boost::format("Neither time nor speed was specified as a method for a script call: ColorTo.\n\n%s\nLine: %s") % __FUNCTION__ % __LINE__ ));

        if( moveNode.isAttributeSet("f_x") )
            spScriptVec.back().SetX( (float)atof(moveNode.getAttribute("f_x")) );

        if( moveNode.isAttributeSet("f_y") )
            spScriptVec.back().SetY( (float)atof(moveNode.getAttribute("f_y")) );

        if( moveNode.isAttributeSet("f_z") )
            spScriptVec.back().SetZ( (float)atof(moveNode.getAttribute("f_z")) );

        if( moveNode.isAttributeSet("i_x") )
            spScriptVec.back().SetX( atoi(moveNode.getAttribute("i_x")) );

        if( moveNode.isAttributeSet("i_y") )
            spScriptVec.back().SetY( atoi(moveNode.getAttribute("i_y")) );

        if( moveNode.isAttributeSet("i_z") )
            spScriptVec.back().SetZ( atoi(moveNode.getAttribute("i_z")) );
    }
    /*
    //////////////////////////////////////
    // Rotation Commands
    //////////////////////////////////////

    // SetRotation Script - Sets the rotation values to a specific amount when called.
    XMLNode setRotationNode = stepNode.getChildNode("SetRotation");
    if( !setRotationNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptSetRotation );
                    
        if( setRotationNode.isAttributeSet("x") )
            pScriptStep->script.back()->rotation.x = atoi(setRotationNode.getAttribute("x"));

        if( setRotationNode.isAttributeSet("y") )
            pScriptStep->script.back()->rotation.y = atoi(setRotationNode.getAttribute("y"));

        if( setRotationNode.isAttributeSet("z") )
            pScriptStep->script.back()->rotation.z = atoi(setRotationNode.getAttribute("z"));
    }

    // IncRotation Script - Adds a specific amount to the current rotation values when called.
    XMLNode incRotationNode = stepNode.getChildNode("IncRotation");
    if( !incRotationNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptIncRotation );
                    
        if( incRotationNode.isAttributeSet("x") )
            pScriptStep->script.back()->rotation.x = atoi(incRotationNode.getAttribute("x"));

        if( incRotationNode.isAttributeSet("y") )
            pScriptStep->script.back()->rotation.y = atoi(incRotationNode.getAttribute("y"));

        if( incRotationNode.isAttributeSet("z") )
            pScriptStep->script.back()->rotation.z = atoi(incRotationNode.getAttribute("z"));
    }

    // RotateTo Script - Increments the current rotation to a specific rotation at a set speed or time.
    XMLNode rotateToNode = stepNode.getChildNode("RotateTo");
    if( !rotateToNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptRotateTo );

        string method = rotateToNode.getAttribute("method");

        if( method == "time" )
        {
            pScriptStep->script.back()->time = atoi(rotateToNode.getAttribute("time"));
        }
        else if ( method == "speed" )
        {
            pScriptStep->script.back()->speed = atoi(rotateToNode.getAttribute("speed"));
        }
        else
        {
            PostDebugMsg( "Neither time nor speed was specified as a method for a script call: RotateTo." );
            assert( false );
        }

        if( rotateToNode.isAttributeSet("x") )
            pScriptStep->script.back()->rotation.x = atoi(rotateToNode.getAttribute("x"));

        if( rotateToNode.isAttributeSet("y") )
            pScriptStep->script.back()->rotation.y = atoi(rotateToNode.getAttribute("y"));

        if( rotateToNode.isAttributeSet("z") )
            pScriptStep->script.back()->rotation.z = atoi(rotateToNode.getAttribute("z"));
    }

    // Rotate Script. Increments the current rotation by a set ammount at a set speed or time.
    XMLNode rotateNode = stepNode.getChildNode("Rotate");
    if( !rotateNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptRotate );

        string method = rotateNode.getAttribute("method");

        if( method == "time" )
        {
            pScriptStep->script.back()->time = atoi(rotateNode.getAttribute("time"));
        }
        else if ( method == "speed" )
        {
            pScriptStep->script.back()->speed = atoi(rotateNode.getAttribute("speed"));
        }
        else
        {
            PostDebugMsg( "Neither time nor speed was specified as a method for a script call: Rotate." );
            assert( false );
        }

        if( rotateNode.isAttributeSet("x") )
            pScriptStep->script.back()->rotation.x = atoi(rotateNode.getAttribute("x"));

        if( rotateNode.isAttributeSet("y") )
            pScriptStep->script.back()->rotation.y = atoi(rotateNode.getAttribute("y"));

        if( rotateNode.isAttributeSet("z") )
            pScriptStep->script.back()->rotation.z = atoi(rotateNode.getAttribute("z"));
    }

    //////////////////////////////////////
    // Scale Commands
    //////////////////////////////////////

    // SetScale Script - Sets the scale values to a specific amount when called.
    XMLNode setScaleNode = stepNode.getChildNode("SetScale");
    if( !setScaleNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptSetScale );
                    
        if( setScaleNode.isAttributeSet("x") )
            pScriptStep->script.back()->scale.x = atoi(setScaleNode.getAttribute("x"));

        if( setScaleNode.isAttributeSet("y") )
            pScriptStep->script.back()->scale.y = atoi(setScaleNode.getAttribute("y"));

        if( setScaleNode.isAttributeSet("z") )
            pScriptStep->script.back()->scale.z = atoi(setScaleNode.getAttribute("z"));
    }

    // IncScale Script - Adds a specific amount to the current scale values when called.
    XMLNode incScaleNode = stepNode.getChildNode("IncScale");
    if( !incScaleNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptIncScale );
                    
        if( incScaleNode.isAttributeSet("x") )
            pScriptStep->script.back()->scale.x = atoi(incScaleNode.getAttribute("x"));

        if( incScaleNode.isAttributeSet("y") )
            pScriptStep->script.back()->scale.y = atoi(incScaleNode.getAttribute("y"));

        if( incScaleNode.isAttributeSet("z") )
            pScriptStep->script.back()->scale.z = atoi(incScaleNode.getAttribute("z"));
    }

    // ScaleTo Script - Increments the current scale to a specific scale at a set speed or time.
    XMLNode scaleToNode = stepNode.getChildNode("ScaleTo");
    if( !scaleToNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptScaleTo );
                    
        string method = scaleToNode.getAttribute("method");

        if( method == "time" )
        {
            pScriptStep->script.back()->time = atoi(scaleToNode.getAttribute("time"));
        }
        else if ( method == "speed" )
        {
            pScriptStep->script.back()->speed = atoi(scaleToNode.getAttribute("speed"));
        }
        else
        {
            PostDebugMsg( "Neither time nor speed was specified as a method for a script call: ScaleTo." );
            assert( false );
        }

        if( scaleToNode.isAttributeSet("x") )
            pScriptStep->script.back()->scale.x = atoi(scaleToNode.getAttribute("x"));

        if( scaleToNode.isAttributeSet("y") )
            pScriptStep->script.back()->scale.y = atoi(scaleToNode.getAttribute("y"));

        if( scaleToNode.isAttributeSet("z") )
            pScriptStep->script.back()->scale.z = atoi(scaleToNode.getAttribute("z"));
    }

    // Scale Script. Increments the current scale by a set ammount at a set speed or time.
    XMLNode scaleNode = stepNode.getChildNode("Scale");
    if( !scaleNode.isEmpty() )
    {
        pScriptStep->script.push_back( new CScriptScale );
                    
        string method = scaleNode.getAttribute("method");

        if( method == "time" )
        {
            pScriptStep->script.back()->time = atoi(scaleNode.getAttribute("time"));
        }
        else if ( method == "speed" )
        {
            pScriptStep->script.back()->speed = atoi(scaleNode.getAttribute("speed"));
        }
        else
        {
            PostDebugMsg( "Neither time nor speed was specified as a method for a script call: Scale." );
            assert( false );
        }

        if( scaleNode.isAttributeSet("x") )
            pScriptStep->script.back()->scale.x = atoi(scaleNode.getAttribute("x"));

        if( scaleNode.isAttributeSet("y") )
            pScriptStep->script.back()->scale.y = atoi(scaleNode.getAttribute("y"));

        if( scaleNode.isAttributeSet("z") )
            pScriptStep->script.back()->scale.z = atoi(scaleNode.getAttribute("z"));
    }*/
}	// LoadFromNode


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CScriptStep::CScriptStep( CScriptStep * pCopy )
{
    spScriptVec.reserve(pCopy->spScriptVec.size());

    for( size_t i = 0; i < pCopy->spScriptVec.size(); ++i )
    {
        CScript * pTmpScript;

        switch( pCopy->spScriptVec[i].type )
        {
            ////////////////////////////////////////////////////////////
            //	FRAME SCRIPTS
            ////////////////////////////////////////////////////////////
            case EST_SET_FRAME:
                pTmpScript = new CScriptSetFrame( pCopy->spScriptVec[i] );
                break;

            case EST_ANIMATE:
                pTmpScript = new CScriptAnimate( pCopy->spScriptVec[i] );
                break;

            case EST_ANIMATE_FRAME:
                pTmpScript = new CScriptAnimateFrame( pCopy->spScriptVec[i] );
                break;

            ////////////////////////////////////////////////////////////
            //	COLOR SCRIPTS
            ////////////////////////////////////////////////////////////
            case EST_SET_COLOR:
                pTmpScript = new CScriptSetColor( pCopy->spScriptVec[i] );
                break;

            case EST_SET_COLOR_TO_DEFAULT:
                pTmpScript = new CScriptSetColorToDefault( pCopy->spScriptVec[i] );
                break;

            case EST_INC_COLOR:
                pTmpScript = new CScriptIncColor( pCopy->spScriptVec[i] );
                break;

            case EST_COLOR_TO:
                pTmpScript = new CScriptColorTo( pCopy->spScriptVec[i] );
                break;

            case EST_COLOR_TO_DEFAULT:
                pTmpScript = new CScriptColorToDefault( pCopy->spScriptVec[i] );
                break;
            
            case EST_COLOR:
                pTmpScript = new CScriptColor( pCopy->spScriptVec[i] );
                break;

            ////////////////////////////////////////////////////////////
            //	POSITION SCRIPTS
            ////////////////////////////////////////////////////////////
            case EST_SET_POSITION:
                pTmpScript = new CScriptSetPosition( pCopy->spScriptVec[i] );
                break;

            case EST_INC_POSITION:
                pTmpScript = new CScriptIncPosition( pCopy->spScriptVec[i] );
                break;

            case EST_MOVE_TO:
                pTmpScript = new CScriptMoveTo( pCopy->spScriptVec[i] );
                break;

            case EST_MOVE:
                pTmpScript = new CScriptMove( pCopy->spScriptVec[i] );
                break;
/*
            ////////////////////////////////////////////////////////////
            //	ROTATION SCRIPTS
            ////////////////////////////////////////////////////////////
            else if( pCopy->pScriptVector[i]->type == EST_SET_ROTATION )
            case EST_SET_ROTATION:
                pTmpScript = new CScriptSetRotation( pCopy->spScriptVec[i] );
                break;

            else if( pCopy->pScriptVector[i]->type == EST_INC_ROTATION )
            case EST_INC_ROTATION:
                pTmpScript = new CScriptIncRotation( pCopy->spScriptVec[i] );
                break;

            else if( pCopy->pScriptVector[i]->type == EST_ROTATE_TO )
            case EST_ROTATE_TO:
                pTmpScript = new CScriptRotateTo( pCopy->spScriptVec[i] );
                break;

            else if( pCopy->pScriptVector[i]->type == EST_ROTATE )
            case EST_ROTATE:
                pTmpScript = new CScriptRotate( pCopy->spScriptVec[i] );
                break;

            ////////////////////////////////////////////////////////////
            //	SCALE SCRIPTS
            ////////////////////////////////////////////////////////////
            else if( pCopy->pScriptVector[i]->type == EST_SET_SCALE )
            case EST_SET_SCALE:
                pTmpScript = new CScriptSetScale( pCopy->spScriptVec[i] );
                break;

            else if( pCopy->pScriptVector[i]->type == EST_INC_SCALE )
            case EST_INC_SCALE:
                pTmpScript = new CScriptIncScale( pCopy->spScriptVec[i] );
                break;

            else if( pCopy->pScriptVector[i]->type == EST_SCALE_TO )
            case EST_SCALE_TO:
                pTmpScript = new CScriptScaleTo( pCopy->spScriptVec[i] );
                break;

            else if( pCopy->pScriptVector[i]->type == EST_SCALE )
            case EST_SCALE:
                pTmpScript = new CScriptScale( pCopy->spScriptVec[i] );
                break;
                */

            ////////////////////////////////////////////////////////////
            //	SOUND SCRIPTS
            ////////////////////////////////////////////////////////////
            case EST_PLAY_SOUND:
                pTmpScript = new CScriptPlaySound( pCopy->spScriptVec[i] );
                break;

            case EST_STOP_SOUND:
                pTmpScript = new CScriptStopSound( pCopy->spScriptVec[i] );
                break;

            case EST_PAUSE_SOUND:
                pTmpScript = new CScriptPauseSound( pCopy->spScriptVec[i] );
                break;

            case EST_UNPAUSE_SOUND:
                pTmpScript = new CScriptUnpauseSound( pCopy->spScriptVec[i] );
                break;

            ////////////////////////////////////////////////////////////
            //	GENERAL SCRIPTS
            ////////////////////////////////////////////////////////////
            //else if( pCopy->spScriptVec[i].type == EST_SET_HOLD )
            case EST_SET_HOLD:
                pTmpScript = new CScriptSetHold( pCopy->spScriptVec[i] );
                break;

            //else if( pCopy->spScriptVec[i].type == EST_SET_VISIBLE )
            case EST_SET_VISIBLE:
                pTmpScript = new CScriptSetVisible( pCopy->spScriptVec[i] );
                break;
        }
        // Insert the script into the vector
        spScriptVec.push_back( pTmpScript );
    }

}   // Constructer


/************************************************************************
*    desc:  Update the script step
*
*    param: CScriptObject * pScriptObj - object to update
*
*	 ret:	bool - update finished
************************************************************************/
bool CScriptStep::Update( CScriptObject * pScriptObj )
{
    // The returned value determining if the script step is complete
    bool finished = true;

    // Updates all scripts in the step
    for( size_t i = 0; i < spScriptVec.size(); ++i )
    {
        // If any script is not finished, the step is not finished
        if( !spScriptVec[i].Update( pScriptObj ) )
        {
            finished = false;
        }
    }

    return finished;

}	// Update


/************************************************************************
*    desc:  Reset the scripts in the step
************************************************************************/
void CScriptStep::Reset()
{
    for( size_t i = 0; i < spScriptVec.size(); ++i )
        spScriptVec[i].Reset();

}	// Reset