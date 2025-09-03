#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link against Winsock library

#define PORT 8080

int main() {
     WSADATA wsaData;
     SOCKET serverSocket, clientSocket;
     struct sockaddr_in serverAddr, clientAddr;
    int clientsize= sizeof(clientAddr);
    char buffer[4096];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)!=0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // Bind socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen
    listen(serverSocket, SOMAXCONN);
    std::cout << "Listening on port " << PORT << "...\n";

    // Accept a client
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientsize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Receive request
    ZeroMemory(buffer, 4096);
    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived > 0) {
        std::cout << "Received:\n" << buffer << "\n";

        // Send response
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n"
            "<html><body><h1>Hello from C++ on Windows!</h1></body></html>";

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
