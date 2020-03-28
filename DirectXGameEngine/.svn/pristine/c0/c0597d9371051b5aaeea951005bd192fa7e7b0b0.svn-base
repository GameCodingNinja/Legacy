
/************************************************************************
*    FILE NAME:       basecontroller.h
*
*    DESCRIPTION:     base controller class
************************************************************************/

// Physical component dependency
#include <controller/basecontroller.h>

// Standard lib dependencies
#include <string>

// Game lib dependencies
#include <utilities/genfunc.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CBaseController::CBaseController()
                : componentCount(0),
                  hWnd(NULL),
                  devicePlayerId(NDevice::MOUSE_KEYBOARD)
{
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CBaseController::~CBaseController()
{
    UnacquireDevice();

}   // Destructer


/************************************************************************
*    desc:  init the device
*
*    param: LPDIRECTINPUTDEVICE8 pDev - pointer to device
*           string devName - name of device
*           HWND handleWnd - handle to window
************************************************************************/
HRESULT CBaseController::InitDevice( LPDIRECTINPUTDEVICE8 pDev, 
                                     const DIDEVICEINSTANCE* pInst,
                                     HWND handleWnd )
{
    spDevice.Attach( pDev );
    name = pInst->tszProductName;
    hWnd = handleWnd;

    return DI_OK;

}   // Constructer


/************************************************************************
*    desc:  start a device query
************************************************************************/
HRESULT CBaseController::EnumDeviceQuery()
{
    // Enumerate the joystick objects.
    return spDevice->EnumObjects( EnumDeviceQueryCallback, this, DIDFT_ALL );

}	// EnumDeviceQuery


/************************************************************************
*    desc:  enumerated call back function for devices
*
*    param: const DIDEVICEINSTANCE* pInst - device instance
*           VOID* pContext - used in the case as the "this" pointer
************************************************************************/
BOOL CALLBACK CBaseController::EnumDeviceQueryCallback( const DIDEVICEOBJECTINSTANCE * pInst,
                                                        VOID* pContext )
{
    // Cast out pointer
    CBaseController * pGameController = NGenFunc::ReintCast<CBaseController *>(pContext);

    return pGameController->QueryDevice( pInst );

}	// EnumObjectsCallback


/************************************************************************
*    desc:  Acquire this device so that it can be polled
************************************************************************/
void CBaseController::AcquireDevice()
{
    spDevice->Acquire();

}	// AcquireDevice


/************************************************************************
*    desc:  Unacquire this device
************************************************************************/
void CBaseController::UnacquireDevice()
{
    spDevice->Unacquire();

}	// UnacquireDevice


/************************************************************************
*    desc:  Poll the device
*
*    ret: HRESULT - result of polling
************************************************************************/
HRESULT CBaseController::Poll()
{
    HRESULT hr;

    // Poll the device to read the current state
    if( FAILED( hr = spDevice->Poll() ) )
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = spDevice->Acquire();
        while( hr == DIERR_INPUTLOST )
            hr = spDevice->Acquire();

        // If we encounter a fatal error, return failure.
        if( (hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED) )
            return E_FAIL;

        // If another application has control of this device, return success.
        // We'll just have to wait our turn to use the joystick.
        if( hr == DIERR_OTHERAPPHASPRIO )
            return E_FAIL;

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return E_FAIL;
    }

    return DI_OK;

}	// Poll


/************************************************************************
*    desc:  Get the movement of this device
************************************************************************/
const CDeviceMovement & CBaseController::GetMovement() const
{
    return devMovement;

}	// GetMovement


/************************************************************************
*    desc:  Get the component count
*
*    ret: int - number of components
************************************************************************/
int CBaseController::GetComponentCount() const
{
    return componentCount;

}	// GetComponentCount


/************************************************************************
*    desc:  Get/Set the device id
************************************************************************/
NDevice::EDeviceId CBaseController::GetDeviceID() const
{
    return deviceId;

}	// GetDeviceID

void CBaseController::SetDeviceID( NDevice::EDeviceId id )
{
    deviceId = id;

}	// SetDeviceID


/************************************************************************
*    desc:  Get the name
************************************************************************/
const std::string & CBaseController::GetName() const
{
    return name;

}	// GetName


/************************************************************************
*    desc:  Get/Set the device player Id
************************************************************************/
NDevice::EDevicePlayerId CBaseController::GetDevicePlayerID() const
{
    return devicePlayerId;

}	// GetMultiplayerID

void CBaseController::SetDevicePlayerID( NDevice::EDevicePlayerId id )
{
    devicePlayerId = id;

}	// SetMultiplayerID
