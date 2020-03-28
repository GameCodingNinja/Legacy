
/************************************************************************
*    FILE NAME:       boostsockettcp.h
*
*    DESCRIPTION:     Boost TCP Socket
************************************************************************/

#ifndef __boost_socket_tcp_h__
#define __boost_socket_tcp_h__

// Standard lib dependencies
#include <vector>
#include <deque>
#include <array>

// Boost lib dependencies
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/utility.hpp>

// Game lib dependencies
#include "common\\defs.h"

class CBoostSocketTCP : public boost::asio::ip::tcp::socket
{
public:

    // Constructor
    CBoostSocketTCP( boost::asio::io_service & ios,
		             const uint _id = 0,
					 const uint recvBufSize = 512,
					 const uint recvPendBufSize = 1024 );

	// Destructor
	virtual ~CBoostSocketTCP();

	// Start a recieve
	void Receive();

	// Get the recieved data
	std::vector<char> GetRecvData();

	// Set/Get socket ID
	void SetID( uint _id ){ id = _id; }
	uint GetID(){ return id; }

	// Is receive data available
	bool IsRecvDataAvailable(){ return (recvDeq.size() > 0); }

	// Send data
	void SendData( const char * pData, const uint dataSize );

	// Send any queued data
	void SendQuedData();

	// Status functions
	bool IsSendBusy(){ return sendBusy; }
	bool IsRecvBusy(){ return recvBusy; }
	bool IsError(){ return (errorCode.value() > 0); }

	// Get the dequed message count
	uint GetQuedMsgCount(){ return sendBufVecDeq.size(); }

private:

	// Handle the receive
	void HandleReceive( const boost::system::error_code & error,
						std::size_t bytes_transferred );

	// Confirms the send. This function only returns when the 
	// data has been sent
	void SendConfirm( const boost::system::error_code & error,
					  std::size_t bytes_transferred );

	// Copy the data to the vector buffer
	void CopyDataToVec( const char * pData,
						std::vector<char> & bufVec,
						const uint dataSize );

private:

	// Shared pointer TCP socket
	//boost::shared_ptr< boost::asio::ip::tcp::socket > spSocket;

	// Receive buffer vector
	std::vector<char> recvBufVec;

	// Pending buffer vector
	std::vector<char> recvPendBufVec;

	// deque of received data
	std::deque< std::vector<char> > recvDeq;

	// Vector to send transmitted data
	std::vector<char> sendBufVec;

	// deque of data to send
	std::deque< std::vector<char> > sendBufVecDeq;

	// Socket ID
	uint id;

	// Counter for the pending buffer
	uint pendBufCounter;

	// pending buffer size
	uint pendBufSize;

	// Send flags
	bool sendBusy;

	// Receive flags
	bool recvBusy;

	// max data block
	const uint MAX_DATA_BLOCK;

	// header/tail ID
	const uint DATA_HEADER_ID;
	const uint DATA_TAIL_ID;

	// Number of bytes used to describe each of the data IDs
	const uint DATA_ID_SIZE;

	// Max number of data headers: header ID, data size ID 
	const uint MAX_DATA_HEADERS;

	// Max number of data IDs, header ID, data size, tail ID
	const uint MAX_DATA_IDS;

	// Max data header size
	const uint MAX_DATA_HEADER_SIZE;

	// Boost error code
	boost::system::error_code errorCode;

};

#endif  // __boost_socket_tcp_h__


