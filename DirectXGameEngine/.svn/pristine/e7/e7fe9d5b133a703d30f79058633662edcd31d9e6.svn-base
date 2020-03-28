
/************************************************************************
*    FILE NAME:       socket.cpp
*
*    DESCRIPTION:     Networking socket class
************************************************************************/

// Game OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

// Physical component dependency
#include "networking\\socketclient.h"

// Standard lib dependencies
#include <assert.h>

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CSocketClient::CSocketClient()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CSocketClient::~CSocketClient()
{
}	// destructer


/************************************************************************
*    desc:  Create the server socket
************************************************************************/
bool CSocketClient::Create( uint ip, int port, bool _nonblocking, int typeSpec, int protocol, int addrFamilySpec )
{
	nonblocking = _nonblocking;

	if( !CWinSocket::Create( addrFamilySpec, typeSpec, protocol ) )
		return false;

	// Bind the socket to the server's ip address
	socketAddrIn.sin_family = addrFamilySpec;
	socketAddrIn.sin_addr.s_addr = ip;
	socketAddrIn.sin_port = htons(port);

	if( nonblocking )
	{
		u_long arg = 1;
		int errorCode = ioctlsocket( hSocket, FIONBIO, &arg );

		if( errorCode == SOCKET_ERROR )
		{
			DisplayError( WSAGetLastError() );

			return false;
		}
	}

	if( typeSpec == SOCK_DGRAM )
	{
		if( bind( hSocket, (LPSOCKADDR)&socketAddrIn, sizeof(socketAddrIn) ) == SOCKET_ERROR )
		{
			DisplayError( WSAGetLastError() );

			return false;
		}
	}

	return true;

}   // Create


/************************************************************************
*    desc:  Connect to a server
************************************************************************/
bool CSocketClient::Connect()
{
	// Try to connect to the server
	if( connect( hSocket, (LPSOCKADDR)&socketAddrIn, sizeof(socketAddrIn) ) == SOCKET_ERROR )
	{
		if( nonblocking && (WSAGetLastError() == WSAEWOULDBLOCK) )
			return true;

		DisplayError( WSAGetLastError() );

		return false;
	}

	return true;

}	// Connect


