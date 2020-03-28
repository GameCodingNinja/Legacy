
/************************************************************************
*    FILE NAME:       socket.h
*
*    DESCRIPTION:     Networking socket class
************************************************************************/

#ifndef __socket_server_h__
#define __socket_server_h__

// Physical component dependency
#include "networking\\winsocket.h"

// Standard lib dependencies
#include <vector>

class CSocketServer : public CWinSocket
{
public:

    // Constructor
    CSocketServer();

	// Destructor
	virtual ~CSocketServer();

	// Create the socket
	bool Create( int port, bool _nonblocking = true, int typeSpec = SOCK_STREAM, int protocol = 0, int addrFamilySpec = AF_INET );

	// Listen for incoming connections
	bool Listen( int listenQueueSize );

	// Accept incoming connections
	bool Accept();

	// Shutdown the clients
	void ShutDownClients();

	// Get the number of clients connected
	uint GetClientCount(){ return socketVec.size(); }

	// get the last connected client
	CSocket & GetLastClient(){ return socketVec.back(); }

private:

	std::vector<CSocket> socketVec;

};

#endif  // __socket_server_h__


