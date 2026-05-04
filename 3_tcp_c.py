import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest_ip = '127.0.0.1'
dest_port = 12345
s.connect((dest_ip, dest_port))

print(f"Connected to server at {dest_ip}:{dest_port}")  # NEW: connection confirmation

msg_count = 0  # NEW: count sent messages

msg = input("Message to send: ")
while not msg == 'quit':
    msg_count += 1  # NEW
    s.send(bytes(msg, 'utf-8'))

    data = s.recv(4096)
    # CHANGED: show message number alongside server response
    print(f"[MSG #{msg_count}] Server replied: {data.decode('utf-8')}")

    msg = input("Message to send: ")

s.close()
print(f"Session ended. Sent {msg_count} messages total.")  # NEW: summary on exit