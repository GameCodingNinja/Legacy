
/************************************************************************
*    FILE NAME:       joypadcontroller.h
*
*    DESCRIPTION:     joystick / gamepad controller class
************************************************************************/

#ifndef __joypad_controller_h__
#define __joypad_controller_h__

// Physical component dependency
#include <controller/basecontroller.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <controller/axischeck.h>
#include <controller/keybuttonaction.h>

class CJoyPadController : public CBaseController
{
public:

    // Constructor
    CJoyPadController();

    // Destructor
    virtual ~CJoyPadController();

    // Init the device
    virtual HRESULT InitDevice( LPDIRECTINPUTDEVICE8 pDev,
                                const DIDEVICEINSTANCE* pInst,
                                HWND handleWnd );

    // Poll the device
    virtual HRESULT Poll();

    // Clear the whole action map
    void ClearActionMap();

    // seach for the action and remove it from the map
    virtual void RemoveActionFromMap( const std::string & actionStr );

    // seach for the action reset it
    virtual void ResetAction( const std::string & actionStr );

    // seach for the action and see if the data was pressed
    virtual NDevice::EActionPress WasActionPressed( const std::string & actionStr );

    // Set the movement data
    virtual void SetMovement( CDeviceMovement & movement );

    // Reset the query flag
    virtual void ResetQuery();

    // Check to see if the device was used
    virtual bool WasDeviceUsed() const;

    // Is ths an xInput device
    virtual bool IsXInput() const { return xInputDevice; }

    // Scan for a button press
    virtual int ScanForButtonPress() const;

    // Get the component string Id
    virtual const std::string & GetComponetIdStr( int id );

    // seach for the action and map it to a given component index
    void SetActionToMap( const std::string & actionStr, NDevice::EJoyPadComponetId id );

    // Load action data from xml node
    void LoadActionFromXMLNode( const XMLNode & node );

    // Set the axis amount
    void SetAxisAmount(int value);

    // Set the dead zone amount
    void SetDeadZoneAmount(int value);

    // clear the poll data
    virtual void ClearPollData();

private:

    // Query device options
    virtual BOOL QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst );

    // Init the controller xml parse map
    void InitControllerXmlParseMap();

    // Is this an xInput device
    bool IsXInputDevice( const GUID* pGuidProductFromDirectInput );

    // Load action data from xml node
    void LoadActionFromNode( const XMLNode & node );

private:

    // flag indicating xInput device
    bool xInputDevice;

    // For setting the device range
    std::vector<DIPROPRANGE> diprgVec;

    // For setting the device's dead zone
    std::vector<DIPROPDWORD> dilongVec;

    // JoyPad poll data
    DIJOYSTATE pollData;

    // Records which axis is available
    CAxisCheck axisCheck;

    // Component action list
    boost::ptr_map< const std::string, CKeyButtonAction<NDevice::EJoyPadComponetId> > spActionMap;
    boost::ptr_map< const std::string, CKeyButtonAction<NDevice::EJoyPadComponetId> >::iterator actionMapIter;

    // Component action list
    // NOTE: These are static so that the same data is not held by each connected joypad controller
    static std::map< std::string, NDevice::EJoyPadComponetId > xmlParseMap;
    std::map< std::string, NDevice::EJoyPadComponetId >::iterator xmlParseIter;

    static std::map< NDevice::EJoyPadComponetId, std::string > xmlSaveMap;
    std::map< NDevice::EJoyPadComponetId, std::string >::iterator xmlSaveIter;

    // Axis amount
    int axisAmount;
};

#endif  // __joypad_controller_h__
