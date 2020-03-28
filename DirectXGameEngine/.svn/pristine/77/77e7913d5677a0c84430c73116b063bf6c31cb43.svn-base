
/************************************************************************
*    FILE NAME:       boostclienttcp.h
*
*    DESCRIPTION:     Boost TCP client
************************************************************************/

#ifndef __boost_client_tcp_h__
#define __boost_client_tcp_h__

// Physical component dependency
#include "networking\\boosttcp.h"

// Game lib dependencies
#include "networking\\boostsockettcp.h"

// Forward declaration(s)

class CBoostClientTCP : public CBoostTCP
{
public:

    // Constructor
    CBoostClientTCP();

	// Destructor
	virtual ~CBoostClientTCP();

	// Create the io service and socket
	void Create( const std::string & host, const std::string & port );

	// Try to connect to server or peer
	void Connect();

	// Check all the services
	void CheckServices();

	// Get the socket
	boost::shared_ptr< CBoostSocketTCP > & GetSocket(){ return spSocket; };

private:

	// Handle the connect
	void HandleConnect( const boost::system::error_code & error );

private:

	// Boost Shared Pointer TCP Socket
	boost::shared_ptr< CBoostSocketTCP > spSocket;

	// Boost error code
	boost::system::error_code errorCode;

};

#endif  // __boost_client_tcp_h__


