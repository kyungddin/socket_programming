// GOAL: UDP Client -> UDP Server -> TCP Client -> TCP Server 순으로 구현하기

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

class MySocket
{
private:
	WSADATA m_wsaData;

protected:
	SOCKET m_sock;

	std::string m_ipAddr;
	int m_port;

	char m_buf[MAX];

public:
	MySocket();
	~MySocket();

	virtual SOCKET createSock()=0; // For Now.. Pure Virtual, Care About Return..
	virtual void bind()=0;
	void configAddr(std::string ipAddr, int port);
};

/////////////////////////////////////////////////////////////////////////////////
// 2. Divide Into TCP & UDP (Child I)
/////////////////////////////////////////////////////////////////////////////////

class UDPSocket : public MySocket
{
protected: // Temp Setup	
	struct sockaddr_in m_serverAddr;
	struct sockaddr_in m_clientAddr; 

public:
	virtual SOCKET createSock();
	virtual void bind();
};


class TCPSocket : public MySocket
{
private:
	SOCKADDR_IN m_hint;

public:
	virtual SOCKET createSock();
	virtual void bind();
};

/////////////////////////////////////////////////////////////////////////////////
// 3-1. Divide Into ""UDP"" Client & Server (Child II)
/////////////////////////////////////////////////////////////////////////////////

class UDPClient : public UDPSocket
{
private:

public:
	void send();
};


class UDPServer : public UDPSocket
{
private:

public:
};

/////////////////////////////////////////////////////////////////////////////////
// 3-2. Divide Into ""TCP"" Client & Server (Child II)
/////////////////////////////////////////////////////////////////////////////////

class TCPClient : public TCPSocket
{
private:

public:
};


class TCPServer : public TCPSocket
{
private:

public:
};
