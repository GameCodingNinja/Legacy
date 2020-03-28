
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
#include "networking\\socket.h"

// Standard lib dependencies
#include <assert.h>

// Standard lib dependencies
#include "utilities\\genfunc.h"

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CSocket::CSocket()
	   : hSocket(INVALID_SOCKET),
	     nonblocking(false)
{
	ZeroMemory( &socketAddrIn, sizeof(socketAddrIn) );

}   // constructor


/************************************************************************
*    desc:  Constructer
************************************************************************/
CSocket::CSocket( SOCKET _hSocket, SOCKADDR_IN &_socketAddrIn )
	   : hSocket(_hSocket),
	     socketAddrIn(_socketAddrIn),
	     nonblocking(false)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CSocket::~CSocket()
{
}	// destructer


/************************************************************************
*    desc:  Send data on a connected socket
************************************************************************/
int CSocket::Send( const char *buf, const int len, const int flags )
{
	if( send( hSocket, buf, len, flags ) == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();

		if( errorCode != WSAEWOULDBLOCK )  // WSAECONNRESET
		{
			DisplayError( errorCode );

			return -1;
		}

		return 0;
	}

	return 1;

}	// Send


/************************************************************************
*    desc:  Receive data on a connected socket
************************************************************************/
int CSocket::Receive( char *buf, const int len, const int flags )
{
	if( recv( hSocket, buf, len, flags ) == SOCKET_ERROR )
	{
		int errorCode = WSAGetLastError();

		if( errorCode != WSAEWOULDBLOCK ) // WSAECONNRESET
		{
			DisplayError( errorCode );

			return -1;
		}

		return 0;
	}

	return 1;

}	// Send


/************************************************************************
*    desc:  Shutdown the socket
************************************************************************/
void CSocket::Shutdown()
{
	const uint SD_BOTH(2);

	if( hSocket != INVALID_SOCKET )
	{
		shutdown( hSocket, SD_BOTH );

		closesocket( hSocket );

		hSocket = INVALID_SOCKET;
	}

}	// Shutdown


/************************************************************************
*    desc:  Display the error message
************************************************************************/
void CSocket::DisplayError( int errorCode )
{
	switch( errorCode )
	{
		case WSASYSNOTREADY:
		{
			NGenFunc::PostDebugMsg( "WSA Startup Error: %s",
					 "The underlying network subsystem is not ready for network communication." );

			break;
		}
		case WSAVERNOTSUPPORTED:
		{
			NGenFunc::PostDebugMsg( "WSA Startup Error: %s",
					 "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation." );

			break;
		}
		case WSAEPROCLIM:
		{
			NGenFunc::PostDebugMsg( "WSA Startup Error: %s",
					 "A limit on the number of tasks supported by the Windows Sockets implementation has been reached." );

			break;
		}
		case WSAEFAULT:
		{
			NGenFunc::PostDebugMsg( "WSA Startup Error: %s",
					 "The lpWSAData parameter is not a valid pointer." );

			break;
		}
		case WSANOTINITIALISED:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "A successful WSAStartup call must occur before using this function." );

			break;
		}
		case WSAENETDOWN:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The network subsystem or the associated service provider has failed." );

			break;
		}
		case WSAEAFNOSUPPORT:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The specified address family is not supported." );

			break;
		}
		case WSAEINPROGRESS:
		{
			NGenFunc::PostDebugMsg( "WSA startup/Socket Error: %s",
					 "A blocking Windows Sockets call is in progress, or the service provider is still processing a callback function." );

			break;
		}
		case WSAEMFILE:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "No more socket descriptors are available." );

			break;
		}
		case WSAEINVAL:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "An invalid argument was supplied." );

			break;
		}
		case WSAENOBUFS:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "No buffer space is available. The socket cannot be created." );

			break;
		}
		case WSAEPROTONOSUPPORT:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The specified protocol is not supported." );

			break;
		}
		case WSAEPROTOTYPE:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The specified protocol is the wrong type for this socket." );

			break;
		}
		case WSAEPROVIDERFAILEDINIT:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The service provider failed to initialize. This error is returned if a layered service provider (LSP) or namespace provider was improperly installed or the provider fails to operate correctly." );

			break;
		}
		case WSAESOCKTNOSUPPORT:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The specified socket type is not supported in this address family." );

			break;
		}
		case WSAEACCES:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "An attempt was made to access a socket in a way forbidden by its access permissions." );

			break;
		}
		case WSAEADDRINUSE:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "Only one usage of each socket address (protocol/network address/port) is normally permitted." );

			break;
		}
		case WSAEADDRNOTAVAIL:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The requested address is not valid in its context." );

			break;
		}
		case WSAENOTSOCK:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "An operation was attempted on something that is not a socket." );

			break;
		}
		case WSAEISCONN:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The socket is already connected." );

			break;
		}
		case WSAEOPNOTSUPP:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "Socket option is not supported." );

			break;
		}
		case WSAECONNRESET:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "An incoming connection was indicated, but was subsequently terminated by the remote peer prior to accepting the call." );

			break;
		}
		case WSAEINTR:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "A blocking Windows Sockets call was canceled through WSACancelBlockingCall." );

			break;
		}
		case WSAEWOULDBLOCK:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "A blocking Windows Sockets call was canceled through WSACancelBlockingCall." );

			break;
		}
		case WSAEALREADY:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "A nonblocking connect call is in progress on the specified socket." );

			break;
		}
		case WSAECONNREFUSED:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The attempt to connect was forcefully rejected." );

			break;
		}
		case WSAENETUNREACH:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The network cannot be reached from this host at this time." );

			break;
		}
		case WSAEHOSTUNREACH:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "A socket operation was attempted to an unreachable host." );

			break;
		}
		case WSAETIMEDOUT:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "An attempt to connect timed out without establishing a connection." );

			break;
		}
		case WSAENOTCONN:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The socket is not connected." );

			break;
		}
		case WSAENETRESET:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity that detected a failure while the operation was in progress. For a datagram socket, this error indicates that the time to live has expired." );

			break;
		}
		case WSAESHUTDOWN:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The socket has been shut down." );

			break;
		}
		case WSAEMSGSIZE:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The message was too large to fit into the specified buffer and was truncated." );

			break;
		}
		case WSAECONNABORTED:
		{
			NGenFunc::PostDebugMsg( "WSA Socket Error: %s",
					 "The virtual circuit was terminated due to a time-out or other failure." );

			break;
		}
		default:
		{
			NGenFunc::PostDebugMsg( "WSA Error: %s",
						"The socket is marked as nonblocking and no connections are present to be accepted." );

			break;
		}
	}
}	// DisplayError