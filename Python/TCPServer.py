# TCP Socket Server
from socket import *
serverName = 'kyungddin'
serverPort = 12000

# TCP 소켓 생성 (환영소켓)
server_socket = socket(AF_INET, SOCK_STREAM)

# 소켓을 포트에 바인딩 (Server Action)
# 환영소켓을 해당 IP:PORT에 등록
server_socket.bind((serverName, serverPort))

# 클라이언트의 연결 요청을 기다림
# 1은 연결 요청 개수
server_socket.listen(1)
print('The server is ready to receive')

while True:
    # 클라이언트의 연결 요청 수락
    """
    클라 connect() 호출
        ↓
    SYN →
        ← SYN-ACK
    ACK →
            ↓
    서버의 accept()가 리턴됨
    """
    # 3 Way 발생!
    # 블로킹 부분
    connectionSocket, addr = server_socket.accept() 
    print('Connected by', addr)

    # 클라이언트로부터 메시지 받기
    # 블로킹 부분
    message = connectionSocket.recv(1024).decode()
    print('Received message from client:', message)

    # 메시지를 대문자로 변환
    modifiedMessage = message.upper()

    # 변환된 메시지를 클라이언트로 전송
    connectionSocket.send(modifiedMessage.encode())

    # 연결 소켓 닫기
    connectionSocket.close()
