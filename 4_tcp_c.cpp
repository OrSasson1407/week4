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

    const char* server_ip = "127.0.0.1"; // Server IP (localhost)
    int server_port = 5555;              // Server port

    // -----------------------------
    // Create TCP socket
    // -----------------------------
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // -----------------------------
    // Setup server address
    // -----------------------------
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);          // Port in network byte order
    server_addr.sin_addr.s_addr = inet_addr(server_ip); // Convert string IP to numeric

    // -----------------------------
    // Connect to server
    // -----------------------------
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Connect failed: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!\n";

    // -----------------------------
    // Message sending loop
    // -----------------------------
    string msg;
    cout << "Message to send: ";
    getline(cin, msg);

    while (msg != "quit") {
        // Send message to server
        send(sock, msg.c_str(), msg.length(), 0);

        // Receive echo from server
        char buffer[4096];
        int bytes_received = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate
            cout << "Server replied: " << buffer << endl;
        }

        cout << "Message to send: ";
        getline(cin, msg);
    }

    // -----------------------------
    // Clean up
    // -----------------------------
    closesocket(sock); // Close client socket
    WSACleanup();      // Cleanup Winsock

    return 0;
}
