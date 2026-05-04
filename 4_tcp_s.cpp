#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main() {

    const int server_port = 5555;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    // NEW: allow port reuse so server can restart quickly
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
    }

    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
    }

    cout << "[SERVER] TCP server listening on port " << server_port << endl;  // NEW

    // CHANGED: loop to accept multiple clients one after another
    int client_count = 0;
    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);

        if (client_sock < 0) {
            perror("error accepting client");
            continue;
        }

        client_count++;
        // NEW: print the connecting client's IP
        cout << "[CLIENT #" << client_count << "] Connected from "
             << inet_ntoa(client_sin.sin_addr) << endl;

        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        int read_bytes = recv(client_sock, buffer, expected_data_len, 0);

        if (read_bytes == 0) {
            cout << "  Client disconnected immediately." << endl;  // NEW
        }
        else if (read_bytes < 0) {
            perror("error receiving from client");
        }
        else {
            buffer[read_bytes] = '\0';  // NEW: null-terminate
            cout << "  Received: " << buffer << endl;  // NEW: indented log

            int sent_bytes = send(client_sock, buffer, read_bytes, 0);
            if (sent_bytes < 0) {
                perror("error sending to client");
            } else {
                cout << "  Echoed " << sent_bytes << " bytes back." << endl;  // NEW
            }
        }

        close(client_sock);
        cout << "[CLIENT #" << client_count << "] Disconnected." << endl;  // NEW
    }

    close(sock);
    return 0;
}