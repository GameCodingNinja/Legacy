// serverTestAp.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <iostream>
#include <string>

#include "networking\\boostservertcp.h"
#include "utilities\\genfunc.h"

// Required namespace(s)
using namespace std;


int main(int argc, char **argv)
{
	CBoostServerTCP boostServerTCP(2);

	try
	{
		boostServerTCP.Create( "localhost", "80" );

		boostServerTCP.ListenForAccept();

		while(1)
		{
			boostServerTCP.CheckServices();
			boostServerTCP.Poll();
		}
	}
	catch( std::exception & ex )
	{
		NGenFunc::PostMsg( "Boost TCP Server: Error.", ex.what() );
	}

	system("PAUSE");


	const int i = 67;

	return 0;
}




/*
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session
{
public:
  session(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
	  memset(data_, 0, sizeof(data_));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      boost::asio::async_write(socket_,
          boost::asio::buffer(data_, bytes_transferred),
          boost::bind(&session::handle_write, this,
            boost::asio::placeholders::error));
    }
    else
    {
      delete this;
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
          boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      delete this;
    }
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

class server
{
public:
  server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    session* new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(session* new_session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_session->start();
    }
    else
    {
      delete new_session;
    }

    start_accept();
  }

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    server s(io_service, 80);

	while(1)
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
*/







/*
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "networking\\socketserver.h"

using namespace std;

int main( int argc, char * argv[] )
{
	CSocketServer socketServer;

	cout << "Attemping to create socket" << endl;
	if( socketServer.Create( 80, true ) )
	{
		cout << "Server Socket created." << endl;

		cout << "Server listening on port 80." << endl;
		if( socketServer.Listen(5) )
		{
			while( socketServer.GetClientCount() < 1 )
			{
				if( socketServer.Accept() )
				{
					cout << "Connection Accepted" << endl;

					string msg = "Welcome to the Howie server";

					socketServer.GetLastClient().Send( msg.c_str(), msg.length() );
				}
			}
		}
	}

	system ("PAUSE");

	return 0;
}*/






 // Description:

//

//    This sample illustrates how to develop a simple echo server Winsock

//    application using the Overlapped I/O model with event notification. This

//    sample is implemented as a console-style application and simply prints

//    messages when connections are established and removed from the server.

//    The application listens for TCP connections on port 5150 and accepts them

//    as they arrive. When this application receives data from a client, it

//    simply echos (this is why we call it an echo server) the data back in

//    it's original form until the client closes the connection.

//

//    Note: There are no command line options for this sample.

