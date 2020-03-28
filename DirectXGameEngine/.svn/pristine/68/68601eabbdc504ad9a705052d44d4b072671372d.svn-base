
/************************************************************************
*    FILE NAME:       gamecontroller.h
*
*    DESCRIPTION:     game controller class
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include <controller/gamecontroller.h>

// Boost lib dependencies
#include <boost/scoped_array.hpp>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <controller/keyboardcontroller.h>
#include <controller/mousecontroller.h>
#include <controller/joypadcontroller.h>
#include <system/xdevice.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <common/defs.h>
#include <misc/settings.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CGameController::CGameController()
               : hWnd(NULL),
                 lastDeviceUsed(NDevice::DEVICE_NULL),
                 xmlActionChanges(false),
                 pActionOwnerThisPtr(NULL),
                 hiDeviceCount(0),
                 hiDeviceCounter(0),
                 multiPlayerId(NDevice::MULTIPLAYER_NULL)
{
    using namespace std;

    // Init the buton To XInput mapping
    butonToXInputMap.insert( make_pair( "B1", "A" ) );
    butonToXInputMap.insert( make_pair( "B3", "X" ) );
    butonToXInputMap.insert( make_pair( "B4", "Y" ) );
    butonToXInputMap.insert( make_pair( "B2", "B" ) );
    butonToXInputMap.insert( make_pair( "B7", "BACK" ) );
    butonToXInputMap.insert( make_pair( "B8", "START" ) );
    butonToXInputMap.insert( make_pair( "B5", "LB" ) );
    butonToXInputMap.insert( make_pair( "B6", "RB" ) );
    butonToXInputMap.insert( make_pair( "B9", "L STICK" ) );
    butonToXInputMap.insert( make_pair( "B10", "R STICK" ) );

    butonToXInputMap.insert( make_pair( "LT", "LT" ) );
    butonToXInputMap.insert( make_pair( "RT", "RT" ) );
    
    butonToXInputMap.insert( make_pair( "UP", "UP" ) );
    butonToXInputMap.insert( make_pair( "UP RIGHT", "UP RIGHT" ) );
    butonToXInputMap.insert( make_pair( "RIGHT", "RIGHT" ) );
    butonToXInputMap.insert( make_pair( "DOWN RIGHT", "DOWN RIGHT" ) );
    butonToXInputMap.insert( make_pair( "DOWN", "DOWN" ) );
    butonToXInputMap.insert( make_pair( "DOWN LEFT", "DOWN LEFT" ) );
    butonToXInputMap.insert( make_pair( "LEFT", "LEFT" ) );
    butonToXInputMap.insert( make_pair( "UP LEFT", "UP LEFT" ) );

    butonToXInputMap.insert( make_pair( "L STICK UP", "L STICK UP" ) );
    butonToXInputMap.insert( make_pair( "L STICK DOWN", "L STICK DOWN" ) );
    butonToXInputMap.insert( make_pair( "L STICK LEFT", "L STICK LEFT" ) );
    butonToXInputMap.insert( make_pair( "L STICK RIGHT", "L STICK RIGHT" ) );

    butonToXInputMap.insert( make_pair( "R STICK UP", "R STICK UP" ) );
    butonToXInputMap.insert( make_pair( "R STICK DOWN", "R STICK DOWN" ) );
    butonToXInputMap.insert( make_pair( "R STICK LEFT", "R STICK LEFT" ) );
    butonToXInputMap.insert( make_pair( "R STICK RIGHT", "R STICK RIGHT" ) );

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CGameController::~CGameController()
{
}   // Destructer


/************************************************************************
*    desc:  Init
*
*    parm: HWND handleWnd - handle to window
************************************************************************/
bool CGameController::Init( HWND handleWnd )
{
    HRESULT hr;

    // Init the window handle
    hWnd = handleWnd;

    if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ),
                             DIRECTINPUT_VERSION,
                             IID_IDirectInput8,
                             ( VOID** )&spDirectInput,
                             NULL ) ) )
    {
        DisplayError( hr );
        return false;
    }

    // Look for devices we can use
    if( FAILED( hr = spDirectInput->EnumDevices( DI8DEVCLASS_ALL,
                                    (const LPDIENUMDEVICESCALLBACK)EnumDevicesCallback,
                                    this,
                                    DIEDFL_ATTACHEDONLY ) ) )
    {
        DisplayError( hr );
        return false;
    }

    // Query the joypad axeses and buttons
    for( auto iter = spJoyPadVec.begin(); 
         iter != spJoyPadVec.end();
         ++iter )
    {
        if( FAILED( hr = iter->EnumDeviceQuery() ) )
        {
            DisplayError( hr );
            return false;
        }

        //NGenFunc::PostDebugMsg( "%s, Componet Count: %d%s", iter->GetName().c_str(), iter->GetComponentCount(), (iter->IsXInput() ? ", XInput Device" : " ") );
    }

    // Query the mouse
    if( spMouseController.get() != NULL )
    {
        if( FAILED( hr = spMouseController->EnumDeviceQuery() ) )
        {
            DisplayError( hr );
            return false;
        }

        //NGenFunc::PostDebugMsg( "%s Componet Count: %d", spMouseController->GetName().c_str(), spMouseController->GetComponentCount() );
    }

    // Query the keyboard
    if( spKeyboardController.get() != NULL )
    {
        if( FAILED( hr = spKeyboardController->EnumDeviceQuery() ) )
        {
            DisplayError( hr );
            return false;
        }

        //NGenFunc::PostDebugMsg( "%s Componet Count: %d", spKeyboardController->GetName().c_str(), spKeyboardController->GetComponentCount() );
    }

    // Count the number of human interface devices we have
    // to do faster joypad checks later
    if( spJoyPadVec.empty() )
        GetRawInputDeviceList(NULL, &hiDeviceCount, sizeof(RAWINPUTDEVICELIST));

    return true;

}   // Init


