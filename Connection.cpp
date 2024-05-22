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

    //socklen_t len = sizeof(serverAddr);
    //// Recevoir le message
    //int n = recvfrom(udpSocket, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&serverAddr, &len);
    //if (n < 0) {
    //    perror("recvfrom failed");
    //    exit(EXIT_FAILURE);
    //}
    //recvbuf[n] = '\0';

    //// Afficher le message reçu
    //std::cout << "Message from server: " << recvbuf << std::endl;
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

void Connection::sendStructUDP(uti::NetworkEntity& ne)
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

//void Connection::sendUDP(const char* sendbuf)
//{
//    // Envoi de données au serveur
//    int bytesSent = sendto(udpSocket, sendbuf, strlen(sendbuf), 0, (const sockaddr*)&serverAddr, sizeof(serverAddr));
//    if (bytesSent == SOCKET_ERROR) {
//        std::cerr << "Erreur lors de l'envoi des donnees au serveur." << std::endl;
//        closesocket(udpSocket);
//        WSACleanup();
//    }
//}

void Connection::recvNETCP(uti::NetworkEntity& ne)
{
    uti::NetworkEntity ne2;
    recv(tcpSocket, reinterpret_cast<char*>(&ne2), sizeof(ne2), 0);

    ne.id = ntohl(ne2.id);
    ne.x  = ntohs(ne2.x);
    ne.y  = ntohs(ne2.y);

    /*while ((iResult = recv(tcpSocket, recvbuf, recvbuflen - 1, 0)) > 0)
    {
        if (iResult > 0) {
            std::cout << "Bytes received: " << iResult << std::endl;
            std::cout << "Message: " << (data = string(recvbuf, iResult)) << std::endl;
        }
        else if (iResult == 0) close();
        else                   std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }*/
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
    uti::NetworkEntity ne2;
    int addrlen = sizeof(serverAddr);
    // Réception des données
    recvfrom(udpSocket, reinterpret_cast<char*>(&ne2), sizeof(ne2), 0, (struct sockaddr*)&serverAddr, &addrlen);
    ne.id = htonl(ne.id);
    ne.x  = htonl(ne.x);
    ne.y  = htonl(ne.y);
}
