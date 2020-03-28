/************************************************************************
*    FILE NAME:       settings.h
*
*    DESCRIPTION:     game settings class
************************************************************************/

// Physical component dependency
#include <misc/settings.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/lexical_cast.hpp>

// Game lib dependencies
#include <utilities/genfunc.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CSettings::CSettings()
         : size(1024,768),
           default_size(1280,768),
           screenRatio(1,1),
           deviceRatio(1),
           transRatio2D(1.f),
           className("game_window"),
           fullScreen(false),
           fullScreenChange(false),
           projType(EPT_PERSPECTIVE),
           viewAngle(45.f),
           minZdist(5.f),
           maxZdist(1000.f),
           orthoScrnPercentage(0.25f),
           createDepthStencilBuffer(true),
           clearStencilBuffer(false),
           enableDepthBuffer(true),
           clearTargetBuffer(true),
           createShadowMapBuffer(false),
           tripleBuffering(false),
           vSync(false),
           vSyncChange(false),
           textFiltering(ETF_LINEAR),
           pre3D(true),
           pre2D(true),
           post3D(false),
           post2D(false),
           joypadStickRange(50),
           joypadStickDeadZone(2500)
{
    // Get the windows Documents folder path.
    userFolderPath = NGenFunc::GetWindowsFolderPath( FOLDERID_Documents );

    // Get the display settings
    EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &dm );

}	// Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CSettings::~CSettings()
{
}   // Destructer


/************************************************************************
*    desc:  Append the game folder name to the path
*  
*    param: string & folderName - name of game folder
************************************************************************/
void CSettings::AppendToFolderPath( const string & folderName )
{
    if( !userFolderPath.empty() )
        userFolderPath += folderName;

}	// AppendToFolderPath


/************************************************************************
*    desc:  Clear the folder path
************************************************************************/
void CSettings::ClearFolderPath()
{
    userFolderPath.clear();

}	// ClearFolderPath


/************************************************************************
*    desc:  Get the folder path
*
*    ret: string - path
************************************************************************/
const string & CSettings::GetFolderPath() const
{
    return userFolderPath;

}	// GetFolderPath