/************************************************************************
*    desc:  enumerated call back function for devices
*
*    param: const DIDEVICEINSTANCE* pInst - device instance
*           VOID* pContext - used in the case as the "this" pointer
************************************************************************/
BOOL CALLBACK CGameController::EnumDevicesCallback( const DIDEVICEINSTANCE* pInst,
                                                    VOID* pContext )
{
    // Cast out pointer
    CGameController * pGameController = NGenFunc::ReintCast<CGameController *>(pContext);

    return pGameController->CreateDevice( pInst );

}	// EnumObjectsCallback


/************************************************************************
*    desc:  Create the devices from enumeration
*
*    param: DIDEVICEOBJECTINSTANCE* pDeviceInstance - device instance
************************************************************************/
BOOL CGameController::CreateDevice( const DIDEVICEINSTANCE* pInst )
{
    LPDIRECTINPUTDEVICE8 pDevice;
    HRESULT hr;

    if( (GET_DIDEVICE_TYPE(pInst->dwDevType) == DI8DEVTYPE_MOUSE) && (spMouseController.get() == NULL) )
    {
        if( SUCCEEDED( hr = spDirectInput->CreateDevice( pInst->guidInstance, &pDevice, NULL ) ) )
        {
            spMouseController.reset( new CMouseController() );
            if( FAILED( hr = spMouseController->InitDevice( pDevice, pInst, hWnd ) ) )
            {
                DisplayError( hr );
                return DIENUM_STOP;
            }
        }
    }
    else if( (GET_DIDEVICE_TYPE(pInst->dwDevType) == DI8DEVTYPE_KEYBOARD) && (spKeyboardController.get() == NULL) )
    {
        if( SUCCEEDED( hr = spDirectInput->CreateDevice( pInst->guidInstance, &pDevice, NULL ) ) )
        {
            spKeyboardController.reset( new CKeyboardController() );
            if( FAILED( spKeyboardController->InitDevice( pDevice, pInst, hWnd ) ) )
            {
                DisplayError( hr );
                return DIENUM_STOP;
            }
        }
    }
    else
    {
        if( CreateJoyPadDevice( pInst ) == DIENUM_STOP )
            return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;

}	// CreateDevice


/************************************************************************
*    desc:  Create the device from enumeration
*
*    param: DIDEVICEOBJECTINSTANCE* pDeviceInstance - device instance
************************************************************************/
BOOL CGameController::CreateJoyPadDevice( const DIDEVICEINSTANCE* pInst )
{
    LPDIRECTINPUTDEVICE8 pDevice;
    HRESULT hr;

    if( GET_DIDEVICE_TYPE(pInst->dwDevType) == DI8DEVTYPE_JOYSTICK ||
        GET_DIDEVICE_TYPE(pInst->dwDevType) == DI8DEVTYPE_GAMEPAD )
    {
        if( SUCCEEDED( hr = spDirectInput->CreateDevice( pInst->guidInstance, &pDevice, NULL ) ) )
        {
            spJoyPadVec.push_back( new CJoyPadController() );

            spJoyPadVec.back().SetDevicePlayerID( static_cast<NDevice::EDevicePlayerId>(spJoyPadVec.size()) );

            if( FAILED( spJoyPadVec.back().InitDevice( pDevice, pInst, hWnd ) ) )
            {
                DisplayError( hr );
                return DIENUM_STOP;
            }
        }
    }

    return DIENUM_CONTINUE;

}	// CreateJoyPadDevice


/************************************************************************
*    desc:  Check for gamepad controllers
************************************************************************/
bool CGameController::CheckForGamepadControllers()
{
    bool result = false;

    if( spJoyPadVec.empty() )
    {
        bool doEnumDevices(false);

        uint nDevices(0);

        // Get the number of human interface devices used to see if the number has changed
        uint rawInputResult = GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));

        if( (rawInputResult == 0) && (nDevices > 0) && (nDevices != hiDeviceCount) )
        {
            doEnumDevices = true;
            ++hiDeviceCounter;

            // Try two time before updating the count
            // to ingnore any new devices added that is not a controller
            if( hiDeviceCounter == 2 )
            {
                hiDeviceCount = nDevices;
                hiDeviceCounter = 0;
            }
        }

        if( doEnumDevices )
        {
            HRESULT hr;

            // Look for devices we can use
            if( FAILED( hr = spDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL,
                                            (const LPDIENUMDEVICESCALLBACK)EnumDevicesCallback,
                                            this,
                                            DIEDFL_ATTACHEDONLY ) ) )
            {
                DisplayError( hr );
            }

            // Query the joypad axeses and buttons
            for( auto iter = spJoyPadVec.begin(); 
                 iter != spJoyPadVec.end();
                 ++iter )
            {
                if( FAILED( hr = iter->EnumDeviceQuery() ) )
                {
                    DisplayError( hr );
                }
                else
                {
                    //NGenFunc::PostDebugMsg( "%s Componet Count: %d", iter->GetName().c_str(), iter->GetComponentCount() );

                    result = true;
                }
            }
        }
    }

    return result;

}	// CheckForGamepadControllers


