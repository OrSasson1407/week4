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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
        return 1;  // NEW: exit with error code
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error connecting to server");
        return 1;  // NEW: exit on failed connect
    }

    cout << "Connected to " << ip_address << ":" << port_no << endl;  // NEW

    // CHANGED: loop for multiple messages instead of one hardcoded message
    string userInput;
    int msg_count = 0;

    while (true) {
        cout << "Enter message (or 'quit' to exit): ";
        getline(cin, userInput);  // NEW: read full line

        if (userInput == "quit") break;  // NEW: exit condition

        int sent_bytes = send(sock, userInput.c_str(), userInput.length(), 0);
        if (sent_bytes < 0) {
            perror("error sending");
            break;
        }
        msg_count++;  // NEW

        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));  // NEW: zero out buffer
        int read_bytes = recv(sock, buffer, sizeof(buffer), 0);

        if (read_bytes == 0) {
            cout << "Server closed connection." << endl;  // CHANGED: clearer message
            break;
        }
        else if (read_bytes < 0) {
            perror("error receiving");
            break;
        }
        else {
            cout << "[MSG #" << msg_count << "] Server replied: " << buffer << endl;  // NEW
        }
    }

    close(sock);
    cout << "Disconnected. Total messages sent: " << msg_count << endl;  // NEW
    return 0;
}