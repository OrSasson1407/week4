# Import necessary socket functions and constants
from socket import socket, AF_INET, SOCK_DGRAM

# Create a UDP socket (SOCK_DGRAM) using the IPv4 protocol (AF_INET)
s = socket(AF_INET, SOCK_DGRAM)

# Bind to all available network interfaces (empty string)
src_ip = ''

# CHANGE: Updated the listening port from 12345 to 8080
src_port = 8080

# Bind the socket to the IP and port so it can listen for incoming traffic
s.bind((src_ip, src_port))

# Start an infinite loop to keep the server running continuously
while True:
    # Receive data (up to 2048 bytes) and the client's address info
    data, sender_info = s.recvfrom(2048)
    
    # Print the decoded message from the client
    print(data.decode('utf-8'))
    
    # Print the client's IP and Port information
    print(sender_info)

    # CHANGE: Convert data to uppercase AND append " [ACK - Received]" before sending back
    response = data.upper() + b" [ACK - Received]"
    
    # Send the modified response back to the exact client that sent the message
    s.sendto(response, sender_info)