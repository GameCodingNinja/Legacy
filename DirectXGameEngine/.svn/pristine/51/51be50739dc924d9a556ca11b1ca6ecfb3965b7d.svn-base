
/************************************************************************
*    FILE NAME:       winsocket.h
*
*    DESCRIPTION:     Networking socket class
************************************************************************/

#ifndef __win_socket_h__
#define __win_socket_h__

// Physical component dependency
#include "networking\\socket.h"

class CWinSocket : public CSocket
{
public:

    // Constructor
    CWinSocket();

	// Destructor
	virtual ~CWinSocket();

	// Create the socket
	bool Create( int addrFamilySpec, int typeSpec, int protocol );

protected:

	// Data structure to hold the details
	// of the Socket implementation
	WSADATA wsaData;

};

#endif  // __win_socket_h__


