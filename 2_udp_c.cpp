#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

// Tell MSVC to link the Winsock library (Ignored by MinGW, but good practice)
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {
    // 1. Initialize Winsock (REQUIRED FOR WINDOWS)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    const char* ip_address = "127.0.0.1";
    const int port_no = 8080; 

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    char data[] = "hello";
    int data_len = sizeof(data);

    // Send data
    int sent_bytes = sendto(sock, data, data_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    if (sent_bytes == SOCKET_ERROR) {
        cout << "error writing to socket: " << WSAGetLastError() << endl;
    }

    struct sockaddr_in from;
    int from_len = sizeof(struct sockaddr_in); // Must be int on Windows, not unsigned int
    char buffer[4096];
    
    // Receive data
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);
    if (bytes == SOCKET_ERROR) {
        cout << "error reading from socket: " << WSAGetLastError() << endl;
    } else {
        buffer[bytes] = '\0'; // Null terminate to print safely
        cout << "The server sent: " << buffer << endl;
    }

    // 2. Cleanup (REQUIRED FOR WINDOWS)
    closesocket(sock); // Use closesocket() instead of close()
    WSACleanup();

    return 0;
}