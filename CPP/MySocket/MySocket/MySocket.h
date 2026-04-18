// To Do List
//1. 생성자 체인 Check
//2. memset 대체
//3. exit(1) 제거
//4. 소멸자 closesocket 조건문 추가
//5. createSocket return type null로 변경 및 예외처리 추가
//6. WSAStratup 1번만 호출
//7. UDPServer 생성자 체크

#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32")

#define MAX 2048

enum PROTOCOL {
	TCP = SOCK_STREAM,
	UDP = SOCK_DGRAM,
	IPv4 = AF_INET // Don't Care IPv6 (For Now Useless)
}; 

/////////////////////////////////////////////////////////////////////////////////
// 1. Basic Socket Class (Parent)
/////////////////////////////////////////////////////////////////////////////////

class MySocket // Polymorphic Class
{
private:
	WSADATA m_wsaData;

protected:
	SOCKET m_sock;

	std::string m_ipAddr;
	int m_port;

	char m_buf[MAX]; // Temp

public:
	MySocket();	
	MySocket(std::string ipAddr, int port);
	virtual ~MySocket(); // just virtual destructor

	virtual SOCKET createSock()=0; // For Now.. Pure Virtual, Care About Return..
};

/////////////////////////////////////////////////////////////////////////////////
// 2. TCP & UDP (Child I)
/////////////////////////////////////////////////////////////////////////////////

class UDPSocket : public MySocket // Polymorphic Class
{
protected:
	struct sockaddr_in m_sockAddr;

public:
	UDPSocket();
	virtual SOCKET createSock();
};

class TCPSocket : public MySocket // Polymorphic Class
{
private:
	SOCKADDR_IN m_hint;

public:
	TCPSocket();
	virtual SOCKET createSock();
};

/////////////////////////////////////////////////////////////////////////////////
// 3-1. **UDP** Client & Server (Child II)
/////////////////////////////////////////////////////////////////////////////////

class UDPClient : public UDPSocket
{
public:
	void send();
};

class UDPServer : public UDPSocket
{
private:
	struct sockaddr_in m_clientAddr; // Only For Server
	int c_len;
public:
	UDPServer();
	UDPServer(std::string ipAddr, int port); // How To Combine?
	void bindServer();
	void recv();
};

/////////////////////////////////////////////////////////////////////////////////
// 3-2. **TCP** Client & Server (Child II)
/////////////////////////////////////////////////////////////////////////////////

//class TCPClient : public TCPSocket
//{
//private:
//
//public:
//	TCPClient();
//	TCPClient(std::string ipAddr, int port);
//	~TCPClient();
//};
//
//class TCPServer : public TCPSocket
//{
//private:
//
//public:
//	TCPServer();
//	TCPServer(std::string ipAddr, int port);
//	~TCPServer();
//};