/************************************************************************
*    desc:  enumerated gamepad call back function for devices
*
*    param: const DIDEVICEINSTANCE* pInst - device instance
*           VOID* pContext - used in the case as the "this" pointer
************************************************************************/
BOOL CALLBACK CGameController::EnumJoyPadCallback( const DIDEVICEINSTANCE* pInst,
                                                    VOID* pContext )
{
    // Cast out pointer
    CGameController * pGameController = NGenFunc::ReintCast<CGameController *>(pContext);

    return pGameController->CreateJoyPadDevice( pInst );

}	// EnumObjectsCallback


/************************************************************************
*    desc:  Add controllers to active list to poll data from
*
*    param: EDeviceId id - controlller type
*           unsigned int index - index of controller incase more then one is available
************************************************************************/
void CGameController::AddToPollLst( NDevice::EDeviceId id, unsigned int index )
{
    // See if the controller has already been added
    auto iter = activeMap.find( id );

    // If it's not found, add the controller to the active list
    if( iter == activeMap.end() )
    {
        if( (id == NDevice::KEYBOARD) && (NULL != spKeyboardController.get()) )
        {
            spKeyboardController->AcquireDevice();
            activeMap.insert( std::make_pair( id, spKeyboardController.get() ) );
        }
        else if( (id == NDevice::MOUSE) && (NULL != spMouseController.get()) )
        {
            spMouseController->AcquireDevice();
            activeMap.insert( std::make_pair( id, spMouseController.get() ) );
        }
        else if( id == NDevice::JOYPAD )
        {
            for( size_t i = 0; i < spJoyPadVec.size(); ++i )
            {
                spJoyPadVec[i].AcquireDevice();
                NDevice::EDeviceId joyPadId = static_cast<NDevice::EDeviceId>(id + i);
                activeMap.insert( std::make_pair( joyPadId, &spJoyPadVec[i] ) );
            }
        }
    }
    else
    {
        // It is assumed that if you want to switch out controllers
        // it will be a joypad. No matter how many mice or keyboards
        // attached to your computer, DirectInput only see one global 
        // device for each.
        if( iter->second->GetDeviceID() == NDevice::JOYPAD )
        {
            // Make sure we are not swapping out the same one
            if( (index < spJoyPadVec.size()) && (iter->second != &spJoyPadVec[index]) )
            {
                iter->second->UnacquireDevice();
                activeMap.erase( iter );
                spJoyPadVec[index].AcquireDevice();
                NDevice::EDeviceId joyPadId = static_cast<NDevice::EDeviceId>(id + index);
                activeMap.insert( std::make_pair( joyPadId, &spJoyPadVec[index] ) );
            }
        }
    }

}	// AddToPollLst


/************************************************************************
*    desc:  Poll the active controllers
************************************************************************/
void CGameController::Poll( bool hasFocus )
{
    // Clear the data before filling
    devMovement.Reset();

    bool deviceUsedFound(false);

    // Loop through the map and poll all the devices
    for( auto iter = activeMap.begin();
         iter != activeMap.end();
         ++iter )
    {
        iter->second->ResetQuery();

        if( hasFocus )
            iter->second->Poll();
        else
            iter->second->ClearPollData();

        iter->second->SetMovement( devMovement );

        // Set which device was the first to be used
        if( !deviceUsedFound && iter->second->WasDeviceUsed() )
        {
            deviceUsedFound = true;
            lastDeviceUsed = iter->second->GetDeviceID();
        }
    }

}	// Poll