// Link to ws2_32.lib
/*
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
 
#define PORT 80
#define DATA_BUFSIZE 1024
 
typedef struct _SOCKET_INFORMATION {
   CHAR Buffer[DATA_BUFSIZE];
   WSABUF DataBuf;
   SOCKET Socket;
   WSAOVERLAPPED Overlapped;
   DWORD BytesSEND;
   DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;


DWORD WINAPI ProcessIO(LPVOID lpParameter);
 

DWORD EventTotal = 0;
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
LPSOCKET_INFORMATION SocketArray[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION CriticalSection;
 
int main(int argc, char **argv)
{
   WSADATA wsaData;
   SOCKET ListenSocket, AcceptSocket;
   SOCKADDR_IN InternetAddr;
   DWORD Flags;
   DWORD ThreadId;
   DWORD RecvBytes;

   InitializeCriticalSection(&CriticalSection);


   if (WSAStartup((2,2),&wsaData) != 0)
   {
      printf("WSAStartup() failed with error %d\n", WSAGetLastError());
      WSACleanup();

      return 1;
   }
   else
     printf("WSAStartup() looks nice!\n");

   if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
   {
      printf("Failed to get a socket %d\n", WSAGetLastError());

      return 1;
   }
   else
      printf("WSASocket() is OK lol!\n");


   InternetAddr.sin_family = AF_INET;
   InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   InternetAddr.sin_port = htons(PORT);


   if (bind(ListenSocket, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
   {
      printf("bind() failed with error %d\n", WSAGetLastError());

      return 1;
   }
   else
      printf("YOu see, bind() is working!\n");

   if (listen(ListenSocket, 5))
   {
      printf("listen() failed with error %d\n", WSAGetLastError());

      return 1;
   }
   else
      printf("listen() is OK maa...\n");

 

   // Setup the listening socket for connections
   if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
   {
      printf("Failed to get a socket %d\n", WSAGetLastError());

      return 1;
   }
   else
      printf("WSASocket() looks OK!\n");

 
   if ((EventArray[0] = WSACreateEvent()) == WSA_INVALID_EVENT)
   {
      printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());

      return 1;
   }
   else
      printf("WSACreateEvent() is OK!\n");

 
   // Create a thread to service overlapped requests
   if (CreateThread(NULL, 0, ProcessIO, NULL, 0, &ThreadId) == NULL)
   {
      printf("CreateThread() failed with error %d\n", GetLastError());

      return 1;
   }
   else
      printf("Nothing to say, CreateThread() is OK!\n");


   EventTotal = 1;


   while(TRUE)
   {
       // Accept inbound connections
      if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) == INVALID_SOCKET)
      {
          printf("accept() failed with error %d\n", WSAGetLastError());

          return 1;
      }
      else
          printf("accept() is OK!\n");

      EnterCriticalSection(&CriticalSection);

      // Create a socket information structure to associate with the accepted socket
      if ((SocketArray[EventTotal] = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
      {
         printf("GlobalAlloc() failed with error %d\n", GetLastError());

         return 1;
      }
      else
         printf("GlobalAlloc() for LPSOCKET_INFORMATION is pretty fine!\n");

 
      // Fill in the details of our accepted socket
      SocketArray[EventTotal]->Socket = AcceptSocket;
      ZeroMemory(&(SocketArray[EventTotal]->Overlapped), sizeof(OVERLAPPED));
      SocketArray[EventTotal]->BytesSEND = 0;
      SocketArray[EventTotal]->BytesRECV = 0;
      SocketArray[EventTotal]->DataBuf.len = DATA_BUFSIZE;
      SocketArray[EventTotal]->DataBuf.buf = SocketArray[EventTotal]->Buffer;

      if ((SocketArray[EventTotal]->Overlapped.hEvent = EventArray[EventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT)
      {
         printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());

         return 1;
      }
      else
         printf("WSACreateEvent() is OK!\n");

 
      // Post a WSARecv() request to to begin receiving data on the socket
      Flags = 0;

      if (WSARecv(SocketArray[EventTotal]->Socket,
         &(SocketArray[EventTotal]->DataBuf), 1, &RecvBytes, &Flags, &(SocketArray[EventTotal]->Overlapped), NULL) == SOCKET_ERROR)
      {
         if (WSAGetLastError() != ERROR_IO_PENDING)
         {
            printf("WSARecv() failed with error %d\n", WSAGetLastError());

            return 1;
         }
      }
      else
            printf("WSARecv() should be working!\n");

 
      EventTotal++;

      LeaveCriticalSection(&CriticalSection);

 
      // Signal the first event in the event array to tell the worker thread to
      // service an additional event in the event array
      if (WSASetEvent(EventArray[0]) == FALSE)
      {
         printf("WSASetEvent() failed with error %d\n", WSAGetLastError());

         return 1;
      }
      else
         printf("Don't worry, WSASetEvent() is OK!\n");
   }
}

 

DWORD WINAPI ProcessIO(LPVOID lpParameter)

{
   DWORD Index;
   DWORD Flags;
   LPSOCKET_INFORMATION SI;
   DWORD BytesTransferred;
   DWORD i;
   DWORD RecvBytes, SendBytes;

 
   // Process asynchronous WSASend, WSARecv requests
   while(TRUE)
   {
      if ((Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE,  WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
      {
         printf("WSAWaitForMultipleEvents() failed %d\n", WSAGetLastError());

         return 0;
      }
      else
         printf("WSAWaitForMultipleEvents() is OK!\n");

 
      // If the event triggered was zero then a connection attempt was made
      // on our listening socket.
      if ((Index - WSA_WAIT_EVENT_0) == 0)
      {
         WSAResetEvent(EventArray[0]);

         continue;
      }

      SI = SocketArray[Index - WSA_WAIT_EVENT_0];
      WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

      if (WSAGetOverlappedResult(SI->Socket, &(SI->Overlapped), &BytesTransferred, FALSE, &Flags) == FALSE || BytesTransferred == 0)
      {
         printf("Closing socket %d\n", SI->Socket);

         if (closesocket(SI->Socket) == SOCKET_ERROR)
         {
            printf("closesocket() failed with error %d\n", WSAGetLastError());
         }
         else
            printf("closesocket() is OK!\n");

         GlobalFree(SI);

         WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

         // Cleanup SocketArray and EventArray by removing the socket event handle
         // and socket information structure if they are not at the end of the arrays
         EnterCriticalSection(&CriticalSection);

         if ((Index - WSA_WAIT_EVENT_0) + 1 != EventTotal)
		 {
            for (i = Index - WSA_WAIT_EVENT_0; i < EventTotal; i++)
            {
               EventArray[i] = EventArray[i + 1];
               SocketArray[i] = SocketArray[i + 1];
            }
		 }

         EventTotal--;

         LeaveCriticalSection(&CriticalSection);

         continue;
      }

      // Check to see if the BytesRECV field equals zero. If this is so, then
      // this means a WSARecv call just completed so update the BytesRECV field
      // with the BytesTransferred value from the completed WSARecv() call.
      if (SI->BytesRECV == 0)
      {
         SI->BytesRECV = BytesTransferred;
         SI->BytesSEND = 0;
      }
      else
      {
         SI->BytesSEND += BytesTransferred;
      }

 
      if (SI->BytesRECV > SI->BytesSEND)
      {
         // Post another WSASend() request.
         // Since WSASend() is not guaranteed to send all of the bytes requested,
         // continue posting WSASend() calls until all received bytes are sent

         ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
         SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];

         SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
         SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;

         if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0, &(SI->Overlapped), NULL) == SOCKET_ERROR)
         {
            if (WSAGetLastError() != ERROR_IO_PENDING)
            {
               printf("WSASend() failed with error %d\n", WSAGetLastError());

               return 0;
            }
         }
         else
              printf("WSASend() is OK!\n");
      }
      else
      {
         SI->BytesRECV = 0;

         // Now that there are no more bytes to send post another WSARecv() request
         Flags = 0;

         ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
         SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];

         SI->DataBuf.len = DATA_BUFSIZE;
         SI->DataBuf.buf = SI->Buffer;

         if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, &(SI->Overlapped), NULL) == SOCKET_ERROR)
         {
            if (WSAGetLastError() != ERROR_IO_PENDING)
            {
               printf("WSARecv() failed with error %d\n", WSAGetLastError());

               return 0;
            }
         }
         else
             printf("WSARecv() is OK!\n");
      }
   }
}
*/




