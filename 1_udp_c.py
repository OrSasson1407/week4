# Import necessary socket functions and constants
from socket import socket, AF_INET, SOCK_DGRAM

# Create a UDP socket (SOCK_DGRAM) using the IPv4 protocol (AF_INET)
s = socket(AF_INET, SOCK_DGRAM)

# Define the destination IP (localhost)
dst_ip = '127.0.0.1'

# CHANGE: Updated the destination port from 12345 to 8080
dst_port = 8080 

# Send the byte string 'Hello' to the server without establishing a persistent connection
s.sendto(b'Hello', (dst_ip, dst_port))

# Wait to receive a response (up to 2048 bytes) and store the sender's info (IP and Port)
data, sender_info = s.recvfrom(2048)

# Decode the received bytes into a string using UTF-8 and print the message
print(data.decode('utf-8'))

# Print the server's IP and Port information
print(sender_info)

# Close the socket to free up system resources
s.close()