/************************************************************************
*    desc:  Set the dead zone of the joypad
*
*    param: int value - Dead zone value
************************************************************************/
void CGameController::SetJoypadDeadZone(int value)
{
    for( auto iter = spJoyPadVec.begin();
         iter != spJoyPadVec.end();
         ++iter )
    {
        iter->SetDeadZoneAmount( value );
    }
}	// SetJoypadDeadZone


/************************************************************************
*    desc:  Set the axis amount of the joypad
*
*    param: int value - axis amount value
************************************************************************/
void CGameController::SetJoypadAxisAmount(int value)
{
    for( auto iter = spJoyPadVec.begin();
         iter != spJoyPadVec.end();
         ++iter )
    {
        iter->SetAxisAmount( value );
    }
}	// SetJoypadAxisAmount


/************************************************************************
*    desc:  bind the action to the device event
************************************************************************/
void CGameController::BindActionToEvent( std::string & actionStr,
                                         NDevice::EDeviceId deviceId,
                                         int componetId )
{
    if( deviceId == NDevice::KEYBOARD )
    {
        if( NULL != spKeyboardController.get() )
            spKeyboardController->SetActionToMap( actionStr, NDevice::EKeyboardComponetId(componetId) );
    }
    else if( deviceId == NDevice::MOUSE )
    {
        if( NULL != spMouseController.get() )
            spMouseController->SetActionToMap( actionStr, NDevice::EMouseComponetId(componetId) );
    }
    else
    {
        for( auto iter = spJoyPadVec.begin();
             iter != spJoyPadVec.end();
             ++iter )
        {
            iter->SetActionToMap( actionStr, NDevice::EJoyPadComponetId(componetId) );
        }
    }

}	// BindActionToEvent


/************************************************************************
*    desc:  Remove this action from the devices event
*
*    param: string & actionStr - String indicating the action that needs checking
*           EDeviceId id - controlller type
************************************************************************/
void CGameController::RemoveActionFromEvent( std::string & actionStr, NDevice::EDeviceId id )
{
    if( (id == NDevice::KEYBOARD) && (NULL != spKeyboardController.get()) )
        spKeyboardController->RemoveActionFromMap( actionStr );

    else if( (id == NDevice::MOUSE) && (NULL != spMouseController.get()) )
        spMouseController->RemoveActionFromMap( actionStr );

    else if( id == NDevice::JOYPAD )
    {
        for( auto iter = spJoyPadVec.begin();
             iter != spJoyPadVec.end();
             ++iter )
        {
            iter->RemoveActionFromMap( actionStr );
        }
    }

}	// RemoveActionFromEvent


/************************************************************************
*    desc:  Poll data for button or key presses
*
*    param: String & actionStr - String indicating the action that needs checking
*
*    ret: EActionPress - Returns the state of the button/key press
************************************************************************/
NDevice::EActionPress CGameController::WasAction( const std::string & actionStr )
{
    NDevice::EActionPress result( NDevice::EAP_IDLE );

    if( multiPlayerId == NDevice::MULTIPLAYER_NULL )
    {
        for( auto iter = activeMap.begin(); 
             iter != activeMap.end();
             ++iter )
        {
            result = iter->second->WasActionPressed( actionStr );

            if( result != NDevice::EAP_IDLE )
                break;
        }
    }
    else if( !devicePlayerIdVec.empty() )
    {
        NDevice::EDevicePlayerId devicePlayerId = devicePlayerIdVec[multiPlayerId];

        if( devicePlayerId == NDevice::MOUSE_KEYBOARD )
        {
            result = spKeyboardController->WasActionPressed( actionStr );

            if( result == NDevice::EAP_IDLE )
                result = spMouseController->WasActionPressed( actionStr );
        }
        else
        {
            result = spJoyPadVec[devicePlayerId-1].WasActionPressed( actionStr );
        }
    }

    return result;

}	// WasAction

bool CGameController::WasAction( const std::string & actionStr, NDevice::EActionPress actionPress )
{
    return (WasAction( actionStr ) == actionPress);

}	// WasAction


/************************************************************************
*    desc:  Get the movement data
*
*    ret: CDeviceMovement & - Filled out movement class
************************************************************************/
const CDeviceMovement & CGameController::GetMovement()
{
    if( (multiPlayerId == NDevice::MULTIPLAYER_NULL) && !devicePlayerIdVec.empty() )
    {
        if( multiPlayerId == NDevice::MOUSE_KEYBOARD )
            return spMouseController->GetMovement();

        else
            return spJoyPadVec[multiPlayerId-1].GetMovement();
    }

    return devMovement;

}	// GetMovement


