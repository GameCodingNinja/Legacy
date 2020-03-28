
/************************************************************************
*    FILE NAME:       gamecontroller.h
*
*    DESCRIPTION:     game controller class
************************************************************************/

#ifndef __game_controller_h__
#define __game_controller_h__

// Windows lib dependencies
#include <atlbase.h>

// Standard lib dependencies
#include <vector>
#include <map>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Game lib dependencies
#include <controller/controllerdefs.h>
#include <controller/devicemovement.h>
#include <common/point.h>
#include <common/defs.h>
#include <xmlParser/xmlParser.h>

// Forward declaration(s)
class CKeyboardController;
class CMouseController;
class CJoyPadController;
class CBaseController;

class CGameController : public boost::noncopyable
{
public:

    // Get the instance of the singleton class
    static CGameController & Instance()
    {
        static CGameController gameController;
        return gameController;
    }

    // Init 
    bool Init( HWND handleWnd );

    // Add controllers to list to poll data from
    void AddToPollLst( NDevice::EDeviceId id, unsigned int index = 0 );

    // Poll the active controllers
    void Poll( bool hasFocus );

    // bind the action to the event
    void BindActionToEvent( std::string & actionStr,
                            NDevice::EDeviceId deviceId,
                            int componetId );

    // Remove this action from the devices event
    void RemoveActionFromEvent( std::string & actionStr, NDevice::EDeviceId id );

    // seach for the action reset it
    void ResetAction( const std::string & actionStr );

    // Poll data for button or key presses
    NDevice::EActionPress WasAction( const std::string & actionStr );
    bool WasAction( const std::string & actionStr, NDevice::EActionPress actionPress );

    // Add the player device Id
    void AddPlayerDeviceId( NDevice::EDevicePlayerId devicePlayerId );

    // Get the movement data
    const CDeviceMovement & GetMovement();

    // Ask if the mouse has been moved
    bool WasMouseMoved();

    // Load the action mappings from xml
    void LoadActionFromXML( const std::string & filePath );
    void LoadAction();

    // Load the JoyPad action mappings from xml
    void LoadJoyPadActionFromXML();

    // Set the dead zone of the joypad
    void SetJoypadDeadZone(int value);

    // Set the joypad axis amount
    void SetJoypadAxisAmount(int value);

    // Check for gamepad controllers
    bool CheckForGamepadControllers();

    // Get the number of joypads currently connected
    size_t GetJoypadCount();

    // Get the last device used
    NDevice::EDeviceId GetLastDevicedUsed();

    // Get the last multiplayer device used
    NDevice::EMultiPlayerId GetLastMultiplierDevicedUsed();

    // Get the action/component strings for the give device id
    bool GetDeviceActionStr( NDevice::EDeviceId deviceId, const std::string & actionNameStr, std::string & componetIdStr, bool & configurable );

    // Scan device for button press
    int ScanDeviceForButtonPress( NDevice::EDeviceId deviceId );

    // Get the device component string Id
    std::string GetDeviceComponetIdStr( NDevice::EDeviceId deviceId, int componetId );

    // Save the xml to file
    void SaveToFile();

    // Set the key binding
    bool SetKeyBinding( NDevice::EDeviceId deviceId, const std::string & actionNameStr, int & componetId );

    // Get the the standard button string to xbox conversion for display
    std::string GetJoypadDisplayStr( std::string & componetIdStr );

    // Set the new component string
    bool SetComponetStr( const XMLNode & playerVisibleNode, std::string & actionNameStr, std::string & componetIdStr );

    // Reset all the key bindings to their default settings
    void ResetKeyBindingsToDefault();

    // Get the absolute mouse position
    const CPoint & GetAbsolutePos() const;

    // Get the absolute mouse position translated to our system
    const CPoint & GetAbsolutePosScaled() const;

    // Get the relative mouse position
    const CPoint & GetRelativePos() const;

    // Get the relative mouse position translated to our system
    const CPoint & GetRelativePosScaled() const;

    // Set the this ptr of the control acting on the mouse down
    // This is only used to compare against other void pointers
    // to determine ownership of action
    void SetActionOwner( void * pthisPtr );
    bool IsActionOwner( void * pthisPtr );

private:

    // Get the action/component strings for the keyboard device id
    bool GetActionStr( const XMLNode & playerVisibleNode, const std::string & actionNameStr, std::string & componetIdStr, bool & configurable );

    // enumerated call back function -  needs to be static to be passed as function pointer
    static BOOL CALLBACK EnumDevicesCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext );

    // enumerated gamepad call back function -  needs to be static to be passed as function pointer
    static BOOL CALLBACK EnumJoyPadCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext );

    // Create the device from enumeration
    BOOL CreateDevice( const DIDEVICEINSTANCE* pInst );

    // Create the device from enumeration
    BOOL CreateJoyPadDevice( const DIDEVICEINSTANCE* pInst );

    // Display error information
    void DisplayError( HRESULT hr );

private:

    // Constructor
    CGameController();

    // Destructor
    ~CGameController();

private:

    // Controller smart pointers
    boost::scoped_ptr<CKeyboardController> spKeyboardController;
    boost::scoped_ptr<CMouseController> spMouseController;

    // Controller smart vector
    boost::ptr_vector<CJoyPadController> spJoyPadVec;

    // Direct input smart pointer
    CComPtr<IDirectInput8> spDirectInput;

    // handle to window
    HWND hWnd;

    // Active controller list
    std::map< NDevice::EDeviceId, CBaseController * > activeMap;

    // device movement
    CDeviceMovement devMovement;

    // Id of the last devise used
    NDevice::EDeviceId lastDeviceUsed;

    // xml node
    XMLNode mainNode;

    // Button mapping for xInput controller
    std::map< std::string, std::string > butonToXInputMap;

    // flag for xml action changes
    bool xmlActionChanges;

    // The save file path
    std::string saveFilePath;

    // Dummy point
    CPoint pointDummy;

    // input control owner
    // DO not free, touch or do anything with this pointer
    void * pActionOwnerThisPtr;

    // Human interface device check counter(s)
    uint hiDeviceCount;
    uint hiDeviceCounter;

    // Multiplayer Id vector
    std::vector<NDevice::EDevicePlayerId> devicePlayerIdVec;

    // Multiplayer Id
    NDevice::EMultiPlayerId multiPlayerId;

};

#endif  // __game_controller_h__
