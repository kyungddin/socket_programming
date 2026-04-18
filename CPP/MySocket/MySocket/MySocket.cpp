#include "MySocket.h"
#include <iostream>


/////////////////////////////////////////////////////////////////////////////////
// 1. Parent
/////////////////////////////////////////////////////////////////////////////////

MySocket::MySocket()
{
	m_port = 8080;	// Default Port: 8080
	m_ipAddr = "127.0.0.1";	// Default IP: localhost

	strcpy(m_buf, "\0");

	int iniResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iniResult != 0)
	{
		std::cerr << "Can't Initialize WinSock!" << std::endl;
		exit(1); // How To End?
	}

}


MySocket::~MySocket()
{
	closesocket(m_sock);
	WSACleanup();
}


void MySocket::configAddr(std::string ipAddr, int port)
{
	m_ipAddr = ipAddr;
	m_port = port;
}

/////////////////////////////////////////////////////////////////////////////////
// 2. Child I 
/////////////////////////////////////////////////////////////////////////////////

SOCKET UDPSocket::createSock()
{
	m_sock = socket(IPv4, UDP, 0);
	if (m_sock == INVALID_SOCKET)
	{
		std::cerr << "Can't Create Socket!" << std::endl;
		exit(1); // How To End?
	}
}


void UDPSocket::bind()
{
	memset(&m_serverAddr, 0, sizeof(m_serverAddr)); // Why?
	m_serverAddr.sin_family = IPv4;
	m_serverAddr.sin_addr.s_addr = inet_addr(m_ipAddr.c_str());
	m_serverAddr.sin_port = htons(m_port);
}


SOCKET TCPSocket::createSock()
{
	m_sock = socket(IPv4, TCP, 0);
	if (m_sock == INVALID_SOCKET)
	{
		std::cerr << "Can't Create Socket!" << std::endl;
		exit(1); // How To End?
	}
}


void TCPSocket::bind()
{
	m_hint.sin_family = IPv4;
	m_hint.sin_port = htons(m_port);
	int convResult = inet_pton(IPv4, m_ipAddr.c_str(), &m_hint.sin_addr);

	if (convResult != 1)
	{
		std::cerr << "Can't Convery IP Address" << std::endl;
		WSACleanup();
		exit(1); // How To End?
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 3. Child II
/////////////////////////////////////////////////////////////////////////////////

void UDPClient::send()
{
	sendto(m_sock, m_buf, MAX, 0, (struct sockaddr*)
		&m_serverAddr, sizeof(m_serverAddr)); // beta: datagram len is static
}
