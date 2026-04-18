#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> 
#include <string>
#pragma comment(lib, "ws2_32") 
using namespace std;

int main()
{   
    // 1. Init Socket System
    const char* serverIpAddr = "127.0.0.1";
    enum ePort { PORT = 54000 };

    WSADATA wsaData; 
    int iniResult = WSAStartup(MAKEWORD(2, 2), &wsaData);     
    if (iniResult != 0)
    {
        cerr << "Can't Init WinSock!" << endl;
        return -1;
    }

    // 2. Create Socket
    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSocket == INVALID_SOCKET)
    {
        cerr << "Can't Create Socket" << endl;
        WSACleanup();
        return -1;
    }

    // 3. Bind IP and Port
    sockaddr_in hint {}; // IPv4 전용 구조체 (sockaddr의 자식)
    hint.sin_family = AF_INET; // IPv4
    hint.sin_port = htons(PORT);    // PORT
    int convResult = inet_pton(AF_INET, serverIpAddr, &hint.sin_addr); 
    if (convResult != 1)
    {
        cerr << "Can't Convert IP adderss" << endl;
        WSACleanup();
        return -1;
    }

    // 4. Connet By TCP (Request 3-Way Handshake)
    int connResult = connect(ClientSocket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't Connect To Server!" << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return -1;
    }

    // 5. Main Communication Logic
    enum eBufSize { BUF_SIZE = 4096 };
	char buf[BUF_SIZE];
	std::string userMsg;
	do
	{
		std::cout << "> ";
		std::getline(std::cin, userMsg);

		if (userMsg.size() > 0)
		{
			
			int sendResult = send(ClientSocket, userMsg.c_str(), userMsg.size() + 1, 0); // flag 0 = no option
			if (sendResult != SOCKET_ERROR)
			{
				
				ZeroMemory(buf, BUF_SIZE); // Windows API 매크로 (= memset(buf, 0, BUF_SIZE);)
				int bytesReceived = recv(ClientSocket, buf, BUF_SIZE, 0); // flag 0 = no option
				if (bytesReceived > 0)
				{
					
					std::cout << "SERVER> " << buf << std::endl;
				}
			}
		}

	} while (userMsg.size() > 0);

    // 6. Close Socket And Exit
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}