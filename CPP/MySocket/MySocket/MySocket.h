#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32")

#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

#define IPv4 AF_INET
#define IPv6 AF_INET6


class MySocket
{
private:
	WSADATA m_wsaData;
	SOCKET m_sock;

	string m_ipAddr;
	int m_port;

public:
	MySocket();
	~MySocket();

	SOCKET createSock(int ipProtocol, int L4Protocol);
	void bind();
	void connect();
	void listen();
	void send();
};
