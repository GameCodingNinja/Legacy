
/************************************************************************
*    FILE NAME:       keyboardcontroller.h
*
*    DESCRIPTION:     keyboard controller class
************************************************************************/

// Physical component dependency
#include <controller/keyboardcontroller.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <system/xdevice.h>
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CKeyboardController::CKeyboardController()
                   : CBaseController()
{
    ZeroMemory( pollData, sizeof(pollData) );

    deviceId = NDevice::KEYBOARD;

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CKeyboardController::~CKeyboardController()
{
}   // Destructer


/************************************************************************
*    desc:  init the device
*
*    param: LPDIRECTINPUTDEVICE8 pDev - pointer to device
*           string devName - name of device
*           HWND handleWnd - handle to window
************************************************************************/
HRESULT CKeyboardController::InitDevice( LPDIRECTINPUTDEVICE8 pDev,
                                         const DIDEVICEINSTANCE* pInst,
                                         HWND handleWnd )
{
    HRESULT hr;

    // Call the parent
    CBaseController::InitDevice( pDev, pInst, handleWnd );

    // Set the data format
    if( FAILED( hr = spDevice->SetDataFormat( &c_dfDIKeyboard ) ) )
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
BOOL CKeyboardController::QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst )
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
HRESULT CKeyboardController::Poll()
{
    HRESULT hr;

    // Call the parent to do the poll
    if( DI_OK == ( hr = CBaseController::Poll() ) )
        hr = spDevice->GetDeviceState( sizeof( pollData ), (LPVOID)&pollData );
    
    // On error, clear the poll data to stop the game
    if( hr != DI_OK )
        ClearPollData();
    
    return hr;

}	// Poll


/************************************************************************
*    desc:  clear the poll data
************************************************************************/
void CKeyboardController::ClearPollData()
{
    memset( &pollData, 0, sizeof(pollData) );

}	// ClearPollData


/************************************************************************
*    desc:  Clear the whole action map
************************************************************************/
void CKeyboardController::ClearActionMap()
{
    spActionMap.clear();

}	// ClearActionMap


/************************************************************************
*    desc:  seach for the action and remove it from the map
*
*    param: string actionStr - action string to set as key
************************************************************************/
void CKeyboardController::RemoveActionFromMap( const std::string & actionStr )
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
*           EKeyboardComponetId id - id to associate with event
************************************************************************/
void CKeyboardController::SetActionToMap( const std::string & actionStr, NDevice::EKeyboardComponetId id )
{
    // See if the controller action string has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, add another id to this map
    if( actionMapIter != spActionMap.end() )
        actionMapIter->second->SetId( id );

    else
        // Add new action to the map
        spActionMap.insert( actionStr, new CKeyButtonAction<NDevice::EKeyboardComponetId>(id) );

}	// SetActionToMap


/************************************************************************
*    desc:  seach for the action and see if the data was pressed
*
*    param: string & actionStr - action string to set as key
*
*	 ret: EActionPress - Returns the state of the button/key press
************************************************************************/
NDevice::EActionPress CKeyboardController::WasActionPressed( const std::string & actionStr )
{
    NDevice::EActionPress result( NDevice::EAP_IDLE );

    // See if the action has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, see if the button was pressed
    if( actionMapIter != spActionMap.end() )
    {
        // Don't do any checks until the query has been reset
        if( !actionMapIter->second->IsQueryActive() )
        {
            bool buttonPressed = false;

            for( unsigned int i = 0; i < actionMapIter->second->GetIdCount(); ++i )
            {
                if( pollData[ actionMapIter->second->GetId(i) ] & 0x80 )
                {
                    buttonPressed = true;
                    break;
                }
            }

            if( buttonPressed )
                actionMapIter->second->Inc();

            else
                actionMapIter->second->Reset();
        }

        result = actionMapIter->second->GetResult();
    }

    return result;

}	// WasActionPressed


/************************************************************************
*    desc:  seach for the action reset it
*
*    param: string & actionStr - action string to set as key
************************************************************************/
void CKeyboardController::ResetAction( const std::string & actionStr )
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
*    desc:  Scan for a button press
*
*    param: int
************************************************************************/
int CKeyboardController::ScanForButtonPress() const
{
    int result = NDevice::KEY_NULL;

    for( int i = 0; i < NDevice::MAX_KEYS; ++i )
    {
        if( pollData[i] & 0x80 )
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
bool CKeyboardController::WasDeviceUsed() const
{
    for( int i = 0; i < NDevice::MAX_KEYS; ++i )
        if( pollData[i] & 0x80 )
            return true;

    return false;

}	// WasDeviceUsed


/************************************************************************
*    desc:  Get the component string Id
*
*    param: string
************************************************************************/
const std::string & CKeyboardController::GetComponetIdStr( int id )
{
    // See if the action has already been added
    xmlSaveIter = xmlSaveMap.find( NDevice::EKeyboardComponetId(id) );

    // If it's found, return the string
    if( xmlSaveIter != xmlSaveMap.end() )
        return xmlSaveIter->second;

    else
        return xmlSaveMap[NDevice::KEY_NULL];

}	// GetComponetIdStr


/************************************************************************
*    desc:  Set the movement data
*
*    param: CDeviceMovement & movement - movement class
************************************************************************/
void CKeyboardController::SetMovement( CDeviceMovement & movement )
{
    // Do nothing. Keyboards don't have movement capabilities

}	// SetMovement


/************************************************************************
*    desc:  Load action data from xml node
*  
*    param: XMLNode & controllerNode - xml node
************************************************************************/
void CKeyboardController::LoadActionFromXMLNode( const XMLNode & node )
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
void CKeyboardController::LoadActionNode( const XMLNode & node )
{
    if( !node.isEmpty() )
    {
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            XMLNode actionNode = node.getChildNode( "actionMap", i );

            // See if we can find the string that represents the com id
            xmlParseIter = xmlParseMap.find( actionNode.getAttribute( "componetId" ) );

            // Add it in if we found it
            if( xmlParseIter != xmlParseMap.end() && (xmlParseIter->second != NDevice::KEY_NULL) )
                SetActionToMap( std::string(actionNode.getAttribute( "action" )), xmlParseIter->second );
        }
    }
}	// LoadActionFromXMLNode


/************************************************************************
*    desc:  Reset the query flag
************************************************************************/
void CKeyboardController::ResetQuery()
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
void CKeyboardController::InitControllerXmlParseMap()
{
    using namespace std;
    using namespace NDevice;

    if( xmlParseMap.size() == 0 )
    {
        xmlParseMap.insert( make_pair( UNBOUND_KEYBINDING_ID, NDevice::KEY_NULL ) );
        xmlParseMap.insert( make_pair( "ESCAPE", KEY_ESCAPE ) );
        xmlParseMap.insert( make_pair( "RETURN", KEY_RETURN ) );

        xmlParseMap.insert( make_pair( "1",  KEY_1 ) );
        xmlParseMap.insert( make_pair( "2",  KEY_2 ) );
        xmlParseMap.insert( make_pair( "3",  KEY_3 ) );
        xmlParseMap.insert( make_pair( "4",  KEY_4 ) );
        xmlParseMap.insert( make_pair( "5",  KEY_5 ) );
        xmlParseMap.insert( make_pair( "6",  KEY_6 ) );
        xmlParseMap.insert( make_pair( "7",  KEY_7 ) );
        xmlParseMap.insert( make_pair( "8",  KEY_8 ) );
        xmlParseMap.insert( make_pair( "9",  KEY_9 ) );
        xmlParseMap.insert( make_pair( "0",  KEY_0 ) );

        xmlParseMap.insert( make_pair( "A",  KEY_A ) );
        xmlParseMap.insert( make_pair( "B",  KEY_B ) );
        xmlParseMap.insert( make_pair( "C",  KEY_C ) );
        xmlParseMap.insert( make_pair( "D",  KEY_D ) );
        xmlParseMap.insert( make_pair( "E",  KEY_E ) );
        xmlParseMap.insert( make_pair( "F",  KEY_F ) );
        xmlParseMap.insert( make_pair( "G",  KEY_G ) );
        xmlParseMap.insert( make_pair( "H",  KEY_H ) );
        xmlParseMap.insert( make_pair( "I",  KEY_I ) );
        xmlParseMap.insert( make_pair( "J",  KEY_J ) );
        xmlParseMap.insert( make_pair( "K",  KEY_K ) );
        xmlParseMap.insert( make_pair( "L",  KEY_L ) );
        xmlParseMap.insert( make_pair( "M",  KEY_M ) );
        xmlParseMap.insert( make_pair( "N",  KEY_N ) );
        xmlParseMap.insert( make_pair( "O",  KEY_O ) );
        xmlParseMap.insert( make_pair( "P",  KEY_P ) );
        xmlParseMap.insert( make_pair( "Q",  KEY_Q ) );
        xmlParseMap.insert( make_pair( "R",  KEY_R ) );
        xmlParseMap.insert( make_pair( "S",  KEY_S ) );
        xmlParseMap.insert( make_pair( "T",  KEY_T ) );
        xmlParseMap.insert( make_pair( "U",  KEY_U ) );
        xmlParseMap.insert( make_pair( "V",  KEY_V ) );
        xmlParseMap.insert( make_pair( "W",  KEY_W ) );
        xmlParseMap.insert( make_pair( "X",  KEY_X ) );
        xmlParseMap.insert( make_pair( "Y",  KEY_Y ) );
        xmlParseMap.insert( make_pair( "Z",  KEY_Z ) );

        xmlParseMap.insert( make_pair( "F1",   KEY_F1 ) );
        xmlParseMap.insert( make_pair( "F2",   KEY_F2 ) );
        xmlParseMap.insert( make_pair( "F3",   KEY_F3 ) );
        xmlParseMap.insert( make_pair( "F4",   KEY_F4 ) );
        xmlParseMap.insert( make_pair( "F5",   KEY_F5 ) );
        xmlParseMap.insert( make_pair( "F6",   KEY_F6 ) );
        xmlParseMap.insert( make_pair( "F7",   KEY_F7 ) );
        xmlParseMap.insert( make_pair( "F8",   KEY_F8 ) );
        xmlParseMap.insert( make_pair( "F9",   KEY_F9 ) );
        xmlParseMap.insert( make_pair( "F10",  KEY_F10 ) );
        xmlParseMap.insert( make_pair( "F11",  KEY_F11 ) );
        xmlParseMap.insert( make_pair( "F12",  KEY_F12 ) );
        xmlParseMap.insert( make_pair( "F13",  KEY_F13 ) );
        xmlParseMap.insert( make_pair( "F14",  KEY_F14 ) );
        xmlParseMap.insert( make_pair( "F15",  KEY_F15 ) );

        xmlParseMap.insert( make_pair( "NUMPAD 1", KEY_NUMPAD1 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 2", KEY_NUMPAD2 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 3", KEY_NUMPAD3 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 4", KEY_NUMPAD4 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 5", KEY_NUMPAD5 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 6", KEY_NUMPAD6 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 7", KEY_NUMPAD7 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 8", KEY_NUMPAD8 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 9", KEY_NUMPAD9 ) );
        xmlParseMap.insert( make_pair( "NUMPAD 0", KEY_NUMPAD0 ) );

        xmlParseMap.insert( make_pair( "NUM LOCK",  KEY_NUMLOCK ) );
        xmlParseMap.insert( make_pair( "NUMPAD ENTER",  KEY_NUMPADENTER ) );
        xmlParseMap.insert( make_pair( "NUMPAD EQUIL",  KEY_NUMPADEQUAL ) );
        xmlParseMap.insert( make_pair( "NUMPAD COMMA",  KEY_NUMPADCOMMA ) );

        xmlParseMap.insert( make_pair( "LEFT CTRL", KEY_LCONTROL ) );
        xmlParseMap.insert( make_pair( "RIGHT CTRL", KEY_RCONTROL ) );
        xmlParseMap.insert( make_pair( "LEFT SHIFT",   KEY_LSHIFT ) );
        xmlParseMap.insert( make_pair( "RIGHT SHIFT",   KEY_RSHIFT ) );

        xmlParseMap.insert( make_pair( "END",    KEY_END ) );
        xmlParseMap.insert( make_pair( "NEXT",   KEY_NEXT ) );
        xmlParseMap.insert( make_pair( "INSERT", KEY_INSERT ) );
        xmlParseMap.insert( make_pair( "DELETE", KEY_DELETE ) );
        xmlParseMap.insert( make_pair( "HOME",   KEY_HOME ) );

        xmlParseMap.insert( make_pair( "ARROW UP",    KEY_UP ) );
        xmlParseMap.insert( make_pair( "ARROW DOWN",  KEY_DOWN ) );
        xmlParseMap.insert( make_pair( "ARROW LEFT",  KEY_LEFT ) );
        xmlParseMap.insert( make_pair( "ARROW RIGHT", KEY_RIGHT ) );
    
        xmlParseMap.insert( make_pair( "MINUS", KEY_MINUS ) );
        xmlParseMap.insert( make_pair( "EQUIL", KEY_EQUAL ) );
        xmlParseMap.insert( make_pair( "BACK",  KEY_BACK ) );
        xmlParseMap.insert( make_pair( "TAB",   KEY_TAB ) );
    
        xmlParseMap.insert( make_pair( "[", KEY_LBRACKET ) );
        xmlParseMap.insert( make_pair( "]", KEY_RBRACKET ) );
    
        xmlParseMap.insert( make_pair( ";",  KEY_SEMICOLON ) );
        xmlParseMap.insert( make_pair( "'", KEY_APOSTROPHE ) );
        xmlParseMap.insert( make_pair( "GRAVE",      KEY_GRAVE ) );
    
        xmlParseMap.insert( make_pair( "\\", KEY_BACKSLASH ) );
        xmlParseMap.insert( make_pair( ",",     KEY_COMMA ) );
        xmlParseMap.insert( make_pair( ".",    KEY_PERIOD ) );
        xmlParseMap.insert( make_pair( "/",     KEY_SLASH ) );
        xmlParseMap.insert( make_pair( ":",     KEY_COLON ) );
        xmlParseMap.insert( make_pair( "_", KEY_UNDERLINE ) );
    
        xmlParseMap.insert( make_pair( "MULTIPLY", KEY_MULTIPLY ) );
        xmlParseMap.insert( make_pair( "DIVIDE",   KEY_DIVIDE ) );
        xmlParseMap.insert( make_pair( "LEFT MENU",    KEY_LMENU ) );
        xmlParseMap.insert( make_pair( "RIGHT MENU",    KEY_RMENU ) );
        xmlParseMap.insert( make_pair( "SPACE",    KEY_SPACE ) );
        xmlParseMap.insert( make_pair( "CAPITAL",  KEY_CAPITAL ) );

        xmlParseMap.insert( make_pair( "SCROLL",   KEY_SCROLL ) );
        xmlParseMap.insert( make_pair( "SUBTRACT", KEY_SUBTRACT ) );
        xmlParseMap.insert( make_pair( "ADD",      KEY_ADD ) );
        xmlParseMap.insert( make_pair( "DECIMAL",  KEY_DECIMAL ) );

        xmlParseMap.insert( make_pair( "SYSRQ", KEY_SYSRQ ) );
        xmlParseMap.insert( make_pair( "PRIOR", KEY_PRIOR ) );
        xmlParseMap.insert( make_pair( "APPS",  KEY_APPS ) );
        xmlParseMap.insert( make_pair( "POWER", KEY_POWER ) );
        xmlParseMap.insert( make_pair( "SLEEP", KEY_SLEEP ) );
        xmlParseMap.insert( make_pair( "WAKE",  KEY_WAKE ) );
        xmlParseMap.insert( make_pair( "LEFT WIN",  KEY_LWIN ) );
        xmlParseMap.insert( make_pair( "RIGHT WIN",  KEY_RWIN ) );
        xmlParseMap.insert( make_pair( "CALC",  KEY_CALCULATOR ) );

        xmlParseMap.insert( make_pair( "OEM_102",    KEY_OEM_102 ) );
        xmlParseMap.insert( make_pair( "KANA",       KEY_KANA ) );
        xmlParseMap.insert( make_pair( "ABNT C1",    KEY_ABNT_C1 ) );
        xmlParseMap.insert( make_pair( "CONVERT",    KEY_CONVERT ) );
        xmlParseMap.insert( make_pair( "NO CONVERT", KEY_NOCONVERT ) );
        xmlParseMap.insert( make_pair( "YEN",        KEY_YEN ) );
        xmlParseMap.insert( make_pair( "ABNT_C2",    KEY_ABNT_C2 ) );
        xmlParseMap.insert( make_pair( "PREV TRACK", KEY_PREVTRACK ) );
        xmlParseMap.insert( make_pair( "AT",         KEY_AT ) );
        xmlParseMap.insert( make_pair( "KANJI",      KEY_KANJI ) );
        xmlParseMap.insert( make_pair( "AX",         KEY_AX ) );
        xmlParseMap.insert( make_pair( "UNLABELED",  KEY_UNLABELED ) );
        xmlParseMap.insert( make_pair( "NEXT TRACK", KEY_NEXTTRACK ) );
    
        xmlParseMap.insert( make_pair( "PLAY PAUSE",  KEY_PLAYPAUSE ) );
        xmlParseMap.insert( make_pair( "STOP",        KEY_STOP ) );
        xmlParseMap.insert( make_pair( "MUTE",        KEY_MUTE ) );
        xmlParseMap.insert( make_pair( "MEDIA STOP",  KEY_MEDIASTOP ) );
        xmlParseMap.insert( make_pair( "VOLUME DOWN", KEY_VOLUMEDOWN ) );
        xmlParseMap.insert( make_pair( "VOLUME UP",   KEY_VOLUMEUP ) );
        xmlParseMap.insert( make_pair( "PAUSE",       KEY_PAUSE ) );
    
        xmlParseMap.insert( make_pair( "WEB HOME",      KEY_WEBHOME ) );
        xmlParseMap.insert( make_pair( "WEB SEARCH",    KEY_WEBSEARCH ) );
        xmlParseMap.insert( make_pair( "WEB FAVORITES", KEY_WEBFAVORITES ) );
        xmlParseMap.insert( make_pair( "WEB REFRESH",   KEY_WEBREFRESH ) );
        xmlParseMap.insert( make_pair( "WEB STOP",      KEY_WEBSTOP ) );
        xmlParseMap.insert( make_pair( "WEB FORWARD",   KEY_WEBFORWARD ) );
        xmlParseMap.insert( make_pair( "WEB BACK",      KEY_WEBBACK ) );
        xmlParseMap.insert( make_pair( "MY COMPUTER",   KEY_MYCOMPUTER ) );
        xmlParseMap.insert( make_pair( "MAIL",          KEY_MAIL ) );
        xmlParseMap.insert( make_pair( "MEDIA SELECT",  KEY_MEDIASELECT ) );

        // now make a reverse of this map so the enum is now the key
        for( auto iter = xmlParseMap.begin();
             iter != xmlParseMap.end();
             ++iter )
        {
            xmlSaveMap.insert( make_pair( iter->second, iter->first ) );
        }
    }

}	// InitControllerXmlParseMap