
/************************************************************************
*    FILE NAME:       boostservertcp.h
*
*    DESCRIPTION:     Boost server TCP
************************************************************************/

#ifndef __boost_server_tcp_h__
#define __boost_server_tcp_h__

// Physical component dependency
#include "networking\\boosttcp.h"

// Standard lib dependencies
#include <vector>

// Game lib dependencies
#include "networking\\boostsockettcp.h"

// Forward declaration(s)

class CBoostServerTCP : public CBoostTCP
{
public:

    // Constructor
    CBoostServerTCP( uint maxConnections = 1 );

	// Destructor
	virtual ~CBoostServerTCP();

	// Create the io service and socket
	void Create( const std::string & host, const std::string & port );

	// Start the listening for socket connections
	void ListenForAccept();

	// Get the socket vector
	std::vector< boost::shared_ptr< CBoostSocketTCP > > & GetSocketVec(){ return socketVec; }

	// Was there an error?
	bool IsError(){ return (errorCode.value() > 0); }

	// Is the server full?
	bool IsServerFull(){ return (socketVec.size() == MAX_CONNECTIONS); }

	// Check all the services
	void CheckServices();

protected:

	// Handle the accept
	void HandleAccept( boost::shared_ptr< CBoostSocketTCP > spSocket,
					   const boost::system::error_code & error );

private:

	// Acceptor shared pointer for accepting new connections
	boost::shared_ptr< boost::asio::ip::tcp::acceptor > spAcceptor;

	// Vector to hold shared socket pointers
	std::vector< boost::shared_ptr< CBoostSocketTCP > > socketVec;

	// Id number
	uint uniqueId;

	// Max number of connections
	const uint MAX_CONNECTIONS;

	// Send flags
	bool acceptBusy;

	// Boost error code
	boost::system::error_code errorCode;

};

#endif  // __boost_server_tcp_h__


