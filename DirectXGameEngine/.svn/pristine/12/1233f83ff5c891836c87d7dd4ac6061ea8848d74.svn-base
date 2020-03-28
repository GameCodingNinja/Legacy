
/************************************************************************
*    FILE NAME:       boostclienttcp.h
*
*    DESCRIPTION:     Boost TCP client
************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

// Physical component dependency
#include "networking\\boostclienttcp.h"

// Standard lib dependencies

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

// Game lib dependencies
#include "utilities\\genfunc.h"

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CBoostClientTCP::CBoostClientTCP()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CBoostClientTCP::~CBoostClientTCP()
{
}	// destructer


/************************************************************************
*    desc:  Create the io service and socket to connect with
************************************************************************/
void CBoostClientTCP::Create( const std::string & host, const std::string & port )
{
	// Create the io service and resolve the endpoint
	ResolveEndpoint( host, port );

	// Create a shared pointer socket to hold the accepted connection
	spSocket.reset( new CBoostSocketTCP( *spIOService ) );

	NGenFunc::PostDebugMsg( "Boost TCP Client: Created and ready to connect." );

}	// Create


/************************************************************************
*    desc:  Try to connect to server or peer
************************************************************************/
void CBoostClientTCP::Connect()
{
	// Try to connect
	boost::asio::async_connect( *spSocket,
								resolverIter,
								boost::bind(&CBoostClientTCP::HandleConnect,
								this,
								boost::asio::placeholders::error) );

	NGenFunc::PostDebugMsg( "Boost TCP Client: Async connect in progress" );

}	// Connect


/************************************************************************
*    desc:  Handle the connect
************************************************************************/
void CBoostClientTCP::HandleConnect( const boost::system::error_code & error )
{
	errorCode = error;

	if( !error )
	{
		NGenFunc::PostDebugMsg( "Boost TCP Client: Socket Connected." );
	}
	else
	{
		NGenFunc::PostDebugMsg( "Boost TCP Client: Connect Error.", error.message().c_str() );
		throw std::exception( "Connection Error! Was not able to connect to Server." );
	}

}	// HandleAccept


/************************************************************************
*    desc:  Check all the services
************************************************************************/
void CBoostClientTCP::CheckServices()
{
	if( !errorCode && !spSocket->IsError() )
	{
		// Check for any received data
		spSocket->Receive();

		// Send any queued data
		spSocket->SendQuedData();
	}

}	// CheckServices



