#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")   // link Winsock library automatically

using namespace std;

int main() {

    const int server_port = 5555;

    // Windows requires initializing Winsock before any socket call
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
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
        cerr << "error binding to socket: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "[SERVER] UDP listening on port " << server_port << "..." << endl;

    struct sockaddr_in from;
    int from_len = sizeof(struct sockaddr_in);
    char buffer[4096];
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);
    if (bytes == SOCKET_ERROR) {
        cerr << "error reading from socket: " << WSAGetLastError() << endl;
    } else {
        buffer[bytes] = '\0';
        cout << "The client sent: " << buffer << endl;

        int sent_bytes = sendto(sock, buffer, bytes, 0, (struct sockaddr *) &from, sizeof(from));
        if (sent_bytes == SOCKET_ERROR) {
            cerr << "error writing to socket: " << WSAGetLastError() << endl;
        }
    }

    closesocket(sock);   // Windows uses closesocket() instead of close()
    WSACleanup();        // Windows requires cleanup at the end
    return 0;
}