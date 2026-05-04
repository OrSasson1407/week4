from socket import socket, AF_INET, SOCK_DGRAM

s = socket(AF_INET, SOCK_DGRAM)

dst_ip = '127.0.0.1'
dst_port = 12345

# CHANGED: instead of sending one hardcoded message, send multiple user messages
while True:
    msg = input("Enter message (or 'quit' to exit): ")  # NEW: interactive input
    if msg == 'quit':
        break  # NEW: exit loop gracefully

    s.sendto(bytes(msg, 'utf-8'), (dst_ip, dst_port))  # CHANGED: encode input to bytes

    data, sender_info = s.recvfrom(2048)
    print(f"Server replied: {data.decode('utf-8')}")  # NEW: cleaner output label

s.close()
print("Connection closed.")  # NEW: confirmation on exit