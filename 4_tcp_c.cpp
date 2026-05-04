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

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &sin.sin_addr);
    sin.sin_port = htons(port_no);

    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
        cerr << "error connecting to server: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "Connected to " << ip_address << ":" << port_no << endl;

    char data_addr[] = "Im a message";
    int data_len = strlen(data_addr);
    int sent_bytes = send(sock, data_addr, data_len, 0);
    if (sent_bytes == SOCKET_ERROR) {
        cerr << "error sending: " << WSAGetLastError() << endl;
    }

    char buffer[4096];
    int expected_data_len = sizeof(buffer);
    int read_bytes = recv(sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        cout << "Connection closed by server." << endl;
    } else if (read_bytes == SOCKET_ERROR) {
        cerr << "error receiving: " << WSAGetLastError() << endl;
    } else {
        buffer[read_bytes] = '\0';
        cout << buffer << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}