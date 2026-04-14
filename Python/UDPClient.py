# UDP Socket Client
from socket import *
serverName = 'kyungddin'
serverPort = 12000

# UDP 소켓 생성
client_socket = socket(AF_INET, SOCK_DGRAM)

# 사용자로부터 메시지 입력
message = input('Input lowercase sentence: ')

# 메시지를 서버로 전송
client_socket.sendto(message.encode(), (serverName, serverPort))

# 서버로부터 응답 받기
modifiedMessage, serverAddress = client_socket.recvfrom(2048)
print('From Server:', modifiedMessage.decode())

# 소켓 닫기
client_socket.close()