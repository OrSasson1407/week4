import socket

# Create a TCP socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Listen on all available interfaces
server_ip = ''

# CHANGE: Updated the listening port from 12345 to 8080
server_port = 8080 

# Bind the socket and start listening for up to 5 concurrent pending connections
server.bind((server_ip, server_port))
server.listen(5)

# Infinite loop to keep the server running
while True:
    # Accept a new client connection (this blocks until a client connects)
    # Returns a new dedicated socket for this client and their address
    client_socket, client_address = server.accept()
    print('Connection from: ', client_address)
    
    # Receive the first message from the client (up to 1024 bytes)
    data = client_socket.recv(1024)
    
    # Loop to handle continuous messages from the same client
    # Breaks when data is empty (meaning the client disconnected)
    while not data.decode('utf-8') == '':
        print('Received: ', data.decode('utf-8'))
        
        # CHANGE: Convert data to uppercase AND append " [ACK]" before sending
        response = data.upper() + b" [ACK]"
        client_socket.send(response)
        
        # Wait for the next message from this client
        data = client_socket.recv(1024)

    # Print status when the inner loop breaks (client disconnected)
    print('Client disconnected')
    
    # Close the dedicated client socket
    client_socket.close()