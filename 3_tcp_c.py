import socket

# Create a TCP socket (SOCK_STREAM) using IPv4 (AF_INET)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

dest_ip = '127.0.0.1'

# CHANGE: Updated the destination port from 12345 to 8080
dest_port = 8080 

# Establish a reliable TCP connection with the server
s.connect((dest_ip, dest_port))

# Prompt the user for a message
msg = input("Message to send: ")

# Keep the connection open and loop until the user types 'quit'
while not msg == 'quit':
    # Send the user's message encoded as utf-8 bytes
    s.send(bytes(msg, 'utf-8'))
    
    # Wait for the server's response (up to 4096 bytes)
    data = s.recv(4096)
    
    # Decode and print the server's response
    print("Server sent: ", data.decode('utf-8'))
    
    # Ask for the next message
    msg = input("Message to send: ")

# Close the TCP connection
s.close()