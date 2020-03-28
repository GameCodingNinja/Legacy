
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
#include "networking\\winsocket.h"

// Standard lib dependencies
#include <assert.h>

// Standard lib dependencies
#include "utilities\\genfunc.h"

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CWinSocket::CWinSocket()
{
	ZeroMemory( &wsaData, sizeof(wsaData) );

}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CWinSocket::~CWinSocket()
{
	Shutdown();

	if( wsaData.wVersion != 0 )
		WSACleanup();

}	// destructer


/************************************************************************
*    desc:  Create the socket
************************************************************************/
bool CWinSocket::Create( int addrFamilySpec, int typeSpec, int protocol )
{
	if( wsaData.wVersion == 0 )
	{
		// Initialize the socket and set our socket version. Loading up version 2.2
		int errorCode = WSAStartup( MAKEWORD(2, 2), &wsaData );

		if( errorCode != 0 || wsaData.wVersion != MAKEWORD(2, 2) )
		{
			DisplayError( errorCode );

			return false;
		}
	}

	// Shutdown the socket if one has been created
	Shutdown();

	// Create the socket
	if( (hSocket = socket( addrFamilySpec, typeSpec, protocol )) == INVALID_SOCKET )
	{
		DisplayError( WSAGetLastError() );

		return false;
	}

	return true;

}   // Create






