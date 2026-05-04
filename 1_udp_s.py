from socket import socket, AF_INET, SOCK_DGRAM
from datetime import datetime  # NEW: to timestamp each message

s = socket(AF_INET, SOCK_DGRAM)

src_ip = ''
src_port = 12345
s.bind((src_ip, src_port))

message_count = 0  # NEW: track how many messages received

print(f"[SERVER] Listening on port {src_port}...")  # NEW: startup message

while True:
    data, sender_info = s.recvfrom(2048)
    message_count += 1  # NEW: increment counter

    # NEW: print with timestamp and message number
    timestamp = datetime.now().strftime("%H:%M:%S")
    print(f"[{timestamp}] Message #{message_count} from {sender_info}: {data.decode('utf-8')}")

    # CHANGED: instead of just uppercasing, prepend "ECHO: " to the response
    response = b"ECHO: " + data.upper()
    s.sendto(response, sender_info)