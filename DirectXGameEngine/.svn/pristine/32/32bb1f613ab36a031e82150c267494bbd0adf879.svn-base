
/************************************************************************
*    FILE NAME:       boostsockettcp.cpp
*
*    DESCRIPTION:     Boost TCP Socket
************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

// Physical component dependency
#include "networking\\boostsockettcp.h"

// Standard lib dependencies
#include <string>

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
CBoostSocketTCP::CBoostSocketTCP( boost::asio::io_service & ios,
								  const uint _id,
								  const uint recvBufSize,
								  const uint recvPendBufSize )
			   : boost::asio::ip::tcp::socket(ios),
			     id(_id),
			     pendBufCounter(0),
			     pendBufSize(0),
				 sendBusy(false),
				 recvBusy(false),
				 MAX_DATA_BLOCK(4096),
				 DATA_HEADER_ID(0xbcde),
				 DATA_TAIL_ID(0xedcb),
				 DATA_ID_SIZE(2),
				 MAX_DATA_HEADERS(2),
				 MAX_DATA_IDS(3),
				 MAX_DATA_HEADER_SIZE( DATA_ID_SIZE * MAX_DATA_HEADERS )
{
	recvBufVec.resize( recvBufSize );
	recvPendBufVec.resize( recvPendBufSize );

}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CBoostSocketTCP::~CBoostSocketTCP()
{
}	// destructer


/************************************************************************
*    desc:  Start a recieve
************************************************************************/
void CBoostSocketTCP::Receive()
{
	if( !errorCode && !recvBusy )
	{
		recvBusy = true;

		async_receive( boost::asio::buffer(recvBufVec),
			boost::bind( &CBoostSocketTCP::HandleReceive,
				this, boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred ) );
	}

}	// Receive

/************************************************************************
*    desc:  Handle the receive
************************************************************************/
void CBoostSocketTCP::HandleReceive( const boost::system::error_code & error,
									 std::size_t bytes_transferred )
{
	recvBusy = false;
	errorCode = error;

	if( !error )
	{
		if( bytes_transferred > 0 )
		{
			// Copy the data over to the pending buffer
			std::copy( recvBufVec.begin(), recvBufVec.begin()+bytes_transferred, recvPendBufVec.begin()+pendBufCounter );

			// Inc the pending counter
			pendBufCounter += bytes_transferred;

			do
			{
				// get our data size from two bytes
				if( (pendBufSize == 0) && (pendBufCounter > MAX_DATA_HEADER_SIZE) )
				{
					uint headerId(0);

					// check the first two bytes for the correct header ID
					for(uint i = 0; i < DATA_ID_SIZE; i++)
						headerId = (headerId << 8) + static_cast<unsigned char>(recvPendBufVec[i]);

					// Check the next two bytes for the data size in bytes
					for(uint i = 0; i < DATA_ID_SIZE; i++)
						pendBufSize = (pendBufSize << 8) + static_cast<unsigned char>(recvPendBufVec[DATA_ID_SIZE + i]);

					// The data size should never be bigger then this
					if( (headerId != DATA_HEADER_ID) || (pendBufSize > MAX_DATA_BLOCK) )
						throw std::exception( "Header data out of sync!" );
				}

				if( (pendBufSize > 0) && (pendBufCounter >= pendBufSize) )
				{
					uint tailId(0);
					std::vector<char> queBufVec;
					queBufVec.resize( pendBufSize - MAX_DATA_HEADER_SIZE );

					// Check the next two bytes for the data size in bytes
					for(uint i = 0; i < DATA_ID_SIZE; i++)
						tailId = (tailId << 8) + static_cast<unsigned char>(recvPendBufVec[pendBufSize - DATA_ID_SIZE + i]);

					// The data size should never be bigger then this
					if( tailId != DATA_TAIL_ID )
						throw std::exception( "Tail data out of sync!" );

					// Copy the data into the queue vector buffer, jumping over the bytes used to save the data size. 
					std::copy( recvPendBufVec.begin() + MAX_DATA_HEADER_SIZE,
								recvPendBufVec.begin() + pendBufSize - DATA_ID_SIZE,
								queBufVec.begin() );

					// Push it onto the deque
					recvDeq.push_back( queBufVec );

					// Calculate the remaining amount of data
					uint remainder = pendBufCounter - pendBufSize;

					// copy it over
					if( remainder > 0 )
						std::copy( recvPendBufVec.begin()+pendBufSize, recvPendBufVec.begin()+pendBufCounter, recvPendBufVec.begin() );

					// reset the counters
					pendBufSize = 0;
					pendBufCounter = remainder;
				}
			}
			// Keep processing the pending buffer for more messages
			while( ((pendBufSize == 0) && (pendBufCounter > MAX_DATA_HEADER_SIZE )) ||
				   ((pendBufSize > 0) && (pendBufCounter >= pendBufSize)) );
		}
	}
	else
		throw std::exception( "Receive Error!" );

}	// HandleReceive


