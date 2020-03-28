
/************************************************************************
*    FILE NAME:       mousecontroller.h
*
*    DESCRIPTION:     mouse controller class
************************************************************************/

// Physical component dependency
#include <controller/mousecontroller.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <system/xdevice.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CMouseController::CMouseController()
                : CBaseController()
{
    ZeroMemory( &pollData, sizeof(pollData) );

    deviceId = NDevice::MOUSE;

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CMouseController::~CMouseController()
{
}   // Destructer


/************************************************************************
*    desc:  init the device
*
*    param: LPDIRECTINPUTDEVICE8 pDev - pointer to device
*           string devName - name of device
*           HWND handleWnd - handle to window
************************************************************************/
HRESULT CMouseController::InitDevice( LPDIRECTINPUTDEVICE8 pDev,
                                      const DIDEVICEINSTANCE* pInst,
                                      HWND handleWnd )
{
    HRESULT hr;

    // Call the parent
    CBaseController::InitDevice( pDev, pInst, handleWnd );

    // Set the data format
    if( FAILED( hr = spDevice->SetDataFormat( &c_dfDIMouse2 ) ) )
        return hr;

    // Set the cooperative level
    if( FAILED( hr = spDevice->SetCooperativeLevel( handleWnd,
                                                   DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
        return hr;

    return hr;

}   // InitDevice


/************************************************************************
*    desc:  init the device
*
*    param: DIDEVICEOBJECTINSTANCE pDev - pointer to object instance
************************************************************************/
BOOL CMouseController::QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst )
{
    // count up the components in this controller
    ++componentCount;

    return DIENUM_CONTINUE;

}	// QueryDevice


/************************************************************************
*    desc:  Poll the device
*
*    ret: HRESULT - result of polling
************************************************************************/
HRESULT CMouseController::Poll()
{
    HRESULT hr;

    // Call the parent to do the poll
    if( DI_OK == ( hr = CBaseController::Poll() ) )
    {
        hr = spDevice->GetDeviceState( sizeof( pollData ), &pollData );

        // Set the internal copy of this devices movement
        SetMovement( devMovement );
    }
    
    // On error, clear the poll data to stop the game
    if( hr != DI_OK )
        ClearPollData();

    // Set the last position to caluclate relative position
    lastPos = curPos;
    lastPosScaled = curPosScaled;

    // Get the mouse position from the Windows API
    POINT pos = GetWindowsAPIMousePos();

    // Update the current position to the latest
    curPos.x = pos.x;
    curPos.y = pos.y;

    // Convert the coordinates of the mouse to our system
    float ratio = CSettings::Instance().GetDeviceRatio();
    curPosScaled.x = ratio * (curPos.x - CSettings::Instance().GetSizeHalf().w);
    curPosScaled.y = ratio * (CSettings::Instance().GetSizeHalf().h - curPos.y);

    // Calculate the relative positions
    relativePos = curPos - lastPos;
    relativePosScaled = curPosScaled - lastPosScaled;
    
    return hr;

}	// Poll


/************************************************************************
*    desc:  clear the poll data
************************************************************************/
void CMouseController::ClearPollData()
{
    memset( &pollData, 0, sizeof(pollData) );

    devMovement.Reset();

}	// ClearPollData


/************************************************************************
*    desc:  Clear the whole action map
************************************************************************/
void CMouseController::ClearActionMap()
{
    spActionMap.clear();

}	// ClearActionMap


/************************************************************************
*    desc:  seach for the action and remove it from the map
*
*    param: string actionStr - action string to set as key
************************************************************************/
void CMouseController::RemoveActionFromMap( const std::string & actionStr )
{
    // See if the controller action string has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, remove the action from the map
    if( actionMapIter != spActionMap.end() )
        spActionMap.erase( actionMapIter );

}	// RemoveActionFromMap


/************************************************************************
*    desc:  seach for the action and map it to a given component index
*
*    param: string actionStr - action string to set as key
*           EMouseComponetId id - id to associate with event
************************************************************************/
void CMouseController::SetActionToMap( const std::string & actionStr, NDevice::EMouseComponetId id )
{
    // See if the controller action string has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, add another id to this map
    if( actionMapIter != spActionMap.end() )
        actionMapIter->second->SetId( id );

    else
        // Add new action to the map
        spActionMap.insert( actionStr, new CKeyButtonAction<NDevice::EMouseComponetId>(id) );

}	// SetActionToMap


/************************************************************************
*    desc:  seach for the action reset it
*
*    param: string & actionStr - action string to set as key
************************************************************************/
void CMouseController::ResetAction( const std::string & actionStr )
{
    // See if the action has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, see if the button was pressed
    if( actionMapIter != spActionMap.end() )
    {
        actionMapIter->second->Reset();
        actionMapIter->second->ClearQuery();
    }

}	// ResetAction


/************************************************************************
*    desc:  seach for the action and see if the data was pressed
*
*    param: string actionStr - action string to set as key
*
*	 ret: EActionPress - Returns the state of the button/key press
************************************************************************/
NDevice::EActionPress CMouseController::WasActionPressed( const std::string & actionStr )
{
    NDevice::EActionPress result( NDevice::EAP_IDLE );

    // See if the controller has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, see if the button was pressed
    if( actionMapIter != spActionMap.end() )
    {
        // Don't do any checks until the query has been reset
        if( !actionMapIter->second->IsQueryActive() )
        {
            bool actionPressed(false);

            for( unsigned int i = 0; i < actionMapIter->second->GetIdCount(); ++i )
            {
                NDevice::EMouseComponetId mouseComponetId = actionMapIter->second->GetId(i);

                if( mouseComponetId < NDevice::MAX_MOUSE_BTNS )
                {
                    if( pollData.rgbButtons[ mouseComponetId ] & 0x80 )
                    {
                        actionPressed = true;
                        break;
                    }
                }
                else if( mouseComponetId < NDevice::MAX_MOUSE_MOVE )
                {
                    if( mouseComponetId == NDevice::MOUSE_MOVE_X_Y )
                    {
                        if( (pollData.lX != 0) || (pollData.lY != 0) )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                    else if( mouseComponetId == NDevice::MOUSE_MOVE_Z )
                    {
                        if( pollData.lZ != 0 )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                }
                else
                {
                    actionMapIter->second->Reset();
                    result = actionMapIter->second->GetResult();
                }
            }

            // If the action was pressed, inc it. If not, reset it.
            if( actionPressed )
                actionMapIter->second->Inc();

            else
                actionMapIter->second->Reset();
        }

        result = actionMapIter->second->GetResult();
    }

    return result;

}	// WasActionPressed


/************************************************************************
*    desc:  Set the movement data
*
*    param: CDeviceMovement & movement - movement class
************************************************************************/
void CMouseController::SetMovement( CDeviceMovement & movement )
{
    // The poll data is in Mickeys and not pixels
    movement.mouseX = pollData.lX;
    movement.mouseY = pollData.lY;
    movement.mouseZ = pollData.lZ;

}	// SetMovement


/************************************************************************
*    desc:  Load action data from xml node
*  
*    param: XMLNode & controllerNode - xml node
************************************************************************/
void CMouseController::LoadActionFromXMLNode( const XMLNode & node )
{
    // Init the parse map data
    InitControllerXmlParseMap();

    // Load the player hidden controls
    LoadActionNode( node.getChildNode("playerHidden") );

    // Load the player visible controls
    LoadActionNode( node.getChildNode("playerVisible") );

}	// LoadActionFromXMLNode


/************************************************************************
*    desc:  Load action data from xml node
*  
*    param: XMLNode & controllerNode - xml node
************************************************************************/
void CMouseController::LoadActionNode( const XMLNode & node )
{
    if( !node.isEmpty() )
    {
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            XMLNode actionNode = node.getChildNode( "actionMap", i );

            // See if we can find the string that represents the com id
            xmlParseIter = xmlParseMap.find( actionNode.getAttribute( "componetId" ) );

            // Add it in if we found it
            if( xmlParseIter != xmlParseMap.end() && (xmlParseIter->second != NDevice::MOUSE_BUTTON_NULL) )
                SetActionToMap( std::string(actionNode.getAttribute( "action" )), xmlParseIter->second );
        }
    }

}	// LoadActionFromXMLNode


/************************************************************************
*    desc:  Reset the query flag
************************************************************************/
void CMouseController::ResetQuery()
{
    for( actionMapIter = spActionMap.begin();
         actionMapIter != spActionMap.end();
         ++actionMapIter )
    {
        actionMapIter->second->ClearQuery();
    }
}	// ResetQuery


/************************************************************************
*    desc:  Init the controller action map
************************************************************************/
void CMouseController::InitControllerXmlParseMap()
{
    using namespace std;
    using namespace NDevice;

    if( xmlParseMap.size() == 0 )
    {
        xmlParseMap.insert( make_pair( UNBOUND_KEYBINDING_ID, MOUSE_BUTTON_NULL ) );
        xmlParseMap.insert( make_pair( "LEFT MOUSE",    MOUSE_BUTTON_LEFT ) );
        xmlParseMap.insert( make_pair( "RIGHT MOUSE",   MOUSE_BUTTON_RIGHT ) );
        xmlParseMap.insert( make_pair( "MIDDLE MOUSE",  MOUSE_BUTTON_CENTER ) );

        xmlParseMap.insert( make_pair( "MOUSE 4",  MOUSE_BUTTON_04 ) );
        xmlParseMap.insert( make_pair( "MOUSE 5",  MOUSE_BUTTON_05 ) );
        xmlParseMap.insert( make_pair( "MOUSE 6",  MOUSE_BUTTON_06 ) );
        xmlParseMap.insert( make_pair( "MOUSE 7",  MOUSE_BUTTON_07 ) );
        xmlParseMap.insert( make_pair( "MOUSE 8",  MOUSE_BUTTON_08 ) );

        xmlParseMap.insert( make_pair( "move_x_y",  MOUSE_MOVE_X_Y ) );
        xmlParseMap.insert( make_pair( "MOUSE WHEEL",  MOUSE_MOVE_Z ) );

        // now make a reverse of this map so the enum is now the key
        for( auto iter = xmlParseMap.begin();
             iter != xmlParseMap.end();
             ++iter )
        {
            xmlSaveMap.insert( make_pair( iter->second, iter->first ) );
        }
    }

}	// InitControllerXmlParseMap


/************************************************************************
*    desc:  Scan for a button press
*
*    param: int
************************************************************************/
int CMouseController::ScanForButtonPress() const
{
    int result = NDevice::MOUSE_BUTTON_NULL;

    for( int i = 0; i < NDevice::MAX_MOUSE_BTNS; ++i )
    {
        if( pollData.rgbButtons[i] & 0x80 )
        {
            result = i;
            break;
        }
    }

    return result;

}	// ScanForButtonPress


/************************************************************************
*    desc:  Check to see if the device was used
*
*    param: bool - whether or not this device was used
************************************************************************/
bool CMouseController::WasDeviceUsed() const
{
    for( int i = 0; i < NDevice::MAX_MOUSE_BTNS; ++i )
        if( pollData.rgbButtons[i] & 0x80 )
            return true;

    if( pollData.lX != 0 )
        return true;
    if( pollData.lY != 0 )
        return true;
    if( pollData.lZ != 0 )
        return true;

    return false;

}	// WasDeviceUsed


/************************************************************************
*    desc:  Get the component string Id
*
*    param: string
************************************************************************/
const std::string & CMouseController::GetComponetIdStr( int id )
{
    // See if the action has already been added
    xmlSaveIter = xmlSaveMap.find( NDevice::EMouseComponetId(id) );

    // If it's found, return the string
    if( xmlSaveIter != xmlSaveMap.end() )
        return xmlSaveIter->second;

    else
        return xmlSaveMap[NDevice::MOUSE_BUTTON_NULL];

}	// GetComponetIdStr


/************************************************************************
*    desc:  Get the windows api mouse position
*
*    ret: POINT & - mouse position
************************************************************************/
POINT CMouseController::GetWindowsAPIMousePos() const
{
    POINT pos;
    GetCursorPos( &pos );

    // The ScreenToClient function is meant to compensate for window borders
    if( !CSettings::Instance().GetFullScreen() )
        ScreenToClient( hWnd, &pos );

    return pos;

}	// GetWindowsAPIMousePos


/************************************************************************
*    desc:  Get the absolute mouse position
*
*    param: CPoint
************************************************************************/
const CPoint & CMouseController::GetAbsolutePos() const
{
    return curPos;

}	// GetAbsolutePos


/************************************************************************
*    desc:  Get the absolute mouse position scaled to our system
*
*    param: CPoint
************************************************************************/
const CPoint & CMouseController::GetAbsolutePosScaled() const
{
    return curPosScaled;

}	// GetAbsolutePos


/************************************************************************
*    desc:  Get the relative mouse position
*
*    param: CPoint
************************************************************************/
const CPoint & CMouseController::GetRelativePos() const
{
    return relativePos;

}	// GetRelativePos


/************************************************************************
*    desc:  Get the relative mouse position scaled to our system
*
*    param: CPoint
************************************************************************/
const CPoint & CMouseController::GetRelativePosScaled() const
{
    return relativePosScaled;

}	// GetRelativePosTranlated