/************************************************************************
*    desc:  Load settings data from xml file
*  
*    param: string & _filePath - path to file
*
*    ret: string - class name
************************************************************************/
void CSettings::LoadFromXML( const string & _filePath )
{
    filePath = userFolderPath + _filePath;

    // this open and parse the XML file:
    mainNode = XMLNode::openFileHelper( filePath.c_str(), "settings" );

    if( !mainNode.isEmpty() )
    {
        XMLNode displayListNode = mainNode.getChildNode("display");

        if( !displayListNode.isEmpty() )
        {
            // Get the attributes from the "resolution" node
            XMLNode resolutionNode = displayListNode.getChildNode("resolution");
            size.w = (float)atof(resolutionNode.getAttribute("width"));
            size.h = (float)atof(resolutionNode.getAttribute("height"));
            
            if( resolutionNode.getAttribute("fullscreen") == string("true") )
                fullScreenChange = fullScreen = true;

            sizeChange = size;

            // Get the attributes from the "defaultHeight" node
            XMLNode defResNode = displayListNode.getChildNode("defaultHeight");
            default_size.h = (float)atof(defResNode.getAttribute("height"));

            CalcRatio( size );
        }

        XMLNode directXListNode = mainNode.getChildNode("directX");

        if( !directXListNode.isEmpty() )
        {
            // Get the projection info
            XMLNode projectionNode = directXListNode.getChildNode("projection");
            minZdist = (float)atof(projectionNode.getAttribute("minZDist"));
            maxZdist = (float)atof(projectionNode.getAttribute("maxZDist"));

            XMLNode perspectiveProjNode = projectionNode.getChildNode("perspectiveProj");

            if( !perspectiveProjNode.isEmpty() )
            {
                projType = EPT_PERSPECTIVE;
                viewAngle = (float)atof(perspectiveProjNode.getAttribute("view_angle"));
            }

            XMLNode orthographicProjNode = projectionNode.getChildNode("orthographicProj");

            if( !orthographicProjNode.isEmpty() )
            {
                projType = EPT_ORTHOGRAPHIC;
                orthoScrnPercentage = (float)atof(orthographicProjNode.getAttribute("screenPercentage"));
            }

            // Get the attribute from the "textureFiltering" node
            XMLNode textFilterNode = directXListNode.getChildNode("textureFiltering");
            if( !textFilterNode.isEmpty() )
            {
                string filter = textFilterNode.getAttribute("filter");

                if( filter == string("point") )
                    textFiltering = ETF_POINT;

                else if( filter == string("linear") )
                    textFiltering = ETF_LINEAR;

                else if(filter == string("anisotropic_2X") )
                    textFiltering = ETF_ANISOTROPIC_2X;

                else if( filter == string("anisotropic_4X") )
                    textFiltering = ETF_ANISOTROPIC_4X;

                else if( filter == string("anisotropic_8X") )
                    textFiltering = ETF_ANISOTROPIC_8X;

                else if( filter == string("anisotropic_16X") )
                    textFiltering = ETF_ANISOTROPIC_16X;

                textFilteringChange = textFiltering;
            }

            // Get the attribute from the "backbuffer" node
            XMLNode backBufferNode = directXListNode.getChildNode("backbuffer");
            if( !backBufferNode.isEmpty() )
            {
                tripleBuffering = (backBufferNode.getAttribute("tripleBuffering") == string("true")) ? true : false;
                vSyncChange = vSync = (backBufferNode.getAttribute("VSync") == string("true")) ? true : false;
            }

            // Get the attribute from the "depthStencilBuffer" node
            XMLNode depthStencilBufferNode = directXListNode.getChildNode("depthStencilBuffer");

            // If depth stencil bufferis not created then there's no stencil buffer to clear
            if( depthStencilBufferNode.getAttribute("create") == string("false") )
                createDepthStencilBuffer = false;

            // Do we clear the stencil buffer
            if( depthStencilBufferNode.getAttribute("clearStencilBuffer") == string("true") )
                clearStencilBuffer = true;

            if( depthStencilBufferNode.isAttributeSet("enableDepthBuffer") )
                if( depthStencilBufferNode.getAttribute("enableDepthBuffer") == string("false") )
                    enableDepthBuffer = false;

            // Get the attribute from the "targetBuffer" node
            XMLNode targetBufferNode = directXListNode.getChildNode("targetBuffer");

            if( targetBufferNode.getAttribute("clear") == string("false") )
                clearTargetBuffer = false;

            // Get the "shadowBuffer" node
            XMLNode shadowBufferNode = directXListNode.getChildNode("shadowBuffer");

            if( !shadowBufferNode.isEmpty() )
            {
                if( shadowBufferNode.getAttribute("create") == string("true") )
                {
                    createShadowMapBuffer = true;
                    shadowMapBufferSize.w = atoi(shadowBufferNode.getAttribute("width"));
                    shadowMapBufferSize.h = atoi(shadowBufferNode.getAttribute("height"));
                }
            }

            // Get the "postProcessBuffer" node
            XMLNode postProcessBufferNode = directXListNode.getChildNode("postProcessBuffer");

            if( !postProcessBufferNode.isEmpty() )
            {
                if( postProcessBufferNode.getAttribute("create") == string("true") )
                {
                    for( int i = 0; i < postProcessBufferNode.nChildNode(); ++i )
                    {
                        CPostProcBuff postProcBuff;
                        postProcBuff.scale = (float)atof(postProcessBufferNode.getChildNode(i).getAttribute("scale"));
                        postProcBuff.format = postProcessBufferNode.getChildNode(i).getAttribute("format");
                        postProcBuffVec.push_back(postProcBuff);
                    }
                }
            }
        }

        XMLNode deviceListNode = mainNode.getChildNode("device");

        if( !deviceListNode.isEmpty() )
        {
            XMLNode joypadNode = deviceListNode.getChildNode("joypad");
            if( !joypadNode.isEmpty() )
            {
                joypadStickRange = atoi(joypadNode.getAttribute("stickRange"));
                joypadStickDeadZone = atoi(joypadNode.getAttribute("stickDeadZone"));
            }
        }
    }

}   // LoadFromXML


/************************************************************************
*    desc:  Calculate the ratio
*
*    ret: string - class name
************************************************************************/
void CSettings::CalcRatio( const CSize<float> & res )
{
    // NOTE: the default width is based on the actual resolution
    default_size.w = (res.w / res.h) * default_size.h;

    // Get half the size for use with screen boundries
    default_size_half = default_size / 2;

    size_half = size / 2;

    // Calculate the screen ratio because of the default height
    screenRatio.w = res.w / default_size.w;
    screenRatio.h = res.h / default_size.h;

    // Ratio for devices (mice) to correctly calculate movement
    // between the default height and actual height
    deviceRatio = default_size.h / res.h;

}	// CalcRatio


