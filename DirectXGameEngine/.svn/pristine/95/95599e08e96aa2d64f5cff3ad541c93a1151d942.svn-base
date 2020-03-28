
/************************************************************************
*    FILE NAME:       boosttcp.cpp
*
*    DESCRIPTION:     Boost TCP
************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

// Physical component dependency
#include "networking\\boosttcp.h"

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
CBoostTCP::CBoostTCP()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CBoostTCP::~CBoostTCP()
{
}	// destructer


/************************************************************************
*    desc:  Create the io service and record the endpoint
************************************************************************/
boost::asio::ip::tcp::endpoint CBoostTCP::ResolveEndpoint(
	const std::string & host, const std::string & port )
{
	spIOService.reset( new boost::asio::io_service );

	// Create the resolver to resolve the port query
	boost::asio::ip::tcp::resolver resolver( *spIOService );
	boost::asio::ip::tcp::resolver::query query( host, port );

	resolverIter = resolver.resolve( query );

	// Resolve the TCP end point which describes this socket
	return *resolverIter;

}	// Create


/************************************************************************
*    desc:  Poll the IO Service
************************************************************************/
int CBoostTCP::Poll()
{
	if( spIOService )
		return spIOService->poll();

	return -1;
}


/************************************************************************
*    desc:  Run the IO Service
************************************************************************/
int CBoostTCP::Run()
{
	if( spIOService )
		return spIOService->run();

	return -1;
}


/************************************************************************
*    desc:  Stop the IO Service
************************************************************************/
void CBoostTCP::Stop()
{
	if( spIOService )
		spIOService->stop();
}





