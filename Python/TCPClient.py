# TCP Socket Client
from socket import *
serverName = 'kyungddin' # ip도 문자열로 처리..!
serverPort = 12000

# TCP 소켓 생성
client_socket = socket(AF_INET, SOCK_STREAM)

# 서버에 연결
# 1. servername이 이름이면 DNS 자동 수행
# 2. server:port로 TCP SYN 전송
# 3. 3 Way Handshake 수행
client_socket.connect((serverName, serverPort))

# 작업부분: 사용자로부터 메시지 입력
message = input('Enter a message to send to the server: ')

# 메시지를 서버로 전송
# 소켓은 바이트 스트림이므로 반드시 바이트로 인코딩 (기본 인코딩 UTF-8)
client_socket.send(message.encode())

# 서버로부터 변환된 메시지 받기
# 최대 1024 바이트 수신 (블로킹 함수: 데이터 올 때 까지 대기)
modifiedMessage = client_socket.recv(1024).decode()
print('Message from server:', modifiedMessage)

# 소켓 닫기
client_socket.close()
