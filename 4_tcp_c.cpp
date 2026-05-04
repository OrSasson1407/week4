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

    const char* ip_address = "127.0.0.1";
    const int port_no = 8080;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
        cout << "error connecting to server: " << WSAGetLastError() << endl;
    }

    char data_addr[] = "Im a message";
    int data_len = strlen(data_addr);
    
    int sent_bytes = send(sock, data_addr, data_len, 0);
    if (sent_bytes == SOCKET_ERROR) {
        cout << "error writing to socket" << endl;
    }

    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    
    int read_bytes = recv(sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        cout << "Connection closed by server." << endl;
    }
    else if (read_bytes == SOCKET_ERROR) {
        cout << "error reading from socket: " << WSAGetLastError() << endl;
    }
    else {
        buffer[read_bytes] = '\0';
        cout << buffer << endl;
    }

    // 2. Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}