#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main() {

    const char* ip_address = "127.0.0.1";
    const int port_no = 5555;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    // CHANGED: loop so user can send multiple messages
    string userInput;
    while (true) {
        cout << "Enter message (or 'quit' to exit): ";
        getline(cin, userInput);  // NEW: read full line from stdin

        if (userInput == "quit") break;  // NEW: exit condition

        int sent_bytes = sendto(sock, userInput.c_str(), userInput.length(), 0,
                                (struct sockaddr *) &sin, sizeof(sin));
        if (sent_bytes < 0) {
            perror("error writing to socket");
            continue;
        }

        struct sockaddr_in from;
        unsigned int from_len = sizeof(struct sockaddr_in);
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));  // NEW: clear buffer before each recv

        int bytes = recvfrom(sock, buffer, sizeof(buffer), 0,
                             (struct sockaddr *) &from, &from_len);
        if (bytes < 0) {
            perror("error reading from socket");
            continue;
        }

        cout << "Server echoed: " << buffer << endl;  // CHANGED: clearer label
    }

    close(sock);
    cout << "Socket closed. Goodbye!" << endl;  // NEW
    return 0;
}