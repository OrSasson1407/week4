#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock library

using namespace std;

int main() {
    // -----------------------------
    // Initialize Winsock
    // -----------------------------
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }

    int server_port = 5555; // Server port

    // -----------------------------
    // Create TCP socket
    // -----------------------------
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // -----------------------------
    // Bind socket to all interfaces
    // -----------------------------
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;          // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all network interfaces
    server_addr.sin_port = htons(server_port); // Port in network byte order

    if (bind(listen_sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    // -----------------------------
    // Listen for incoming connections
    // -----------------------------
    if (listen(listen_sock, 5) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port " << server_port << "...\n";

    // -----------------------------
    // Accept a client connection
    // -----------------------------
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    SOCKET client_sock = accept(listen_sock, (sockaddr*)&client_addr, &client_addr_size);
    if (client_sock == INVALID_SOCKET) {
        cerr << "Accept failed: " << WSAGetLastError() << endl;
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    cout << "Client connected!\n";

    // -----------------------------
    // Receive data from client
    // -----------------------------
    char buffer[4096];
    int bytes_received = recv(client_sock, buffer, sizeof(buffer)-1, 0); // -1 to leave space for null terminator
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        cout << "Received: " << buffer << endl;

        // Echo the same message back to the client
        send(client_sock, buffer, bytes_received, 0);
    }

    // -----------------------------
    // Clean up
    // -----------------------------
    closesocket(client_sock); // Close client socket
    closesocket(listen_sock); // Close listening socket
    WSACleanup();             // Cleanup Winsock

    return 0;
}
