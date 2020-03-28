
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
#include "networking\\socketserver.h"

// Standard lib dependencies
#include <assert.h>
#include <string>

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer
************************************************************************/
CSocketServer::CSocketServer()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CSocketServer::~CSocketServer()
{
	ShutDownClients();

}	// destructer


/************************************************************************
*    desc:  Create the server socket
************************************************************************/
bool CSocketServer::Create( int port, bool _nonblocking, int typeSpec, int protocol, int addrFamilySpec )
{
	nonblocking = _nonblocking;

	if( !CWinSocket::Create( addrFamilySpec, typeSpec, protocol ) )
		return false;

	// Bind the socket to any address and the specified port.
	socketAddrIn.sin_family = addrFamilySpec;
	socketAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddrIn.sin_port = htons(port);

	if( nonblocking )
	{
		u_long arg = 1;
		int errorCode = ioctlsocket(hSocket, FIONBIO, &arg);

		if( errorCode == SOCKET_ERROR )
		{
			DisplayError( WSAGetLastError() );

			return false;
		}
	}

	if( bind( hSocket, (LPSOCKADDR)&socketAddrIn, sizeof(socketAddrIn) ) == SOCKET_ERROR )
	{
		DisplayError( WSAGetLastError() );

		return false;
	}

	return true;

}   // Create


/************************************************************************
*    desc:  Listen for incoming connections
************************************************************************/
bool CSocketServer::Listen( int listenQueueSize )
{
	// Make the socket listen
	if( listen( hSocket, listenQueueSize ) == SOCKET_ERROR )
	{
		DisplayError( WSAGetLastError() );

		return false;
	}

	return true;

}	// Listen


/************************************************************************
*    desc:  Accept incoming connections
************************************************************************/
bool CSocketServer::Accept()
{
	SOCKADDR_IN socketAddrIn;
	ZeroMemory( &socketAddrIn, sizeof(socketAddrIn) );
	int sizeOfSocketAddrIn = sizeof(socketAddrIn);

	// Make the socket listen
	SOCKET hAcceptSocket = accept( hSocket, (LPSOCKADDR)&socketAddrIn, &sizeOfSocketAddrIn );

	if( hAcceptSocket == INVALID_SOCKET )
	{
		int error = WSAGetLastError();

		if( !nonblocking || (error != WSAEWOULDBLOCK) )
			DisplayError( error );

		return false;
	}

	// Add the socket to our vector list
	socketVec.push_back( CSocket(hAcceptSocket, socketAddrIn) );

	return true;

}	// Accept


/************************************************************************
*    desc:  Shutdown the clients
************************************************************************/
void CSocketServer::ShutDownClients()
{
	for( uint i = 0; i < socketVec.size(); i++ )
		socketVec[i].Shutdown();

	socketVec.clear();

}	// ShutDownClients






