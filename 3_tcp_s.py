import socket
from datetime import datetime  # NEW: for timestamps

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# NEW: allow reuse of port immediately after restart (avoids "Address already in use")
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_ip = ''
server_port = 12345
server.bind((server_ip, server_port))
server.listen(5)

print(f"[SERVER] TCP server listening on port {server_port}")  # NEW

client_count = 0  # NEW: track total clients served

while True:
    client_socket, client_address = server.accept()
    client_count += 1  # NEW
    print(f"[{datetime.now().strftime('%H:%M:%S')}] Client #{client_count} connected from {client_address}")  # NEW

    data = client_socket.recv(1024)
    while not data.decode('utf-8') == '':
        decoded = data.decode('utf-8')
        print(f'  Received: {decoded}')

        # CHANGED: response now includes message length info
        response = f"[{len(decoded)} chars] {decoded.upper()}"
        client_socket.send(bytes(response, 'utf-8'))

        data = client_socket.recv(1024)

    print(f'  Client #{client_count} disconnected')  # CHANGED: include client number
    client_socket.close()