
/************************************************************************
*    FILE NAME:       socket.h
*
*    DESCRIPTION:     Networking socket class
************************************************************************/

#ifndef __socket_client_h__
#define __socket_client_h__

// Physical component dependency
#include "networking\\winsocket.h"

// Standard lib dependencies
#include <string>

// Game lib dependency
#include "common\\defs.h"

class CSocketClient : public CWinSocket
{
public:

    // Constructor
    CSocketClient();

	// Destructor
	virtual ~CSocketClient();

	// Create the socket
	bool Create( uint ip, int port, bool _nonblocking = true, int typeSpec = SOCK_STREAM, int protocol = 0, int addrFamilySpec = AF_INET );

	// Connect to a server
	bool Connect();

};

#endif  // __socket_client_h__


