#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
using namespace std;

enum ePort {SERVER_PORT = 54000};

int main()
{
    // 1. WinSock INIT
    WSADATA wsaData;
    int initResult = WSAStartup(MAKEWORD(2,2), &wsaData); // WinSock 2.2 요청
    if (initResult != 0)
    {
        cerr << "Can't Init WinSock!" << endl;
        return -1;
    }

    // 2. Create Socket
    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket == INVALID_SOCKET)
    {
        cerr << "Can't Create Socket!" << endl;
        WSACleanup();
        return -1;
    }

    // 3. Bind IP and Port
    // OS에 어떤 소켓이 특정 주소와 전송 계층 포트를 쓰겠다고 알려주는 절차
    // 
    sockaddr_in hint {};
    hint.sin_family = AF_INET;
    hint.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 이 컴퓨터의 모든 NIC(ex: Ethernet, Wi-Fi)로 들어오는 모든 데이터를 수신하겠다
    hint.sin_port = htons(SERVER_PORT); // Client가 사용할 포트 번호

    int bindResult = bind(ServerSocket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
    if(bindResult == SOCKET_ERROR)
    {
        cerr << "Can't Bind Socket!"<< endl;
        closesocket(ServerSocket);
        WSACleanup();
        return -1;
    }

    // 4. Listening Connect
    int listenResult = listen(ServerSocket, SOMAXCONN); // Accept 되기 전에 쌓아둘 TCP 연결 요청을 최대로 허용
    if (listenResult == SOCKET_ERROR)
    {
        cerr << "Can't Listen Socket!" << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return -1;
    }

    cout << "Server is listening on port " << SERVER_PORT << endl;

    while (true)  // Server Never die
    {
        sockaddr_in clientSockInfo;
        int clientSize = sizeof(clientSockInfo);

        SOCKET ClientSocket = accept(ServerSocket,
            reinterpret_cast<sockaddr*>(&clientSockInfo),
            &clientSize);

        if (ClientSocket == INVALID_SOCKET)
        {
            cerr << "Accept failed!" << endl;
            continue;   // 서버는 죽지 않음
        }

        cout << "Client connected!" << endl;

        // 🔽 클라이언트 처리 루프
        enum eBufSize { BUF_SIZE = 4096 };
        char buf[BUF_SIZE];

        while (true)
        {
            ZeroMemory(buf, BUF_SIZE);

            int bytesReceived = recv(ClientSocket, buf, BUF_SIZE, 0);

            if (bytesReceived == SOCKET_ERROR)
            {
                cerr << "recv error. Closing client." << endl;
                break;
            }

            if (bytesReceived == 0)
            {
                cout << "Client disconnected." << endl;
                break;
            }

            cout << "CLIENT> " << buf << endl;

            send(ClientSocket, buf, bytesReceived, 0);
        }

        // Only Close Client Socket
        closesocket(ClientSocket);
        cout << "Ready for next client..." << endl;
    }
}