/************************************************************************
*    desc:  Add the player device Id
************************************************************************/
void CGameController::AddPlayerDeviceId( NDevice::EDevicePlayerId devicePlayerId )
{
    auto iter = find( devicePlayerIdVec.begin(), devicePlayerIdVec.end(), devicePlayerId );

    if( iter != devicePlayerIdVec.end() )
        devicePlayerIdVec.push_back(devicePlayerId);

}	// AddPlayerDeviceId


/************************************************************************
*    desc:  seach for the action reset it
*
*    param: String & actionStr - String indicating the action that needs checking
*
*    ret: EActionPress - Returns the state of the button/key press
************************************************************************/
void CGameController::ResetAction( const std::string & actionStr )
{
    for( auto iter = activeMap.begin(); 
         iter != activeMap.end();
         ++iter )
    {
        iter->second->ResetAction( actionStr );
    }

}	// ResetAction


/************************************************************************
*    desc:  Ask if the mouse has been moved
*
*    ret:	bool - mouse was or was not moved
************************************************************************/
bool CGameController::WasMouseMoved()
{
    if( devMovement.mouseX != 0 || devMovement.mouseY != 0 )
        return true;

    return false;

}	// WasMouseMoved


/************************************************************************
*    desc:  Get the last device used
*
*    ret: EDeviceId - last deviced used
************************************************************************/
NDevice::EDeviceId CGameController::GetLastDevicedUsed()
{
    return lastDeviceUsed;

}	// GetLastDevicedUsed


/************************************************************************
*    desc:  Get the absolute mouse position scaled to our system
*
*    ret:	CPoint & - mouse position
************************************************************************/
const CPoint & CGameController::GetAbsolutePos() const
{
    if( spMouseController.get() != NULL )
        return spMouseController->GetAbsolutePos();

    return pointDummy;

}	// GetAbsolutePos


/************************************************************************
*    desc:  Get the absolute mouse position scaled to our system
*
*    ret:	CPoint & - mouse position
************************************************************************/
const CPoint & CGameController::GetAbsolutePosScaled() const
{
    if( spMouseController.get() != NULL )
        return spMouseController->GetAbsolutePosScaled();

    return pointDummy;

}	// GetAbsolutePosScaled


/************************************************************************
*    desc:  Get the relative mouse position
*
*    ret:	CPoint & - mouse position
************************************************************************/
const CPoint & CGameController::GetRelativePos() const
{
    if( spMouseController.get() != NULL )
        return spMouseController->GetRelativePos();

    return pointDummy;

}	// GetRelativePos


/************************************************************************
*    desc:  Get the relative mouse position scaled to our system
*
*    ret:	CPoint & - mouse position
************************************************************************/
const CPoint & CGameController::GetRelativePosScaled() const
{
    if( spMouseController.get() != NULL )
        return spMouseController->GetRelativePosScaled();

    return pointDummy;

}	// GetRelativePosScaled


/************************************************************************
*    desc:  Load the action mappings from xml
*  
*    param: std::string filePath - path to file
************************************************************************/
void CGameController::LoadActionFromXML( const std::string & filePath )
{
    saveFilePath = CSettings::Instance().GetFolderPath() + filePath;

    mainNode = XMLNode::openFileHelper( saveFilePath.c_str(), "controllerMapping" );

    LoadAction();

}	// LoadActionFromXML


/************************************************************************
*    desc:  Load the action mappings from xml
*  
*    param: std::string filePath - path to file
************************************************************************/
void CGameController::LoadAction()
{
    if( NULL != spKeyboardController.get() )
        spKeyboardController->LoadActionFromXMLNode( mainNode.getChildNode( "keyboardMapping" ) );

    if( NULL != spMouseController.get() )
        spMouseController->LoadActionFromXMLNode( mainNode.getChildNode( "mouseMapping" ) );

    LoadJoyPadActionFromXML();

}	// LoadActionFromXML


/************************************************************************
*    desc:  Load the JoyPad action mappings from xml
************************************************************************/
void CGameController::LoadJoyPadActionFromXML()
{
    if( !spJoyPadVec.empty() )
    {
        XMLNode controllerNode = mainNode.getChildNode( "joypadMapping" );

        for( auto iter = spJoyPadVec.begin();
             iter != spJoyPadVec.end();
             ++iter )
        {
            iter->LoadActionFromXMLNode( controllerNode );
        }
    }
}	// LoadActionFromXML


