
/************************************************************************
*    FILE NAME:       keyboardcontroller.h
*
*    DESCRIPTION:     keyboard controller class
************************************************************************/

#ifndef __keyboard_controller_h__
#define __keyboard_controller_h__

// Physical component dependency
#include <controller/basecontroller.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <controller/keybuttonaction.h>

class CKeyboardController : public CBaseController
{
public:

    // Constructor
    CKeyboardController();

    // Destructor
    virtual ~CKeyboardController();

    // Init the device
    virtual HRESULT InitDevice( LPDIRECTINPUTDEVICE8 pDev,
                                const DIDEVICEINSTANCE* pInst,
                                HWND handleWnd);

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

    // Scan for a key press
    virtual int ScanForButtonPress() const;

    // Get the component string Id
    virtual const std::string & GetComponetIdStr( int id );

    // seach for the action and map it to a given component index
    void SetActionToMap( const std::string & actionStr, NDevice::EKeyboardComponetId id );

    // Load action data from xml node
    void LoadActionFromXMLNode( const XMLNode & node );

    // clear the poll data
    virtual void ClearPollData();

private:

    // Load action data from xml node
    void LoadActionNode( const XMLNode & node );

    // Query device options
    virtual BOOL QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst );

    // Init the controller xml parse map
    void InitControllerXmlParseMap();

private:

    // keyboard poll data
    char pollData[NDevice::MAX_KEYS];

    // Component action map
    boost::ptr_map< const std::string, CKeyButtonAction<NDevice::EKeyboardComponetId> > spActionMap;
    boost::ptr_map< const std::string, CKeyButtonAction<NDevice::EKeyboardComponetId> >::iterator actionMapIter;

    std::map< std::string, NDevice::EKeyboardComponetId > xmlParseMap;
    std::map< std::string, NDevice::EKeyboardComponetId >::iterator xmlParseIter;

    std::map< NDevice::EKeyboardComponetId, std::string > xmlSaveMap;
    std::map< NDevice::EKeyboardComponetId, std::string >::iterator xmlSaveIter;

};

#endif  // __keyboard_controller_h__