/*
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

boost::mutex global_stream_lock;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
	global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
            << "] Thread started: " << std::endl;
    global_stream_lock.unlock();

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service->run( ec );
            if( ec )
            {
                    global_stream_lock.lock();
                    std::cout << "[" << boost::this_thread::get_id()
                            << "] Error: " << ec << std::endl;
                    global_stream_lock.unlock();
            }
            break;
        }
        catch( std::exception & ex )
        {
            global_stream_lock.lock();
            std::cout << "[" << boost::this_thread::get_id()
                    << "] Exception: " << ex.what() << std::endl;
            global_stream_lock.unlock();
        }
    }

	global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()
            << "] Thread ended: " << std::endl;
    global_stream_lock.unlock();
}

void Accept( vector< boost::shared_ptr< boost::asio::ip::tcp::socket > > & socketSPVec,
	         boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor,
			 boost::shared_ptr< boost::asio::io_service > io_service )
{
	while(true)
	{
		global_stream_lock.lock();
		cout << "Listening..." << endl;
		global_stream_lock.unlock();

		boost::shared_ptr< boost::asio::ip::tcp::socket > spSock(
				new boost::asio::ip::tcp::socket( *io_service ) );

		acceptor->accept( *spSock );

		if( acceptor->is_open() )
		{
			global_stream_lock.lock();
			cout << "Connection Accepted" << endl;
			global_stream_lock.unlock();

			std::string message = make_daytime_string();

			boost::system::error_code ignored_error;
			boost::asio::write( *spSock, boost::asio::buffer(message),
				boost::asio::transfer_all(), ignored_error );

			socketSPVec.push_back( spSock );
		}
	}
}

int main( int argc, char * argv[] )
{
	boost::shared_ptr< boost::asio::io_service > io_service(
                new boost::asio::io_service );

	boost::asio::ip::tcp::resolver resolver( *io_service );

	boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor(
            new boost::asio::ip::tcp::acceptor( *io_service ) );

	boost::shared_ptr< boost::asio::io_service::work > work(
                new boost::asio::io_service::work( *io_service ) );

	boost::thread_group worker_threads;

	try
	{
		boost::asio::ip::tcp::resolver::query query( 
							"127.0.0.1",
							boost::lexical_cast< std::string >( 80 ) );

		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );

		acceptor->open( endpoint.protocol() );
		acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
		acceptor->bind( endpoint );
		acceptor->listen( boost::asio::socket_base::max_connections );

		vector< boost::shared_ptr< boost::asio::ip::tcp::socket > > socketSPVec;

        for( int x = 0; x < 2; ++x )
        {
                worker_threads.create_thread( 
                        boost::bind( &WorkerThread, io_service )
                );
        }

		io_service->post( boost::bind( &Accept, socketSPVec, acceptor, io_service ) );

		std::cin.get();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;

		std::cin.get();
	}

	

	global_stream_lock.lock();
    std::cout << "Attempting to stop threads." << std::endl;
    global_stream_lock.unlock();

	io_service->stop();
	boost::system::error_code ec;
	acceptor->cancel(ec);
	acceptor->close(ec);
	
	//work.reset();

	worker_threads.join_all();

	global_stream_lock.lock();
    std::cout << "Threads stopped." << std::endl;
    global_stream_lock.unlock();

	system ("PAUSE");

}*/


