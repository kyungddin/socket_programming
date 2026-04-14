#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // To Use inet_pton
#include <string>
#pragma comment(lib, "ws2_32.lib") // Linking Stage
using namespace std;

int main()
{   
    // 0. Port And Address Define
    const char* serverIpAddr = "127.0.0.1";
	enum ePort { PORT = 54000 };

    // 1. WinSock 동적 라이브러리 활성화(초기화)
    // Runtime Stage Init
    // 리눅스는 네트워크 스택을 등록할 필요가 없음
    // 윈도우는 네트워크 스택을 사용할 떄 등록해줘야 함
    // C++은 이를 직접 WSADATA를 통해 해줘야 함
    // Python은 이를 라이브러리 import 과정에서 자동으로 처리
    WSADATA wsaData; 
    int iniResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // WinSock 내부 전역 상태 초기화
        
    if (iniResult != 0)
    {
        cerr << "Can't Init WinSock!" << endl;
        return -1;
    }

    // 2. Client Socket 생성
    // 명시적으로는 세 번째 인자에 TCP/UDP를 넣어줘야 하지만, 두 번째에서
    // 사실상 결정되므로 0(Auto)로 두는 편..
    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
        
    if (ClientSocket == INVALID_SOCKET)
    {
        cerr << "Can't Create Socket" << endl;
        WSACleanup();
        return -1;
    }

    // 3. IP and Port Setting
    /*
    struct sockaddr_in {
        short sin_family;      // AF_INET
        unsigned short sin_port;
        struct in_addr sin_addr;
        char sin_zero[8];
    };
    */
    sockaddr_in hint {}; // IPv4 전용 구조체 (sockaddr의 자식)
    hint.sin_family = AF_INET; // IPv4
    hint.sin_port = htons(PORT);    // PORT
    // char* IP를 바이트로 변환 후 hint.sin_addr에 저장 
    // 따라서 IP Addr 써야 함..! intet_pton은 DNS 불가
    // DNS는 쓸데없는 오버헤드니.. IP를 직접 사용하는 방식을 지향하자!
    int convResult = inet_pton(AF_INET, serverIpAddr, &hint.sin_addr); 
    
    if (convResult != 1)
    {
        cerr << "Can't Convert IP adderss" << endl;
        WSACleanup();
        return -1;
    }

    // 4. Conenct To Server
    int connResult = connect(ClientSocket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't Connect To Server!" << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return -1;
    }

    //////////////////// 5. Work Function(일 함수) ////////////////////

    enum eBufSize { BUF_SIZE = 4096 };
	char buf[BUF_SIZE];
	std::string userMsg;

	// 유저의 메세지 입력을 반복하여 처리
	do
	{
		std::cout << "> ";
		std::getline(std::cin, userMsg);

		if (userMsg.size() > 0)
		{
			// 입력한 메세지를 서버에 전송합니다.
			int sendResult = send(ClientSocket, userMsg.c_str(), userMsg.size() + 1, 0); // flag 0 = no option
			if (sendResult != SOCKET_ERROR)
			{
				// 서버로부터 전송된 메세지를 receive 합니다. (recv())
				ZeroMemory(buf, BUF_SIZE); // Windows API 매크로 (= memset(buf, 0, BUF_SIZE);)
				int bytesReceived = recv(ClientSocket, buf, BUF_SIZE, 0); // flag 0 = no option
				if (bytesReceived > 0)
				{
					// 서버로부터 받은 메세지를 출력합니다.
					std::cout << "SERVER> " << buf << std::endl;
				}
			}
		}

	} while (userMsg.size() > 0);

    //////////////////////////////////////////////////////////////////

    // 6. Close Socket And Exit
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}