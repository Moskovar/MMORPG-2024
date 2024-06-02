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

		void sendNETCP(uti::NetworkEntity ne);
		void sendNESTCP(uti::NetworkEntitySpell nes);
		bool recvTCP(uti::NetworkEntity& ne, uti::NetworkEntitySpell& nes, uti::NetworkEntityFaction& nef, SDL_bool& run);




		void sendNEUDP(uti::NetworkEntity& ne);
		void recvNEUDP(uti::NetworkEntity& ne);

	private:
		WSADATA wsaData;

		SOCKET tcpSocket = INVALID_SOCKET;
		SOCKET udpSocket = INVALID_SOCKET;
		sockaddr_in tcpServerAddr, udpServerAddr;

};