/************************************************************************
*    desc:  Get the action/component strings for the give device id
************************************************************************/
bool CGameController::GetDeviceActionStr( NDevice::EDeviceId deviceId, const std::string & actionNameStr, std::string & componetIdStr, bool & configurable )
{
    bool result = false;

    if( deviceId == NDevice::KEYBOARD )
    {
        XMLNode playerVisibleNode = mainNode.getChildNode( "keyboardMapping" ).getChildNode( "playerVisible" );
        result = GetActionStr( playerVisibleNode, actionNameStr, componetIdStr, configurable );
    }
    else if( deviceId == NDevice::MOUSE )
    {
        XMLNode playerVisibleNode = mainNode.getChildNode( "mouseMapping" ).getChildNode( "playerVisible" );
        result = GetActionStr( playerVisibleNode, actionNameStr, componetIdStr, configurable );
    }
    else if( deviceId == NDevice::JOYPAD )
    {
        std::string joypadTypeStr = "miscDevice";

        // See if a joypad has been used
        if( lastDeviceUsed >= NDevice::JOYPAD )
        {
            if( activeMap[lastDeviceUsed]->IsXInput() )
                joypadTypeStr = "xInputDevice";
        }
        // If one hasn't been used, use the joyPad vector if only one is present
        else
        {
            for( size_t i = 0; i < spJoyPadVec.size(); ++i )
            {
                if( spJoyPadVec[i].IsXInput() )
                {
                    joypadTypeStr = "xInputDevice";
                    break;
                }
            }
        }

        XMLNode playerVisibleNode = mainNode.getChildNode( "joypadMapping" ).getChildNode( joypadTypeStr.c_str() ).getChildNode( "playerVisible" );

        // If none have been found, use the misc button mapping one.
        result = GetActionStr( playerVisibleNode, actionNameStr, componetIdStr, configurable );
    }

    return result;

}	// GetActionStr


/************************************************************************
*    desc:  Get the action/component strings for the keyboard device id
************************************************************************/
bool CGameController::GetActionStr( const XMLNode & playerVisibleNode, 
                                    const std::string & actionNameStr, 
                                    std::string & componetIdStr, 
                                    bool & configurable )
{
    for( int i = 0; i < playerVisibleNode.nChildNode(); ++i )
    {
        XMLNode actionMapNode = playerVisibleNode.getChildNode( "actionMap", i );

        std::string actionStr = actionMapNode.getAttribute( "action" );

        if( actionNameStr.find(actionStr) != std::string::npos )
        {
            componetIdStr = actionMapNode.getAttribute( "componetId" );

            configurable = false;

            if( actionMapNode.isAttributeSet( "configurable" ) )
                if( std::string(actionMapNode.getAttribute( "configurable")) == std::string("true") )
                    configurable = true;

            // Map button strings to xInput strings
            if( std::string(playerVisibleNode.getParentNode().getName()) == std::string("xInputDevice") )
                componetIdStr = butonToXInputMap[componetIdStr];

            return true;
        }
    }
    
    return false;

}	// GetKeyboardActionStr


/************************************************************************
*    desc:  Set the new component string
************************************************************************/
bool CGameController::SetComponetStr( const XMLNode & playerVisibleNode, std::string & actionNameStr, std::string & componetIdStr )
{
    for( int i = 0; i < playerVisibleNode.nChildNode(); ++i )
    {
        XMLNode actionMapNode = playerVisibleNode.getChildNode( "actionMap", i );

        std::string actionStr = actionMapNode.getAttribute( "action" );

        if( actionNameStr.find(actionStr) != std::string::npos )
        {
            if( actionMapNode.isAttributeSet( "configurable" ) &&
                (std::string(actionMapNode.getAttribute( "configurable")) == std::string("true")) )
            {
                // Reset the name to the proper name
                actionNameStr = actionStr;

                // Check the current component ID
                std::string compIdStr = actionMapNode.getAttribute( "componetId" );

                // If the ID is different, then set the new component
                if( compIdStr != componetIdStr )
                    actionMapNode.updateAttribute(componetIdStr.c_str(), "componetId", "componetId");

                // If the component ID is the same, set the ID as the NULL unbound ID
                else
                {
                    actionMapNode.updateAttribute(NDevice::UNBOUND_KEYBINDING_ID.c_str(), "componetId", "componetId");
                    componetIdStr = NDevice::UNBOUND_KEYBINDING_ID;
                }

                return true;
            }
            else
                return false;
        }
    }
    
    return false;

}	// SetComponetStr


/************************************************************************
*    desc:  Save the xml to file
*  
*    param: std::string filePath - path to file
************************************************************************/
void CGameController::SaveToFile()
{
    // Save it to file
    if( xmlActionChanges )
        mainNode.writeToFile(saveFilePath.c_str(), "utf-8");

    xmlActionChanges = false;

}	// SaveToFile


