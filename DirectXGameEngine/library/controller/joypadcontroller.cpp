
/************************************************************************
*    FILE NAME:       joypadcontroller.h
*
*    DESCRIPTION:     joystick / gamepad controller class
************************************************************************/

// Physical component dependency
#include <controller/joypadcontroller.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <algorithm>

// Game lib dependencies
#include <utilities/genfunc.h>
#include <utilities/smartpointers.h>
#include <utilities/deletefuncs.h>
#include <misc/settings.h>

// Needed to determine if controller supports xInput
#include <wbemidl.h>

// Define our static members
std::map< std::string, NDevice::EJoyPadComponetId > CJoyPadController::xmlParseMap;
std::map< NDevice::EJoyPadComponetId, std::string > CJoyPadController::xmlSaveMap;

// Turn off depreciated warnings
#pragma warning(disable : 4996)


/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CJoyPadController::CJoyPadController()
                 : CBaseController(),
                   xInputDevice(false),
                   axisAmount(0)
{
    ZeroMemory( &pollData, sizeof(pollData) );
    pollData.rgdwPOV[0] = NDevice::POV_IDLE;

    deviceId = NDevice::JOYPAD;

}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CJoyPadController::~CJoyPadController()
{
}   // Destructer


/************************************************************************
*    desc:  init the device
*
*    param: LPDIRECTINPUTDEVICE8 pDev - pointer to device
*           string devName - name of device
*           HWND handleWnd - handle to window
************************************************************************/
HRESULT CJoyPadController::InitDevice( LPDIRECTINPUTDEVICE8 pDev,
                                       const DIDEVICEINSTANCE* pInst,
                                       HWND handleWnd )
{
    HRESULT hr;

    // Call the parent
    CBaseController::InitDevice( pDev, pInst, handleWnd );

    // Set the data format
    if( FAILED( hr = spDevice->SetDataFormat( &c_dfDIJoystick ) ) )
        return hr;

    // Set the cooperative level
    if( FAILED( hr = spDevice->SetCooperativeLevel( handleWnd,
                                                   DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
        return hr;

    // Check if this is an xInput controller
    if( IsXInputDevice( &pInst->guidProduct ) )
        xInputDevice = true;

    return hr;

}   // InitDevice


//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
// Unfortunately this information can not be found by just using DirectInput 
//-----------------------------------------------------------------------------
bool CJoyPadController::IsXInputDevice( const GUID* pGuidProductFromDirectInput )
{
    const int MAX_DEVICES(20);
    IWbemLocator*           pIWbemLocator  = NULL;
    IEnumWbemClassObject*   pEnumDevices   = NULL;
    IWbemClassObject*       pDevices[MAX_DEVICES]   = {0};
    IWbemServices*          pIWbemServices = NULL;
    BSTR                    bstrNamespace  = NULL;
    BSTR                    bstrDeviceID   = NULL;
    BSTR                    bstrClassName  = NULL;
    DWORD                   uReturned      = 0;
    bool                    bIsXinputDevice= false;
    UINT                    iDevice        = 0;
    VARIANT                 var;
    HRESULT                 hr;

    // CoInit if needed
    hr = CoInitialize(NULL);
    bool bCleanupCOM = SUCCEEDED(hr);

    // Create WMI
    hr = CoCreateInstance( __uuidof(WbemLocator),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof(IWbemLocator),
                           (LPVOID*) &pIWbemLocator);
    if( FAILED(hr) || pIWbemLocator == NULL )
        goto LCleanup;

    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if( bstrNamespace == NULL ) goto LCleanup;        
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if( bstrClassName == NULL ) goto LCleanup;        
    bstrDeviceID  = SysAllocString( L"DeviceID" );          if( bstrDeviceID == NULL )  goto LCleanup;        
    
    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED(hr) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE. 
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );                    

    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 
    if( FAILED(hr) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for( ;; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, MAX_DEVICES, pDevices, &uReturned );
        if( FAILED(hr) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice=0; iDevice<uReturned; ++iDevice )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it's an XInput device
                    // This information can not be found from DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    // Compare the VID/PID to the DInput device
                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );
                    if( dwVidPid == pGuidProductFromDirectInput->Data1 )
                    {
                        bIsXinputDevice = true;
                        goto LCleanup;
                    }
                }
            }   
            NDelFunc::Release( pDevices[iDevice] );
        }
    }

