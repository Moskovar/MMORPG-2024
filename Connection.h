#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include "uti.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class Connection
{
	public:
		Connection();
		~Connection();

		//bool isTCPSocketValid() { return tcpSocket != INVALID_SOCKET; }

		void sendNETCP(string data);
		void sendNETCP(uti::NetworkEntity ne);
		void sendNEUDP(uti::NetworkEntity& ne);
		bool recvNETCP(uti::NetworkEntity& ne);
		void recvNEUDP(uti::NetworkEntity& ne);

	private:
		WSADATA wsaData;

		SOCKET tcpSocket = INVALID_SOCKET;
		SOCKET udpSocket = INVALID_SOCKET;
		sockaddr_in tcpServerAddr, udpServerAddr;

};

