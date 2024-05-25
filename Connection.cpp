#include "Connection.h"
#include <chrono>
#include <thread>

Connection::Connection()//gérer les erreurs avec des exceptions
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //--- TCP SOCKET ---//
    // Initialisation du socket TCP
    tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la création du socket TCP." << std::endl;
        WSACleanup();
        exit(1);
    }

    //--- Définition de l'adresse du serveur TCP ---//
    tcpServerAddr.sin_family = AF_INET;
    tcpServerAddr.sin_port = htons(9090);
    inet_pton(AF_INET, "127.0.0.1", &tcpServerAddr.sin_addr);

    //--- Connexion au serveur TCP ---//
    if (connect(tcpSocket, (sockaddr*)&tcpServerAddr, sizeof(tcpServerAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la connexion au serveur TCP." << std::endl;
        closesocket(tcpSocket);
        WSACleanup();
        exit(1);
    }

    cout << "Connected to distant TCP server" << endl;

    //--- UDP SOCKET ---//
    // Initialisation du socket UDP
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la création du socket UDP." << std::endl;
        closesocket(tcpSocket);
        WSACleanup();
        exit(1);
    }

    //--- Définition de l'adresse du serveur UDP ---//
    udpServerAddr.sin_family = AF_INET;
    udpServerAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &udpServerAddr.sin_addr);

    //--- Socket TCP et UDP mit en mode non bloquant ---//
    u_long modeTCP = 1;
    ioctlsocket(tcpSocket, FIONBIO, &modeTCP);

    u_long modeUDP = 1;
    ioctlsocket(udpSocket, FIONBIO, &modeUDP);
}

Connection::~Connection()
{
    closesocket(tcpSocket);
    closesocket(udpSocket);
    WSACleanup();
}

void Connection::sendNETCP(string data)
{
    int iResult = ::send(tcpSocket, data.c_str(), (int)strlen(data.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        //closesocket(tcpSocket);
        //WSACleanup();
    }
    //std::cout << "Bytes Sent: " << iResult << std::endl;
}

void Connection::sendNETCP(uti::NetworkEntity ne)
{
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Invalid TCP socket." << std::endl;
        return;
    }
    ne.id = htons(ne.id);
    ne.countDir = htons(ne.countDir);
    ne.x = htonl(ne.x);
    ne.y = htonl(ne.y);
    ne.timestamp = htonll(ne.timestamp);
    int iResult = ::send(tcpSocket, (const char*)&ne, sizeof(ne), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
    }
    //std::cout << "Bytes Sent: " << iResult << std::endl;
}

void Connection::sendNEUDP(uti::NetworkEntity& ne)
{
    ne.id = htons(ne.id);
    ne.x = htonl(ne.x);
    ne.y = htonl(ne.y);

    // Envoi des données sérialisées
    int bytesSent = sendto(udpSocket, (const char*)&ne, sizeof(ne), 0, (sockaddr*)&udpServerAddr, sizeof(udpServerAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Erreur lors de l'envoi des donnees -> " << WSAGetLastError() << std::endl;
    }
    else if (bytesSent != sizeof(ne)) 
    {
        std::cerr << "Erreur : seuls " << bytesSent << " octets sur " << sizeof(ne) << " ont ete envoyes." << std::endl;
    }
}

bool Connection::recvNETCP(uti::NetworkEntity& ne)
{
    fd_set readfds;//structure pour surveiller un ensemble de descripteurs de fichiers pour lire (ici les sockets)
    timeval timeout;
    int bytesReceived = 0;
    int totalReceived = 0;
    char buffer[512];

    while (totalReceived < sizeof(uti::NetworkEntity))
    {

        bytesReceived = recv(tcpSocket, buffer + totalReceived, sizeof(uti::NetworkEntity) - totalReceived, 0);
        if (bytesReceived <= 0)
        {
            int wsaError = WSAGetLastError();
            if (wsaError == 10035) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); return true; }
            cout << "Error receiving msg " << wsaError << endl;
            // Gestion des erreurs ou de la déconnexion
            return false;
        }

        totalReceived += bytesReceived;
        std::memcpy(&ne, buffer, sizeof(uti::NetworkEntity));

        ne.id       = ntohs(ne.id);
        ne.countDir = ntohs(ne.countDir);
        ne.x        = ntohl(ne.x);
        ne.y        = ntohl(ne.y);
        //cout << "Received: " << ne.id << " : " << ne.x << " : " << ne.y << endl;
    }

    return true;
}

void Connection::recvNEUDP(uti::NetworkEntity& ne)
{
    uti::NetworkEntity ne2;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(udpSocket, &readfds);
    int udpServerAddrLen = sizeof(udpServerAddr);

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
            int bytesReceived = recvfrom(udpSocket, (char*)&ne2, sizeof(ne2), 0, (sockaddr*)&udpServerAddr, &udpServerAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Erreur lors de la reception des donnees: " << WSAGetLastError() << std::endl;
            }
            else if (bytesReceived == 0) {
                std::cout << "Aucune donnee recue." << std::endl;
                return;
            }

            ne.id = ntohl(ne2.id);
            ne.x = ntohl(ne2.x);
            ne.y = ntohl(ne2.y);

            float x = (float)ne.x / 100;
            float y = (float)ne.y / 100;

            std::cout << "ID: " << ne.id << ", X: " << x << ", Y: " << y << std::endl;
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Erreur lors de la reception des donnees: " << WSAGetLastError() << std::endl;
            }
        }
    }
}