/************************************************************************
*    desc:  Set the key binding
*
*    ret:  bool  indicates control is to be mapped
************************************************************************/
bool CGameController::SetKeyBinding( NDevice::EDeviceId deviceId, const std::string & actionNameStr, int & componetId )
{
    bool result(false);
    XMLNode playerVisibleNode;

    // Make a copy of the action string name because it will need to be modified
    // Basicly the ":" will need to be stripped off for resetting the action
    std::string modifiedActionNameStr = actionNameStr;
    
    // Get the componet string ID that needs to be reset
    std::string componetIdStr = GetDeviceComponetIdStr( deviceId, componetId );

    if( deviceId == NDevice::KEYBOARD )
    {
        playerVisibleNode = mainNode.getChildNode( "keyboardMapping" ).getChildNode( "playerVisible" );
    }
    else if( deviceId == NDevice::MOUSE )
    {
        playerVisibleNode = mainNode.getChildNode( "mouseMapping" ).getChildNode( "playerVisible" );
    }
    else if( deviceId == NDevice::JOYPAD )
    {
        std::string joypadTypeStr = "miscDevice";
        CJoyPadController * pJoyPadController;

        // See if a joypad has been used
        if( lastDeviceUsed >= NDevice::JOYPAD )
        {
            pJoyPadController = NGenFunc::DynCast<CJoyPadController *>(activeMap[lastDeviceUsed]);

            if( pJoyPadController->IsXInput() )
                joypadTypeStr = "xInputDevice";
        }
        // If one hasn't been used, use the joyPad vector if only one is present
        else if( spJoyPadVec.size() == 1 )
        {
            pJoyPadController = &spJoyPadVec.back();

            if( spJoyPadVec.back().IsXInput() )
                joypadTypeStr = "xInputDevice";
        }

        if( pJoyPadController != NULL )
            playerVisibleNode = mainNode.getChildNode( "joypadMapping" ).getChildNode( joypadTypeStr.c_str() ).getChildNode( "playerVisible" );
    }

    // This function checks if the same ID is being set and then sets the componetIdStr as the unbound state
    if( SetComponetStr( playerVisibleNode, modifiedActionNameStr, componetIdStr ) )
    {
        RemoveActionFromEvent( modifiedActionNameStr, deviceId );

        if( componetIdStr != NDevice::UNBOUND_KEYBINDING_ID )
            BindActionToEvent( modifiedActionNameStr, deviceId, componetId );

        // Set the component ID to the NULL unbind state
        else
            componetId = -1;

        // set the internal flag that allows for saving the config file
        xmlActionChanges = true;

        result = true;
    }

    return result;

}	// SetKeyBinding


/************************************************************************
*    desc:  Scan device for button press
*
*    return: EKeyboardComponetId
************************************************************************/
int CGameController::ScanDeviceForButtonPress( NDevice::EDeviceId deviceId )
{
    int result = -1;

    if( deviceId == NDevice::KEYBOARD )
    {
        if( spKeyboardController.get() != NULL )
            result = spKeyboardController->ScanForButtonPress();
    }
    else if( deviceId == NDevice::MOUSE )
    {
        if( spMouseController.get() != NULL )
            result = spMouseController->ScanForButtonPress();
    }
    else if( deviceId == NDevice::JOYPAD )
    {
        if( lastDeviceUsed >= NDevice::JOYPAD )
            result = activeMap[lastDeviceUsed]->ScanForButtonPress();

        else if( spJoyPadVec.size() == 1 )
            result = spJoyPadVec.back().ScanForButtonPress();
    }

    return result;

}	// ScanDeviceForButtonPress


/************************************************************************
*    desc:  Get the device component string Id
*
*    return: string
************************************************************************/
std::string CGameController::GetDeviceComponetIdStr( NDevice::EDeviceId deviceId, int componetId )
{
    std::string result = "NULL";

    if( deviceId == NDevice::KEYBOARD )
    {
        if( spKeyboardController.get() != NULL )
            result = spKeyboardController->GetComponetIdStr( componetId );
    }
    else if( deviceId == NDevice::MOUSE )
    {
        if( spMouseController.get() != NULL )
            result = spMouseController->GetComponetIdStr( componetId );
    }
    else if( deviceId == NDevice::JOYPAD )
    {
        if( lastDeviceUsed >= NDevice::JOYPAD )
        {
            result = activeMap[lastDeviceUsed]->GetComponetIdStr( componetId );
        }
        else if( spJoyPadVec.size() == 1 )
        {
            result = spJoyPadVec.back().GetComponetIdStr( componetId );
        }
    }

    return result;

}	// GetDeviceComponetIdStr


/************************************************************************
*    desc:  Get the the standard button string to xbox conversion for display
*
*    return: string
************************************************************************/
std::string CGameController::GetJoypadDisplayStr( std::string & componetIdStr )
{
    std::string result = componetIdStr;

    if( lastDeviceUsed >= NDevice::JOYPAD )
    {
        // Map button strings to xInput strings
        if( activeMap[lastDeviceUsed]->IsXInput() )
            result = butonToXInputMap[result];
    }
    else if( spJoyPadVec.size() == 1 )
    {
        // Map button strings to xInput strings
        if( spJoyPadVec.back().IsXInput() )
            result = butonToXInputMap[result];
    }

    return result;

}	// GetJoypadDisplayStr