/*boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                << "] Thread Start" << std::endl;
        global_stream_lock.unlock();

        while( true )
        {
                try
                {
                        boost::system::error_code ec;
                        io_service->run( ec );
                        if( ec )
                        {
                                global_stream_lock.lock();
                                std::cout << "[" << boost::this_thread::get_id()
                                        << "] Error: " << ec << std::endl;
                                global_stream_lock.unlock();
                        }
                        break;
                }
                catch( std::exception & ex )
                {
                        global_stream_lock.lock();
                        std::cout << "[" << boost::this_thread::get_id()
                                << "] Exception: " << ex.what() << std::endl;
                        global_stream_lock.unlock();
                }
        }

        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                << "] Thread Finish" << std::endl;
        global_stream_lock.unlock();
}

void OnAccept( const boost::system::error_code & ec, boost::shared_ptr< boost::asio::ip::tcp::socket > sock )
{
        if( ec )
        {
                global_stream_lock.lock();
                std::cout << "[" << boost::this_thread::get_id()
                        << "] Error: " << ec << std::endl;
                global_stream_lock.unlock();
        }
        else
        {
                global_stream_lock.lock();
                std::cout << "[" << boost::this_thread::get_id()
                        << "] Accepted!" << std::endl;
                global_stream_lock.unlock();
        }
}

int main( int argc, char * argv[] )
{
        boost::shared_ptr< boost::asio::io_service > io_service(
                new boost::asio::io_service
                );
        boost::shared_ptr< boost::asio::io_service::work > work(
                new boost::asio::io_service::work( *io_service )
                );
        boost::shared_ptr< boost::asio::io_service::strand > strand(
                new boost::asio::io_service::strand( *io_service )
                );

        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()
                << "] Press [return] to exit." << std::endl;
        global_stream_lock.unlock();

        boost::thread_group worker_threads;
        for( int x = 0; x < 2; ++x )
        {
                worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
        }

        boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor(
                new boost::asio::ip::tcp::acceptor( *io_service )
                );
        boost::shared_ptr< boost::asio::ip::tcp::socket > sock(
                new boost::asio::ip::tcp::socket( *io_service )
                );

        try
        {
                boost::asio::ip::tcp::resolver resolver( *io_service );
                boost::asio::ip::tcp::resolver::query query( 
                        "127.0.0.1", 
                        boost::lexical_cast< std::string >( 80 )
                        );
                boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
                acceptor->open( endpoint.protocol() );
                acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
                acceptor->bind( endpoint );
                acceptor->listen( boost::asio::socket_base::max_connections );
                acceptor->async_accept( *sock, boost::bind( OnAccept, _1, sock ) );

                global_stream_lock.lock();
                std::cout << "Listening on: " << endpoint << std::endl;
                global_stream_lock.unlock();
        }
        catch( std::exception & ex )
        {
                global_stream_lock.lock();
                std::cout << "[" << boost::this_thread::get_id()
                        << "] Exception: " << ex.what() << std::endl;
                global_stream_lock.unlock();
        }

        std::cin.get();

        boost::system::error_code ec;
        acceptor->close( ec );

        sock->shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
        sock->close( ec );

        io_service->stop();

        worker_threads.join_all();

		system ("PAUSE");

        return 0;
}*/

/*
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query( 
							"127.0.0.1",
							boost::lexical_cast< std::string >( 80 ) );

    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  system ("PAUSE");
  return 0;
}*/