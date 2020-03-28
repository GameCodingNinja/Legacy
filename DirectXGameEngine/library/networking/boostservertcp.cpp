
/************************************************************************
*    FILE NAME:       boostservertcp.cpp
*
*    DESCRIPTION:     Boost server TCP
************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

// Physical component dependency
#include "networking\\boostservertcp.h"

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

// Game lib dependencies
#include "utilities\\genfunc.h"

// Required namespace(s)
using namespace std;

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
//#pragma warning(disable : 4244)
// disable warning about the use of the "this" pointer in the constructor list
//#pragma warning(disable : 4355)


/************************************************************************
*    desc:  Constructer
************************************************************************/
CBoostServerTCP::CBoostServerTCP( uint maxConnections )
			   : uniqueId(0),
				 MAX_CONNECTIONS( maxConnections ),
				 acceptBusy(false)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CBoostServerTCP::~CBoostServerTCP()
{
}	// destructer


/************************************************************************
*    desc:  Create the io service and socket to listen with
************************************************************************/
void CBoostServerTCP::Create( const std::string & host, const std::string & port )
{
	// Create the io service and resolve the endpoint
	boost::asio::ip::tcp::endpoint endpoint = ResolveEndpoint( host, port );

	// Create the Acceptor to accept new connections
	spAcceptor.reset( new boost::asio::ip::tcp::acceptor( *spIOService ) );

	// Use the endpoint to bind to the socket and set it up to listen
	spAcceptor->open( endpoint.protocol() );
	spAcceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
	spAcceptor->bind( endpoint );
	spAcceptor->listen( boost::asio::socket_base::max_connections );

	NGenFunc::PostDebugMsg( "Boost TCP Server: Created and ready to listen." );

}	// Create


/************************************************************************
*    desc:  Start the listening for socket connections
************************************************************************/
void CBoostServerTCP::ListenForAccept()
{
	// Make sure we haven't exceeded our connection requirement
	if( !acceptBusy )
	{
		acceptBusy = true;

		// Create a shared pointer socket to hold the accepted connection
		boost::shared_ptr< CBoostSocketTCP > spSocket( new CBoostSocketTCP( *spIOService, uniqueId++ ) );

		// Prime the acceptor to use this socket to listen for incoming connections
		spAcceptor->async_accept( *spSocket, boost::bind(&CBoostServerTCP::HandleAccept, this, spSocket, boost::asio::placeholders::error) );

		NGenFunc::PostDebugMsg( "Boost TCP Server: Accept ready for IO Service Run/Poll" );
	}

}	// ListenForAccept


/************************************************************************
*    desc:  Handle the accept
************************************************************************/
void CBoostServerTCP::HandleAccept( boost::shared_ptr< CBoostSocketTCP > spSocket,
									const boost::system::error_code & error )
{
	errorCode = error;
	acceptBusy = false;

	if( !error )
	{
		// Make sure we haven't exceeded our connection requirement
		if( socketVec.size() < MAX_CONNECTIONS )
		{
			socketVec.push_back( spSocket );

			NGenFunc::PostDebugMsg( "Boost TCP Server: Socket Accepted." );
		}
		else
		{
			NGenFunc::PostDebugMsg( "Boost TCP Server: Connection Limit reached (%d).", socketVec.size() );

			spSocket->close();
		}
	}
	else
		NGenFunc::PostMsg( "Boost TCP Server: Accept Error.", error.message().c_str() );

}	// HandleAccept


/************************************************************************
*    desc:  Check all the services
************************************************************************/
void CBoostServerTCP::CheckServices()
{
	ListenForAccept();

	for( uint i = 0; i < socketVec.size(); i++ )
	{
		if( !socketVec[i]->IsError() )
		{
			// Check for any received data
			socketVec[i]->Receive();

			// Send any queued data
			socketVec[i]->SendQuedData();
		}
	}

}	// CheckServices