/************************************************************************
*    desc:  Get the number of joypads currently connected
*
*    return: string
************************************************************************/
size_t CGameController::GetJoypadCount()
{
    return spJoyPadVec.size();
}


/************************************************************************
*    desc:  Reset all the key bindings to their default settings
************************************************************************/
void CGameController::ResetKeyBindingsToDefault()
{
    std::vector<std::string> stringPathsVec;
    stringPathsVec.push_back("keyboardMapping/playerVisible");
    stringPathsVec.push_back("mouseMapping/playerVisible");
    stringPathsVec.push_back("joypadMapping/xInputDevice/playerVisible");
    stringPathsVec.push_back("joypadMapping/miscDevice/playerVisible");

    for( size_t i = 0; i < stringPathsVec.size(); ++i )
    {
        XMLNode playerVisibleNode = mainNode.getChildNodeByPath( stringPathsVec[i].c_str() );

        for( int j = 0; j < playerVisibleNode.nChildNode("actionMap"); ++j )
        {
            XMLNode actionMapNode = playerVisibleNode.getChildNode( "actionMap", j );

            if( actionMapNode.isAttributeSet( "configurable" ) &&
                (std::string(actionMapNode.getAttribute( "configurable")) == "true") &&
                actionMapNode.isAttributeSet( "defaultId" ) )
            {
                std::string test = actionMapNode.getAttribute( "defaultId");
                actionMapNode.updateAttribute( actionMapNode.getAttribute( "defaultId"), "componetId", "componetId");
            }
        }
    }

    // Clear out all the actions to reload
    if( NULL != spKeyboardController.get() )
        spKeyboardController->ClearActionMap();

    if( NULL != spMouseController.get() )
        spMouseController->ClearActionMap();

    if( !spJoyPadVec.empty() )
    {
        for( auto iter = spJoyPadVec.begin();
             iter != spJoyPadVec.end();
             ++iter )
        {
            iter->ClearActionMap();
        }
    }

    // Load ll the actions again
    LoadAction();

    mainNode.writeToFile(saveFilePath.c_str(), "utf-8");

}	// ResetKeyBindingsToDefault


/************************************************************************
*    desc:  Set the this ptr of the control acting on the down action
*           This is only used to compare against other void pointers
*           to determine ownership of action
*
*    parm: pthisPtr - Set the this ptr
************************************************************************/
void CGameController::SetActionOwner( void * pthisPtr )
{
    pActionOwnerThisPtr = pthisPtr;

}	// SetOwnerThisPtr

bool CGameController::IsActionOwner( void * pthisPtr )
{
    return (pActionOwnerThisPtr == pthisPtr);

}	// IsOwnerThisPtr


/************************************************************************
*    desc:  Display error information
*
*    param: HRESULT hr - return result from function call
************************************************************************/
void CGameController::DisplayError( HRESULT hr )
{
    switch( hr )
    {
        case DIERR_OLDDIRECTINPUTVERSION:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "Newer version of DirectInput is required.");
            break;
        }

        case DIERR_BETADIRECTINPUTVERSION:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "Unsupported prerelease version of DirectInput.");
            break;
        }

        case DIERR_BADDRIVERVER:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The object could not be created due to an incompatible driver version or mismatched or incomplete driver components.");
            break;
        }

        case DIERR_DEVICENOTREG:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The device or device instance or effect is not registered with DirectInput.");
            break;
        }

        case DIERR_NOTFOUND:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The requested object does not exist.");
            break;
        }

        case DIERR_INVALIDPARAM:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "Invalid parameter.");
            break;
        }

        case DIERR_NOINTERFACE:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The specified interface is not supported by the object.");
            break;
        }

        case DIERR_GENERIC:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "An undetermined error occured inside the DInput subsystem.");
            break;
        }

        case DIERR_OUTOFMEMORY:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The DInput subsystem couldn't allocate sufficient memory to complete the request.");
            break;
        }

        case DIERR_UNSUPPORTED:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The function called is not supported at this time.");
            break;
        }

        case DIERR_NOTINITIALIZED:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "This object has not been initialized.");
            break;
        }

        case DIERR_ALREADYINITIALIZED:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "This object is already initialized.");
            break;
        }

        case DIERR_NOAGGREGATION:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "This object does not support aggregation.");
            break;
        }

        case DIERR_OTHERAPPHASPRIO:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "Another app has a higher priority level, preventing this call from succeeding.");
            break;
        }

        case DIERR_INPUTLOST:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "Access to the device has been lost. It must be re-acquired.");
            break;
        }

        case DIERR_ACQUIRED:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The operation cannot be performed while the device is acquired.");
            break;
        }

        case DIERR_NOTACQUIRED:
        {
            throw NExcept::CCriticalException("DirectInput Error!", "The operation cannot be performed unless the device is acquired.");
            break;
        }
    }
}	// DisplayError
