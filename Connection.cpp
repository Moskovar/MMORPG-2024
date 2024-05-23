#include "Connection.h"

Connection::Connection()//gérer les erreurs avec des exceptions
{
    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        tcpSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (tcpSocket == INVALID_SOCKET) {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(1);
        }

        iResult = connect(tcpSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(tcpSocket);
            tcpSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
    }


    //--- UDP SOCKET ---//
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la création du socket UDP." << std::endl;
        WSACleanup();
        exit(1);
    }

    // Configuration de l'adresse et du port du serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port du serveur
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) { // Adresse IP du serveur
        std::cerr << "Erreur lors de la conversion de l'adresse IP." << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        exit(1);
    }

    // Lier la socket à l'adresse du serveur
    if (bind(udpSocket, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    u_long mode = 1;
    ioctlsocket(udpSocket, FIONBIO, &mode);
}

Connection::~Connection()
{
    close();
}

void Connection::sendTCP(string data)
{
    int iResult = ::send(tcpSocket, data.c_str(), (int)strlen(data.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        //closesocket(tcpSocket);
        //WSACleanup();
    }
    //std::cout << "Bytes Sent: " << iResult << std::endl;
}

void Connection::sendTCP(short data)
{
    data = htons(data);
    int iResult = ::send(tcpSocket, (const char*)&data, sizeof(data), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        //closesocket(tcpSocket);
        //WSACleanup();
    }
    //std::cout << "Bytes Sent: " << iResult << std::endl;
}

void Connection::sendNEUDP(uti::NetworkEntity& ne)
{
    ne.id = htonl(ne.id);
    ne.x = htonl(ne.x);
    ne.y = htonl(ne.y);
    char buffer[sizeof(ne)];
    memcpy(buffer, &ne, sizeof(ne));

    // Envoi des données sérialisées
    int bytesSent = sendto(udpSocket, buffer, sizeof(ne), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Erreur lors de l'envoi des données." << std::endl;
    }
}

void Connection::recvNETCP(uti::NetworkEntity& ne)
{
    uti::NetworkEntity ne2;
    recv(tcpSocket, reinterpret_cast<char*>(&ne2), sizeof(ne2), 0);

    ne.id = ntohl(ne2.id);
    ne.x  = ntohl(ne2.x);
    ne.y  = ntohl(ne2.y);
}

void Connection::close()
{
    closesocket(tcpSocket);
    closesocket(udpSocket);
    WSACleanup();
    cout << "Connection closed !" << endl;
}

void Connection::recvNEUDP(uti::NetworkEntity& ne)
{
    sockaddr_in serverAddr;
    int serverAddrLen = sizeof(serverAddr);

    uti::NetworkEntity ne2;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(udpSocket, &readfds);

    // Configurer le timeout
    timeval timeout;
    timeout.tv_sec = 1; // Attendez 1 seconde
    timeout.tv_usec = 0;

    // Appeler select
    int result = select(0, &readfds, NULL, NULL, &timeout);
    if (result == SOCKET_ERROR) std::cerr << "Erreur lors de l'appel de select." << std::endl;
    else if (result == 0) std::cout << "Delai d'attente ecoule." << std::endl;//no data available
    else {
        // Des données sont disponibles sur le socket
        if (FD_ISSET(udpSocket, &readfds)) {
            int bytesReceived = recvfrom(udpSocket, (char*)&ne2, sizeof(ne2), 0, (sockaddr*)&serverAddr, &serverAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Erreur lors de la reception des donnees: " << WSAGetLastError() << std::endl;
            }
        }
    }

    ne.id = htonl(ne2.id);
    ne.x  = htonl(ne2.x);
    ne.y  = htonl(ne2.y);

    std::cout << "ID: " << ne.id << ", X: " << ne.x / 100 << ", Y: " << ne.y / 100<< std::endl;
}
