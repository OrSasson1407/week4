#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    const int server_port = 5555;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
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
        cerr << "error binding socket: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (listen(sock, 5) == SOCKET_ERROR) {
        cerr << "error listening: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "[SERVER] TCP listening on port " << server_port << "..." << endl;

    struct sockaddr_in client_sin;
    int addr_len = sizeof(client_sin);
    SOCKET client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
    if (client_sock == INVALID_SOCKET) {
        cerr << "error accepting client: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        cout << "Connection closed by client." << endl;
    } else if (read_bytes == SOCKET_ERROR) {
        cerr << "error receiving: " << WSAGetLastError() << endl;
    } else {
        buffer[read_bytes] = '\0';
        cout << buffer << endl;

        int sent_bytes = send(client_sock, buffer, read_bytes, 0);
        if (sent_bytes == SOCKET_ERROR) {
            cerr << "error sending to client: " << WSAGetLastError() << endl;
        }
    }

    closesocket(client_sock);
    closesocket(sock);
    WSACleanup();
    return 0;
}