/************************************************************************
*    desc:  Save the settings file
************************************************************************/
void CSettings::SaveSettings()
{
    if( !mainNode.isEmpty() )
    {
        XMLNode displayListNode = mainNode.getChildNode("display");

        if( !displayListNode.isEmpty() )
        {
            // Get the attributes from the "resolution" node
            XMLNode resolutionNode = displayListNode.getChildNode("resolution");

            {
                string tmpStr = boost::lexical_cast<string>( sizeChange.w );
                resolutionNode.updateAttribute(tmpStr.c_str(), "width", "width");

                tmpStr = boost::lexical_cast<string>( sizeChange.h );
                resolutionNode.updateAttribute(tmpStr.c_str(), "height", "height");

                size = sizeChange;

                CalcRatio( size );
            }

            {
                string tmpStr = "false";

                if( fullScreenChange )
                    tmpStr = "true";

                resolutionNode.updateAttribute(tmpStr.c_str(), "fullscreen", "fullscreen");

                fullScreen = fullScreenChange;
            }
        }

        XMLNode directXListNode = mainNode.getChildNode("directX");

        if( !directXListNode.isEmpty() )
        {
            {
                // Get the attribute from the "backbuffer" node
                XMLNode backBufferNode = directXListNode.getChildNode("backbuffer");

                string tmpStr = "false";

                if( vSyncChange )
                    tmpStr = "true";

                backBufferNode.updateAttribute(tmpStr.c_str(), "VSync", "VSync");

                vSync = vSyncChange;
            }
            {
                // Get the attribute from the "backbuffer" node
                XMLNode textFilterNode = directXListNode.getChildNode("textureFiltering");

                string tmpStr = GetTextFilterString();

                textFiltering = textFilteringChange;

                textFilterNode.updateAttribute(tmpStr.c_str(), "filter", "filter");
            }
        }

        XMLNode deviceListNode = mainNode.getChildNode("device");

        if( !deviceListNode.isEmpty() )
        {
            {
                // Get the attribute from the "joypad" node
                XMLNode joypadNode = deviceListNode.getChildNode("joypad");

                string tmpStr = boost::lexical_cast<string>( joypadStickDeadZone );

                joypadNode.updateAttribute(tmpStr.c_str(), "stickDeadZone", "stickDeadZone");
            }
        }

        // Save the settings file
        mainNode.writeToFile(filePath.c_str(), "utf-8");
    }

}	// SaveSettings


/***************************************************************************
*    decs:  Was the resolution changed
****************************************************************************/
bool CSettings::WasResolutionChanged() const
{
    return (size != sizeChange);

}	// WasResolutionChanged


/***************************************************************************
*    decs:  Was the full screen changed
****************************************************************************/
bool CSettings::WasScreenModeChanged() const
{
    return (fullScreen != fullScreenChange);

}	// WasScreenModeChanged


/***************************************************************************
*    decs:  Are we changing to full screen?
****************************************************************************/
bool CSettings::IsChangeToFullScreen() const
{
    return (!fullScreen && fullScreenChange);

}	// IsChangeToFullScreen


/***************************************************************************
*    decs:  Are we changing to window view?
****************************************************************************/
bool CSettings::IsChangeToWindowView() const
{
    return (fullScreen && !fullScreenChange);

}	// IsChangeToWindowView


/***************************************************************************
*    decs:  Was the VSync changed
****************************************************************************/
bool CSettings::WasVSyncChanged() const
{
    return (vSync != vSyncChange);

}	// WasVSyncChanged


/***************************************************************************
*    decs:  Was the text filtering changed
****************************************************************************/
bool CSettings::WasTextFilteringChanged() const
{
    return (textFiltering != textFilteringChange);

}	// WasTextFilteringChanged


/***************************************************************************
*    decs:  Get the filter index - the filter values are not linear
****************************************************************************/
int CSettings::GetTextFilterIndex() const
{
    int filterIndex = 0;

    if( textFiltering == ETF_LINEAR )
        filterIndex = 1;

    else if( textFiltering == ETF_ANISOTROPIC_2X )
        filterIndex = 2;

    else if( textFiltering == ETF_ANISOTROPIC_4X )
        filterIndex = 3;

    else if( textFiltering == ETF_ANISOTROPIC_8X )
        filterIndex = 4;

    else if( textFiltering == ETF_ANISOTROPIC_16X )
        filterIndex = 5;

    return filterIndex;

}	// GetFilterIndex


