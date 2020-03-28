
/************************************************************************
*    FILE NAME:       boosttcp.h
*
*    DESCRIPTION:     Boost TCP
************************************************************************/

#ifndef __boost_tcp_h__
#define __boost_tcp_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

// Game lib dependencies
#include "common\\defs.h"

// Forward declaration(s)


class CBoostTCP
{
public:

    // Constructor
    CBoostTCP();

	// Destructor
	virtual ~CBoostTCP();

	// Create the io service and socket
	boost::asio::ip::tcp::endpoint ResolveEndpoint(
		const std::string & host, const std::string & port );

	// Poll the IO Service
	int Poll();

	// Run the IO Service
	int Run();

	// Stop the IO Service
	void Stop();

protected:

	// io service shared pointer
	boost::shared_ptr< boost::asio::io_service > spIOService;

	// Resolver iterator
	boost::asio::ip::tcp::resolver::iterator resolverIter;

};

#endif  // __boost_tcp_h__


