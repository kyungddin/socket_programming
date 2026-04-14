# UDP Socket Server
from socket import *
serverName = 'kyungddin'
serverPort = 12000

# UDP 소켓 생성
server_socket = socket(AF_INET, SOCK_DGRAM)

# 소켓을 포트에 바인딩
server_socket.bind((serverName, serverPort))
print('The server is ready to receive')

while True:
    # 클라이언트로부터 메시지 받기
    message, clientAddress = server_socket.recvfrom(2048)
    print('Received message from client:', message.decode())

    # 메시지를 대문자로 변환
    modifiedMessage = message.decode().upper()

    # 변환된 메시지를 클라이언트로 전송
    server_socket.sendto(modifiedMessage.encode(), clientAddress)

# 소켓 닫기 (이 코드는 실제로는 실행되지 않음, 서버는 계속 실행됨)
server_socket.close()