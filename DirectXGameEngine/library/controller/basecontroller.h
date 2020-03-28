
/************************************************************************
*    FILE NAME:       basecontroller.h
*
*    DESCRIPTION:     base controller class
************************************************************************/

#ifndef __base_controller_h__
#define __base_controller_h__

// Windows lib dependencies
#include <atlbase.h>

// Standard lib dependencies
#include <string>
#include <map>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <controller/controllerdefs.h>
#include <controller/devicemovement.h>
#include <xmlParser/xmlParser.h>

class CBaseController : public boost::noncopyable
{
public:

    // Constructor
    CBaseController();

    // Destructor
    virtual ~CBaseController();

    // Init the device
    virtual HRESULT InitDevice( LPDIRECTINPUTDEVICE8 pDev,
                                const DIDEVICEINSTANCE* pInst,
                                HWND handleWnd );

    // start a device query
    HRESULT EnumDeviceQuery();

    // Acquire this device so that it can be polled
    void AcquireDevice();

    // Unacquire this device
    void UnacquireDevice();

    // Poll the device
    virtual HRESULT Poll();

    // Get the component count
    int GetComponentCount() const;

    // Get the name
    const std::string & GetName() const;

    // Get/Set the device id
    NDevice::EDeviceId GetDeviceID() const;
    void SetDeviceID( NDevice::EDeviceId id );

    // seach for the action reset it
    virtual void ResetAction( const std::string & actionStr ) = 0;

    // seach for the action and see if the data was pressed
    virtual NDevice::EActionPress WasActionPressed( const std::string & actionStr ) = 0;

    // Set the movement data
    virtual void SetMovement( CDeviceMovement & movement ) = 0;

    // Load action data from xml node
    virtual void ResetQuery() = 0;

    // Check to see if the device was used
    virtual bool WasDeviceUsed() const = 0;

    // Is ths an xInput device
    virtual bool IsXInput() const { return false; }

    // Scan for a key press
    virtual int ScanForButtonPress() const = 0;

    // Get the component string Id
    virtual const std::string & GetComponetIdStr( int id ) = 0;

    // Get the component string Id
    virtual void ClearPollData() = 0;

    // Get the movement of this device
    const CDeviceMovement & GetMovement() const;

    // Get/Set the device player Id
    NDevice::EDevicePlayerId GetDevicePlayerID() const;
    void SetDevicePlayerID( NDevice::EDevicePlayerId id );

protected:

    // enumerated call back function -  needs to be static for passing function pointer
    static BOOL CALLBACK EnumDeviceQueryCallback( const DIDEVICEOBJECTINSTANCE * pInst, VOID* pContext );

    // pure virtual function
    virtual BOOL QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst ) = 0;

    // Pointer to device
    CComPtr<IDirectInputDevice8> spDevice;

    // Device name
    std::string name;

    // Device ID
    NDevice::EDeviceId deviceId;

    // Component count
    int componentCount;

    // Window Handle
    HWND hWnd;

    // device movement
    CDeviceMovement devMovement;

    // Device player Id
    NDevice::EDevicePlayerId devicePlayerId;

};

#endif  // __base_controller_h__