LCleanup:
    if(bstrNamespace)
        SysFreeString(bstrNamespace);
    if(bstrDeviceID)
        SysFreeString(bstrDeviceID);
    if(bstrClassName)
        SysFreeString(bstrClassName);
    for( iDevice=0; iDevice<MAX_DEVICES; ++iDevice )
        NDelFunc::Release( pDevices[iDevice] );
    NDelFunc::Release( pEnumDevices );
    NDelFunc::Release( pIWbemLocator );
    NDelFunc::Release( pIWbemServices );

    if( bCleanupCOM )
        CoUninitialize();

    return bIsXinputDevice;

}	// IsXInputDevice


/************************************************************************
*    desc:  init the device
*
*    param: DIDEVICEOBJECTINSTANCE pDev - pointer to object instance
************************************************************************/
BOOL CJoyPadController::QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst )
{
    // count up the components in this controller
    ++componentCount;

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pInst->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof( DIPROPRANGE );
        diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pInst->dwType; // Specify the enumerated axis

        axisAmount = CSettings::Instance().GetJoyPadStickRange();
        diprg.lMin = -axisAmount;
        diprg.lMax = axisAmount;
        
        diprgVec.push_back(diprg);

        // Set the range for the axis
        if( FAILED( spDevice->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
            return DIENUM_STOP;

        DIPROPDWORD dilong;
        dilong.diph.dwSize = sizeof( DIPROPDWORD );
        dilong.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        dilong.diph.dwHow = DIPH_BYID;
        dilong.diph.dwObj = pInst->dwType;
        dilong.dwData = CSettings::Instance().GetJoyPadStickDeadZone();

        dilongVec.push_back(dilong);

        // Set the dead zone
        if( FAILED( spDevice->SetProperty( DIPROP_DEADZONE, &dilong.diph ) ) )
            return DIENUM_STOP;

        if( pInst->guidType == GUID_XAxis )
            axisCheck.axisX = true;

        else if( pInst->guidType == GUID_YAxis )
            axisCheck.axisY = true;

        else if( pInst->guidType == GUID_ZAxis )
            axisCheck.axisZ = true;

        else if( pInst->guidType == GUID_RxAxis )
            axisCheck.axisRX = true;

        else if( pInst->guidType == GUID_RyAxis )
            axisCheck.axisRY = true;

        else if( pInst->guidType == GUID_RzAxis )
            axisCheck.axisRZ = true;
    }

    return DIENUM_CONTINUE;

}	// QueryDevice


/************************************************************************
*    desc:  Poll the device
*
*    ret: HRESULT - result of polling
************************************************************************/
HRESULT CJoyPadController::Poll()
{
    HRESULT hr;

    // Call the parent to do the poll
    if( SUCCEEDED( hr = CBaseController::Poll() ) )
    {
        // Set the internal copy of this devices movement
        SetMovement( devMovement );

        // On error, clear the poll data to stop the game
        if( FAILED( hr = spDevice->GetDeviceState( sizeof( pollData ), &pollData )) )
            ClearPollData();
    }
    
    return hr;

}	// Poll


/************************************************************************
*    desc:  clear the poll data
************************************************************************/
void CJoyPadController::ClearPollData()
{
    memset( &pollData, 0, sizeof(pollData) );
    pollData.rgdwPOV[0] = NDevice::POV_IDLE;

    devMovement.Reset();

}	// ClearPollData


/************************************************************************
*    desc:  Clear the whole action map
************************************************************************/
void CJoyPadController::ClearActionMap()
{
    spActionMap.clear();

}	// ClearActionMap


/************************************************************************
*    desc:  seach for the action and remove it from the map
*
*    param: string actionStr - action string to set as key
************************************************************************/
void CJoyPadController::RemoveActionFromMap( const std::string & actionStr )
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
*    param: string & actionStr - action string to set as key
*           EJoyPadComponetId id - id to associate with event
************************************************************************/
void CJoyPadController::SetActionToMap( const std::string & actionStr, NDevice::EJoyPadComponetId id )
{
    // See if the controller action string has already been added
    actionMapIter = spActionMap.find( actionStr );

    // If it's found, add another id to this map
    if( actionMapIter != spActionMap.end() )
        actionMapIter->second->SetId( id );

    else
        // Add new action to the map
        spActionMap.insert( actionStr, new CKeyButtonAction<NDevice::EJoyPadComponetId>(id) );

}	// SetActionToMap


/************************************************************************
*    desc:  seach for the action reset it
*
*    param: string & actionStr - action string to set as key
************************************************************************/
void CJoyPadController::ResetAction( const std::string & actionStr )
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
*    param: string & actionStr - action string to set as key
*
*	 ret: EActionPress - Returns the state of the button/key press
************************************************************************/
NDevice::EActionPress CJoyPadController::WasActionPressed( const std::string & actionStr )
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
                NDevice::EJoyPadComponetId joyPadComponetId = actionMapIter->second->GetId(i);

                // Was this a button press?
                if( joyPadComponetId < NDevice::MAX_JOYPAD_BTNS )
                {
                    if( pollData.rgbButtons[ joyPadComponetId ] & 0x80 )
                    {
                        actionPressed = true;
                        break;
                    }
                }
                // Was this a 360 trigger press?
                else if( joyPadComponetId < NDevice::MAX_360_TRIGGERS )
                {
                    if( joyPadComponetId == NDevice::XBOX_360_TRIGGER_LEFT )
                    {
                        if( pollData.lZ > 0 )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                    else if( joyPadComponetId == NDevice::XBOX_360_TRIGGER_RIGHT )
                    {
                        if( pollData.lZ < 0 )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                }
                // Was this a POV press?
                else if( joyPadComponetId < NDevice::MAX_POV_BTNS )
                {
                    if( pollData.rgdwPOV[0] != NDevice::POV_IDLE )
                    {
                        // needed to setup a false value for POV_DEGREE_0 because 
                        // pollData.rgdwPOV[0] returns 0 for the 0 degree position press.
                        // The default value of pollData.rgdwPOV[0] is -1
                        if( joyPadComponetId == NDevice::POV_UP )
                        {
                            if( pollData.rgdwPOV[0] == 0 )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                        else
                        {
                            if( pollData.rgdwPOV[0] == joyPadComponetId )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                    }
                }
                // Is the left or right controller stick used as a button press
                else if( joyPadComponetId < NDevice::MAX_ANALOG_DIR )
                {
                    // For left analog stick
                    if( joyPadComponetId == NDevice::ANALOG1_UP )
                    {
                        if( (pollData.lY < -(axisAmount>>1)) && ( abs(pollData.lY) > abs(pollData.lX) ) )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                    else if( joyPadComponetId == NDevice::ANALOG1_DOWN )
                    {
                        if( (pollData.lY > (axisAmount>>1)) && ( abs(pollData.lY) > abs(pollData.lX) ) )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                    else if( joyPadComponetId == NDevice::ANALOG1_LEFT )
                    {
                        if( (pollData.lX < -(axisAmount>>1)) && ( abs(pollData.lX) > abs(pollData.lY) ) )
                        {
                            actionPressed = true;
                            break;
                        }
                    }
                    else if( joyPadComponetId == NDevice::ANALOG1_RIGHT )
                    {
                        if( (pollData.lX > (axisAmount>>1)) && ( abs(pollData.lX) > abs(pollData.lY) ) )
                        {
                            actionPressed = true;
                            break;
                        }
                    }

                    // For right analog stick, specific analog checks are needed
                    else if( joyPadComponetId == NDevice::ANALOG2_UP )
                    {
                        if( axisCheck.axisZ && axisCheck.axisRZ )
                        {
                            if( (pollData.lRz < -(axisAmount>>1)) && ( abs(pollData.lRz) > abs(pollData.lZ) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                        else if( axisCheck.axisRX && axisCheck.axisRY )
                        {
                            if( (pollData.lRy < -(axisAmount>>1)) && ( abs(pollData.lRy) > abs(pollData.lRx) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                    }
                    else if( joyPadComponetId == NDevice::ANALOG2_DOWN )
                    {
                        if( axisCheck.axisZ && axisCheck.axisRZ )
                        {
                            if( (pollData.lRz > (axisAmount>>1)) && ( abs(pollData.lRz) > abs(pollData.lZ) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                        else if( axisCheck.axisRX && axisCheck.axisRY )
                        {
                            if( (pollData.lRy > (axisAmount>>1)) && ( abs(pollData.lRy) > abs(pollData.lRx) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                    }
                    else if( joyPadComponetId == NDevice::ANALOG2_LEFT )
                    {
                        if( axisCheck.axisZ && axisCheck.axisRZ )
                        {
                            if( (pollData.lZ < -(axisAmount>>1)) && ( abs(pollData.lZ) > abs(pollData.lRz) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                        else if( axisCheck.axisRX && axisCheck.axisRY )
                        {
                            if( (pollData.lRx < -(axisAmount>>1)) && ( abs(pollData.lRx) > abs(pollData.lRy) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                    }
                    else if( joyPadComponetId == NDevice::ANALOG2_RIGHT )
                    {
                        if( axisCheck.axisZ && axisCheck.axisRZ )
                        {
                            if( (pollData.lZ > (axisAmount>>1)) && ( abs(pollData.lZ) > abs(pollData.lRz) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                        else if( axisCheck.axisRX && axisCheck.axisRY )
                        {
                            if( (pollData.lRx > (axisAmount>>1)) && ( abs(pollData.lRx) > abs(pollData.lRy) ) )
                            {
                                actionPressed = true;
                                break;
                            }
                        }
                    }
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
void CJoyPadController::SetMovement( CDeviceMovement & movement )
{
    // 1x is usually always the first analog stick
    if( axisCheck.axisX && axisCheck.axisY && !movement.gamepad1X && !movement.gamepad1Y )
    {
        movement.gamepad1X = pollData.lX;
        movement.gamepad1Y = pollData.lY;
    }

    // This analog configuration is used by Logitech Dual Action controller
    if( !movement.gamepad2X && !movement.gamepad2Y )
    {
        if( axisCheck.axisZ && axisCheck.axisRZ )
        {
            movement.gamepad2X = pollData.lZ;
            movement.gamepad2Y = pollData.lRz;
        }
        // This analog configuration is used by the xbox 360 controller
        else if( axisCheck.axisRX && axisCheck.axisRY )
        {
            movement.gamepad2X = pollData.lRx;
            movement.gamepad2Y = pollData.lRy;
        }
    }

    // Set the triggers for 360 movement
    if( xInputDevice && !pollData.lZ )
    {
        if( pollData.lZ > 0 )
            movement.left360Trigger = pollData.lZ;

        else if( pollData.lZ < 0 )
            movement.right360Trigger = abs(pollData.lZ);
    }

}	// SetMovement


/************************************************************************
*    desc:  Set the axis amount
************************************************************************/
void CJoyPadController::SetAxisAmount(int value)
{
    if( (spDevice != NULL) )
    {
        axisAmount = value;

        for( size_t i = 0; i < diprgVec.size(); ++i )
        {
            diprgVec[i].lMin = -value;
            diprgVec[i].lMax = value;

            if( FAILED( spDevice->SetProperty( DIPROP_RANGE, &diprgVec[i].diph ) ) )
            {
                NGenFunc::PostMsg( "DirectInput Error", "Error setting analog stick range." );
                break;
            }
        }

    }
}	// SetAxisAmount


/************************************************************************
*    desc:  Set the dead zone amount
************************************************************************/
void CJoyPadController::SetDeadZoneAmount(int value)
{
    if( spDevice != NULL )
    {
        for( size_t i = 0; i < dilongVec.size(); ++i )
        {
            dilongVec[i].dwData = value;

            if( FAILED( spDevice->SetProperty( DIPROP_DEADZONE, &dilongVec[i].diph ) ) )
            {
                NGenFunc::PostMsg( "DirectInput Error", "Error setting analog stick dead zone." );
                break;
            }
        }
    }
}	// SetDeadZoneAmount


/************************************************************************
*    desc:  Load action data from xml node
*  
*    param: XMLNode & controllerNode - xml node
************************************************************************/
void CJoyPadController::LoadActionFromXMLNode( const XMLNode & node )
{
    // Init the parse map data
    InitControllerXmlParseMap();

    XMLNode actionNode;

    // Load the controller specific actions
    if( xInputDevice )
        actionNode = node.getChildNode( "xInputDevice" );
    else
        actionNode = node.getChildNode( "miscDevice" );

    // Load the player hidden controls
    LoadActionFromNode( actionNode.getChildNode( "playerHidden" ) );

    // Load the player visible controls
    LoadActionFromNode( actionNode.getChildNode( "playerVisible" ) );

}	// LoadActionFromXMLNode


/************************************************************************
*    desc:  Load action data from xml node
*  
*    param: XMLNode & controllerNode - xml node
************************************************************************/
void CJoyPadController::LoadActionFromNode( const XMLNode & node )
{
    if( !node.isEmpty() )
    {
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            XMLNode actionNode = node.getChildNode( "actionMap", i );

            // See if we can find the string that represents the com id
            xmlParseIter = xmlParseMap.find( actionNode.getAttribute( "componetId" ) );

            // Add it in if we found it
            if( xmlParseIter != xmlParseMap.end() && (xmlParseIter->second != NDevice::JOYPAD_BUTTON_NULL) )
                SetActionToMap( std::string(actionNode.getAttribute( "action" )), xmlParseIter->second );
        }
    }

}	// LoadActionFromNode


/************************************************************************
*    desc:  Reset the query flag
************************************************************************/
void CJoyPadController::ResetQuery()
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
void CJoyPadController::InitControllerXmlParseMap()
{
    using namespace std;
    using namespace NDevice;

    if( xmlParseMap.size() == 0 )
    {
        xmlParseMap.insert( make_pair( UNBOUND_KEYBINDING_ID,  JOYPAD_BUTTON_NULL ) );
        xmlParseMap.insert( make_pair( "B1",  BUTTON_01 ) );
        xmlParseMap.insert( make_pair( "B2",  BUTTON_02 ) );
        xmlParseMap.insert( make_pair( "B3",  BUTTON_03 ) );
        xmlParseMap.insert( make_pair( "B4",  BUTTON_04 ) );
        xmlParseMap.insert( make_pair( "B5",  BUTTON_05 ) );
        xmlParseMap.insert( make_pair( "B6",  BUTTON_06 ) );
        xmlParseMap.insert( make_pair( "B7",  BUTTON_07 ) );
        xmlParseMap.insert( make_pair( "B8",  BUTTON_08 ) );
        xmlParseMap.insert( make_pair( "B9",  BUTTON_09 ) );
        xmlParseMap.insert( make_pair( "B10", BUTTON_10 ) );
        xmlParseMap.insert( make_pair( "B11", BUTTON_11 ) );
        xmlParseMap.insert( make_pair( "B12", BUTTON_12 ) );
        xmlParseMap.insert( make_pair( "B13", BUTTON_13 ) );
        xmlParseMap.insert( make_pair( "B14", BUTTON_14 ) );
        xmlParseMap.insert( make_pair( "B15", BUTTON_15 ) );
        xmlParseMap.insert( make_pair( "B16", BUTTON_16 ) );
        xmlParseMap.insert( make_pair( "B17", BUTTON_17 ) );
        xmlParseMap.insert( make_pair( "B18", BUTTON_18 ) );
        xmlParseMap.insert( make_pair( "B19", BUTTON_19 ) );
        xmlParseMap.insert( make_pair( "B20", BUTTON_20 ) );
        xmlParseMap.insert( make_pair( "B21", BUTTON_21 ) );
        xmlParseMap.insert( make_pair( "B22", BUTTON_22 ) );
        xmlParseMap.insert( make_pair( "B23", BUTTON_23 ) );
        xmlParseMap.insert( make_pair( "B24", BUTTON_24 ) );
        xmlParseMap.insert( make_pair( "B25", BUTTON_25 ) );
        xmlParseMap.insert( make_pair( "B26", BUTTON_26 ) );
        xmlParseMap.insert( make_pair( "B27", BUTTON_27 ) );
        xmlParseMap.insert( make_pair( "B28", BUTTON_28 ) );
        xmlParseMap.insert( make_pair( "B29", BUTTON_29 ) );
        xmlParseMap.insert( make_pair( "B30", BUTTON_30 ) );
        xmlParseMap.insert( make_pair( "B31", BUTTON_31 ) );
        xmlParseMap.insert( make_pair( "B32", BUTTON_32 ) );

        xmlParseMap.insert( make_pair( "LT", XBOX_360_TRIGGER_LEFT ) );
        xmlParseMap.insert( make_pair( "RT", XBOX_360_TRIGGER_RIGHT ) );

        xmlParseMap.insert( make_pair( "UP",         POV_UP ) );
        xmlParseMap.insert( make_pair( "UP RIGHT",   POV_UP_RIGHT ) );
        xmlParseMap.insert( make_pair( "RIGHT",      POV_RIGHT ) );
        xmlParseMap.insert( make_pair( "DOWN RIGHT", POV_DOWN_RIGHT ) );
        xmlParseMap.insert( make_pair( "DOWN",       POV_DOWN ) );
        xmlParseMap.insert( make_pair( "DOWN LEFT",  POV_DOWN_LEFT ) );
        xmlParseMap.insert( make_pair( "LEFT",       POV_LEFT ) );
        xmlParseMap.insert( make_pair( "UP LEFT",    POV_UP_LEFT ) );

        xmlParseMap.insert( make_pair( "L STICK UP",    ANALOG1_UP ) );
        xmlParseMap.insert( make_pair( "L STICK DOWN",  ANALOG1_DOWN ) );
        xmlParseMap.insert( make_pair( "L STICK LEFT",  ANALOG1_LEFT ) );
        xmlParseMap.insert( make_pair( "L STICK RIGHT", ANALOG1_RIGHT ) );

        xmlParseMap.insert( make_pair( "R STICK UP",    ANALOG2_UP ) );
        xmlParseMap.insert( make_pair( "R STICK DOWN",  ANALOG2_DOWN ) );
        xmlParseMap.insert( make_pair( "R STICK LEFT",  ANALOG2_LEFT ) );
        xmlParseMap.insert( make_pair( "R STICK RIGHT", ANALOG2_RIGHT ) );

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
int CJoyPadController::ScanForButtonPress() const
{
    int result = NDevice::JOYPAD_BUTTON_NULL;

    // Check for button press
    for( int i = 0; i < NDevice::MAX_JOYPAD_BTNS; ++i )
    {
        if( pollData.rgbButtons[i] & 0x80 )
        {
            result = i;
            break;
        }
    }

    // Was this a 360 trigger press?
    if( result == NDevice::JOYPAD_BUTTON_NULL && xInputDevice )
    {
        if( pollData.lZ > 0 )
            result = NDevice::XBOX_360_TRIGGER_LEFT;

        else if( pollData.lZ < 0 )
            result = NDevice::XBOX_360_TRIGGER_RIGHT;
    }

    // Check for POV press
    if( result == NDevice::JOYPAD_BUTTON_NULL )
    {
        if( pollData.rgdwPOV[0] != NDevice::POV_IDLE )
        {
            // needed to setup a false value for POV_DEGREE_0 because 
            // pollData.rgdwPOV[0] returns 0 for the 0 degree position press.
            // The default value of pollData.rgdwPOV[0] is -1
            if( pollData.rgdwPOV[0] == 0 )
                result = NDevice::POV_UP;

            else
                result = pollData.rgdwPOV[0];
        }
    }

    return result;

}	// ScanForButtonPress


/************************************************************************
*    desc:  Check to see if the device was used
*
*    ret:	bool - whether or not this device has provided input
************************************************************************/
bool CJoyPadController::WasDeviceUsed() const
{
    for( int i = 0; i < NDevice::MAX_JOYPAD_BTNS; ++i )
        if( pollData.rgbButtons[i] & 0x80 )
            return true;

    if( pollData.lRx != 0 )
        return true;
    if( pollData.lRy != 0 )
        return true;
    if( pollData.lRz != 0 )
        return true;
    if( pollData.lX != 0 )
        return true;
    if( pollData.lY != 0 )
        return true;
    if( pollData.lZ != 0 )
        return true;

    if( pollData.rgdwPOV[0] != NDevice::POV_IDLE )
        return true;

    return false;

}	// WasDeviceUsed


/************************************************************************
*    desc:  Get the component string Id
*
*    param: string
************************************************************************/
const std::string & CJoyPadController::GetComponetIdStr( int id )
{
    // See if the action has already been added
    xmlSaveIter = xmlSaveMap.find( NDevice::EJoyPadComponetId(id) );

    // If it's found, return the string
    if( xmlSaveIter != xmlSaveMap.end() )
        return xmlSaveIter->second;

    else
        return xmlSaveMap[NDevice::JOYPAD_BUTTON_NULL];

}	// GetComponetIdStr
