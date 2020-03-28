
/************************************************************************
*    FILE NAME:       mousecontroller.h
*
*    DESCRIPTION:     mouse controller class
************************************************************************/

#ifndef __mouse_controller_h__
#define __mouse_controller_h__

// Physical component dependency
#include <controller/basecontroller.h>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Game lib dependencies
#include <controller/keybuttonaction.h>
#include <common/point.h>

class CMouseController : public CBaseController
{
public:

    // Constructor
    CMouseController();

    // Destructor
    virtual ~CMouseController();

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

    // Scan for a button press
    virtual int ScanForButtonPress() const;

    // Get the component string Id
    virtual const std::string & GetComponetIdStr( int id );

    // seach for the action and map it to a given component index
    void SetActionToMap( const std::string & actionStr, NDevice::EMouseComponetId id );

    // Load action data from xml node
    void LoadActionFromXMLNode( const XMLNode & node );

    // Get the absolute mouse position
    const CPoint & GetAbsolutePos() const;

    // Get the absolute mouse position translated to our system
    const CPoint & GetAbsolutePosScaled() const;

    // Get the relative mouse position
    const CPoint & GetRelativePos() const;

    // Get the relative mouse position translated to our system
    const CPoint & GetRelativePosScaled() const;

    // clear the poll data
    virtual void ClearPollData();

private:

    // Load action data from xml node
    void LoadActionNode( const XMLNode & node );

    // Query device options
    virtual BOOL QueryDevice( const DIDEVICEOBJECTINSTANCE * pInst );

    // Init the controller xml parse map
    void InitControllerXmlParseMap();

    // Get the windows api mouse position
    POINT GetWindowsAPIMousePos() const;

private:

    // Mouse poll data
    DIMOUSESTATE2 pollData;

    // Component action list
    boost::ptr_map< const std::string, CKeyButtonAction<NDevice::EMouseComponetId> > spActionMap;
    boost::ptr_map< const std::string, CKeyButtonAction<NDevice::EMouseComponetId> >::iterator actionMapIter;

    std::map< std::string, NDevice::EMouseComponetId > xmlParseMap;
    std::map< std::string, NDevice::EMouseComponetId >::iterator xmlParseIter;

    std::map< NDevice::EMouseComponetId, std::string > xmlSaveMap;
    std::map< NDevice::EMouseComponetId, std::string >::iterator xmlSaveIter;

    // Mouse Position
    CPoint curPos;
    CPoint lastPos;

    // Mouse position converted based on screen ratio
    CPoint curPosScaled;
    CPoint lastPosScaled;

    // Relative mouse positions
    CPoint relativePos;
    CPoint relativePosScaled;

};

#endif  // __mouse_controller_h__

