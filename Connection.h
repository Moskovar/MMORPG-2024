#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "uti.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

using namespace std;

class Connection
{
	public:
		Connection();
		~Connection();

		void sendTCP(string data);
		void sendStructUDP(uti::NetworkEntity& ne);
		void recvNETCP(uti::NetworkEntity& ne);
		void recvNEUDP(uti::NetworkEntity& ne);
		void close();

	private:
		WSADATA wsaData;

		SOCKET tcpSocket = INVALID_SOCKET;
		struct addrinfo* result = NULL, * ptr = NULL, hints;

		SOCKET udpSocket = INVALID_SOCKET;
		sockaddr_in serverAddr;
};

