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

void Connection::sendNETCP(uti::NetworkEntity ne)
{
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Invalid TCP socket." << std::endl;
        return;
    }
    ne.header    = htons(ne.header);
    ne.id        = htons(ne.id);
    ne.countDir  = htons(ne.countDir);
    ne.hp        = htons(ne.hp);
    ne.xMap      = htonl(ne.xMap);
    ne.yMap      = htonl(ne.yMap);
    ne.timestamp = htonll(ne.timestamp);
    int iResult = ::send(tcpSocket, (const char*)&ne, sizeof(ne), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
    }
    //std::cout << "Bytes Sent: " << iResult << std::endl;
}

void Connection::sendNESTCP(uti::NetworkEntitySpell nes)
{
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Invalid TCP socket." << std::endl;
        return;
    }
    nes.header = htons(nes.header);
    nes.id     = htons(nes.id);
    nes.spellID = htons(nes.spellID);
    int iResult = ::send(tcpSocket, (const char*)&nes, sizeof(nes), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
    }
    //std::cout << "Bytes Sent: " << iResult << std::endl;
}

void Connection::sendNESETCP(uti::NetworkEntitySpellEffect nese)
{
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Invalid TCP socket." << std::endl;
        return;
    }
    nese.header  = htons(nese.header);
    nese.id      = htons(nese.id);
    nese.spellID = htons(nese.spellID);

    int iResult = ::send(tcpSocket, (const char*)&nese, sizeof(nese), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
    }
}

void Connection::sendNEUDP(uti::NetworkEntity& ne)
{
    ne.id = htons(ne.id);
    ne.xMap = htonl(ne.xMap);
    ne.yMap = htonl(ne.yMap);

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

bool Connection::recvTCP(uti::NetworkEntity& ne, uti::NetworkEntitySpell& nes, uti::NetworkEntityFaction& nef, SDL_bool& run)
{
    int bytesReceived = 0;
    int totalReceived = 0;
    char buffer[512];

    short header = 0;

    // Réception du header
    while (totalReceived < sizeof(header)) {
        bytesReceived = recv(tcpSocket, ((char*)&header) + totalReceived, sizeof(header) - totalReceived, 0);
        if (bytesReceived <= 0) {
            int wsaError = WSAGetLastError();
            if (wsaError == 10035) { // socket en mode non bloquant n'a rien reçu
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                if (!run) break;
                continue;
            }
            std::cerr << "Error receiving header: " << wsaError << std::endl;
            return false;
        }
        totalReceived += bytesReceived;
    }

    // Convertir le header
    header = ntohs(header);
    //std::cout << "Header: " << header << std::endl;

    // Copier le header dans le buffer
    std::memcpy(buffer, &header, sizeof(header));

    // Définir la taille des données en fonction du header
    unsigned long dataSize = 0;
    if      (header == 0) dataSize = sizeof(uti::NetworkEntity);
    else if (header == 1) dataSize = sizeof(uti::NetworkEntitySpell);
    else if (header == 3) dataSize = sizeof(uti::NetworkEntityFaction);

    // Réception des données restantes
    totalReceived = sizeof(header); // Réinitialiser totalReceived pour recevoir les données après le header
    while (totalReceived < dataSize) {
        bytesReceived = recv(tcpSocket, buffer + totalReceived, dataSize - totalReceived, 0);
        if (bytesReceived <= 0) {
            int wsaError = WSAGetLastError();
            if (wsaError == 10035) { // socket en mode non bloquant n'a rien reçu
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                if (!run) break;
                continue;
            }
            std::cerr << "Error receiving message: " << wsaError << std::endl;
            return false;
        }
        totalReceived += bytesReceived;
    }

    if (header == 0)
    {
        // Copier les données reçues (y compris le header) dans la structure NetworkEntity
        std::memcpy(&ne, buffer, dataSize);

        // Convertir les champs en endian correct si nécessaire
        // ne.header = header; // ne convertissez pas deux fois le header, il est déjà converti
        ne.id = ntohs(ne.id);
        ne.countDir = ntohs(ne.countDir);
        ne.hp = ntohs(ne.hp);
        ne.xMap = ntohl(ne.xMap);
        ne.yMap = ntohl(ne.yMap);
        ne.timestamp = ntohll(ne.timestamp);
        // ne.spell    = ntohs(ne.spell);

        std::cout << "POS" << " : " << ne.countDir << " : " << ne.hp << " : " << ne.xMap << " : " << ne.yMap << " : " << ne.timestamp << std::endl;
    }
    else if (header == 1)
    {
        std::memcpy(&nes, buffer, dataSize);
        //ne.header = header;
        nes.id      = ntohs(nes.id);
        nes.spellID = ntohs(nes.spellID);

        cout << "Spell" << " : " << nes.id << " : " << nes.spellID << endl;
    }
    else if (header == 3)
    {
        std::memcpy(&nef, buffer, dataSize);
        nef.id      = ntohs(nef.id);
        nef.faction = ntohs(nef.faction);
        //cout << "Received H: " << nef.header << " : " << nef.id << " : faction-> " << nef.faction << endl;
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
            ne.xMap = ntohl(ne2.xMap);
            ne.yMap = ntohl(ne2.yMap);

            float x = (float)ne.xMap / 100;
            float y = (float)ne.yMap / 100;

            std::cout << "ID: " << ne.id << ", X: " << x << ", Y: " << y << std::endl;
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Erreur lors de la reception des donnees: " << WSAGetLastError() << std::endl;
            }
        }
    }
}
