#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    const char* ip_address = "127.0.0.1";
    const int port_no = 5555;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &sin.sin_addr);  // inet_pton works on both Windows and Linux
    sin.sin_port = htons(port_no);

    char data[] = "hello";
    int data_len = sizeof(data);

    int sent_bytes = sendto(sock, data, data_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    if (sent_bytes == SOCKET_ERROR) {
        cerr << "error writing to socket: " << WSAGetLastError() << endl;
    }

    struct sockaddr_in from;
    int from_len = sizeof(struct sockaddr_in);
    char buffer[4096];
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);
    if (bytes == SOCKET_ERROR) {
        cerr << "error reading from socket: " << WSAGetLastError() << endl;
    } else {
        buffer[bytes] = '\0';
        cout << "The server sent: " << buffer << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}