/************************************************************************
*    desc:  Get the recieved data
************************************************************************/
std::vector<char> CBoostSocketTCP::GetRecvData()
{
	std::vector<char> data;

	if( recvDeq.size() > 0 )
	{
		data = recvDeq.front();
		recvDeq.pop_front();
	}

	return data;

}	// GetRecvData


/************************************************************************
*    desc:  Send data
************************************************************************/
void CBoostSocketTCP::SendData( const char * pData, const uint dataSize )
{
	if( !errorCode )
	{
		// Are we clear for a send?
		if( !sendBusy && (sendBufVecDeq.size() == 0) )
		{
			// Copy the data to the supplied buffer
			CopyDataToVec( pData, sendBufVec, dataSize );
		}
		// If the send is busy, add the data to the queue
		// If there is data in the queue, add to the queue
		// and pop the front if send is not busy
		else if( sendBusy || (sendBufVecDeq.size() > 0) )
		{
			std::vector<char> data;

			// Copy the data to the supplied buffer
			CopyDataToVec( pData, data, dataSize );

			// Push the data into the buffer deque
			sendBufVecDeq.push_back( data );

			if( !sendBusy )
			{
				sendBufVec.clear();
				sendBufVec = sendBufVecDeq.front();
				sendBufVecDeq.pop_front();
			}
		}

		if( !sendBusy )
		{
			sendBusy = true;

			// This function only returns when the data has been sent or there has been an error
			boost::asio::async_write( *this, boost::asio::buffer(sendBufVec),
				boost::bind( &CBoostSocketTCP::SendConfirm,
					this, boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred ) );
		}
	}
	else
		throw std::exception( "Send Data Error!" );

}	// SendData


/************************************************************************
*    desc:  Copy the data to the vector buffer
************************************************************************/
void CBoostSocketTCP::CopyDataToVec( const char * pData,
									 std::vector<char> & bufVec,
									 const uint dataSize )
{
	// Calculate the finial size by adding in the size header
	uint finalDataSize = dataSize + (DATA_ID_SIZE * MAX_DATA_IDS);

	// Resize the vector to fit the data size
	bufVec.resize(finalDataSize);

	// Break the header down into two bytes
	bufVec[0] = (DATA_HEADER_ID >> 8);
	bufVec[1] = (DATA_HEADER_ID & 0xff);

	// Break the size down into two bytes
	bufVec[2] = (finalDataSize >> 8);
	bufVec[3] = (finalDataSize & 0xff);

	// Copy the data into the vector buffer. Skip past the size header
	std::copy( pData,
			   pData + dataSize,
			   bufVec.begin() + (DATA_ID_SIZE * MAX_DATA_HEADERS) );

	// Break the tail down into two bytes
	bufVec[finalDataSize-2] = (DATA_TAIL_ID >> 8);
	bufVec[finalDataSize-1] = (DATA_TAIL_ID & 0xff);

}	// CopyDataToVec


/************************************************************************
*    desc:  Confirms the send. This function only returns when the data
*           has been sent or there has been an error
************************************************************************/
void CBoostSocketTCP::SendConfirm( const boost::system::error_code & error,
								   std::size_t bytes_transferred )
{
	sendBusy = false;
	errorCode = error;

}	// SendConfirm


/************************************************************************
*    desc:  Send any queued data
************************************************************************/
void CBoostSocketTCP::SendQuedData()
{
	// See if there is any pending data to send
	if( !errorCode && !sendBusy && (sendBufVecDeq.size() > 0) )
	{
		sendBusy = true;

		sendBufVec = sendBufVecDeq.front();
		sendBufVecDeq.pop_front();

		// This function only returns when the data has been sent or there has been an error
		boost::asio::async_write( *this, boost::asio::buffer(sendBufVec),
			boost::bind( &CBoostSocketTCP::SendConfirm,
				this, boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred ) );
	}

}	// SendQuedData


