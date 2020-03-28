// clientTestAp.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <iostream>
#include <string>

#include "networking\\boostclienttcp.h"
#include "utilities\\genfunc.h"

// Required namespace(s)
using namespace std;


int main(int argc, char **argv)
{
	CBoostClientTCP boostClientTCP;

	try
	{
		boostClientTCP.Create( "localhost", "80" );
		boostClientTCP.Connect();

		while(1)
		{
			boostClientTCP.CheckServices();
			boostClientTCP.Poll();
		}
	}
	catch( std::exception & ex )
	{
		NGenFunc::PostMsg( "Boost TCP Client: Error.", ex.what() );
	}

	system("PAUSE");

	return 0;
}




/*
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "networking\\socketclient.h"

using namespace std;

int main( int argc, char * argv[] )
{
	CSocketClient socketClient;

	cout << "Attemping to create socket" << endl;
	if( socketClient.Create( inet_addr("127.0.0.1"), 80 ) )
	{
		cout << "Attemping to connect to server" << endl;

		while( !socketClient.Connect() )
		{}
			
		cout << "Connected to server" << endl;
	}

	system ("PAUSE");

	return 0;
}
*/
