#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    const int server_port = 8080;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
        cout << "error binding to socket: " << WSAGetLastError() << endl;
    }

    struct sockaddr_in from;
    int from_len = sizeof(struct sockaddr_in);
    char buffer[4096];
    
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);
    if (bytes == SOCKET_ERROR) {
        cout << "error reading from socket: " << WSAGetLastError() << endl;
    } else {
        buffer[bytes] = '\0';
        cout << "The client sent: " << buffer << endl;
    }

    int sent_bytes = sendto(sock, buffer, bytes, 0, (struct sockaddr *) &from, sizeof(from));
    if (sent_bytes == SOCKET_ERROR) {
        cout << "error writing to socket: " << WSAGetLastError() << endl;
    }

    // 2. Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}