/***************************************************************************
*    decs:  Get the filter index - the filter values are not linear
****************************************************************************/
string CSettings::GetTextFilterString() const
{
    string filterStr = "point";

    if( textFilteringChange == ETF_LINEAR )
        filterStr = "linear";

    else if( textFilteringChange == ETF_ANISOTROPIC_2X )
        filterStr = "anisotropic_2X";

    else if( textFilteringChange == ETF_ANISOTROPIC_4X )
        filterStr = "anisotropic_4X";

    else if( textFilteringChange == ETF_ANISOTROPIC_8X )
        filterStr = "anisotropic_8X";

    else if( textFilteringChange == ETF_ANISOTROPIC_16X )
        filterStr = "anisotropic_16X";

    return filterStr;

}	// GetFilterIndex


/***************************************************************************
*    decs:  Get the filter index - the filter values are not linear
****************************************************************************/
void CSettings::SetTextFilterByIndex( int index )
{
    textFilteringChange = ETF_POINT;

    if( index == 1 )
        textFilteringChange = ETF_LINEAR;

    else if( index == 2 )
        textFilteringChange = ETF_ANISOTROPIC_2X;

    else if( index == 3 )
        textFilteringChange = ETF_ANISOTROPIC_4X;

    else if( index == 4 )
        textFilteringChange = ETF_ANISOTROPIC_8X;

    else if( index == 5 )
        textFilteringChange = ETF_ANISOTROPIC_16X;

}	// SetTextFilterByIndex


/************************************************************************
*    desc:  Get the class name
*
*    ret: string - class name
************************************************************************/
const string & CSettings::GetGameWndClassName() const
{
    return className;
}


/************************************************************************
*    desc:  Get game window size
*
*    ret: CSize - size of game window
************************************************************************/
const CSize<float> & CSettings::GetSize() const
{
    return size;
}


/************************************************************************
*    desc:  Get game window size / 2
*
*    ret: CSize - size of game window / 2
************************************************************************/
const CSize<float> & CSettings::GetSizeHalf() const
{
    return size_half;
}


/************************************************************************
*    desc:  Get game window size change
*
*    ret: CSize - size of game window
************************************************************************/
const CSize<float> & CSettings::GetSizeChange() const
{
    return sizeChange;
}


/************************************************************************
*    desc:  Set game window change size
************************************************************************/
void CSettings::SetChangeSize( const CSize<float> & _size )
{
    sizeChange = _size;
}


/************************************************************************
*    desc:  Get 3d area size
*
*    ret: CSize - size of game window
************************************************************************/
const CSize<float> & CSettings::GetDefaultSize() const
{
    return default_size;
}


/************************************************************************
*    desc:  Get 3d area size in half
*
*    ret: CSize - size of game window
************************************************************************/
const CSize<float> & CSettings::GetDefaultSizeHalf() const
{
    return default_size_half;
}


/************************************************************************
*    desc:  Get the height and width ratios of the screen
*
*    ret: CSize - ratio of the screen
************************************************************************/
const CSize<float> & CSettings::GetScreenRatio() const
{
    return screenRatio;
}


/************************************************************************
*    desc:  Get the height and width ratios of the screen
*
*    ret: CSize - ratio of the screen
************************************************************************/
float CSettings::GetDeviceRatio() const
{
    return deviceRatio;
}


/************************************************************************
*    desc:  Get the projection type
************************************************************************/
CSettings::EProjectionType CSettings::GetProjectionType() const
{
    return projType;
}


/************************************************************************
*    desc:  Get the view angle
*
*    ret: float - view angle
************************************************************************/
float CSettings::GetViewAngle() const
{
    return viewAngle;
}


/************************************************************************
*    desc:  Get the minimum z distance
*
*    ret: float - minimum z distance
************************************************************************/
float CSettings::GetMinZdist() const
{
    return minZdist;
}


/************************************************************************
*    desc:  Get the maximum z distance
*
*    ret: float - maximum z distance
************************************************************************/
float CSettings::GetMaxZdist() const
{
    return maxZdist;
}


/************************************************************************
*    desc:  Get the orthographic screen projection
************************************************************************/
float CSettings::GetOrthoScreenPercentage() const
{
    return orthoScrnPercentage;
}


/************************************************************************
*    desc:  Do we create the depth stencil buffer
*
*    ret: bool - createDepthStencilBuffer
************************************************************************/
bool CSettings::GetCreateDepthStencilBuffer() const
{
    return createDepthStencilBuffer;
}


/************************************************************************
*    desc:  Do we clear the stencil buffer
*
*    ret: bool - clearStencilBuffer
************************************************************************/
bool CSettings::GetClearStencilBuffer() const
{
    return clearStencilBuffer;
}


/************************************************************************
*    desc:  Is the depth buffer enabled by default
*
*    ret: bool - enableDepthBuffer
************************************************************************/
bool CSettings::GetEnableDepthBuffer() const
{
    return enableDepthBuffer;
}


