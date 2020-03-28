
/************************************************************************
*    FILE NAME:       socket.h
*
*    DESCRIPTION:     Networking socket class
************************************************************************/

#ifndef __socket_h__
#define __socket_h__

// Game lib dependencies
#include "common\\defs.h"

class CSocket
{
public:

    // Constructor
    CSocket();

	// Constructor
    CSocket( SOCKET _hSocket, SOCKADDR_IN &_socketAddrIn );

	// Destructor
	virtual ~CSocket();

	// Sends data on a connected socket
	int Send( const char *buf, const int len, const int flags = 0 );

	// Receive data on a connected socket
	int Receive( char *buf, const int len, const int flags = 0 );

	// Shutdown the socket
	void Shutdown();

	// Is the socket valid
	bool IsValid(){ return hSocket != INVALID_SOCKET; }

protected:

	// Display the error message
	void DisplayError( int errorCode );

protected:

	// Socket handle
	SOCKET hSocket;

	// connection info
	SOCKADDR_IN socketAddrIn;

	// Blocking socket flag
	bool nonblocking;

};

#endif  // __socket_h__


