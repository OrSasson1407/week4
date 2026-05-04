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

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding to socket");
    }

    // NEW: print startup message
    cout << "[SERVER] Listening on port " << server_port << "..." << endl;

    // CHANGED: wrap in a loop so server handles multiple clients
    int message_count = 0;  // NEW: message counter
    while (true) {
        struct sockaddr_in from;
        unsigned int from_len = sizeof(struct sockaddr_in);
        char buffer[4096];

        int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);
        if (bytes < 0) {
            perror("error reading from socket");
            continue;  // NEW: don't crash, keep going
        }

        buffer[bytes] = '\0';  // NEW: null-terminate so string prints cleanly
        message_count++;       // NEW: count messages

        // NEW: print client IP address using inet_ntoa
        cout << "[MSG #" << message_count << "] From " 
             << inet_ntoa(from.sin_addr) << ": " << buffer << endl;

        int sent_bytes = sendto(sock, buffer, bytes, 0, (struct sockaddr *) &from, sizeof(from));
        if (sent_bytes < 0) {
            perror("error writing to socket");
        }
    }

    close(sock);
    return 0;
}