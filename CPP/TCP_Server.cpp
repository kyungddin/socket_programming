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

    // 5. Accept Connection
    sockaddr_in clientSockInfo;
    int clientSize = sizeof(clientSockInfo);

    SOCKET ClientSocket = accept(ServerSocket, reinterpret_cast<sockaddr*>(&clientSockInfo), &clientSize);
    if (ClientSocket == INVALID_SOCKET)
    {
        cerr << "Can't Accept Socket!" << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return -1;
    }

    // 6. Close Listening Socket
    int closeResult = closesocket(ServerSocket);

    //////////////////// 7. Work Function(일 함수) ////////////////////

    char host[NI_MAXHOST];	         // 클라이언트의 host 이름
    char service[NI_MAXHOST];        // 클라이언트의 PORT 번호
    ZeroMemory(host, NI_MAXHOST);    // memset(host, 0, NI_MAXHOST)와 동일
    ZeroMemory(service, NI_MAXHOST);

    // clientSockInfo에 저장된 IP 주소를 통해 도메인 정보를 얻습니다. host 이름은 host에, 포트 번호는 service에 저장됩니다.
    // getnameinfo()는 성공 시 0을 반환합니다. 실패 시 0이 아닌 값을 반환합니다.
    if (getnameinfo((sockaddr*)&clientSockInfo, sizeof(clientSockInfo), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected ON port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &clientSockInfo.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(clientSockInfo.sin_port) << endl;
    }


    // While loop: 클라이언트의 메세지를 받아서 출력 후 클라이언트에 다시 보냅니다.
    enum eBufSize { BUF_SIZE = 4096 };
    char buf[BUF_SIZE];

    while (true)
    {
        ZeroMemory(buf, BUF_SIZE);

        // Wait for client to send data
        // 메세지를 성공적으로 받으면 recv 함수는 메세지의 크기를 반환한다.
        int bytesReceived = recv(ClientSocket, buf, BUF_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }

        // Echo message back to client
        cout << buf << endl;
        send(ClientSocket, buf, bytesReceived + 1, 0);
    }

    //////////////////////////////////////////////////////////////////

    // 8. Close Client Socket And Exit WinSock
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}