/************************************************************************
*    desc:  Do we clear the target buffer
*
*    ret: bool - clearTargetBuffer
************************************************************************/
bool CSettings::GetClearTargetBuffer() const
{
    return clearTargetBuffer;
}


/************************************************************************
*    desc:  Calculate the 2D ratio for translations
************************************************************************/
void CSettings::CalculateTransRatio2D()
{
    if( screenRatio.w < screenRatio.h )
        transRatio2D = screenRatio.w;
    else
        transRatio2D = screenRatio.h;

}	// CalculateTransRatio2D


/************************************************************************
*    desc:  Do we create a shadow map buffer
*
*    ret: bool
************************************************************************/
bool CSettings::GetCreateShadowMapBuffer() const
{
    return createShadowMapBuffer;

}	// GetCreateShadowMapBuffer


/************************************************************************
*    desc:  Get the height and width of the shadow map buffer
*
*    ret: CSize
************************************************************************/
const CSize<int> & CSettings::GetShadowMapBufferSize() const
{
    return shadowMapBufferSize;

}	// GetShadowMapBufferSize


/************************************************************************
*    desc:  Do we create a post process buffer
*
*    ret: bool
************************************************************************/
bool CSettings::GetCreatePostProcBuf() const
{
    return !postProcBuffVec.empty();

}	// GetCreatePostProcBuf


/************************************************************************
*    desc:  Get the number of post process buffers
*
*    ret: size_t
************************************************************************/
size_t CSettings::GetPostProcBufCount() const
{
    return postProcBuffVec.size();

}	// GetPostProcBufCount


/************************************************************************
*    desc:  Get the post process buffer info
************************************************************************/
const CSettings::CPostProcBuff & CSettings::GetPostProcBufInfo(int index) const
{
    return postProcBuffVec[index];

}	// GetPostProcBufCount


/************************************************************************
*    desc:  Do we want tripple buffering?
*
*    ret: bool
************************************************************************/
bool CSettings::GetTripleBuffering() const
{
    return tripleBuffering;

}	// GetTripleBuffering


/************************************************************************
*    desc:  Get the texture filtering
************************************************************************/
CSettings::ETextFilter CSettings::GetTextFiltering() const
{
    return textFiltering;

}	// GetTextFiltering


/************************************************************************
*    desc:  Get the texture filtering change
************************************************************************/
CSettings::ETextFilter CSettings::GetTextFilteringChange() const
{
    return textFilteringChange;

}	// GetTextFilteringChange


////// Members that can be reset ///////



/************************************************************************
*    desc:  Is the window full screen
*
*    ret: bool - true or false if full screen
************************************************************************/
bool CSettings::GetFullScreen() const
{
    return fullScreen;
}


/************************************************************************
*    desc:  Is the full screen window 
*
*    ret: bool - true or false if full screen
************************************************************************/
bool CSettings::GetFullScreenChange() const
{
    return fullScreenChange;
}


/************************************************************************
*    desc:  Set the window to full screen
*
*    param: value - true or false if full screen
************************************************************************/
void CSettings::SetFullScreenChange( bool value )
{
    fullScreenChange = value;
}


/************************************************************************
*    desc:  Do we want vSync?
************************************************************************/
bool CSettings::GetVSync() const
{
    return vSync;
}


/************************************************************************
*    desc:  Do we want vSync?
************************************************************************/
bool CSettings::GetVSyncChange() const
{
    return vSyncChange;
}


/************************************************************************
*    desc:  Do we want vSync?
************************************************************************/
void CSettings::SetVSyncChange( bool value )
{
    vSyncChange = value;
}


/************************************************************************
*    desc:  Get the joypad stick range
************************************************************************/
int CSettings::GetJoyPadStickRange() const
{
    return joypadStickRange;
}


/************************************************************************
*    desc:  Set the joypad stick range
************************************************************************/
void CSettings::SetJoyPadStickRange( int value )
{
    joypadStickRange = value;
}


/************************************************************************
*    desc:  Get the joypad stick dead zone
************************************************************************/
int CSettings::GetJoyPadStickDeadZone() const
{
    return joypadStickDeadZone;
}


/************************************************************************
*    desc:  Set the joypad stick dead zone
************************************************************************/
void CSettings::SetJoyPadStickDeadZone( int value )
{
    joypadStickDeadZone = value;
}


/************************************************************************
*    desc:  Get the display settings
************************************************************************/
const DEVMODE & CSettings::GetDisplaySettings() const
{
    return dm;
}